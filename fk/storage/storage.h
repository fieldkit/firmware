#pragma once

#include "common.h"
#include "hal/hal.h"
#include "storage/file.h"

namespace fk {

class Storage {
private:
    DataMemory *memory_;
    FileHeader files_[NumberOfFiles];
    uint32_t timestamp_{ InvalidTimestamp };
    uint32_t free_block_{ 0 };

public:
    Storage(DataMemory *memory);
    virtual ~Storage();

public:
    friend class File;

public:
    bool begin();
    bool clear();
    File file(uint8_t file);
    uint32_t fsck();

private:
    uint32_t allocate(uint8_t file, uint32_t first, uint32_t tail_address);
    SeekValue seek(SeekSettings settings);

};

}
