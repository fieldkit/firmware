#pragma once

#include "common.h"
#include "config.h"
#include "modules/modules.h"
#include "modules_bridge.h"
#include "scanning.h"

namespace fk {

class ResolvedModules {
private:
    friend class ModuleRegistry;

private:
    ModuleMetadata const *metas_[MaximumNumberOfModules];
    int32_t size_;

public:
    ResolvedModules();
    virtual ~ResolvedModules();

public:
    int32_t size() const;
    ModuleMetadata const *get(int32_t i) const;
    void set(int32_t i, ModuleMetadata const *meta);

};

class ModuleRegistry {
public:
    ModuleRegistry();
    virtual ~ModuleRegistry();

public:
    ModuleMetadata const *resolve(ModuleHeader const &header);
    bool resolve(ModuleScan const &scan, ResolvedModules &resolved);

};

#ifdef __cplusplus
extern "C" {
#endif

uint32_t fk_modules_builtin_register(ModuleMetadata const *modmeta);

#ifdef __cplusplus
}
#endif

}
