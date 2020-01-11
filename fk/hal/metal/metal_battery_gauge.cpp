#include "hal/metal/metal_battery_gauge.h"
#include "platform.h"

#include <Wire.h>

namespace fk {

FK_DECLARE_LOGGER("battery");

MetalBatteryGauge::MetalBatteryGauge() {
}

bool MetalBatteryGauge::begin() {
    return true;
}

BatteryReading MetalBatteryGauge::get() {
    return {
        .available = false,
        .cellv = 0,
        .soc = 0,
        .temp = 0,
        .tte = 0.0f,
        .ttf = 0.0f,
    };
}


}
