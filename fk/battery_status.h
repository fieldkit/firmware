#pragma once

#include "state.h"

namespace fk {

class NoopBatteryChecker {
public:
    bool available() const {
        return false;
    }

    bool low_power_dangerous() const {
        return false;
    }

    bool low_power() const {
        return false;
    }

public:
    void refresh(bool initialize = false) {
    }
};

class BatteryChecker {
public:
    bool have_charge_{ false };
    BatteryStatus battery_status_{ BatteryStatus::Unknown };
    float charge_{ 0.0f };

public:
    bool available() const {
        return battery_status_ != BatteryStatus::Unknown;
    }

    bool low_power_dangerous() const {
        return battery_status_ == BatteryStatus::Dangerous;
    }

    bool low_power() const {
        return battery_status_is_low_power(battery_status_);
    }

public:
    void refresh(bool initialize = false);

public:
    static float voltage_to_percentange(float voltage);
};

} // namespace fk
