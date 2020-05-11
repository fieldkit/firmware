#pragma once

#include "hal/hal.h"
#include "storage/storage.h"
#include "progress.h"

namespace fk {

class FactoryWipe {
private:
    Display *display_;
    Buttons *buttons_;
    Storage *storage_;

public:
    explicit FactoryWipe(Storage &storage);
    explicit FactoryWipe(Display *display, Buttons *buttons, Storage *storage);

public:
    bool wipe(ProgressCallbacks *progress);

};

}
