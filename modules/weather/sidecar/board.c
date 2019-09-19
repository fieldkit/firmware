#include "board.h"
#include "logging.h"

int32_t board_initialize() {
    system_init();

    SEGGER_RTT_Init();
    SEGGER_RTT_WriteString(0, "\n\n");

    delay_driver_init();
    TIMER_0_init();
    I2C_0_init();
    I2C_1_init();
    WDT_0_init();

    i2c_m_sync_enable(&I2C_0);
    i2c_m_sync_enable(&I2C_1);

    return FK_SUCCESS;
}

int32_t board_timer_setup(struct timer_task *const timer_task, uint32_t interval, timer_cb_t cb) {
    timer_task->interval = interval;
    timer_task->cb = cb;
    timer_task->mode = TIMER_TASK_REPEAT;
    timer_add_task(&TIMER_0, timer_task);
    timer_start(&TIMER_0);
}

int32_t fk_assert(const char *message, const char *file, int32_t line) {
    volatile int32_t i = 0;
    while (1) {
        i++;
    }

    return 0;
}
