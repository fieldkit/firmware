#pragma once

#include <tl/expected.hpp>

#include "storage/lfs_driver.h"

namespace fk {

struct record_file_search_t {
    uint32_t start_record_of_first_file;
    uint32_t start_record_of_last_file;
    uint32_t bytes_before_start_of_last_file;
    uint32_t last_record;
};

class FileMap {
private:
    struct cache_entry_t {
        uint32_t first_record;
        uint32_t nrecords;
        uint32_t size;
        cache_entry_t *np;
    };

private:
    LfsDriver *lfs_{ nullptr };
    const char *directory_{ nullptr };
    int32_t number_of_files_to_keep_{ 0u };
    Pool *cache_pool_{ nullptr };
    cache_entry_t *cache_{ nullptr };
    char *path_{ nullptr };
    bool initialized_{ false };

public:
    FileMap(LfsDriver *lfs, const char *directory, int32_t number_of_files_to_keep, Pool &pool);
    virtual ~FileMap();

public:
    const char *directory() {
        return directory_;
    }

public:
    bool refresh();

    tl::expected<record_file_search_t, Error> find(uint32_t desired_record, Pool &pool);

    void invalidate();

    bool prune();

private:
    lfs_t *lfs() {
        return lfs_->lfs();
    }
};

} // namespace fk
