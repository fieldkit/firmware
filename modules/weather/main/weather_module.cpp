#include "weather_module.h"

using namespace fk;

bool WeatherModule::initialize(ModuleContext mc) {
    return true;
}

ModuleReadings *WeatherModule::take_readings(ModuleContext mc) {
    return nullptr;
}
