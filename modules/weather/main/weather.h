#pragma once

#include <inttypes.h>

#ifdef __cplusplus
namespace fk {
#endif

typedef struct fk_wind_t {
    uint32_t speed;
    uint32_t direction;
    uint32_t speed_60m;
    uint32_t direction_60m;
    uint32_t speed_10m;
    uint32_t direction_10m;
    uint32_t speed_2m;
    uint32_t direction_2m;
} fk_wind_t;

typedef struct fk_rain_t {
    uint32_t this_hour;
    uint32_t previous_hour;
} fk_rain_t;

typedef struct fk_weather_t {
    uint32_t seconds;

    uint32_t humidity;
    uint32_t temperature_1;

    uint32_t pressure;
    uint32_t altitude;
    uint32_t temperature_2;

    fk_wind_t wind;
    fk_rain_t rain;

    uint32_t crc;
} fk_weather_t;

typedef struct fk_weather_config_t {
    uint16_t interval;
    uint16_t rain_interval;
    uint16_t wind_interval;
    uint32_t crc;
} fk_weather_config_t;

fk_weather_config_t fk_weather_config_default;

#ifdef __cplusplus
}
#endif
