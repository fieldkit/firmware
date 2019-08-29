#include <tiny_printf.h>

#if defined(__SAMD51__)
#include <hal_rand_sync.h>
#endif
#if defined(linux)
#endif

#include "hal/hal.h"

extern "C" {

#if defined(__SAMD51__)

struct rand_sync_desc random_main;

int32_t fk_random_initialize() {
    hri_mclk_set_APBCMASK_TRNG_bit(MCLK);
    rand_sync_init(&random_main, TRNG);
    rand_sync_enable(&random_main);
    return 0;
}

int32_t fk_random_fill_u32(uint32_t *data, size_t size) {
    rand_sync_read_buf32(&random_main, data, size);
    return 0;
}

int32_t fk_random_fill_u8(uint8_t *data, size_t size) {
    rand_sync_read_buf8(&random_main, data, size);
    return 0;
}

int32_t fk_random_i32(int32_t start, int32_t end) {
    return (rand_sync_read32(&random_main) % (end - start)) + start;
}

#endif

#if defined(linux)

int32_t fk_random_initialize() {
    return 0;
}

int32_t fk_random_fill_u32(uint32_t *data, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        *data = rand();
        data++;
    }
    return 0;
}

int32_t fk_random_fill_u8(uint8_t *data, size_t size) {
    FK_ASSERT((size % sizeof(uint32_t)) == 0);
    return fk_random_fill_u32((uint32_t *)data, size / sizeof(uint32_t));
}

int32_t fk_random_i32(int32_t start, int32_t end) {
    return (rand() % (end - start)) + start;
}

#endif

}
