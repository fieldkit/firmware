#pragma once

#include <inttypes.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct fk_uuid_t {
    uint8_t data[16];
} fk_uuid_t;

int32_t fk_uuid_generate(fk_uuid_t *uuid);

typedef struct fk_uuid_formatted_t {
    char str[16 * 2 + 1 + 4];
} fk_uuid_formatted_t;

int32_t fk_uuid_sprintf(fk_uuid_t const *uuid, fk_uuid_formatted_t *f);

int32_t fk_uuid_is_valid(fk_uuid_t const *uuid);

#ifdef __cplusplus
}
#endif
