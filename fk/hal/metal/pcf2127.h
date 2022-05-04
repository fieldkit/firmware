#pragma once

#include "common.h"
#include "hal/board.h"
#include "io.h"

#if defined(__SAMD51__)

namespace fk {

class Pcf2127 {
private:
    AcquireTwoWireBus *acquire_bus_{ nullptr };

public:
    explicit Pcf2127(AcquireTwoWireBus *acquire_bus);

public:
    bool begin();
    bool read_register(uint8_t reg, uint8_t &value);
    bool read_buffer(uint8_t reg, uint8_t *value, size_t size);
};

} // namespace fk

#endif
