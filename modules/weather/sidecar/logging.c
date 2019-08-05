#include "logging.h"

int32_t loginfo(const char *f, ...) {
    va_list args;
    va_start(args, f);
    SEGGER_RTT_WriteString(0, RTT_CTRL_TEXT_GREEN "weather " RTT_CTRL_TEXT_YELLOW "info " RTT_CTRL_RESET);
    SEGGER_RTT_WriteString(0, f);
    SEGGER_RTT_WriteString(0, RTT_CTRL_RESET "\n");
    va_end(args);
}

int32_t logerror(const char *f, ...) {
    va_list args;
    va_start(args, f);
    SEGGER_RTT_WriteString(0, RTT_CTRL_TEXT_GREEN "weather " RTT_CTRL_TEXT_RED "error ");
    SEGGER_RTT_WriteString(0, f);
    SEGGER_RTT_WriteString(0, RTT_CTRL_RESET "\n");
    va_end(args);
}
