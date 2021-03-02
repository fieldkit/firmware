#pragma once

#include "hal/hal.h"

namespace fk {

class LinuxButtons : public Buttons {
public:
    LinuxButtons();

public:
    bool begin() override;
    bool is_debug_pressed() const override;
    bool get(uint8_t which) const override;

};

}
