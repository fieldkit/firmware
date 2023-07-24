#pragma once

#include "hal/hal.h"
#include "timer.h"
#include "runnable.h"

namespace fk {

class GpsService : public Runnable {
private:
    Gps *gps_{ nullptr };
    IntervalTimer status_timer_{ ThirtySecondsMs };
    IntervalTimer update_timer_{ FiveSecondsMs };
    uint32_t started_at_{ fk_uptime() };
    uint32_t fixed_at_{ 0u };
    uint32_t adjusted_at_{ 0u };
    bool running_{ false };
    bool first_fix_{ false };

public:
    GpsService(Gps *gps);
    virtual ~GpsService();

public:
    bool is_running() const override {
        return running_;
    }

public:
    bool begin();
    bool service();
    bool stop();
    bool first_fix() const {
        return first_fix_;
    }
};

} // namespace fk
