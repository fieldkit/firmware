#pragma once

#include "hal/hal.h"
#include "storage/storage.h"

namespace fk {

class FactoryWipe {
private:
    Display *display_;
    Buttons *buttons_;
    Storage *storage_;

public:
    FactoryWipe(Display *display, Buttons *buttons, Storage *storage);

public:
    bool wipe_if_necessary();

};

}
