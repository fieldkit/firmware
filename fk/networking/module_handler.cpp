#include "networking/module_handler.h"
#include "state_ref.h"
#include "modules/module_factory.h"
#include "modules/enable_module_power.h"

namespace fk {

FK_DECLARE_LOGGER("mod-api");

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

    auto configuration = (*constructed)->configuration;
    EnableModulePower module_power{ true, configuration.power, (*constructed)->found.position };
    if (!module_power.enable()) {
        connection->error(500, "error powering module");
        return true;
    }
    if (module_power.enabled_once()) {
        fk_delay(configuration.wake_delay);
    }

    {
        auto gs = get_global_state_ro();

        ScanningContext ctx{ mm, gs.get()->location(pool), module_bus, pool };

        auto mc = ctx.module(bay_, pool);

        if (!mc.open()) {
            connection->error(500, "error choosing module");
            return true;
        }

        if (!(*constructed)->module->api(mc, connection, pool)) {
            connection->error(500, "error servicing module api");
            return true;
        }
    }

    {
        auto gs = get_global_state_rw();
        configuration = (*constructed)->module->get_configuration(factory.pool());
        (*constructed)->configuration = configuration;

        if (configuration.message != nullptr) {
            fk_dump_memory("mod-cfg ", configuration.message->buffer, configuration.message->size);
        }
        else {
            loginfo("no updated configuration message");
        }
    }

    return true;
}

} // namespace fk
