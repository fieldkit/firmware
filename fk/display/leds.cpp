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
    case ModuleStatus::Unknown: return { 0, 0, 0 };
    case ModuleStatus::Empty: return { 0, 0, 0 };
    case ModuleStatus::Ok: return { 0xff, 0xff, 0xff };
    case ModuleStatus::Warning: return { 0xcc, 0xcc, 0 };
    case ModuleStatus::Fatal: return { 0xff, 0, 0 };
    }

    return { 0xff, 0, 0 };
}

void LedsController::tick() {
    auto gs = get_global_state_ro();
    if (gs.get()->version == version_) {
        return;
    }

    auto module_leds = get_module_leds();

    for (auto bay = 0u; bay < MaximumNumberOfPhysicalModules; ++bay) {
        auto &physical_module = gs.get()->physical_modules[bay];
        auto color = get_color(physical_module.status);

        module_leds->color(bay, color, false);

        logverbose("[%d] color = (%d, %d, %d)", bay, color.r, color.g, color.b);
    }

    module_leds->on();

    version_ = gs.get()->version;
}

}
