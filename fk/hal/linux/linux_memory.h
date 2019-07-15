#pragma once

#if defined(linux)

#include "hal/memory.h"
#include "hal/linux/debug_log.h"

namespace fk {

class LinuxDataMemory : public DataMemory {
private:
    /* These mimic a single bank of memory. */
    constexpr static uint32_t PageSize = 2112;
    constexpr static uint32_t BlockSize = 2112 * 64;
    constexpr static uint32_t NumberOfBlocks = 2048;

private:
    StorageLog log_;
    flash_geometry_t geometry_;
    uint8_t *memory_{ nullptr };
    size_t size_{ 0 };

public:
    static uint8_t EraseByte;

public:
    LinuxDataMemory();

public:
    bool begin() override;

    flash_geometry_t geometry() const override;

    size_t read(uint32_t address, uint8_t *data, size_t length) override;

    size_t write(uint32_t address, const uint8_t *data, size_t length) override;

    size_t erase_block(uint32_t address) override;

public:
    StorageLog &log() {
        return log_;
    }

};

}

#endif
