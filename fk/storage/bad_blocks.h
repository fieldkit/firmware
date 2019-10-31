#pragma once

#include "pool.h"
#include "hal/memory.h"

namespace fk {

class BadBlocks {
private:
    DataMemory *memory_;
    flash_geometry_t geometry_;
    Pool *pool_;
    uint8_t *table_;

public:
    BadBlocks(DataMemory *memory);
    virtual ~BadBlocks();

public:
    void mark_address_as_bad(uint32_t address);
    void mark_block_as_bad(uint32_t block);
    bool is_address_bad(uint32_t address);
    bool is_block_bad(uint32_t block);

};

}