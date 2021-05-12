#include "water_module.h"

using namespace fk;

extern "C" {

static Module *fk_module_create_water(Pool &pool) {
    return new (pool) WaterModule(pool);
}

ModuleMetadata const fk_module_meta_water_ph = {
    .manufacturer = FK_MODULES_MANUFACTURER,
    .kind = FK_MODULES_KIND_WATER_PH,
    .version = 0x01,
    .name = "water.ph",
    .flags = FK_MODULES_FLAG_NONE,
    .ctor = fk_module_create_water,
};

ModuleMetadata const fk_module_meta_water_ec = {
    .manufacturer = FK_MODULES_MANUFACTURER,
    .kind = FK_MODULES_KIND_WATER_EC,
    .version = 0x01,
    .name = "water.ec",
    .flags = FK_MODULES_FLAG_NONE,
    .ctor = fk_module_create_water,
};

ModuleMetadata const fk_module_meta_water_do = {
    .manufacturer = FK_MODULES_MANUFACTURER,
    .kind = FK_MODULES_KIND_WATER_DO,
    .version = 0x01,
    .name = "water.do",
    .flags = FK_MODULES_FLAG_NONE,
    .ctor = fk_module_create_water,
};

ModuleMetadata const fk_module_meta_water_temp = {
    .manufacturer = FK_MODULES_MANUFACTURER,
    .kind = FK_MODULES_KIND_WATER_TEMP,
    .version = 0x01,
    .name = "water.temp",
    .flags = FK_MODULES_FLAG_NONE,
    .ctor = fk_module_create_water,
};

ModuleMetadata const fk_module_meta_water_orp = {
    .manufacturer = FK_MODULES_MANUFACTURER,
    .kind = FK_MODULES_KIND_WATER_ORP,
    .version = 0x01,
    .name = "water.orp",
    .flags = FK_MODULES_FLAG_NONE,
    .ctor = fk_module_create_water,
};

__attribute__((constructor))
void fk_module_initialize_water() {
    fk_modules_builtin_register(&fk_module_meta_water_ph);
    fk_modules_builtin_register(&fk_module_meta_water_ec);
    fk_modules_builtin_register(&fk_module_meta_water_do);
    fk_modules_builtin_register(&fk_module_meta_water_temp);
    fk_modules_builtin_register(&fk_module_meta_water_orp);
}

}
