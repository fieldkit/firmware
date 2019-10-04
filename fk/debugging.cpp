#include "common.h"
#include "debugging.h"
#include "platform.h"
#include "config.h"

#if defined(__SAMD51__)
#include <Arduino.h>
#endif

#include "hal/display.h"

#include <SEGGER_RTT.h>

namespace fk {

static bool console_attached = false;

bool fk_debug_is_attached() {
    return false;
}

bool fk_debug_get_console_attached() {
    return console_attached;
}

void fk_debug_set_console_attached() {
    if (!console_attached) {
        console_attached = true;
    }
}

volatile uint32_t fk_debugger_triggered = 0;

void fk_debugger() {
    fk_debugger_triggered++;
}

class NoopDebuggerOfLastResort : public DebuggerOfLastResort {
public:
    void message(const char *message) override { }

};

static NoopDebuggerOfLastResort noop;

void DebuggerOfLastResort::message(const char *message) {
    SimpleScreen ss{ message };
    auto display = get_display();
    display->simple(ss);
    fk_delay(500);
}

DebuggerOfLastResort DebuggerOfLastResort::instance_;
DebuggerOfLastResort *DebuggerOfLastResort::selected_;

DebuggerOfLastResort *DebuggerOfLastResort::get() {
    return selected_;
}

void DebuggerOfLastResort::enable() {
    selected_ = &instance_;
}

void DebuggerOfLastResort::disable() {
    selected_ = &noop;
}

}
