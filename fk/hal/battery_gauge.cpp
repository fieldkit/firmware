#include "hal/hal.h"
#include "hal/metal/metal.h"
#include "hal/linux/linux.h"

namespace fk {

#if defined(FK_HARDWARE_FULL)
#if defined(FK_UNDERWATER)
static NoopBatteryGauge gauge;
#else
static MetalBatteryGauge gauge;
#endif
#else
static LinuxBatteryGauge gauge;
#endif

BatteryGauge *get_battery_gauge() {
    return &gauge;
}

} // namespace fk
