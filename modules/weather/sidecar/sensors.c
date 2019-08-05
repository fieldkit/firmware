#include "board.h"
#include "sensors.h"

#define SHT31_I2C_ADDRESS                                (0x44 >> 1)
#define SHT31_MEAS_HIGHREP_STRETCH                       (0x2C06)
#define SHT31_MEAS_MEDREP_STRETCH                        (0x2C0D)
#define SHT31_MEAS_LOWREP_STRETCH                        (0x2C10)
#define SHT31_MEAS_HIGHREP                               (0x2400)
#define SHT31_MEAS_MEDREP                                (0x240B)
#define SHT31_MEAS_LOWREP                                (0x2416)
#define SHT31_READ_STATUS                                (0xF32D)
#define SHT31_CLEAR_STATUS                               (0x3041)
#define SHT31_SOFT_RESET                                 (0x30A2)
#define SHT31_HEATER_EN                                  (0x306D)
#define SHT31_HEATER_DIS                                 (0x3066)

#define MPL3115A2_I2C_ADDRESS                            (0x60)
#define MPL3115A2_REGISTER_STATUS                        (0x00)
#define MPL3115A2_REGISTER_PRESSURE_MSB                  (0x01)
#define MPL3115A2_REGISTER_PRESSURE_CSB                  (0x02)
#define MPL3115A2_REGISTER_PRESSURE_LSB                  (0x03)
#define MPL3115A2_REGISTER_TEMP_MSB                      (0x04)
#define MPL3115A2_REGISTER_TEMP_LSB                      (0x05)
#define MPL3115A2_REGISTER_DR_STATUS                     (0x06)
#define MPL3115A2_OUT_P_DELTA_MSB                        (0x07)
#define MPL3115A2_OUT_P_DELTA_CSB                        (0x08)
#define MPL3115A2_OUT_P_DELTA_LSB                        (0x09)
#define MPL3115A2_OUT_T_DELTA_MSB                        (0x0A)
#define MPL3115A2_OUT_T_DELTA_LSB                        (0x0B)
#define MPL3115A2_WHOAMI                                 (0x0C)
#define MPL3115A2_BAR_IN_MSB                             (0x14)
#define MPL3115A2_BAR_IN_LSB                             (0x15)
#define MPL3115A2_WHOAMI_EXPECTED                        (0xC4)

#define ADC081C_I2C_ADDRESS                              (0x50)
#define ADC081C_REGISTER_READING                         (0x00)
#define ADC081C_REGISTER_ALERT_STATUS                    (0x01)
#define ADC081C_REGISTER_CONFIGURATION                   (0x02)

static int32_t i2c_write_command_u16(struct i2c_m_sync_desc *i2c, uint8_t address, uint16_t command) {
    struct _i2c_m_msg msg;
    int32_t rv;

    msg.addr   = address;
    msg.flags  = I2C_M_SEVEN;
    msg.buffer = (void *)&command;
    msg.len    = sizeof(uint16_t);
    rv = _i2c_m_sync_transfer(&i2c->device, &msg);
    if (rv != 0) {
        return rv;
    }

    return FK_SUCCESS;
}

static int32_t i2c_write_buffer(struct i2c_m_sync_desc *i2c, uint8_t address, uint8_t *buffer, size_t size) {
    struct _i2c_m_msg msg;
    int32_t rv;

    msg.addr   = address;
    msg.flags  = I2C_M_SEVEN | I2C_M_STOP;
    msg.buffer = (void *)buffer;
    msg.len    = size;
    rv = _i2c_m_sync_transfer(&i2c->device, &msg);
    if (rv != 0) {
        return rv;
    }

    return FK_SUCCESS;
}

static int32_t i2c_read_buffer(struct i2c_m_sync_desc *i2c, uint8_t address, uint8_t *buffer, size_t size) {
    struct _i2c_m_msg msg;
    int32_t rv;

    msg.addr   = address;
    msg.flags  = I2C_M_SEVEN | I2C_M_RD | I2C_M_STOP;
    msg.buffer = (void *)buffer;
    msg.len    = size;
    rv = _i2c_m_sync_transfer(&i2c->device, &msg);
    if (rv != 0) {
        return rv;
    }

    return FK_SUCCESS;
}

static int32_t i2c_read_u8(struct i2c_m_sync_desc *i2c, uint8_t address, uint8_t reg, uint8_t *value) {
    struct _i2c_m_msg msg;
    int32_t rv;

    msg.addr   = address;
    msg.flags  = I2C_M_SEVEN;
    msg.buffer = (void *)&reg;
    msg.len    = sizeof(uint8_t);
    rv = _i2c_m_sync_transfer(&i2c->device, &msg);
    if (rv != 0) {
        return rv;
    }

    msg.flags  = I2C_M_STOP | I2C_M_RD | I2C_M_SEVEN;
    msg.buffer = (void *)value;
    msg.len    = sizeof(uint8_t);
    rv = _i2c_m_sync_transfer(&i2c->device, &msg);
    if (rv != 0) {
        return rv;
    }

    return FK_SUCCESS;
}

