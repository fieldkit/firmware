#include "weather_module.h"

using namespace fk;

bool WeatherModule::initialize(ModuleContext mc, fk::Pool &pool) {
    return true;
}

ModuleReadings *WeatherModule::take_readings(ModuleContext mc, fk::Pool &pool) {
    return nullptr;
}
