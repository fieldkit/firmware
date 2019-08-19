#pragma once

#include "common.h"
#include "config.h"
#include "hal/modmux.h"
#include "modules/modules.h"
#include "pool.h"

namespace fk {

using ModuleHeaderCollection = std::list<ModuleHeader, pool_allocator<ModuleHeader>>;

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
    bool configure(uint8_t position, ModuleHeader &header);
    nonstd::optional<ModuleHeaderCollection> scan(Pool &pool);

private:
    bool available();

};

}
