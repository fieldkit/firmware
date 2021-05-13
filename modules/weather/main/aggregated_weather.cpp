#include "aggregated_weather.h"

#include "weather.h"
#include "clock.h"

namespace fk {

FK_DECLARE_LOGGER("weather");

struct AggregatedWeatherHelpers {
    fk_weather_aggregated_t *aw{ nullptr };

    AggregatedWeatherHelpers(fk_weather_aggregated_t *aw) : aw(aw) {
    }

    WindReading get_wind_gust() {
        if (aw->wind_gust.ticks == FK_WEATHER_TICKS_NULL) {
            return WindReading{ };
        }
        return WindReading{ aw->wind_gust };
    }

    WindReading get_wind_10m_max() {
        auto max = WindReading{ };
        for (auto &raw_wind : aw->wind_10m) {
            if (raw_wind.ticks != FK_WEATHER_TICKS_NULL) {
                auto wind = WindReading{ raw_wind };
                if (wind.stronger_than(max)) {
                    max = wind;
                }
            }
        }
        return max;
    }

    WindReading get_wind_two_minute_average(Pool &pool) {
        WindReading *readings = pool.malloc<WindReading>(120);
        for (auto i = 0u; i < 120; ++i) {
            if (aw->wind_120s[i].ticks != FK_WEATHER_TICKS_NULL) {
                readings[i] = WindReading{ aw->wind_120s[i] };
            }
        }
        return WindReading::get_average(readings, 120);
    }

    WindReading get_wind_30_second_average(Pool &pool) {
        WindReading *readings = pool.malloc<WindReading>(30);
        auto index = aw->counter_120s;
        loginfo("getting 30s average %zd", sizeof(readings));
        for (auto i = 0u; i < 30; ++i) {
            if (aw->wind_120s[index].ticks != FK_WEATHER_TICKS_NULL) {
                loginfo("wind120s[%d/%d] %" PRIu32 " %" PRIu32, i, index, aw->wind_120s[index].ticks, aw->wind_120s[index].direction);
                readings[i] = WindReading{ aw->wind_120s[index] };
            }
            if (index == 0) {
                index = 120;
            }
            index--;
        }
        return WindReading::get_average(readings, 30);
    }

    float get_rain_mm_per_hour() {
        float value = 0.0f;

        for (auto &r : aw->rain_60m) {
            if (r.ticks != FK_WEATHER_TICKS_NULL) {
                value += r.ticks * RainPerTick;
            }
        }

        return value;
    }
};

ModuleReturn AggregatedWeather::initialize(ModuleContext mc, Pool &pool) {
    auto &bus = mc.module_bus();

    auto now = get_clock_now();

    for (auto i = 0u; i < 3u; ++i) {
        // TODO Check version
        if (false) {
            loginfo("sending clock (u32)");

            if (I2C_CHECK(bus.write_register_u32(FK_WEATHER_I2C_ADDRESS, FK_WEATHER_I2C_COMMAND_CONFIG, now))) {
                return { ModuleStatus::Ok };
            }

            logerror("error sending clock (ms::fatal)");
        }
        else {
            loginfo("sending clock (calendar)");

            DateTime date_time{ now };

            struct fkw_calendar_date_time dt;
            dt.date.year  = date_time.year();
            dt.date.month = date_time.month();
            dt.date.day   = date_time.day();
            dt.time.hour  = date_time.hour();
            dt.time.min   = date_time.minute();
            dt.time.sec   = date_time.second();

            if (I2C_CHECK(bus.write_register_buffer(FK_WEATHER_I2C_ADDRESS, FK_WEATHER_I2C_COMMAND_CONFIG, &dt, sizeof(dt)))) {
                return { ModuleStatus::Ok };
            }

            logerror("error sending clock (ms::fatal)");
        }

        fk_delay(10);
    }

    return { ModuleStatus::Fatal };
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

    fk_weather_aggregated_t *aw = pool.malloc<fk_weather_aggregated_t>();
    memzero(aw, sizeof(fk_weather_aggregated_t));
    if (!I2C_CHECK(bus.read_register_buffer(FK_WEATHER_I2C_ADDRESS, FK_WEATHER_I2C_COMMAND_READ, (uint8_t *)aw, sizeof(aw)))) {
        logwarn("error reading weather memory");
        return nullptr;
    }

    logdebug("time: %d/%02d/%02d (%" PRIu32 ")", aw->hour, aw->minute, aw->second, aw->uptime);

    AggregatedWeatherHelpers awh{ aw };

    auto unmetered = aw->wind_120s[0].ticks == FK_WEATHER_UNMETERED_MAGIC && aw->rain_60m[0].ticks == FK_WEATHER_UNMETERED_MAGIC;
    auto mr = new_module_readings(unmetered, pool);
    auto i = 0u;

    auto having_supply_chain_nightmare = ((aw->initialized & FK_WEATHER_SENSORS_SHT31) > 0) &&
                                         ((aw->initialized & FK_WEATHER_SENSORS_MPL3115A2) > 0);
    if (having_supply_chain_nightmare) {
        loginfo("variant: sht31/mpl3115a2 initialized=0x%x failures=0x%x", aw->initialized, aw->failures);

        mr->set(i++, 100.0f * ((float)aw->humidity / (0xffff)));
        mr->set(i++, -45.0f + 175.0f * ((float)aw->temperature_1 / (0xffff)));
        mr->set(i++, aw->pressure / 64.0f / 1000.0f);
        mr->set(i++, aw->temperature_2 / 16.0f);

        if (aw->failures == (FK_WEATHER_SENSORS_ADC | FK_WEATHER_SENSORS_COUNTERS | FK_WEATHER_SENSORS_BME280)) {
            logerror("too many failures, hup module");
            return nullptr;
        }
    }
    else {
        loginfo("variant: bme280 initialized=0x%x failures=0x%x", aw->initialized, aw->failures);

        mr->set(i++, fkw_weather_humidity(aw));
        mr->set(i++, fkw_weather_temperature_1(aw));
        mr->set(i++, fkw_weather_pressure(aw));
        mr->set(i++, fkw_weather_temperature_2(aw));

        if (aw->failures == (FK_WEATHER_SENSORS_ADC | FK_WEATHER_SENSORS_COUNTERS | FK_WEATHER_SENSORS_SHT31 | FK_WEATHER_SENSORS_MPL3115A2)) {
            logerror("too many failures, hup module");
            return nullptr;
        }
    }

    // Detect the unmetered weather scenario.
    if (unmetered) {
        loginfo("variant: unmetered");
        return mr;
    }

    mr->set(i++, awh.get_rain_mm_per_hour());

    auto wind_30s_average = awh.get_wind_30_second_average(pool);
    mr->set(i++, wind_30s_average.speed);
    mr->set(i++, wind_30s_average.direction.angle);
    mr->set(i++, wind_30s_average.direction.raw);

    auto wind_gust = awh.get_wind_gust();
    mr->set(i++, wind_gust.speed);
    mr->set(i++, wind_gust.direction.angle);

    auto wind_10m_max = awh.get_wind_10m_max();
    mr->set(i++, wind_10m_max.speed);
    mr->set(i++, wind_10m_max.direction.angle);

    auto wind_2m_average = awh.get_wind_two_minute_average(pool);
    mr->set(i++, wind_2m_average.speed);
    mr->set(i++, wind_2m_average.direction.angle);

    mr->set(i++, awh.get_rain_mm_per_hour());
    mr->set(i++, 0);

    return mr;
}

} // namespace fk
