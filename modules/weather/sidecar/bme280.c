#include "bme280.h"
#include "i2c.h"
#include "board.h"

#define BME280_I2C_ADDRESS                               (0x76)

#define BME280_SAMPLING_NONE                             (0b000)
#define BME280_SAMPLING_X1                               (0b001)
#define BME280_SAMPLING_X2                               (0b010)
#define BME280_SAMPLING_X4                               (0b011)
#define BME280_SAMPLING_X8                               (0b100)
#define BME280_SAMPLING_X16                              (0b101)

#define BME280_MODE_SLEEP                                (0b00)
#define BME280_MODE_FORCED                               (0b01)
#define BME280_MODE_NORMAL                               (0b11)

#define BME280_FILTER_OFF                                (0b000)
#define BME280_FILTER_X2                                 (0b001)
#define BME280_FILTER_X4                                 (0b010)
#define BME280_FILTER_X8                                 (0b011)
#define BME280_FILTER_X16                                (0b100)

#define BME280_STANDBY_MS_0_5                            (0b000)
#define BME280_STANDBY_MS_10                             (0b110)
#define BME280_STANDBY_MS_20                             (0b111)
#define BME280_STANDBY_MS_62_5                           (0b001)
#define BME280_STANDBY_MS_125                            (0b010)
#define BME280_STANDBY_MS_250                            (0b011)
#define BME280_STANDBY_MS_500                            (0b100)
#define BME280_STANDBY_MS_1000                           (0b101)

#define BME280_REGISTER_CHIPID                           (0xD0)
#define BME280_REGISTER_SOFTRESET                        (0xE0)

#define BME280_REGISTER_CONTROLHUMID                     (0xF2)
#define BME280_REGISTER_STATUS                           (0XF3)
#define BME280_REGISTER_CONTROL                          (0xF4)
#define BME280_REGISTER_CONFIG                           (0xF5)
#define BME280_REGISTER_PRESSUREDATA                     (0xF7)
#define BME280_REGISTER_TEMPDATA                         (0xFA)
#define BME280_REGISTER_HUMIDDATA                        (0xFD)

#define BME280_REGISTER_DIG_T1                           (0x88)
#define BME280_REGISTER_DIG_T2                           (0x8A)
#define BME280_REGISTER_DIG_T3                           (0x8C)

#define BME280_REGISTER_DIG_P1                           (0x8E)
#define BME280_REGISTER_DIG_P2                           (0x90)
#define BME280_REGISTER_DIG_P3                           (0x92)
#define BME280_REGISTER_DIG_P4                           (0x94)
#define BME280_REGISTER_DIG_P5                           (0x96)
#define BME280_REGISTER_DIG_P6                           (0x98)
#define BME280_REGISTER_DIG_P7                           (0x9A)
#define BME280_REGISTER_DIG_P8                           (0x9C)
#define BME280_REGISTER_DIG_P9                           (0x9E)

#define BME280_REGISTER_DIG_H1                           (0xA1)
#define BME280_REGISTER_DIG_H2                           (0xE1)
#define BME280_REGISTER_DIG_H3                           (0xE3)
#define BME280_REGISTER_DIG_H4                           (0xE4)
#define BME280_REGISTER_DIG_H5                           (0xE5)
#define BME280_REGISTER_DIG_H6                           (0xE7)

typedef struct {
    uint16_t dig_T1;
    int16_t dig_T2;
    int16_t dig_T3;

    uint16_t dig_P1;
    int16_t dig_P2;
    int16_t dig_P3;
    int16_t dig_P4;
    int16_t dig_P5;
    int16_t dig_P6;
    int16_t dig_P7;
    int16_t dig_P8;
    int16_t dig_P9;

    uint8_t dig_H1;
    int16_t dig_H2;
    uint8_t dig_H3;
    int16_t dig_H4;
    int16_t dig_H5;
    int8_t dig_H6;

    int32_t t_fine;
} bme280_calibration_data;

