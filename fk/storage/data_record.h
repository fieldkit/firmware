#pragma once

#include <loading.h>
#include <fk-data-protocol.h>

#include "modules/module_factory.h"
#include "state.h"

namespace fk {

class DataRecord {
private:
    fk_data_DataRecord record_;

public:
    DataRecord();

public:
    fk_data_DataRecord &record() {
        return record_;
    }

};

}
