#pragma once

#include "hal/hal.h"

namespace fk {

class MetalModMux : public ModMux {
private:
    uint8_t gpio_{ 0 };

public:
    MetalModMux();

public:
    bool begin() override;
    bool enable_all_modules() override;
    bool disable_all_modules() override;
    bool enable_module(uint8_t position) override;
    bool disable_module(uint8_t position) override;
    bool choose(uint8_t position) override;

private:
    bool update_gpio();

};

}
