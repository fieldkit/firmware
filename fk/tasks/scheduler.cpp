#include <lwcron/lwcron.h>

#include "tasks/tasks.h"
#include "hal/hal.h"
#include "clock.h"

namespace fk {

FK_DECLARE_LOGGER("schedule");

static bool can_start_task(os_task_t *task);

static bool start_task_if_necessary(os_task_t *task);

class SchedulerTask {
};

class CronTask : public lwcron::CronTask, public SchedulerTask {
private:
    os_task_t *task_;

public:
    CronTask(lwcron::CronSpec spec, os_task_t *task) : lwcron::CronTask(spec), task_(task) {
    }

public:
    void run() override {
        start_task_if_necessary(task_);
    }

public:
    const char *toString() const override {
        return task_->name;
    }

};

void task_handler_scheduler(void *params) {
    lwcron::CronSpec readings_cron_spec{ lwcron::CronSpec::interval(10) };

    CronTask readings_job{ readings_cron_spec, &readings_task };

    lwcron::Task *tasks[1] { &readings_job };
    lwcron::Scheduler scheduler{ tasks };

    scheduler.begin( get_clock_now() );

    auto check_time = fk_uptime() + OneSecondMs;

    while (true) {
        // This throttles this loop, so we take a pass when we dequeue or timeout.
        Activity *activity = nullptr;
        if (get_ipc()->dequeue_activity(&activity)) {
            start_task_if_necessary(&display_task);
        }

        if (check_time < fk_uptime()) {
            auto time = lwcron::DateTime{ get_clock_now() };
            scheduler.check(time);
            check_time = fk_uptime() + OneSecondMs;
        }
    }
}

static bool can_start_task(os_task_t *task) {
    auto status = os_task_get_status(task);
    return status == OS_TASK_STATUS_SUSPENDED || status == OS_TASK_STATUS_FINISHED;
}

static bool start_task_if_necessary(os_task_t *task) {
    if (!can_start_task(task)) {
        return false;
    }

    loginfo("starting task '%s'", task->name);
    os_task_start(task);
    return true;
}

}
