#include <common.h>
#include <modules.h>

#include "random_module.h"

using namespace fk;

extern "C" {

static Module *fk_module_create_random(Pool &pool) {
    return new(pool) RandomModule();
}

ModuleMetadata const fk_module_meta_random = {
    .manufacturer = FK_MODULES_MANUFACTURER,
    .kind = FK_MODULES_KIND_RANDOM,
    .version = 0x01,
    .name = "random",
    .flags = FK_MODULES_FLAG_INTERNAL,
    .ctor = fk_module_create_random,
};

__attribute__((constructor))
void fk_module_initialize_random() {
    fk_modules_builtin_register(&fk_module_meta_random);
}

}
