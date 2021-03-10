#pragma once

#include "storage/lfs_driver.h"

namespace fk {

class PartitionedReader {
private:
    LfsDriver *lfs_{ nullptr };
    const char *directory_{ nullptr };
    bool initialized_{ false };
    char *path_{ nullptr };
    uint8_t *buffer_{ nullptr };
    lfs_size_t buffer_size_{ MaximumRecordSizeEncodedSize };

public:
    PartitionedReader(LfsDriver *lfs, const char *directory, Pool &pool);

public:
    bool seek(uint32_t desired_block, Pool &pool);

private:
    lfs_t *lfs() {
        return lfs_->lfs();
    }

};

}
