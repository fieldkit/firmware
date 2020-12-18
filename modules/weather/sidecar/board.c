#include <time.h>

#include "board.h"
#include "logging.h"

static volatile uint32_t timer_system_time_ticks = 0;

static struct timer_task timer_system_time;

static void timer_system_time_cb(struct timer_task const *const timer_task) {
    timer_system_time_ticks++;
}

uint32_t board_system_time_get() {
    return timer_system_time_ticks;
}

int32_t board_initialize() {
    system_initialize();

    SEGGER_RTT_Init();
    SEGGER_RTT_WriteString(0, "\n\n");

    delay_driver_initialize();
    EXTERNAL_IRQ_0_initialize();
    TIMER_0_initialize();
    I2C_1_initialize();
    WDT_0_initialize();
    CALENDAR_0_initialize();

	calendar_enable(&CALENDAR_0);

    FK_ASSERT(board_timer_setup(&timer_system_time, 1, timer_system_time_cb) == FK_SUCCESS);

    return FK_SUCCESS;
}

int32_t board_timer_setup(struct timer_task *const timer_task, uint32_t interval, timer_cb_t cb) {
    timer_task->interval = interval;
    timer_task->cb = cb;
    timer_task->mode = TIMER_TASK_REPEAT;
    timer_add_task(&TIMER_0, timer_task);
    timer_start(&TIMER_0);

    return FK_SUCCESS;
}

static int32_t eeprom_i2c_enabled = false;

int32_t board_eeprom_i2c_enable() {
    if (!eeprom_i2c_enabled) {
        I2C_0_master_initialize();
        i2c_m_sync_enable(&I2C_0_m);
        eeprom_i2c_enabled = true;
    }

    return FK_SUCCESS;
}

int32_t board_eeprom_i2c_disable() {
    if (eeprom_i2c_enabled) {
        i2c_m_sync_disable(&I2C_0_m);
        eeprom_i2c_enabled = false;
    }
    return FK_SUCCESS;
}

static int32_t sensors_i2c_enabled = false;

int32_t board_sensors_i2c_enable() {
    if (!sensors_i2c_enabled) {
        I2C_1_initialize();
        i2c_m_sync_enable(&I2C_1);
        sensors_i2c_enabled = true;
    }

    return FK_SUCCESS;
}

int32_t board_sensors_i2c_disable() {
    if (sensors_i2c_enabled) {
        i2c_m_sync_disable(&I2C_1);
        sensors_i2c_enabled = false;
    }
    return FK_SUCCESS;
}

static struct io_descriptor *i2c_subordinate_io = NULL;
static board_register_map_t *i2c_regmap = NULL;

static void i2c_0_rx_complete(const struct i2c_s_async_descriptor *const descr) {
}

static void i2c_0_rx_stop(const struct i2c_s_async_descriptor *const descr) {
    uint8_t command;
    io_read(i2c_subordinate_io, &command, 1);

    switch (command) {
    case FK_WEATHER_I2C_COMMAND_READ: {
        if (i2c_regmap->before_read != NULL) {
            i2c_regmap->before_read(i2c_regmap->registers);
        }

        // SEGGER_RTT_WriteString(0, "R");

        break;
    }
    case FK_WEATHER_I2C_COMMAND_CONFIG: {
        int32_t bytes = i2c_s_async_get_bytes_received(descr);
        if (bytes != sizeof(struct fkw_calendar_date_time)) {
            break;
        }

        // SEGGER_RTT_WriteString(0, "C");

        // This structures is identical to the
        // fkw_calendar_date_time used on the other side.
        struct calendar_date_time clock;
        io_read(i2c_subordinate_io, (uint8_t *)&clock, sizeof(clock));

        calendar_set_date(&CALENDAR_0, &clock.date);
        calendar_set_time(&CALENDAR_0, &clock.time);

        break;
    }
    }

    // We zero our position every time.
    i2c_regmap->position = 0;
}

static void i2c_0_tx_pending(const struct i2c_s_async_descriptor *const descr) {
	struct _i2c_s_async_device *device = (struct _i2c_s_async_device *)&descr->device;

    _i2c_s_async_write_byte(device, i2c_regmap->registers[i2c_regmap->position]);

    i2c_regmap->position = (i2c_regmap->position + 1) % i2c_regmap->size;
}

static void i2c_0_tx_complete(const struct i2c_s_async_descriptor *const descr) {
}

int32_t board_subordinate_initialize(board_register_map_t *regmap) {
    i2c_regmap = regmap;
    i2c_regmap->position = 0;

    I2C_0_async_subordinate_initialize();
    i2c_s_async_get_io_descriptor(&I2C_0_s, &i2c_subordinate_io);
    i2c_s_async_register_callback(&I2C_0_s, I2C_S_RX_COMPLETE, i2c_0_rx_complete);
    i2c_s_async_register_callback(&I2C_0_s, I2C_S_TX_COMPLETE, i2c_0_tx_complete);
    i2c_s_async_register_callback(&I2C_0_s, I2C_S_TX_PENDING, i2c_0_tx_pending);
    i2c_s_async_register_callback(&I2C_0_s, I2C_S_RX_STOP, i2c_0_rx_stop);
    i2c_s_async_set_addr(&I2C_0_s, 0x42);
    i2c_s_async_enable(&I2C_0_s);

    return FK_SUCCESS;
}
