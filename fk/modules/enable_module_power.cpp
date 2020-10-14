#include "modules/enable_module_power.h"

namespace fk {

FK_DECLARE_LOGGER("readings");

EnableModulePower::~EnableModulePower() {
    if (enabled_once()) {
        logverbose("[%d] powering off", position_.integer());
        if (!get_modmux()->disable_module(position_)) {
            logerror("[%d] disabling module failed", position_.integer());
        }
    }
}

bool EnableModulePower::enable() {
    if (enabled_once() || always_enabled()) {
        logverbose("[%d] powering on", position_.integer());
        if (!get_modmux()->enable_module(position_)) {
            logerror("[%d] enabling module failed", position_.integer());
            return false;
        }
    }
    return true;
}

} // namespace fk
