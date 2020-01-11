#pragma once

#include "hal/hal.h"

namespace fk {

class LinuxLeds : public ModuleLeds {
private:

public:
    LinuxLeds();

public:
    bool begin() override;
    void brightness(uint8_t value) override;
    void off() override;
    void on() override;

};

}
