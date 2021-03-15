#include <dhara/map.h>
#include <dhara/nand.h>

#include "storage/dhara.h"

namespace fk {

FK_DECLARE_LOGGER("dhara");

class DharaNand {
private:
    DataMemory *memory_;

public:
    DharaNand(DataMemory *memory) : memory_(memory) {
    }

public:
    /*
     * Is the given block bad?
     */
    int32_t is_bad(const struct dhara_nand *n, dhara_block_t b);

    /*
     * Mark bad the given block (or attempt to). No return value is
     * required, because there's nothing that can be done in response.
     */
    void mark_bad(const struct dhara_nand *n, dhara_block_t b);

    /*
     * Erase the given block. This function should return 0 on success or -1
     * on failure.
     *
     * The status reported by the chip should be checked. If an erase
     * operation fails, return -1 and set err to E_BAD_BLOCK.
     */
    int32_t erase(const struct dhara_nand *n, dhara_block_t b, dhara_error_t *err);

    /*
     * Program the given page. The data pointer is a pointer to an entire
     * page ((1 << log2_page_size) bytes). The operation status should be
     * checked. If the operation fails, return -1 and set err to
     * E_BAD_BLOCK.
     *
     * Pages will be programmed sequentially within a block, and will not be
     * reprogrammed.
     */
    int32_t prog(const struct dhara_nand *n, dhara_page_t p, const uint8_t *data, dhara_error_t *err);

    /*
     * Check that the given page is erased.
     */
    int32_t is_free(const struct dhara_nand *n, dhara_page_t p);

    /*
     * Read a portion of a page. ECC must be handled by the NAND
     * implementation. Returns 0 on sucess or -1 if an error occurs. If an
     * uncorrectable ECC error occurs, return -1 and set err to E_ECC.
     */
    int32_t read(const struct dhara_nand *n, dhara_page_t p, size_t offset, size_t length, uint8_t *data,
                 dhara_error_t *err);

    /*
     * Read a page from one location and reprogram it in another location.
     * This might be done using the chip's internal buffers, but it must use
     * ECC.
     */
    int32_t copy(const struct dhara_nand *n, dhara_page_t src, dhara_page_t dst, dhara_error_t *err);
};

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
    loginfo("nand-is-bad:");
    return 0;
}

void DharaNand::mark_bad(const struct dhara_nand *n, dhara_block_t b) {
    loginfo("nand-mark-bad:");
}

int32_t DharaNand::erase(const struct dhara_nand *n, dhara_block_t b, dhara_error_t *err) {
    loginfo("nand-erase:");
    return 0;
}

int32_t DharaNand::prog(const struct dhara_nand *n, dhara_page_t p, const uint8_t *data, dhara_error_t *err) {
    loginfo("nand-prog:");
    return 0;
}

int32_t DharaNand::is_free(const struct dhara_nand *n, dhara_page_t p) {
    loginfo("nand-is-free:");
    return 0;
}

int32_t DharaNand::read(const struct dhara_nand *n, dhara_page_t p, size_t offset, size_t length, uint8_t *data,
                        dhara_error_t *err) {
    loginfo("nand-read:");
    return 0;
}

int32_t DharaNand::copy(const struct dhara_nand *n, dhara_page_t src, dhara_page_t dst, dhara_error_t *err) {
    loginfo("nand-copy:");
    return 0;
}

} // namespace fk
