#include "configure_module_worker.h"
#include "state_ref.h"
#include "storage/storage.h"
#include "hal/board.h"

#include "modules/bridge/modules.h"
#include "modules/scanning.h"
#include "modules/configure.h"

namespace fk {

FK_DECLARE_LOGGER("cfgworker");

ConfigureModuleWorker::ConfigureModuleWorker(ConfigureModuleKind kind) : kind_(kind) {
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
        FK_ASSERT(configurer.weather(6));
        break;
    }
    case ConfigureModuleKind::Water: {
        FK_ASSERT(configurer.water(6));
        break;
    }
    case ConfigureModuleKind::Ultrasonic: {
        FK_ASSERT(configurer.ultrasonic(6));
        break;
    }
    default: {
        break;
    }
    }
}

}
