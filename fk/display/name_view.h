#pragma once

#include "common.h"
#include "hal/display.h"
#include "display_views.h"

namespace fk {

class NameView : public DisplayView {
public:
    void tick(ViewController *views, Pool &pool) override {
        auto bus = get_board()->i2c_core();
        auto display = get_display();
        display->simple({ "Name" });
    }

};

}
