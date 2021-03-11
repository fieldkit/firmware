#pragma once

#include "storage/lfs_driver.h"
#include "storage/file_map.h"

namespace fk {

struct block_seek_t {
    uint32_t block;
    uint32_t first_block_of_containing_file;
    uint32_t absolute_position;
    uint32_t file_position;
};

class PartitionedReader : Reader {
private:
    LfsDriver *lfs_{ nullptr };
    FileMap *map_{ nullptr };
    bool initialized_{ false };
    char *path_{ nullptr };
    uint8_t *buffer_{ nullptr };
    lfs_size_t buffer_size_{ MaximumRecordSizeEncodedSize };
    lfs_file_t file_;
    lfs_file_config file_cfg_;
    bool opened_{ false };
    Pool *reader_pool_{ nullptr };

public:
    PartitionedReader(LfsDriver *lfs, FileMap *map, Pool &pool);

public:
    tl::expected<block_seek_t, Error> seek(uint32_t desired_block, Pool &pool);

    Reader *open_reader(Pool &pool);

    int32_t read(uint8_t *buffer, size_t size) override;

    void close();

private:
    lfs_t *lfs() {
        return lfs_->lfs();
    }

    const char *directory() {
        return map_->directory();
    }

};

}
