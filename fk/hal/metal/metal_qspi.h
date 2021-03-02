#pragma once

#if defined(__SAMD51__)

#include "hal/memory.h"
#include "hal/metal/metal_memory.h"

#include <Adafruit_SPIFlash.h>

namespace fk {

class MetalQspiMemory : public ExecutableMemory {
public:
    constexpr static uint32_t PageSize = 256;
    constexpr static uint32_t SectorSize = 4096;
    constexpr static uint32_t BlockSize = 64 * 1024;
    constexpr static uint32_t NumberOfBlocks = 128;

private:
    Availability status_{ Availability::Unknown };
    Adafruit_FlashTransport_QSPI transport_;
    Adafruit_SPIFlash flash_;

public:
    MetalQspiMemory();

public:
    bool begin() override;

    FlashGeometry geometry() const override;

    int32_t read(uint32_t address, uint8_t *data, size_t length, MemoryReadFlags flags) override;

    int32_t write(uint32_t address, const uint8_t *data, size_t length, MemoryWriteFlags flags) override;

    int32_t erase(uint32_t address, size_t length);

    int32_t flush() override;

public:
    int32_t execute(uint32_t *got, uint32_t *entry) override;

};

}

#endif
