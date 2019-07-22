#include <atmel_start.h>
#include <atmel_start_pins.h>
#include <hal_delay.h>
#include <hal_gpio.h>

#define EEPROM_ADDRESS                      (0x50)

static void passed() {
    delay_ms(100);
}

static void failed() {
    delay_ms(100);
}

__int32_t main() {
    system_init();
    I2C_0_init();
    delay_driver_init();

    gpio_set_pin_direction(PA16, GPIO_DIRECTION_OUT);

    struct io_descriptor *I2C_0_io;

    i2c_m_sync_get_io_descriptor(&I2C_0, &I2C_0_io);
    i2c_m_sync_enable(&I2C_0);
    i2c_m_sync_set_slaveaddr(&I2C_0, EEPROM_ADDRESS, I2C_M_SEVEN);

    while (true) {
        {
            gpio_set_pin_level(PA16, false);
            uint8_t writing[1] = { 0xcc };
            int32_t rv = i2c_m_sync_cmd_write(&I2C_0, 0x00, writing, sizeof(writing));
            if (rv != 0) {
                failed();
            }
            gpio_set_pin_level(PA16, true);
        }

        {
            uint8_t reading[1] = { 0x00 };
            int32_t rv = i2c_m_sync_cmd_read(&I2C_0, 0x00, reading, sizeof(reading));
            if (rv != 0) {
                failed();
            }

            if (reading[0] != 0xcc) {
                failed();
            }
            else {
                passed();
            }
        }

        delay_ms(500);
    }

    volatile uint32_t i = 0;
    while (1) {
        i++;
    }

    return 0;
}
