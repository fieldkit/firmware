#include <atmel_start.h>
#include <atmel_start_pins.h>
#include <hal_delay.h>
#include <hpl_reset.h>

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

int32_t take_readings(fk_weather_t *weather) {
    adc081c_reading_t wind_direction;
    if (adc081c_reading_get(&I2C_1, &wind_direction) != FK_SUCCESS) {
        logerror("reading adc081c");
        return FK_ERROR_GENERAL;
    }

    mpl3115a2_reading_t mpl3115a2_reading;
    if (mpl3115a2_reading_get(&I2C_1, &mpl3115a2_reading) != FK_SUCCESS) {
        logerror("reading mpl3115a2");
        return FK_ERROR_GENERAL;
    }

    sht31_reading_t sht31_reading;
    if (sht31_reading_get(&I2C_1, &sht31_reading) != FK_SUCCESS) {
        logerror("reading sht31");
        return FK_ERROR_GENERAL;
    }

    counters_reading_t counters_reading;
    if (counters_reading_get(&I2C_1, &counters_reading) != FK_SUCCESS) {
        logerror("reading counters");
        return FK_ERROR_GENERAL;
    }

    weather->seconds++;
    weather->session++;
    weather->humidity = sht31_reading.humidity;
    weather->temperature_1 = sht31_reading.temperature;
    weather->pressure = mpl3115a2_reading.pressure;
    weather->temperature_2 = mpl3115a2_reading.temperature;
    weather->wind.direction = wind_direction.value;
    weather->wind.ticks = counters_reading.wind;
    weather->rain.ticks = counters_reading.rain;
    weather->crc = fk_weather_sign(weather);

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
            loginfof("found end 0x%04" PRIx32 " (0x%" PRIx32 " != 0x%" PRIx32 ")", address, expected, reading.crc);
            region->tail = address;
            break;
        }

        if (reading.seconds < *seconds) {
            loginfof("found end 0x%04" PRIx32 " (wrap)", address);
            region->tail = address;
            break;
        }

        address += sizeof(fk_weather_t);
        *seconds = reading.seconds;
    }

    loginfof("found end 0x%04" PRIx32, address);

    return FK_SUCCESS;
}

int32_t eeprom_region_append_error(eeprom_region_t *region, uint32_t error, uint32_t failures) {
    fk_weather_t weather;
    memset(&weather, 0, sizeof(fk_weather_t));

    weather.error = error;
    weather.failures = failures;
    weather.crc = fk_weather_sign(&weather);

    int32_t rv = eeprom_region_append(region, &weather);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    return FK_SUCCESS;
}

