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
