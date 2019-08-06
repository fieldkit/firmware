#pragma once

#include <atmel_start.h>

#include "sht31.h"
#include "mpl3115a2.h"
#include "adc081c.h"

int32_t sensors_initialize(struct i2c_m_sync_desc *i2c);
