#include "fk.h"
#include "self_check.h"

namespace fk {

SelfCheck::SelfCheck(Display *display) : display_(display) {
}

void SelfCheck::check() {
    display_->company_logo();

    // TODO: Check RTC
    // TODO: Check Battery gauge
    // TODO: Check QSPI memory
    // TODO: Check SPI memory
    // TODO: Check Temperature
    // TODO: Check WiFi
    // TODO: Check GPS

    // NOTE: If the display isn't working we can still create Wifi.
}

}
