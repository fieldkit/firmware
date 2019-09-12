#include "board.h"
#include "sensors.h"
#include "i2c.h"
#include "sht31.h"
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
 *                                          OLL ELL
 *                                          EKR OKR
 *                                          RRR WWW
 */
#define MCP2803_CONTROL_GPIO_INITIAL    (0b00010001)
#define MCP2803_CONTROL_GPIO_STORE_1    (0b00010001)
#define MCP2803_CONTROL_GPIO_STORE_2    (0b00110011)
#define MCP2803_CONTROL_GPIO_RESET_1    (0b00010001)
#define MCP2803_CONTROL_GPIO_RESET_2    (0b00010001)
#define MCP2803_CONTROL_GPIO_OUTPUT     (0b00010001)

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

int32_t sensors_initialize(struct i2c_m_sync_desc *i2c) {
    uint16_t status;
    int32_t rv;

    rv = configure_io_expander(i2c, MCP2803_RAIN_I2C_ADDRESS, MCP2803_RAIN_IODIR, 0);
    if (rv != FK_SUCCESS) {
        logerror("rain-mcp: error initializing");
        // return rv;
    }

    rv = configure_io_expander(i2c, MCP2803_WIND_I2C_ADDRESS, MCP2803_WIND_IODIR, 0);
    if (rv != FK_SUCCESS) {
        logerror("wind-mcp: error initializing");
        // return rv;
    }

    rv = configure_io_expander(i2c, MCP2803_CONTROL_I2C_ADDRESS, MCP2803_CONTROL_IODIR, MCP2803_CONTROL_GPIO_INITIAL);
    if (rv != FK_SUCCESS) {
        logerror("control-mcp: error initializing");
        // return rv;
    }

    rv = sht31_initialize(i2c);
    if (rv != FK_SUCCESS) {
        logerror("sht31: error initializing");
        // return rv;
    }

    rv = sht31_status_get(i2c, &status);
    if (rv != FK_SUCCESS) {
        logerror("sht31: error getting status");
        // return rv;
    }

    rv = mpl3115a2_initialize(i2c);
    if (rv != FK_SUCCESS) {
        logerror("mpl3115a2: error initializing");
        // return rv;
    }

    rv = adc081c_initialize(i2c);
    if (rv != FK_SUCCESS) {
        logerror("adc081c: error initializing");
        // return FK_ERROR_GENERAL;
    }

    return FK_SUCCESS;
}

int32_t counters_cycle(struct i2c_m_sync_desc *i2c) {
    int32_t rv;

    uint8_t states[] = {
        MCP2803_CONTROL_GPIO_STORE_1,
        MCP2803_CONTROL_GPIO_STORE_2,
        MCP2803_CONTROL_GPIO_RESET_1,
        MCP2803_CONTROL_GPIO_RESET_2,
        MCP2803_CONTROL_GPIO_OUTPUT,
    };

    for (size_t i = 0; i < sizeof(states); ++i) {
        uint8_t state = states[i];

        rv = i2c_write_u8(i2c, MCP2803_CONTROL_I2C_ADDRESS, MCP23008_GPIO, state);
        if (rv != FK_SUCCESS) {
            return rv;
        }
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
    uint8_t rain;
    uint8_t wind;

    // NOTE Should this be an invalid value?
    reading->wind = 0;
    reading->rain = 0;

    rv = counters_cycle(i2c);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    rv = i2c_command_read_buffer(i2c, MCP2803_RAIN_I2C_ADDRESS, MCP23008_GPIO, &rain, sizeof(uint8_t));
    if (rv != FK_SUCCESS) {
        return rv;
    }

    rv = i2c_command_read_buffer(i2c, MCP2803_WIND_I2C_ADDRESS, MCP23008_GPIO, &wind, sizeof(uint8_t));
    if (rv != FK_SUCCESS) {
        return rv;
    }

    rv = i2c_write_u8(i2c, MCP2803_CONTROL_I2C_ADDRESS, MCP23008_GPIO, MCP2803_CONTROL_GPIO_INITIAL);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    reading->wind = reverse(wind);
    reading->rain = reverse(rain);

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

/**
 *
 *                                           RC  RC
 *                                           CC  CC
 *                                          OLL OLL
 *                                          EKR EKR
 *                                         _RRR_WWW
 */
#define MCP2803_CONTROL_GPIO_A          (0b00010001)
#define MCP2803_CONTROL_GPIO_B          (0b00110011)
#define MCP2803_CONTROL_GPIO_C          (0b00000000)
#define MCP2803_CONTROL_GPIO_D          (0b00110011)

int32_t counters_test(struct i2c_m_sync_desc *i2c) {
    int32_t rv;

    while (1) {
        rv = i2c_write_u8(i2c, MCP2803_CONTROL_I2C_ADDRESS, MCP23008_GPIO, MCP2803_CONTROL_GPIO_A);
        if (rv != FK_SUCCESS) {
            return rv;
        }

        rv = i2c_write_u8(i2c, MCP2803_CONTROL_I2C_ADDRESS, MCP23008_GPIO, MCP2803_CONTROL_GPIO_B);
        if (rv != FK_SUCCESS) {
            return rv;
        }

        uint8_t rain_gpio = 10;
        rv = i2c_read_u8(i2c, MCP2803_RAIN_I2C_ADDRESS, MCP23008_GPIO, &rain_gpio);
        if (rv != FK_SUCCESS) {
            return rv;
        }
        uint8_t wind_gpio = 10;
        rv = i2c_read_u8(i2c, MCP2803_WIND_I2C_ADDRESS, MCP23008_GPIO, &wind_gpio);
        if (rv != FK_SUCCESS) {
            return rv;
        }

        if (wind_gpio > 0 || rain_gpio > 0) {
            loginfof("counters: %" PRIx32 " %" PRIx32, reverse(rain_gpio), reverse(wind_gpio));
        }

        rv = i2c_write_u8(i2c, MCP2803_CONTROL_I2C_ADDRESS, MCP23008_GPIO, MCP2803_CONTROL_GPIO_C);
        if (rv != FK_SUCCESS) {
            return rv;
        }

        rv = i2c_write_u8(i2c, MCP2803_CONTROL_I2C_ADDRESS, MCP23008_GPIO, MCP2803_CONTROL_GPIO_D);
        if (rv != FK_SUCCESS) {
            return rv;
        }

        delay_ms(1000);
    }

    return FK_SUCCESS;
}
