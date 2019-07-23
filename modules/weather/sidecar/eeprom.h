#pragma once

#include <hal_i2c_m_sync.h>

/**
 *
 */
#define EEPROM_ADDRESS                    (0x50)

/**
 *
 */
#define EEPROM_TIMEOUT_WRITE              (10)

/**
 *
 */
int32_t eeprom_write(struct i2c_m_sync_desc *i2c, uint16_t address, uint8_t *data, size_t size);

/**
 *
 */
int32_t eeprom_read(struct i2c_m_sync_desc *i2c, uint16_t address, uint8_t *data, size_t size);
