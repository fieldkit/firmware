#include "deep_sleep.h"
#include "tasks/tasks.h"
#include "clock.h"
#include "hal/random.h"
#include "hal/network.h"

namespace fk {

FK_DECLARE_LOGGER("deepsleep");

constexpr uint32_t MinimumDeepSleepMs = 8192;

static uint32_t deep_sleep() {
    auto now_before = get_clock_now();

    loginfo("now=%" PRIu32, now_before);

    fk_deep_sleep(MinimumDeepSleepMs);

    auto now_after = get_clock_now();
    auto elapsed = std::min(now_after - now_before, (uint32_t)0u) * 1000;

    loginfo("now=%" PRIu32 " elapsed=%" PRIu32, now_after, elapsed);

    return elapsed;
}

static bool can_deep_sleep() {
    if (get_network()->enabled()) return false;
    if (os_task_is_running(&network_task)) return false;
    if (os_task_is_running(&display_task)) return false;
    if (os_task_is_running(&gps_task)) return false;
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
                logerror("no workers: no next task");
                break;
            }

            auto remaining_seconds = nextTask.time - now;
            loginfo("no workers: next task: %" PRIu32 "s", remaining_seconds);

            // If we have enough time for a nap, otherwise we bail.
            if (remaining_seconds * 1000 < MinimumDeepSleepMs) {
                break;
            }

            // Can we even do this? No network, etc...
            if (deep_sleep() < MinimumDeepSleepMs) {
                loginfo("fake delay");
                fk_delay(MinimumDeepSleepMs);
            }
        }

        return true;
    }
    else {
        loginfo("no workers: unable to deep sleep");
    }

    return false;
}

} // namespace fk
