#include "water_module.h"

using namespace fk;

extern "C" {

static Module *fk_module_create_water(Pool &pool) {
    return new(pool) WaterModule();
}

ModuleMetadata const fk_module_meta_water = {
    .manufacturer = FK_MODULES_MANUFACTURER,
    .kind = FK_MODULES_KIND_WATER,
    .version = 0x01,
    .name = "water",
    .flags = FK_MODULES_FLAG_NONE,
    .ctor = fk_module_create_water,
};

__attribute__((constructor))
void fk_module_initialize_water() {
    fk_modules_builtin_register(&fk_module_meta_water);
}

}
