#pragma once

#include "hal/hal.h"

namespace fk {

class LinuxButtons : public Buttons {
public:
    LinuxButtons();

public:
    bool begin() override;
    uint8_t number_pressed() const override;

};

}
