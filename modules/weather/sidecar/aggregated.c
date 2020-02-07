#include "aggregated.h"
#include "eeprom.h"
#include "board.h"
#include "crc.h"

uint32_t aggregated_weather_sign(fk_weather_aggregated_t const *aw) {
    return crc32_checksum(FK_MODULES_CRC_SEED, (uint8_t const *)aw, sizeof(fk_weather_aggregated_t) - sizeof(uint32_t));
}

int32_t aggregated_weather_include(fk_weather_aggregated_t *aw, struct calendar_date_time *clock, fk_weather_t *weather) {
    aw->second = clock->time.sec;

    aw->humidity = weather->humidity;
    aw->temperature_1 = weather->temperature_1;

    aw->pressure = weather->pressure;
    aw->temperature_2 = weather->temperature_2;

    // Check for a new minute.
    if (aw->minute != clock->time.min) {
        aw->minute = clock->time.min;
        aw->second = 0;

        // Start this minute's rain over.
        aw->rain_60m[aw->minute].ticks = 0;

        // Zero the new 10m wind value.
        if (++aw->counter_10m >= 10) {
            aw->counter_10m = 0;
        }
        aw->wind_10m[aw->counter_10m].ticks = 0;
        aw->wind_10m[aw->counter_10m].direction = 0;

        // Check for a new hour.
        if (aw->hour != clock->time.hour) {
            aw->hour = clock->time.hour;
            aw->minute = 0;

            // Roll up this hour of rain into the previous hour value.
            aw->rain_previous_hour.ticks = 0;
            for (int8_t i = 0u; i < 60u; ++i) {
                aw->rain_previous_hour.ticks += aw->rain_60m[i].ticks;
                aw->rain_60m[i].ticks = 0;
            }

            // Clear the gust.
            aw->wind_gust.ticks = 0;
            aw->wind_gust.direction = 0;
        }
    }

    // Accumulate rain for the minute.
    aw->rain_60m[aw->minute].ticks += weather->rain.ticks;

    // Keep 120 seconds of wind.
    if (++aw->counter_120s >= 120) {
        aw->counter_120s = 0;
    }
    aw->wind_120s[aw->counter_120s] = weather->wind;

    // Check for a new gust of wind for this minute.
    if (weather->wind.ticks > aw->wind_10m[aw->counter_10m].ticks) {
        aw->wind_10m[aw->counter_10m] = weather->wind;
    }

    // Look for a new gust.
    if (weather->wind.ticks > aw->wind_gust.ticks) {
        aw->wind_gust = weather->wind;
    }

    aw->wind.ticks += weather->wind.ticks;
    aw->wind.direction = weather->wind.direction;
    aw->rain.ticks += weather->rain.ticks;

    aw->uptime = board_system_time_get();
    aw->crc = aggregated_weather_sign(aw);

    return FK_SUCCESS;
}
