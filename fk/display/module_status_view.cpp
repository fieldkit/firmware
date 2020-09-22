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

ModuleStatusView::ModuleStatusView() : module_(get_modmux()->available_positions().begin()) {
}

void ModuleStatusView::tick(ViewController *views, Pool &pool) {
    auto index = (*module_).integer();
    auto gs = get_global_state_ro();
    auto &physical = gs.get()->physical_modules[index];
    auto bus = get_board()->i2c_core();
    auto display = get_display();
    auto screen = get_module_status_screen(index, physical);
    display->module_status(screen);
}

void ModuleStatusView::up(ViewController *views) {
    if (module_ == get_modmux()->available_positions().begin()) {
        module_ = get_modmux()->available_positions().end();
    }

    --module_;

    loginfo("selected %d", (*module_).integer());
}

void ModuleStatusView::down(ViewController *views) {
    ++module_;

    if (module_ == get_modmux()->available_positions().end()) {
        module_ = get_modmux()->available_positions().begin();
    }

    loginfo("selected %d", (*module_).integer());
}

void ModuleStatusView::enter(ViewController *views) {
    views->show_module_menu((*module_).integer());
}

}
