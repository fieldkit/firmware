#include <atmel_start.h>
#include <atmel_start_pins.h>
#include <hal_delay.h>

#include <SEGGER_RTT.h>

#include <modules/modules.h>
#include <weather.h>

#include "sidecar.h"
#include "board.h"
#include "eeprom.h"
#include "sensors.h"

fk_weather_config_t fk_weather_config_default = { 60, 60, 60, 0 };

int32_t read_configuration(fk_weather_config_t *config) {
    return FK_SUCCESS;
}

static uint8_t take_readings_triggered = 0;

static void timer_task_cb(struct timer_task const *const timer_task) {
    take_readings_triggered = 1;
}

int32_t take_readings(fk_weather_t *weather) {
    adc081c_reading_t wind_direction;
    if (adc081c_reading_get(&I2C_1, &wind_direction) != FK_SUCCESS) {
        logerror("reading adc081c");
    }

    mpl3115a2_reading_t mpl3115a2_reading;
    if (mpl3115a2_reading_get(&I2C_1, &mpl3115a2_reading) != FK_SUCCESS) {
        logerror("reading mpl3115a2");
    }

    sht31_reading_t sht31_reading;
    if (sht31_reading_get(&I2C_1, &sht31_reading) != FK_SUCCESS) {
        logerror("reading sht31");
    }

    counters_reading_t counters_reading;
    if (counters_reading_get(&I2C_1, &counters_reading) != FK_SUCCESS) {
        logerror("reading counters");
    }

    SEGGER_RTT_WriteString(0, "\n");

    weather->seconds++;
    weather->humidity = sht31_reading.humidity;
    weather->temperature_1 = sht31_reading.temperature;
    weather->pressure = mpl3115a2_reading.pressure;
    weather->temperature_2 = mpl3115a2_reading.temperature;
    weather->wind.direction = wind_direction.value;

    loginfof("adc081c: %d", wind_direction.value);
    loginfof("wind: %d", counters_reading.wind);
    loginfof("rain: %d", counters_reading.rain);
    loginfof("pressure: %d", mpl3115a2_reading.pressure);
    loginfof("temp: %d", mpl3115a2_reading.temperature);
    loginfof("humidity: %d", sht31_reading.humidity);
    loginfof("temp: %d", sht31_reading.temperature);

    return FK_SUCCESS;
}

__int32_t main() {
    board_initialize();

    loginfo("checking eeprom...");

    // Always leave EEPROM ready for writes.
    eeprom_write_enable_always();

    loginfo("configuration...");

    // Read configuration, if that fails use the default.
    fk_weather_config_t config;
    if (read_configuration(&config) != FK_SUCCESS) {
        config = fk_weather_config_default;
    }

    loginfo("sensors...");

    if (sensors_initialize(&I2C_1) != FK_SUCCESS) {
        logerror("sensors: error initializing");
    }

    struct timer_task timer_task;

    board_timer_setup(&timer_task, 1000, timer_task_cb);

    loginfo("ready!");

    fk_weather_t weather;

    eeprom_region_t readings_region;

    eeprom_region_create(&readings_region, &I2C_1, EEPROM_ADDRESS_READINGS, EEPROM_ADDRESS_END, sizeof(fk_weather_t));

    while (true) {
        if (take_readings_triggered) {
            take_readings_triggered = 0;

            int32_t rv;

            rv = take_readings(&weather);
            if (rv != FK_SUCCESS) {
                logerror("readings: error taking");
            }

            rv = eeprom_region_append(&readings_region, &weather);
            if (rv != FK_SUCCESS) {
                logerror("readings: error appending");
            }
        }
        delay_ms(1000);
    }

    return 0;
}
