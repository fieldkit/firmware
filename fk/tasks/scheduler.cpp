#include "tasks/tasks.h"
#include "hal/hal.h"
#include "clock.h"

namespace fk {

FK_DECLARE_LOGGER("schededule");

static bool can_start_task(os_task_t *task) {
    auto status = os_task_get_status(task);
    return status == OS_TASK_STATUS_SUSPENDED || status == OS_TASK_STATUS_FINISHED;
}

static bool start_task_if_necessary(os_task_t *task) {
    if (!can_start_task(task)) {
        return false;
    }

    loginfo("starting task '%s'", task->name);
    os_task_start(task);
    return true;
}

void task_handler_scheduler(void *params) {
    auto readings_time = fk_uptime() + ThirtySecondsMs;
    auto misc_time = fk_uptime() + FiveMinutesMs;

    while (true) {
        // This throttles this loop, so we take a pass when we dequeue or timeout.
        Activity *activity = nullptr;
        if (get_ipc()->dequeue_activity(&activity)) {
            start_task_if_necessary(&network_task);
            start_task_if_necessary(&display_task);
        }

        if (readings_time < fk_uptime()) {
            start_task_if_necessary(&readings_task);
            readings_time = fk_uptime() + ThirtySecondsMs;
        }

        if (misc_time < fk_uptime()) {
            start_task_if_necessary(&misc_task);
            misc_time = fk_uptime() + FiveMinutesMs;
        }
    }
}

}
