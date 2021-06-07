#include "dhara_map.h"
#include "working_buffers.h"

namespace phylum {

// Find the smallest power of 2 greater than or equal to a
static inline uint32_t lfs_npw2(uint32_t a) {
#if !defined(LFS_NO_INTRINSICS) && (defined(__GNUC__) || defined(__CC_ARM))
    return 32 - __builtin_clz(a-1);
#else
    uint32_t r = 0;
    uint32_t s;
    a -= 1;
    s = (a > 0xffff) << 4; a >>= s; r |= s;
    s = (a > 0xff  ) << 3; a >>= s; r |= s;
    s = (a > 0xf   ) << 2; a >>= s; r |= s;
    s = (a > 0x3   ) << 1; a >>= s; r |= s;
    return (r | (a >> 1)) + 1;
#endif
}

dhara_sector_map::dhara_sector_map(working_buffers &buffers, flash_memory &target, sector_page_cache *page_cache) : buffers_(&buffers), target_(&target), page_cache_(page_cache) {
}

dhara_sector_map::~dhara_sector_map() {
}

int32_t dhara_sector_map::begin(bool force_create) {
    auto block_size = target_->block_size();
    auto nblocks = target_->number_blocks();
    auto page_size = target_->page_size();

    phydebugf("page-size: %zu block-size: %zu total-blocks: %zu", page_size, block_size, nblocks);

    uint8_t log2_page_size = (uint8_t)lfs_npw2(page_size);
    uint8_t log2_ppb = (uint8_t)lfs_npw2(block_size / page_size);

    assert((size_t)(1 << log2_page_size) == page_size);
    assert((size_t)(1 << log2_ppb) * page_size == block_size);

    nand_ = {
        .dhara = {
            .log2_page_size = log2_page_size,
            .log2_ppb = log2_ppb,
            .num_blocks = (unsigned)nblocks,
        },
        .dn = this,
    };

    page_size_ = page_size;
    block_size_ = block_size;
    nblocks_ = nblocks;

    if (!buffer_.valid()) {
        buffer_ = buffers_->allocate(page_size);
    }

    // Notice that this is just named this way to indicate that we're
    // doing something unusual compared to the other calls to
    // unsafe_all, in that dhara basically owns this buffer now.
    auto err = buffer_.unsafe_forever([&](uint8_t *ptr, size_t size) {
        assert(size == page_size);
        memset(&dmap_, 0, sizeof(struct dhara_map));
        dhara_map_init(&dmap_, &nand_.dhara, ptr, gc_ratio_);
        return 0;
    });
    if (err < 0) {
        return err;
    }

    dhara_error_t derr;
    if (force_create) {
        phywarnf("dhara clearing");
        dhara_map_clear(&dmap_);
    }
    else  {
        phydebugf("resuming");
        if (dhara_map_resume(&dmap_, &derr) < 0) {
            phyerrorf("dhara resume failed (%d)", derr);
            return -1;
        }
        phydebugf("dhara ready");
    }

    auto capacity = dhara_map_capacity(&dmap_);
    auto capacity_bytes = capacity * page_size_;
    auto total_size = nblocks * block_size;
    auto size = dhara_map_size(&dmap_);

    auto loss = (float)(total_size - capacity_bytes) / total_size * 100.0f;
    auto used = ((float)capacity / (float)size) * 100.0f;

    phyinfof("dmap: capacity=%d size=%d bytes=%" PRIu32 " total=%" PRIu32 " loss=%.2f used=%.2f",
             capacity, size, capacity_bytes, total_size, loss, used);

    return 0;
}

size_t dhara_sector_map::sector_size() {
    return page_size_;
}

dhara_sector_t dhara_sector_map::size() {
    return dhara_map_size(&dmap_);
}

int32_t dhara_sector_map::write(dhara_sector_t sector, uint8_t const *data, size_t size) {
    phydebugf("dhara-write sector=%" PRIu32 " size=%" PRIu32, sector, size);

    assert(page_size_ > 0);
    assert(size == page_size_);

    dhara_error_t derr;
    auto err = dhara_map_write(&dmap_, sector, data, &derr);
    if (err < 0) {
        phyerrorf("write");
        return err;
    }

    // This is easier than invalidating.
    dhara_page_t page = 0;
    err = dhara_map_find(&dmap_, sector, &page, &derr);
    if (err == 0) {
        page_cache_->set(sector, page);
    }

    if (false) {
        phydebug_dump_memory("write ", data, size);
    }

    return 0;
}

int32_t dhara_sector_map::find(dhara_sector_t sector, dhara_page_t *page) {
    phydebugf("dhara-find: sector=%" PRIu32 "", sector);

    assert(page_size_ > 0);

    dhara_error_t derr;
    auto err = dhara_map_find(&dmap_, sector, page, &derr);
    if (err < 0) {
        phywarnf("find");
        return err;
    }

    return 0;
}

int32_t dhara_sector_map::trim(dhara_sector_t sector) {
    phydebugf("dhara-trim sector=%" PRIu32 "", sector);

    assert(page_size_ > 0);

    dhara_error_t derr;
    auto err = dhara_map_trim(&dmap_, sector, &derr);
    if (err < 0) {
        phyerrorf("trim");
        return err;
    }

    return 0;
}

int32_t dhara_sector_map::read(dhara_sector_t sector, uint8_t *data, size_t size) {
    phydebugf("dhara-read sector=%" PRIu32 " size=%" PRIu32, sector, size);

    assert(page_size_ > 0);
    assert(size == page_size_);

    dhara_error_t derr;
    dhara_page_t page = 0;
    if (!page_cache_->get(sector, &page)) {
        auto err = dhara_map_find(&dmap_, sector, &page, &derr);
        if (err < 0) {
            phywarnf("cache-find");
            auto err = dhara_map_read(&dmap_, sector, data, &derr);
            if (err < 0) {
                phyerrorf("read");
                return err;
            }

            return err;
        }

        page_cache_->set(sector, page);
    }

    phydebugf("fast-dhara-read sector=%d page=%d", sector, page);
    auto err = dhara_nand_read(&nand_.dhara, page, 0, size, data, &derr);
    if (err < 0) {
        phyerrorf("read");
        return err;
    }

    if (false) {
        phydebug_dump_memory("read ", data, size);
    }

    return 0;
}

int32_t dhara_sector_map::clear() {
    dhara_map_clear(&dmap_);

    return 0;
}

int32_t dhara_sector_map::sync() {
    logged_task lt{ "dhara-sync" };

    phyinfof("syncing");

    dhara_error_t derr;
    auto err = dhara_map_sync(&dmap_, &derr);
    if (err < 0) {
        phyerrorf("sync");
        return err;
    }

    phydebugf("synced");

    return 0;
}

int dhara_sector_map::dhara_erase(const struct dhara_nand */*n*/, dhara_block_t b, dhara_error_t *err) {
    phydebugf("dhara-erase block=%" PRIu32 "", b);

    auto address = b * block_size_;
    if (target_->erase(address, block_size_) < 0) {
        phydebugf("erase");
        *err = DHARA_E_BAD_BLOCK;
        return -1;
    }

    if (err != nullptr) {
        *err = DHARA_E_NONE;
    }

    return 0;
}

int dhara_sector_map::dhara_prog(const struct dhara_nand */*n*/, dhara_page_t p, const uint8_t *data, dhara_error_t *err) {
    assert(page_size_ > 0);

    auto address = p * page_size_;
    auto nbytes = target_->write(address, data, page_size_);
    if (nbytes < 0) {
        phydebugf("writing");
        return nbytes;
    }

    if (err != nullptr) {
        *err = DHARA_E_NONE;
    }

    return 0;
}

int dhara_sector_map::dhara_is_bad(const struct dhara_nand */*n*/, dhara_block_t /*b*/) {
    return 0;
}

int dhara_sector_map::dhara_is_free(const struct dhara_nand */*n*/, dhara_page_t /*p*/) {
    return 0;
}

void dhara_sector_map::dhara_mark_bad(const struct dhara_nand */*n*/, dhara_block_t /*b*/) {
}

int dhara_sector_map::dhara_read(const struct dhara_nand */*n*/, dhara_page_t p, size_t offset, size_t length, uint8_t *data, dhara_error_t *err) {
    assert(page_size_ > 0);

    auto address = p * page_size_ + offset;
    auto nbytes = target_->read(address, data, length);
    if (nbytes < 0) {
        return nbytes;
    }

    if (err != nullptr) {
        *err = DHARA_E_NONE;
    }

    return 0;
}

int dhara_sector_map::dhara_copy(const struct dhara_nand */*n*/, dhara_page_t src, dhara_page_t dst, dhara_error_t *err) {
    assert(page_size_ > 0);

    if (target_->copy_page(src * page_size_, dst * page_size_, page_size_) < 0) {
        phydebugf("copy-page");
        return -1;
    }

    if (err != nullptr) {
        *err = DHARA_E_NONE;
    }

    return 0;
}

extern "C" {

#include <dhara/map.h>

/* Is the given block bad? */
int dhara_nand_is_bad(const struct dhara_nand *n, dhara_block_t b) {
    phylum_dhara_t *fkd = (phylum_dhara_t *)n;
    return fkd->dn->dhara_is_bad(n, b);
}

/* Mark bad the given block (or attempt to). No return value is
 * required, because there's nothing that can be done in response.
 */
void dhara_nand_mark_bad(const struct dhara_nand *n, dhara_block_t b) {
    phylum_dhara_t *fkd = (phylum_dhara_t *)n;
    fkd->dn->dhara_mark_bad(n, b);
}

/* Erase the given block. This function should return 0 on success or -1
 * on failure.
 *
 * The status reported by the chip should be checked. If an erase
 * operation fails, return -1 and set err to E_BAD_BLOCK.
 */
int dhara_nand_erase(const struct dhara_nand *n, dhara_block_t b, dhara_error_t *err) {
    phylum_dhara_t *fkd = (phylum_dhara_t *)n;
    return fkd->dn->dhara_erase(n, b, err);
}

/* Program the given page. The data pointer is a pointer to an entire
 * page ((1 << log2_page_size) bytes). The operation status should be
 * checked. If the operation fails, return -1 and set err to
 * E_BAD_BLOCK.
 *
 * Pages will be programmed sequentially within a block, and will not be
 * reprogrammed.
 */
int dhara_nand_prog(const struct dhara_nand *n, dhara_page_t p, const uint8_t *data, dhara_error_t *err) {
    phylum_dhara_t *fkd = (phylum_dhara_t *)n;
    return fkd->dn->dhara_prog(n, p, data, err);
}

/* Check that the given page is erased */
int dhara_nand_is_free(const struct dhara_nand *n, dhara_page_t p) {
    phylum_dhara_t *fkd = (phylum_dhara_t *)n;
    return fkd->dn->dhara_is_free(n, p);
}

/* Read a portion of a page. ECC must be handled by the NAND
 * implementation. Returns 0 on sucess or -1 if an error occurs. If an
 * uncorrectable ECC error occurs, return -1 and set err to E_ECC.
 */
int dhara_nand_read(const struct dhara_nand *n, dhara_page_t p, size_t offset, size_t length, uint8_t *data,
                    dhara_error_t *err) {
    phylum_dhara_t *fkd = (phylum_dhara_t *)n;
    return fkd->dn->dhara_read(n, p, offset, length, data, err);
}

/* Read a page from one location and reprogram it in another location.
 * This might be done using the chip's internal buffers, but it must use
 * ECC.
 */
int dhara_nand_copy(const struct dhara_nand *n, dhara_page_t src, dhara_page_t dst, dhara_error_t *err) {
    phylum_dhara_t *fkd = (phylum_dhara_t *)n;
    return fkd->dn->dhara_copy(n, src, dst, err);
}

}

} // namespace phylum
