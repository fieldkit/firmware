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
        return{ bay, name, "<unknown>" };
    case ModuleStatus::Empty:
        return{ bay, "<empty>", "" };
    case ModuleStatus::Found:
        return{ bay, name, "found" };
    case ModuleStatus::Ok:
        return{ bay, name, "ok" };
    case ModuleStatus::Warning:
        return{ bay, name, "warning" };
    case ModuleStatus::Fatal:
        return{ bay, name, "fatal" };
    }
    return { bay, "<error>", "<error>" };
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
    views->show_module_menu(bay_);
}

}
