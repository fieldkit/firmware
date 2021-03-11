#pragma once

#include "storage/lfs_driver.h"
#include "storage/file_map.h"
#include "storage/lfs_attributes.h"
#include "records.h"

namespace fk {

class FileSizeRollover {
private:
    lfs_size_t size_;

public:
    FileSizeRollover(lfs_size_t size) : size_(size) {
    }

public:
    bool should_rollover(lfs_t *lfs, lfs_file_t *file);

};

struct appended_block_t {
    uint32_t block;
    uint32_t first_block_of_containing_file;
    lfs_size_t absolute_position;
    lfs_size_t file_position;
    lfs_size_t record_size;
};

using AppendedBlockOrError = tl::expected<appended_block_t, Error>;

class BlockAppender {
private:
    LfsDriver *lfs_{ nullptr };
    FileMap *map_{ nullptr };
    FileSizeRollover strategy_;
    uint32_t start_block_of_last_file_{ 0 };
    uint32_t bytes_before_start_of_last_file_{ 0 };
    bool initialized_{ false };
    char *path_{ nullptr };

public:
    BlockAppender(LfsDriver *lfs, FileMap *map, lfs_size_t rollover_size, Pool &pool);

public:
    bool create_directory_if_necessary();

    AppendedBlockOrError append_data_record(fk_data_DataRecord *record, Pool &pool);

    AppendedBlockOrError append_changes(uint8_t kind, void *record, pb_msgdesc_t const *fields, Pool &pool);

private:
    optional<Error> locate_tail(Pool &pool);

    AppendedBlockOrError write_record(lfs_file_t &file, Attributes &attributes, void *record, pb_msgdesc_t const *fields, Pool &pool);

private:
    lfs_t *lfs() {
        return lfs_->lfs();
    }

    const char *directory() {
        return map_->directory();
    }

    bool should_rollover(lfs_file_t *file) {
        return strategy_.should_rollover(lfs(), file);
    }

};

} // namespace fk
