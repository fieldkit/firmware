#include "water_module.h"

using namespace fk;

bool WaterModule::initialize(ModuleContext mc, fk::Pool &pool) {
    return true;
}

ModuleReadings *WaterModule::take_readings(ModuleContext mc, fk::Pool &pool) {
    return nullptr;
}
