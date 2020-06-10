#pragma once

#include "hal/hal.h"

namespace fk {

class LinuxBatteryGauge : public BatteryGauge {
public:
    LinuxBatteryGauge();

public:
    bool begin() override;

    PowerReading get() override;

    ChargingStatus status() override;

    bool battery_available() override;

    bool solar_available() override;

};

}
