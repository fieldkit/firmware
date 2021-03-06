#include <string.h>

#include <atmel_start_pins.h>

#include <hal_delay.h>

#include "eeprom.h"
#include "crc.h"
#include "board.h"

/**
 * Return the smaller of two values.
 */
#define MIN(a, b)   ((a > b) ? (b) : (a))

void fk_dump_memory(const uint8_t *p, size_t size) {
    SEGGER_RTT_WriteString(0, RTT_CTRL_RESET);
    for (size_t i = (size_t)0; i < size; ++i) {
        SEGGER_RTT_printf(0, "%02x ", p[i]);
        if ((i + 1) % 32 == 0) {
            if (i + 1 < size) {
                SEGGER_RTT_printf(0, "\n");
            }
        }
    }
    SEGGER_RTT_WriteString(0, "\n");
}

int32_t eeprom_write_page(struct i2c_m_sync_desc *i2c, uint16_t address, uint8_t *data, size_t size) {
    FK_ASSERT(size <= EEPROM_PAGE_SIZE);
    FK_ASSERT(address + size <= EEPROM_ADDRESS_END);

    uint8_t buffer[size + sizeof(address)];
    buffer[0] = (address >> 8) & 0xff;
    buffer[1] = (address) & 0xff;
    memcpy(buffer + sizeof(address), data, size);

    i2c_m_sync_enable(i2c);

    struct _i2c_m_msg msg;
    msg.addr   = EEPROM_I2C_ADDRESS;
    msg.flags  = I2C_M_STOP;
    msg.buffer = buffer;
    msg.len    = size + sizeof(address);
    int32_t rv = _i2c_m_sync_transfer(&i2c->device, &msg);
    if (rv != ERR_NONE) {
        return rv;
    }

    msg.buffer = NULL;
    msg.len = 0;

    rv = FK_ERROR_TIMEOUT;

    int32_t to = EEPROM_TIMEOUT_WRITE;
    while (to > 0) {
        rv = _i2c_m_sync_transfer(&i2c->device, &msg);
        if (rv == ERR_NONE) {
            rv = FK_SUCCESS;
            break;
        }
        delay_ms(1);
        to--;
    }

    return rv;
}

int32_t eeprom_read_page(struct i2c_m_sync_desc *i2c, uint16_t address, uint8_t *data, size_t size) {
    FK_ASSERT(size <= EEPROM_PAGE_SIZE);
    FK_ASSERT(address + size <= EEPROM_ADDRESS_END);

    struct _i2c_m_msg msg;
    int32_t           rv;

    uint8_t buffer[sizeof(address)];
    buffer[0] = (address >> 8) & 0xff;
    buffer[1] = (address) & 0xff;

    i2c_m_sync_enable(i2c);

    msg.addr   = EEPROM_I2C_ADDRESS;
    msg.len    = sizeof(buffer);
    msg.flags  = 0;
    msg.buffer = (void *)buffer;
    rv = _i2c_m_sync_transfer(&i2c->device, &msg);
    if (rv != ERR_NONE) {
        return rv;
    }

    msg.flags  = I2C_M_STOP | I2C_M_RD;
    msg.buffer = data;
    msg.len    = size;
    rv = _i2c_m_sync_transfer(&i2c->device, &msg);
    if (rv != ERR_NONE) {
        return rv;
    }

    return FK_SUCCESS;
}

int32_t eeprom_write(struct i2c_m_sync_desc *i2c, uint16_t address, uint8_t *data, size_t size) {
    uint8_t *ptr = data;
    size_t remaining = size;

    while (remaining > 0) {
        size_t to_write = MIN(EEPROM_PAGE_SIZE, remaining);
        int32_t rv = eeprom_write_page(i2c, address, ptr, to_write);
        if (rv != FK_SUCCESS) {
            return rv;
        }

        ptr += to_write;
        remaining -= to_write;
        address += to_write;
    }

    return FK_SUCCESS;
}

int32_t eeprom_read(struct i2c_m_sync_desc *i2c, uint16_t address, uint8_t *data, size_t size) {
    uint8_t *ptr = data;
    size_t remaining = size;

    while (remaining > 0) {
        size_t to_read = MIN(EEPROM_PAGE_SIZE, remaining);
        int32_t rv = eeprom_read_page(i2c, address, ptr, to_read);
        if (rv != FK_SUCCESS) {
            return rv;
        }

        ptr += to_read;
        remaining -= to_read;
        address += to_read;
    }

    return FK_SUCCESS;
}

int32_t eeprom_write_enable_always() {
    gpio_set_pin_direction(PA16, GPIO_DIRECTION_IN);
    return FK_SUCCESS;
}

int32_t eeprom_write_enable() {
    gpio_set_pin_direction(PA16, GPIO_DIRECTION_OUT);
    gpio_set_pin_level(PA16, 0);
    return FK_SUCCESS;
}

