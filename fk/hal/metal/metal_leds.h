#pragma once

#include "hal/hal.h"

namespace fk {

class MetalLeds : public ModuleLeds {
private:

public:
    MetalLeds();

public:
    bool begin() override;
    void brightness(uint8_t value) override;
    void off() override;
    void on() override;

};

}
