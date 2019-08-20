#pragma once

#include "pool.h"
#include "hal/modmux.h"
#include "modules_bridge.h"
#include "scanning.h"
#include "registry.h"

namespace fk {

class ModuleFactory {
private:
    ModuleScanning *scanning_;
    Pool *pool_;

public:
    ModuleFactory(ModuleScanning &scanning, Pool *pool);
    virtual ~ModuleFactory();

public:
    nonstd::optional<ConstructedModulesCollection> create();

private:
    Pool &pool() {
        return *pool_;
    }

};

}
