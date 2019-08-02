#pragma once

#include <hal_i2c_m_sync.h>

/**
 *
 */
#define EEPROM_I2C_ADDRESS                (0x50)

/**
 *
 */
#define EEPROM_TIMEOUT_WRITE              (10)

/**
 *
 */
#define EEPROM_ADDRESS_HEADER             (0x00)
#define EEPROM_ADDRESS_CONFIG             (0x20)
#define EEPROM_ADDRESS_READINGS           (0x40)

/**
 *
 */
int32_t eeprom_write(struct i2c_m_sync_desc *i2c, uint16_t address, uint8_t *data, size_t size);

/**
 *
 */
int32_t eeprom_read(struct i2c_m_sync_desc *i2c, uint16_t address, uint8_t *data, size_t size);

/**
 *
 */
int32_t eeprom_write_enable_always();

/**
 *
 */
int32_t eeprom_write_enable();


/**
 *
 */
int32_t eeprom_write_disable();
