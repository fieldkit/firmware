#include "fk.h"
#include "self_check.h"
#include "clock.h"
#include "temperature.h"
#include "hal/metal/metal.h"

namespace fk {

FK_DECLARE_LOGGER("check");

void check_message(const char *name, bool ok, uint32_t elapsed, bool critical) {
    alogf(ok ? LogLevels::INFO : (critical ? LogLevels::ERROR : LogLevels::INFO), LOG_FACILITY, "%s... %s (%" PRIu32  "ms)", name, ok ? "OK" : "ERROR", elapsed);
}

template<typename T>
bool single_check(const char *name, T fn) {
    auto started = fk_uptime();
    auto ok = fn();
    check_message(name, ok, fk_uptime() - started, true);
    return ok;
}

template<typename T>
bool single_noncritical_check(const char *name, T fn) {
    auto started = fk_uptime();
    auto ok = fn();
    check_message(name, ok, fk_uptime() - started, false);
    return ok;
}

SelfCheck::SelfCheck(Display *display, Network *network, ModMux *mm, ModuleLeds *leds) : display_(display), network_(network), mm_(mm), leds_(leds) {
}

static CheckStatus to_status(bool ok) {
    return ok ? CheckStatus::Pass : CheckStatus::Fail;
}

void SelfCheck::check(SelfCheckSettings settings, SelfCheckCallbacks &callbacks) {
    loginfo("starting");

    SelfCheckStatus status;

    callbacks.update(status);

    status.rtc = to_status(rtc());
    callbacks.update(status);

    status.battery_gauge = to_status(battery_gauge());
    callbacks.update(status);

    status.temperature = to_status(temperature());
    callbacks.update(status);

    status.qspi_memory = to_status(qspi_memory());
    callbacks.update(status);

    status.spi_memory = to_status(spi_memory());
    callbacks.update(status);

    status.wifi = to_status(wifi());
    callbacks.update(status);

    if (settings.check_gps) {
        status.gps = to_status(gps());
    }
    else {
        status.gps = CheckStatus::Unknown;
    }
    callbacks.update(status);

    if (settings.check_sd_card) {
        status.sd_card_open = to_status(sd_card_open());
        status.sd_card_write = to_status(sd_card_write());
    }
    else {
        status.sd_card_open = CheckStatus::Unknown;
        status.sd_card_write = CheckStatus::Unknown;
    }
    callbacks.update(status);

    if (settings.check_backplane) {
        status.bp_shift = to_status(backplane_shift());
        callbacks.update(status);

        status.bp_mux = to_status(backplane_mux());
        callbacks.update(status);

        status.bp_leds = to_status(backplane_leds());
        callbacks.update(status);
    }
    else {
        status.bp_mux = CheckStatus::Unknown;
        status.bp_shift = CheckStatus::Unknown;
        status.bp_leds = CheckStatus::Unknown;
    }

    if (settings.check_lora) {
        status.lora = to_status(lora());
        callbacks.update(status);
    }

    callbacks.update(status);

    loginfo("done");
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
        check_message("bank memory", false, 0, true);
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
    return single_check("wifi", [=]() {
        auto settings = NetworkSettings{
            .valid = false,
            .create = false,
            .ssid = nullptr,
            .password = nullptr,
            .port = 0,
        };
        auto ok = network_->begin(settings);
        if (ok) {
            network_->stop();
        }

        return ok;
    });
}

bool SelfCheck::sd_card_open() {
    return single_noncritical_check("sd card open", []() {
        auto sd_card = get_sd_card();

        if (!sd_card->begin()) {
            return false;
        }

        return true;
    });
}

bool SelfCheck::sd_card_write() {
    return single_noncritical_check("sd card write", []() {
        StandardPool pool{ "sdw" };

        auto sd_card = get_sd_card();

        if (!sd_card->begin()) {
            return false;
        }

        FormattedTime formatted{ get_clock_now(), TimeFormatMachine };
        char file_name[strlen(formatted.cstr()) + 4 + 1];
        tiny_snprintf(file_name, sizeof(file_name), "%s.chk", formatted.cstr());

        auto file = sd_card->open(file_name, true, pool);
        if (file == nullptr || !file) {
            logerror("error opening %s", file_name);
            return false;
        }

        auto buffer = "Self Check";

        file->write((uint8_t *)buffer, strlen(buffer));
        file->close();

        return true;
    });
}

bool SelfCheck::backplane_shift() {
    return single_check("bp shift", [=]() {
        if (!mm_->begin()) {
            return false;
        }
        return true;
    });
}

bool SelfCheck::backplane_mux() {
    return single_check("bp mux", [=]() {
        if (!mm_->choose(0)) {
            return false;
        }
        return true;
    });
}

bool SelfCheck::backplane_leds() {
    return single_check("bp leds", [=]() {
        if (!leds_->begin()) {
            return false;
        }
        return true;
    });
}

bool SelfCheck::lora() {
    return single_noncritical_check("lora", [=]() {
        auto lora = get_lora_network();

        if (!lora->begin()) {
            return false;
        }

        return true;
    });
}

}
