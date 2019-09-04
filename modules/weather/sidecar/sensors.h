#pragma once

#include <atmel_start.h>

#include "sht31.h"
#include "mpl3115a2.h"
#include "adc081c.h"

int32_t sensors_initialize(struct i2c_m_sync_desc *i2c);

typedef struct counters_reading_t {
    uint8_t wind;
    uint8_t rain;
} counters_reading_t;

int32_t counters_reading_get(struct i2c_m_sync_desc *i2c, counters_reading_t *reading);

int32_t counters_test(struct i2c_m_sync_desc *i2c);
