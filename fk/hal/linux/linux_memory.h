#pragma once

#if defined(linux)

#include <algorithm>

#include "hal/memory.h"
#include "hal/linux/debug_log.h"

namespace fk {

class LinuxDataMemory : public ExecutableMemory {
private:
    /* These mimic a single bank of memory. */
    constexpr static uint32_t PageSize = 2048;
    constexpr static uint32_t BlockSize = 2048 * 64;
    constexpr static uint32_t NumberOfBlocks = 2048;

private:
    StorageLog log_;
    FlashGeometry geometry_;
    uint8_t *memory_{ nullptr };
    size_t size_{ 0 };

public:
    static uint8_t EraseByte;

public:
    LinuxDataMemory();

public:
    bool begin() override;

    FlashGeometry geometry() const override;

    int32_t read(uint32_t address, uint8_t *data, size_t length, MemoryReadFlags flags) override;

    int32_t write(uint32_t address, const uint8_t *data, size_t length, MemoryWriteFlags flags) override;

    int32_t erase_block(uint32_t address) override;

    int32_t flush() override;

    void erase_all();

    int32_t execute(uint32_t *got, uint32_t *entry) override;

public:
    uint8_t *memory() {
        return memory_;
    }

    StorageLog &log() {
        return log_;
    }

private:
    struct Region {
        uint32_t start;
        uint32_t length;

        uint32_t end() const {
            return start + length;
        }

        Region(uint32_t start, uint32_t length) : start(start), length(length) {
        }

        bool overlaps(Region const &r) const {
            return start < r.end() && r.start < end();
        }
    };

    struct BadBlock {
        uint32_t address;
        bool factory{ false };

        BadBlock(uint32_t address, bool factory) : address(address), factory(factory) {
        }

        bool overlaps(uint32_t test) const {
            // TODO Magic number
            return test >= address && test < address + (2048 * 64);
        }
    };

    std::list<Region> bad_regions_;
    std::list<BadBlock> bad_blocks_;

public:
    void mark_region_bad(uint32_t start, uint32_t length) {
        bad_regions_.emplace_back(Region{ start, length });
    }

    void clear_bad_regions() {
        bad_regions_.clear();
        bad_blocks_.clear();
    }

    bool affects_bad_region(uint32_t start, uint32_t length) {
        auto test = Region{start, length};
        for (auto &r : bad_regions_) {
            if (r.overlaps(test)) {
                return true;
            }
        }
        return false;
    }

    void mark_block_bad_from_wear(uint32_t address) {
        bad_blocks_.emplace_back(BadBlock{ address, false });
    }

    void mark_block_bad_from_factory(uint32_t address) {
        bad_blocks_.emplace_back(BadBlock{ address, true });
    }

    bool affects_bad_block_from_wear(uint32_t address) {
        for (auto &b : bad_blocks_) {
            if (b.overlaps(address)) {
                return !b.factory;
            }
        }
        return false;
    }

    bool affects_bad_block_from_factory(uint32_t address) {
        for (auto &b : bad_blocks_) {
            if (b.overlaps(address)) {
                return b.factory;
            }
        }
        return false;
    }

};

}

#endif
