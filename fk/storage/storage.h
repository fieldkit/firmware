#pragma once

#include "common.h"
#include "hal/memory.h"
#include "progress.h"
#include "progress_tracker.h"
#include "storage/file.h"
#include "storage/storage.h"
#include "storage/bad_blocks.h"
#include "storage/sequential_memory.h"
#include "storage/statistics_memory.h"

namespace fk {

/**
 * Format specifier for addresses.
 */
#define PRADDRESS                                 "0x%08" PRIx32

struct SavedState {
    FileHeader files[NumberOfFiles];
    uint32_t timestamp;
    uint32_t free_block;
    uint32_t version;
};

class Storage {
public:
    constexpr static FileNumber Data = 0;
    constexpr static FileNumber Meta = 1;

    friend class File;

private:
    DataMemory *data_memory_;
    Pool *pool_;
    SequentialWrapper<BufferedPageMemory> memory_;
    BadBlocks bad_blocks_;
    FileHeader files_[NumberOfFiles];
    BlockNumber file_headers_block_;
    uint32_t timestamp_{ InvalidTimestamp };
    BlockNumber free_block_{ InvalidBlock };
    uint32_t version_{ InvalidVersion };
    bool read_only_;

public:
    Storage(DataMemory *memory, Pool &pool, bool read_only = true);
    virtual ~Storage();

public:
    bool begin();
    bool clear();
    File file(FileNumber file_number);
    File file(FileNumber file_number, Pool &pool);
    bool flush();

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

    FlashGeometry geometry() const {
        return memory_.geometry();
    }

    bool is_block_bad(uint32_t block) const {
        return bad_blocks_.is_block_bad(block);
    }

private:
    SeekValue seek(SeekSettings settings);
    uint32_t allocate(uint8_t file, StorageAddress previous_tail_address, BlockTail &block_tail);
    bool valid_block_header(BlockHeader &header) const;
    void verify_opened() const;
    void verify_mutable() const;
    uint32_t fsck(File &opened_file, ProgressTracker &tracker);

    struct BlocksAfter {
        BlockNumber starting;
        BlockNumber free;
        BlockNumber tail;

        BlocksAfter() : starting(InvalidAddress), free(InvalidAddress), tail(InvalidAddress) {
        }

        BlocksAfter(BlockNumber starting, BlockNumber free, BlockNumber tail) : starting(starting), free(free), tail(tail) {
        }

        operator bool() const {
            return starting != InvalidAddress && free != InvalidAddress && tail != InvalidAddress;
        }
    };

    BlocksAfter find_blocks_after(uint32_t starting_block, FileNumber file, bool end);

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
