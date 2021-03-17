#pragma once

#include "pool.h"
#include "hal/memory.h"

namespace fk {

class BadBlocks {
private:
    DataMemory *memory_{ nullptr };
    Pool *pool_{ nullptr };
    FlashGeometry geometry_;
    uint8_t *table_{ nullptr };
    size_t size_{ 0 };

public:
    BadBlocks(DataMemory *memory, Pool &pool);
    virtual ~BadBlocks();

public:
    void mark_address_as_bad(uint32_t address);
    void mark_block_as_bad(uint32_t block);
    bool is_address_bad(uint32_t address) const;
    bool is_block_bad(uint32_t block) const;

};

}
