#pragma once

#include "common.h"
#include "state/power_state.h"

namespace fk {

class BatteryGauge {
public:
    virtual bool begin() = 0;

    virtual PowerReading get() = 0;

    virtual ChargingStatus status() = 0;

    virtual bool battery_available() = 0;

    virtual bool solar_available() = 0;
};

extern BatteryGauge *get_battery_gauge();

} // namespace fk
