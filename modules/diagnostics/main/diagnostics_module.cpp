#include "diagnostics_module.h"
#include "platform.h"
#include "state.h"

using namespace fk;

bool DiagnosticsModule::initialize(ModuleContext mc, fk::Pool &pool) {
    return true;
}

ModuleReadings *DiagnosticsModule::take_readings(ModuleContext mc, fk::Pool &pool) {
    auto mr = new(pool) NModuleReadings<4>();
    mr->set(0, mc.gs()->power.charge);
    mr->set(1, mc.gs()->power.voltage);
    mr->set(2, fk_free_memory());
    mr->set(3, fk_uptime());
    return mr;
}
