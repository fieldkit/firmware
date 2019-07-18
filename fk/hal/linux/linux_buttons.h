#pragma once

#include "hal/hal.h"

namespace fk {

class LinuxButtons : public Buttons {
public:
    LinuxButtons();

public:
    bool begin() override;

};

}
