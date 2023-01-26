#include "gps_service.h"
#include "state_manager.h"
#include "hal/clock.h"

namespace fk {

FK_DECLARE_LOGGER("gps");

static uint32_t get_gps_duration() {
    auto gs = get_global_state_ro();
    return gs.get()->scheduler.gps.duration;
}

GpsService::GpsService(Gps *gps) : gps_(gps) {
}

GpsService::~GpsService() {
    stop();
}

bool GpsService::begin() {
    if (running_) {
        logerror("already running");
        return false;
    }

    if (!gps_->begin()) {
        logerror("initializing");
        return false;
    }

    loginfo("begin");

    running_ = true;
    status_timer_ = IntervalTimer{ ThirtySecondsMs };
    update_timer_ = IntervalTimer{ FiveSecondsMs };
    started_at_ = fk_uptime();
    fixed_at_ = 0u;
    adjusted_at_ = 0u;

    GlobalStateManager gsm;
    gsm.apply([=](GlobalState *gs) {
        gs->gps = {};
        gs->gps.enabled = true;
    });

    return true;
}

bool GpsService::service() {
    if (!running_) {
        return true;
    }

    if (first_fix_) {
        first_fix_ = false;
    }

    GpsFix fix;
    FK_ASSERT(gps_->service(fix));

    GlobalStateManager gsm;

    bool log_status = false;
    if (fix.chars > 0) {
        if (update_timer_.expired()) {
            if (fix.valid) {
                // We only update our memorized fix/location if we
                // have a valid fix. This way any previous loaded,
                // unfixed location can be linked to records.
                gsm.apply([=](GlobalState *gs) {
                    gs->gps.enabled = true;
                    gs->gps.fix = true;
                    gs->gps.time = fix.time;
                    gs->gps.longitude = fix.longitude;
                    gs->gps.latitude = fix.latitude;
                    gs->gps.altitude = fix.altitude;
                    gs->gps.satellites = fix.satellites;
                    gs->gps.hdop = fix.hdop;
                    gs->gps.chars = fix.chars;
                    if (gs->gps.fixed_at == 0) {
                        gs->gps.fixed_at = fk_uptime();
                    }
                });

                if (fixed_at_ == 0) {
                    fixed_at_ = fk_uptime();
                    first_fix_ = true;
                }

                FK_ASSERT(fix.time > 0);

                if (adjusted_at_ != fix.time) {
                    clock_adjust(fix.time);
                    adjusted_at_ = fix.time;
                }

                log_status = true;
            } else {
                gsm.apply([=](GlobalState *gs) {
                    gs->gps.enabled = true;
                    gs->gps.fix = false;
                    gs->gps.satellites = fix.satellites;
                    gs->gps.chars = fix.chars;
                });

                if (fixed_at_ > 0) {
                    fixed_at_ = 0;
                }
            }
        }
    }

    if (status_timer_.expired() || log_status) {
        loginfo("satellites(%d) time(%" PRIu32 ") location(%f, %f) statistics(%" PRIu32 "chrs, %d/%d)", fix.satellites, fix.time,
                fix.longitude, fix.latitude, fix.chars, fix.good, fix.failed);

        auto duration = get_gps_duration();
        if (duration < UINT32_MAX) {
            auto elapsed = (fk_uptime() - started_at_) / 1000;
            if (elapsed > duration) {
                loginfo("gps fix waiting reached: %" PRIu32, duration);
                stop();
            }

            if (fixed_at_ > 0 && fk_uptime() - fixed_at_ > OneMinuteMs) {
                loginfo("gps fix hold reached: %" PRIu32, OneMinuteMs);
                stop();
            }
        }
    }

    return true;
}

bool GpsService::stop() {
    if (!running_) {
        return true;
    }

    FK_ASSERT(gps_->stop());

    GlobalStateManager gsm;
    gsm.apply([=](GlobalState *gs) {
        gs->gps.enabled = false;
        gs->gps.fix = false;
    });

    running_ = false;

    return true;
}

} // namespace fk
