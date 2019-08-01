#include <common.h>
#include <modules.h>

extern "C" {

fk::ModuleMetadata const fk_module_meta_water = {
    .manufacturer = 0x01,
    .kind = 0x02,
    .version = 0x01,
    .name = "water",
};

__attribute__((constructor))
void fk_module_initialize_water() {
    fk_modules_builtin_register(&fk_module_meta_water);
}

}
