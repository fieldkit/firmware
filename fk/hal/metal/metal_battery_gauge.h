#pragma once

#include "hal/hal.h"

namespace fk {

class MetalBatteryGauge : public BatteryGauge {
private:
    Availability battery_{ Availability::Unknown };
    Availability solar_{ Availability::Unknown };
    uint32_t last_tick_{ 0 };
    uint32_t ticks_{ 0 };
    uint32_t blinks_{ 0 };

public:
    MetalBatteryGauge();

public:
    bool begin() override;

    PowerReading get() override;

    ChargingStatus status() override;

    bool battery_available() override;

    bool solar_available() override;

public:
    void irq();

};

}
