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
    return true;
}

flash_geometry_t LinuxDataMemory::geometry() const {
    return { PageSize, BlockSize, NumberOfBlocks, NumberOfBlocks * BlockSize };
}

bool LinuxDataMemory::read(uint32_t address, uint8_t *data, uint32_t length) {
    assert(address >= 0 && address < size_);
    assert(address + length <= size_);
    assert(length <= PageSize);

    uint32_t page = address / PageSize;
    assert((address + length - 1) / PageSize == page);

    uint32_t block = address / BlockSize;
    uint32_t start_of_block = block * BlockSize;
    assert(address >= start_of_block && address + length <= start_of_block + BlockSize);

    auto p = memory_ + address;
    memcpy(data, p, length);

    return true;
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

bool LinuxDataMemory::write(uint32_t address, const uint8_t *data, uint32_t length) {
    assert(address >= 0 && address < size_);
    assert(address + length <= size_);
    assert(length <= PageSize);

    uint32_t page = address / PageSize;
    assert((address + length - 1) / PageSize == page);

    uint32_t block = address / BlockSize;
    uint32_t start_of_block = block * BlockSize;
    assert(address >= start_of_block && address + length <= start_of_block + BlockSize);

    auto p = memory_ + address;
    verify_erased(address, p, length);
    memcpy(p, data, length);

    return true;
}

bool LinuxDataMemory::erase_block(uint32_t address) {
    assert(address >= 0 && address < size_);
    assert(address % BlockSize == 0);

    memset(memory_ + address, EraseByte, BlockSize);

    return true;
}

}

#endif
