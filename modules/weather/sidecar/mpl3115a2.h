#pragma once

#include "sidecar.h"

typedef struct mpl3115a2_reading_t {
    uint32_t pressure;
    uint32_t temperature;
} mpl3115a2_reading_t;

int32_t mpl3115a2_initialize(struct i2c_m_sync_desc *i2c);

int32_t mpl3115a2_reading_get(struct i2c_m_sync_desc *i2c, mpl3115a2_reading_t *reading);