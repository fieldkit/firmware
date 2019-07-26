#pragma once

namespace fk {

int32_t fk_random_initialize();

int32_t fk_random_get(uint32_t *data, size_t size);

int32_t fk_random_i32(int32_t start, int32_t end);

}
