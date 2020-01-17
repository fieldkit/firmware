#include "module_status_view.h"
#include "hal/board.h"
#include "hal/display.h"
#include "config.h"
#include "state_ref.h"

namespace fk {

FK_DECLARE_LOGGER("modstatus");

static ModuleStatusScreen get_module_status_screen(uint8_t bay, PhysicalModuleState const &pm) {
    auto name = pm.meta == nullptr ? "<unknown>" : pm.meta->name;

    switch (pm.status) {
    case ModuleStatus::Unknown:
        return{
            .bay = bay,
            .name = name,
            .message = "<unknown>",
        };
    case ModuleStatus::Empty:
        return{
            .bay = bay,
            .name = "<empty>",
            .message = "",
        };
    case ModuleStatus::Found:
        return{
            .bay = bay,
            .name = name,
            .message = "found",
        };
    case ModuleStatus::Ok:
        return{
            .bay = bay,
            .name = name,
            .message = "ok",
        };
    case ModuleStatus::Warning:
        return{
            .bay = bay,
            .name = name,
            .message = "warning",
        };
    case ModuleStatus::Fatal:
        return{
            .bay = bay,
            .name = name,
            .message = "fatal",
        };
    }

    return {
        .bay = bay,
        .name = "<error>",
        .message = "<error>",
    };
}

void ModuleStatusView::tick(ViewController *views) {
    auto gs = get_global_state_ro();
    auto &physical = gs.get()->physical_modules[bay_];

    auto bus = get_board()->i2c_core();
    auto display = get_display();
    auto screen = get_module_status_screen(bay_, physical);
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
