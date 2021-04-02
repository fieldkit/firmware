#include "sht31.h"
#include "i2c.h"

#if defined(FK_ENABLE_SHT31_AND_MPL3115A2)

#define SHT31_I2C_ADDRESS                                (0x44)
#define SHT31_MEAS_HIGH_REPEATABILITY_STRETCH            (0x2C06)
#define SHT31_MEAS_HIGH_REPEATABILITY                    (0x2400)
#define SHT31_READ_STATUS                                (0xF32D)
#define SHT31_SOFT_RESET                                 (0x30A2)

static uint8_t sht31_crc8(uint8_t const *data, size_t len);

int32_t sht31_initialize(struct i2c_m_sync_desc *i2c) {
    int32_t rv;

    rv = i2c_write_command_u16(i2c, SHT31_I2C_ADDRESS, SHT31_SOFT_RESET);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    delay_ms(1);

    return FK_SUCCESS;
}

int32_t sht31_status_get(struct i2c_m_sync_desc *i2c, uint16_t *status) {
    int32_t rv;

    *status = 0;

    rv = i2c_write_command_u16(i2c, SHT31_I2C_ADDRESS, SHT31_READ_STATUS);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    uint8_t buffer[3];
    rv = i2c_read_buffer(i2c, SHT31_I2C_ADDRESS, buffer, sizeof(buffer));
    if (rv != FK_SUCCESS) {
        return rv;
    }

    *status |= buffer[0] << 8;
    *status |= buffer[1];

    return FK_SUCCESS;
}

int32_t sht31_reading_get(struct i2c_m_sync_desc *i2c, sht31_reading_t *reading) {
    int32_t rv;

    reading->temperature = 0;
    reading->humidity = 0;

    rv = i2c_write_command_u16(i2c, SHT31_I2C_ADDRESS, SHT31_MEAS_HIGH_REPEATABILITY_STRETCH);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    uint8_t buffer[6];
    rv = i2c_read_buffer(i2c, SHT31_I2C_ADDRESS, buffer, sizeof(buffer));
    if (rv != FK_SUCCESS) {
        return rv;
    }

    // Verify CRCs of the pair.
    if (buffer[2] != sht31_crc8(&buffer[0], 2)) {
        return FK_ERROR_GENERAL;
    }

    if (buffer[5] != sht31_crc8(&buffer[3], 2)) {
        return FK_ERROR_GENERAL;
    }

    int32_t temperature = (buffer[0] << 8) | (buffer[1]);
    int32_t humidity = (buffer[3] << 8) | (buffer[4]);

    reading->temperature = temperature;
    // temperature = -45 + 175 * (temperature / (2**16 - 1))

    reading->humidity = humidity;
    // humidity = 100 * (humidity / (2**16 -1))

    return FK_SUCCESS;
}

static uint8_t sht31_crc8(uint8_t const *data, size_t len) {
    const uint8_t polynomial = 0x31;

    uint8_t crc = 0xFF;
    for (int32_t j = len; j; --j ) {
        crc ^= *data++;
        for (int32_t i = 8; i; --i ) {
            crc = (crc & 0x80) ? (crc << 1) ^ polynomial : (crc << 1);
        }
    }
    return crc;
}

#endif
