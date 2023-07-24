#include "module_lockout.h"
#include "platform.h"

namespace fk {

FK_DECLARE_LOGGER("lockout");

void ModuleLockout::enable_until_uptime(uint32_t unlocked) {
    unlocked_ = unlocked;
    loginfo("lockout enabled until %" PRIu32, unlocked);
}

bool ModuleLockout::can_enable() {
    if (unlocked_ > 0) {
        auto uptime = fk_uptime();
        if (uptime < unlocked_) {
            auto remaining = unlocked_ - uptime;
            if (remaining < 0) {
                loginfo("locked (negative) %" PRIu32, remaining);
                unlocked_ = fk_uptime() + OneMinuteMs;
                return false;
            }
            if (remaining > FiveSecondsMs) {
                loginfo("locked %" PRIu32, remaining);
                return false;
            }
        } else {
            loginfo("locked expired");
        }

        unlocked_ = 0;
    } else {
        loginfo("unlocked");
    }

    return true;
}

bool ModuleLockout::try_enable(ModulePosition position) {
    auto uptime = fk_uptime();

    // If we were locked out, check to see if it's expired, otherwise we return
    // nothing, no readings. It may be necessary later to actually specify what
    // happened to the caller.
    if (unlocked_ > 0) {
        if (uptime < unlocked_) {
            auto remaining = unlocked_ - uptime;
            if (remaining < 0) {
                loginfo("[%d] locked (negative) %" PRIu32, position, remaining);
                unlocked_ = fk_uptime() + OneMinuteMs;
                return false;
            }
            if (remaining > FiveSecondsMs) {
                loginfo("[%d] locked %" PRIu32, position, remaining);
                return false;
            }

            loginfo("[%d] locked %" PRIu32 " waiting for expiration.", position, remaining);
            fk_delay(remaining);
        } else {
            loginfo("[%d] locked expired", position);
        }

        unlocked_ = 0;
    } else {
        loginfo("[%d] unlocked", position);
    }

    return true;
}

} // namespace fk