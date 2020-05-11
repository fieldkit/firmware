#pragma once

#include <loading.h>
#include <tiny_printf.h>

#include "common.h"
#include "hal/display.h"
#include "display_views.h"
#include "config.h"

extern const struct fkb_header_t fkb_header;

namespace fk {

class BuildView : public DisplayView {
private:
    char message_[MaximumBuildStringLength];

public:
    void tick(ViewController *views, Pool &pool) override {
        tiny_snprintf(message_, sizeof(message_), "Build #%" PRIu32, fkb_header.firmware.number);
        auto bus = get_board()->i2c_core();
        auto display = get_display();
        display->simple(SimpleScreen{ message_ });
    }

};

}
