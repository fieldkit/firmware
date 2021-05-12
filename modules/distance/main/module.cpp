#include "distance_module.h"

using namespace fk;

extern "C" {

static Module *fk_module_create_distance(Pool &pool) {
    return new (pool) DistanceModule(pool);
}

ModuleMetadata const fk_module_meta_distance = {
    .manufacturer = FK_MODULES_MANUFACTURER,
    .kind = FK_MODULES_KIND_DISTANCE,
    .version = 0x01,
    .name = "distance",
    .flags = FK_MODULES_FLAG_NONE,
    .ctor = fk_module_create_distance,
};

__attribute__((constructor))
void fk_module_initialize_distance() {
    fk_modules_builtin_register(&fk_module_meta_distance);
}

}
