#include "scheduling.h"

#include "storage/backup_worker.h"

namespace fk {

CurrentSchedules::CurrentSchedules() {
}

CurrentSchedules::CurrentSchedules(GlobalState const *gs) {
    readings = gs->scheduler.readings.cron;
    gps = gs->scheduler.gps.cron;
    lora = gs->scheduler.lora.cron;
    network = gs->scheduler.network.cron;
    network_jitter = gs->scheduler.network.jitter;
    backup = gs->scheduler.backup.cron;
    service_interval = 0; // module_factory.service_interval();
}

bool CurrentSchedules::equals(CurrentSchedules const &o) const {
    return readings == o.readings && network == o.network && gps == o.gps && lora == o.lora && service_interval == o.service_interval &&
           network_jitter == o.network_jitter;
}

ReadingsTask::ReadingsTask(lwcron::CronSpec cron_spec) : lwcron::CronTask(cron_spec) {
}

void ReadingsTask::run() {
    get_ipc()->launch_worker(WorkerCategory::Readings, create_pool_worker<ReadingsWorker>(false, false, true, ModulePowerState::Unknown));
}

const char *ReadingsTask::toString() const {
    return "readings";
}

GpsTask::GpsTask(lwcron::CronSpec cron_spec, GpsService &gps_service) : lwcron::CronTask(cron_spec), gps_service_(gps_service) {
}

void GpsTask::run() {
    if (!gps_service_.is_running()) {
        gps_service_.begin();
    }
}

const char *GpsTask::toString() const {
    return "gps";
}

LoraTask::LoraTask(lwcron::CronSpec cron_spec, LoraWorkOperation op) : lwcron::CronTask(cron_spec), op_(op) {
}

void LoraTask::run() {
    get_ipc()->launch_worker(create_pool_worker<LoraWorker>(LoraWork{ op_ }));
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
    return false; // get_module_factory().service_interval() > 0;
}

BackupTask::BackupTask(lwcron::CronSpec cron_spec) : lwcron::CronTask(cron_spec) {
}

void BackupTask::run() {
    get_ipc()->launch_worker(WorkerCategory::Transfer, create_pool_worker<BackupWorker>());
}

const char *BackupTask::toString() const {
    return "backup";
}

} // namespace fk
