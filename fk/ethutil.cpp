#include "ethutil.h"

uint16_t ethutil_htons(unsigned short hostshort) {
#if (SYSTEM_ENDIAN == _ENDIAN_LITTLE_)
    return ethutil_swaps(hostshort);
#else
    return hostshort;
#endif
}

uint32_t ethutil_htonl(unsigned long hostlong) {
#if (SYSTEM_ENDIAN == _ENDIAN_LITTLE_)
    return ethutil_swapl(hostlong);
#else
    return hostlong;
#endif
}

uint16_t ethutil_ntohs(unsigned short netshort) {
#if (SYSTEM_ENDIAN == _ENDIAN_LITTLE_)
    return ethutil_swaps(netshort);
#else
    return netshort;
#endif
}

uint32_t ethutil_ntohl(unsigned long netlong) {
#if (SYSTEM_ENDIAN == _ENDIAN_LITTLE_)
    return ethutil_swapl(netlong);
#else
    return netlong;
#endif
}

uint16_t ethutil_swaps(uint16_t i) {
    uint16_t ret = 0;
    ret = (i & 0xFF) << 8;
    ret |= ((i >> 8) & 0xFF);
    return ret;
}

uint32_t ethutil_swapl(uint32_t l) {
    uint32_t ret = 0;
    ret = (l & 0xFF) << 24;
    ret |= ((l >> 8) & 0xFF) << 16;
    ret |= ((l >> 16) & 0xFF) << 8;
    ret |= ((l >> 24) & 0xFF);
    return ret;
}
