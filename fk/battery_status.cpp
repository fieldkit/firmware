#include "battery_status.h"
#include "hal/battery_gauge.h"
#include "state_ref.h"

namespace fk {

FK_DECLARE_LOGGER("battery");

void BatteryStatus::refresh() {
    if (!get_battery_gauge()->available()) {
        return;
    }

    auto lock = get_modmux()->lock();

    auto battery = get_battery_gauge()->get();
    if (!battery.available) {
        logerror("battery status unavilable");
        return;
    }

    // Bradley gave me this.
    auto charge = (battery.bus_voltage - 3.5f) * 142.85f;
    if (charge < 0.0f) charge = 0.0f;
    if (charge > 100.0f) charge = 100.0f;

    auto gs = get_global_state_rw();
    gs.get()->power.voltage = battery.bus_voltage;
    gs.get()->power.charge = charge;
    gs.get()->power.vbus = battery.bus_voltage;
    gs.get()->power.vs = battery.shunted_voltage;
    gs.get()->power.ma = battery.ma;
    gs.get()->power.mw = battery.mw;

    loginfo("battery:%s v_bus = %fV v_s = %fmV %fmA %fmW %f%%", battery.charging ? " charging": "", battery.bus_voltage, battery.shunted_voltage, battery.ma, battery.mw, charge);
}

}
