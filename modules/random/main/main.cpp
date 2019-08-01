#include <common.h>
#include <modules.h>

extern "C" {

fk::ModuleMetadata const fk_module_meta_random = {
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
