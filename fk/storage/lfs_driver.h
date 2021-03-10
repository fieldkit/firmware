#pragma once

#include <lfs.h>

#include "hal/memory.h"
#include "pool.h"
#include "storage/types.h"
#include "io.h"

#include "progress.h"

namespace fk {

int32_t lfs_test();

#define LFS_DRIVER_READ_SIZE             (1024)
#define LFS_DRIVER_PROG_SIZE             (1024)
#define LFS_DRIVER_CACHE_SIZE            (LFS_DRIVER_READ_SIZE * 2)
#define LFS_DRIVER_LOOKAHEAD_SIZE        (16)

#define LFS_DRIVER_ATTR_NBLOCKS          (0x00)
#define LFS_DRIVER_ATTR_FIRST_BLOCK      (0x01)
#define LFS_DRIVER_ATTR_CONFIG_MODULES   (0x02)
#define LFS_DRIVER_ATTR_CONFIG_SCHEDULE  (0x03)
#define LFS_DRIVER_ATTR_CONFIG_STATE     (0x04)
#define LFS_DRIVER_ATTR_CONFIG_OTHER     (0x05)

typedef struct fklfs_attribute_template_t {
    uint8_t type;
    lfs_size_t size;
} fklfs_attribute_template_t;

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
    bool begin(bool force_create = false);

    lfs_file_config make_file_cfg(fklfs_attribute_template_t const *attributes, lfs_size_t nattributes, Pool &pool);

    lfs_file_config make_data_cfg(Pool &pool);

    lfs_file_config make_meta_cfg(Pool &pool);

    lfs_t *lfs() {
        return &lfs_;
    }

};

}
