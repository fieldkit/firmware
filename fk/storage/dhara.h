#pragma once

extern "C" {

#include <dhara/map.h>
#include <dhara/nand.h>

}

#include "hal/memory.h"
#include "storage/statistics_memory.h"
#include "common.h"
#include "pool.h"

namespace fk {

class DharaNand;

typedef struct fk_dhara_t {
    struct dhara_nand dhara;
    DharaNand *dn;
} fk_dhara_t;

class Dhara {
private:
    Pool *pool_{ nullptr };
    fk_dhara_t nand_;
    struct dhara_map dmap_;
    uint8_t gc_ratio_{ 10 };
    uint32_t page_size_{ 0 };
    uint32_t block_size_{ 0 };
    StatisticsMemory *mapped_{ nullptr };

public:
    Dhara();
    virtual ~Dhara();

public:
    bool begin(DataMemory *memory, bool force_create, Pool &pool);
    bool write(dhara_sector_t sector, uint8_t const *data, size_t size);
    bool read(dhara_sector_t sector, uint8_t *data, size_t size);
    bool trim(dhara_sector_t sector);
    void clear();
    bool sync();

public:
    uint32_t page_size() const {
        return page_size_;
    }

    uint32_t block_size() const {
        return block_size_;
    }

    uint32_t total_sectors() const {
        return dhara_map_capacity(&dmap_);
    }

    uint32_t used_sectors() const {
        return dhara_map_size(&dmap_);
    }

    DataMemory *map();

};

} // namespace fk
