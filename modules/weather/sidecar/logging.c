#include "logging.h"

#if defined(FK_LOGGING)

int32_t loginfo(const char *f) {
    SEGGER_RTT_WriteString(0, RTT_CTRL_TEXT_GREEN "weather " RTT_CTRL_TEXT_YELLOW "info " RTT_CTRL_RESET);
    SEGGER_RTT_WriteString(0, f);
    SEGGER_RTT_WriteString(0, RTT_CTRL_RESET "\n");
    return 0;
}

int32_t loginfof(const char *f, ...) {
    va_list args;
    va_start(args, f);
    SEGGER_RTT_WriteString(0, RTT_CTRL_TEXT_GREEN "weather " RTT_CTRL_TEXT_YELLOW "info " RTT_CTRL_RESET);
    SEGGER_RTT_vprintf(0, f, &args);
    SEGGER_RTT_WriteString(0, RTT_CTRL_RESET "\n");
    va_end(args);
    return 0;
}

int32_t logerror(const char *f) {
    SEGGER_RTT_WriteString(0, RTT_CTRL_TEXT_GREEN "weather " RTT_CTRL_TEXT_RED "error ");
    SEGGER_RTT_WriteString(0, f);
    SEGGER_RTT_WriteString(0, RTT_CTRL_RESET "\n");
    return 0;
}

int32_t logerrorf(const char *f, ...) {
    va_list args;
    va_start(args, f);
    SEGGER_RTT_WriteString(0, RTT_CTRL_TEXT_GREEN "weather " RTT_CTRL_TEXT_RED "error ");
    SEGGER_RTT_vprintf(0, f, &args);
    SEGGER_RTT_WriteString(0, RTT_CTRL_RESET "\n");
    va_end(args);
    return 0;
}

#else

int32_t loginfo(const char *f) {
    return 0;
}

int32_t loginfof(const char *f, ...) {
    return 0;
}

int32_t logerror(const char *f) {
    return 0;
}

int32_t logerrorf(const char *f, ...) {
    return 0;
}

#endif
