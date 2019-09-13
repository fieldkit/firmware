#include "mpl3115a2.h"
#include "i2c.h"

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

#define MPL3115A2_REGISTER_STATUS_TDR                    (0x02)
#define MPL3115A2_REGISTER_STATUS_PDR                    (0x04)
#define MPL3115A2_REGISTER_STATUS_PTDR                   (0x08)

#define MPL3115A2_CTRL_REG1                              (0x26)
#define MPL3115A2_CTRL_REG2                              (0x27)
#define MPL3115A2_CTRL_REG3                              (0x28)
#define MPL3115A2_CTRL_REG4                              (0x29)
#define MPL3115A2_CTRL_REG5                              (0x2A)

#define MPL3115A2_CTRL_REG1_SBYB                         (0x01)
#define MPL3115A2_CTRL_REG1_OST                          (0x02)
#define MPL3115A2_CTRL_REG1_RST                          (0x04)
#define MPL3115A2_CTRL_REG1_RAW                          (0x40)
#define MPL3115A2_CTRL_REG1_ALT                          (0x80)
#define MPL3115A2_CTRL_REG1_BAR                          (0x00)

#define MPL3115A2_PT_DATA_CFG                            (0x13)
#define MPL3115A2_PT_DATA_CFG_TDEFE                      (0x01)
#define MPL3115A2_PT_DATA_CFG_PDEFE                      (0x02)
#define MPL3115A2_PT_DATA_CFG_DREM                       (0x04)

#define MPL3115A2_CTRL_REG1_OS1                          (0x00)
#define MPL3115A2_CTRL_REG1_OS2                          (0x08)
#define MPL3115A2_CTRL_REG1_OS4                          (0x10)
#define MPL3115A2_CTRL_REG1_OS8                          (0x18)
#define MPL3115A2_CTRL_REG1_OS16                         (0x20)
#define MPL3115A2_CTRL_REG1_OS32                         (0x28)
#define MPL3115A2_CTRL_REG1_OS64                         (0x30)
#define MPL3115A2_CTRL_REG1_OS128                        (0x38)

typedef union {
    struct {
        uint8_t SBYB:1;
        uint8_t OST:1;
        uint8_t RST:1;
        uint8_t OS:3;
        uint8_t RAW:1;
        uint8_t ALT:1;
    } bit;
    uint8_t reg;
} mpl3115a2_ctrl_reg1;

static int32_t mpl3115a2_flag_poll(struct i2c_m_sync_desc *i2c, uint8_t reg, uint8_t flag, uint8_t desired) {
    uint8_t status = 0;

    while (true) {
        uint8_t value;
        int32_t rv = i2c_read_u8(i2c, MPL3115A2_I2C_ADDRESS, reg, &value);
        if (rv != FK_SUCCESS) {
            return rv;
        }

        if ((value & flag) == desired) {
            break;
        }

        delay_ms(10);
    }

    return FK_SUCCESS;
}

static int32_t mpl3115a2_status_poll(struct i2c_m_sync_desc *i2c, uint8_t flag) {
    return mpl3115a2_flag_poll(i2c, MPL3115A2_REGISTER_STATUS, flag, flag);
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

    // Initiate a reset, this never returns success. We poll for ready after.
    rv = i2c_write_u8(i2c, MPL3115A2_I2C_ADDRESS, MPL3115A2_CTRL_REG1, MPL3115A2_CTRL_REG1_RST);
    delay_ms(10); // NOTE Is this too long?
    rv = mpl3115a2_flag_poll(i2c, MPL3115A2_CTRL_REG1, MPL3115A2_CTRL_REG1_RST, 0);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    // Configure oversampling.
    rv = i2c_write_u8(i2c, MPL3115A2_I2C_ADDRESS, MPL3115A2_CTRL_REG1, MPL3115A2_CTRL_REG1_OS128);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    // Configure data ready flags.
    rv = i2c_write_u8(i2c, MPL3115A2_I2C_ADDRESS, MPL3115A2_PT_DATA_CFG, MPL3115A2_PT_DATA_CFG_TDEFE | MPL3115A2_PT_DATA_CFG_PDEFE | MPL3115A2_PT_DATA_CFG_DREM);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    return FK_SUCCESS;
}

int32_t mpl3115a2_reading_get(struct i2c_m_sync_desc *i2c, mpl3115a2_reading_t *reading) {
    int32_t rv;

    reading->pressure = 0;
    reading->temperature = 0;

    rv = mpl3115a2_flag_poll(i2c, MPL3115A2_CTRL_REG1, MPL3115A2_CTRL_REG1_OST, 0);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    // Initiate a reading.
    mpl3115a2_ctrl_reg1 reg1;
    reg1.reg = MPL3115A2_CTRL_REG1_OS128;
    reg1.bit.OST = 1;
    rv = i2c_write_u8(i2c, MPL3115A2_I2C_ADDRESS, MPL3115A2_CTRL_REG1, reg1.reg);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    rv = mpl3115a2_status_poll(i2c, MPL3115A2_REGISTER_STATUS_PTDR);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    uint8_t praw[3];
    rv = i2c_command_read_buffer(i2c, MPL3115A2_I2C_ADDRESS, MPL3115A2_REGISTER_PRESSURE_MSB, praw, sizeof(praw));
    if (rv != FK_SUCCESS) {
        return rv;
    }

    uint8_t traw[2];
    rv = i2c_command_read_buffer(i2c, MPL3115A2_I2C_ADDRESS, MPL3115A2_REGISTER_TEMP_MSB, traw, sizeof(traw));
    if (rv != FK_SUCCESS) {
        return rv;
    }

    /**
     * The Pressure data is stored as an unsigned fractional 20-bit value in Pascals in Q18.2 format. The OUT_P_MSB and
     * OUT_P_CSB registers and bits 7-6 of the OUT_P_LSB register contain the integer part in Pascals. Bits 5-4 of OUT_P_LSB con-
     * tain the fractional component. Left shifting the OUT_T_MSB byte by 16 bits into a 32 variable and doing a logical OR with the
     * OUT_T_CSB byte left shifted 8 bits and a logical OR with the OUT_T_LSB byte
     * gives the pressure in Pascals times 64
     */
    reading->pressure = ((praw[0] << 16) | (praw[1] << 8) | (praw[2]));
    // pressure pascals = pressure / 64.0

    /**
     * The Temperature data is stored as a signed fractional 12-bit value in °C in Q12.4 format. The OUT_T_MSB register contains
     * the integer part in °C and the OUT_T_LSB register contains the fractional part. Left shifting the OUT_T_MSB byte by 8 bits into
     * a 16-bit variable and adding the OUT_T_LSB byte with a logical OR gives the temperature in °C times 256. When RAW is se-
     * lected then the RAW value is stored in all 16 bits of OUT_T_MSB and OUT_T_LSB.
     */
    reading->temperature = ((traw[0] << 8) | (traw[1])) >> 4;
    if (reading->temperature & 0x800) {
        reading->temperature |= 0xF000;
    }
    // temp °C  = temperature / 16.0

    return FK_SUCCESS;
}
