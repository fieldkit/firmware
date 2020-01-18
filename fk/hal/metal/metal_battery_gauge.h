#pragma once

#include "hal/hal.h"

namespace fk {

class MetalBatteryGauge : public BatteryGauge {
private:
    Availability status_{ Availability::Unknown };
    uint16_t config_;
    uint32_t calibration_value_;
    float ma_divider_;
    float power_multiplier_;

public:
    MetalBatteryGauge();

public:
    bool begin() override;

    BatteryReading get() override;

    bool available() override {
        return status_ == Availability::Available;
    }

};

}