static int32_t i2c_write_u8(struct i2c_m_sync_desc *i2c, uint8_t address, uint8_t reg, uint8_t value) {
    uint8_t buffer[2] = { reg, value };
    struct _i2c_m_msg msg;
    int32_t rv;

    msg.addr   = address;
    msg.flags  = I2C_M_SEVEN | I2C_M_STOP;
    msg.buffer = (void *)&buffer;
    msg.len    = sizeof(buffer);

    rv = _i2c_m_sync_transfer(&i2c->device, &msg);
    if (rv != 0) {
        return rv;
    }

    return FK_SUCCESS;
}

static int32_t i2c_read_u16(struct i2c_m_sync_desc *i2c, uint8_t address, uint8_t reg, uint16_t *value) {
    struct _i2c_m_msg msg;
    int32_t rv;

    msg.addr   = address;
    msg.flags  = I2C_M_SEVEN;
    msg.buffer = (void *)&reg;
    msg.len    = sizeof(uint8_t);
    rv = _i2c_m_sync_transfer(&i2c->device, &msg);
    if (rv != 0) {
        return rv;
    }

    msg.flags  = I2C_M_STOP | I2C_M_RD | I2C_M_SEVEN;
    msg.buffer = (void *)value;
    msg.len    = sizeof(uint16_t);
    rv = _i2c_m_sync_transfer(&i2c->device, &msg);
    if (rv != 0) {
        return rv;
    }

    return FK_SUCCESS;
}

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

int32_t sht31_initialize(struct i2c_m_sync_desc *i2c) {
    int32_t rv;

    rv = i2c_write_command_u16(i2c, SHT31_I2C_ADDRESS, SHT31_SOFT_RESET);
    if (rv != 0) {
        return rv;
    }

    delay_ms(1);

    return FK_SUCCESS;
}

int32_t sht31_status_get(struct i2c_m_sync_desc *i2c, uint16_t *status) {
    int32_t rv;

    rv = i2c_write_command_u16(i2c, SHT31_I2C_ADDRESS, SHT31_READ_STATUS);
    if (rv != 0) {
        return rv;
    }

    uint8_t buffer[3];
    rv = i2c_read_buffer(i2c, SHT31_I2C_ADDRESS, buffer, sizeof(buffer));
    if (rv != 0) {
        return rv;
    }

    *status = 0;
    *status |= buffer[0] << 8;
    *status |= buffer[1];

    return FK_SUCCESS;
}

int32_t sht31_reading_get(struct i2c_m_sync_desc *i2c, sht31_reading_t *reading) {
    int32_t rv;

    rv = i2c_write_command_u16(i2c, SHT31_I2C_ADDRESS, SHT31_MEAS_HIGHREP);
    if (rv != 0) {
        return rv;
    }

    uint8_t buffer[6];
    rv = i2c_read_buffer(i2c, SHT31_I2C_ADDRESS, buffer, sizeof(buffer));
    if (rv != 0) {
        return rv;
    }

    return FK_SUCCESS;
}

int32_t mpl3115a2_initialize(struct i2c_m_sync_desc *i2c) {
    int32_t rv;

    uint8_t identity;
    rv = i2c_read_u8(i2c, MPL3115A2_I2C_ADDRESS, MPL3115A2_WHOAMI, &identity);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    if (identity != MPL3115A2_WHOAMI_EXPECTED) {
        return FK_ERROR_GENERAL;
    }

    return FK_SUCCESS;
}

int32_t mpl3115a2_reading_get(struct i2c_m_sync_desc *i2c, mpl3115a2_reading_t *reading) {
    return FK_SUCCESS;
}

int32_t adc081c_initialize(struct i2c_m_sync_desc *i2c) {
    int32_t rv;

    uint8_t configuration;
    rv = i2c_read_u8(i2c, ADC081C_I2C_ADDRESS, ADC081C_REGISTER_CONFIGURATION, &configuration);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    return FK_SUCCESS;
}

int32_t adc081c_reading_get(struct i2c_m_sync_desc *i2c, adc081c_reading_t *reading) {
    int32_t rv;

    rv = i2c_read_u16(i2c, ADC081C_I2C_ADDRESS, ADC081C_REGISTER_READING, &reading->value);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    return FK_SUCCESS;
}
