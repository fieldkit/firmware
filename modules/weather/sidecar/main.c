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
#include "aggregated.h"

static volatile uint8_t take_readings_triggered = 0;

static void timer_task_cb(struct timer_task const *const timer_task) {
    take_readings_triggered = 1;
}

/**
 * See take_readings.
 */
#define FK_WEATHER_TAKE_READINGS_SENSORS            (4)

int32_t fake_readings(fk_weather_t *weather) {
    weather->seconds++;
    weather->session++;
    weather->humidity = 0;
    weather->temperature_1 = 0;
    weather->pressure = 0;
    weather->temperature_2 = 0;
    #if !defined(FK_WEATHER_UNMETERED)
    weather->wind.direction = 0;
    weather->wind.ticks = 0;
    weather->rain.ticks = 1;
    #endif
    weather->crc = fk_weather_sign(weather);

    return FK_SUCCESS;
}

int32_t take_readings(fk_weather_t *weather, sensors_t *sensors, uint8_t *failures_rv) {
    uint8_t failures = 0;
    int32_t rv;

    if ((sensors->initialized & FK_WEATHER_SENSORS_BME280) == 1) {
        failures |= FK_WEATHER_SENSORS_SHT31;
        failures |= FK_WEATHER_SENSORS_MPL3115A2;

        bme280_reading_t bme280_reading;
        rv = bme280_reading_get(&I2C_1, &bme280_reading);
        if (rv != FK_SUCCESS) {
            logerrorf("bme280 (%d)", rv);
            failures |= FK_WEATHER_SENSORS_BME280;
        }
        else {
            weather->humidity = bme280_reading.humidity;
            weather->temperature_1 = bme280_reading.temperature;
            weather->pressure = bme280_reading.pressure;
            weather->temperature_2 = weather->temperature_1;
        }
    }
    else {
        failures |= FK_WEATHER_SENSORS_BME280;

        #if defined(FK_ENABLE_SHT31_AND_MPL3115A2)
        mpl3115a2_reading_t mpl3115a2_reading;
        rv = mpl3115a2_reading_get(&I2C_1, &mpl3115a2_reading);
        if (rv != FK_SUCCESS) {
            logerrorf("mpl3115a2 (%d)", rv);
            failures |= FK_WEATHER_SENSORS_MPL3115A2;
        }

        sht31_reading_t sht31_reading;
        rv = sht31_reading_get(&I2C_1, &sht31_reading);
        if (rv != FK_SUCCESS) {
            logerrorf("sht31 (%d)", rv);
            failures |= FK_WEATHER_SENSORS_SHT31;
        }

        weather->humidity = sht31_reading.humidity;
        weather->temperature_1 = sht31_reading.temperature;
        weather->pressure = mpl3115a2_reading.pressure;
        weather->temperature_2 = mpl3115a2_reading.temperature;
        #else
        failures |= FK_WEATHER_SENSORS_SHT31;
        failures |= FK_WEATHER_SENSORS_MPL3115A2;
        #endif
    }

    #if !defined(FK_WEATHER_UNMETERED)
    adc081c_reading_t wind_direction;
    rv = adc081c_reading_get(&I2C_1, &wind_direction);
    if (rv != FK_SUCCESS) {
        logerrorf("adc081c (%d)", rv);
        failures |= FK_WEATHER_SENSORS_ADC;
    }

    counters_reading_t counters_reading;
    rv = counters_reading_get(&I2C_1, &counters_reading);
    if (rv != FK_SUCCESS) {
        logerrorf("counters (%d)", rv);
        failures |= FK_WEATHER_SENSORS_COUNTERS;
    }
    #endif

    (*failures_rv) = failures;
    weather->initialized = sensors->initialized;
    weather->failures = failures;
    weather->seconds++;
    weather->session++;
    #if !defined(FK_WEATHER_UNMETERED)
    weather->wind.direction = wind_direction.value;
    weather->wind.ticks = counters_reading.wind;
    weather->rain.ticks = counters_reading.rain;
    #endif
    weather->crc = fk_weather_sign(weather);

    if (failures > 0) {
        return FK_ERROR_GENERAL;
    }

    return FK_SUCCESS;
}

static void i2c_sensors_recover() {
    gpio_set_pin_direction(PA22, GPIO_DIRECTION_OUT);
    gpio_set_pin_direction(PA23, GPIO_DIRECTION_OUT);

    for (int32_t i = 0; i < 9; ++i) {
        gpio_set_pin_level(PA22, 1);
        delay_ms(10);
        gpio_set_pin_level(PA22, 0);
        delay_ms(10);
        gpio_set_pin_level(PA22, 1);
        delay_ms(10);
    }

    for (int32_t i = 0; i < 9; ++i) {
        gpio_set_pin_level(PA23, 1);
        delay_ms(10);
        gpio_set_pin_level(PA23, 0);
        delay_ms(10);
        gpio_set_pin_level(PA23, 1);
        delay_ms(10);
    }

    gpio_set_pin_direction(PA22, GPIO_DIRECTION_OFF);
    gpio_set_pin_direction(PA23, GPIO_DIRECTION_OFF);
}

