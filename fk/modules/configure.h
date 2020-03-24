#pragma once

#include "common.h"
#include "modules/shared/modules.h"
#include "modules/scanning.h"

namespace fk {

class ModuleConfigurer {
private:
    ModuleScanning *scanning_;

public:
    ModuleConfigurer(ModuleScanning &scanning);

public:
    bool configure(uint8_t position, ModuleHeader header);
    bool erase(uint8_t position);

};

} // namespace fk
