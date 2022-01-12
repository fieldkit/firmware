#pragma once

#include "common.h"
#include "display_views.h"
#include "hal/display.h"

namespace fk {

class MessageView : public DisplayView {
private:
    uint32_t disappear_ms_{ 0 };
    const char *message_{ nullptr };

public:
    void tick(ViewController *views, Pool &pool) override {
        auto bus = get_board()->i2c_core();
        auto display = get_display();
        display->simple(SimpleScreen{ message_ });
        if (disappear_ms_ > 0 && fk_uptime() > disappear_ms_) {
            views->show_home();
        }
    }

public:
    void message(const char *message, uint32_t visible_ms = 0) {
        message_ = message;
        if (visible_ms > 0) {
            disappear_ms_ = visible_ms + fk_uptime();
        } else {
            disappear_ms_ = 0;
        }
    }
};

class FaultView : public DisplayView {
private:
    FaultCode *fault_code_{ nullptr };

public:
    void tick(ViewController *views, Pool &pool) override {
        if (fault_code_ != nullptr) {
            get_display()->fault(fault_code_);
        }
    }

public:
    void fault_code(FaultCode *fault_code) {
        fault_code_ = fault_code;
    }
};

} // namespace fk
