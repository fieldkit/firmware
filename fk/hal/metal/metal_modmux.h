#pragma once

#include "hal/hal.h"

namespace fk {

class MetalModMux : public ModMux {
private:

public:
    MetalModMux();

public:
    bool begin() override;
    bool enable_all_modules() override;
    bool disable_all_modules() override;

};

}
