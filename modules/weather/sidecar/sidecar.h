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

int32_t fk_assert(const char *message, const char *file, int32_t line);

#define FK_ASSERT(expression)  (void)((expression) || (fk_assert("ASSERTION", __FILE__, __LINE__), 0))

uint32_t fk_weather_sign(fk_weather_t const *weather);

#include "logging.h"
