#include "display/leds.h"
#include "state_ref.h"
#include "hal/leds.h"

namespace fk {

FK_DECLARE_LOGGER("leds");

LedsController::LedsController() {
}

bool LedsController::begin() {
    return get_module_leds()->begin();
}

static Color get_color(ModuleStatus status) {
    switch (status) {
    case ModuleStatus::Unknown:
        return { 0, 0, 0 };
    case ModuleStatus::Empty:
        return { 0, 0, 0 };
    case ModuleStatus::Found:
        return { 0x20, 0x20, 0x20 };
    case ModuleStatus::Ok:
        return { 0xff, 0xff, 0xff };
    case ModuleStatus::Warning:
        return { 0xcc, 0xcc, 0 };
    case ModuleStatus::Fatal:
        return { 0xff, 0, 0 };
    }

    return { 0xff, 0, 0 };
}

void LedsController::tick() {
    if (get_modmux()->available_positions().size() <= 1) {
        return;
    }

    auto gs = get_global_state_ro();
    if (gs.get()->version == version_) {
        return;
    }

    auto module_leds = get_module_leds();

    for (auto bay : get_modmux()->available_positions()) {
        auto index = bay.integer();
        if (index > 0) {
            auto attached = gs.get()->dynamic.attached();
            FK_ASSERT(attached != nullptr);

            auto module_status = attached->get_module_status(bay);
            auto color = get_color(module_status);
            module_leds->color(index - 1, color, false);
        }
    }

    module_leds->on();

    version_ = gs.get()->version;
}

} // namespace fk
