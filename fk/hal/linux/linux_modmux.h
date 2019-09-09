#pragma once

#include "hal/hal.h"

namespace fk {

class LinuxModMux : public ModMux {
private:

public:
    LinuxModMux();

public:
    bool begin() override;
    bool enable_all_modules() override;
    bool disable_all_modules() override;
    bool enable_module(uint8_t position) override;
    bool disable_module(uint8_t position) override;
    bool choose(uint8_t position) override;
    bool choose_nothing() override;

};

}
