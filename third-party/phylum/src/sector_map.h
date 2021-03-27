#pragma once

#include <cinttypes>
#include <cstdlib>

namespace phylum {

typedef uint32_t dhara_block_t;
typedef uint32_t dhara_sector_t;
typedef uint32_t dhara_page_t;

class sector_map {
public:
    virtual ~sector_map() { }

public:
    virtual int32_t begin(bool force_create) = 0;
    virtual size_t sector_size() = 0;
    virtual dhara_sector_t size() = 0;
    virtual int32_t find(dhara_sector_t sector, dhara_page_t *page) = 0;
    virtual int32_t trim(dhara_sector_t sector) = 0;
    virtual int32_t read(dhara_sector_t sector, uint8_t *data, size_t size) = 0;
    virtual int32_t write(dhara_sector_t sector, uint8_t const *data, size_t size) = 0;
    virtual int32_t clear() = 0;
    virtual int32_t sync() = 0;
};

} // namespace phylum
