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
    auto power = get_battery_gauge()->get();
    auto gs = get_global_state_rw();

    if (power.available) {
        // Bradley gave me this.
        auto charge = (power.battery.bus_voltage - 3.5f) * 142.85f;
        if (charge < 0.0f) charge = 0.0f;
        if (charge > 100.0f) charge = 100.0f;

        gs.get()->power.charge = charge;
        gs.get()->power.voltage = power.battery.bus_voltage;
        gs.get()->power.vbus = power.battery.bus_voltage;
        gs.get()->power.vs = power.battery.shunted_voltage;
        gs.get()->power.ma = power.battery.ma;
        gs.get()->power.mw = power.battery.mw;

        loginfo("battery: v_bus = %fV v_s = %fmV %fmA %fmW %f%% (%s)", power.battery.bus_voltage,
                power.battery.shunted_voltage, power.battery.ma, power.battery.mw, charge,
                power.charging ? " charging" : "");
    }
    else {
        logerror("battery: status unavilable");
    }

    if (power.solar.available) {
        loginfo("solar: v_bus = %fV v_s = %fmV %fmA %fmW", power.solar.bus_voltage, power.solar.shunted_voltage,
                power.solar.ma, power.solar.mw);
    }
    else {
        loginfo("solar: status unavailable");
    }
}

} // namespace fk
