#pragma once

#include <lfs.h>

#include "hal/memory.h"
#include "pool.h"
#include "storage/types.h"
#include "io.h"

#include "progress.h"

namespace fk {

int32_t lfs_test();

#define LFS_DRIVER_READ_SIZE                  (1024)
#define LFS_DRIVER_PROG_SIZE                  (1024)
#define LFS_DRIVER_CACHE_SIZE                 (LFS_DRIVER_READ_SIZE * 2)
#define LFS_DRIVER_LOOKAHEAD_SIZE             (16)

/**
 * Warning: These are also used as indices.
 */
#define LFS_DRIVER_DIR_ATTR_NFILES            (0x00)

/**
 * Warning: These are also used as indices.
 */
#define LFS_DRIVER_FILE_ATTR_NRECORDS         (0x00)
#define LFS_DRIVER_FILE_ATTR_FIRST_RECORD     (0x01)
#define LFS_DRIVER_FILE_ATTR_TAIL_RECORD      (0x02)
#define LFS_DRIVER_FILE_ATTR_CONFIG_MODULES   (0x03)
#define LFS_DRIVER_FILE_ATTR_CONFIG_SCHEDULE  (0x04)
#define LFS_DRIVER_FILE_ATTR_CONFIG_STATE     (0x05)
#define LFS_DRIVER_FILE_ATTR_CONFIG_OTHER     (0x06)

typedef struct fklfs_attribute_template_t {
    uint8_t type;
    lfs_size_t size;
    uint8_t fill;
} fklfs_attribute_template_t;

constexpr size_t MaximumRecordSizeEncodedSize = 4;

class LfsDriver {
private:
    DataMemory *memory_{ nullptr };
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

    lfs_file_config make_data_cfg(Pool &pool);

    lfs_file_config make_meta_cfg(Pool &pool);

    int32_t get_number_of_files(const char *path);

    int32_t set_number_of_files(const char *path, int32_t value);

    int32_t add_number_of_files(const char *path, int32_t value);

    lfs_t *lfs() {
        return &lfs_;
    }

private:
    lfs_file_config make_file_cfg(fklfs_attribute_template_t const *attributes, lfs_size_t nattributes, Pool &pool);

};

}
