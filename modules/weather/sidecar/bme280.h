#pragma once

#include "sidecar.h"

typedef struct bme280_reading_t {
    uint32_t humidity;
    uint32_t temperature;
    uint32_t pressure;
} bme280_reading_t;

int32_t bme280_initialize(struct i2c_m_sync_desc *i2c);

int32_t bme280_reading_get(struct i2c_m_sync_desc *i2c, bme280_reading_t *reading);
