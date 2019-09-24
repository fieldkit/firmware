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

template<typename T>
class LambdaSchedulerTask : public lwcron::CronTask, public SchedulerTask {
private:
    const char *label_;
    T fn_;

public:
    LambdaSchedulerTask(lwcron::CronSpec spec, const char *label, T fn) : lwcron::CronTask(spec), label_(label), fn_(fn) {
    }

public:
    void run() override {
        fn_();
    }

    const char *toString() const override {
        return label_;
    }

};

template<typename T>
LambdaSchedulerTask<T> to_lambda_task(lwcron::CronSpec spec, const char *label, T fn) {
    return LambdaSchedulerTask<T>(spec, label, fn);
}

void task_handler_scheduler(void *params) {
    lwcron::CronSpec readings_cron_spec{ lwcron::CronSpec::interval(fk_config().scheduler.readings_interval) };
    lwcron::CronSpec lora_cron_spec{ lwcron::CronSpec::interval(fk_config().scheduler.lora_interval) };
    lwcron::CronSpec gps_cron_spec{ lwcron::CronSpec::interval(fk_config().scheduler.gps_interval) };

    auto readings_job = to_lambda_task(readings_cron_spec, "readings", []() {
        auto worker = create_pool_wrapper<ReadingsWorker, DefaultWorkerPoolSize, PoolWorker<ReadingsWorker>>(false);
        if (!get_ipc()->launch_worker(worker)) {
            delete worker;
            return;
        }
    });
    auto lora_job = to_lambda_task(lora_cron_spec, "lora", []() {
        auto worker = create_pool_wrapper<LoraWorker, DefaultWorkerPoolSize, PoolWorker<LoraWorker>>();
        if (!get_ipc()->launch_worker(worker)) {
            delete worker;
            return;
        }
    });
    auto gps_job = to_lambda_task(lora_cron_spec, "gps", []() {
        start_task_if_necessary(&gps_task);
    });

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
