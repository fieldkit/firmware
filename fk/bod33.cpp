#include <os.h>
#include "platform.h"
#include "hal/display.h"

#if defined(__SAMD21__) || defined(__SAMD51__)

extern "C" {

void SUPC_1_Handler(void) {
    SEGGER_RTT_WriteString(0, "supc_1_irq: bod33!\n");
    fk::get_active_display()->off();
    NVIC_SystemReset();
}

}

#endif // defined(__SAMD21__) || defined(__SAMD51__)
