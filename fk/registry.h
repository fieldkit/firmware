#pragma once

#include "common.h"
#include "modules/modules.h"

namespace fk {

class ModuleRegistry {
private:

public:
    ModuleRegistry();
    virtual ~ModuleRegistry();

public:
    ModuleMetadata const *lookup(ModuleHeader &header);

};

}
