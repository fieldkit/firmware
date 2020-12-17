#include "i2c.h"

int32_t i2c_write_command_u16(struct i2c_m_sync_desc *i2c, uint8_t address, uint16_t command) {
    struct _i2c_m_msg msg;
    int32_t rv;

    uint8_t buffer[] = {
        (command >> 8) & 0xff,
        (command     ) & 0xff
    };

    msg.addr   = address;
    msg.flags  = I2C_M_SEVEN | I2C_M_STOP;
    msg.buffer = (void *)buffer;
    msg.len    = sizeof(buffer);
    rv = _i2c_m_sync_transfer(&i2c->device, &msg);
    if (rv != 0) {
        return rv;
    }

    return FK_SUCCESS;
}

int32_t i2c_write_buffer(struct i2c_m_sync_desc *i2c, uint8_t address, uint8_t *buffer, size_t size) {
    struct _i2c_m_msg msg;
    int32_t rv;

    msg.addr   = address;
    msg.flags  = I2C_M_SEVEN | I2C_M_STOP;
    msg.buffer = (void *)buffer;
    msg.len    = size;
    rv = _i2c_m_sync_transfer(&i2c->device, &msg);
    if (rv != 0) {
        return rv;
    }

    return FK_SUCCESS;
}

int32_t i2c_read_buffer(struct i2c_m_sync_desc *i2c, uint8_t address, uint8_t *buffer, size_t size) {
    struct _i2c_m_msg msg;
    int32_t rv;

    msg.addr   = address;
    msg.flags  = I2C_M_SEVEN | I2C_M_STOP | I2C_M_RD;
    msg.buffer = (void *)buffer;
    msg.len    = size;
    rv = _i2c_m_sync_transfer(&i2c->device, &msg);
    if (rv != 0) {
        return rv;
    }

    return FK_SUCCESS;
}

int32_t i2c_read_u8(struct i2c_m_sync_desc *i2c, uint8_t address, uint8_t reg, uint8_t *value) {
    struct _i2c_m_msg msg;
    int32_t rv;

    msg.addr   = address;
    msg.flags  = I2C_M_SEVEN;
    msg.buffer = (void *)&reg;
    msg.len    = sizeof(uint8_t);
    rv = _i2c_m_sync_transfer(&i2c->device, &msg);
    if (rv != 0) {
        return rv;
    }

    msg.flags  = I2C_M_SEVEN | I2C_M_STOP | I2C_M_RD;
    msg.buffer = (void *)value;
    msg.len    = sizeof(uint8_t);
    rv = _i2c_m_sync_transfer(&i2c->device, &msg);
    if (rv != 0) {
        return rv;
    }

    return FK_SUCCESS;
}

int32_t i2c_command_read_buffer(struct i2c_m_sync_desc *i2c, uint8_t address, uint8_t reg, uint8_t *buffer, size_t size) {
    struct _i2c_m_msg msg;
    int32_t rv;

    msg.addr   = address;
    msg.flags  = I2C_M_SEVEN;
    msg.buffer = (void *)&reg;
    msg.len    = sizeof(uint8_t);
    rv = _i2c_m_sync_transfer(&i2c->device, &msg);
    if (rv != 0) {
        return rv;
    }

    msg.flags  = I2C_M_SEVEN | I2C_M_STOP | I2C_M_RD;
    msg.buffer = (void *)buffer;
    msg.len    = size;
    rv = _i2c_m_sync_transfer(&i2c->device, &msg);
    if (rv != 0) {
        return rv;
    }

    return FK_SUCCESS;
}

int32_t i2c_write_u8(struct i2c_m_sync_desc *i2c, uint8_t address, uint8_t reg, uint8_t value) {
    uint8_t buffer[2] = { reg, value };
    struct _i2c_m_msg msg;
    int32_t rv;

    msg.addr   = address;
    msg.flags  = I2C_M_SEVEN | I2C_M_STOP;
    msg.buffer = (void *)&buffer;
    msg.len    = sizeof(buffer);

    rv = _i2c_m_sync_transfer(&i2c->device, &msg);
    if (rv != 0) {
        return rv;
    }

    return FK_SUCCESS;
}

int32_t i2c_read_u16(struct i2c_m_sync_desc *i2c, uint8_t address, uint8_t reg, uint16_t *value) {
    struct _i2c_m_msg msg;
    int32_t rv;

    msg.addr   = address;
    msg.flags  = I2C_M_SEVEN;
    msg.buffer = (void *)&reg;
    msg.len    = sizeof(uint8_t);
    rv = _i2c_m_sync_transfer(&i2c->device, &msg);
    if (rv != 0) {
        return rv;
    }

    msg.flags  = I2C_M_SEVEN | I2C_M_STOP | I2C_M_RD;
    msg.buffer = (void *)value;
    msg.len    = sizeof(uint16_t);
    rv = _i2c_m_sync_transfer(&i2c->device, &msg);
    if (rv != 0) {
        return rv;
    }

    return FK_SUCCESS;
}

int32_t i2c_read_s16(struct i2c_m_sync_desc *i2c, uint8_t address, uint8_t reg, int16_t *value) {
    uint16_t raw;
    int32_t rv = i2c_read_u16(i2c, address, reg, &raw);
    *value = (int16_t)raw;
    return rv;
}

int32_t i2c_read_u24(struct i2c_m_sync_desc *i2c, uint8_t address, uint8_t reg, uint32_t *value) {
    struct _i2c_m_msg msg;
    int32_t rv;

    msg.addr   = address;
    msg.flags  = I2C_M_SEVEN;
    msg.buffer = (void *)&reg;
    msg.len    = sizeof(uint8_t);
    rv = _i2c_m_sync_transfer(&i2c->device, &msg);
    if (rv != 0) {
        return rv;
    }

    msg.flags  = I2C_M_SEVEN | I2C_M_STOP | I2C_M_RD;
    msg.buffer = (void *)value;
    msg.len    = 3; // 3 bytes * 8 = 24
    rv = _i2c_m_sync_transfer(&i2c->device, &msg);
    if (rv != 0) {
        return rv;
    }

    return FK_SUCCESS;
}