typedef union config_register {
    uint8_t raw;
    struct {
        unsigned int t_sb : 3;     ///< inactive duration (standby time) in normal mode
        unsigned int filter : 3;   ///< filter settings
        unsigned int none : 1;     ///< unused - don't set
        unsigned int spi3w_en : 1; ///< unused - don't set
    } fields;
} config_register;

typedef union ctrl_measure_register {
    uint8_t raw;
    struct {
        unsigned int osrs_t : 3; ///< temperature oversampling
        unsigned int osrs_p : 3; ///< pressure oversampling
        unsigned int mode : 2;   ///< device mode
    } fields;
} ctrl_measure_register;

typedef union ctrl_humidity_register {
    uint8_t raw;
    struct fields {
        unsigned int none : 5;
        unsigned int osrs_h : 3; ///< pressure oversampling
    } fields;
} ctrl_humidity_register;

static uint8_t bme280_is_reading_calibration(struct i2c_m_sync_desc *i2c);

static int32_t bme280_read_coefficients(struct i2c_m_sync_desc *i2c, bme280_calibration_data *calib);

static int32_t bme280_read_temperature(struct i2c_m_sync_desc *i2c, bme280_calibration_data *calib, bme280_reading_t *reading);

static int32_t bme280_read_pressure(struct i2c_m_sync_desc *i2c, bme280_calibration_data *calib, bme280_reading_t *reading);

static int32_t bme280_read_humidity(struct i2c_m_sync_desc *i2c, bme280_calibration_data *calib, bme280_reading_t *reading);

static bme280_calibration_data bme280_calib;


int32_t bme280_initialize(struct i2c_m_sync_desc *i2c) {
    int32_t rv;
    uint8_t sensor_id = 0;

    rv = i2c_read_u8(i2c, BME280_I2C_ADDRESS, BME280_REGISTER_CHIPID, &sensor_id);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    if (sensor_id != 0x60) {
        return FK_ERROR_GENERAL;
    }

    rv = i2c_write_u8(i2c, BME280_I2C_ADDRESS, BME280_REGISTER_SOFTRESET, 0xB6);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    uint32_t started = board_system_time_get();
    while (bme280_is_reading_calibration(i2c)) {
        delay_ms(10);
        if (board_system_time_get() - started > 1000) {
            return FK_ERROR_TIMEOUT;
        }
    }

    rv = i2c_write_u8(i2c, BME280_I2C_ADDRESS, BME280_REGISTER_CONTROL, BME280_MODE_SLEEP);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    rv = bme280_read_coefficients(i2c, &bme280_calib);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    config_register cfg_reg;
    ctrl_humidity_register hum_reg;
    ctrl_measure_register mea_reg;

    cfg_reg.raw = 0;
    cfg_reg.fields.filter = BME280_FILTER_OFF;
    cfg_reg.fields.t_sb = BME280_STANDBY_MS_0_5;

    mea_reg.raw = 0;
    mea_reg.fields.mode = BME280_MODE_NORMAL;
    mea_reg.fields.osrs_t = BME280_SAMPLING_X16;
    mea_reg.fields.osrs_p = BME280_SAMPLING_X16;

    hum_reg.raw = 0;
    hum_reg.fields.osrs_h = BME280_SAMPLING_X16;

    // You must make sure to also set REGISTER_CONTROL after setting the
    // CONTROLHUMID register, otherwise the values won't be applied (see
    // DS 5.4.3)
    rv = i2c_write_u8(i2c, BME280_I2C_ADDRESS, BME280_REGISTER_CONTROLHUMID, hum_reg.raw);
    rv = i2c_write_u8(i2c, BME280_I2C_ADDRESS, BME280_REGISTER_CONFIG, cfg_reg.raw);
    rv = i2c_write_u8(i2c, BME280_I2C_ADDRESS, BME280_REGISTER_CONTROL, mea_reg.raw);

    if (rv != FK_SUCCESS) {
        return rv;
    }

    return FK_SUCCESS;
}

