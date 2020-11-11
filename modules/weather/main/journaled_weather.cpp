#include "journaled_weather.h"
#include "modules/eeprom.h"

namespace fk {

FK_DECLARE_LOGGER("weather");

ModuleReturn JournaledWeather::initialize(ModuleContext mc, Pool &pool) {
    auto module_bus = get_board()->i2c_module();
    auto eeprom = ModuleEeprom{ module_bus };

    fk_weather_t reading;
    bzero(&reading, sizeof(reading));
    bzero(&weather_, sizeof(weather_));
    address_ = 0;
    seconds_ = 0;
    checks_ = 0;

    // Scan all of the EEPROM to find the MAX(seconds) reading. This will be the
    // end of them and we'll be resuming from there.

    uint32_t iter = EEPROM_ADDRESS_READINGS;
    while (true) {
        if (iter + sizeof(fk_weather_t) >= EEPROM_ADDRESS_READINGS_END) {
            break;
        }

        fk_weather_t temp;
        if (!eeprom.read_data(iter, &temp, sizeof(fk_weather_t))) {
            logerror("error reading eeprom");
            return { ModuleStatus::Fatal };
        }

        auto expected = calculate_crc(FK_MODULES_CRC_SEED, temp);
        if (expected == temp.crc) {
            logdebug("[0x%04" PRIx32 "] reading (%" PRIu32 ") (%.2f)", iter, temp.seconds, temp.temperature_2 / 16.0f);
            if (address_ == 0 || temp.seconds > reading.seconds) {
                reading = temp;
                seconds_ = temp.seconds;
                session_ = temp.session;
                address_ = iter;
            }
        }
        else {
            loginfo("[0x%04" PRIx32 "] bad crc (0x%" PRIx32 " != 0x%" PRIx32 ")", iter, expected, temp.crc);
            break;
        }

        iter += sizeof(fk_weather_t);
    }

    if (address_ > 0) {
        loginfo("[0x%04" PRIx32 "] end (%" PRIu32 ")", address_, seconds_);
    }

    weather_ = { };

    return { ModuleStatus::Ok };
}

ModuleReturn JournaledWeather::service(ModuleContext mc, Pool &pool) {
    if (fk_uptime() - serviced_ < 1000) {
        loginfo("servicing skipped");
        return { ModuleStatus::Ok };
    }

    loginfo("servicing");

    auto module_bus = get_board()->i2c_module();
    auto eeprom = ModuleEeprom{ module_bus };

    // Keep trying to find the end if we weren't able to.
    if (address_ == 0) {
        if (initialize(mc, pool).status != ModuleStatus::Ok) {
            logerror("initialize failed");
            return { ModuleStatus::Fatal };
        }

        if (address_ == 0) {
            logerror("no readings");
            return { ModuleStatus::Fatal };
        }
    }

    bzero(&weather_.reading, sizeof(fk_weather_t));

    uint32_t now = mc.now();
    uint32_t old_session = session_;
    uint32_t errors = 0;
    uint32_t original = address_;

    while (true) {
        if (address_ + sizeof(fk_weather_t) >= EEPROM_ADDRESS_READINGS_END) {
            address_ = EEPROM_ADDRESS_READINGS;

            if (address_ == original) {
                logerror("error finding readings, wrap around");
                break;
            }
        }

        fk_weather_t temp;
        if (!eeprom.read_data(address_, &temp, sizeof(fk_weather_t))) {
            logerror("error reading eeprom");
            return { ModuleStatus::Fatal };
        }

        auto expected = calculate_crc(FK_MODULES_CRC_SEED, temp);
        if (expected == temp.crc) {
            float temperature = temp.temperature_2 / 16.0f;

            if (temp.error != 0) {
                logerror("[0x%04" PRIx32 "] error detected err(%" PRIu32 ") memf(%" PRIu32 ") readf(%" PRIu32 ")", address_, temp.error, temp.memory_failures, temp.reading_failures);
                errors++;
            }

            if (temp.seconds < seconds_) {
                break;
            }

            if (temp.session < session_) {
                logwarn("[0x%04" PRIx32 "] module restarted (%" PRIu32 " < %" PRIu32 ") memf(%" PRIu32 ") readf(%" PRIu32 ")", address_, temp.session, session_, temp.memory_failures, temp.reading_failures);
                // Clear seconds so we can be sure to get a known delta.
                seconds_ = 0;
            }

            logdebug("[0x%04" PRIx32 "] reading (%" PRIu32 ") memf(%" PRIu32 ") readf(%" PRIu32 ") (%.2f)", address_, seconds_, temp.memory_failures, temp.reading_failures, temperature);

            include(now, temp);

            seconds_ = temp.seconds;
            session_ = temp.session;

            weather_.reading = temp;
            weather_.rain_ticks += temp.rain.ticks;
            weather_.wind_ticks  = temp.wind.ticks;
        }
        else {
            loginfo("[0x%04" PRIx32 "] bad crc (0x%" PRIx32 " != 0x%" PRIx32 ")", address_, expected, temp.crc);
            break;
        }

        address_ += sizeof(fk_weather_t);

        if (address_ == original) {
            logerror("error finding readings, wrap around");
            break;
        }
    }

    // Detect stalled conditions, until we can figure out why this happened.
    if (old_session == session_ || weather_.reading.seconds == 0) {
        if (checks_ > 0 && errors > 0) {
            logwarn("no readings, hupping module...");
            if (!mc.power_cycle()) {
                logerror("error power cycling");
            }
        }
        else {
            logwarn("no readings...");
        }
    }

    serviced_ = fk_uptime();

    checks_++;

    return { ModuleStatus::Ok };
}

ModuleReadings *JournaledWeather::take_readings(ModuleContext mc, Pool &pool) {
    auto &bus = mc.module_bus();

    fk_weather_aggregated_t aw;
    bzero(&aw, sizeof(aw));
    if (!I2C_CHECK(bus.read_register_buffer(FK_WEATHER_I2C_ADDRESS, FK_WEATHER_I2C_COMMAND_READ, (uint8_t *)&aw, sizeof(aw)))) {
        logwarn("error reading");
    }

    if (service(mc, pool).status != ModuleStatus::Ok) {
        return false;
    }

    auto &reading = weather_.reading;

    auto wind_adc_raw = (__builtin_bswap16(reading.wind.direction) >> 4) & 0xff;
    auto wind_adc_mv = wind_adc_raw * (3.3f / 256.0f) * 1000.0f;
    auto wind_angle = get_wind_direction(wind_adc_raw);

    auto i = 0;
    auto mr = new(pool) NModuleReadings<16>();
    mr->set(i++, 100.0f * ((float)reading.humidity / (0xffff)));
    mr->set(i++, -45.0f + 175.0f * ((float)reading.temperature_1 / (0xffff)));
    mr->set(i++, reading.pressure / 64.0f / 1000.0f);
    mr->set(i++, reading.temperature_2 / 16.0f);

    mr->set(i++, weather_.rain_ticks * RainPerTick);

    mr->set(i++, weather_.wind_ticks * WindPerTick);
    mr->set(i++, wind_angle);
    mr->set(i++, wind_adc_mv);

    mr->set(i++, weather_.hourly_wind_gust.speed);
    mr->set(i++, weather_.hourly_wind_gust.direction.angle);

    auto wind_10m_max = weather_.get_10m_wind_max();
    mr->set(i++, wind_10m_max.speed);
    mr->set(i++, wind_10m_max.direction.angle);

    auto wind_2m_average = weather_.get_two_minute_wind_average();
    mr->set(i++, wind_2m_average.speed);
    mr->set(i++, wind_2m_average.direction.angle);

    mr->set(i++, weather_.get_hourly_rain());
    mr->set(i++, weather_.previous_hourly_rain);

    weather_.rain_ticks = 0;
    weather_.wind_ticks = 0;

    return mr;
}

WindDirection JournaledWeather::get_wind_direction(fk_weather_t const &raw) {
    auto adc_raw = (__builtin_bswap16(raw.wind.direction) >> 4) & 0xff;
    auto wind_angle = get_wind_direction(adc_raw);
    return { (int16_t)adc_raw, wind_angle };
}

WindReading JournaledWeather::get_wind_reading(fk_weather_t const &raw, int32_t seconds_elapsed) {
    auto speed = raw.wind.ticks / ((float)seconds_elapsed) * WindPerTick;
    return WindReading{ speed, get_wind_direction(raw) };
}

void JournaledWeather::include(uint32_t now, fk_weather_t const &raw) {
    // Check to see if this is the reading we're expecting.
    auto seconds_elapsed = raw.seconds - seconds_;

    // We nearly always hope that this is 1. I'm hoping to dial these conditions
    // in over time.
    if (seconds_elapsed != 1) {
        logwarn("unexpected elapsed time (%" PRIu32 " - %" PRIu32 ") = %" PRIu32, raw.seconds, seconds_, seconds_elapsed);
        weather_ = { };
        return;
    }

    auto previous_full_time = DateTime{ weather_.time };
    auto new_time = weather_.time > 0 ? weather_.time + seconds_elapsed : now;
    auto new_full_time = DateTime{ new_time };

    auto wind = get_wind_reading(raw, seconds_elapsed);

    weather_.hour_of_rain[new_full_time.minute()] += raw.rain.ticks * RainPerTick;

    weather_.two_minutes_of_wind[weather_.two_minute_seconds_counter] = wind;
    weather_.two_minute_seconds_counter = (weather_.two_minute_seconds_counter + seconds_elapsed) % 120;

    if (wind.stronger_than(weather_.wind_gusts[weather_.ten_minute_minute_counter])) {
        weather_.wind_gusts[weather_.ten_minute_minute_counter] = wind;
        logdebug("[%0d] new 10m max wind: %f", weather_.ten_minute_minute_counter, wind.speed);
    }

    if (wind.stronger_than(weather_.hourly_wind_gust)) {
        weather_.hourly_wind_gust = wind;
    }

    weather_.time = new_time;

    if (new_full_time.minute() != previous_full_time.minute()) {
        // TODO: Verify it's the expected minute.
        loginfo("new minute: %d (%d)", new_full_time.minute(), weather_.ten_minute_minute_counter);
        weather_.ten_minute_minute_counter = (weather_.ten_minute_minute_counter + 1) % 10;
        weather_.wind_gusts[weather_.ten_minute_minute_counter] = { };
        weather_.hour_of_rain[new_full_time.minute()] = 0;

        if (new_full_time.hour() != previous_full_time.hour()) {
            loginfo("new hour: %d", new_full_time.hour());

            weather_.hourly_wind_gust = { };
            weather_.previous_hourly_rain = weather_.get_hourly_rain();
            for (auto i = 0u; i < 60u; ++i) {
                weather_.hour_of_rain[i] = 0;
            }


            if (new_full_time.hour() < previous_full_time.hour()) {
                loginfo("new day");
            }
        }
    }
}

WindReading WeatherState::get_two_minute_wind_average() const {
    return WindReading::get_average(two_minutes_of_wind);
}

WindReading WeatherState::get_10m_wind_max() const {
    auto max = WindReading{ };
    for (auto i = 0; i < 10; ++i) {
        if (wind_gusts[i].stronger_than(max)) {
            max = wind_gusts[i];
        }
    }
    return max;
}

float WeatherState::get_hourly_rain() const {
    auto total = 0.0f;
    for (auto i = 0; i < 60; ++i) {
        total += hour_of_rain[i];
    }
    return total;
}

}
