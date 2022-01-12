#pragma once

#include "common.h"

namespace fk {

inline bool battery_status_is_low_power(BatteryStatus status) {
    return status == BatteryStatus::Dangerous || status == BatteryStatus::Low;
}

inline const char *battery_status_to_string(BatteryStatus status) {
    switch (status) {
    case BatteryStatus::Unknown:
        return "unknown";
    case BatteryStatus::Good:
        return "good";
    case BatteryStatus::External:
        return "external";
    case BatteryStatus::Low:
        return "low";
    case BatteryStatus::Dangerous:
        return "dangerous";
    default:
        return "unknown";
    }
}

struct MeterReading {
    bool available;
    float bus_voltage;
    float shunted_voltage;
    float ma;
    float mw;
};

struct PowerReading {
    bool available;
    bool charging;
    MeterReading battery;
    MeterReading solar;
};

struct ChargingStatus {
    uint32_t ticks;
    uint32_t blinks;
};

struct PowerState {
    bool low_battery{ false };
    MeterReading battery{};
    MeterReading solar{};
    BatteryStatus battery_status{ BatteryStatus::Unknown };
    bool allow_deep_sleep{ false };
    float charge{ 0 };
};

} // namespace fk
