#include "tasks/tasks.h"
#include "hal/hal.h"

namespace fk {

FK_DECLARE_LOGGER("gps");

void task_handler_gps(void *params) {
    auto gps_serial = get_board()->gps_serial();
    auto gps = get_gps();

    FK_ASSERT(gps->begin());

    auto status = fk_uptime() + OneMinuteMs;

    // TODO: This would be way better if we used an IRQ to wake this task and
    // slurp down the latest characters.
    while (true) {
        GpsFix fix;
        FK_ASSERT(gps->service(fix));
        fk_delay(10);

        if (fk_uptime() > status) {
            loginfo("satellites(%d) time(%d) location(%f, %f) statistics(%dchrs, %d/%d)",
                    fix.satellites, fix.time, fix.longitude, fix.latitude,
                    fix.chars, fix.good, fix.failed);
            status = fk_uptime() + OneMinuteMs;
        }
    }
}

}
