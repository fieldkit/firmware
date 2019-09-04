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
#define MCP2803_CONTROL_IODIR           (0b10001000)
#define MCP2803_CONTROL_GPIO_DEFAULT    (0b00010001)

/**
 * Control:
 * CCLR-CLEAR
 * RCLK-UPDATE-OUTPUT
 * OUTPUT-ENABLE
 */

#define MCP23008_IODIR           0x00
#define MCP23008_IPOL            0x01
#define MCP23008_GPINTEN         0x02
#define MCP23008_DEFVAL          0x03
#define MCP23008_INTCON          0x04
#define MCP23008_IOCON           0x05
#define MCP23008_GPPU            0x06
#define MCP23008_INTF            0x07
#define MCP23008_INTCAP          0x08
#define MCP23008_GPIO            0x09
#define MCP23008_OLAT            0x0A

static int32_t configure_io_expander(struct i2c_m_sync_desc *i2c, uint8_t address, uint8_t iodir, uint8_t gpio);

int32_t sensors_initialize(struct i2c_m_sync_desc *i2c) {
    uint16_t status;
    int32_t rv;

    rv = configure_io_expander(i2c, MCP2803_RAIN_I2C_ADDRESS, MCP2803_RAIN_IODIR, 0);
    if (rv != FK_SUCCESS) {
        logerror("rain-mcp: error initializing");
        return rv;
    }

    rv = configure_io_expander(i2c, MCP2803_WIND_I2C_ADDRESS, MCP2803_WIND_IODIR, 0);
    if (rv != FK_SUCCESS) {
        logerror("wind-mcp: error initializing");
        return rv;
    }

    rv = configure_io_expander(i2c, MCP2803_CONTROL_I2C_ADDRESS, MCP2803_CONTROL_IODIR, MCP2803_CONTROL_GPIO_DEFAULT);
    if (rv != FK_SUCCESS) {
        logerror("control-mcp: error initializing");
        return rv;
    }

    rv = sht31_initialize(i2c);
    if (rv != FK_SUCCESS) {
        logerror("sht31: error initializing");
        return rv;
    }

    rv = sht31_status_get(i2c, &status);
    if (rv != FK_SUCCESS) {
        logerror("sht31: error getting status");
        return rv;
    }

    rv = mpl3115a2_initialize(i2c);
    if (rv != FK_SUCCESS) {
        logerror("mpl3115a2: error initializing");
        return rv;
    }

    rv = adc081c_initialize(i2c);
    if (rv != FK_SUCCESS) {
        logerror("adc081c: error initializing");
        return FK_ERROR_GENERAL;
    }

    return FK_SUCCESS;
}

int32_t counters_reading_get(struct i2c_m_sync_desc *i2c, counters_reading_t *reading) {
    int32_t rv;

    rv = i2c_command_read_buffer(i2c, MCP2803_RAIN_I2C_ADDRESS, MCP23008_GPIO, &reading->rain, sizeof(uint8_t));
    if (rv != FK_SUCCESS) {
        return rv;
    }

    rv = i2c_command_read_buffer(i2c, MCP2803_WIND_I2C_ADDRESS, MCP23008_GPIO, &reading->wind, sizeof(uint8_t));
    if (rv != FK_SUCCESS) {
        return rv;
    }

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
