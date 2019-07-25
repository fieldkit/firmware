#include "tasks/tasks.h"

#include "hal/hal.h"
#include "clock.h"

namespace fk {

FK_DECLARE_LOGGER("schededule");

void task_handler_scheduler(void *params) {
    auto last_readings = fk_uptime();

    while (true) {
        // This throttles this loop, so we take a pass when we dequeue or timeout.
        void *message = nullptr;
        if (get_ipc()->dequeue(&message, FiveSecondsMs)) {
            auto status = os_task_get_status(&network_task);
            if (status == OS_TASK_STATUS_SUSPENDED || status == OS_TASK_STATUS_FINISHED) {
                loginfo("starting task '%s'", network_task.name);
                os_task_start(&network_task);
            }
        }

        auto reading = get_battery_gauge()->get();
        if (reading.available) {
            loginfo("battery(%dmv %d%% %dC %fs %fs)", reading.cellv, reading.soc, reading.temp, reading.tte, reading.ttf);
        }

        void *outgoing = (void *)0xdeadbeef;
        FK_ASSERT(get_ipc()->enqueue_data(outgoing, 250));

        if (fk_uptime() - last_readings > ThirtySecondsMs) {
            auto status = os_task_get_status(&readings_task);
            if (status == OS_TASK_STATUS_SUSPENDED || status == OS_TASK_STATUS_FINISHED) {
                loginfo("starting task '%s'", readings_task.name);
                os_task_start(&readings_task);
            }
            else {
                loginfo("task is still busy %s", readings_task.name);
            }
            last_readings = fk_uptime();
        }
    }
}

}
