#pragma once

#include "pool.h"
#include "hal/modmux.h"
#include "modules_bridge.h"

namespace fk {

class ModuleFactory {
private:
    ModMux *mm_;
    Pool *pool_;

public:
    ModuleFactory(ModMux *mm, Pool *pool);
    virtual ~ModuleFactory();

public:
    bool create();

private:
    Pool &pool() {
        return *pool_;
    }

};

}
