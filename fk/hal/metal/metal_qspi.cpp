#include "hal/hal.h"
#include "hal/metal/metal.h"

#include "modules/dyn/dyn.h"

#if defined(__SAMD51__)

namespace fk {

FK_DECLARE_LOGGER("qspi");

MetalQspiMemory::MetalQspiMemory() : transport_{ PIN_QSPI_SCK, QSPI_FLASH_CS, PIN_QSPI_IO0, PIN_QSPI_IO1, PIN_QSPI_IO2, PIN_QSPI_IO3 }, flash_{ &transport_ } {
}

bool MetalQspiMemory::begin() {
    pinMode(QSPI_FLASH_CS, OUTPUT);
    digitalWrite(QSPI_FLASH_CS, LOW);

    if (!flash_.begin()) {
        return false;
    }

    loginfo("qspi jedec id: 0x%" PRIx32, flash_.getJEDECID());
    loginfo("qspi size: 0x%" PRIx32 " pages: %" PRIu32 " page-size: %" PRIu32,
            flash_.size(), (uint32_t)flash_.numPages(), (uint32_t)flash_.pageSize());

    // Teach the peripheral how to do reads so we can execute in place.
    flash_.readBuffer(0, nullptr, 0);

    return true;
}

int32_t MetalQspiMemory::execute(uint32_t *got, uint32_t *entry) {
    // Teach the peripheral how to do reads so we can execute in place.
    flash_.readBuffer(0, nullptr, 0);

    // Dive into the code.
    fk_dyn_run(got, entry);

    return 0;
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
