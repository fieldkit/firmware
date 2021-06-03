#include "clock.h"
#include "hal/hal.h"
#include "tasks/tasks.h"
#include "gps_service.h"

namespace fk {

FK_DECLARE_LOGGER("gps");

void task_handler_gps(void *params) {
    // TODO: This would be way better if we used an IRQ to wake this task and
    // slurp down the latest characters.
    GpsService gps_service{ get_gps() };

    if (!gps_service.begin()) {
        return;
    }

    uint32_t signal_checked = 0u;

    while (true) {
        gps_service.service();

        if (fk_task_stop_requested(&signal_checked)) {
            loginfo("stop requested");
            break;
        }

        fk_delay(10);
    }

    loginfo("gps stopped");
}

} // namespace fk
