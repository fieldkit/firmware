#include <lwcron/lwcron.h>

#include "hal/hal.h"
#include "clock.h"
#include "readings_worker.h"
#include "lora_worker.h"
#include "tasks/tasks.h"
#include "config.h"

namespace fk {

FK_DECLARE_LOGGER("schedule");

static bool can_start_task(os_task_t *task);

static bool start_task_if_necessary(os_task_t *task);

class SchedulerTask {
};

class ReadingsTask : public lwcron::CronTask, public SchedulerTask {
public:
    ReadingsTask(lwcron::CronSpec cron_spec) : lwcron::CronTask(cron_spec) {
    }

public:
public:
    void run() override {
        auto worker = create_pool_worker<ReadingsWorker>(false);
        if (!get_ipc()->launch_worker(worker)) {
            delete worker;
            return;
        }
    }

    const char *toString() const override {
        return "readings";
    }

};

class GpsTask : public lwcron::CronTask, public SchedulerTask {
public:
    GpsTask(lwcron::CronSpec cron_spec) : lwcron::CronTask(cron_spec) {
    }

public:
    void run() override {
        start_task_if_necessary(&gps_task);
    }

    const char *toString() const override {
        return "gps";
    }

};

class LoraTask : public lwcron::CronTask, public SchedulerTask {
public:
    LoraTask(lwcron::CronSpec cron_spec) : lwcron::CronTask(cron_spec) {
    }

public:
    void run() override {
        auto worker = create_pool_worker<LoraWorker>();
        if (!get_ipc()->launch_worker(worker)) {
            delete worker;
            return;
        }
    }

    const char *toString() const override {
        return "lora";
    }

    bool enabled() const override {
        return get_lora_network()->available();
    }

};

void task_handler_scheduler(void *params) {
    lwcron::CronSpec readings_cron_spec{ lwcron::CronSpec::interval(fk_config().scheduler.readings_interval) };
    lwcron::CronSpec lora_cron_spec{ lwcron::CronSpec::interval(fk_config().scheduler.lora_interval) };
    lwcron::CronSpec gps_cron_spec{ lwcron::CronSpec::interval(fk_config().scheduler.gps_interval) };

    ReadingsTask readings_job{ readings_cron_spec };
    LoraTask lora_job{ lora_cron_spec };
    GpsTask gps_job{ gps_cron_spec };

    lwcron::Task *tasks[3] { &readings_job, &lora_job, &gps_job };
    lwcron::Scheduler scheduler{ tasks };

    scheduler.begin( get_clock_now() );

    FK_ASSERT(start_task_if_necessary(&display_task));
    FK_ASSERT(start_task_if_necessary(&network_task));
    FK_ASSERT(start_task_if_necessary(&gps_task));

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
    return status == OS_TASK_STATUS_SUSPENDED || status == OS_TASK_STATUS_FINISHED || status == OS_TASK_STATUS_PANIC || status == OS_TASK_STATUS_ABORTED;
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
