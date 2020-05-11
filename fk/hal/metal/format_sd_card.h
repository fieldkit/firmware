#pragma once

#if defined(__SAMD51__)

#include "common.h"

#include <SdFat.h>
#include <sdios.h>

namespace fk {

class FormatSdCard {
private:
    SdFatSpiDriver spi_driver_;
    SdSpiCard card_;
    cache_t cache_;

    uint32_t card_size_blocks_{ 0 };
    uint32_t card_capacity_mb_{ 0 };
    uint32_t sectors_per_cluster_{ 0 };
    uint32_t number_of_heads_{ 0 };
    uint32_t sectors_per_track_{ 0 };
    uint8_t partition_type_{ 0 };
    uint32_t relative_sector_{ 0 };
    uint32_t partition_size_{ 0 };

    uint16_t const BU16 = 128;
    uint16_t const BU32 = 8192;

public:
    FormatSdCard() { }

public:
    bool begin();
    bool erase();
    bool format();

private:
    uint8_t write_cache(uint32_t lbn) {
        return card_.writeBlock(lbn, cache_.data);
    }

    uint8_t lbn_to_head(uint32_t lbn) {
        return (lbn % (number_of_heads_ * sectors_per_track_)) / sectors_per_track_;
    }

    uint8_t lbn_to_sector(uint32_t lbn) {
        return (lbn % sectors_per_track_) + 1;
    }

    uint16_t lbn_to_cylinder(uint32_t lbn) {
        return lbn / (number_of_heads_ * sectors_per_track_);
    }

    void clear_cache(uint8_t add_sig) {
        memset(&cache_, 0, sizeof(cache_));
        if (add_sig) {
            cache_.mbr.mbrSig0 = BOOTSIG0;
            cache_.mbr.mbrSig1 = BOOTSIG1;
        }
    }

    bool write_mbr();
    uint32_t volume_serial_number();
    bool clear_fat_directory(uint32_t bgn, uint32_t count);
};

}

#endif
