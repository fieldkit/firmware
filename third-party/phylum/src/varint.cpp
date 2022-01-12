// https://github.com/sorribas/varint.c.git
#include <assert.h>
#include "varint.h"

namespace phylum {

static const char MSB = (char)0x80;
static const char MSBALL = (char)~0x7F;

static const unsigned long long N1 = 128; // 2 ^ 7
static const unsigned long long N2 = 16384;
static const unsigned long long N3 = 2097152;
static const unsigned long long N4 = 268435456;
static const unsigned long long N5 = 34359738368;
static const unsigned long long N6 = 4398046511104;
static const unsigned long long N7 = 562949953421312;
static const unsigned long long N8 = 72057594037927936;
static const unsigned long long N9 = 9223372036854775808U;

size_t varint_encoding_length(unsigned long long n) {
    return (
        n < N1 ? 1
        : n < N2 ? 2
        : n < N3 ? 3
        : n < N4 ? 4
        : n < N5 ? 5
        : n < N6 ? 6
        : n < N7 ? 7
        : n < N8 ? 8
        : n < N9 ? 9
        :         10
        );
}

uint8_t *varint_encode(unsigned long long n, uint8_t *buf, int32_t len) {
    uint8_t *ptr = buf;

    while (n > 127) {
        *(ptr++) = (n & 0xFF) | MSB;
        n = n >> 7;
        assert((ptr - buf) < len);
    }
    *ptr = n;

    return buf;
}

unsigned long long varint_decode(uint8_t const *buf, size_t len, int32_t *err) {
    unsigned long long result = 0;
    int bits = 0;
    uint8_t const *ptr = buf;
    unsigned long long ll;
    while (*ptr & MSB) {
        ll = *ptr;
        result += ((ll & 0x7F) << bits);
        ptr++;
        bits += 7;

        if ((ptr - buf) >= (int32_t)len) {
            *err = -1;
            return 0;
        }
    }
    ll = *ptr;
    result += ((ll & 0x7F) << bits);

    *err = 0;
    return result;
}

} // namespace phylum
