#pragma once

#include <atmel_start.h>
#include <inttypes.h>
#include <string.h>
#include <weather.h>

#define FK_SUCCESS           (0)

#define FK_ERROR_GENERAL    (-1)
#define FK_ERROR_BUSY       (-2)
#define FK_ERROR_TIMEOUT    (-3)
#define FK_ERROR_IO         (-4)
#define FK_ERROR_EMPTY      (-5)

// #define FK_LOGGING
// #define FK_WEATHER_IGNORE_NO_SENSORS

int32_t fk_assert(const char *message, const char *file, int32_t line);

#define FK_ASSERT(expression)  (void)((expression) || (fk_assert("ASSERTION", __FILE__, __LINE__), 0))

uint32_t fk_weather_sign(fk_weather_t const *weather);

#if !defined(memzero)
/**
 * Zero a region of memory.
 */
#define memzero(ptr, size)  memset(ptr, 0, size)
#endif

#include "logging.h"
