#pragma once

#include <hal_i2c_m_sync.h>

#include "sidecar.h"

/**
 *
 */
#define EEPROM_I2C_ADDRESS                (0x50)

/**
 *
 */
#define EEPROM_TIMEOUT_WRITE              (10)

/**
 * Size in bytes of an individual page of EEPROM. This is typically the
 * maximum granularity of writes.
 */
#define EEPROM_PAGE_SIZE                  (0x20)

#define EEPROM_ADDRESS_HEADER             (0x00)
#define EEPROM_ADDRESS_CONFIG             (EEPROM_PAGE_SIZE * 1)
#define EEPROM_ADDRESS_READINGS           (EEPROM_PAGE_SIZE * 2)

/**
 * Last address in our EEPROM, effectively the size of the EEPROM. All our
 * modules use the same chip with 64k.
 */
#define EEPROM_ADDRESS_END                (64 * 1024)

/**
 *
 */
int32_t eeprom_write_page(struct i2c_m_sync_desc *i2c, uint16_t address, uint8_t *data, size_t size);

/**
 *
 */
int32_t eeprom_read_page(struct i2c_m_sync_desc *i2c, uint16_t address, uint8_t *data, size_t size);

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

typedef struct eeprom_region_t {
    struct i2c_m_sync_desc *i2c;
    uint32_t start;
    uint32_t end;
    uint16_t item_size;
    uint32_t tail;
} eeprom_region_t;

/**
 *
 */
int32_t eeprom_region_create(eeprom_region_t *region, struct i2c_m_sync_desc *i2c, uint32_t start, uint32_t end, uint16_t item_size);

/**
 *
 */
int32_t eeprom_region_append(eeprom_region_t *region, void *item);

/**
 *
 */
int32_t eeprom_region_find(eeprom_region_t *region);
