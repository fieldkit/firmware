#include "board.h"
#include "sensors.h"
#include "i2c.h"
#include "sht31.h"
#include "mpl3115a2.h"
#include "adc081c.h"

static int32_t configure_io_expander(struct i2c_m_sync_desc *i2c, uint8_t address, uint8_t iodir) {
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
        0x00,   // GPIO
    };

    int32_t rv = i2c_write_buffer(i2c, address, buffer, sizeof(buffer));
    if (rv != FK_SUCCESS) {
        return rv;
    }

    return FK_SUCCESS;
}

int32_t sensors_initialize(struct i2c_m_sync_desc *i2c) {
    uint16_t status;
    int32_t rv;

    rv = configure_io_expander(i2c, MCP2803_RAIN_I2C_ADDRESS, MCP2803_RAIN_IODIR);
    if (rv != FK_SUCCESS) {
        logerror("rain-mcp: error initializing");
        return rv;
    }

    // TODO: Disabled until we get new hardware.
    if (false) {
        rv = configure_io_expander(i2c, MCP2803_WIND_I2C_ADDRESS, MCP2803_WIND_IODIR);
        if (rv != FK_SUCCESS) {
            logerror("wind-mcp: error initializing");
            return rv;
        }
    }
    else {
        loginfo("wind-mcp: skipping");
    }

    rv = configure_io_expander(i2c, MCP2803_CONTROL_I2C_ADDRESS, MCP2803_CONTROL_IODIR);
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
