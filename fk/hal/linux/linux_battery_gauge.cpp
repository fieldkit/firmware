#include "hal/linux/linux.h"

namespace fk {

LinuxBatteryGauge::LinuxBatteryGauge() {
}

bool LinuxBatteryGauge::begin() {
    return true;
}

BatteryReading LinuxBatteryGauge::get() {
    return {
    };
}

ChargingStatus LinuxBatteryGauge::status() {
    return {
    };
}

bool LinuxBatteryGauge::available() {
    return true;
}

} // namespace fk
