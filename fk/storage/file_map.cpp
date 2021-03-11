#include <tiny_printf.h>

#include "storage/file_map.h"

namespace fk {

FK_DECLARE_LOGGER("lfs");

FileMap::FileMap(LfsDriver *lfs, const char *directory, Pool &pool) : lfs_(lfs), directory_(directory) {
    path_ = (char *)pool.malloc(LFS_NAME_MAX);
    cache_pool_ = pool.subpool("file-map");
}

FileMap::~FileMap() {
}

bool FileMap::refresh() {
    int32_t number_of_files = 0;
    int32_t expected_number_of_files = lfs_->get_number_of_files(directory_);
    if (number_of_files < 0) {
        return false;
    }

    // Clear the pool and invalidate cache.
    cache_pool_->clear();
    cache_ = nullptr;

    lfs_dir_t dir;

    if (lfs_dir_open(lfs(), &dir, directory_) < 0) {
        return false;
    }

    // We're building a new cache.
    cache_entry_t *head = nullptr;
    cache_entry_t *tail = nullptr;

    struct lfs_info info;
    while (lfs_dir_read(lfs(), &dir, &info)) {
        if (info.name[0] == '.') {
            continue;
        }

        tiny_snprintf(path_, LFS_NAME_MAX, "%s/%s", directory_, info.name);

        uint32_t first_block = 0;
        if (lfs_getattr(lfs(), path_, LFS_DRIVER_FILE_ATTR_FIRST_BLOCK, &first_block, sizeof(first_block)) < 0) {
            return false;
        }

        uint32_t nblocks = 0;
        if (lfs_getattr(lfs(), path_, LFS_DRIVER_FILE_ATTR_NBLOCKS, &nblocks, sizeof(nblocks)) < 0) {
            return false;
        }

        loginfo("ls: '%s' type=%d size=%d attrs: first-block=%" PRIu32 " nblocks=%" PRIu32, info.name, info.type,
                info.size, first_block, nblocks);

        auto entry = (cache_entry_t *)cache_pool_->malloc(sizeof(cache_entry_t));
        entry->first_block = first_block;
        entry->nblocks = nblocks;
        entry->size = info.size;
        entry->np = nullptr;

        // TODO We be verifying the sort order.
        if (head == nullptr) {
            head = entry;
            tail = entry;
        }
        else {
            tail->np = entry;
            tail = entry;
        }

        number_of_files++;
    }

    lfs_dir_close(lfs(), &dir);

    // Only update the number of files when they change.
    if (number_of_files != expected_number_of_files) {
        lfs_->set_number_of_files(directory_, number_of_files);
    }

    cache_ = head;

    return true;
}

tl::expected<block_file_search_t, Error> FileMap::find(uint32_t desired_block, Pool &pool) {
    if (!initialized_) {
        if (!refresh()) {
            return tl::unexpected<Error>(Error::IO);
        }

        initialized_ = true;
    }

    uint32_t start_block_of_first_file{ UINT32_MAX };
    uint32_t start_block_of_last_file{ 0 };
    uint32_t bytes_before_start_of_last_file{ 0 };
    uint32_t last_block{ 0 };

    for (auto iter = cache_; iter != nullptr; iter = iter->np) {
        if (iter->first_block > start_block_of_last_file) {
            start_block_of_last_file = iter->first_block;
        }

        if (iter->first_block < start_block_of_first_file) {
            start_block_of_first_file = iter->first_block;
        }

        if (desired_block >= iter->first_block && desired_block < iter->first_block + iter->nblocks) {
            break;
        }

        bytes_before_start_of_last_file += iter->size;
    }

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
