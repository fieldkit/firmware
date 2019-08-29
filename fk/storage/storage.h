#pragma once

#include "common.h"
#include "hal/memory.h"
#include "progress.h"
#include "storage/file.h"

namespace fk {

/**
 * Format specifier for addresses.
 */
#define PRADDRESS                                 "0x%06" PRIx32

class Storage {
private:
    DataMemory *memory_;
    FileHeader files_[NumberOfFiles];
    uint32_t timestamp_{ InvalidTimestamp };
    uint32_t free_block_{ InvalidBlock };
    uint32_t version_{ InvalidVersion };
    bool read_only_;

public:
    Storage(DataMemory *memory, bool read_only = true);
    virtual ~Storage();

public:
    using FileNumber = uint8_t;
    constexpr static FileNumber Data = 0;
    constexpr static FileNumber Meta = 1;

    friend class File;

public:
    bool begin();
    bool clear();
    File file(FileNumber file);
    uint32_t fsck(ProgressCallbacks *progress);
    uint32_t version() const {
        return version_;
    }
    FileHeader const &file_header(FileNumber file) const {
        return files_[file];
    }

private:
    uint32_t allocate(uint8_t file, uint32_t previous_tail_address, BlockTail &block_tail);
    SeekValue seek(SeekSettings settings);
    void verify_opened() const;
    void verify_mutable() const;
    bool valid_block_header(BlockHeader &header) const;

};

}
