#include "hal/linux/linux.h"
#include "utilities.h"

#if defined(linux)

#include <cstring>

namespace fk {

FK_DECLARE_LOGGER("memory");

uint8_t LinuxDataMemory::EraseByte = 0xff;

LinuxDataMemory::LinuxDataMemory() : memory_(nullptr) {
}

bool LinuxDataMemory::begin() {
    size_ = BlockSize * NumberOfBlocks;
    if (memory_ == nullptr) {
        memory_ = (uint8_t *)fk_malloc(size_);
        memset(memory_, 0xff, size_);
    }

    log_.logging(false);
    log_.clear();
    log_.append(LogEntry{ OperationType::Opened, 0x0, memory_ });

    return true;
}

void LinuxDataMemory::erase_all() {
    memset(memory_, 0xff, size_);
}

int32_t LinuxDataMemory::execute(uint32_t *got, uint32_t *entry) {
    FK_ASSERT(false);
    return 0;
}

FlashGeometry LinuxDataMemory::geometry() const {
    return { PageSize, BlockSize, NumberOfBlocks, NumberOfBlocks * BlockSize };
}

int32_t LinuxDataMemory::read(uint32_t address, uint8_t *data, size_t length, MemoryReadFlags flags) {
    assert(address >= 0 && address < size_);
    assert(address + length <= size_);
    assert(length <= PageSize);

    logverbose("[" PRADDRESS "] read %zd bytes", address, length);

    if (affects_bad_block_from_factory(address)) {
        bzero(data, length);
        return length;
    }

    size_t page = address / PageSize;
    assert((address + length - 1) / PageSize == page);

    size_t block = address / BlockSize;
    size_t start_of_block = block * BlockSize;
    assert(address >= start_of_block && address + length <= start_of_block + BlockSize);

    auto p = memory_ + address;
    memcpy(data, p, length);

    log_.append(LogEntry{ OperationType::Read, address, p, length });

    return length;
}

int32_t LinuxDataMemory::write(uint32_t address, const uint8_t *data, size_t length, MemoryWriteFlags flags) {
    assert(address >= 0 && address < size_);
    assert(address + length <= size_);
    assert(length <= PageSize);

    logverbose("[" PRADDRESS "] write %zd bytes", address, length);

    if (affects_bad_region(address, length)) {
        return 0;
    }

    size_t page = address / PageSize;
    assert((address + length - 1) / PageSize == page);

    size_t block = address / BlockSize;
    size_t start_of_block = block * BlockSize;
    assert(address >= start_of_block && address + length <= start_of_block + BlockSize);

    auto p = memory_ + address;
    memcpy(p, data, length);

    log_.append(LogEntry{ OperationType::Write, address, p, length });

    return length;
}

int32_t LinuxDataMemory::erase(uint32_t address, size_t length) {
    return for_each_block_between(address, length, BlockSize, [=](uint32_t block_address) {
        return erase_block(block_address);
    });
}

int32_t LinuxDataMemory::erase_block(uint32_t address) {
    assert(address >= 0 && address < size_);
    assert(address % BlockSize == 0);

    if (affects_bad_block_from_wear(address)) {
        return 0;
    }

    auto p = memory_ + address;
    memset(p, EraseByte, BlockSize);

    log_.append(LogEntry{ OperationType::EraseBlock, address, p });

    return true;
}

int32_t LinuxDataMemory::flush() {
    return true;
}

}

#endif
