#pragma once

#include "storage/lfs_driver.h"
#include "storage/file_map.h"

namespace fk {

class PartitionedReader {
private:
    LfsDriver *lfs_{ nullptr };
    FileMap *map_{ nullptr };
    bool initialized_{ false };
    char *path_{ nullptr };
    uint8_t *buffer_{ nullptr };
    lfs_size_t buffer_size_{ MaximumRecordSizeEncodedSize };

public:
    PartitionedReader(LfsDriver *lfs, FileMap *map, Pool &pool);

public:
    bool seek(uint32_t desired_block, Pool &pool);

private:
    lfs_t *lfs() {
        return lfs_->lfs();
    }

    const char *directory() {
        return map_->directory();
    }

};

}
