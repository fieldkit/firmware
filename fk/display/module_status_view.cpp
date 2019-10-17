#include "module_status_view.h"
#include "hal/board.h"
#include "hal/display.h"
#include "config.h"
#include "state_ref.h"

namespace fk {

FK_DECLARE_LOGGER("modstatus");

void ModuleStatusView::tick(ViewController *views) {
    auto bus = get_board()->i2c_core();
    auto display = get_display();

    auto gs = get_global_state_ro();
    auto &s = gs.get()->physical_modules[bay_];

    auto message = "no module";

    if (s.available) {
        if (s.initialized) {
            message = s.meta->name;
        }
        else {
            message = "uninitialized";
        }
    }
    else {
        if (s.attempted) {
            message = "eeprom error";
        }
    }

    ModuleStatusScreen screen{
        .bay = bay_,
        .message = message,
    };
    display->module_status(screen);
}

void ModuleStatusView::up(ViewController *views) {
    bay_ = (bay_ - 1) % MaximumNumberOfPhysicalModules;
    loginfo("selected %d", bay_);
}

void ModuleStatusView::down(ViewController *views) {
    bay_ = (bay_ + 1) % MaximumNumberOfPhysicalModules;
    loginfo("selected %d", bay_);
}

void ModuleStatusView::enter(ViewController *views) {
    views->show_home();
}

}
