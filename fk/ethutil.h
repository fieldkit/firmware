#pragma once

#include <cstdint>

uint16_t ethutil_swaps(uint16_t i);
uint32_t ethutil_swapl(uint32_t l);
uint16_t ethutil_htons(unsigned short hostshort);
uint32_t ethutil_htonl(unsigned long hostlong);
uint32_t ethutil_ntohl(unsigned long netlong);
uint16_t ethutil_ntohs(unsigned short netshort);
