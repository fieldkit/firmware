#include "deep_sleep.h"
#include "tasks/tasks.h"
#include "clock.h"
#include "hal/random.h"
#include "hal/network.h"

namespace fk {

FK_DECLARE_LOGGER("deepsleep");

constexpr uint32_t MinimumDeepSleepMs = 8192;

constexpr uint32_t MinimumAcceptableDeepSleepMs = (MinimumDeepSleepMs / 1000) * 1000;

static uint32_t deep_sleep() {
    auto now_before = get_clock_now();

    fk_deep_sleep(MinimumDeepSleepMs);

    auto now_after = get_clock_now();
    if (now_after < now_before) {
        loginfo("before=%" PRIu32 " now=%" PRIu32, now_before, now_after);
        return 0;
    }

    auto elapsed = (now_after - now_before) * 1000;
    loginfo("before=%" PRIu32 " now=%" PRIu32 " elapsed=%" PRIu32, now_before, now_after, elapsed);

    fk_uptime_adjust_after_sleep(elapsed);

    return elapsed;
}

static bool can_deep_sleep() {
    if (get_network()->enabled()) {
        loginfo("no deep sleep: network");
        return false;
    }
    if (os_task_is_running(&network_task)) {
        loginfo("no deep sleep: network task");
        return false;
    }
    if (os_task_is_running(&display_task)) {
        loginfo("no deep sleep: display task");
        return false;
    }
    if (os_task_is_running(&gps_task)) {
        loginfo("no deep sleep: gps task");
        return false;
    }

    return true;
}

bool DeepSleep::once() {
    return deep_sleep() > 0;
}

bool DeepSleep::try_deep_sleep(lwcron::Scheduler &scheduler) {
    if (can_deep_sleep()) {
        while (true) {
            auto now = get_clock_now();
            auto nextTask = scheduler.nextTask(lwcron::DateTime{ now }, 0);
            if (!nextTask) {
                // NOTE This would be so strange.
                logerror("no next task");
                break;
            }

            auto remaining_seconds = nextTask.time - now;
            loginfo("next task: %" PRIu32 "s (try)", remaining_seconds);

            // If we have enough time for a nap, otherwise we bail.
            if (remaining_seconds * 1000 < MinimumDeepSleepMs) {
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
                loginfo("bailing");
                break;
            }
        }

        return true;
    }

    return false;
}

} // namespace fk
