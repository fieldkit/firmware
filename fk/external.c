#include <loading.h>
#include <SEGGER_RTT.h>

uint32_t fkb_external_printf(const char *str, ...) {
    va_list args;
    va_start(args, str);
    uint32_t r = (uint32_t)SEGGER_RTT_vprintf(0, str, &args);
    va_end(args);
    return r;
}

uint32_t fkb_external_println(const char *str, ...) {
    va_list args;
    va_start(args, str);
    uint32_t r = (uint32_t)SEGGER_RTT_vprintf(0, str, &args);
    SEGGER_RTT_WriteString(0, "\n");
    va_end(args);
    return r;
}
