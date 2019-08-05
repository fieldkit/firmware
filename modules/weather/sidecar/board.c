#include "board.h"
#include "logging.h"

int32_t board_initialize() {
    system_init();

    SEGGER_RTT_Init();
    SEGGER_RTT_WriteString(0, "\n\n");

    loginfo("initializing...");

    delay_driver_init();
    I2C_0_init();
    I2C_1_init();

    return FK_SUCCESS;
}
