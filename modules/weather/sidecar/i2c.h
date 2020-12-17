#pragma once

#include "sidecar.h"

int32_t i2c_write_command_u16(struct i2c_m_sync_desc *i2c, uint8_t address, uint16_t command);

int32_t i2c_write_buffer(struct i2c_m_sync_desc *i2c, uint8_t address, uint8_t *buffer, size_t size);

int32_t i2c_read_buffer(struct i2c_m_sync_desc *i2c, uint8_t address, uint8_t *buffer, size_t size);

int32_t i2c_read_u8(struct i2c_m_sync_desc *i2c, uint8_t address, uint8_t reg, uint8_t *value);

int32_t i2c_command_read_buffer(struct i2c_m_sync_desc *i2c, uint8_t address, uint8_t reg, uint8_t *buffer, size_t size);

int32_t i2c_write_u8(struct i2c_m_sync_desc *i2c, uint8_t address, uint8_t reg, uint8_t value);

int32_t i2c_read_u16(struct i2c_m_sync_desc *i2c, uint8_t address, uint8_t reg, uint16_t *value);

int32_t i2c_read_s16(struct i2c_m_sync_desc *i2c, uint8_t address, uint8_t reg, int16_t *value);

int32_t i2c_read_u24(struct i2c_m_sync_desc *i2c, uint8_t address, uint8_t reg, uint32_t *value);
