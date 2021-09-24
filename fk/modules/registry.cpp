#include "modules/registry.h"
#include "modules/unknown.h"

namespace fk {

/**
 * Maximum number of modules that can be builtin to the firmware.
 */
#define FK_MODULES_BUILTIN_MAXIMUM (20)

typedef struct ModuleNode {
    ModuleMetadata const *meta;
    struct ModuleNode *link;
} ModuleNode;

static ModuleNode nodes[FK_MODULES_BUILTIN_MAXIMUM];

static uint32_t fk_modules_builtin_get(ModuleNode **iter);

FK_DECLARE_LOGGER("modreg");

ModuleRegistry::ModuleRegistry() {
}

ModuleRegistry::~ModuleRegistry() {
}

void ModuleRegistry::initialize() {
    ModuleNode *iter = nullptr;

    if (fk_modules_builtin_get(&iter) == 0) {
        for (; iter != nullptr; iter = iter->link) {
            auto meta = iter->meta;

            loginfo("module mk=%02" PRIx32 "%02" PRIx32 " version=%" PRIu32 " %s", meta->manufacturer, meta->kind, meta->version, meta->name);
        }
    }
}

ModuleMetadata const *ModuleRegistry::resolve(ModuleHeader const &header) {
    ModuleNode *iter = nullptr;

    if (fk_modules_builtin_get(&iter) != 0) {
        return &fk_unknown_module;
    }

    for ( ; iter != nullptr; iter = iter->link) {
        auto meta = iter->meta;

        if (header.manufacturer == meta->manufacturer &&
            header.kind == meta->kind &&
            header.version == meta->version) {
            return iter->meta;
        }
    }

    return &fk_unknown_module;
}

static uint32_t fk_modules_builtin_get(ModuleNode **iter) {
    if (nodes[0].meta == NULL) {
        *iter = NULL;
        return 0;
    }
    *iter = &nodes[0];
    return 0;
}

#ifdef __cplusplus
extern "C" {
#endif

uint32_t fk_modules_builtin_clear() {
    bzero(nodes, sizeof(nodes));
    return 0;
}

uint32_t fk_modules_builtin_register(ModuleMetadata const *modmeta) {
    ModuleNode *prev = NULL;

    for (auto i = 0; i < FK_MODULES_BUILTIN_MAXIMUM; ++i) {
        if (nodes[i].meta == modmeta) {
            return 0;
        }
        if (nodes[i].meta == NULL) {
            nodes[i].meta = modmeta;
            nodes[i].link = NULL;
            if (prev != NULL) {
                prev->link = &nodes[i];
            }
            break;
        }
        prev = &nodes[i];
    }
    return 0;
}

#ifdef __cplusplus
}
#endif

}
