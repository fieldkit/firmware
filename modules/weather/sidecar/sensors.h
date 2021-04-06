#pragma once

#include <atmel_start.h>

#include "sht31.h"
#include "mpl3115a2.h"
#include "adc081c.h"
#include "bme280.h"

typedef struct sensors_t {
    int8_t initialized;
} sensors_t;

int32_t sensors_initialize(struct i2c_m_sync_desc *i2c, sensors_t *sensors);

typedef struct counters_reading_t {
    uint8_t wind;
    uint8_t rain;
} counters_reading_t;

int32_t counters_reading_get(struct i2c_m_sync_desc *i2c, counters_reading_t *reading);
