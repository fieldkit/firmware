#pragma once

#include "uuid.h"

namespace fk {

int32_t fk_random_initialize();

int32_t fk_random_fill_u32(uint32_t *data, size_t size);

int32_t fk_random_fill_u8(uint8_t *data, size_t size);

int32_t fk_random_i32(int32_t start, int32_t end);

int32_t fk_uuid_generate(fk_uuid_t *uuid);

int32_t fk_uuid_sprintf(fk_uuid_t *uuid, fk_uuid_formatted_t *f);

}
