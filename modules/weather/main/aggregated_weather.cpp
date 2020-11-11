#include "aggregated_weather.h"

#include "weather.h"
#include "clock.h"

namespace fk {

FK_DECLARE_LOGGER("weather");

struct AggregatedWeatherHelpers {
    fk_weather_aggregated_t &aw;

    AggregatedWeatherHelpers(fk_weather_aggregated_t &aw) : aw(aw) {
    }

    WindReading get_wind_gust() {
        if (aw.wind_gust.ticks == FK_WEATHER_TICKS_NULL) {
            return WindReading{ };
        }
        return WindReading{ aw.wind_gust };
    }

    WindReading get_wind_10m_max() {
        auto max = WindReading{ };
        for (auto &raw_wind : aw.wind_10m) {
            if (raw_wind.ticks != FK_WEATHER_TICKS_NULL) {
                auto wind = WindReading{ raw_wind };
                if (wind.stronger_than(max)) {
                    max = wind;
                }
            }
        }
        return max;
    }

    WindReading get_wind_two_minute_average() {
        WindReading readings[30];
        for (auto i = 0u; i < 30; ++i) {
            if (aw.wind_120s[i].ticks != FK_WEATHER_TICKS_NULL) {
                readings[i] = WindReading{ aw.wind_120s[i] };
            }
        }
        return WindReading::get_average(readings);
    }

    WindReading get_wind_30_second_average() {
        WindReading readings[120];
        for (auto i = 0u; i < 120; ++i) {
            if (aw.wind_120s[i].ticks != FK_WEATHER_TICKS_NULL) {
                readings[i] = WindReading{ aw.wind_120s[i] };
            }
        }
        return WindReading::get_average(readings);
    }

    float get_rain_inches_per_hour() {
        float value = 0.0f;

        for (auto &r : aw.rain_60m) {
            if (r.ticks != FK_WEATHER_TICKS_NULL) {
                value += r.ticks * RainPerTick;
            }
        }

        return value;
    }
};

ModuleReturn AggregatedWeather::initialize(ModuleContext mc, Pool &pool) {
    auto &bus = mc.module_bus();

    if (!I2C_CHECK(bus.write_register_u32(FK_WEATHER_I2C_ADDRESS, FK_WEATHER_I2C_COMMAND_CONFIG, get_clock_now()))) {
        return { ModuleStatus::Fatal };
    }

    return { ModuleStatus::Ok };
}

ModuleReturn AggregatedWeather::service(ModuleContext mc, Pool &pool) {
    return { ModuleStatus::Ok };
}

static ModuleReadings *new_module_readings(bool unmetered, Pool &pool) {
    if (unmetered) {
        return new (pool) NModuleReadings<4>();
    }
    return new (pool) NModuleReadings<16>();
}

ModuleReadings *AggregatedWeather::take_readings(ModuleContext mc, Pool &pool) {
    auto &bus = mc.module_bus();

    if (!I2C_CHECK(bus.write_register_u32(FK_WEATHER_I2C_ADDRESS, FK_WEATHER_I2C_COMMAND_CONFIG, get_clock_now()))) {
        logwarn("unable to configure");
    }

    fk_weather_aggregated_t aw;
    memzero(&aw, sizeof(aw));
    if (!I2C_CHECK(bus.read_register_buffer(FK_WEATHER_I2C_ADDRESS, FK_WEATHER_I2C_COMMAND_READ, (uint8_t *)&aw, sizeof(aw)))) {
        logwarn("error reading weather memory");
        return nullptr;
    }

    logdebug("time: %d/%02d/%02d (%" PRIu32 ")", aw.hour, aw.minute, aw.second, aw.uptime);

    AggregatedWeatherHelpers awh{ aw };

    auto unmetered = aw.wind_120s[0].ticks == FK_WEATHER_UNMETERED_MAGIC && aw.rain_60m[0].ticks == FK_WEATHER_UNMETERED_MAGIC;
    auto mr = new_module_readings(unmetered, pool);
    auto i = 0u;

    mr->set(i++, 100.0f * ((float)aw.humidity / (0xffff)));
    mr->set(i++, -45.0f + 175.0f * ((float)aw.temperature_1 / (0xffff)));
    mr->set(i++, aw.pressure / 64.0f / 1000.0f);
    mr->set(i++, aw.temperature_2 / 16.0f);

    // Detect the unmetered weather scenario.
    if (unmetered) {
        loginfo("unmetered weather detected");
        return mr;
    }

    mr->set(i++, awh.get_rain_inches_per_hour());

    auto wind_30s_average = awh.get_wind_30_second_average();
    mr->set(i++, wind_30s_average.speed);
    mr->set(i++, wind_30s_average.direction.angle);
    mr->set(i++, wind_30s_average.direction.raw);

    auto wind_gust = awh.get_wind_gust();
    mr->set(i++, wind_gust.speed);
    mr->set(i++, wind_gust.direction.angle);

    auto wind_10m_max = awh.get_wind_10m_max();
    mr->set(i++, wind_10m_max.speed);
    mr->set(i++, wind_10m_max.direction.angle);

    auto wind_2m_average = awh.get_wind_two_minute_average();
    mr->set(i++, wind_2m_average.speed);
    mr->set(i++, wind_2m_average.direction.angle);

    mr->set(i++, awh.get_rain_inches_per_hour());
    mr->set(i++, 0);

    return mr;
}

} // namespace fk
