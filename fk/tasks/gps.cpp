#include "tasks/tasks.h"
#include "hal/hal.h"
#include "state_manager.h"
#include "clock.h"
#include "timer.h"

namespace fk {

FK_DECLARE_LOGGER("gps");

void task_handler_gps(void *params) {
    auto gps = get_gps();

    if (!gps->begin()) {
        logerror("error initializing gps");
        return;
    }

    IntervalTimer status_timer;
    IntervalTimer update_timer;
    uint32_t started_at = fk_uptime();
    uint32_t fixed_at = 0u;
    uint32_t signal_checked = 0u;

    GlobalStateManager gsm;
    gsm.apply([=](GlobalState *gs) {
        gs->gps = { };
        gs->gps.enabled = true;
    });

    // TODO: This would be way better if we used an IRQ to wake this task and
    // slurp down the latest characters.
    while (true) {
        auto log_status = false;

        GpsFix fix;
        FK_ASSERT(gps->service(fix));

        fk_delay(10);

        if (fix.chars > 0) {
            if (update_timer.expired(FiveSecondsMs)) {
                if (fix.valid) {
                    // We only update our memorized fix/location if we
                    // have a valid fix. This way any previous loaded,
                    // unfixed location can be linked to records.
                    gsm.apply([=](GlobalState *gs) {
                        gs->gps.enabled = true;
                        gs->gps.fix = true;
                        gs->gps.longitude = fix.longitude;
                        gs->gps.latitude = fix.latitude;
                        gs->gps.altitude = fix.altitude;
                        gs->gps.satellites = fix.satellites;
                        gs->gps.hdop = fix.hdop;
                        gs->gps.chars = fix.chars;
                    });

                    if (fixed_at == 0) {
                        fixed_at = fk_uptime();
                    }

                    FK_ASSERT(fix.time > 0);

                    clock_adjust(fix.time);

                    log_status = true;
                }
                else {
                    gsm.apply([=](GlobalState *gs) {
                        gs->gps.enabled = true;
                        gs->gps.fix = false;
                        gs->gps.satellites = fix.satellites;
                        gs->gps.chars = fix.chars;
                    });

                    if (fixed_at > 0) {
                        fixed_at = 0;
                    }
                }
            }
        }

        if (status_timer.expired(OneMinuteMs) || log_status) {
            loginfo("satellites(%d) time(%" PRIu32 ") location(%f, %f) statistics(%" PRIu32 "chrs, %d/%d)",
                    fix.satellites, fix.time, fix.longitude, fix.latitude,
                    fix.chars, fix.good, fix.failed);

            auto gs = get_global_state_ro();
            auto duration = gs.get()->scheduler.gps.duration;
            if (duration < UINT32_MAX) {
                auto elapsed = (fk_uptime() - started_at) / 1000;
                if (elapsed > duration) {
                    loginfo("gps fix waiting reached: %" PRIu32, duration);
                    break;
                }

                if (fixed_at > 0 && fk_uptime() - fixed_at > OneMinuteMs) {
                    loginfo("gps fix hold reached: %" PRIu32, OneMinuteMs);
                    break;
                }
            }
        }

        if (fk_task_stop_requested(&signal_checked)) {
            loginfo("stop requested");
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
