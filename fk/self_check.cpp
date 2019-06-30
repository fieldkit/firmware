#include "fk.h"
#include "self_check.h"

#include <Wire.h>

namespace fk {

SelfCheck::SelfCheck(Display *display, Wifi *wifi) : display_(display), wifi_(wifi) {
}

void SelfCheck::check() {
    display_->company_logo();

    // TODO: Check RTC
    // TODO: Check Battery gauge
    // TODO: Check QSPI memory
    // TODO: Check SPI memory
    // TODO: Check Temperature
    // TODO: Check WiFi
    // TODO: Check GPS

    // NOTE: If the display isn't working we can still create Wifi.
}

bool SelfCheck::rtc() {
    return true;
}

bool SelfCheck::temperature() {
    return true;
}

uint16_t read_u16(uint8_t address, uint8_t reg) {
    Wire1.beginTransmission(address);
    Wire1.write(reg);
    Wire1.endTransmission();
    Wire1.requestFrom(address, 2);
    uint16_t value  = Wire1.read();
    value |= (uint16_t)Wire1.read() << 8;
    return value;
}

bool write_u16(uint8_t address, uint8_t reg, uint16_t value) {
    Wire1.beginTransmission(address);
    Wire1.write(reg);
    Wire1.write( value       & 0xFF);
    Wire1.write((value >> 8) & 0xFF);
    return Wire1.endTransmission() == 0;
}

bool SelfCheck::battery_gauge() {
    Wire1.begin();

    for (auto i = 0; i < 0x128; ++i) {
        auto address = 0x36;

        if (!write_u16(address, 0x18, 2000 * 2)) {
            fkinfo("failed to write capacity");
            break;
        }

        auto raw_voltage = read_u16(address, 0x09);
        auto voltage = (float)raw_voltage * 7.8125e-5f;

        fkinfo("raw voltage=%d", raw_voltage);
        fkinfo("%f", voltage);

        break;
    }

    return true;
}

bool SelfCheck::qspi_memory() {
    return true;
}

bool SelfCheck::spi_memory() {
    return true;
}

bool SelfCheck::gps() {
    fkinfo("checking gps");

    auto working = false;

    uint32_t started = fk_uptime();
    while ((fk_uptime() - started) < 5000) {
        if (Serial1.available()) {
            Serial1.read();
            working = true;
        }
    }

    if (working) {
        fkinfo("gps found!");
    }
    else {
        fkinfo("no gps!");
    }

    return true;
}

bool SelfCheck::wifi() {
    return true;
}

}
