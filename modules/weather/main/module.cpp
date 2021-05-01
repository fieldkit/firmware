#include "weather_module.h"

using namespace fk;

extern "C" {

static Module *fk_module_create_weather(Pool &pool) {
    return new (pool) WeatherModule(pool);
}

ModuleMetadata const fk_module_meta_weather = {
    .manufacturer = FK_MODULES_MANUFACTURER,
    .kind = FK_MODULES_KIND_WEATHER,
    .version = 0x01,
    .name = "weather",
    .flags = FK_MODULES_FLAG_NONE,
    .ctor = fk_module_create_weather,
};

__attribute__((constructor))
void fk_module_initialize_weather() {
    fk_modules_builtin_register(&fk_module_meta_weather);
}

}
