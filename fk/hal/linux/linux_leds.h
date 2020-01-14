#pragma once

#include "hal/hal.h"

namespace fk {

class LinuxLeds : public ModuleLeds {
private:

public:
    LinuxLeds();

public:
    bool begin() override;
    void brightness(uint8_t value, bool refresh = true) override;
    void off() override;
    void on() override;
    void color(uint8_t position, Color color, bool refresh = true) override;
    void off(uint8_t position) override;
    void on(uint8_t position) override;
    bool refresh() override;

};

}
