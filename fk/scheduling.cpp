#include "scheduling.h"

namespace fk {

CurrentSchedules::CurrentSchedules() {
}

CurrentSchedules::CurrentSchedules(GlobalState const *gs, ModuleFactory const &module_factory) {
    readings = gs->scheduler.readings.cron;
    gps = gs->scheduler.gps.cron;
    lora = gs->scheduler.lora.cron;
    network = gs->scheduler.network.cron;
    network_jitter = gs->scheduler.network.jitter;
    service_interval = module_factory.service_interval();
}

bool CurrentSchedules::equals(CurrentSchedules const &o) const {
    return readings == o.readings && network == o.network && gps == o.gps && lora == o.lora &&
           service_interval == o.service_interval && network_jitter == o.network_jitter;
}

ReadingsTask::ReadingsTask(lwcron::CronSpec cron_spec) : lwcron::CronTask(cron_spec) {
}

void ReadingsTask::run() {
    get_ipc()->launch_worker(WorkerCategory::Readings, create_pool_worker<ReadingsWorker>(false, false, true, ModulePowerState::Unknown));
}

const char *ReadingsTask::toString() const {
    return "readings";
}

GpsTask::GpsTask(lwcron::CronSpec cron_spec) : lwcron::CronTask(cron_spec) {
}

void GpsTask::run() {
    fk_start_task_if_necessary(&gps_task);
}

const char *GpsTask::toString() const {
    return "gps";
}

LoraTask::LoraTask(lwcron::CronSpec cron_spec) : lwcron::CronTask(cron_spec) {
}

void LoraTask::run() {
    get_ipc()->launch_worker(create_pool_worker<LoraWorker>());
}

const char *LoraTask::toString() const {
    return "lora";
}

bool LoraTask::enabled() const {
    return get_lora_network()->available();
}

UploadDataTask::UploadDataTask(lwcron::CronSpec cron_spec, uint32_t jitter) : lwcron::CronTask(cron_spec, jitter) {
}

void UploadDataTask::run() {
    get_ipc()->launch_worker(create_pool_worker<UploadDataWorker>());
}

const char *UploadDataTask::toString() const {
    return "upldata";
}

SynchronizeTimeTask::SynchronizeTimeTask(uint32_t interval) : lwcron::PeriodicTask(interval) {
}

void SynchronizeTimeTask::run() {
    if (!get_network()->online()) {
        alogf(LogLevels::INFO, toString(), "offline");
    } else {
        alogf(LogLevels::INFO, toString(), "starting");
        get_network()->synchronize_time();
    }
}

const char *SynchronizeTimeTask::toString() const {
    return "synctime";
}

ServiceModulesTask::ServiceModulesTask(uint32_t interval) : lwcron::PeriodicTask(interval) {
}

void ServiceModulesTask::run() {
    get_ipc()->launch_worker(create_pool_worker<ServiceModulesWorker>());
}

const char *ServiceModulesTask::toString() const {
    return "modsvc";
}

bool ServiceModulesTask::enabled() const {
    return get_module_factory().service_interval() > 0;
}

} // namespace fk
