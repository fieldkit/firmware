#pragma once

#include <lwcron/lwcron.h>

#include "hal/ipc.h"
#include "hal/lora.h"

#include "readings_worker.h"
#include "lora_worker.h"
#include "service_modules_worker.h"
#include "scan_modules_worker.h"

#include "networking/upload_data_worker.h"

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
    ReadingsTask(lwcron::CronSpec cron_spec);

public:
    void run() override;
    const char *toString() const override;

};

class GpsTask : public lwcron::CronTask, public SchedulerTask {
public:
    GpsTask(lwcron::CronSpec cron_spec);

public:
    void run() override;
    const char *toString() const override;

};

class LoraTask : public lwcron::CronTask, public SchedulerTask {
public:
    LoraTask(lwcron::CronSpec cron_spec);

public:
    void run() override;
    const char *toString() const override;
    bool enabled() const override;

};

class UploadDataTask : public lwcron::CronTask, public SchedulerTask {
public:
    UploadDataTask(lwcron::CronSpec cron_spec);

public:
    void run() override;
    const char *toString() const override;

};

class SynchronizeTimeTask : public lwcron::PeriodicTask, public SchedulerTask {
public:
    SynchronizeTimeTask(uint32_t interval);

public:
    void run() override;
    const char *toString() const override;

};

class ServiceModulesTask : public lwcron::PeriodicTask, public SchedulerTask {
public:
    ServiceModulesTask(uint32_t interval);

public:
    void run() override ;
    const char *toString() const override;
    bool enabled() const override;

};

} // namespace fk
