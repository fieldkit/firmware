#include "hal/linux/linux.h"

#if defined(linux)

#include <cstring>

namespace fk {

FK_DECLARE_LOGGER("memory");

uint8_t LinuxDataMemory::EraseByte = 0xff;

LinuxDataMemory::LinuxDataMemory() {
}

bool LinuxDataMemory::begin() {
    if (memory_ != nullptr) {
        free(memory_);
    }
    size_ = BlockSize * NumberOfBlocks;
    memory_ = (uint8_t *)malloc(size_);

    log_.logging(false);
    log_.clear();
    log_.append(LogEntry{ OperationType::Opened, 0x0, memory_ });

    return true;
}

flash_geometry_t LinuxDataMemory::geometry() const {
    return { PageSize, BlockSize, NumberOfBlocks, NumberOfBlocks * BlockSize };
}

size_t LinuxDataMemory::read(uint32_t address, uint8_t *data, size_t length) {
    assert(address >= 0 && address < size_);
    assert(address + length <= size_);
    assert(length <= PageSize);

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

static void verify_erased(uint32_t address, uint8_t *p, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        if (*p != LinuxDataMemory::EraseByte) {
            logerror("corruption: 0x%x", address);
            assert(*p == LinuxDataMemory::EraseByte);
        }
        p++;
    }
}

size_t LinuxDataMemory::write(uint32_t address, const uint8_t *data, size_t length) {
    assert(address >= 0 && address < size_);
    assert(address + length <= size_);
    assert(length <= PageSize);

    size_t page = address / PageSize;
    assert((address + length - 1) / PageSize == page);

    size_t block = address / BlockSize;
    size_t start_of_block = block * BlockSize;
    assert(address >= start_of_block && address + length <= start_of_block + BlockSize);

    auto p = memory_ + address;
    verify_erased(address, p, length);
    memcpy(p, data, length);

    log_.append(LogEntry{ OperationType::Write, address, p, length });

    return length;
}

size_t LinuxDataMemory::erase_block(uint32_t address) {
    assert(address >= 0 && address < size_);
    assert(address % BlockSize == 0);

    auto p = memory_ + address;
    memset(p, EraseByte, BlockSize);

    log_.append(LogEntry{ OperationType::EraseBlock, address, p });

    return true;
}

}

#endif
