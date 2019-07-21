#pragma once

#include "hal/hal.h"

namespace fk {

class MetalModMux : public ModMux {
private:
    bool available_{ false };

public:
    MetalModMux();

public:
    bool begin() override;
    bool enable_all_modules() override;
    bool disable_all_modules() override;

};

}
