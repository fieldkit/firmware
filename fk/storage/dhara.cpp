#include "storage/dhara.h"
#include "storage/dhara_nand.h"
#include "utilities.h"
#include "lfs_util.h"

namespace fk {

FK_DECLARE_LOGGER("dhara");

Dhara::Dhara() {
}

Dhara::~Dhara() {
}

bool Dhara::begin(DataMemory *memory, bool force_create, Pool &pool) {
    auto g = memory->geometry();
    page_size_ = g.page_size;
    block_size_ = g.block_size;
    pool_ = &pool;

    loginfo("page-size: %zu block-size: %zu total-blocks: %zu total-size: %zu",
            page_size_, block_size_, g.nblocks, g.total_size);

    uint8_t log2_page_size = (uint8_t)lfs_npw2(page_size_);
    uint8_t log2_ppb = (uint8_t)lfs_npw2(block_size_ / page_size_);

    FK_ASSERT((size_t)(1 << log2_page_size) == page_size_);
    FK_ASSERT((size_t)(1 << log2_ppb) * page_size_ == block_size_);

    nand_ = {
        .dhara = {
            .log2_page_size = log2_page_size,
            .log2_ppb = log2_ppb,
            .num_blocks = g.nblocks,
        },
        .dn = new (pool) DharaNand(memory, (uint8_t *)pool.malloc(page_size_)),
    };

    auto buffer = (uint8_t *)pool.malloc(page_size_);

    dhara_error_t derr;

    dhara_map_init(&dmap_, &nand_.dhara, buffer, gc_ratio_);

    if (dhara_map_resume(&dmap_, &derr) < 0) {
        logerror("resume");
        dhara_map_clear(&dmap_);
    }

    if (force_create) {
        logwarn("force create, clearing");
        dhara_map_clear(&dmap_);
    }

    auto capacity = dhara_map_capacity(&dmap_);
    auto capacity_bytes = capacity * page_size_;

    auto loss = (float)(g.total_size - capacity_bytes) / g.total_size * 100.0f;

    loginfo("dmap: capacity=%d size=%d bytes=%" PRIu32 " loss=%.2f",
            capacity, dhara_map_size(&dmap_), capacity_bytes, loss);

    return true;
}

bool Dhara::write(dhara_sector_t sector, uint8_t const *data, size_t size) {
    dhara_error_t derr;

    logverbose("dhara-write: sector=%" PRIu32 " size=%" PRIu32, sector, size);

    FK_ASSERT(size == page_size_);
    if (dhara_map_write(&dmap_, sector, data, &derr) < 0) {
        logerror("write");
        return false;
    }
    return true;
}

bool Dhara::read(dhara_sector_t sector, uint8_t *data, size_t size) {
    dhara_error_t derr;

    logverbose("dhara-read: sector=%" PRIu32 " size=%" PRIu32, sector, size);

    FK_ASSERT(size == page_size_);
    if (dhara_map_read(&dmap_, sector, data, &derr) < 0) {
        logerror("read");
        return false;
    }
    return true;
}

bool Dhara::trim(dhara_sector_t sector) {
    dhara_error_t derr;

    logverbose("dhara-trim: sector=%" PRIu32 "" PRIu32, sector);

    if (dhara_map_trim(&dmap_, sector, &derr) < 0) {
        logerror("trim");
        return false;
    }
    return true;
}

void Dhara::clear() {
    dhara_map_clear(&dmap_);
}

bool Dhara::sync() {
    loginfo("sync!");

    dhara_error_t derr;
    if (dhara_map_sync(&dmap_, &derr) < 0) {
        logerror("sync");
        return false;
    }

    if (mapped_ != nullptr) {
        mapped_->log_statistics();
    }

    return true;
}

class DharaMemory : public DataMemory {
private:
    Dhara *dhara_{ nullptr };

public:
    DharaMemory(Dhara *dhara);

public:
    bool begin() override;

    FlashGeometry geometry() const override;

    int32_t read(uint32_t address, uint8_t *data, size_t length, MemoryReadFlags flags) override;

    int32_t write(uint32_t address, uint8_t const *data, size_t length, MemoryWriteFlags flags) override;

    int32_t erase(uint32_t address, size_t length) override;

