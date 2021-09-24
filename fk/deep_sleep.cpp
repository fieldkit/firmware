#include "deep_sleep.h"
#include "clock.h"
#include "hal/network.h"
#include "hal/random.h"
#include "tasks/tasks.h"

namespace fk {

FK_DECLARE_LOGGER("sleep");

constexpr uint32_t MinimumDeepSleepMs = 8192;

constexpr uint32_t MinimumAcceptableDeepSleepMs = (MinimumDeepSleepMs / 1000) * 1000;

static uint32_t deep_sleep() {
    auto now_before = get_clock_now();

    fk_deep_sleep(MinimumDeepSleepMs);

    auto now_after = get_clock_now();
    if (now_after < now_before) {
        logwarn("before=%" PRIu32 " now=%" PRIu32, now_before, now_after);
        return 0;
    }

    auto elapsed = (now_after - now_before) * 1000;
    if (elapsed > 0) {
        logdebug("before=%" PRIu32 " now=%" PRIu32 " elapsed=%" PRIu32, now_before, now_after, elapsed);
    }

    fk_uptime_adjust_after_sleep(elapsed);

    return elapsed;
}

static bool can_deep_sleep(Runnable const &runnable) {
    if (os_task_is_running(&network_task)) {
        loginfo("no-sleep: network task");
        return false;
    }
    if (os_task_is_running(&display_task)) {
        loginfo("no-sleep: display task");
        return false;
    }
    if (runnable.is_running()) {
        loginfo("no-sleep: runnable (gps)");
        return false;
    }
    if (get_network()->enabled()) {
        loginfo("no-sleep: network");
        return false;
    }

    return true;
}

bool DeepSleep::once() {
    return deep_sleep() > 0;
}

bool DeepSleep::try_deep_sleep(lwcron::Scheduler &scheduler, Runnable const &runnable) {
    if (can_deep_sleep(runnable)) {
        while (true) {
            auto now = get_clock_now();
            auto nextTask = scheduler.nextTask(lwcron::DateTime{ now }, 0);
            if (!nextTask) {
                logerror("no next task, that's very strange");
                break;
            }

            // If we have enough time for a nap, otherwise we bail.
            auto remaining_seconds = nextTask.time - now;
            if (remaining_seconds * 1000 < MinimumDeepSleepMs) {
                loginfo("no-sleep: task-eta=%" PRIu32 "s", remaining_seconds);
                break;
            }

            // Sleep!
            // This can return early for a few reasons:
            // 1) We're unable to sleep, in which case this will
            // return 0.
            // 2) We were woken up via IRQ of some kind, which can
            // also return 0. So we basically gotta just bail out of
            // here in either case.
            if (deep_sleep() < MinimumAcceptableDeepSleepMs) {
                break;
            }
        }

        return true;
    }

    return false;
}

} // namespace fk
