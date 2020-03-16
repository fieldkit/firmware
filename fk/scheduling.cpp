#include "scheduling.h"

namespace fk {

CurrentSchedules::CurrentSchedules(GlobalState const *gs, ModuleFactory const &module_factory) {
    readings = gs->scheduler.readings.cron;
    network = gs->scheduler.network.cron;
    gps = gs->scheduler.gps.cron;
    lora = gs->scheduler.lora.cron;
    service_interval = module_factory.service_interval();
}

void SynchronizeTimeTask::run() {
    if (!get_network()->online()) {
        alogf(LogLevels::INFO, toString(), "offline");
        return;
    }

    alogf(LogLevels::INFO, toString(), "starting");

    get_network()->synchronize_time();
}

} // namespace fk
