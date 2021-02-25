#include "hal/hal.h"
#include "hal/metal/metal.h"

#if defined(__SAMD51__)

namespace fk {

MetalQspiMemory::MetalQspiMemory() : transport_{ PIN_QSPI_SCK, QSPI_FLASH_CS, PIN_QSPI_IO0, PIN_QSPI_IO1, PIN_QSPI_IO2, PIN_QSPI_IO3 }, flash_{ &transport_ } {
}

bool MetalQspiMemory::begin() {
    pinMode(QSPI_FLASH_CS, OUTPUT);
    digitalWrite(QSPI_FLASH_CS, LOW);

    if (!flash_.begin()) {
        return false;
    }

    alogf(LogLevels::INFO, "qspi", "qspi jedec id: 0x%" PRIx32, flash_.getJEDECID());
    alogf(LogLevels::INFO, "qspi", "qspi size: 0x%" PRIx32 " pages: %" PRIu32 " page-size: %" PRIu32, flash_.size(), (uint32_t)flash_.numPages(), (uint32_t)flash_.pageSize());

    if (false) {
        StandardPool pool{ "qspi" };
        auto buffer = (uint8_t *)pool.malloc(2048);

        alogf(LogLevels::INFO, "qspi", "reading");
        auto nread = flash_.readBuffer(0, buffer, 256);
        fk_dump_memory("qspi ", buffer, nread);
        alogf(LogLevels::INFO, "qspi", "read: 0x%" PRIx32, nread);

        for (auto i = 0u; i < 256; ++i) {
            buffer[i] = i;
        }

        alogf(LogLevels::INFO, "qspi", "writing");
        auto nwrote = flash_.writeBuffer(0, buffer, 256);
        alogf(LogLevels::INFO, "qspi", "wrote: 0x%" PRIx32, nwrote);

        while (true) {
            fk_delay(1000);
        }
    }

    return true;
}

FlashGeometry MetalQspiMemory::geometry() const {
    return  { };
}

int32_t MetalQspiMemory::read(uint32_t address, uint8_t *data, size_t length, MemoryReadFlags flags) {
    return false;
}

int32_t MetalQspiMemory::write(uint32_t address, const uint8_t *data, size_t length, MemoryWriteFlags flags) {
    return false;
}

int32_t MetalQspiMemory::erase_block(uint32_t address) {
    return false;
}

int32_t MetalQspiMemory::flush() {
    return true;
}

}

#endif
