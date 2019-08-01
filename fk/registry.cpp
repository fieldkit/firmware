#include "registry.h"

namespace fk {

/**
 * Maximum number of modules that can be builtin to the firmware.
 */
#define FK_MODULES_BUILTIN_MAXIMUM (10)

typedef struct ModuleNode {
    ModuleMetadata const *meta;
    struct ModuleNode *link;
} ModuleNode;

static ModuleNode nodes[FK_MODULES_BUILTIN_MAXIMUM];

static uint32_t fk_modules_builtin_get(ModuleNode **iter);

FK_DECLARE_LOGGER("modreg");

ResolvedModules::ResolvedModules() {
    for (size_t i = 0; i < MaximumNumberOfModules; ++i) {
        metas_[i] = nullptr;
    }
}

ResolvedModules::~ResolvedModules() {
}

int32_t ResolvedModules::size() const {
    return size_;
}

ModuleMetadata const *ResolvedModules::get(int32_t i) const {
    return metas_[i];
}

void ResolvedModules::set(int32_t i, ModuleMetadata const *meta) {
    metas_[i] = meta;
    size_ = 0;
    for (size_t i = 0; i < MaximumNumberOfModules; ++i) {
        if (metas_[i] != nullptr) {
            size_++;
        }
    }
}

ModuleRegistry::ModuleRegistry() {
}

ModuleRegistry::~ModuleRegistry() {
}

ModuleMetadata const *ModuleRegistry::resolve(ModuleHeader const &header) {
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

bool ModuleRegistry::resolve(ModuleScan const &scan, ResolvedModules &resolved) {
    for (size_t i = 0; i < MaximumNumberOfModules; ++i) {
        auto &header = scan.get(i);
        if (header.manufacturer > 0) {
            auto meta = resolve(header);
            FK_ASSERT(meta != nullptr);

            resolved.metas_[i] = meta;
        }
        else {
            resolved.metas_[i] = nullptr;
        }
    }

    resolved.size_ = scan.size();

    return true;
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

uint32_t fk_modules_builtin_register(ModuleMetadata const *modmeta) {
    ModuleNode *prev = NULL;

    for (auto i = 0; i < FK_MODULES_BUILTIN_MAXIMUM; ++i) {
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
