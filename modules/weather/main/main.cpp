#include <common.h>
#include <modules.h>

extern "C" {

fk::ModuleMetadata const fk_module_meta_weather = {
    .manufacturer = 0x01,
    .kind = 0x01,
    .version = 0x01,
    .name = "weather",
};

__attribute__((constructor))
void fk_module_initialize_weather() {
    fk_modules_builtin_register(&fk_module_meta_weather);
}

}
