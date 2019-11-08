#include "hal/hal.h"
#include "hal/metal/metal.h"

#if defined(__SAMD51__)

namespace fk {

MetalDataMemory::MetalDataMemory(uint8_t cs_pin) : flash_{ cs_pin } {
}

bool MetalDataMemory::begin() {
    return flash_.begin();
}

FlashGeometry MetalDataMemory::geometry() const {
    return flash_.geometry();
}

int32_t MetalDataMemory::read(uint32_t address, uint8_t *data, size_t length) {
    return flash_.read(address, data, length);
}

int32_t MetalDataMemory::write(uint32_t address, const uint8_t *data, size_t length) {
    return flash_.write(address, data, length);
}

int32_t MetalDataMemory::erase_block(uint32_t address) {
    return flash_.erase_block(address);
}

int32_t MetalDataMemory::flush() {
    return true;
}

}

#endif
