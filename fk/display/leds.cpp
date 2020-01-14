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

void LedsController::tick() {
    auto gs = get_global_state_ro();
    if (gs.get()->version == version_) {
        return;
    }

    auto module_leds = get_module_leds();

    for (auto bay = 0u; bay < MaximumNumberOfPhysicalModules; ++bay) {
        auto &status = gs.get()->physical_modules[bay];
        auto color = Color{ 0, 0, 0 };

        if (status.available) {
            color = Color{ 255, 255, 255 };
        }

        module_leds->color(bay, color, false);
    }

    module_leds->on();

    version_ = gs.get()->version;
}

}