int32_t bme280_reading_get(struct i2c_m_sync_desc *i2c, bme280_reading_t *reading) {
    int32_t rv;

    reading->temperature = 0;
    reading->humidity = 0;
    reading->pressure = 0;

    rv = bme280_read_temperature(i2c, &bme280_calib, reading);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    rv = bme280_read_pressure(i2c, &bme280_calib, reading);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    rv = bme280_read_humidity(i2c, &bme280_calib, reading);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    return FK_SUCCESS;
}

static uint8_t bme280_is_reading_calibration(struct i2c_m_sync_desc *i2c) {
    uint8_t status;

    int32_t rv = i2c_read_u8(i2c, BME280_I2C_ADDRESS, BME280_REGISTER_STATUS, &status);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    return (status & (1 << 0)) != 0;
}

static int32_t bme280_read_coefficients(struct i2c_m_sync_desc *i2c, bme280_calibration_data *calib) {
    i2c_read_u16(i2c, BME280_I2C_ADDRESS, BME280_REGISTER_DIG_T1, &calib->dig_T1);
    i2c_read_s16(i2c, BME280_I2C_ADDRESS, BME280_REGISTER_DIG_T2, &calib->dig_T2);
    i2c_read_s16(i2c, BME280_I2C_ADDRESS, BME280_REGISTER_DIG_T3, &calib->dig_T3);

    i2c_read_u16(i2c, BME280_I2C_ADDRESS, BME280_REGISTER_DIG_P1, &calib->dig_P1);
    i2c_read_s16(i2c, BME280_I2C_ADDRESS, BME280_REGISTER_DIG_P2, &calib->dig_P2);
    i2c_read_s16(i2c, BME280_I2C_ADDRESS, BME280_REGISTER_DIG_P3, &calib->dig_P3);
    i2c_read_s16(i2c, BME280_I2C_ADDRESS, BME280_REGISTER_DIG_P4, &calib->dig_P4);
    i2c_read_s16(i2c, BME280_I2C_ADDRESS, BME280_REGISTER_DIG_P5, &calib->dig_P5);
    i2c_read_s16(i2c, BME280_I2C_ADDRESS, BME280_REGISTER_DIG_P6, &calib->dig_P6);
    i2c_read_s16(i2c, BME280_I2C_ADDRESS, BME280_REGISTER_DIG_P7, &calib->dig_P7);
    i2c_read_s16(i2c, BME280_I2C_ADDRESS, BME280_REGISTER_DIG_P8, &calib->dig_P8);
    i2c_read_s16(i2c, BME280_I2C_ADDRESS, BME280_REGISTER_DIG_P9, &calib->dig_P9);

    i2c_read_u8(i2c, BME280_I2C_ADDRESS, BME280_REGISTER_DIG_H1, &calib->dig_H1);
    i2c_read_s16(i2c, BME280_I2C_ADDRESS, BME280_REGISTER_DIG_H2, &calib->dig_H2);
    i2c_read_u8(i2c, BME280_I2C_ADDRESS, BME280_REGISTER_DIG_H3, &calib->dig_H3);

    uint8_t val1, val2;
    i2c_read_u8(i2c, BME280_I2C_ADDRESS, BME280_REGISTER_DIG_H4,     &val1);
    i2c_read_u8(i2c, BME280_I2C_ADDRESS, BME280_REGISTER_DIG_H4 + 1, &val2);

    calib->dig_H4 = ((int8_t)val1 << 4) | (val2 & 0xF);

    i2c_read_u8(i2c, BME280_I2C_ADDRESS, BME280_REGISTER_DIG_H5 + 1, &val1);
    i2c_read_u8(i2c, BME280_I2C_ADDRESS, BME280_REGISTER_DIG_H5,     &val2);

    calib->dig_H5 = ((int8_t)val1 << 4) | (val2 >> 4);

    i2c_read_u8(i2c, BME280_I2C_ADDRESS, BME280_REGISTER_DIG_H6, &calib->dig_H6);

    calib->t_fine = 0;

    return FK_SUCCESS;
}

