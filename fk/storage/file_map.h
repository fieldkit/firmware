#pragma once

#include "storage/lfs_driver.h"

namespace fk {

class FileMap {
private:
    LfsDriver *lfs_{ nullptr };
    char *path_{ nullptr };
    uint32_t first_file_{ 0 };
    uint32_t start_of_last_file_{ 0 };
    uint32_t last_block_{ 0 };
    uint32_t bytes_traversed_{ 0 };
    bool initialized_{ false };

public:
    FileMap(LfsDriver *lfs, Pool &pool);

public:
    uint32_t first_file() {
        return first_file_;
    }

    uint32_t start_of_last_file() {
        return start_of_last_file_;
    }

    uint32_t last_block() {
        return last_block_;
    }

    uint32_t bytes_traversed() {
        return bytes_traversed_;
    }

public:
    bool refresh(const char *directory, uint32_t desired_block, Pool &pool);

    bool refresh(const char *directory, Pool &pool);

private:
    lfs_t *lfs() {
        return lfs_->lfs();
    }
};

} // namespace fk
