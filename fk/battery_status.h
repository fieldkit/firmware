#pragma once

namespace fk {

class BatteryStatus {
public:
    bool have_charge_{ false };
    bool low_battery_{ false };
    float charge_{ 0.0f };

public:
    bool have_charge() const {
        return have_charge_;
    }

    bool low_battery() const {
        return low_battery_;
    }

public:
    void refresh();

public:
    static float voltage_to_percentange(float voltage);

};

} // namespace fk
