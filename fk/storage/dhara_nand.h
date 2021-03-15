#pragma once

extern "C" {

#include <dhara/map.h>
#include <dhara/nand.h>

}

#include "hal/memory.h"

namespace fk {

class DharaNand {
private:
    DataMemory *memory_;
    uint8_t *copy_buffer_{ nullptr };

public:
    DharaNand(DataMemory *memory, uint8_t *copy_buffer) : memory_(memory), copy_buffer_(copy_buffer) {
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

} // namespace fk
