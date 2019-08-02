#include <string.h>

#include <atmel_start_pins.h>

#include <hal_delay.h>

#include "eeprom.h"

int32_t eeprom_write(struct i2c_m_sync_desc *i2c, uint16_t address, uint8_t *data, size_t size) {
    // FK_ASSERT(size <= 32);

    uint8_t buffer[size + sizeof(address)];
    buffer[0] = (address >> 8) & 0xff;
    buffer[1] = (address);
    memcpy(buffer + sizeof(address), data, size);

    i2c_m_sync_enable(i2c);

    struct _i2c_m_msg msg;
    msg.addr   = EEPROM_I2C_ADDRESS;
    msg.flags  = I2C_M_STOP;
    msg.buffer = buffer;
    msg.len    = size + sizeof(address);
    int32_t ret = _i2c_m_sync_transfer(&i2c->device, &msg);

    msg.buffer = NULL;
    msg.len = 0;

    int32_t to = EEPROM_TIMEOUT_WRITE;
    while (to > 0) {
        ret = _i2c_m_sync_transfer(&i2c->device, &msg);
        if (ret == ERR_NONE) {
            break;
        }
        delay_ms(1);
        to--;
    }

    return ret;
}

int32_t eeprom_read(struct i2c_m_sync_desc *i2c, uint16_t address, uint8_t *data, size_t size) {
    struct _i2c_m_msg msg;
    int32_t           ret;

    // FK_ASSERT(size <= 32);

    i2c_m_sync_enable(i2c);

    msg.addr   = EEPROM_I2C_ADDRESS;
    msg.len    = sizeof(uint16_t);
    msg.flags  = 0;
    msg.buffer = (void *)&address;

    ret = _i2c_m_sync_transfer(&i2c->device, &msg);

    if (ret != 0) {
        /* error occurred */
        return ret;
    }

    msg.flags  = I2C_M_STOP | I2C_M_RD;
    msg.buffer = data;
    msg.len    = size;

    ret = _i2c_m_sync_transfer(&i2c->device, &msg);

    if (ret != 0) {
        /* error occurred */
        return ret;
    }

    return ERR_NONE;
}

int32_t eeprom_write_enable_always() {
    gpio_set_pin_direction(PA16, GPIO_DIRECTION_IN);
    return 0;
}

int32_t eeprom_write_enable() {
    gpio_set_pin_direction(PA16, GPIO_DIRECTION_OUT);
    gpio_set_pin_level(PA16, 0);
    return 0;
}

int32_t eeprom_write_disable() {
    gpio_set_pin_level(PA16, 1);
    return 0;
}
