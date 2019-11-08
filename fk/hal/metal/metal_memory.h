#pragma once

#if defined(__SAMD51__)

#include "hal/memory.h"
#include "hal/metal/metal_memory.h"
#include "hal/metal/spi_flash.h"

namespace fk {

class MetalDataMemory : public DataMemory {
private:
    SpiFlash flash_;

public:
    MetalDataMemory(uint8_t cs_pin);

public:
    bool begin() override;

    flash_geometry_t geometry() const override;

    int32_t read(uint32_t address, uint8_t *data, size_t length) override;

    int32_t write(uint32_t address, const uint8_t *data, size_t length) override;

    int32_t erase_block(uint32_t address) override;

    int32_t flush() override;
};

}

#endif
