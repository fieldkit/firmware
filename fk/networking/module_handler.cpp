#include "networking/module_handler.h"
#include "modules/enable_module_power.h"
#include "state_ref.h"

namespace fk {

FK_DECLARE_LOGGER("mod-api");

ModuleHandler::ModuleHandler(ModulePosition bay) : bay_(bay) {
}

bool ModuleHandler::handle(HttpServerConnection *connection, Pool &pool) {
    auto gs = get_global_state_ro();
    auto attached = gs.get()->dynamic.attached();
    auto attached_module = attached->get_by_position(bay_);
    if (attached_module == nullptr) {
        connection->error(HttpStatus::NotFound, "invalid module", pool);
        return true;
    }

    auto configuration = attached_module->configuration();

    auto mm = get_modmux();
    auto lock = mm->lock();
    auto module_bus = get_board()->i2c_module();

    EnableModulePower module_power{ bay_, ModulePower::Always, configuration.wake_delay };
    if (!module_power.enable()) {
        connection->error(HttpStatus::ServerError, "error powering module", pool);
        return true;
    }

    ScanningContext ctx{ mm, gs.get()->location(pool), module_bus, pool };

    auto mc = ctx.open_module(bay_, pool);

    if (!mc.open()) {
        connection->error(HttpStatus::ServerError, "error choosing module", pool);
        return true;
    }

    if (!attached_module->get()->api(mc, connection, pool)) {
        connection->error(HttpStatus::ServerError, "error servicing module api", pool);
        return true;
    }

    // The memory for this is handled by the module itself, which is risky.
    configuration = attached_module->get_configuration(&pool);

    return true;
}

} // namespace fk
