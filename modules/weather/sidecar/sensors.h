#pragma once

#include <atmel_start.h>

int32_t sensors_initialize(struct i2c_m_sync_desc *i2c);

int32_t sht31_initialize(struct i2c_m_sync_desc *i2c);

int32_t sht31_status_get(struct i2c_m_sync_desc *i2c, uint16_t *status);

typedef struct sht31_reading_t {

} sht31_reading_t;

int32_t sht31_reading_get(struct i2c_m_sync_desc *i2c, sht31_reading_t *reading);

int32_t mpl3115a2_initialize(struct i2c_m_sync_desc *i2c);

typedef struct mpl3115a2_reading_t {

} mpl3115a2_reading_t;

int32_t mpl3115a2_reading_get(struct i2c_m_sync_desc *i2c);

int32_t adc_initialize(struct i2c_m_sync_desc *i2c);
