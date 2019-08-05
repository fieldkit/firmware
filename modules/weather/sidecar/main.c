#include <atmel_start.h>
#include <atmel_start_pins.h>
#include <hal_delay.h>

#include <SEGGER_RTT.h>

#include <modules/modules.h>
#include <weather.h>

#include "sidecar.h"
#include "eeprom.h"
#include "sensors.h"

int32_t ensure_module_header() {
    ModuleHeader expected = {
        .manufacturer = 0x01,
        .kind = 0x01,
        .version = 0x01,
        .reserved = { 0x00, 0x00, 0x00, 0x00 },
        .crc = 0x00,
    };
    ModuleHeader actual;
    int32_t rv;

    rv = eeprom_read(&I2C_0, 0x00, (uint8_t *)&actual, sizeof(actual));
    if (rv != 0) {
        return FK_ERROR_GENERAL;
    }

    // Calculate hash of expected header, which is the entire block minus the 4
    // bytes at the end for the CRC itself.
    expected.crc = fk_module_header_sign(&expected);

    if (memcmp(&actual, &expected, sizeof(ModuleHeader)) == 0) {
        return FK_SUCCESS;
    }

    rv = eeprom_write(&I2C_0, 0x00, (uint8_t *)&expected, sizeof(expected));
    if (rv != ERR_NONE) {
        return FK_ERROR_GENERAL;
    }

    return FK_SUCCESS;
}

fk_weather_config_t fk_weather_config_default = { 60, 60, 60, 0 };

int32_t read_configuration(fk_weather_config_t *config) {
    return FK_SUCCESS;
}

static void error(const char *message) {
    SEGGER_RTT_WriteString(0, "w: error! ");
    SEGGER_RTT_WriteString(0, message);
    SEGGER_RTT_WriteString(0, "\n");
}

static void debug(uint32_t status, sht31_reading_t *reading) {
    SEGGER_RTT_WriteString(0, "w: debug\n");
}

int32_t counters_configure(struct i2c_m_sync_desc *i2c, uint8_t address) {
    int32_t rv;
    uint8_t buffer[] = {
        0x00,  // IODIR (Address)
        0x00,  // IODIR
        0x00,  // IPOL
        0x00,  // GPINTEN
        0x00,  // DEFVAL
        0x00,  // INTCON
        0x00,  // IOCON
        0x00,  // GPPU
        0x00,  // INTF
        0x00,  // INTCAP
        0x00,  // GPIO
    };

    i2c_m_sync_enable(i2c);

    struct _i2c_m_msg msg;
    msg.addr   = address;
    msg.flags  = I2C_M_SEVEN;
    msg.buffer = (void *)&buffer;
    msg.len    = sizeof(buffer);

    rv = _i2c_m_sync_transfer(&i2c->device, &msg);
    if (rv != 0) {
        return rv;
    }

    return FK_SUCCESS;
}

int32_t read_sht31() {
    int32_t rv;

    rv = sht31_initialize(&I2C_1);
    if (rv != FK_SUCCESS) {
        error("sht31 error initializing");
        return rv;
    }

    uint16_t status;
    rv = sht31_status_get(&I2C_1, &status);
    if (rv != FK_SUCCESS) {
        error("sht31 error getting status");
        return rv;
    }

    sht31_reading_t reading;
    rv = sht31_reading_get(&I2C_1, &reading);
    if (rv != FK_SUCCESS) {
        error("sht31 error getting reading");
        return rv;
    }

    SEGGER_RTT_WriteString(0, "w: sht31 success\n");

    return FK_SUCCESS;
}

int32_t read_mpl3115a2() {
    int32_t rv;

    rv = mpl3115a2_initialize(&I2C_1);
    if (rv != FK_SUCCESS) {
        error("mpl3115a2 error initializing");
        return rv;
    }

    SEGGER_RTT_WriteString(0, "w: mpl3115a2 success\n");

    return FK_SUCCESS;
}

static void found(int32_t address) {
    SEGGER_RTT_WriteString(0, "FOUND\n");
}

// ➜  ~ git:(master) ✗ 33 (counter 1)
// ➜  ~ git:(master) ✗ 34 (counter 2)
// ➜  ~ git:(master) ✗ 68 (sht31)
// ➜  ~ git:(master) ✗ 80
// ➜  ~ git:(master) ✗ 96 (mpl3115a2)

__int32_t main() {
    system_init();

    SEGGER_RTT_Init();
    SEGGER_RTT_WriteString(0, "w: initializing...\n");

    delay_driver_init();
    I2C_0_init();
    I2C_1_init();

    SEGGER_RTT_WriteString(0, "w: checking eeprom...\n");

    // Always leave EEPROM ready for writes.
    eeprom_write_enable_always();

    // Check the module header is there and if so, use that. Not sure what to do
    // if this fails, usually will indicate an error with the EEPROM.
    if (ensure_module_header() != FK_SUCCESS) {
        // NOTE: This is bad!
    }

    SEGGER_RTT_WriteString(0, "w: reading configuration...\n");

    // Read configuration, if that fails use the default.
    fk_weather_config_t config;
    if (read_configuration(&config) != FK_SUCCESS) {
        config = fk_weather_config_default;
    }

    int32_t rv;

    SEGGER_RTT_WriteString(0, "w: configure counter 1...\n");

    rv = counters_configure(&I2C_1, 0x20 + 0x1);
    if (rv != FK_SUCCESS) {
        error("counters error initializing");
    }

    SEGGER_RTT_WriteString(0, "w: configure counter 2...\n");

    rv = counters_configure(&I2C_1, 0x20 + 0x2);
    if (rv != FK_SUCCESS) {
        error("counters error initializing");
    }

    SEGGER_RTT_WriteString(0, "w: sensors...\n");

    read_sht31();

    read_mpl3115a2();

    if (adc_initialize(&I2C_1) != FK_SUCCESS) {
        error("adc error initializing");
    }

    SEGGER_RTT_WriteString(0, "w: ready!\n");

    while (true) {
        delay_ms(1000);
    }

    return 0;
}
