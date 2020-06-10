#include "hal/linux/linux.h"

namespace fk {

LinuxBatteryGauge::LinuxBatteryGauge() {
}

bool LinuxBatteryGauge::begin() {
    return true;
}

PowerReading LinuxBatteryGauge::get() {
    return PowerReading{ };
}

ChargingStatus LinuxBatteryGauge::status() {
    return ChargingStatus{ };
}

bool LinuxBatteryGauge::battery_available() {
    return true;
}

bool LinuxBatteryGauge::solar_available() {
    return true;
}

} // namespace fk
