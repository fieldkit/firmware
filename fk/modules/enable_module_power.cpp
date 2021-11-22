#include "modules/enable_module_power.h"

namespace fk {

FK_DECLARE_LOGGER("readings");

EnableModulePower::EnableModulePower(ModulePosition position, ModulePower power, uint32_t wake_delay)
    : position_(position), power_(power), wake_delay_(wake_delay) {
}

EnableModulePower::~EnableModulePower() {
    if (enabled_once()) {
        logdebug("[%d] powering off", position_.integer());
        if (!get_modmux()->disable_module(position_)) {
            logerror("[%d] disabling module failed", position_.integer());
        }
    } else {
        logdebug("[%d] leaving alone", position_.integer());
    }
}

bool EnableModulePower::can_control() {
    return position_ != ModulePosition::Virtual;
}

bool EnableModulePower::enabled_once() {
    return can_control() && power_ == ModulePower::ReadingsOnly;
}

bool EnableModulePower::always_enabled() {
    return can_control() && power_ == ModulePower::Always;
}

bool EnableModulePower::was_enabled() {
    return was_enabled_;
}

bool EnableModulePower::enable() {
    auto mm = get_modmux();

    was_enabled_ = false;

    if (enabled_once() || always_enabled()) {
        if (!mm->is_module_on(position_)) {
            logdebug("[%d] powering on", position_.integer());
            was_enabled_ = true;
        }

        // Updates power flag even if the module is already on.
        loginfo("enable-module power=%d", power_);
        if (!mm->enable_module(position_, power_)) {
            logerror("[%d] enabling module failed", position_.integer());
            return false;
        }

        if (was_enabled_ && wake_delay_ > 0) {
            logdebug("[%d] wake delay: %" PRIu32 "ms", position_.integer(), wake_delay_);
            fk_delay(wake_delay_);
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
