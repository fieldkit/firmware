#include "adc081c.h"
#include "i2c.h"

#define ADC081C_I2C_ADDRESS                              (0x50)
#define ADC081C_REGISTER_READING                         (0x00)
#define ADC081C_REGISTER_ALERT_STATUS                    (0x01)
#define ADC081C_REGISTER_CONFIGURATION                   (0x02)

int32_t adc081c_initialize(struct i2c_m_sync_desc *i2c) {
    int32_t rv;

    uint8_t configuration;
    rv = i2c_read_u8(i2c, ADC081C_I2C_ADDRESS, ADC081C_REGISTER_CONFIGURATION, &configuration);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    uint8_t default_configuration = 0x00;
    if (configuration != default_configuration) {
        rv = i2c_write_u8(i2c, ADC081C_I2C_ADDRESS, ADC081C_REGISTER_CONFIGURATION, default_configuration);
        if (rv != FK_SUCCESS) {
            return rv;
        }
    }

    return FK_SUCCESS;
}

int32_t adc081c_reading_get(struct i2c_m_sync_desc *i2c, adc081c_reading_t *reading) {
    int32_t rv;

    reading->value = 0;

    rv = i2c_read_u16(i2c, ADC081C_I2C_ADDRESS, ADC081C_REGISTER_READING, &reading->value);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    return FK_SUCCESS;
}
