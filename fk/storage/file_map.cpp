#include <tiny_printf.h>

#include "storage/file_map.h"

namespace fk {

FK_DECLARE_LOGGER("lfs");

FileMap::FileMap(LfsDriver *lfs, const char *directory, Pool &pool) : lfs_(lfs), directory_(directory) {
    path_ = (char *)pool.malloc(LFS_NAME_MAX);
}

bool FileMap::refresh(Pool &pool) {
    if (!find(UINT32_MAX, pool)) {
        return false;
    }
    return true;
}

tl::expected<block_file_search_t, Error> FileMap::find(uint32_t desired_block, Pool &pool) {
    uint32_t start_block_of_first_file{ UINT32_MAX };
    uint32_t start_block_of_last_file{ 0 };
    uint32_t bytes_before_start_of_last_file{ 0 };
    uint32_t last_block{ 0 };

    lfs_dir_t dir;
    lfs_dir_open(lfs(), &dir, directory_);

    /**
     * This is basically an array of indices/block numbers and
     * their lengths, which for full memory consumes
     *
     * (4 + 4) * (NBLOCKS / BLOCKS_PER_FILE) bytes
     *
     * You can even get this down to 4 bytes if you assume
     * contiguous ranges, and verify before assuming.
     *
     * Which will easily sit inside a single page of memory:
     *
     * 8192 / 4 = 2048 files, or one file per block on two chip.
     */
    struct lfs_info info;
    while (lfs_dir_read(lfs(), &dir, &info)) {
        if (info.name[0] == '.') {
            continue;
        }

        tiny_snprintf(path_, LFS_NAME_MAX, "%s/%s", directory_, info.name);

        uint32_t first_block = 0;
        lfs_getattr(lfs(), path_, LFS_DRIVER_ATTR_FIRST_BLOCK, &first_block, sizeof(first_block));

        uint32_t nblocks = 0;
        lfs_getattr(lfs(), path_, LFS_DRIVER_ATTR_NBLOCKS, &nblocks, sizeof(nblocks));

        loginfo("ls: '%s' type=%d size=%d attrs: first-block=%" PRIu32 " nblocks=%" PRIu32, info.name, info.type,
                info.size, first_block, nblocks);

        if (first_block > start_block_of_last_file) {
            start_block_of_last_file = first_block;
        }

        if (first_block < start_block_of_first_file) {
            start_block_of_first_file = first_block;
        }

        bytes_before_start_of_last_file += info.size;

        if (desired_block >= first_block && desired_block < first_block + nblocks) {
            break;
        }
    }

    lfs_dir_close(lfs(), &dir);

    return block_file_search_t{
        .start_block_of_first_file = start_block_of_first_file,
        .start_block_of_last_file = start_block_of_last_file,
        .bytes_before_start_of_last_file = bytes_before_start_of_last_file,
        .last_block = last_block,
    };
}

void FileMap::invalidate() {
    initialized_ = false;
}

} // namespace fk
