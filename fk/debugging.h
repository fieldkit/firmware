#pragma once

namespace fk {

bool fk_debugging_initialize() ;

void fk_debug_mode_configure(const char *type);

const char *fk_debug_mode();

bool fk_debug_is_attached();

bool fk_debug_get_console_attached();

bool fk_debug_override_schedules();

void fk_debug_set_console_attached();

#define FK_DEBUG_PREFIX "~"

#define FK_DEBUG_LOG(f, ...)                         \
    if (fk_debug_get_console_attached()) {           \
        SEGGER_RTT_LOCK();                           \
        SEGGER_RTT_WriteString(0, FK_DEBUG_PREFIX);  \
        SEGGER_RTT_printf(0, f, ##__VA_ARGS__);      \
        SEGGER_RTT_UNLOCK();                         \
    }

class DebuggerOfLastResort {
private:
    static DebuggerOfLastResort instance_;
    static DebuggerOfLastResort *selected_;

public:
    virtual void message(const char *message);

public:
    static DebuggerOfLastResort *get();
    static void enable();
    static void disable();
};

void fk_debugger_break();

}
