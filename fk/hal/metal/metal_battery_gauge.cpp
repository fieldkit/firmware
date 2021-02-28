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
    battery_ = Availability::Unavailable;
    solar_ = Availability::Unavailable;

    pinMode(PIN_BATTERY_CHARGING, INPUT);
    attachInterrupt(digitalPinToInterrupt(PIN_BATTERY_CHARGING), irq_charge_pulse, FALLING);

    auto bus = get_board()->i2c_module();

    Ina219 battery_monitor{ bus, INA219_ADDRESS_BATTERY };
    if (!battery_monitor.begin(false)) {
        loginfo("battery: ina219 missing");
    }
    else {
        battery_ = Availability::Available;
        loginfo("battery: ina219 found");
    }

    Ina219 solar_monitor{ bus, INA219_ADDRESS_SOLAR };
    if (!solar_monitor.begin(false)) {
        loginfo("solar: ina219 missing");
    }
    else {
        solar_ = Availability::Available;
        loginfo("solar: ina219 found");
    }

    return battery_ == Availability::Available;
}

PowerReading MetalBatteryGauge::get() {
    auto charging = digitalRead(PIN_BATTERY_CHARGING) == 0;
    auto bus = get_board()->i2c_module();

    auto reading = PowerReading{
        .available = true,
        .charging = charging,
        .battery = { },
        .solar = { },
    };

    if (battery_ == Availability::Available) {
        Ina219 battery_monitor{ bus, INA219_ADDRESS_BATTERY };
        if (battery_monitor.begin(true)) {
            reading.battery = battery_monitor.read();
        }
        if (!battery_monitor.begin(false)) {
            logwarn("error powering down ina219");
        }
    }

    if (solar_ == Availability::Available) {
        Ina219 solar_monitor{ bus, INA219_ADDRESS_SOLAR };
        if (solar_monitor.begin(true)) {
            reading.solar = solar_monitor.read();
        }
        if (!solar_monitor.begin(false)) {
            logwarn("error powering down ina219");
        }
    }

    return reading;
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

bool MetalBatteryGauge::battery_available() {
    return battery_ == Availability::Available;
}

bool MetalBatteryGauge::solar_available() {
    return solar_ == Availability::Available;
}

void MetalBatteryGauge::irq() {
    if (last_tick_ == 0 || (fk_uptime() - last_tick_ > 100)) {
        blinks_++;
    }

    last_tick_ = fk_uptime();

    ticks_++;
}

} // namespace fk
