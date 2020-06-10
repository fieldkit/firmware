#include "hal/metal/metal_battery_gauge.h"
#include "hal/metal/ina219.h"
#include "platform.h"

#include <Wire.h>

namespace fk {

FK_DECLARE_LOGGER("power");

constexpr uint8_t PIN_BATTERY_CHARGING = 98u;

constexpr uint8_t INA219_ADDRESS_BATTERY = 0x40;
constexpr uint8_t INA219_ADDRESS_SOLAR = 0x41;

static void irq_charge_pulse() {
    reinterpret_cast<MetalBatteryGauge*>(get_battery_gauge())->irq();
}

MetalBatteryGauge::MetalBatteryGauge() {
}

bool MetalBatteryGauge::begin() {
    status_ = Availability::Unavailable;

    pinMode(PIN_BATTERY_CHARGING, INPUT);
    attachInterrupt(digitalPinToInterrupt(PIN_BATTERY_CHARGING), irq_charge_pulse, FALLING);

    auto bus = get_board()->i2c_module();

    Ina219 battery_monitor{ bus, INA219_ADDRESS_BATTERY };
    if (!battery_monitor.begin()) {
        loginfo("battery: ina219 missing");
        return false;
    }

    Ina219 solar_monitor{ bus, INA219_ADDRESS_SOLAR };
    if (!solar_monitor.begin()) {
        loginfo("solar: ina219 missing");
    }

    loginfo("initialized");

    status_ = Availability::Available;

    return true;
}

BatteryReading MetalBatteryGauge::get() {
    if (status_ != Availability::Available) {
        return BatteryReading{ .available = false };
    }

    auto charging = digitalRead(PIN_BATTERY_CHARGING) == 0;
    auto bus = get_board()->i2c_module();

    Ina219 battery_monitor{ bus, INA219_ADDRESS_BATTERY };
    auto battery = battery_monitor.read();
    if (!battery.available) {
        return BatteryReading{ .available = false };
    }

    Ina219 solar_monitor{ bus, INA219_ADDRESS_SOLAR };
    auto solar = solar_monitor.read();

    return BatteryReading{
        .available = true,
        .charging = charging,
        .battery = battery,
        .solar = solar,
    };
}

ChargingStatus MetalBatteryGauge::status() {
    auto status = ChargingStatus{
        .ticks = ticks_,
        .blinks = blinks_,
    };

    ticks_ = 0;
    blinks_ = 0;

    return status;
}

bool MetalBatteryGauge::available() {
    return status_ == Availability::Available;
}

void MetalBatteryGauge::irq() {
    if (last_tick_ == 0 || (fk_uptime() - last_tick_ > 100)) {
        blinks_++;
    }

    last_tick_ = fk_uptime();

    ticks_++;
}

} // namespace fk
