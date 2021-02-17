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
    bool erase(ModulePosition position);
    bool provision(ModulePosition position, ModuleHeader header);
    bool configure(ModulePosition position, uint8_t const *buffer, size_t size);

};

} // namespace fk
