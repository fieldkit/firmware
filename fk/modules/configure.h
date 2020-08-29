#pragma once

#include "common.h"
#include "modules/shared/modules.h"
#include "modules/scanning.h"

namespace fk {

class ModuleConfigurer {
private:
    ModuleScanning *scanning_;

public:
    explicit ModuleConfigurer(ModuleScanning &scanning);

public:
    bool configure(ModulePosition position, ModuleHeader header);
    bool erase(ModulePosition position);

};

} // namespace fk
