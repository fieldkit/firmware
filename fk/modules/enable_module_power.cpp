#include "modules/enable_module_power.h"

namespace fk {

FK_DECLARE_LOGGER("readings");

EnableModulePower::EnableModulePower(ModulePosition position, ModulePower power, uint32_t wake_delay)
    : position_(position), power_(power), wake_delay_(wake_delay) {
}

EnableModulePower::~EnableModulePower() {
    if (enabled_once()) {
        loginfo("[%d] module power off", position_.integer());
        if (!get_modmux()->disable_module(position_)) {
            logerror("[%d] disabling module failed", position_.integer());
        }

        auto off_time = fk_uptime();
        if (enabled_at_ > 0) {
            loginfo("[%d] module power on for %" PRIu32, position_.integer(), off_time - enabled_at_);
        } else {
            logwarn("[%d] module power unexpectedly on for %" PRIu32, position_.integer(), off_time - enabled_at_);
        }
    } else {
        loginfo("[%d] module power left as-is", position_.integer());
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
    return enabled_at_ > 0;
}

bool EnableModulePower::enable() {
    auto mm = get_modmux();

    enabled_at_ = 0;

    if (enabled_once() || always_enabled()) {
        if (!mm->is_module_on(position_)) {
            loginfo("[%d] module power on", position_.integer());
            enabled_at_ = fk_uptime();
        } else {
            logwarn("[%d] module power already on", position_.integer());
            enabled_at_ = fk_uptime();
        }

        // Updates power flag even if the module is already on.
        if (!mm->enable_module(position_, power_)) {
            logerror("[%d] enabling module failed", position_.integer());
            return false;
        }

        if (enabled_at_ > 0 && wake_delay_ > 0) {
            loginfo("[%d] wake delay: %" PRIu32 "ms", position_.integer(), wake_delay_);
            fk_delay(wake_delay_);
        }
    } else {
        if (mm->is_module_on(position_)) {
            logwarn("[%d] module power enable ignored and already on", position_.integer());
        } else {
            logwarn("[%d] module power enable ignored", position_.integer());
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
