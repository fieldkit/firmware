#include "module_status_view.h"
#include "hal/board.h"
#include "hal/display.h"
#include "config.h"

namespace fk {

FK_DECLARE_LOGGER("modstatus");

void ModuleStatusView::tick(ViewController *views) {
    auto bus = get_board()->i2c_core();
    auto display = get_display();

    ModuleStatusScreen screen{
        .bay = bay_,
        .message = "",
    };
    display->module_status(screen);
}

void ModuleStatusView::up(ViewController *views) {
    bay_ = (bay_ - 1) % MaximumNumberOfPhysicalModules;
}

void ModuleStatusView::down(ViewController *views) {
    bay_ = (bay_ + 1) % MaximumNumberOfPhysicalModules;
}

void ModuleStatusView::enter(ViewController *views) {
    views->show_home();
}

}
