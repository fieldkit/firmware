#pragma once

#include "common.h"
#include "config.h"
#include "modules/modules.h"
#include "modules_bridge.h"
#include "scanning.h"

namespace fk {

using ModuleAndMetadata = std::pair<ModuleMetadata const*, Module*>;
using ModuleAndMetadataCollection = std::list<ModuleAndMetadata, pool_allocator<ModuleAndMetadata>>;

class ResolvedModules {
private:
    friend class ModuleRegistry;

private:
    Pool &pool_;
    ModuleMetadata const *metas_[MaximumNumberOfModules];
    Module *instances_[MaximumNumberOfModules];
    int32_t size_{ 0 };

public:
    ResolvedModules(Pool &pool);
    virtual ~ResolvedModules();

public:
    int32_t size() const;
    ModuleMetadata const *meta(int32_t i) const;
    Module *instance(int32_t i) const;
    void set(int32_t i, ModuleMetadata const *meta);

};

class ModuleRegistry {
public:
    ModuleRegistry();
    virtual ~ModuleRegistry();

public:
    ModuleMetadata const *resolve(ModuleHeader const &header);
    bool resolve(ModuleScan const &scan, ResolvedModules &resolved);
    nonstd::optional<ModuleAndMetadataCollection> resolve(ModuleHeaderCollection &headers, Pool &pool);

};

#ifdef __cplusplus
extern "C" {
#endif

uint32_t fk_modules_builtin_register(ModuleMetadata const *modmeta);

#ifdef __cplusplus
}
#endif

}
