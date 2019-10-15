#pragma once

#include "common.h"
#include "hal/display.h"
#include "display_views.h"

namespace fk {

class MessageView : public DisplayView {
private:
    const char *message_{ nullptr };

public:
    void tick(ViewController *views) override {
        SimpleScreen simple = { message_ };

        auto bus = get_board()->i2c_core();
        auto display = get_display();
        display->simple(simple);
    }

public:
    void message(const char *message) {
        message_ = message;
    }

};

}
