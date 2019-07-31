#include "registry.h"

namespace fk {

FK_DECLARE_LOGGER("modreg");

ModuleRegistry::ModuleRegistry() {
}

ModuleRegistry::~ModuleRegistry() {
}

ModuleMetadata const *ModuleRegistry::lookup(ModuleHeader &header) {
    ModuleNode *iter = nullptr;

    if (fk_modules_builtin_get(&iter) != 0) {
        return nullptr;
    }

    for ( ; iter != nullptr; iter = iter->link) {
        auto meta = iter->meta;

        if (header.manufacturer == meta->manufacturer &&
            header.kind == meta->kind &&
            header.version == meta->version) {
            return iter->meta;
        }
    }

    return nullptr;
}

}
