#pragma once

#include "hal/hal.h"

namespace fk {

class MetalButtons : public Buttons {
public:
    MetalButtons();

public:
    bool begin() override;
    uint8_t number_pressed() const override;

public:
    void irq(uint8_t pin, uint8_t index);

};

}