    int32_t flush() override;

};

DataMemory *Dhara::map() {
    if (mapped_ == nullptr) {
        auto dm = new (pool_) DharaMemory(this);
        mapped_ = new (pool_) StatisticsMemory(dm);
    }
    return mapped_;
}

extern "C" {

/* Is the given block bad? */
int dhara_nand_is_bad(const struct dhara_nand *n, dhara_block_t b) {
    fk_dhara_t *fkd = (fk_dhara_t *)n;
    return fkd->dn->is_bad(n, b);
}

/* Mark bad the given block (or attempt to). No return value is
 * required, because there's nothing that can be done in response.
 */
void dhara_nand_mark_bad(const struct dhara_nand *n, dhara_block_t b) {
    fk_dhara_t *fkd = (fk_dhara_t *)n;
    fkd->dn->mark_bad(n, b);
}

/* Erase the given block. This function should return 0 on success or -1
 * on failure.
 *
 * The status reported by the chip should be checked. If an erase
 * operation fails, return -1 and set err to E_BAD_BLOCK.
 */
int dhara_nand_erase(const struct dhara_nand *n, dhara_block_t b, dhara_error_t *err) {
    fk_dhara_t *fkd = (fk_dhara_t *)n;
    return fkd->dn->erase(n, b, err);
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
    fk_dhara_t *fkd = (fk_dhara_t *)n;
    return fkd->dn->prog(n, p, data, err);
}

/* Check that the given page is erased */
int dhara_nand_is_free(const struct dhara_nand *n, dhara_page_t p) {
    fk_dhara_t *fkd = (fk_dhara_t *)n;
    return fkd->dn->is_free(n, p);
}

/* Read a portion of a page. ECC must be handled by the NAND
 * implementation. Returns 0 on sucess or -1 if an error occurs. If an
 * uncorrectable ECC error occurs, return -1 and set err to E_ECC.
 */
int dhara_nand_read(const struct dhara_nand *n, dhara_page_t p, size_t offset, size_t length, uint8_t *data,
                    dhara_error_t *err) {
    fk_dhara_t *fkd = (fk_dhara_t *)n;
    return fkd->dn->read(n, p, offset, length, data, err);
}

/* Read a page from one location and reprogram it in another location.
 * This might be done using the chip's internal buffers, but it must use
 * ECC.
 */
int dhara_nand_copy(const struct dhara_nand *n, dhara_page_t src, dhara_page_t dst, dhara_error_t *err) {
    fk_dhara_t *fkd = (fk_dhara_t *)n;
    return fkd->dn->copy(n, src, dst, err);
}

}

int32_t DharaNand::is_bad(const struct dhara_nand *n, dhara_block_t b) {
    loginfo("nand-is-bad: %d", b);
    return 0;
}

void DharaNand::mark_bad(const struct dhara_nand *n, dhara_block_t b) {
    loginfo("nand-mark-bad: %d", b);
}

int32_t DharaNand::erase(const struct dhara_nand *n, dhara_block_t b, dhara_error_t *err) {
    logdebug("nand-erase: %d", b);

    auto block_size = memory_->geometry().block_size;
    auto address = b * block_size;
    if (memory_->erase(address, block_size) < 0) {
        logerror("erase");
    }

    return 0;
}

int32_t DharaNand::prog(const struct dhara_nand *n, dhara_page_t p, const uint8_t *data, dhara_error_t *err) {
    logverbose("nand-prog: page=%d", p);

    auto page_size = memory_->geometry().page_size;
    auto address = p * page_size;
    auto nbytes = memory_->write(address, data, page_size);
    if (nbytes < 0) {
        logerror("writing");
        return nbytes;
    }

    return 0;
}

int32_t DharaNand::is_free(const struct dhara_nand *n, dhara_page_t p) {
    loginfo("nand-is-free: page=%d", p);
    return 0;
}

int32_t DharaNand::read(const struct dhara_nand *n, dhara_page_t p, size_t offset, size_t length, uint8_t *data,
                        dhara_error_t *err) {
    logverbose("nand-read: page=%d off=%d len=%d", p, offset, length);

    auto page_size = memory_->geometry().page_size;
    auto address = p * page_size + offset;
    auto nbytes = memory_->read(address, data, length);
    if (nbytes < 0) {
        return nbytes;
    }

    return 0;
}

int32_t DharaNand::copy(const struct dhara_nand *n, dhara_page_t src, dhara_page_t dst, dhara_error_t *err) {
    loginfo("nand-copy: %d -> %d", src, dst);

    auto page_size = memory_->geometry().page_size;

    if (memory_->read(src * page_size, copy_buffer_, page_size) < 0) {
        logerror("reading");
        return -1;
    }

    if (memory_->write(dst * page_size, copy_buffer_, page_size) < 0) {
        logerror("writing");
        return -1;
    }

    return 0;
}

DharaMemory::DharaMemory(Dhara *dhara) : dhara_(dhara) {
}

bool DharaMemory::begin() {
    return true;
}

FlashGeometry DharaMemory::geometry() const {
    auto page_size = dhara_->page_size();
    auto nsectors = dhara_->total_sectors();

    return {
        .page_size = page_size,
        .block_size = page_size,
        .nblocks = nsectors,
        .total_size = nsectors * page_size,
        .prog_size = page_size,
    };
}

int32_t DharaMemory::read(uint32_t address, uint8_t *data, size_t length, MemoryReadFlags flags) {
    auto sector = address / dhara_->page_size();

    logverbose("read: %" PRIu32 " size=%" PRIu32, address, length);

    if (!dhara_->read(sector, data, length)) {
        logerror("read");
        return -1;
    }

    return length;
}

int32_t DharaMemory::write(uint32_t address, uint8_t const *data, size_t length, MemoryWriteFlags flags) {
    auto sector = address / dhara_->page_size();

    logverbose("prog: %" PRIu32 " size=%" PRIu32, address, length);

    if (!dhara_->write(sector, data, length)) {
        return -1;
    }

    return length;
}

int32_t DharaMemory::erase(uint32_t address, size_t length) {
    auto sector = address / dhara_->page_size();

    loginfo("erase: %" PRIu32 " size=%" PRIu32 " (noop)", address, length);

    if (!dhara_->trim(sector)) {
        return -1;
    }

    return 0;
}

int32_t DharaMemory::flush() {
    return 0;
}

} // namespace fk
