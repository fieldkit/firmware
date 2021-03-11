#pragma once

#include "storage/lfs_driver.h"

namespace fk {

class Attributes {
private:
    lfs_file_config &cfg_;

public:
    Attributes(lfs_file_config &cfg) : cfg_(cfg) {
    }

public:
    uint32_t nblocks() const {
        return *(uint32_t *)cfg_.attrs[LFS_DRIVER_ATTR_NBLOCKS].buffer;
    }

    uint32_t first_block() const {
        return *(uint32_t *)cfg_.attrs[LFS_DRIVER_ATTR_FIRST_BLOCK].buffer;
    }

    uint32_t increase_nblocks() {
        auto &nblocks = *(uint32_t *)cfg_.attrs[LFS_DRIVER_ATTR_NBLOCKS].buffer;
        nblocks++;
        return nblocks;
    }

    void set(uint8_t index, uint32_t value) {
        *(uint32_t *)cfg_.attrs[index].buffer = value;
    }

    uint32_t get(uint8_t index) {
        return *(uint32_t *)cfg_.attrs[index].buffer;
    }

    void first_block(uint32_t first_block) {
        set(LFS_DRIVER_ATTR_FIRST_BLOCK, first_block);
    }

    void nblocks(uint32_t nblocks) {
        set(LFS_DRIVER_ATTR_NBLOCKS, nblocks);
    }
};

} // namespace fk
