#include "scan_modules_worker.h"
#include "modules/module_factory.h"
#include "modules/scanning.h"
#include "state_manager.h"

namespace fk {

FK_DECLARE_LOGGER("modscan");

ScanModulesWorker::ScanModulesWorker() {
}

void ScanModulesWorker::run(Pool &pool) {
    ModuleScanning scanning{ get_modmux() };
    auto maybe_modules = get_module_factory().rescan(scanning, pool);
    if (!maybe_modules) {
        logerror("error resolving modules");
        return;
    }

    for (auto &module : *maybe_modules) {
        loginfo("module: %s", module.meta->name);
    }

    GlobalStateManager gsm;
    gsm.apply([&](GlobalState *gs) {
        gs->update_physical_modules(*maybe_modules);
    });
}

} // namespace fk
