#include <atmel_start.h>
#include <atmel_start_pins.h>
#include <hal_delay.h>

#include <SEGGER_RTT.h>

#include <modules.h>
#include <weather.h>

#include "sidecar.h"
#include "board.h"
#include "eeprom.h"
#include "sensors.h"
#include "crc.h"

fk_weather_config_t fk_weather_config_default = { 60, 60, 60, 0 };

int32_t read_configuration(fk_weather_config_t *config) {
    return FK_SUCCESS;
}

static uint8_t take_readings_triggered = 0;

static void timer_task_cb(struct timer_task const *const timer_task) {
    take_readings_triggered = 1;
}

uint32_t fk_weather_sign(fk_weather_t const *weather) {
    return crc32_checksum(FK_MODULES_CRC_SEED, (uint8_t const *)weather, sizeof(fk_weather_t) - sizeof(uint32_t));
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
    weather->crc = fk_weather_sign(weather);

    loginfof("crc: %" PRIu32 " 0x%" PRIx32, weather->seconds, weather->crc);
    loginfof("adc081c wind dir: %d", wind_direction.value);
    loginfof("wind: %d", counters_reading.wind);
    loginfof("rain: %d", counters_reading.rain);
    loginfof("mpl pressure: %d", mpl3115a2_reading.pressure);
    loginfof("mpl temp: %d", mpl3115a2_reading.temperature);
    loginfof("sht humidity: %d", sht31_reading.humidity);
    loginfof("sht temp: %d", sht31_reading.temperature);

    return FK_SUCCESS;
}

int32_t eeprom_region_seek_end(eeprom_region_t *region, uint32_t *seconds) {
    uint16_t address = region->start;
    while (address + sizeof(fk_weather_t) <= region->end) {
        fk_weather_t reading;

        int32_t rv = eeprom_read(region->i2c, address, (uint8_t *)&reading, sizeof(fk_weather_t));
        if (rv != FK_SUCCESS) {
            return rv;
        }

        uint32_t expected = crc32_checksum(FK_MODULES_CRC_SEED, (uint8_t *)&reading, sizeof(fk_weather_t) - sizeof(uint32_t));
        if (expected != reading.crc) {
            loginfof("found end 0x%04 (bad crc)" PRIx32, address);
            region->tail = address;
            break;
        }

        if (reading.seconds < *seconds) {
            loginfof("found end 0x%04 (wrap)" PRIx32, address);
            region->tail = address;
            break;
        }

        address += sizeof(fk_weather_t);
        *seconds = reading.seconds;
    }

    loginfof("found end 0x%04" PRIx32, address);

    return FK_SUCCESS;
}

__int32_t main() {
    SEGGER_RTT_SetFlagsUpBuffer(0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);

    board_initialize();

    // Floating allows writes, just leave the thing alone.
    // eeprom_write_enable_always();

    loginfo("waiting for eeprom...");

    // When we startup, sometimes the parent will hold this high so that it can
    // talk to the bus. Give the parent a chance to do that.
    while (eeprom_lock_test()) {
        delay_ms(100);
    }

    loginfo("initialize sensors...");

    sensors_initialize(&I2C_1);

    fk_weather_t weather;
    memset(&weather, 0, sizeof(fk_weather_t));

    eeprom_region_t readings_region;
    eeprom_region_create(&readings_region, &I2C_0, EEPROM_ADDRESS_READINGS, EEPROM_ADDRESS_READINGS_END, sizeof(fk_weather_t));
    if (eeprom_region_seek_end(&readings_region, &weather.seconds) != FK_SUCCESS) {
        logerror("error finding eeprom end");
    }

    struct timer_task timer_task;
    board_timer_setup(&timer_task, 1000, timer_task_cb);

    loginfo("ready!");

    while (true) {
        if (take_readings_triggered) {
            int32_t rv;

            take_readings_triggered = 0;

            rv = take_readings(&weather);
            if (rv != FK_SUCCESS) {
                logerror("readings: error taking");
            }

            rv = eeprom_region_append(&readings_region, &weather);
            if (rv != FK_SUCCESS) {
                if (rv == FK_ERROR_BUSY) {
                    loginfo("readings: eeprom busy");
                }
                else {
                    logerror("readings: error appending");
                }
            }
        }

        delay_ms(250);
    }

    return 0;
}
