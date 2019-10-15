#pragma once

#include "common.h"
#include "hal/memory.h"
#include "progress.h"
#include "storage/file.h"
#include "storage/progress_tracker.h"

namespace fk {

/**
 * Format specifier for addresses.
 */
#define PRADDRESS                                 "0x%06" PRIx32

struct SavedState {
    FileHeader files[NumberOfFiles];
    uint32_t timestamp;
    uint32_t free_block;
    uint32_t version;
};

class Storage {
public:
    using FileNumber = uint8_t;
    constexpr static FileNumber Data = 0;
    constexpr static FileNumber Meta = 1;

    friend class File;

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
    bool begin();
    bool clear();
    File file(FileNumber file);

public:
    uint32_t fsck(ProgressCallbacks *progress);

public:
    SavedState save() const;
    void restore(SavedState const &state);

public:
    uint32_t timestamp() const {
        return timestamp_;
    }

    uint32_t version() const {
        return version_;
    }

    uint32_t free_block() const {
        return free_block_;
    }

    FileHeader const &file_header(FileNumber file) const {
        return files_[file];
    }

    flash_geometry_t geometry() const {
        return memory_->geometry();
    }

private:
    SeekValue seek(SeekSettings settings);
    uint32_t allocate(uint8_t file, uint32_t previous_tail_address, BlockTail &block_tail);
    bool valid_block_header(BlockHeader &header) const;
    void verify_opened() const;
    void verify_mutable() const;
    uint32_t fsck(File &opened_file, ProgressTracker &tracker);

};

inline bool is_memory_completely(uint8_t const *data, size_t size, uint8_t byte) {
    for (auto i = 0u; i < size; ++i) {
        if (data[i] != byte) {
            return false;
        }
    }
    return true;
}

inline bool is_memory_erased(uint8_t const *data, size_t size) {
    return is_memory_completely(data, size, 0xff);
}

inline bool is_memory_zeros(uint8_t const *data, size_t size) {
    return is_memory_completely(data, size, 0x00);
}

}
