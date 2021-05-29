#include "networking/module_handler.h"
#include "state_ref.h"
#include "modules/module_factory.h"
#include "modules/enable_module_power.h"

namespace fk {

FK_DECLARE_LOGGER("mod-api");

ModuleHandler::ModuleHandler(ModulePosition bay) : bay_(bay) {
}

bool ModuleHandler::handle(HttpServerConnection *connection, Pool &pool) {
#if defined(FK_OLD_STATE)
    auto &factory = get_module_factory();

    auto constructed = factory.get(bay_);
    if (!constructed) {
        connection->error(HttpStatus::NotFound, "invalid module", pool);
        return true;
    }

    auto mm = get_modmux();
    auto lock = mm->lock();
    auto module_bus = get_board()->i2c_module();

    auto configuration = (*constructed)->configuration;
    EnableModulePower module_power{ true, configuration.power, (*constructed)->found.position };
    if (!module_power.enable()) {
        connection->error(HttpStatus::ServerError, "error powering module", pool);
        return true;
    }
    if (module_power.was_enabled()) {
        fk_delay(configuration.wake_delay);
    }

    // Grab a read only lock for the global state before we descend
    // into the module's code. We get a read write lock later, but I'm
    // hoping to remove that eventually and would rather keep things
    // read only where I can.
    {
        auto gs = get_global_state_ro();

        ScanningContext ctx{ mm, gs.get()->location(pool), module_bus, pool };

        auto mc = ctx.open_module(bay_, pool);

        if (!mc.open()) {
            connection->error(HttpStatus::ServerError, "error choosing module", pool);
            return true;
        }

        if (!(*constructed)->module_instance->api(mc, connection, pool)) {
            connection->error(HttpStatus::ServerError, "error servicing module api", pool);
            return true;
        }
    }

    // HACK In order to get the new calibration code up and running
    // this manually fixes up the module configuration in our global
    // state in two places. First in the module factory copy of module
    // state and then in the global state. We're also careful to
    // create the copy in the correct pool.
    {
        auto gs = get_global_state_rw();
        configuration = (*constructed)->module_instance->get_configuration(factory.pool());
        (*constructed)->configuration = configuration;

        for (auto &module_meta_and_readings : gs.get()->modules->readings) {
            if (module_meta_and_readings.position == (*constructed)->found.position) {
                module_meta_and_readings.configuration = configuration;
            }
        }

        if (configuration.message != nullptr) {
            loginfo("updated configuration");
        }
        else {
            loginfo("no updated configuration");
        }
    }
#else
    auto gs = get_global_state_rw();
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

    EnableModulePower module_power{ true, configuration.power, bay_ };
    if (!module_power.enable()) {
        connection->error(HttpStatus::ServerError, "error powering module", pool);
        return true;
    }

    if (module_power.was_enabled()) {
        fk_delay(configuration.wake_delay);
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
#endif
    return true;
}

} // namespace fk