int32_t eeprom_write_disable() {
    gpio_set_pin_level(PA16, 1);
    return FK_SUCCESS;
}

int32_t eeprom_lock_test() {
    return gpio_get_pin_level(PA25);
}

int32_t eeprom_region_create(eeprom_region_t *region, struct i2c_m_sync_desc *i2c, uint16_t start, uint16_t end, uint16_t item_size) {
    region->i2c = i2c;
    region->start = start;
    region->end = end;
    region->item_size = item_size;
    region->tail = start;

    return FK_SUCCESS;
}

int32_t eeprom_verify_header(eeprom_region_t *region) {
    int32_t rv;

    ModuleHeader header;
    rv = eeprom_read(region->i2c, 0, (uint8_t *)&header, sizeof(header));
    if (rv != FK_SUCCESS) {
        return FK_ERROR_IO;
    }

    if (!fk_module_header_valid(&header)) {
        fk_dump_memory((uint8_t *)&header, sizeof(header));
        return FK_ERROR_GENERAL;
    }

    return FK_SUCCESS;
}

int32_t eeprom_region_append(eeprom_region_t *region, fk_weather_t *item) {
    int32_t rv;

    if (eeprom_lock_test()) {
        return FK_ERROR_BUSY;
    }

    board_eeprom_i2c_enable();

    // Write this item into memory, we've been given the size already.
    rv = eeprom_write(region->i2c, region->tail, (uint8_t *)item, region->item_size);
    if (rv != FK_SUCCESS) {
        return FK_ERROR_GENERAL;
    }

    // Move to the next item slot and if we've reached the end of the region
    // wrap around to the beginning. Notice we check to see if there's room for
    // another reading, so the following append will work.
    region->tail += region->item_size;
    if (region->tail + region->item_size >= region->end) {
        region->tail = region->start;
    }
    return FK_SUCCESS;
}

int32_t eeprom_region_append_unwritten(eeprom_region_t *region, unwritten_readings_t *ur) {
    fk_weather_t *weather = NULL;
    while (unwritten_readings_peek(ur, &weather) == FK_SUCCESS) {
        int32_t rv;

        #if defined(FK_LOGGING_VERBOSE)
        loginfof("writing 0x%04" PRIx32 " 0x%" PRIx32 " %d %d", region->tail, weather->crc, ur->tail, ur->head);
        #endif

        rv = eeprom_region_append(region, weather);
        if (rv != FK_SUCCESS) {
            board_eeprom_i2c_disable();
            return rv;
        }

        rv = unwritten_readings_pop(ur, NULL);
        if (rv != FK_SUCCESS) {
            board_eeprom_i2c_disable();
            return rv;
        }
    }

    board_eeprom_i2c_disable();

    return FK_SUCCESS;
}

int32_t eeprom_region_seek_beginning(eeprom_region_t *region) {
    region->tail = region->start;

    return FK_SUCCESS;
}

int32_t eeprom_region_seek_end(eeprom_region_t *region, uint32_t *seconds, uint32_t *startup_counter) {
    uint16_t address = region->start;
    while (address + sizeof(fk_weather_t) <= region->end) {
        fk_weather_t reading;

        int32_t rv = eeprom_read(region->i2c, address, (uint8_t *)&reading, sizeof(fk_weather_t));
        if (rv != FK_SUCCESS) {
            return rv;
        }

        uint32_t expected = crc32_checksum(FK_MODULES_CRC_SEED, (uint8_t *)&reading, sizeof(fk_weather_t) - sizeof(uint32_t));
        if (expected != reading.crc) {
            loginfof("found end 0x%04" PRIx32 " (0x%" PRIx32 " != 0x%" PRIx32 ")", address, expected, reading.crc);
            region->tail = address;
            break;
        }

        if (reading.startups > *startup_counter) {
            *startup_counter = reading.startups;
        }

        if (reading.seconds < *seconds) {
            loginfof("found end 0x%04" PRIx32 " (wrap)", address);
            region->tail = address;
            break;
        }

        address += sizeof(fk_weather_t);
        *seconds = reading.seconds;
    }

    loginfof("found end 0x%04" PRIx32, address);

    return FK_SUCCESS;
}

int32_t eeprom_region_append_error(eeprom_region_t *region, uint32_t startups, uint32_t error, uint32_t memory_failures, uint32_t reading_failures) {
    fk_weather_t weather;
    memzero(&weather, sizeof(fk_weather_t));

    weather.error = error;
    weather.startups = startups;
    weather.memory_failures = memory_failures;
    weather.reading_failures = reading_failures;
    weather.crc = fk_weather_sign(&weather);

    int32_t rv = eeprom_region_append(region, &weather);

    board_eeprom_i2c_disable();

    return rv;
}