static bool eeprom_clear_or_outside_window(uint32_t now) {
    return true;
}

static void regmap_before_read(void *ptr) {
    fk_weather_aggregated_t *aw = (fk_weather_aggregated_t *)ptr;

    #if defined(FK_WEATHER_UNMETERED)
    aw->wind_120s[0].ticks = FK_WEATHER_UNMETERED_MAGIC;
    aw->rain_60m[0].ticks = FK_WEATHER_UNMETERED_MAGIC;
    #endif

    aw->uptime = board_system_time_get();
    aw->crc = aggregated_weather_sign(aw);
}

__int32_t main() {
    SEGGER_RTT_SetFlagsUpBuffer(0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);

    board_initialize();

    loginfof("board ready (%zd, %zd)", sizeof(fk_weather_t), sizeof(fk_weather_aggregated_t));

    FK_ASSERT(sizeof(struct fkw_calendar_date_time) == sizeof(struct calendar_date_time));

    uint32_t clk_rate = 1000;
    uint16_t to_period = 4096;
    wdt_set_timeout_period(&WDT_0, clk_rate, to_period);
    wdt_enable(&WDT_0);

    fk_weather_t weather;
    memzero(&weather, sizeof(fk_weather_t));

    fk_weather_aggregated_t aggregated;
    aggregated_weather_initialize(&aggregated);

    board_register_map_t regmap = {
        (uint8_t *)&aggregated,
        sizeof(aggregated),
        0,
        regmap_before_read,
    };

    FK_ASSERT(board_subordinate_initialize(&regmap) == FK_SUCCESS);

    board_sensors_i2c_enable();

    sensors_t sensors = { 0 };
    int32_t rv = sensors_initialize(&I2C_1, &sensors);
    if (rv != FK_SUCCESS) {
        if (sensors.initialized == 0) {
            #if !defined(FK_WEATHER_IGNORE_NO_SENSORS)
            i2c_sensors_recover();
            delay_ms(8000);
            NVIC_SystemReset();
            #else
            loginfo("ignoring no sensors");
            #endif
        }
    }

    struct timer_task timer_task;
    FK_ASSERT(board_timer_setup(&timer_task, 1000, timer_task_cb) == FK_SUCCESS);

    while (true) {
        uint32_t now = board_system_time_get();

        if (take_readings_triggered && eeprom_clear_or_outside_window(now)) {
            take_readings_triggered = 0;

            #if defined(FK_WEATHER_IGNORE_NO_SENSORS)
            fake_readings(&weather);
            #else
            uint8_t failures = 0;
            int32_t rv = take_readings(&weather, &sensors, &failures);
            if (rv != FK_SUCCESS) {
                weather.error = FK_WEATHER_ERROR_SENSORS_READING;
                weather.reading_failures = failures;

                // Restart if we can't read any sensors.
                if (failures == FK_WEATHER_TAKE_READINGS_SENSORS) {
                    delay_ms(8000);
                    NVIC_SystemReset();
                }
            }
            else {
                weather.error = 0;
                weather.reading_failures = 0;
            }
            #endif

            struct calendar_date_time clock;
            calendar_get_date_time(&CALENDAR_0, &clock);

            FK_ASSERT(aggregated_weather_include(&aggregated, &clock, &weather) == FK_SUCCESS);

            #if defined(FK_LOGGING)
            SEGGER_RTT_WriteString(0, "\n");

            loginfof("crc: %" PRIu32 " 0x%" PRIx32 "", weather.seconds, weather.crc);
            loginfof("adc081c wind dir: %d", weather.wind.direction);
            loginfof("wind: %d", weather.wind.ticks);
            loginfof("rain: %d", weather.rain.ticks);
            loginfof("mpl pressure: %d", weather.pressure);
            loginfof("mpl temp: %d", weather.temperature_2);
            loginfof("sht humidity: %d", weather.humidity);
            loginfof("sht temp: %d", weather.temperature_1);
            #else // defined(FK_LOGGING)
            SEGGER_RTT_WriteString(0, ".");
            #endif // defined(FK_LOGGING)
        }

        wdt_feed(&WDT_0);

        sleep(SYSTEM_SLEEPMODE_IDLE_2);
    }

    return 0;
}

int32_t fk_assert(const char *message, const char *file, int32_t line) {
    delay_ms(8000);
    NVIC_SystemReset();

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
    NVIC_SystemReset();
}

void HardFault_Handler(void) {
    NVIC_SystemReset();
}

void MemManage_Handler(void) {
    NVIC_SystemReset();
}

void BusFault_Handler(void) {
    NVIC_SystemReset();
}

void WDT_Handler(void) {

}
