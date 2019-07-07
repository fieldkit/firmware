#include "hal/metal/metal.h"
#include "hal/metal/spi_flash.h"
#include "board.h"

#if defined(ARDUINO)

namespace fk {

#define loginfo(f, ...)  loginfof("memory", f, ##__VA_ARGS__)

#define logerror(f, ...) logerrorf("memory", f, ##__VA_ARGS__)

MetalDataMemory::MetalDataMemory() {
}

static void dump_memory(uint8_t *ptr, size_t size) {
    fkb_external_printf("\n");
    for (size_t i = 0; i < size; ++i) {
        fkb_external_printf("0x%02x ", ptr[i]);
        if (((i + 1) % 32) == 0) {
            fkb_external_printf("\n");
        }
    }
    fkb_external_printf("\n");
}

bool MetalDataMemory::begin() {
    SpiFlash banks[4]{
        SPI_FLASH_CS_BANK_1,
        SPI_FLASH_CS_BANK_2,
        SPI_FLASH_CS_BANK_3,
        SPI_FLASH_CS_BANK_4,
    };

    auto &spi = banks[0];

    FK_ASSERT(spi.begin());

    if (false) {
        uint32_t address = 0x0;
        uint8_t memory[2112] = { 0x00 };

        FK_ASSERT(spi.erase_block(address));

        FK_ASSERT(spi.read(address, memory, sizeof(memory)));
        dump_memory(memory, sizeof(memory));

        for (size_t i = 0; i < sizeof(memory); ++i) {
            memory[i] = i & 0xff;
        }
        FK_ASSERT(spi.write(address, memory, sizeof(memory)));

        FK_ASSERT(spi.read(address, memory, sizeof(memory)));
        dump_memory(memory, sizeof(memory));
    }

    {
        uint8_t memory[128] = { 0x00 };
        FK_ASSERT(spi.read(0, memory, sizeof(memory)));
        dump_memory(memory, sizeof(memory));
    }

    {
        uint8_t memory[128] = { 0x00 };
        FK_ASSERT(spi.read(128, memory, sizeof(memory)));
        dump_memory(memory, sizeof(memory));
    }

    {
        uint8_t memory[128] = { 0x00 };
        FK_ASSERT(spi.read(2112, memory, sizeof(memory)));
        dump_memory(memory, sizeof(memory));
    }

    return true;
}

}

#endif
