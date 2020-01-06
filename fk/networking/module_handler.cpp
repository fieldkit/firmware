#include "networking/module_handler.h"
#include "state_ref.h"

namespace fk {

ModuleHandler::ModuleHandler(uint8_t bay) : bay_(bay) {
}

bool ModuleHandler::handle(HttpServerConnection *connection, Pool &pool) {
    auto &factory = get_module_factory();

    auto constructed = factory.get(bay_);
    if (!constructed) {
        connection->error("invalid module");
        return true;
    }

    auto lock = get_board()->lock_eeprom();
    auto module_bus = get_board()->i2c_module();
    auto gs = get_global_state_ro();
    auto mm = get_modmux();

    ScanningContext ctx{ mm, gs.get(), module_bus };

    auto mc = ctx.module(bay_);

    if (!mc.open()) {
        connection->error("error choosing module");
        return true;
    }

    if (!constructed->module->api(mc, connection, pool)) {
        connection->error("error servicing module api");
        return true;
    }

    return true;
}

} // namespace fk