__int32_t main() {
    SEGGER_RTT_SetFlagsUpBuffer(0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);

    board_initialize();

    loginfof("board ready!");

    loginfof("sizeof(fk_weather_t) = %zd (%zd readings)", sizeof(fk_weather_t), EEPROM_AVAILABLE_DATA / sizeof(fk_weather_t));

    loginfo("waiting for eeprom...");

    // When we startup, sometimes the parent will hold this high so that it can
    // talk to the bus. Give the parent a chance to do that.
    while (eeprom_lock_test()) {
        delay_ms(100);
    }

    loginfo("watchdog...");

    uint32_t clk_rate = 1000;
    uint16_t to_period = 4096;
    wdt_set_timeout_period(&WDT_0, clk_rate, to_period);
    wdt_enable(&WDT_0);

    fk_weather_t weather;
    memset(&weather, 0, sizeof(fk_weather_t));

    eeprom_region_t readings_region;
    FK_ASSERT(eeprom_region_create(&readings_region, &I2C_0, EEPROM_ADDRESS_READINGS, EEPROM_ADDRESS_READINGS_END, sizeof(fk_weather_t)) == FK_SUCCESS);

    loginfo("eeprom...");

    board_eeprom_i2c_enable();

    if (eeprom_verify_header(&readings_region) != FK_SUCCESS) {
        logerror("error verifying header");
        delay_ms(1000);
        NVIC_SystemReset();
    }

    if (eeprom_region_seek_end(&readings_region, &weather.seconds) != FK_SUCCESS) {
        logerror("error finding eeprom end");
        delay_ms(1000);
        NVIC_SystemReset();
    }

    board_eeprom_i2c_disable();

    loginfo("sensors...");

    int32_t rv = sensors_initialize(&I2C_1);
    if (rv != FK_SUCCESS) {
        logerror("error initializing sensors");
        rv = eeprom_region_append_error(&readings_region, FK_WEATHER_ERROR_SENSORS, 0);
        if (rv != FK_SUCCESS) {
            logerror("error writing error");
        }
        delay_ms(1000);
        NVIC_SystemReset();
    }

    struct timer_task timer_task;
    FK_ASSERT(board_timer_setup(&timer_task, 1000, timer_task_cb) == FK_SUCCESS);

    unwritten_readings_t ur;
    FK_ASSERT(unwritten_readings_initialize(&ur) == FK_SUCCESS);

    loginfo("ready!");

    while (true) {
        if (take_readings_triggered) {
            take_readings_triggered = 0;

            int32_t rv = take_readings(&weather);
            if (rv != FK_SUCCESS) {
                unwritten_readings_push_error(&ur, FK_WEATHER_ERROR_SENSORS, weather.failures);
            }
            else {
                unwritten_readings_push(&ur, &weather);
            }

            rv = eeprom_region_append_unwritten(&readings_region, &ur);
            if (rv != FK_SUCCESS) {
                if (rv == FK_ERROR_BUSY) {
                    loginfo("readings: eeprom busy");
                }
                else {
                    logerror("readings: error appending");
                    weather.failures++;
                }
            }

            #if defined(FK_LOGGING)
            SEGGER_RTT_WriteString(0, "\n");

            loginfof("crc: %" PRIu32 " 0x%" PRIx32 " %" PRIu32 " 0x%04" PRIx32, weather.seconds, weather.crc, free_memory(), readings_region.tail);
            loginfof("adc081c wind dir: %d", weather.wind.direction);
            loginfof("wind: %d", weather.wind.ticks);
            loginfof("rain: %d", weather.rain.ticks);
            loginfof("mpl pressure: %d", weather.pressure);
            loginfof("mpl temp: %d", weather.temperature_2);
            loginfof("sht humidity: %d", weather.humidity);
            loginfof("sht temp: %d", weather.temperature_1);
            #endif
        }

        wdt_feed(&WDT_0);

        sleep(SYSTEM_SLEEPMODE_IDLE_2);
    }

    return 0;
}

int32_t fk_assert(const char *message, const char *file, int32_t line) {
    volatile int32_t i = 0;
    while (1) {
        i++;
    }

    return FK_SUCCESS;
}

uint32_t fk_weather_sign(fk_weather_t const *weather) {
    return crc32_checksum(FK_MODULES_CRC_SEED, (uint8_t const *)weather, sizeof(fk_weather_t) - sizeof(uint32_t));
}

extern char *sbrk(int32_t i);

uint32_t fk_free_memory() {
    char stack = 0;
    return (uint32_t)&stack - (uint32_t)sbrk(0);
}

void NMI_Handler(void) {
    logerror("isr: nmi fault");

    volatile int32_t i = 0;
    while (1) {
        i++;
    }
}

void HardFault_Handler(void) {
    logerror("isr: hard fault");

    volatile int32_t i = 0;
    while (1) {
        i++;
    }
}

void MemManage_Handler(void) {
    logerror("isr: memmanage fault");

    volatile int32_t i = 0;
    while (1) {
        i++;
    }
}

void BusFault_Handler(void) {
    logerror("isr: bus fault");

    volatile int32_t i = 0;
    while (1) {
        i++;
    }
}

void WDT_Handler(void) {
    logerror("isr: watchdog");
}
