#pragma once

#include "sidecar.h"

typedef struct adc081c_reading_t {
    uint16_t value;
} adc081c_reading_t;

int32_t adc081c_initialize(struct i2c_m_sync_desc *i2c);

int32_t adc081c_reading_get(struct i2c_m_sync_desc *i2c, adc081c_reading_t *reading);
