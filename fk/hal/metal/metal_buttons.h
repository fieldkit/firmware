#pragma once

#include "hal/hal.h"

namespace fk {

class MetalButtons : public Buttons {
public:
    MetalButtons();

public:
    bool begin() override;
    bool is_debug_pressed() const override;
    bool get(uint8_t which) const override;

public:
    void irq(uint8_t index, bool down);

};

}
