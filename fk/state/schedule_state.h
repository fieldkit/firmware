#pragma once

#include <fk-app-protocol.h>
#include <lwcron/lwcron.h>

#include "common.h"
#include "config.h"

namespace fk {

struct ScheduledTime {
    uint32_t now;
    uint32_t time;
    uint32_t seconds;
};

struct Interval {
    uint32_t start{ 0 };
    uint32_t end{ 0 };
    uint32_t interval{ 0 };
};

struct Schedule {
    lwcron::CronSpec cron{};
    uint32_t interval{ 0 };
    uint32_t repeated{ 0 };
    uint32_t duration{ 0 };
    uint32_t jitter{ 0 };
    uint32_t mark{ 0 };
    Interval intervals[MaximumScheduleIntervals];
    ScheduledTime upcoming;

    Schedule &operator=(const fk_app_Schedule &s);

    void recreate();
    void simple(uint32_t interval);
};

struct SchedulerState {
    Schedule readings{};
    Schedule network{};
    Schedule gps{};
    Schedule lora{};
    Schedule backup{};
};

struct UpcomingUpdate {
    ScheduledTime readings;
    ScheduledTime network;
    ScheduledTime gps;
    ScheduledTime lora;
    ScheduledTime backup;
};

} // namespace fk
