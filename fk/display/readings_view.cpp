#include "readings_view.h"
#include "state_ref.h"
#include "hal/board.h"
#include "hal/display.h"
#include "platform.h"

namespace fk {

FK_DECLARE_LOGGER("readings");

void ReadingsView::tick(ViewController *views, Pool &pool) {
    if (fk_uptime() < dirty_) {
        return;
    }

    dirty_ = fk_uptime() + 1000;

    auto gs = get_global_state_ro();

    auto modules = gs.get()->modules;

    if (modules == nullptr) {
        return;
    }

    size_t number_sensors = 0;
    for (auto m = 0u; m < modules->nmodules; ++m) {
        auto &module = modules->modules[m];
        number_sensors += module.nsensors;
    }

    auto index = position_ % number_sensors;

    for (auto m = 0u; m < modules->nmodules; ++m) {
        auto &module = modules->modules[m];
        for (auto s = 0u; s < module.nsensors; ++s) {
            auto &sensor = module.sensors[s];
            if (index-- == 0) {
                if (sensor.has_live_vaue) {
                    ReadingScreen reading{ module.name, sensor.name, sensor.live_value };
                    auto bus = get_board()->i2c_core();
                    auto display = get_display();
                    display->reading(reading);
                    return;
                }
            }
        }
    }
}

void ReadingsView::up(ViewController *views) {
    position_--;
    dirty_ = 0;
}

void ReadingsView::down(ViewController *views) {
    position_++;
    dirty_ = 0;
}

void ReadingsView::enter(ViewController *views) {
    views->show_home();
}

}
