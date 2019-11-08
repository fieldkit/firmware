#pragma once

#include "hal/memory.h"

namespace fk {

class SequentialMemory {
private:
    DataMemory *memory_;

public:
    SequentialMemory(DataMemory *memory);

public:
    int32_t read(uint32_t address, uint8_t *data, size_t length) const;
    int32_t write(uint32_t address, uint8_t *data, size_t length);
    int32_t flush();

};

}