static int32_t bme280_read_temperature(struct i2c_m_sync_desc *i2c, bme280_calibration_data *calib, bme280_reading_t *reading) {
    uint32_t adc_T = 0;

    int32_t rv = i2c_read_u24(i2c, BME280_I2C_ADDRESS, BME280_REGISTER_TEMPDATA, &adc_T);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    if (adc_T == 0x800000) { // Value in case temp was disabled, should never happen.
        return FK_ERROR_GENERAL;
    }

    adc_T >>= 4;

    int32_t var1 = ((((adc_T >> 3) - ((int32_t)calib->dig_T1 << 1))) *
                    ((int32_t)calib->dig_T2)) >> 11;

    int32_t var2 = (((((adc_T >> 4) - ((int32_t)calib->dig_T1)) *
                      ((adc_T >> 4) - ((int32_t)calib->dig_T1))) >> 12) *
                    ((int32_t)calib->dig_T3)) >> 14;

    // calib->t_fine = var1 + var2 + /*calib->t_fine_adjust*/0;

    // reading->temperature = (calib->t_fine * 5 + 128) >> 8;

    // float T = (calib->t_fine * 5 + 128) >> 8;
    // return T / 100;
    return FK_SUCCESS;
}

static int32_t bme280_read_pressure(struct i2c_m_sync_desc *i2c, bme280_calibration_data *calib, bme280_reading_t *reading) {
    uint32_t adc_P = 0;

    int32_t rv = i2c_read_u24(i2c, BME280_I2C_ADDRESS, BME280_REGISTER_PRESSUREDATA, &adc_P);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    if (adc_P == 0x800000) { // Value in case temp was disabled, should never happen.
        return FK_ERROR_GENERAL;
    }

    int64_t var1, var2, p;

    adc_P >>= 4;

    var1 = ((int64_t)calib->t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)calib->dig_P6;
    var2 = var2 + ((var1 * (int64_t)calib->dig_P5) << 17);
    var2 = var2 + (((int64_t)calib->dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)calib->dig_P3) >> 8) + ((var1 * (int64_t)calib->dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)calib->dig_P1) >> 33;

    if (var1 == 0) {
        return 0; // avoid exception caused by division by zero
    }
    p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)calib->dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)calib->dig_P8) * p) >> 19;

    p = ((p + var1 + var2) >> 8) + (((int64_t)calib->dig_P7) << 4);

    // reading->pressure = p;

    // return (float)p / 256;

    return FK_SUCCESS;
}

static int32_t bme280_read_humidity(struct i2c_m_sync_desc *i2c, bme280_calibration_data *calib, bme280_reading_t *reading) {
    uint32_t adc_H = 0;

    int32_t rv = i2c_read_u24(i2c, BME280_I2C_ADDRESS, BME280_REGISTER_HUMIDDATA, &adc_H);
    if (rv != FK_SUCCESS) {
        return rv;
    }

    if (adc_H == 0x800000) { // Value in case temp was disabled, should never happen.
        return FK_ERROR_GENERAL;
    }

    int32_t v_x1_u32r;

    v_x1_u32r = (calib->t_fine - ((int32_t)76800));

    v_x1_u32r = (((((adc_H << 14) - (((int32_t)calib->dig_H4) << 20) -
                    (((int32_t)calib->dig_H5) * v_x1_u32r)) +
                   ((int32_t)16384)) >>
                  15) *
                 (((((((v_x1_u32r * ((int32_t)calib->dig_H6)) >> 10) *
                      (((v_x1_u32r * ((int32_t)calib->dig_H3)) >> 11) +
                       ((int32_t)32768))) >>
                     10) +
                    ((int32_t)2097152)) *
                   ((int32_t)calib->dig_H2) +
                   8192) >>
                  14));

    v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
                               ((int32_t)calib->dig_H1)) >>
                              4));

    v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;
    v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;

    // reading->humidity = (v_x1_u32r >> 12);

    // float h = (v_x1_u32r >> 12);
    // return h / 1024.0;

    return FK_SUCCESS;
}
