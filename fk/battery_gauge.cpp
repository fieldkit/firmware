#include "battery_gauge.h"

#include <Wire.h>

namespace fk {

FK_DECLARE_LOGGER("battery");

/*
static uint16_t read_u16(uint8_t address, uint8_t reg) {
    Wire1.beginTransmission(address);
    Wire1.write(reg);
    Wire1.endTransmission();
    Wire1.requestFrom(address, 2);
    uint16_t value  = Wire1.read();
    value |= (uint16_t)Wire1.read() << 8;
    return value;
}

static bool write_u16(uint8_t address, uint8_t reg, uint16_t value) {
    Wire1.beginTransmission(address);
    Wire1.write(reg);
    Wire1.write( value       & 0xFF);
    Wire1.write((value >> 8) & 0xFF);
    return Wire1.endTransmission() == 0;
}
*/

BatteryGauge::BatteryGauge(TwoWire &wire) : wire_(&wire) {
}

bool BatteryGauge::begin() {
    Wire1.begin();

    auto address = 0x36;

    Wire1.beginTransmission(address);
    Wire1.write(0x00);
    if (!I2C_CHECK(Wire1.endTransmission())) {
        return false;
    }

    Wire1.requestFrom(address, 1);
    auto value  = Wire1.read();
    if (!I2C_CHECK(Wire1.endTransmission())) {
        return false;
    }

    loginfo("gauge version: %d", value);

    return true;
}

void BatteryGauge::check() {

}

}
