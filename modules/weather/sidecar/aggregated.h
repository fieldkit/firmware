#pragma once

#include "sidecar.h"

int32_t aggregated_weather_initialize(fk_weather_aggregated_t *aw);

uint32_t aggregated_weather_sign(fk_weather_aggregated_t const *aw);

int32_t aggregated_weather_include(fk_weather_aggregated_t *aw, struct calendar_date_time *clock, fk_weather_t *weather);
