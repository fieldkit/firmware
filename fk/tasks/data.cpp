#include "tasks/tasks.h"
#include "hal/hal.h"
#include "state.h"

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
            if (iter->status != OS_TASK_STATUS_FINISHED && iter->status != OS_TASK_STATUS_WAIT && iter->status != OS_TASK_STATUS_SUSPENDED) {
                logerror("%s has status %s", iter->name, os_task_status_str(iter->status));
            }
        }
    }
    __enable_irq();
}

void task_handler_data(void *params) {
    while (true) {
        StateChange *state_change = nullptr;
        if (get_ipc()->dequeue_data(&state_change, FiveSecondsMs)) {
            verify_tasks_priority();

            loginfo("updating 0x%p", state_change);

            auto gs = get_global_state_rw();
            state_change->apply(gs.get());
            delete state_change;
        }
    }
}

}
