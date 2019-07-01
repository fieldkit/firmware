#pragma once

#include "common.h"

#include <Wire.h>

namespace fk {

class BatteryGauge {
private:
    TwoWire *wire_;

public:
    BatteryGauge(TwoWire &wire);

public:
    /**
     *
     */
    bool begin();

    /**
     *
     */
    void check();

};

}
