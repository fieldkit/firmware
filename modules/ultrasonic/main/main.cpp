#include "ultrasonic_module.h"

using namespace fk;

extern "C" {

static Module *fk_module_create_ultrasonic(Pool &pool) {
    return new(pool) UltrasonicModule();
}

ModuleMetadata const fk_module_meta_ultrasonic = {
    .manufacturer = FK_MODULES_MANUFACTURER,
    .kind = FK_MODULES_KIND_ULTRASONIC,
    .version = 0x01,
    .name = "ultrasonic",
    .flags = FK_MODULES_FLAG_NONE,
    .ctor = fk_module_create_ultrasonic,
};

__attribute__((constructor))
void fk_module_initialize_ultrasonic() {
    fk_modules_builtin_register(&fk_module_meta_ultrasonic);
}

}
