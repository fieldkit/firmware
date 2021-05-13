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

bool EnableModulePower::can_control() {
    return enabled_ && position_ != ModulePosition::Virtual;
}

bool EnableModulePower::enabled_once() {
    return can_control() && power_ == ModulePower::ReadingsOnly;
}

bool EnableModulePower::always_enabled() {
    return can_control() && power_ == ModulePower::Always;
}

bool EnableModulePower::should_enable() {
    return can_control() && power_ == ModulePower::RareStarts;
}

bool EnableModulePower::enable() {
    auto mm = get_modmux();
    if (enabled_once() || always_enabled() || should_enable()) {
        if (!mm->is_module_on(position_)) {
            logverbose("[%d] powering on", position_.integer());
            was_enabled_ = true;
        }

        // Updates power flag even if the module is already on.
        loginfo("enable-module power=%d", power_);
        if (!mm->enable_module(position_, power_)) {
            logerror("[%d] enabling module failed", position_.integer());
            return false;
        }
    }
    return true;
}

void EnableModulePower::fatal_error() {
    if (always_enabled()) {
        if (!get_modmux()->disable_module(position_)) {
            logerror("[%d] disabling module failed", position_.integer());
        }
    }
}

} // namespace fk
