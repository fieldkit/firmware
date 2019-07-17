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

}
