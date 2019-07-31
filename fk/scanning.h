#pragma once

#include "common.h"
#include "hal/hal.h"

namespace fk {

class ModuleScanning {
private:
    ModMux *mm_;
    ModuleHeader headers_[MaximumNumberOfModules];
    size_t number_of_modules_{ 0 };

public:
    ModuleScanning(ModMux *mm);

public:
    bool scan();

    void clear();

    ModuleHeader &header(int32_t index);

    size_t number_of_modules() const;

};

}
