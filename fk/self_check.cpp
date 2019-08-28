#include "fk.h"
#include "self_check.h"
#include "clock.h"
#include "temperature.h"
#include "hal/metal/metal.h"

namespace fk {

FK_DECLARE_LOGGER("check");

SelfCheck::SelfCheck(Display *display, Network *network) : display_(display), network_(network) {
}

void SelfCheck::check(SelfCheckSettings settings) {
    loginfo("starting");

    // TODO: Check this failure scenario.
    display_->company_logo();

    rtc();
    battery_gauge();
    temperature();
    qspi_memory();
    spi_memory();
    wifi();

    if (settings.check_gps) {
        gps();
    }

    if (settings.check_sd_card) {
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
        CoreTemperature temp{ get_board()->i2c_core() };

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
        auto memory = MemoryFactory::get_qspi_memory();

        if (!memory->begin()) {
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

    loginfo("bank memory ready (%" PRIu32 "MB) (%" PRIu32 " blocks)", g.total_size / OneMegabyte, g.nblocks);

    return nbanks > 0;
}

bool SelfCheck::gps() {
    return single_check("gps", []() {
        get_board()->enable_gps();

        auto gps = get_gps();

        if (!gps->begin()) {
            return false;
        }

        auto started = fk_uptime();
        while ((fk_uptime() - started) < FiveSecondsMs) {
            GpsFix fix;
            if (!gps->service(fix)) {
                return false;
            }

            if (fix.chars > 10) {
                return true;
            }
        }

        return false;
    });
}

bool SelfCheck::wifi() {
    return single_check("wifi", []() {
        auto network = get_network();

        auto settings = NetworkSettings{
            .create = false,
            .ssid = nullptr,
            .password = nullptr,
            .name = nullptr,
            .port = 0,
        };
        auto ok = network->begin(settings);
        if (ok) {
            network->stop();
        }

        return ok;
    });
}

bool SelfCheck::sd_card() {
    return single_check("sd card", []() {
        auto sd_card = get_sd_card();

        if (!sd_card->begin()) {
            return false;
        }

        return true;
    });
}

}
