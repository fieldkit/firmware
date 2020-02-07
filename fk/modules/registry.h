#pragma once

#include "common.h"
#include "modules/scanning.h"
#include "modules/bridge/modules.h"

namespace fk {

class ModuleRegistry {
public:
    ModuleRegistry();
    virtual ~ModuleRegistry();

public:
    ModuleMetadata const *resolve(ModuleHeader const &header);

};

#ifdef __cplusplus
extern "C" {
#endif

uint32_t fk_modules_builtin_clear();

#ifdef __cplusplus
} // extern "C"
#endif

} // namespace fk
