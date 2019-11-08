#pragma once

#include "hal/memory.h"

namespace fk {

class MemoryPageStore {
private:
    DataMemory *target_;

public:
    MemoryPageStore(DataMemory *target);

public:
    int32_t load_page(uint32_t address, uint8_t *ptr, size_t size);
    int32_t save_page(uint32_t address, uint8_t const *ptr, size_t size, uint16_t start, uint16_t end);

};

}
