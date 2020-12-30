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

    uint32_t reserved[1];
    uint8_t failures;
    uint8_t reserved_failures;
    uint8_t reserved_u8[2];

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
    uint8_t counter_60m;

    uint32_t humidity;
    uint32_t temperature_1;

    uint32_t pressure;
    uint32_t temperature_2;

    fk_wind_t wind_120s[120];
    fk_wind_t wind_10m[10];
    fk_wind_t wind_gust;

    fk_rain_t rain_60m[60];

    uint32_t reserved_u32[6];
    uint8_t failures;
    uint8_t reserved_failures;
    uint8_t reserved_u8[2];

    uint32_t uptime;
    uint32_t crc;
} fk_weather_aggregated_t;

#define FK_WEATHER_ERROR_SENSORS_STARTUP               (0x01)
#define FK_WEATHER_ERROR_SENSORS_READING               (0x02)
#define FK_WEATHER_ERROR_SENSORS_MEMORY                (0x03)

#define FK_WEATHER_I2C_ADDRESS                         (0x42)
#define FK_WEATHER_I2C_COMMAND_READ                    (0x00)
#define FK_WEATHER_I2C_COMMAND_CONFIG                  (0x01)

#define FK_WEATHER_UNMETERED_MAGIC                     (0x0D15EA5E)
#define FK_WEATHER_TICKS_NULL                          (0x0D15EA5E)

#define FK_WEATHER_SENSORS_BME280                      (1 << 0)
#define FK_WEATHER_SENSORS_MPL3115A2                   (1 << 1)
#define FK_WEATHER_SENSORS_SHT31                       (1 << 2)
#define FK_WEATHER_SENSORS_ADC                         (1 << 3)
#define FK_WEATHER_SENSORS_COUNTERS                    (1 << 4)

static inline float fkw_weather_temperature_1(fk_weather_aggregated_t *w) {
    return ((float)w->temperature_1) / 100.0f;
}

static inline float fkw_weather_temperature_2(fk_weather_aggregated_t *w) {
    return ((float)w->temperature_1) / 100.0f;
}

static inline float fkw_weather_pressure(fk_weather_aggregated_t *w) {
    return ((float)w->pressure) / 256.0f / 1000.0f;
}

static inline float fkw_weather_humidity(fk_weather_aggregated_t *w) {
    return ((float)w->humidity) / 1024.0f;
}

/**
 * \brief Time struct for calendar
 */
struct fkw_calendar_time {
	/*range from 0 to 59*/
	uint8_t sec;
	/*range from 0 to 59*/
	uint8_t min;
	/*range from 0 to 23*/
	uint8_t hour;
};

/**
 * \brief Time struct for calendar
 */
struct fkw_calendar_date {
	/*range from 1 to 28/29/30/31*/
	uint8_t day;
	/*range from 1 to 12*/
	uint8_t month;
	/*absolute year>= 1970(such as 2000)*/
	uint16_t year;
};

/** \brief Date&Time struct for calendar
 */
struct fkw_calendar_date_time {
	struct fkw_calendar_time time;
	struct fkw_calendar_date date;
};

#ifdef __cplusplus
} // namespace fk
#endif
