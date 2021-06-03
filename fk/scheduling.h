#pragma once

#include <lwcron/lwcron.h>

#include "hal/ipc.h"
#include "hal/lora.h"

#include "readings_worker.h"
#include "lora_worker.h"

#include "modules/service_modules_worker.h"
#include "modules/scan_modules_worker.h"

#include "networking/upload_data_worker.h"

#include "tasks/tasks.h"

#include "gps_service.h"

namespace fk {

struct CurrentSchedules {
    lwcron::CronSpec readings;
    lwcron::CronSpec network;
    lwcron::CronSpec gps;
    lwcron::CronSpec lora;
    uint32_t service_interval;
    uint32_t network_jitter;

    CurrentSchedules();
    CurrentSchedules(GlobalState const *gs);

    bool equals(CurrentSchedules const &o) const;
};

class SchedulerTask {
};

class ReadingsTask : public lwcron::CronTask, public SchedulerTask {
public:
    explicit ReadingsTask(lwcron::CronSpec cron_spec);

public:
    void run() override;
    const char *toString() const override;

};

class GpsTask : public lwcron::CronTask, public SchedulerTask {
private:
    GpsService &gps_service_;

public:
    explicit GpsTask(lwcron::CronSpec cron_spec, GpsService &gps_service);

public:
    void run() override;
    const char *toString() const override;

};

class LoraTask : public lwcron::CronTask, public SchedulerTask {
public:
    explicit LoraTask(lwcron::CronSpec cron_spec);

public:
    void run() override;
    const char *toString() const override;
    bool enabled() const override;

};

class UploadDataTask : public lwcron::CronTask, public SchedulerTask {
public:
    explicit UploadDataTask(lwcron::CronSpec cron_spec, uint32_t jitter);

public:
    void run() override;
    const char *toString() const override;

};

class SynchronizeTimeTask : public lwcron::PeriodicTask, public SchedulerTask {
public:
    explicit SynchronizeTimeTask(uint32_t interval);

public:
    void run() override;
    const char *toString() const override;

};

class ServiceModulesTask : public lwcron::PeriodicTask, public SchedulerTask {
public:
    explicit ServiceModulesTask(uint32_t interval);

public:
    void run() override ;
    const char *toString() const override;
    bool enabled() const override;

};

} // namespace fk
