#pragma once

#include "common.h"
#include "hal/hal.h"

namespace fk {

class ModuleScan {
private:
    friend class ModuleScanning;

private:
    ModuleHeader headers_[MaximumNumberOfModules];
    int32_t size_{ 0 };

public:
    ModuleScan();
    virtual ~ModuleScan();

public:
    int32_t size() const;

    ModuleHeader const &get(int32_t i) const;

};

class ModuleScanning {
private:
    ModMux *mm_;

public:
    ModuleScanning(ModMux *mm);

public:
    bool scan(ModuleScan &scan);

};

}
