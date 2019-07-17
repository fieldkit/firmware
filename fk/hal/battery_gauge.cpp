#include "hal/hal.h"
#include "hal/metal/metal.h"
#include "hal/linux/linux.h"

namespace fk {

#if defined(FK_HARDWARE_FULL)
static MetalBatteryGauge gauge;
#else
static LinuxBatteryGauge gauge;
#endif

BatteryGauge *get_battery_gauge() {
    return &gauge;
}

}
