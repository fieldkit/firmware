#pragma once

#include "common.h"
#include "hal/board.h"

namespace fk {

class CoreTemperature {
private:
    constexpr static uint8_t Address = 0x48u;

private:
    TwoWireWrapper wire_;

public:
    CoreTemperature(TwoWireWrapper wire);

public:
    bool begin();

    bool read(float *temperature);

};

}
