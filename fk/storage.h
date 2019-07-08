#pragma once

#include "common.h"
#include "hal/hal.h"

#include <fk-data-protocol.h>

namespace fk {

class Storage {
private:
    DataMemory *memory_;
    uint32_t sequence_{ 0 };
    uint32_t tail_{ 0 };

public:
    Storage(DataMemory *memory);

public:
    bool begin();
    bool clear();

public:
    bool append(fk_data_DataRecord *record);

};

}
