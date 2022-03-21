#include <tiny_printf.h>

#include "uuid.h"
#include "hal/random.h"

int32_t fk_uuid_generate(fk_uuid_t *uuid) {
    if (fk_random_fill_u8(uuid->data, sizeof(uuid->data)) != 0) {
        return -1;
    }
    uuid->data[6] = 0x40 | (0x0F & uuid->data[6]);
    uuid->data[8] = 0x80 | (0x3F & uuid->data[8]);
    return 0;
}

int32_t fk_uuid_sprintf(fk_uuid_t const *uuid, fk_uuid_formatted_t *f) {
    char *p = f->str;
    size_t k = 0;
    for (size_t i = 0; i < sizeof(uuid->data); ++i) {
        if (i == 4)
            p[k++] = '-';
        if (i == 6)
            p[k++] = '-';
        if (i == 8)
            p[k++] = '-';
        if (i == 10)
            p[k++] = '-';
        tiny_snprintf(p + k, 3, "%02x", uuid->data[i]);
        k += 2;
    }
    return 0;
}

int32_t fk_uuid_is_valid(fk_uuid_t const *uuid) {
    for (size_t i = 0; i < sizeof(uuid->data); ++i) {
        if (uuid->data[i] != 0xff) {
            return 1;
        }
    }
    return 0;
}