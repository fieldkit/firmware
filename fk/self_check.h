#pragma once

#include "common.h"
#include "hal/hal.h"

namespace fk {

struct SelfCheckSettings {
    bool check_gps{ false };
    bool check_sd_card{ false };
    bool check_backplane{ false };
    bool check_lora{ false };
    bool flash_leds{ false };
    bool module_presence{ true };

    SelfCheckSettings() {
    }

    SelfCheckSettings(bool gps, bool sd_card, bool backplane, bool lora, bool flash_leds) : check_gps(gps), check_sd_card(sd_card), check_backplane(backplane), check_lora(lora), flash_leds(flash_leds) {
    }

    static SelfCheckSettings detailed() {
        return { true, true, true, true, true };
    }

    static SelfCheckSettings defaults() {
        return { true, true, true, true, false };
    }
};

enum class CheckStatus : uint8_t {
    Pending,
    Unknown,
    Pass,
    Fail,
};

class ModuleCheckStatus {
private:
    uint32_t value_{ 0 };

public:
    ModuleCheckStatus(uint32_t value) : value_(value) {
    }

public:
    uint16_t value() const {
        return value_;
    }

    operator bool() const {
        return value_ > 0;
    }
};

struct SelfCheckStatus {
    CheckStatus rtc{ CheckStatus::Pending };
    CheckStatus temperature{ CheckStatus::Pending };
    CheckStatus battery_gauge{ CheckStatus::Pending };
    CheckStatus solar_gauge{ CheckStatus::Pending };
    CheckStatus qspi_memory{ CheckStatus::Pending };
    CheckStatus spi_memory{ CheckStatus::Pending };
    CheckStatus gps{ CheckStatus::Pending };
    CheckStatus wifi{ CheckStatus::Pending };
    CheckStatus sd_card_open{ CheckStatus::Pending };
    CheckStatus sd_card_write{ CheckStatus::Pending };
    CheckStatus bp_mux{ CheckStatus::Pending };
    CheckStatus bp_shift{ CheckStatus::Pending };
    CheckStatus bp_leds{ CheckStatus::Pending };
    CheckStatus lora{ CheckStatus::Pending };
    ModuleCheckStatus modules{ 0 };
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
    ModuleLeds *leds_;

public:
    SelfCheck(Display *display, Network *network, ModMux *mm, ModuleLeds *leds);

public:
    void check(SelfCheckSettings settings, SelfCheckCallbacks &callback, Pool *pool);

private:
    bool rtc();
    bool temperature();
    bool battery_gauge();
    bool solar_gauge();
    bool qspi_memory();
    bool spi_memory();
    bool gps();
    bool wifi(Pool *pool);
    bool sd_card_open();
    bool sd_card_write();
    bool backplane_shift();
    bool backplane_mux();
    bool backplane_leds();
    bool lora();
    ModuleCheckStatus modules(Pool *pool);
    void flash_leds();

};

}
