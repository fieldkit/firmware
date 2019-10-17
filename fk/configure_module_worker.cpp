#include "configure_module_worker.h"
#include "state_ref.h"
#include "storage/storage.h"
#include "hal/board.h"

#include "modules/bridge/modules.h"
#include "modules/scanning.h"
#include "modules/configure.h"

namespace fk {

FK_DECLARE_LOGGER("cfgworker");

ConfigureModuleWorker::ConfigureModuleWorker(uint8_t bay, ConfigureModuleKind kind) : bay_(bay), kind_(kind) {
}

template<typename T>
void configure_bay_and_update_state(uint8_t which, GlobalState *gs, T fn) {
    for (auto bay = 0u; bay < MaximumNumberOfPhysicalModules; ++bay) {
        if (which == AllModuleBays || which == bay) {
            gs->physical_modules[bay].attempted = true;

            if (!fn(bay)) {
                return;
            }

            gs->physical_modules[bay].configured = true;
        }
        else {
            gs->physical_modules[bay].configured = { };
        }
    }
}

void ConfigureModuleWorker::run(Pool &pool) {
    auto module_bus = get_board()->i2c_module();
    auto gs = get_global_state_rw();

    ScanningContext ctx{ get_modmux(), gs.get(), module_bus };
    Storage storage{ nullptr }; // NOTE: Not opened!
    ModuleScanning scanning{ get_modmux() };
    ModuleConfigurer configurer{ scanning };

    switch (kind_) {
    case ConfigureModuleKind::Weather: {
        configure_bay_and_update_state(bay_, gs.get(), [&](uint8_t b) {
            loginfo("configuring weather: %d", b);
            return configurer.weather(b);
        });
        break;
    }
    case ConfigureModuleKind::Water: {
        configure_bay_and_update_state(bay_, gs.get(), [&](uint8_t b) {
            loginfo("configuring water: %d", b);
            return configurer.water(b);
        });
        break;
    }
    case ConfigureModuleKind::Ultrasonic: {
        configure_bay_and_update_state(bay_, gs.get(), [&](uint8_t b) {
            loginfo("configuring ultrasonic: %d", b);
            return configurer.ultrasonic(b);
        });
        break;
    }
    default: {
        break;
    }
    }

    loginfo("checking modules");

    auto modules = get_module_factory().create(scanning, ctx, pool);
    if (!modules) {
    }
}

}
