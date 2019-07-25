#pragma once

#include "common.h"
#include "hal/hal.h"

namespace fk {

class ModuleScanning {
private:
    ModMux *mm_;

public:
    ModuleScanning(ModMux *mm);

public:
    bool scan();

};

}
