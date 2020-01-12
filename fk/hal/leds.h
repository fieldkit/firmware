#pragma once

#include "common.h"

namespace fk {

class ModuleLeds {
private:

public:
    ModuleLeds();
    virtual ~ModuleLeds();

public:
    virtual bool begin() = 0;
    virtual void brightness(uint8_t value) = 0;
    virtual void off() = 0;
    virtual void on() = 0;

};

ModuleLeds *get_module_leds();

} // namespace fk
