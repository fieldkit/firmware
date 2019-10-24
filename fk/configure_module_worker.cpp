#include <samd51_common.h>

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
        gs->physical_modules[bay] = { };

        if (which == AllModuleBays || which == bay) {
            gs->physical_modules[bay].attempted = true;
            if (!fn(bay)) {
                return;
            }
        }
    }
}

bool ConfigureModuleWorker::configure(Pool &pool) {
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
    case ConfigureModuleKind::Erase: {
        configure_bay_and_update_state(bay_, gs.get(), [&](uint8_t b) {
            loginfo("erasing: %d", b);
            return configurer.erase(b);
        });
        break;
    }
    } default: {
        break;
    }
    }

    return true;
}

bool ConfigureModuleWorker::scan(Pool &pool) {
    auto module_bus = get_board()->i2c_module();
    auto gs = get_global_state_rw();
    auto mm = get_modmux();

    mm->enable_all_modules();

    ScanningContext ctx{ mm, gs.get(), module_bus };
    Storage storage{ nullptr }; // NOTE: Not opened!
    ModuleScanning scanning{ get_modmux() };

    auto &factory = get_module_factory();

    factory.clear();

    auto constructed_modules = factory.create(scanning, ctx, pool);
    if (!constructed_modules) {
        return false;
    }

    gs.get()->update_physical_modules(*constructed_modules);

    return true;
}

void ConfigureModuleWorker::run(Pool &pool) {
    auto lock = get_board()->lock_eeprom();

    configure(pool);

    scan(pool);
}

}
