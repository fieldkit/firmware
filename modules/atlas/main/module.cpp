#include "atlas_module.h"

using namespace fk;

extern "C" {

static Module *fk_module_create_atlas(Pool &pool) {
    return new(pool) AtlasModule();
}

ModuleMetadata const fk_module_meta_atlas_ph = {
    .manufacturer = FK_MODULES_MANUFACTURER,
    .kind = FK_MODULES_KIND_ATLAS_PH,
    .version = 0x01,
    .name = "water.ph",
    .flags = FK_MODULES_FLAG_NONE,
    .ctor = fk_module_create_atlas,
};

ModuleMetadata const fk_module_meta_atlas_ec = {
    .manufacturer = FK_MODULES_MANUFACTURER,
    .kind = FK_MODULES_KIND_ATLAS_EC,
    .version = 0x01,
    .name = "water.ec",
    .flags = FK_MODULES_FLAG_NONE,
    .ctor = fk_module_create_atlas,
};

ModuleMetadata const fk_module_meta_atlas_do = {
    .manufacturer = FK_MODULES_MANUFACTURER,
    .kind = FK_MODULES_KIND_ATLAS_DO,
    .version = 0x01,
    .name = "water.do",
    .flags = FK_MODULES_FLAG_NONE,
    .ctor = fk_module_create_atlas,
};

ModuleMetadata const fk_module_meta_atlas_temp = {
    .manufacturer = FK_MODULES_MANUFACTURER,
    .kind = FK_MODULES_KIND_ATLAS_TEMP,
    .version = 0x01,
    .name = "water.temp",
    .flags = FK_MODULES_FLAG_NONE,
    .ctor = fk_module_create_atlas,
};

ModuleMetadata const fk_module_meta_atlas_orp = {
    .manufacturer = FK_MODULES_MANUFACTURER,
    .kind = FK_MODULES_KIND_ATLAS_ORP,
    .version = 0x01,
    .name = "water.orp",
    .flags = FK_MODULES_FLAG_NONE,
    .ctor = fk_module_create_atlas,
};

__attribute__((constructor))
void fk_module_initialize_atlas() {
    fk_modules_builtin_register(&fk_module_meta_atlas_ph);
    fk_modules_builtin_register(&fk_module_meta_atlas_ec);
    fk_modules_builtin_register(&fk_module_meta_atlas_do);
    fk_modules_builtin_register(&fk_module_meta_atlas_temp);
    fk_modules_builtin_register(&fk_module_meta_atlas_orp);
}

}
