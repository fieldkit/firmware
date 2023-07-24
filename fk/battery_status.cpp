#include "battery_status.h"
#include "hal/battery_gauge.h"
#include "state_ref.h"

namespace fk {

FK_DECLARE_LOGGER("battery");

constexpr float MinimumPlausibleBatteryVoltage = 0.3f;
constexpr float LowBatteryDangerousVoltage = 3.5f;
constexpr float LowBatteryVoltage = 3.7f;

void BatteryChecker::refresh(bool initialize) {
    auto gauge = get_battery_gauge();
    if (!gauge->expected()) {
        return;
    }

    logdebug("taking battery reading");

    if (initialize) {
        if (!gauge->begin()) {
            logerror("battery gauge initialize");
        } else {
            loginfo("gauge ready");
        }
    }

    auto power = gauge->get();

    auto charge = voltage_to_percentange(power.battery.bus_voltage);
    if (power.battery.available) {
        charge_ = charge;
        have_charge_ = true;

        if (power.battery.bus_voltage > MinimumPlausibleBatteryVoltage) {
            FK_ASSERT(LowBatteryDangerousVoltage < LowBatteryVoltage);
            if (power.battery.bus_voltage < LowBatteryDangerousVoltage) {
                battery_status_ = BatteryStatus::Dangerous;
            } else if (power.battery.bus_voltage < LowBatteryVoltage) {
                battery_status_ = BatteryStatus::Low;
            } else {
                battery_status_ = BatteryStatus::Good;
            }
        } else {
            battery_status_ = BatteryStatus::External;
        }

        loginfo("battery: v_bus=%fV v_s=%fmV %fmA %fmW %f%% %s %s", power.battery.bus_voltage, power.battery.shunted_voltage,
                power.battery.ma, power.battery.mw, charge, battery_status_to_string(battery_status_), power.charging ? "(charging)" : "");
    } else {
        logerror("battery: status unavilable, battery dangerously low");
        battery_status_ = BatteryStatus::Unknown;
    }

    if (power.solar.available) {
        loginfo("solar: v_bus=%fV v_s=%fmV %fmA %fmW", power.solar.bus_voltage, power.solar.shunted_voltage, power.solar.ma,
                power.solar.mw);
    } else {
        loginfo("solar: status unavailable");
    }

    auto gs = get_global_state_rw();
    gs.get()->power.battery = power.battery;
    gs.get()->power.solar = power.solar;
    gs.get()->power.battery_trend = gs.get()->power.battery_trend.update(power.battery);
    gs.get()->power.solar_trend = gs.get()->power.solar_trend.update(power.solar);
    gs.get()->power.charge = charge;
    gs.get()->power.battery_status = battery_status_;
}

float BatteryChecker::voltage_to_percentange(float voltage) {
    // Bradley gave me this.
    auto charge = (voltage - 3.5f) * 142.85f;
    if (charge < 0.0f)
        charge = 0.0f;
    if (charge > 100.0f)
        charge = 100.0f;
    return charge;
}

} // namespace fk
