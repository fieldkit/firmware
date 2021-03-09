#pragma once

#include <lfs.h>

#include "hal/memory.h"
#include "pool.h"

namespace fk {

int32_t lfs_test();

#define LFS_DRIVER_READ_SIZE             (1024)
#define LFS_DRIVER_PROG_SIZE             (1024)
#define LFS_DRIVER_CACHE_SIZE            (LFS_DRIVER_READ_SIZE * 2)
#define LFS_DRIVER_LOOKAHEAD_SIZE        (16)

class LfsDriver {
private:
    DataMemory *memory_{ nullptr };
    Pool *pool_{ nullptr };
    struct lfs_config cfg_;
    lfs_t lfs_;

public:
    LfsDriver(DataMemory *memory, Pool &pool);
    virtual ~LfsDriver();

public:
    int32_t read(struct lfs_config const *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size);
    int32_t prog(struct lfs_config const *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size);
    int32_t erase(struct lfs_config const *c, lfs_block_t block);

public:
    bool begin();

    lfs_t &lfs() {
        return lfs_;
    }

    lfs_file_config make_file_cfg() {
        return {
            .buffer = pool_->malloc(LFS_DRIVER_CACHE_SIZE),
            .attrs = nullptr,
            .attr_count = 0,
        };
    };

};

}
