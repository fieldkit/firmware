#include "tasks/tasks.h"
#include "hal/hal.h"
#include "state_manager.h"

namespace fk {

FK_DECLARE_LOGGER("gps");

void task_handler_gps(void *params) {
    auto gps_serial = get_board()->gps_serial();
    auto gps = get_gps();

    FK_ASSERT(gps->begin());

    auto status = fk_uptime() + OneMinuteMs;
    auto update_gs = fk_uptime() + FiveSecondsMs;

    GlobalStateManager gsm;
    gsm.apply([=](GlobalState *gs) {
        gs->gps = { };
        gs->gps.enabled = true;
    });

    // TODO: This would be way better if we used an IRQ to wake this task and
    // slurp down the latest characters.
    while (true) {
        GpsFix fix;
        FK_ASSERT(gps->service(fix));
        fk_delay(10);

        if (fk_uptime() > status) {
            loginfo("satellites(%d) time(%" PRIu32 ") location(%f, %f) statistics(%" PRIu32 "chrs, %d/%d)",
                    fix.satellites, fix.time, fix.longitude, fix.latitude,
                    fix.chars, fix.good, fix.failed);
            status = fk_uptime() + OneMinuteMs;
        }

        if (fk_uptime() > update_gs) {
            gsm.apply([=](GlobalState *gs) {
                gs->gps.enabled = true;
                gs->gps.fix = fix.good;
                gs->gps.longitude = fix.longitude;
                gs->gps.latitude = fix.latitude;
                gs->gps.altitude = fix.altitude;
            });
            update_gs = fk_uptime() + FiveSecondsMs;
        }
    }

    gsm.apply([=](GlobalState *gs) {
        gs->gps.enabled = false;
        gs->gps.fix = false;
    });
}

}
