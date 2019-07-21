#pragma once

#include "hal/hal.h"
#include "storage.h"

namespace fk {

class FactoryWipe {
private:
    Buttons *buttons_;
    Storage *storage_;

public:
    FactoryWipe(Buttons *buttons, Storage *storage);

public:
    bool wipe_if_necessary();

};

}
