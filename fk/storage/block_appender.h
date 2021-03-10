#pragma once

#include "storage/lfs_driver.h"
#include "storage/file_map.h"
#include "records.h"

namespace fk {

class BlockAppender {
private:
    LfsDriver *lfs_{ nullptr };
    FileMap *map_{ nullptr };
    uint32_t start_block_of_last_file_{ 0 };
    bool initialized_{ false };
    char *path_{ nullptr };

public:
    BlockAppender(LfsDriver *lfs, FileMap *map, Pool &pool);

public:
    bool create_directory_if_necessary();

    bool append(fk_data_DataRecord *record, Pool &pool);

private:
    lfs_t *lfs() {
        return lfs_->lfs();
    }

    const char *directory() {
        return map_->directory();
    }

    bool should_rollover(lfs_file_t *file);

};

} // namespace fk
