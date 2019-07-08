#pragma once

#include "common.h"
#include "hal/hal.h"

namespace fk {

class SelfCheck {
private:
    Display *display_;
    Network *network_;

public:
    SelfCheck(Display *display, Network *network);

public:
    void check();

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
