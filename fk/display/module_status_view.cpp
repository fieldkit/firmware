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

    auto name = "<empty>";
    auto message = "";

    if (s.meta != nullptr) {
        name = s.meta->name;
        message = "uninitialized";
    }

    switch (s.status) {
    case ModuleStatus::Unknown:
        name = "<unknown>";
        break;
    case ModuleStatus::Empty:
        break;
    case ModuleStatus::Ok:
        message = "ok";
        break;
    case ModuleStatus::Warning:
        message = "warning";
        break;
    case ModuleStatus::Fatal:
        message = "fatal";
        break;
    }

    ModuleStatusScreen screen{
        .bay = bay_,
        .name = name,
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
