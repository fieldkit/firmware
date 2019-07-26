#include "hal/hal.h"
#include "hal/metal/metal.h"

#if defined(ARDUINO)

namespace fk {

MetalDataMemory::MetalDataMemory(uint8_t cs_pin) : flash_{ cs_pin } {
}

bool MetalDataMemory::begin() {
    return flash_.begin();
}

flash_geometry_t MetalDataMemory::geometry() const {
    return flash_.geometry();
}

size_t MetalDataMemory::read(uint32_t address, uint8_t *data, size_t length) {
    return flash_.read(address, data, length);
}

size_t MetalDataMemory::write(uint32_t address, const uint8_t *data, size_t length) {
    return flash_.write(address, data, length);
}

size_t MetalDataMemory::erase_block(uint32_t address) {
    return flash_.erase_block(address);
}

size_t MetalDataMemory::flush() {
    return flash_.flush();
}

}

#endif
