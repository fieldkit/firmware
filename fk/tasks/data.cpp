#include <malloc.h>

#include "tasks/tasks.h"
#include "hal/hal.h"
#include "state.h"
#include "clock.h"

namespace fk {

FK_DECLARE_LOGGER("data");

/**
 * This can go away when we're sure this holds. Basically, if we're
 * running then every task of higher priority should be in WAIT,
 * FINISHED, SUSPENDED, or PANIC... basically anything other than
 * IDLE, which would mean that it's waiting for a turn to run and
 * could possibly be in the middle of something.
 */
static void verify_tasks_priority() {
    __disable_irq();
    for (auto iter = osg.tasks; iter != nullptr; iter = iter->np) {
        if (iter->priority > os_task_self()->priority) {
            if (iter->status != OS_TASK_STATUS_FINISHED && iter->status != OS_TASK_STATUS_WAIT && iter->status != OS_TASK_STATUS_SUSPENDED && iter->status != OS_TASK_STATUS_PANIC) {
                logerror("%s has status %s", iter->name, os_task_status_str(iter->status));
            }
        }
    }
    __enable_irq();
}

static void log_status() {
    auto gs = get_global_state_ro();
    auto now = get_clock_now();
    auto mi = mallinfo();
    ip4_address ip{ gs.get()->network.ip };

    FormattedTime formatted{ now };
    loginfo("%s (%d.%d.%d.%d) (free = %" PRIu32 ", arena = %zd, used = %zd)",
            formatted.cstr(),
            ip.u.bytes[0], ip.u.bytes[1], ip.u.bytes[2], ip.u.bytes[3],
            fk_free_memory(), mi.arena, mi.uordblks);
}

void task_handler_data(void *params) {
    auto status = fk_uptime() + FiveSecondsMs;

    while (true) {
        StateChange *state_change = nullptr;
        if (get_ipc()->dequeue_data(&state_change)) {
            verify_tasks_priority();

            logtrace("from %s", state_change->source());

            auto gs = get_global_state_rw();
            state_change->apply(gs.get());
            delete state_change;
        }

        if (fk_uptime() > status) {
            log_status();
            status = fk_uptime() + FiveSecondsMs;
        }
    }
}

}
