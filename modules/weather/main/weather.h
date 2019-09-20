#pragma once

#include <inttypes.h>

#ifdef __cplusplus
namespace fk {
#endif

typedef struct fk_wind_t {
    uint32_t ticks;
    uint32_t direction;
} fk_wind_t;

typedef struct fk_rain_t {
    uint32_t ticks;
} fk_rain_t;

typedef struct fk_weather_t {
    uint32_t seconds;

    uint32_t humidity;
    uint32_t temperature_1;

    uint32_t pressure;
    uint32_t temperature_2;

    fk_wind_t wind;
    fk_rain_t rain;

    uint32_t reserved[5];
    uint32_t session;
    uint32_t error;

    uint32_t crc;
} fk_weather_t;

typedef struct fk_weather_config_t {
    uint16_t interval;
    uint16_t reserved[3];
    uint32_t crc;
} fk_weather_config_t;

#define FK_WEATHER_ERROR_SENSORS               (0xff)

#ifdef __cplusplus
}
#endif
