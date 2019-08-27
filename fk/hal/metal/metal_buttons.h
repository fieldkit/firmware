#pragma once

#include "hal/hal.h"

namespace fk {

class MetalButtons : public Buttons {
public:
    MetalButtons();

public:
    bool begin() override;
    uint8_t number_pressed() const override;
    bool get(uint8_t which) const override;

public:
    void irq(uint8_t index, bool down);

};

}
