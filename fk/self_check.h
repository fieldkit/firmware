#pragma once

#include "common.h"
#include "hal/hal.h"

namespace fk {

struct SelfCheckSettings {
    bool check_gps{ false };
    bool check_sd_card{ false };

    SelfCheckSettings() {
    }

    SelfCheckSettings(bool gps, bool sd_card) : check_gps(gps), check_sd_card(sd_card) {
    }
};

class SelfCheck {
private:
    Display *display_;
    Network *network_;

public:
    SelfCheck(Display *display, Network *network);

public:
    void check(SelfCheckSettings settings);

private:
    bool rtc();
    bool temperature();
    bool battery_gauge();
    bool qspi_memory();
    bool spi_memory();
    bool gps();
    bool wifi();
    bool sd_card();

};

}
