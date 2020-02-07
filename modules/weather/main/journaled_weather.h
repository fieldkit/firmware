#pragma once

#include <modules/bridge/modules.h>

#include "weather.h"
#include "weather_types.h"

namespace fk {

struct WeatherState {
    uint32_t time{ 0 };
    uint8_t two_minute_seconds_counter{ 0 };
    uint8_t ten_minute_minute_counter{ 0 };
    uint32_t rain_ticks{ 0 };
    uint32_t wind_ticks{ 0 };
    fk_weather_t reading;

    /**
     * Last 120 wind readings. Used to calculate an average.
     */
    WindReading two_minutes_of_wind[120];
    /**
     * For the last 10 minutes the largest gust that occured during that minute.
     */
    WindReading wind_gusts[10];
    /*
     * The maximum gust of wind for the hour.
     */
    WindReading hourly_wind_gust;

    /**
     * Amount of rain rolled over.
     */
    float previous_hourly_rain{ 0 };

    /**
     * Stores the rainfall per minute over the last 60 minutes.
     */
    float hour_of_rain[60] = { 0 };

    /**
     * Return a DateTime for the UNIX time stamp in our state.
     */
    DateTime date_time() const {
        return DateTime{ time };
    }

    /**
     * Clear the weather related state. We have to do this because we need to
     * leave the MinimumFlashState alone.
     */
    void clear() {
        two_minute_seconds_counter = 0;
        ten_minute_minute_counter = 0;
        for (auto i = 0; i < 120; ++i) {
            two_minutes_of_wind[i] = { };
        }
        for (auto i = 0; i < 10; ++i) {
            wind_gusts[i] = {  };
        }
        hourly_wind_gust = { };
        previous_hourly_rain = 0;
        for (auto i = 0; i < 60; ++i) {
            hour_of_rain[i] = 0;
        }
    }

    WindReading get_two_minute_wind_average() const;
    WindReading get_10m_wind_max() const;
    float get_hourly_rain() const;
};

class JournaledWeather {
private:
    uint32_t seconds_{ 0 };
    uint32_t session_{ 0 };
    uint32_t address_{ 0 };
    uint32_t checks_{ 0 };
    uint32_t serviced_{ 0 };
    WeatherState weather_;

public:
    ModuleReturn initialize(ModuleContext mc, Pool &pool);
    ModuleReturn service(ModuleContext mc, Pool &pool);
    ModuleReadings *take_readings(ModuleContext mc, Pool &pool);

public:
    void include(uint32_t now, fk_weather_t const &raw);

private:
    static int16_t get_wind_direction(uint8_t adc);
    static WindDirection get_wind_direction(fk_weather_t const &raw);
    static WindReading get_wind_reading(fk_weather_t const &raw, int32_t seconds_elapsed);

};

} // namespace fk
