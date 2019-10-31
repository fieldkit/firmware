#pragma once

#include "hal/memory.h"

namespace fk {

class MemoryPageStore {
private:
    DataMemory *target_;

public:
    MemoryPageStore(DataMemory *target);

public:
    bool load_page(uint32_t address, uint8_t *ptr, size_t size);
    bool save_page(uint32_t address, uint8_t const *ptr, size_t size, uint16_t start, uint16_t end);

};

}
