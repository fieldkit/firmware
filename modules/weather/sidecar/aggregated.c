#include "aggregated.h"

int32_t aggregated_weather_include(fk_weather_aggregated_t *aw, fk_weather_t *weather) {
    aw->humidity = weather->humidity;
    aw->temperature_1 = weather->temperature_1;

    aw->pressure = weather->pressure;
    aw->temperature_2 = weather->temperature_2;

    aw->second++;

    if (aw->second >= 60) {
        aw->second = 0;

        aw->minute++;

        aw->rain_60m[aw->minute].ticks = 0;

        if (++aw->counter_10m >= 10) {
            aw->counter_10m = 0;
        }

        aw->wind_10m[aw->counter_10m].ticks = 0;
        aw->wind_10m[aw->counter_10m].direction = 0;

        if (aw->minute >= 60) {
            aw->minute = 0;

            loginfo("new minute");
        }
    }

    if (++aw->counter_120s >= 120) {
        aw->counter_120s = 0;
    }

    if (weather->wind.ticks > aw->wind_10m[aw->counter_10m].ticks) {
        aw->wind_10m[aw->counter_10m] = weather->wind;
    }

    aw->wind_120s[aw->counter_120s] = weather->wind;
    aw->rain_60m[aw->minute].ticks += weather->rain.ticks;

    return FK_SUCCESS;
}
