#include "hal/hal.h"
#include "hal/metal/metal.h"

#if defined(__SAMD51__)

namespace fk {

MetalQspiMemory::MetalQspiMemory() : transport_{ PIN_QSPI_SCK, QSPI_FLASH_CS, PIN_QSPI_IO0, PIN_QSPI_IO1, PIN_QSPI_IO2, PIN_QSPI_IO3 }, flash_{ &transport_ } {
}

bool MetalQspiMemory::begin() {
    pinMode(QSPI_FLASH_CS, OUTPUT);
    digitalWrite(QSPI_FLASH_CS, LOW);

    if (!flash_.begin()){
        return false;
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
