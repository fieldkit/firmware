#pragma once

#include "common.h"

#include <Wire.h>

namespace fk {

class CoreTemperature {
private:
    constexpr static uint8_t Address = 0x48u;

private:
    TwoWire *wire_;

public:
    CoreTemperature(TwoWire &wire);

public:
    bool begin();

    bool read(float *temperature);

};

}
