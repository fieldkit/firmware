#pragma once

#include <tl/expected.hpp>

#include "storage/lfs_driver.h"

namespace fk {

struct block_file_search_t {
    uint32_t start_block_of_first_file;
    uint32_t start_block_of_last_file;
    uint32_t bytes_before_start_of_last_file;
    uint32_t last_block;
};

class FileMap {
private:
    LfsDriver *lfs_{ nullptr };
    const char *directory_{ nullptr };
    char *path_{ nullptr };
    bool initialized_{ false };

public:
    FileMap(LfsDriver *lfs, const char *directory, Pool &pool);

public:
    const char *directory() {
        return directory_;
    }

public:
    bool refresh(Pool &pool);

    tl::expected<block_file_search_t, Error> find(uint32_t desired_block, Pool &pool);

    void invalidate();

private:
    lfs_t *lfs() {
        return lfs_->lfs();
    }
};

} // namespace fk
