#pragma once

#include "scanning.h"
#include "storage/storage.h"
#include "modules_bridge.h"

namespace fk {

class ReadingsTaker {
private:
    ModuleScanning &scanning_;
    Storage &storage_;

public:
    ReadingsTaker(ModuleScanning &scanning, Storage &storage);

public:
    bool take(ModuleContext &mc, Pool &pool);

};

}
