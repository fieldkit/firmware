#include <tiny_printf.h>

#if defined(__SAMD51__)
#include <hal_rand_sync.h>
#endif
#if defined(linux)
#endif

#include "hal/hal.h"

namespace fk {

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

int32_t fk_uuid_generate(fk_uuid_t *uuid) {
    FK_ASSERT(fk_random_fill_u8(uuid->data, sizeof(uuid->data)) == 0);
    uuid->data[6] = 0x40 | (0x0F & uuid->data[6]);
    uuid->data[8] = 0x80 | (0x3F & uuid->data[8]);
    return true;
}

int32_t fk_uuid_sprintf(fk_uuid_t *uuid, fk_uuid_formatted_t *f) {
    auto p = f->str;
    size_t k = 0;
    for (size_t i = 0; i < sizeof(uuid->data); ++i) {
        if (i ==  4) p[k++] = '-';
        if (i ==  6) p[k++] = '-';
        if (i ==  8) p[k++] = '-';
        if (i == 10) p[k++] = '-';
        tiny_snprintf(p + k, 3, "%02x", uuid->data[i]);
        k += 2;
    }
    return true;
}

}
