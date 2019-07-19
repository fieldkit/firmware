#include "tasks/tasks.h"

#include "hal/hal.h"

namespace fk {

FK_DECLARE_LOGGER("gps");

void task_handler_gps(void *params) {
    auto gps = get_gps();

    if (!gps->begin()) {
        logerror("gps failed");
        return;
    }

    while (true) {
        GpsFix fix;
        gps->service(fix);
        fk_delay(10);
    }
}

}
