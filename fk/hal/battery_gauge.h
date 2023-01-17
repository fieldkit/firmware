#pragma once

#include "common.h"
#include "state/power_state.h"

namespace fk {

class BatteryGauge {
public:
    virtual bool begin() = 0;

    virtual PowerReading get() = 0;

    virtual ChargingStatus status() = 0;

    virtual bool expected() {
        return true;
    }

    virtual bool battery_available() = 0;

    virtual bool solar_available() = 0;
};

class NoopBatteryGauge : public BatteryGauge {
public:
    virtual bool begin() {
        return false;
    }

    virtual PowerReading get() {
        return PowerReading{};
    }

    virtual ChargingStatus status() {
        return ChargingStatus{};
    }

    virtual bool expected() {
        return false;
    }

    virtual bool battery_available() {
        return false;
    }

    virtual bool solar_available() {
        return false;
    }
};

extern BatteryGauge *get_battery_gauge();

} // namespace fk
