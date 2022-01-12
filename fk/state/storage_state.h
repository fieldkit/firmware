#pragma once

#include "common.h"

namespace fk {

struct StreamState {
    uint32_t size{ 0 };
    uint32_t block{ 0 };
    uint32_t modified{ 0 };
};

struct MemoryState {
    uint32_t installed{ 0 };
    uint32_t used{ 0 };
};

struct StorageState {
    MemoryState spi;
    MemoryState qspi;
    StreamState data;
    StreamState meta;

    bool is_phylum() const {
        return data.block > 0 && meta.block == 0;
    }
};

struct StorageStreamUpdate {
    uint32_t size;
    uint32_t records;
};

struct StorageUpdate {
    StorageStreamUpdate meta;
    StorageStreamUpdate data;
    uint32_t nreadings;
    uint32_t installed;
    uint32_t used;
    uint32_t time;
};

struct SdCardState {
    bool found{ false };
    bool working{ false };
    uint32_t size{ 0 };
    uint32_t remaining{ 0 };
    uint32_t written{ 0 };
};

} // namespace fk
