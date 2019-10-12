#pragma once

#include <hal_i2c_m_sync.h>

#include <modules.h>

#include "sidecar.h"
#include "unwritten.h"

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

int32_t eeprom_lock_acquire();
int32_t eeprom_lock_release();

typedef struct eeprom_region_t {
    struct i2c_m_sync_desc *i2c;
    uint16_t start;
    uint16_t end;
    uint16_t item_size;
    uint16_t tail;
} eeprom_region_t;

/**
 *
 */
int32_t eeprom_region_create(eeprom_region_t *region, struct i2c_m_sync_desc *i2c, uint16_t start, uint16_t end, uint16_t item_size);

int32_t eeprom_region_seek_beginning(eeprom_region_t *region);

/**
 *
 */
int32_t eeprom_region_append(eeprom_region_t *region, fk_weather_t *item);

int32_t eeprom_region_append_unwritten(eeprom_region_t *region, unwritten_readings_t *ur);

int32_t eeprom_verify_header(eeprom_region_t *region);

int32_t eeprom_lock_test();
