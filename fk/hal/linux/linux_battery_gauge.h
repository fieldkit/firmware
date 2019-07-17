#pragma once

#include "hal/hal.h"

namespace fk {

class LinuxBatteryGauge : public BatteryGauge {
public:
    LinuxBatteryGauge();

public:
    bool begin() override;

    BatteryReading get() override;

};

}
