#include <malloc.h>

#include "status_logging.h"
#include "tasks/tasks.h"
#include "hal/hal.h"
#include "clock.h"
#include "state_manager.h"
#include "state_ref.h"

namespace fk {

FK_DECLARE_LOGGER("status");

/**
 * This can go away when we're sure this holds. Basically, if we're
 * running then every task of higher priority should be in WAIT,
 * FINISHED, SUSPENDED, or PANIC... basically anything other than
 * IDLE, which would mean that it's waiting for a turn to run and
 * could possibly be in the middle of something.
 */
static void verify_tasks_priority() __attribute__((unused)) ;
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

static void log_task_info() __attribute__((unused));
static void log_task_info() {
    __disable_irq();
    for (auto iter = osg.tasks; iter != nullptr; iter = iter->np) {
        logverbose("task: %10s %-30s %" PRIu32 "us", iter->name, os_task_status_str(iter->status), iter->runtime);
    }
    __enable_irq();
}

static void log_status() {
    auto gs = try_get_global_state_ro();
    if (!gs) {
        return;
    }

    auto now = get_clock_now();
    auto mi = mallinfo();
    auto name = gs.get()->general.name;
    ip4_address ip{ gs.get()->network.ip };

    FormattedTime formatted{ now };
    loginfo("%s '%s' (%d.%d.%d.%d) (free = %" PRIu32 ", arena = %zd, used = %zd)",
            formatted.cstr(), name, ip.u.bytes[0], ip.u.bytes[1], ip.u.bytes[2], ip.u.bytes[3],
            fk_free_memory(), (size_t)mi.arena, (size_t)mi.uordblks);
}

void fk_status_log() {
    verify_tasks_priority();
    log_status();
}

}
