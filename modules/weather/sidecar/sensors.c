#include "sidecar.h"
#include "sensors.h"

#define SHT31_DEFAULT_ADDR           0x44 >> 1
#define SHT31_MEAS_HIGHREP_STRETCH   0x2C06
#define SHT31_MEAS_MEDREP_STRETCH    0x2C0D
#define SHT31_MEAS_LOWREP_STRETCH    0x2C10
#define SHT31_MEAS_HIGHREP           0x2400
#define SHT31_MEAS_MEDREP            0x240B
#define SHT31_MEAS_LOWREP            0x2416
#define SHT31_READ_STATUS            0xF32D
#define SHT31_CLEAR_STATUS           0x3041
#define SHT31_SOFT_RESET             0x30A2
#define SHT31_HEATER_EN              0x306D
#define SHT31_HEATER_DIS             0x3066

static int32_t write_command_u16(struct i2c_m_sync_desc *i2c, uint8_t address, uint16_t command) {
    i2c_m_sync_enable(i2c);

    struct _i2c_m_msg msg;
    msg.addr   = address;
    msg.len    = sizeof(uint16_t);
    msg.flags  = I2C_M_SEVEN;
    msg.buffer = (void *)&command;

    int32_t rv = _i2c_m_sync_transfer(&i2c->device, &msg);
    if (rv != 0) {
        return rv;
    }

    return 0;
}

int32_t sensors_initialize(struct i2c_m_sync_desc *i2c) {
    return FK_SUCCESS;
}

int32_t sht31_initialize(struct i2c_m_sync_desc *i2c) {
    int32_t rv;

    i2c_m_sync_enable(i2c);

    rv = write_command_u16(i2c, SHT31_DEFAULT_ADDR, SHT31_SOFT_RESET);
    if (rv != 0) {
        return rv;
    }

    delay_ms(1);

    return FK_SUCCESS;
}

int32_t sht31_status_get(struct i2c_m_sync_desc *i2c, uint16_t *status) {
    int32_t rv;

    i2c_m_sync_enable(i2c);

    rv = write_command_u16(i2c, SHT31_DEFAULT_ADDR, SHT31_READ_STATUS);
    if (rv != 0) {
        return rv;
    }

    uint8_t buffer[3];
    struct _i2c_m_msg msg;
    msg.addr   = SHT31_DEFAULT_ADDR;
    msg.flags  = I2C_M_STOP | I2C_M_RD;
    msg.buffer = buffer;
    msg.len    = sizeof(buffer);
    rv = _i2c_m_sync_transfer(&i2c->device, &msg);
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

    i2c_m_sync_enable(i2c);

    rv = write_command_u16(i2c, SHT31_DEFAULT_ADDR, SHT31_MEAS_HIGHREP);
    if (rv != 0) {
        return rv;
    }

    uint8_t buffer[6];
    struct _i2c_m_msg msg;
    msg.addr   = SHT31_DEFAULT_ADDR;
    msg.flags  = I2C_M_STOP | I2C_M_RD;
    msg.buffer = buffer;
    msg.len    = sizeof(buffer);
    rv = _i2c_m_sync_transfer(&i2c->device, &msg);
    if (rv != 0) {
        return rv;
    }

    return FK_SUCCESS;
}

#define MPL3115A2_ADDRESS                        (0x60)
#define MPL3115A2_REGISTER_STATUS                (0x00)

#define MPL3115A2_REGISTER_PRESSURE_MSB          (0x01)
#define MPL3115A2_REGISTER_PRESSURE_CSB          (0x02)
#define MPL3115A2_REGISTER_PRESSURE_LSB          (0x03)

#define MPL3115A2_REGISTER_TEMP_MSB              (0x04)
#define MPL3115A2_REGISTER_TEMP_LSB              (0x05)

#define MPL3115A2_REGISTER_DR_STATUS             (0x06)

#define MPL3115A2_OUT_P_DELTA_MSB                (0x07)
#define MPL3115A2_OUT_P_DELTA_CSB                (0x08)
#define MPL3115A2_OUT_P_DELTA_LSB                (0x09)

#define MPL3115A2_OUT_T_DELTA_MSB                (0x0A)
#define MPL3115A2_OUT_T_DELTA_LSB                (0x0B)

#define MPL3115A2_WHOAMI                         (0x0C)

#define MPL3115A2_BAR_IN_MSB                     (0x14)
#define MPL3115A2_BAR_IN_LSB                     (0x15)

#define MPL3115A2_WHOAMI_EXPECTED                (0xC4)

int32_t mpl3115a2_initialize(struct i2c_m_sync_desc *i2c) {
    int32_t rv;

    i2c_m_sync_enable(i2c);
    i2c_m_sync_set_slaveaddr(i2c, MPL3115A2_ADDRESS, I2C_M_SEVEN);

    uint8_t reg = MPL3115A2_WHOAMI;

    struct _i2c_m_msg msg;
    msg.addr   = MPL3115A2_ADDRESS;
    msg.flags  = I2C_M_SEVEN;
    msg.buffer = (void *)&reg;
    msg.len    = sizeof(reg);
    rv = _i2c_m_sync_transfer(&i2c->device, &msg);
    if (rv != 0) {
        return rv;
    }

    uint8_t value;
    msg.flags  = I2C_M_STOP | I2C_M_RD | I2C_M_SEVEN;
    msg.buffer = (void *)&value;
    msg.len    = sizeof(value);
    rv = _i2c_m_sync_transfer(&i2c->device, &msg);
    if (rv != 0) {
        return rv;
    }

    if (value != MPL3115A2_WHOAMI_EXPECTED) {
        return FK_ERROR_GENERAL;
    }

    return FK_SUCCESS;
}

int32_t mpl3115a2_reading_get(struct i2c_m_sync_desc *i2c) {
    i2c_m_sync_enable(i2c);

    return FK_SUCCESS;
}

#define ADC_ADDRESS                              (0x50)

int32_t adc_initialize(struct i2c_m_sync_desc *i2c) {
    int32_t rv;

    i2c_m_sync_enable(i2c);
    i2c_m_sync_set_slaveaddr(i2c, ADC_ADDRESS, I2C_M_SEVEN);

    uint8_t reg = 0x00;

    struct _i2c_m_msg msg;
    msg.addr   = ADC_ADDRESS;
    msg.flags  = I2C_M_SEVEN;
    msg.buffer = (void *)&reg;
    msg.len    = sizeof(reg);
    rv = _i2c_m_sync_transfer(&i2c->device, &msg);
    if (rv != 0) {
        return rv;
    }

    uint16_t value;
    msg.flags  = I2C_M_STOP | I2C_M_RD | I2C_M_SEVEN;
    msg.buffer = (void *)&value;
    msg.len    = sizeof(value);
    rv = _i2c_m_sync_transfer(&i2c->device, &msg);
    if (rv != 0) {
        return rv;
    }

    return FK_SUCCESS;
}
