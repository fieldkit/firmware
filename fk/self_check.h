#pragma once

#include "common.h"
#include "display.h"
#include "hal/hal.h"

namespace fk {

class SelfCheck {
private:
    Display *display_;
    Wifi *wifi_;

public:
    SelfCheck(Display *display, Wifi *wifi);

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

};

}
