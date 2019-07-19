#include "tasks/tasks.h"

#include "hal/hal.h"

namespace fk {

FK_DECLARE_LOGGER("data");

void task_handler_data(void *params) {
    while (true) {
        void *message = nullptr;
        if (get_ipc()->dequeue_data(&message, FiveSecondsMs)) {
            loginfo("incoming 0x%p", message);
        }
    }
}

}
