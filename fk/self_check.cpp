#include "fk.h"
#include "self_check.h"
#include "clock.h"
#include "temperature.h"
#include "hal/metal/metal.h"

#include <Wire.h>
#include <Adafruit_QSPI.h>
#include <Adafruit_QSPI_Flash.h>

#include <phylum/backend.h>
#include <backends/arduino_sd/arduino_sd.h>

namespace fk {

FK_DECLARE_LOGGER("check");

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
    wifi();
    if (fkc.full_self_check) {
        gps();
        sd_card();
    }

    loginfo("done");
}

void check_message(const char *name, bool ok) {
    alogf(ok ? LogLevels::INFO : LogLevels::ERROR, LOG_FACILITY, "%s... %s", name, ok ? "OK" : "ERROR");
}

template<typename T>
bool single_check(const char *name, T fn) {
    auto ok = fn();
    check_message(name, ok);
    return ok;
}

bool SelfCheck::rtc() {
    return single_check("rtc", []() {
        auto clock = get_clock();

        if (!clock->begin()) {
            return false;
        }

        return true;
    });
}

bool SelfCheck::temperature() {
    return single_check("temperature", []() {
        CoreTemperature temp{ Wire };

        if (!temp.begin()) {
            return false;
        }

        return true;
    });
}

bool SelfCheck::battery_gauge() {
    return single_check("battery gauge", []() {
        auto gauge = get_battery_gauge();

        if (!gauge->begin()) {
            return false;
        }

        return true;
    });
}

bool SelfCheck::qspi_memory() {
    return single_check("qspi memory", []() {
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
    auto banks = MemoryFactory::get_data_memory_banks();
    auto memory = MemoryFactory::get_data_memory();

    auto nbanks = 0;
    for (size_t i = 0; i < MemoryFactory::NumberOfDataMemoryBanks; ++i) {
        auto &bank = *banks[i];

        // TODO: Why is this necessary?
        fk_delay(100);

        if (bank.begin()) {
            loginfo("memory bank #%d... OK", nbanks);
            nbanks++;
        }
    }

    if (!memory->begin()) {
        check_message("bank memory", false);
        return false;
    }

    auto g = memory->geometry();

    loginfo("bank memory ready (%luMB) (%lu blocks)", g.total_size / OneMegabyte, g.nblocks);

    return nbanks > 0;
}

bool SelfCheck::gps() {
    return single_check("gps", []() {
        Serial1.begin(9600);

        get_board()->enable_gps();

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
    return single_check("wifi", []() {
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

bool SelfCheck::sd_card() {
    return single_check("sd card", []() {
        SPI2.begin();

        phylum::Geometry g;
        phylum::ArduinoSdBackend storage;
        if (!storage.initialize(g, PIN_SD_CS)) {
            logwarn("initialize failed");
            return false;
        }

        if (!storage.open()) {
            logwarn("open failed");
            return false;
        }
        return true;
    });
}

}
