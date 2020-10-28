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

int32_t take_readings(fk_weather_t *weather, uint8_t *failures) {
    int32_t rv;

    mpl3115a2_reading_t mpl3115a2_reading;
    rv = mpl3115a2_reading_get(&I2C_1, &mpl3115a2_reading);
    if (rv != FK_SUCCESS) {
        logerrorf("mpl3115a2 (%d)", rv);
        *failures++;
    }

    sht31_reading_t sht31_reading;
    rv = sht31_reading_get(&I2C_1, &sht31_reading);
    if (rv != FK_SUCCESS) {
        logerrorf("sht31 (%d)", rv);
        *failures++;
    }

    #if !defined(FK_WEATHER_UNMETERED)
    adc081c_reading_t wind_direction;
    rv = adc081c_reading_get(&I2C_1, &wind_direction);
    if (rv != FK_SUCCESS) {
        logerrorf("adc081c (%d)", rv);
        *failures++;
    }

    counters_reading_t counters_reading;
    rv = counters_reading_get(&I2C_1, &counters_reading);
    if (rv != FK_SUCCESS) {
        logerrorf("counters (%d)", rv);
        *failures++;
    }
    #endif

    weather->seconds++;
    weather->session++;
    weather->humidity = sht31_reading.humidity;
    weather->temperature_1 = sht31_reading.temperature;
    weather->pressure = mpl3115a2_reading.pressure;
    weather->temperature_2 = mpl3115a2_reading.temperature;
    #if !defined(FK_WEATHER_UNMETERED)
    weather->wind.direction = wind_direction.value;
    weather->wind.ticks = counters_reading.wind;
    weather->rain.ticks = counters_reading.rain;
    #endif
    weather->crc = fk_weather_sign(weather);

    if (*failures > 0) {
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

#if defined(FK_WEATHER_STAND_ALONE)

static bool eeprom_clear_or_outside_window(uint32_t now) {
    return true;
}

static void regmap_before_read(void *ptr) {
    #if !defined(FK_WEATHER_UNMETERED)
    fk_weather_aggregated_t *aw = (fk_weather_aggregated_t *)ptr;

    aw->previous_wind = aw->wind;
    aw->previous_rain = aw->rain;

    memzero(&aw->wind, sizeof(aw->wind));
    memzero(&aw->rain, sizeof(aw->rain));
    #endif
}

#else

static volatile uint32_t eeprom_signaled = 0;
static volatile uint32_t eeprom_signals = 0;

static void eeprom_signal() {
    uint32_t now = board_system_time_get();
    if (eeprom_lock_test()) {
        if (eeprom_signaled == 0 || now - eeprom_signaled > FK_MODULES_EEPROM_WARNING_WINDOW) {
            eeprom_signaled = now;
            eeprom_signals = 0;
        }
        eeprom_signals++;
    }
}

static bool eeprom_clear_or_outside_window(uint32_t now) {
    return eeprom_signaled == 0 || now - eeprom_signaled > FK_MODULES_EEPROM_WARNING_WINDOW;
}

#endif

__int32_t main() {
    SEGGER_RTT_SetFlagsUpBuffer(0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);

    board_initialize();

    loginfof("board ready (%zd, %zd)", sizeof(fk_weather_t), sizeof(fk_weather_aggregated_t));

    #if !defined(FK_WEATHER_STAND_ALONE)
    if (ext_irq_register(PA25, eeprom_signal) != 0) {
        logerror("registering irq");
    }

    if (ext_irq_enable(PA25) != 0) {
        logerror("enabling irq");
    }
    #endif

    #if !defined(FK_WEATHER_STAND_ALONE)
    loginfo("eeprom...");

    // When we startup, sometimes the parent will hold this high so that it can
    // talk to the bus. Give the parent a chance to do that.
    while (eeprom_lock_test()) {
        delay_ms(100);
    }
    #endif

    uint32_t clk_rate = 1000;
    uint16_t to_period = 4096;
    wdt_set_timeout_period(&WDT_0, clk_rate, to_period);
    wdt_enable(&WDT_0);

    fk_weather_t weather;
    memzero(&weather, sizeof(fk_weather_t));

    #if defined(FK_WEATHER_STAND_ALONE)

    fk_weather_aggregated_t aggregated;
    memzero(&aggregated, sizeof(fk_weather_aggregated_t));

    board_register_map_t regmap = {
        (uint8_t *)&aggregated,
        sizeof(aggregated),
        0,
        regmap_before_read,
    };

    FK_ASSERT(board_subordinate_initialize(&regmap) == FK_SUCCESS);

    #else // defined(FK_WEATHER_STAND_ALONE)

    eeprom_region_t readings_region;
    FK_ASSERT(eeprom_region_create(&readings_region, &I2C_0_m, EEPROM_ADDRESS_READINGS, EEPROM_ADDRESS_READINGS_END, sizeof(fk_weather_t)) == FK_SUCCESS);

    loginfo("eeprom...");

    board_eeprom_i2c_enable();

    if (eeprom_region_seek_end(&readings_region, &weather.seconds, &weather.startups) != FK_SUCCESS) {
        logerror("eeprom end");
        board_eeprom_i2c_disable();
        delay_ms(8000);
        NVIC_SystemReset();
    }

    unwritten_readings_t ur;
    FK_ASSERT(unwritten_readings_initialize(&ur) == FK_SUCCESS);

    board_eeprom_i2c_disable();

    weather.startups++;

    loginfof("startup=%d, done", weather.startups);

    #endif // defined(FK_WEATHER_STAND_ALONE)

    board_sensors_i2c_enable();

    sensors_t sensors = { 0, 0 };
    int32_t rv = sensors_initialize(&I2C_1, &sensors);
    if (rv != FK_SUCCESS) {
        #if !defined(FK_WEATHER_STAND_ALONE)
        eeprom_region_append_error(&readings_region, weather.startups, FK_WEATHER_ERROR_SENSORS_STARTUP, 0, sensors.failures);
        #endif

        if (sensors.working == 0) {
            i2c_sensors_recover();
            delay_ms(8000);
            NVIC_SystemReset();
        }
    }

    struct timer_task timer_task;
    FK_ASSERT(board_timer_setup(&timer_task, 1000, timer_task_cb) == FK_SUCCESS);

    loginfo("ready!");

    while (true) {
        uint32_t now = board_system_time_get();

        if (take_readings_triggered && eeprom_clear_or_outside_window(now)) {
            take_readings_triggered = 0;

            uint8_t failures = 0;
            int32_t rv = take_readings(&weather, &failures);
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

            #if defined(FK_WEATHER_STAND_ALONE)

            struct calendar_date_time clock;
            calendar_get_date_time(&CALENDAR_0, &clock);

            FK_ASSERT(aggregated_weather_include(&aggregated, &clock, &weather) == FK_SUCCESS);

            #else // defined(FK_WEATHER_STAND_ALONE)

            unwritten_readings_push(&ur, &weather);

            int32_t nentries = unwritten_readings_get_size(&ur);
            rv = eeprom_region_append_unwritten(&readings_region, &ur);
            if (rv != FK_SUCCESS) {
                if (rv == FK_ERROR_BUSY) {
                    loginfo("readings: eeprom busy");
                }
                else {
                    logerrorf("readings: error appending (%" PRIu32 ")", nentries);
                    weather.memory_failures++;
                }
            }
            else {
                weather.memory_failures = 0;
            }

            #endif // defined(FK_WEATHER_STAND_ALONE)

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
            #else // defined(FK_LOGGING)
            SEGGER_RTT_WriteString(0, ".");
            #endif // defined(FK_LOGGING)
        }

        wdt_feed(&WDT_0);

        sleep(SYSTEM_SLEEPMODE_IDLE_2);

        #if !defined(FK_WEATHER_STAND_ALONE)
        if (eeprom_signals >= FK_MODULES_EEPROM_WARNING_TICKS) {
            loginfof("warning signal: %" PRIu32, eeprom_signals);
            delay_ms((eeprom_signals - 1) * FK_MODULES_EEPROM_WARNING_SLEEP_PER_TICK);
            eeprom_signals = 0;
        }
        #endif // !defined(FK_WEATHER_STAND_ALONE)
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
