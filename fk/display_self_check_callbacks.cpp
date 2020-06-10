#include "display_self_check_callbacks.h"

namespace fk {

DisplaySelfCheckCallbacks::DisplaySelfCheckCallbacks() {
    screen_.checks = queued_;
}

void DisplaySelfCheckCallbacks::update(SelfCheckStatus status) {
    number_ = 0;
    append("rtc", status.rtc);
    append("temp", status.temperature);
    append("bg", status.battery_gauge);
    append("sg", status.solar_gauge);
    append("qspi", status.qspi_memory);
    append("spi", status.spi_memory);
    append("wifi", status.wifi);
    append("gps", status.gps);
    append("sdo", status.sd_card_open);
    append("sdw", status.sd_card_write);
    append("bpm", status.bp_mux);
    append("bps", status.bp_shift);
    append("led", status.bp_leds);
    append("lora", status.lora);
}

void DisplaySelfCheckCallbacks::append(const char *name, CheckStatus status) {
    FK_ASSERT(number_ < NumberOfChecks);
    if (status == CheckStatus::Pass) {
        checks_[number_] = { name, true };
        queued_[number_] = &checks_[number_];
        number_++;
        queued_[number_] = nullptr;
    }
    else if (status == CheckStatus::Fail) {
        checks_[number_] = { name, false };
        queued_[number_] = &checks_[number_];
        number_++;
        queued_[number_] = nullptr;
    }
}

void DisplaySelfCheckCallbacks::clear() {
    queued_[0] = nullptr;
    number_ = 0;
}

SelfCheckScreen &DisplaySelfCheckCallbacks::screen() {
    return screen_;
}

}
