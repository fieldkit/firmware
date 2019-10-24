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
    bool weather(uint8_t position);
    bool water(uint8_t position);
    bool ultrasonic(uint8_t position);
    bool erase(uint8_t position);

private:
    bool configure(uint8_t position, ModuleHeader &header);

};

}
