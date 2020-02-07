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

/**
 * This accumulates one second of weather information.
 */
typedef struct fk_weather_t {
    uint32_t seconds;

    uint32_t humidity;
    uint32_t temperature_1;

    uint32_t pressure;
    uint32_t temperature_2;

    fk_wind_t wind;
    fk_rain_t rain;

    uint32_t reserved[2];
    uint32_t startups;
    uint32_t reading_failures;
    uint32_t memory_failures;
    uint32_t session;
    uint32_t error;

    uint32_t crc;
} fk_weather_t;

typedef struct fk_weather_config_t {
    uint16_t interval;
    uint16_t reserved[3];
    uint32_t crc;
} fk_weather_config_t;

/**
 * This accumulates aggregated weather information.
 */
typedef struct fk_weather_aggregated_t {
    uint16_t second;
    uint16_t minute;
    uint16_t hour;
    uint8_t counter_120s;
    uint8_t counter_10m;

    uint32_t humidity;
    uint32_t temperature_1;

    uint32_t pressure;
    uint32_t temperature_2;

    fk_wind_t wind_120s[120];
    fk_wind_t wind_10m[10];
    fk_wind_t wind_gust;

    fk_rain_t rain_60m[60];

    fk_rain_t rain_previous_hour;

    fk_rain_t previous_rain;
    fk_wind_t previous_wind;
    fk_rain_t rain;
    fk_wind_t wind;

    uint32_t uptime;
    uint32_t crc;
} fk_weather_aggregated_t;

#define FK_WEATHER_ERROR_SENSORS_STARTUP               (0x01)
#define FK_WEATHER_ERROR_SENSORS_READING               (0x02)
#define FK_WEATHER_ERROR_SENSORS_MEMORY                (0x03)

#define FK_WEATHER_I2C_ADDRESS                         (0x42)
#define FK_WEATHER_I2C_COMMAND_READ                    (0x00)
#define FK_WEATHER_I2C_COMMAND_CONFIG                  (0x01)

#ifdef __cplusplus
}
#endif
