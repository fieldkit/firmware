#pragma once

#include <alogging/alogging.h>

#include <algorithm>
#include <cassert>
#include <cinttypes>
#include <cstdarg>
#include <cstring>
#include <cstdio>

#if defined(PHYLUM_LOCAL_EXCHANGE)
#include <exchange.h>
#endif

namespace phylum {

static inline int32_t phy_vsnprintf(char *buffer, size_t size, const char *f, va_list args) {
    return vsnprintf(buffer, size, f, args);
}

uint32_t crc32_checksum(const char *str);

uint32_t crc32_checksum(const uint8_t *data, size_t size);

void phyinfof(const char *f, ...);

void phydebugf(const char *f, ...);

void phyerrorf(const char *f, ...);

void phywarnf(const char *f, ...);

void phyverbosef(const char *f, ...);

void phygraphf(const char *f, ...);

void phydebug_dump_memory(const char *prefix, uint8_t const *p, size_t size, ...);

} // namespace phylum

#include "entries.h"
