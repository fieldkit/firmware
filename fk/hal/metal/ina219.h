#pragma once

#include "hal/board.h"
#include "hal/battery_gauge.h"
#include "platform.h"


namespace fk {

class Ina219 {
private:
    TwoWireWrapper *bus_;
    uint8_t address_;
    uint16_t config_;
    uint32_t calibration_value_;
    float ma_divider_;
    float power_multiplier_;

public:
    Ina219(TwoWireWrapper &bus, uint8_t address);

public:
    bool begin();
    Ina219Reading read() ;

};

}
