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
    uint32_t last_tick_{ 0 };
    uint32_t ticks_{ 0 };
    uint32_t blinks_{ 0 };

public:
    MetalBatteryGauge();

public:
    bool begin() override;

    BatteryReading get() override;

    ChargingStatus status() override;

    bool available() override;

public:
    void irq();

};

}
