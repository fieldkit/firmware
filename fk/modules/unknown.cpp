#include "modules/unknown.h"

namespace fk {

static Module *fk_module_create_unknown(Pool &pool) {
    return new (pool) UnknownModule();
}

ModuleMetadata const fk_unknown_module = {
    .manufacturer = 0,
    .kind = 0,
    .version = 0,
    .name = "unknown",
    .flags = FK_MODULES_FLAG_NONE,
    .ctor = fk_module_create_unknown,
};

ModuleReturn UnknownModule::initialize(ModuleContext mc, Pool &pool) {
    return { ModuleStatus::Ok };
}

ModuleReturn UnknownModule::api(ModuleContext mc, HttpServerConnection *connection, Pool &pool) {
    return { ModuleStatus::Ok };
}

ModuleReturn UnknownModule::service(ModuleContext mc, Pool &pool) {
    return { ModuleStatus::Ok };
}

static ModuleSensors fk_module_unknown_empty_sensors = {
    .nsensors = 0,
    .sensors = nullptr,
};

ModuleSensors const *UnknownModule::get_sensors(Pool &pool) {
    return &fk_module_unknown_empty_sensors;
}

ModuleConfiguration const UnknownModule::get_configuration(Pool &pool) {
    return { "modules.unknown" };
}

ModuleReadings *UnknownModule::take_readings(ReadingsContext mc, Pool &pool) {
    return nullptr;
}

} // namespace fk
