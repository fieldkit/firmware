#pragma once

#include <lwcron/lwcron.h>

#include "hal/ipc.h"
#include "hal/lora.h"

#include "readings_worker.h"
#include "upload_data_worker.h"
#include "lora_worker.h"
#include "service_modules_worker.h"
#include "scan_modules_worker.h"

#include "tasks/tasks.h"

namespace fk {

struct CurrentSchedules {
    lwcron::CronSpec readings;
    lwcron::CronSpec network;
    lwcron::CronSpec gps;
    lwcron::CronSpec lora;
    uint32_t service_interval;

    CurrentSchedules() {
    }

    CurrentSchedules(GlobalState const *gs, ModuleFactory const &module_factory);

    bool equals(CurrentSchedules const &o) const {
        return readings == o.readings && network == o.network && gps == o.gps && lora == o.lora && service_interval == o.service_interval;
    }
};

class SchedulerTask {
};

class ReadingsTask : public lwcron::CronTask, public SchedulerTask {
public:
    ReadingsTask(lwcron::CronSpec cron_spec) : lwcron::CronTask(cron_spec) {
    }

public:
public:
    void run() override {
        get_ipc()->launch_worker(create_pool_worker<ReadingsWorker>(false, false));
    }

    const char *toString() const override {
        return "readings";
    }

};

class UploadDataTask : public lwcron::CronTask, public SchedulerTask {
public:
    UploadDataTask(lwcron::CronSpec cron_spec) : lwcron::CronTask(cron_spec) {
    }

public:
public:
    void run() override {
        get_ipc()->launch_worker(create_pool_worker<UploadDataWorker>());
    }

    const char *toString() const override {
        return "upldata";
    }

};

class GpsTask : public lwcron::CronTask, public SchedulerTask {
public:
    GpsTask(lwcron::CronSpec cron_spec) : lwcron::CronTask(cron_spec) {
    }

public:
    void run() override {
        fk_start_task_if_necessary(&gps_task);
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
        get_ipc()->launch_worker(worker);
    }

    const char *toString() const override {
        return "lora";
    }

    bool enabled() const override {
        return get_lora_network()->available();
    }

};

class ServiceModulesTask : public lwcron::PeriodicTask, public SchedulerTask {
public:
    ServiceModulesTask(uint32_t interval) : lwcron::PeriodicTask(interval) {
    }

public:
    void run() override {
        auto worker = create_pool_worker<ServiceModulesWorker>();
        get_ipc()->launch_worker(worker);
    }

    const char *toString() const override {
        return "modsvc";
    }

    bool enabled() const override {
        return get_module_factory().service_interval() > 0;
    }

};

} // namespace fk
