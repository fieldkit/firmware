#include "modules/scan_modules_worker.h"
#include "modules/module_factory.h"
#include "modules/scanning.h"
#include "state_manager.h"

namespace fk {

FK_DECLARE_LOGGER("modscan");

ScanModulesWorker::ScanModulesWorker() {
}

static bool scan_modules(Pool &pool) {
    auto topology = get_modmux()->read_topology_register();
    if (!topology) {
        logwarn("unable to read topology");
    }
    else {
        loginfo("topology: [%s]", topology->string());
    }

    ModuleScanning scanning{ get_modmux() };
    auto modules_maybe = get_module_factory().rescan(scanning, pool);
    if (!modules_maybe) {
        logerror("error resolving modules");
        return false;
    }

    return true;
}

static bool initialize_modules(Pool &pool) {
    auto gs = get_global_state_ro();
    auto module_bus = get_board()->i2c_module();
    ScanningContext ctx{ get_modmux(), gs.get(), module_bus, pool };
    if (!get_module_factory().initialize(ctx, pool)) {
        return false;
    }

    return true;
}

static void update_modules(Pool &pool) {
    GlobalStateManager gsm;
    gsm.apply([&](GlobalState *gs) {
        auto modules = get_module_factory().modules();
        gs->update_physical_modules(modules);
    });
}

void ScanModulesWorker::run(Pool &pool) {
    auto lock = get_modmux()->lock();

    scan_modules(pool);

    update_modules(pool);

    initialize_modules(pool);

    update_modules(pool);
}

} // namespace fk
