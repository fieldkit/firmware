#include "scan_modules_worker.h"
#include "modules/module_factory.h"
#include "modules/scanning.h"
#include "state_manager.h"

namespace fk {

FK_DECLARE_LOGGER("modscan");

ScanModulesWorker::ScanModulesWorker() {
}

static void scan_modules(Pool &pool) {
    ModuleScanning scanning{ get_modmux() };
    auto maybe_modules = get_module_factory().rescan(scanning, pool);
    if (!maybe_modules) {
        logerror("error resolving modules");
        return;
    }

    GlobalStateManager gsm;
    gsm.apply([&](GlobalState *gs) {
        gs->update_physical_modules(*maybe_modules);
    });
}

static void initialize_modules(Pool &pool) {
    auto gs = get_global_state_ro();
    auto module_bus = get_board()->i2c_module();
    ScanningContext ctx{ get_modmux(), gs.get(), module_bus };
    if (!get_module_factory().initialize(ctx, pool)) {
        logerror("error initializing");
    }
}

void ScanModulesWorker::run(Pool &pool) {
    auto lock = get_modmux()->lock();

    scan_modules(pool);

    initialize_modules(pool);

    GlobalStateManager gsm;
    gsm.apply([&](GlobalState *gs) {
        auto modules = get_module_factory().modules();
        gs->update_physical_modules(modules);
    });
}

} // namespace fk
