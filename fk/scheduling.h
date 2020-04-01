#pragma once

#include <lwcron/lwcron.h>

#include "hal/ipc.h"
#include "hal/lora.h"

#include "readings_worker.h"
#include "lora_worker.h"

#include "modules/service_modules_worker.h"
#include "modules/scan_modules_worker.h"
#include "modules/module_factory.h"

#include "networking/upload_data_worker.h"

#include "tasks/tasks.h"

namespace fk {

struct CurrentSchedules {
    lwcron::CronSpec readings;
    lwcron::CronSpec network;
    lwcron::CronSpec gps;
    lwcron::CronSpec lora;
    uint32_t service_interval;
    uint32_t network_jitter;

    CurrentSchedules();
    CurrentSchedules(GlobalState const *gs, ModuleFactory const &module_factory);

    bool equals(CurrentSchedules const &o) const;
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
    UploadDataTask(lwcron::CronSpec cron_spec, uint32_t jitter);

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
