#include <atmel_start.h>
#include <atmel_start_pins.h>
#include <hal_delay.h>
#include <string.h>

#include <modules/modules.h>

#include "eeprom.h"

void passed() {
    delay_ms(100);
}

void failed() {
    delay_ms(100);
}

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
        return 0;
    }

    // Calculate hash of expected header, which is the entire block minus the 4
    // bytes at the end for the CRC itself.
    expected.crc = fk_module_header_sign(&expected);

    if (memcmp(&actual, &expected, sizeof(ModuleHeader)) == 0) {
        return 1;
    }

    rv = eeprom_write(&I2C_0, 0x00, (uint8_t *)&expected, sizeof(expected));
    if (rv != ERR_NONE) {
        return 0;
    }

    return 1;
}

__int32_t main() {
    system_init();
    delay_driver_init();

    I2C_0_init();
    I2C_1_init();

    if (!ensure_module_header()) {
        failed();
    }

    while (true) {
        delay_ms(500);
    }

    return 0;
}
