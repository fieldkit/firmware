#include "omni_water_module.h"

using namespace fk;

extern "C" {

static Module *fk_module_create_omni_water(Pool &pool) {
    return new (pool) OmniWaterModule(pool);
}

ModuleMetadata const fk_module_meta_omni_water = {
    .manufacturer = FK_MODULES_MANUFACTURER,
    .kind = FK_MODULES_KIND_WATER_OMNI,
    .version = 0x01,
    .name = "water.omni",
    .flags = FK_MODULES_FLAG_NONE,
    .ctor = fk_module_create_omni_water,
};

__attribute__((constructor)) void fk_module_initialize_omni_water() {
    fk_modules_builtin_register(&fk_module_meta_omni_water);
}
}
