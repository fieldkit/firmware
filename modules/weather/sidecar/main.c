#include <atmel_start.h>
#include <atmel_start_pins.h>
#include <hal_delay.h>
#include <string.h>

#include "eeprom.h"

void passed() {
    delay_ms(100);
}

void failed() {
    delay_ms(100);
}

__int32_t main() {
    system_init();
    delay_driver_init();

    I2C_0_init();
    I2C_1_init();

    uint8_t counter = 0;

    while (true) {
        {
            uint8_t writing[3] = { counter, 0xbb, 0xcc };
            int32_t rv = eeprom_write(&I2C_0, 0x00, writing, sizeof(writing));
            if (rv != ERR_NONE) {
                failed();
            }
        }

        {
            uint8_t reading[3];
            int32_t rv = eeprom_read(&I2C_0, 0x00, reading, sizeof(reading));
            if (rv != 0) {
                failed();
            }
            if (reading[0] == counter) {
                passed();
            }
        }

        delay_ms(500);

        counter++;
    }

    volatile uint32_t i = 0;
    while (1) {
        i++;
    }

    return 0;
}
