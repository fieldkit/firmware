#pragma once

extern "C" {

#include <dhara/error.h>
#include <dhara/map.h>

}

#include "phylum.h"
#include "sector_map.h"
#include "flash_memory.h"
#include "simple_buffer.h"
#include "page_cache.h"

namespace phylum {

class working_buffers;
class dhara_sector_map;

typedef struct phylum_dhara_t {
    struct dhara_nand dhara;
    dhara_sector_map *dn;
} phylum_dhara_t;

class dhara_sector_map : public sector_map {
private:
    working_buffers *buffers_{ nullptr };
    flash_memory *target_{ nullptr };
    sector_page_cache *page_cache_{ nullptr };
    simple_buffer buffer_;
    phylum_dhara_t nand_;
    struct dhara_map dmap_;
    uint8_t gc_ratio_{ 20 };
    uint32_t page_size_{ 0 };
    uint32_t block_size_{ 0 };
    uint32_t nblocks_{ 0 };

public:
    dhara_sector_map(working_buffers &buffers, flash_memory &target, sector_page_cache *page_cache);
    virtual ~dhara_sector_map();

public:
    int32_t begin(bool force_create) override;
    size_t sector_size() override;
    dhara_sector_t size() override;
    int32_t find(dhara_sector_t sector, dhara_page_t *page) override;
    int32_t trim(dhara_sector_t sector) override;
    int32_t read(dhara_sector_t sector, uint8_t *data, size_t size) override;
    int32_t write(dhara_sector_t sector, uint8_t const *data, size_t size) override;
    int32_t clear() override;
    int32_t sync() override;

public:
    int dhara_erase(const struct dhara_nand *n, dhara_block_t b, dhara_error_t *err);
    int dhara_prog(const struct dhara_nand *n, dhara_page_t p, const uint8_t *data, dhara_error_t *err);
    int dhara_is_bad(const struct dhara_nand *n, dhara_block_t b);
    int dhara_is_free(const struct dhara_nand *n, dhara_page_t p);
    void dhara_mark_bad(const struct dhara_nand *n, dhara_block_t b);
    int dhara_read(const struct dhara_nand *n, dhara_page_t p, size_t offset, size_t length, uint8_t *data, dhara_error_t *err);
    int dhara_copy(const struct dhara_nand *n, dhara_page_t src, dhara_page_t dst, dhara_error_t *err);
};

} // namespace phylum
