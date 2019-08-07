#include "board.h"
#include "logging.h"

int32_t board_initialize() {
    system_init();

    SEGGER_RTT_Init();
    SEGGER_RTT_WriteString(0, "\n\n");

    loginfo("initializing...");

    delay_driver_init();
    TIMER_0_init();
    I2C_0_init();
    I2C_1_init();

    i2c_m_sync_enable(&I2C_0);
    i2c_m_sync_enable(&I2C_1);

    return FK_SUCCESS;
}
