#pragma once

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct fk_uuid_t {
    uint8_t data[16];
} fk_uuid_t;

typedef struct fk_uuid_formatted_t {
    char str[16 * 2 + 1 + 4];
} fk_uuid_formatted_t;

#ifdef __cplusplus
}
#endif
