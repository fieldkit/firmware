#pragma once

#include "common.h"
#include "modules/modules.h"
#include "modules_bridge.h"

namespace fk {

class ModuleRegistry {
private:

public:
    ModuleRegistry();
    virtual ~ModuleRegistry();

public:
    ModuleMetadata const *lookup(ModuleHeader &header);

};

typedef struct ModuleNode {
    ModuleMetadata const *meta;
    struct ModuleNode *link;
} ModuleNode;

#ifdef __cplusplus
extern "C" {
#endif

uint32_t fk_modules_builtin_register(ModuleMetadata const *modmeta);

#ifdef __cplusplus
}
#endif

}
