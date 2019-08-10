#include <common.h>
#include <modules.h>

#include "diagnostics_module.h"

using namespace fk;

extern "C" {

static Module *fk_module_create_diagnostics(Pool &pool) {
    return new(pool) DiagnosticsModule();
}

ModuleMetadata const fk_module_meta_diagnostics = {
    .manufacturer = FK_MODULES_MANUFACTURER,
    .kind = FK_MODULES_KIND_DIAGNOSTICS,
    .version = 0x01,
    .name = "diagnostics",
    .ctor = fk_module_create_diagnostics,
};

__attribute__((constructor))
void fk_module_initialize_diagnostics() {
    fk_modules_builtin_register(&fk_module_meta_diagnostics);
}

}
