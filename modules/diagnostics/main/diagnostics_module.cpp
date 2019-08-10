#include "diagnostics_module.h"

using namespace fk;

bool DiagnosticsModule::initialize(ModuleContext mc, fk::Pool &pool) {
    return true;
}

ModuleReadings *DiagnosticsModule::take_readings(ModuleContext mc, fk::Pool &pool) {
    return nullptr;
}
