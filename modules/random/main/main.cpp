#include <common.h>
#include <modules/modules.h>

extern "C" {

const ModuleMetadata fk_module_meta_random = {
    .manufacturer = 0x01,
    .kind = 0xa1,
    .version = 0x01,
    .name = "random",
};

__attribute__((constructor))
void fk_module_initialize_random() {
    fk_modules_builtin_register(&fk_module_meta_random);
}

}
