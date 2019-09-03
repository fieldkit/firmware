#include <lwcron/lwcron.h>

#include "tasks/tasks.h"
#include "hal/hal.h"
#include "clock.h"
#include "lora_worker.h"

namespace fk {

FK_DECLARE_LOGGER("schedule");

static bool can_start_task(os_task_t *task);

static bool start_task_if_necessary(os_task_t *task);

class SchedulerTask {
};

class StartTaskFromCron : public lwcron::CronTask, public SchedulerTask {
private:
    os_task_t *task_;

public:
    StartTaskFromCron(lwcron::CronSpec spec, os_task_t *task) : lwcron::CronTask(spec), task_(task) {
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

template<typename T>
class StartWorkerFromCron : public lwcron::CronTask, public SchedulerTask {
public:
    StartWorkerFromCron(lwcron::CronSpec spec) : lwcron::CronTask(spec) {
    }

public:
    void run() override {
        auto worker = create_pool_wrapper<T, DefaultWorkerPoolSize, PoolWorker<T>>();
        if (!get_ipc()->launch_worker(worker)) {
            return;
        }
    }

public:
    const char *toString() const override {
        return "worker";
    }

};

void task_handler_scheduler(void *params) {
    lwcron::CronSpec readings_cron_spec{ lwcron::CronSpec::interval(10) };
    lwcron::CronSpec lora_cron_spec{ lwcron::CronSpec::interval(180) };

    StartTaskFromCron readings_job{ readings_cron_spec, &readings_task };
    StartWorkerFromCron<LoraWorker> lora_job{ lora_cron_spec };

    lwcron::Task *tasks[2] { &readings_job, &lora_job };
    lwcron::Scheduler scheduler{ tasks };

    scheduler.begin( get_clock_now() );

    FK_ASSERT(os_task_start(&network_task) == OSS_SUCCESS);
    FK_ASSERT(os_task_start(&gps_task) == OSS_SUCCESS);

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
