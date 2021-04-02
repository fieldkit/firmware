#pragma once

#include "sidecar.h"

#if defined(FK_ENABLE_SHT31_AND_MPL3115A2)

typedef struct sht31_reading_t {
    uint32_t humidity;
    uint32_t temperature;
} sht31_reading_t;

int32_t sht31_initialize(struct i2c_m_sync_desc *i2c);

int32_t sht31_status_get(struct i2c_m_sync_desc *i2c, uint16_t *status);

int32_t sht31_reading_get(struct i2c_m_sync_desc *i2c, sht31_reading_t *reading);

#endif
