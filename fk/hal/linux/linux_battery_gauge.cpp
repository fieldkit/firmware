#include "hal/linux/linux.h"

namespace fk {

LinuxBatteryGauge::LinuxBatteryGauge() {
}

bool LinuxBatteryGauge::begin() {
    return true;
}

PowerReading LinuxBatteryGauge::get() {
    return PowerReading{
        .available = true,
        .charging = 0,
        .battery = { .available = true, .bus_voltage = 3.6, .shunted_voltage = 0, .ma = 0, .mw = 0 },
        .solar = { .available = true, .bus_voltage = 3.6, .shunted_voltage = 0, .ma = 0, .mw = 0 },
    };
}

ChargingStatus LinuxBatteryGauge::status() {
    return ChargingStatus{};
}

bool LinuxBatteryGauge::battery_available() {
    return true;
}

bool LinuxBatteryGauge::solar_available() {
    return true;
}

} // namespace fk
