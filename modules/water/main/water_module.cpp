#include "water_module.h"

using namespace fk;

bool WaterModule::initialize(ModuleContext mc) {
    return true;
}

ModuleReadings *WaterModule::take_readings(ModuleContext mc) {
    return nullptr;
}
