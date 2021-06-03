#pragma once

#include "hal/hal.h"
#include "timer.h"

namespace fk {

class GpsService {
private:
    Gps *gps_{ nullptr };
    IntervalTimer status_timer_{ ThirtySecondsMs };
    IntervalTimer update_timer_{ FiveSecondsMs };
    uint32_t started_at_ = fk_uptime();
    uint32_t fixed_at_ = 0u;
    bool stopped_{ true };

public:
    GpsService(Gps *gps);
    virtual ~GpsService();

public:
    bool begin();
    bool service();
    bool stop();
};

} // namespace fk
