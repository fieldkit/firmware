#pragma once

#include "hal/hal.h"

namespace fk {

class FactoryWipe {
private:
    Buttons *buttons_;
    DataMemory *memory_;

public:
    FactoryWipe(Buttons *buttons, DataMemory *memory);

public:
    bool wipe_if_necessary();

};

}
