#pragma once

#include "common.h"

namespace fk {

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

class ModuleLeds {
public:
    virtual bool begin() = 0;
    virtual bool brightness(uint8_t value, bool refresh = true) = 0;
    virtual void off() = 0;
    virtual void on() = 0;
    virtual void color(uint8_t position, Color color, bool refresh = true) = 0;
    virtual void off(uint8_t position) = 0;
    virtual void on(uint8_t position) = 0;
    virtual bool refresh() = 0;
};

ModuleLeds *get_module_leds();

} // namespace fk
