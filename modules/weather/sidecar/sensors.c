#include "board.h"
#include "sensors.h"
#include "i2c.h"
#include "sht31.h"
#include "bme280.h"
#include "mpl3115a2.h"
#include "adc081c.h"

#define MCP2803_RAIN_I2C_ADDRESS        (0x20 + 0x1)
#define MCP2803_RAIN_IODIR              (0b11111111)

#define MCP2803_WIND_I2C_ADDRESS        (0x20 + 0x4)
#define MCP2803_WIND_IODIR              (0b11111111)

#define MCP2803_CONTROL_I2C_ADDRESS     (0x20 + 0x2)
#define MCP2803_CONTROL_IODIR           (0b00000000)

/**
 *
 *                                           RC  RC
 *                                           CC  CC
 *                                          OLL OLL
 *                                          EKR EKR
 *                                          RRR WWW
 */
#define MCP2803_CONTROL_GPIO_INITIAL    (0b00010001)

#define MCP2803_CONTROL_GPIO_A          (0b00010001)
#define MCP2803_CONTROL_GPIO_B          (0b00110011)
#define MCP2803_CONTROL_GPIO_C          (0b00000000)
#define MCP2803_CONTROL_GPIO_D          (0b00110011)

#define MCP23008_IODIR                  (0x00)
#define MCP23008_IPOL                   (0x01)
#define MCP23008_GPINTEN                (0x02)
#define MCP23008_DEFVAL                 (0x03)
#define MCP23008_INTCON                 (0x04)
#define MCP23008_IOCON                  (0x05)
#define MCP23008_GPPU                   (0x06)
#define MCP23008_INTF                   (0x07)
#define MCP23008_INTCAP                 (0x08)
#define MCP23008_GPIO                   (0x09)
#define MCP23008_OLAT                   (0x0A)

static int32_t configure_io_expander(struct i2c_m_sync_desc *i2c, uint8_t address, uint8_t iodir, uint8_t gpio);

static void log_sensor_error(const char *which, int32_t rv) {
    logerrorf("%s: error (%d)", which, rv);
}

int32_t sensors_initialize(struct i2c_m_sync_desc *i2c, sensors_t *sensors) {
    uint16_t status;
    int32_t rv;

    // All sensors are initialized to begin with and we clear these
    // bits as we go.
    sensors->initialized = FK_WEATHER_SENSORS_ALL;

    #if !defined(FK_WEATHER_UNMETERED)
    rv = configure_io_expander(i2c, MCP2803_RAIN_I2C_ADDRESS, MCP2803_RAIN_IODIR, 0);
    if (rv != FK_SUCCESS) {
        log_sensor_error("rain-mcp", rv);
        sensors->initialized ^= FK_WEATHER_SENSORS_COUNTERS;
    }

    rv = configure_io_expander(i2c, MCP2803_WIND_I2C_ADDRESS, MCP2803_WIND_IODIR, 0);
    if (rv != FK_SUCCESS) {
        log_sensor_error("wind-mcp", rv);
        sensors->initialized ^= FK_WEATHER_SENSORS_COUNTERS;
    }

    rv = configure_io_expander(i2c, MCP2803_CONTROL_I2C_ADDRESS, MCP2803_CONTROL_IODIR, MCP2803_CONTROL_GPIO_INITIAL);
    if (rv != FK_SUCCESS) {
        log_sensor_error("control-mcp", rv);
        sensors->initialized ^= FK_WEATHER_SENSORS_COUNTERS;
    }

    rv = adc081c_initialize(i2c);
    if (rv != FK_SUCCESS) {
        log_sensor_error("adc081c", rv);
        sensors->initialized ^= FK_WEATHER_SENSORS_ADC;
    }
    #endif

    rv = bme280_initialize(i2c);
    if (rv != FK_SUCCESS) {
        log_sensor_error("bme280", rv);

        sensors->initialized ^= FK_WEATHER_SENSORS_BME280;

        #if defined(FK_ENABLE_SHT31_AND_MPL3115A2)
        rv = sht31_initialize(i2c);
        if (rv != FK_SUCCESS) {
            log_sensor_error("sht31", rv);
            sensors->initialized ^= FK_WEATHER_SENSORS_SHT31;
        }

        rv = mpl3115a2_initialize(i2c);
        if (rv != FK_SUCCESS) {
            log_sensor_error("mpl3115a2", rv);
            sensors->initialized ^= FK_WEATHER_SENSORS_MPL3115A2;
        }
        #endif
    }
    else {
        sensors->initialized ^= FK_WEATHER_SENSORS_SHT31;
        sensors->initialized ^= FK_WEATHER_SENSORS_MPL3115A2;
    }

    return FK_SUCCESS;
}

static uint8_t reverse(uint8_t b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

int32_t counters_reading_get(struct i2c_m_sync_desc *i2c, counters_reading_t *reading) {
    int32_t rv;
    uint8_t rain_reversed;
    uint8_t wind_reversed;

    // NOTE Should this be an invalid value?
    reading->wind = 0;
    reading->rain = 0;

    rv = i2c_write_u8(i2c, MCP2803_CONTROL_I2C_ADDRESS, MCP23008_GPIO, MCP2803_CONTROL_GPIO_A);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    rv = i2c_write_u8(i2c, MCP2803_CONTROL_I2C_ADDRESS, MCP23008_GPIO, MCP2803_CONTROL_GPIO_B);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    rv = i2c_read_u8(i2c, MCP2803_RAIN_I2C_ADDRESS, MCP23008_GPIO, &rain_reversed);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    rv = i2c_read_u8(i2c, MCP2803_WIND_I2C_ADDRESS, MCP23008_GPIO, &wind_reversed);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    rv = i2c_write_u8(i2c, MCP2803_CONTROL_I2C_ADDRESS, MCP23008_GPIO, MCP2803_CONTROL_GPIO_C);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    rv = i2c_write_u8(i2c, MCP2803_CONTROL_I2C_ADDRESS, MCP23008_GPIO, MCP2803_CONTROL_GPIO_D);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    reading->wind = reverse(wind_reversed);
    reading->rain = reverse(rain_reversed);

    #if defined(FK_LOGGING)
    loginfof("wind: raw=%" PRIu32 " val=%" PRIu32, wind_reversed, reading->wind);
    loginfof("rain: raw=%" PRIu32 " val=%" PRIu32, rain_reversed, reading->rain);
    #endif

    return FK_SUCCESS;
}

static int32_t configure_io_expander(struct i2c_m_sync_desc *i2c, uint8_t address, uint8_t iodir, uint8_t gpio) {
    uint8_t buffer[] = {
        0x00,   // IODIR (Address)
        iodir,  // IODIR
        0x00,   // IPOL
        0x00,   // GPINTEN
        0x00,   // DEFVAL
        0x00,   // INTCON
        0x00,   // IOCON
        0x00,   // GPPU
        0x00,   // INTF
        0x00,   // INTCAP
        gpio,   // GPIO
    };

    int32_t rv = i2c_write_buffer(i2c, address, buffer, sizeof(buffer));
    if (rv != FK_SUCCESS) {
        return rv;
    }

    return FK_SUCCESS;
}
