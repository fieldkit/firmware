#pragma once

#include "common.h"
#include "hal/hal.h"

namespace fk {

struct SelfCheckSettings {
    bool check_gps{ false };
    bool check_sd_card{ false };
    bool check_backplane{ false };
    bool check_lora{ false };

    SelfCheckSettings() {
    }

    SelfCheckSettings(bool gps, bool sd_card, bool backplane, bool lora) : check_gps(gps), check_sd_card(sd_card), check_backplane(backplane), check_lora(lora) {
    }
};

enum class CheckStatus : uint8_t {
    Pending,
    Unknown,
    Pass,
    Fail,
};

struct SelfCheckStatus {
    CheckStatus rtc{ CheckStatus::Pending };
    CheckStatus temperature{ CheckStatus::Pending };
    CheckStatus battery_gauge{ CheckStatus::Pending };
    CheckStatus qspi_memory{ CheckStatus::Pending };
    CheckStatus spi_memory{ CheckStatus::Pending };
    CheckStatus gps{ CheckStatus::Pending };
    CheckStatus wifi{ CheckStatus::Pending };
    CheckStatus sd_card{ CheckStatus::Pending };
    CheckStatus bp_mux{ CheckStatus::Pending };
    CheckStatus bp_shift{ CheckStatus::Pending };
    CheckStatus lora{ CheckStatus::Pending };
};

class SelfCheckCallbacks {
public:
    virtual void update(SelfCheckStatus status) = 0;
};

class NoopSelfCheckCallbacks : public SelfCheckCallbacks {
public:
    void update(SelfCheckStatus status) override {
    }
};

class SelfCheck {
private:
    Display *display_;
    Network *network_;
    ModMux *mm_;

public:
    SelfCheck(Display *display, Network *network, ModMux *mm);

public:
    void check(SelfCheckSettings settings, SelfCheckCallbacks &callback);

private:
    bool rtc();
    bool temperature();
    bool battery_gauge();
    bool qspi_memory();
    bool spi_memory();
    bool gps();
    bool wifi();
    bool sd_card();
    bool backplane_shift();
    bool backplane_mux();
    bool lora();

};

}
