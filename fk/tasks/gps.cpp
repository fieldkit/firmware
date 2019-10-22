#include "tasks/tasks.h"
#include "hal/hal.h"
#include "state_manager.h"
#include "clock.h"

namespace fk {

FK_DECLARE_LOGGER("gps");

void task_handler_gps(void *params) {
    auto gps = get_gps();

    if (!gps->begin()) {
        logerror("error initializing gps");
        return;
    }

    auto &fkc = fk_config();
    auto started_at = fk_uptime();
    auto status = fk_uptime() + OneMinuteMs;
    auto update_gs = fk_uptime() + FiveSecondsMs;
    auto fixed_at = 0;

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
                gs->gps.satellites = fix.satellites;
                gs->gps.hdop = fix.hdop;
            });

            update_gs = fk_uptime() + FiveSecondsMs;

            if (fix.good) {
                if (fixed_at == 0) {
                    fixed_at = fk_uptime();
                    clock_adjust(fix.time);
                }
                else if (fk_uptime() - fixed_at > fkc.scheduler.fix_hold) {
                    loginfo("gps fix hold reached: %" PRIu32, fkc.scheduler.fix_hold);
                    break;
                }
            }
            else {
                if (fixed_at > 0) {
                    fixed_at = 0;
                }
            }
        }

        if (fk_uptime() - started_at > fkc.scheduler.fix_waiting) {
            loginfo("gps fix waiting reached: %" PRIu32, fkc.scheduler.fix_waiting);
            break;
        }
    }

    FK_ASSERT(gps->stop());

    gsm.apply([=](GlobalState *gs) {
        gs->gps.enabled = false;
        gs->gps.fix = false;
    });

    loginfo("gps stopped");
}

}
