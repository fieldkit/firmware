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
 * This accumulates one minute of weather information.
 */
typedef struct fk_weather_minute_t {
    uint32_t minute;

    uint32_t humidity;
    uint32_t temperature_1;

    uint32_t pressure;
    uint32_t temperature_2;

    fk_wind_t wind[60];
    fk_rain_t rain[60];

    uint32_t crc;
} fk_weather_minute_t;

typedef struct fk_eeprom_record_t {
    uint8_t type;

    union {
        fk_weather_minute_t weather;
    } r;
} fk_eeprom_record_t;

#define FK_WEATHER_ERROR_SENSORS_STARTUP               (0x1)
#define FK_WEATHER_ERROR_SENSORS_READING               (0x2)
#define FK_WEATHER_ERROR_SENSORS_MEMORY                (0x3)

#ifdef __cplusplus
}
#endif
