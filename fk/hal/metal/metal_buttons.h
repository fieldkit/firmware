#pragma once

#include "hal/hal.h"

namespace fk {

class MetalButtons : public Buttons {
public:
    MetalButtons();

public:
    bool begin() override;

public:
    void irq(uint8_t pin, uint8_t index);

};

}
