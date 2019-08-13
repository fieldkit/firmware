#pragma once

#include <fk-data-protocol.h>

#include "storage/storage.h"
#include "registry.h"
#include "state.h"

namespace fk {

class MetaLog {
private:
    Storage *storage_;
    GlobalState const *gs_;
    fk_data_DataRecord record_;

public:
    MetaLog(Storage *storage, GlobalState const *gs);
    virtual ~MetaLog();

public:
    bool open();

    fk_data_DataRecord &record();

};

}
