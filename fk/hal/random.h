#pragma once

#ifdef __cplusplus
extern "C" {
#endif

int32_t fk_random_initialize();

int32_t fk_random_fill_u32(uint32_t *data, size_t size);

int32_t fk_random_fill_u8(uint8_t *data, size_t size);

int32_t fk_random_i32(int32_t start, int32_t end);

#ifdef __cplusplus
}
#endif
