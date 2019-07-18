#pragma once

#include "hal/hal.h"

namespace fk {

class MetalModMux {
private:

public:
    MetalModMux();

public:
    bool begin();
    bool enable_all_modules();
    bool disable_all_modules();

};

}
