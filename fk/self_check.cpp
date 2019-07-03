#include "fk.h"
#include "self_check.h"
#include "clock.h"
#include "battery_gauge.h"
#include "temperature.h"
#include "hal/metal/metal.h"

#include <Wire.h>
#include <Adafruit_QSPI.h>
#include <Adafruit_QSPI_Flash.h>
#include <SerialFlash.h>

namespace fk {

#define loginfo(f, ...)  loginfof("check", f, ##__VA_ARGS__)

#define logerror(f, ...) logerrorf("check", f, ##__VA_ARGS__)

SelfCheck::SelfCheck(Display *display, Network *network) : display_(display), network_(network) {
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

template<typename T>
bool single_check(const char *name, T fn) {
    auto ok = fn();
    loginfo("%s... %s", name, ok ? "OK" : "ERROR");
    return ok;
}

bool SelfCheck::rtc() {
    return single_check("rtc", [&]() {
        CoreClock clock{ Wire };

        if (!clock.begin()) {
            return false;
        }

        return true;
    });
}

bool SelfCheck::temperature() {
    return single_check("temperature", [&]() {
        CoreTemperature temp{ Wire };

        if (!temp.begin()) {
            return false;
        }

        return true;
    });
}

bool SelfCheck::battery_gauge() {
    return single_check("battery gauge", [&]() {
        BatteryGauge gauge{ Wire1 };

        if (!gauge.begin()) {
            return false;
        }

        return true;
    });
}

bool SelfCheck::qspi_memory() {
    return single_check("qspi memory", [&]() {
        Adafruit_QSPI_Flash qspi_flash;

        pinMode(QSPI_FLASH_CS, OUTPUT);
        digitalWrite(QSPI_FLASH_CS, LOW);

        if (!qspi_flash.begin()){
            return false;
        }

        return true;
    });
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
    return single_check("gps", [&]() {
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
            return false;
        }

        return true;
    });
}

bool SelfCheck::wifi() {
    return single_check("wifi", [&]() {
        MetalNetwork network;
        auto settings = NetworkSettings{
            .create = false,
            .ssid = nullptr,
            .password = nullptr,
            .name = nullptr,
            .port = 0,
        };
        auto ok = network.begin(settings);
        if (ok) {
            network.stop();
        }

        return ok;
    });
}

}
