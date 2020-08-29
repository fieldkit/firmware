#include "networking/module_handler.h"
#include "state_ref.h"
#include "modules/module_factory.h"

namespace fk {

ModuleHandler::ModuleHandler(ModulePosition bay) : bay_(bay) {
}

bool ModuleHandler::handle(HttpServerConnection *connection, Pool &pool) {
    auto &factory = get_module_factory();

    auto constructed = factory.get(bay_);
    if (!constructed) {
        connection->error(404, "invalid module");
        return true;
    }

    auto mm = get_modmux();
    auto lock = mm->lock();
    auto module_bus = get_board()->i2c_module();
    auto gs = get_global_state_ro();

    ScanningContext ctx{ mm, gs.get()->location(pool), module_bus, pool };

    auto mc = ctx.module(bay_, pool);

    if (!mc.open()) {
        connection->error(500, "error choosing module");
        return true;
    }

    if (!constructed->module->api(mc, connection, pool)) {
        connection->error(500, "error servicing module api");
        return true;
    }

    return true;
}

} // namespace fk
