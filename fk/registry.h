#pragma once

#include "common.h"
#include "config.h"
#include "modules/modules.h"
#include "modules_bridge.h"
#include "scanning.h"

namespace fk {

struct ConstructedModule {
    FoundModule found;
    ModuleMetadata const *meta;
    Module *module;
};

using ConstructedModulesCollection = std::list<ConstructedModule, pool_allocator<ConstructedModule>>;

class ModuleRegistry {
public:
    ModuleRegistry();
    virtual ~ModuleRegistry();

public:
    ModuleMetadata const *resolve(ModuleHeader const &header);
    nonstd::optional<ConstructedModulesCollection> resolve(FoundModuleCollection &headers, Pool &pool);

};

#ifdef __cplusplus
extern "C" {
#endif

uint32_t fk_modules_builtin_clear();
uint32_t fk_modules_builtin_register(ModuleMetadata const *modmeta);

#ifdef __cplusplus
}
#endif

}
