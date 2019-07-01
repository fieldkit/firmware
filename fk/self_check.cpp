#include "fk.h"
#include "self_check.h"
#include "clock.h"
#include "battery_gauge.h"
#include "temperature.h"

#include <Wire.h>
#include <Adafruit_QSPI.h>
#include <Adafruit_QSPI_Flash.h>
#include <SerialFlash.h>

namespace fk {

#define loginfo(f, ...)  loginfof("check", f, ##__VA_ARGS__)

#define logerror(f, ...) logerrorf("check", f, ##__VA_ARGS__)

SelfCheck::SelfCheck(Display *display, Wifi *wifi) : display_(display), wifi_(wifi) {
}

void SelfCheck::check() {
    loginfo("starting");

    // TODO: Check this failure scenario.
    display_->company_logo();

    rtc();
    battery_gauge();
    temperature();
    qspi_memory();
    spi_memory();
    gps();
    wifi();

    loginfo("done");
}

bool SelfCheck::rtc() {
    CoreClock clock{ Wire };

    if (!clock.begin()) {
        loginfo("rtc... ERROR");
        return false;
    }

    loginfo("rtc... OK");
    return true;
}

bool SelfCheck::temperature() {
    CoreTemperature temp{ Wire };

    if (!temp.begin()) {
        loginfo("temperature... ERROR");
        return false;
    }

    loginfo("temperature... OK");
    return true;
}

bool SelfCheck::battery_gauge() {
    BatteryGauge gauge{ Wire1 };

    if (!gauge.begin()) {
        loginfo("battery gauge... ERROR");
        return false;
    }

    loginfo("battery gauge... OK");
    return true;
}

bool SelfCheck::qspi_memory() {
    Adafruit_QSPI_Flash qspi_flash;

    pinMode(QSPI_FLASH_CS, OUTPUT);
    digitalWrite(QSPI_FLASH_CS, LOW);

    if (!qspi_flash.begin()){
        loginfo("qspi memory... ERROR");
        return false;
    }

    loginfo("qspi memory... OK");
    return true;
}

bool SelfCheck::spi_memory() {
    uint8_t cs_pins[] = {
        SPI_FLASH_CS_BANK_1,
        SPI_FLASH_CS_BANK_2,
        SPI_FLASH_CS_BANK_3,
        SPI_FLASH_CS_BANK_4,
    };

    for (auto pin : cs_pins) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
    }

    SPI.begin();

    auto nbanks = 0;

    for (auto pin : cs_pins) {
        SerialFlashChip flash;

        if (flash.begin(pin)) {
            loginfo("spi memory (%d)... OK", pin);
            nbanks++;
        }
    }

    return nbanks > 0;
}

bool SelfCheck::gps() {
    Serial1.begin(9600);

    board.enable_gps();

    auto received = 0;
    auto started = fk_uptime();
    while ((fk_uptime() - started) < 5000) {
        if (Serial1.available()) {
            Serial1.read();
            received++;
            if (received == 10) {
                break;
            }
        }
    }

    if (received < 10) {
        loginfo("gps... ERROR");
        return false;
    }

    loginfo("gps... OK");
    return true;
}

bool SelfCheck::wifi() {
    board.enable_wifi();

    loginfo("wifi... OK");

    return true;
}

}
