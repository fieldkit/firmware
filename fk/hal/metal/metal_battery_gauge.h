#pragma once

#include "hal/hal.h"

namespace fk {

class MetalBatteryGauge : public BatteryGauge {
private:
    bool available_;

public:
    MetalBatteryGauge();

public:
    bool begin() override;

    BatteryReading get() override;

};

}
