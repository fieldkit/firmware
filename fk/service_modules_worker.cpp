#include "service_modules_worker.h"
#include "hal/hal.h"
#include "readings_taker.h"
#include "state_manager.h"

namespace fk {

FK_DECLARE_LOGGER("modsvc");

ServiceModulesWorker::ServiceModulesWorker() {
}

void ServiceModulesWorker::run(Pool &pool) {
    auto gs = get_global_state_ro();
    auto lock = storage_mutex.acquire(UINT32_MAX);
    auto mm = get_modmux();
    auto modules_lock = mm->lock();
    auto modules_bus = get_board()->i2c_module();

    ScanningContext ctx{ mm, gs.get(), modules_bus };
    if (!get_module_factory().service(ctx, pool)) {
        logerror("error servicing modules");
    }
}

}
