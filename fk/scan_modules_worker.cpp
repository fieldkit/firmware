#include "scan_modules_worker.h"
#include "modules/module_factory.h"
#include "modules/scanning.h"

namespace fk {

FK_DECLARE_LOGGER("modscan");

ScanModulesWorker::ScanModulesWorker() {
}

void ScanModulesWorker::run(Pool &pool) {
    /*
    ModuleScanning scanning{ get_modmux() };
    auto modules = get_module_factory().rescan(scanning, pool);
    if (!modules) {
        logerror("error resolving modules");
        return;
    }

    for (auto &module : *modules) {
        loginfo("module: %s", module.meta->name);
    }
    */
}

} // namespace fk
