#pragma once

#if defined(linux)

#include <algorithm>

#include "hal/memory.h"
#include "hal/linux/debug_log.h"

namespace fk {

class LinuxDataMemory : public DataMemory {
private:
    /* These mimic a single bank of memory. */
    constexpr static uint32_t PageSize = 2048;
    constexpr static uint32_t BlockSize = 2048 * 64;
    constexpr static uint32_t NumberOfBlocks = 2048;

private:
    StorageLog log_;
    flash_geometry_t geometry_;
    uint8_t *memory_{ nullptr };
    size_t size_{ 0 };

public:
    static uint8_t EraseByte;

public:
    LinuxDataMemory();

public:
    bool begin() override;

    flash_geometry_t geometry() const override;

    size_t read(uint32_t address, uint8_t *data, size_t length) override;

    size_t write(uint32_t address, const uint8_t *data, size_t length) override;

    size_t erase_block(uint32_t address) override;

    size_t flush() override;

    void erase_all();

public:
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
            return start <= r.end() && r.start <= end();
        }
    };

    std::list<Region> bad_regions_;
    std::list<uint32_t> bad_blocks_;

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

    void mark_block_bad(uint32_t address) {
        bad_blocks_.emplace_back(address);
    }

    bool affects_bad_block(uint32_t address) {
        return std::find(bad_blocks_.begin(), bad_blocks_.end(), address) != bad_blocks_.end();
    }

};

}

#endif
