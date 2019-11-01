#pragma once

#include "hal/memory.h"

namespace fk {

class SequentialMemory {
private:
    DataMemory *memory_;

public:
    SequentialMemory(DataMemory *memory);

public:
    size_t read(uint32_t address, uint8_t *data, size_t length) const;
    size_t write(uint32_t address, uint8_t *data, size_t length);
    size_t flush();

};

}
