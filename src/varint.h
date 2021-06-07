#pragma once

#include <cstdlib>
#include <cinttypes>

namespace phylum {

// https://github.com/sorribas/varint.c.git

size_t varint_encoding_length(unsigned long long n);

uint8_t *varint_encode(unsigned long long n, uint8_t *buf, int32_t len);

unsigned long long varint_decode(uint8_t const *buf, size_t len, int32_t *err);

} // namespace phylum
