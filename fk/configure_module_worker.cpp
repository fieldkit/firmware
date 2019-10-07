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

void ConfigureModuleWorker::run(Pool &pool) {
    auto module_bus = get_board()->i2c_module();
    auto gs = get_global_state_ro();

    ScanningContext ctx{ get_modmux(), gs.get(), module_bus };
    Storage storage{ nullptr }; // NOTE: Not opened!
    ModuleScanning scanning{ get_modmux() };
    ModuleConfigurer configurer{ scanning };

    switch (kind_) {
    case ConfigureModuleKind::Weather: {
        if (bay_ == AllModuleBays) {
            for (auto bay = 0; bay < 4; ++bay) {
                loginfo("configuring weather: %d", bay);
                FK_ASSERT(configurer.weather(bay));
            }
        }
        else {
            loginfo("configuring weather: %d", bay_);
            FK_ASSERT(configurer.weather(bay_));
        }
        break;
    }
    case ConfigureModuleKind::Water: {
        if (bay_ == AllModuleBays) {
            for (auto bay = 0u; bay < 4u; ++bay) {
                loginfo("configuring water: %d", bay);
                FK_ASSERT(configurer.water(bay));
            }
        }
        else {
            loginfo("configuring water: %d", bay_);
            FK_ASSERT(configurer.water(bay_));
        }
        break;
    }
    case ConfigureModuleKind::Ultrasonic: {
        if (bay_ == AllModuleBays) {
            for (auto bay = 0; bay < 4; ++bay) {
                loginfo("configuring ultrasonic: %d", bay);
                FK_ASSERT(configurer.ultrasonic(bay));
            }
        }
        else {
            loginfo("configuring ultrasonic: %d", bay_);
            FK_ASSERT(configurer.ultrasonic(bay_));
        }
        break;
    }
    default: {
        break;
    }
    }
}

}
