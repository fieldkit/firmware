#include <samd51_common.h>

#include "hal/board.h"
#include "hal/ipc.h"
#include "state_manager.h"
#include "state_ref.h"

#include "modules/bridge/modules.h"
#include "modules/configure.h"
#include "modules/configure_module_worker.h"
#include "modules/enable_module_power.h"
#include "modules/scanning.h"
#include "poll_sensors_worker.h"

namespace fk {

FK_DECLARE_LOGGER("cfgworker");

ConfigureModuleWorker::ConfigureModuleWorker(ModulePosition bay) : bay_(bay), erase_(true) {
}

ConfigureModuleWorker::ConfigureModuleWorker(ModulePosition bay, ModuleHeader header) : bay_(bay), header_(header) {
}

template <typename T> void configure_bay_and_update_state(ModMux *mm, ModulePosition which, GlobalState *gs, T fn) {
    for (auto bay = 0u; bay < MaximumNumberOfPhysicalModules; ++bay) {
        if (which == ModulePosition::All || which == ModulePosition::from(bay)) {
            if (!fn(ModulePosition::from(bay))) {
                if (which != ModulePosition::All) {
                    return;
                }
            }
        }
    }
}

bool ConfigureModuleWorker::configure(Pool &pool) {
    auto module_bus = get_board()->i2c_module();
    auto gs = get_global_state_rw();
    auto mm = get_modmux();

    EnableModulePower module_power{ bay_, ModulePower::Always, MinimumModuleStartupDelayMs };
    if (!module_power.enable()) {
        return false;
    }

    ScanningContext ctx{ get_modmux(), gs.get()->location(pool), module_bus, pool };
    ModuleScanning scanning{ get_modmux() };
    ModuleConfigurer configurer{ scanning };

    configure_bay_and_update_state(mm, bay_, gs.get(), [&](ModulePosition b) {
        if (erase_) {
            loginfo("erasing: %d", b.integer());
            return configurer.erase(b);
        } else {
            loginfo("provisioning: %d", b.integer());
            return configurer.provision(b, header_);
        }
    });

    return true;
}

void ConfigureModuleWorker::run(Pool &pool) {
    auto lock = get_modmux()->lock();

    configure(pool);

    auto worker = create_pool_worker<PollSensorsWorker>(true, false, true, ThirtySecondsMs);
    get_ipc()->signal_workers(WorkerCategory::Polling, 9);
    get_ipc()->launch_worker(WorkerCategory::Polling, worker, true);

    GlobalStateManager gsm;
    gsm.apply([=](GlobalState *gs) {
        gs->display.open_menu.time = fk_uptime();
        gs->display.open_menu.readings = true;
    });
}

} // namespace fk
