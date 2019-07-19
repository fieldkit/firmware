#include "tasks/tasks.h"

#include "hal/hal.h"

namespace fk {

FK_DECLARE_LOGGER("data");

void task_handler_data(void *params) {
    while (true) {
        void *message = nullptr;
        if (get_ipc()->dequeue_data(&message, FiveSecondsMs)) {
            // This can go away when we're sure this holds. Basically, if we'r
            // running then every task of higher priority should be in WAIT,
            // FINISHED, SUSPENDED, or PANIC... basically anything other than
            // IDLE, which would mean that it's waiting for a turn to run and
            // could possibly be in the middle of something.
            __disable_irq();
            for (auto iter = osg.tasks; iter != nullptr; iter = iter->np) {
                if (iter->priority > os_task_self()->priority) {
                    FK_ASSERT(iter->status != OS_TASK_STATUS_IDLE);
                }
            }
            __enable_irq();
        }
    }
}

}
