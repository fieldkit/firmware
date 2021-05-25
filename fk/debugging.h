#pragma once

namespace fk {

bool fk_debugging_initialize() ;

void fk_debug_mode_configure(const char *type);

const char *fk_debug_mode();

bool fk_debug_is_attached();

bool fk_debug_get_console_attached();

bool fk_debug_override_schedules();

void fk_debug_set_console_attached();

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
