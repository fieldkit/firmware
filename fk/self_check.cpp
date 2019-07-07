#include "fk.h"
#include "self_check.h"
#include "clock.h"
#include "battery_gauge.h"
#include "temperature.h"
#include "hal/metal/metal.h"

#include <Wire.h>
#include <Adafruit_QSPI.h>
#include <Adafruit_QSPI_Flash.h>

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

void check_message(const char *name, bool ok) {
    loginfo("%s... %s", name, ok ? "OK" : "ERROR");
}

template<typename T>
bool single_check(const char *name, T fn) {
    auto ok = fn();
    check_message(name, ok);
    return ok;
}

bool SelfCheck::rtc() {
    return single_check("rtc", []() {
        CoreClock clock{ Wire };

        if (!clock.begin()) {
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
        BatteryGauge gauge{ Wire1 };

        if (!gauge.begin()) {
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

constexpr uint32_t OneMegabyte = 1024 * 1024;

bool SelfCheck::spi_memory() {
    MetalDataMemory banks[]{
        { SPI_FLASH_CS_BANK_1 },
        { SPI_FLASH_CS_BANK_2 },
        { SPI_FLASH_CS_BANK_3 },
        { SPI_FLASH_CS_BANK_4 },
    };
    DataMemory *bank_pointers[]{
        &banks[0],
        &banks[1],
        &banks[2],
        &banks[3],
    };
    BankedDataMemory memory{ bank_pointers, 4 };

    auto nbanks = 0;
    for (auto &bank : banks) {
        // TODO: Why is this necessary?
        delay(100);

        if (bank.begin()) {
            loginfo("memory bank #%d... OK", nbanks);
            nbanks++;
        }
    }

    if (!memory.begin()) {
        check_message("bank memory", false);
        return false;
    }

    auto g = memory.get_geometry();

    if (false) {
        auto started = fk_uptime();
        for (uint32_t i = 0; i < g.nblocks; ++i) {
            uint8_t buffer[32];
            FK_ASSERT(memory.read(i * g.block_size, buffer, sizeof(buffer)));
        }

        loginfo("scan done %lums", fk_uptime() - started);
    }

    loginfo("bank memory ready (%luMB) (%lu blocks)", g.total_size / OneMegabyte, g.nblocks);

    return nbanks > 0;
}

bool SelfCheck::gps() {
    return single_check("gps", []() {
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

}
