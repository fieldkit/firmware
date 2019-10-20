#include "common.h"
#include "debugging.h"
#include "platform.h"
#include "config.h"

#if defined(__SAMD51__)
#include <Arduino.h>
#endif

#include "hal/display.h"
#include "tasks/tasks.h"

#include <SEGGER_RTT.h>

extern "C" {

#if defined(__SAMD51__)

extern void *__real_malloc(size_t size);
extern void __real_free(void *ptr);

void *__wrap_malloc(size_t size) {
    auto ptr = __real_malloc(size);
    alogf(LogLevels::DEBUG, "memory", "[0x%" PRIxPTR "] malloc(%zd)", (uintptr_t)ptr, size);
    return ptr;
}

void __wrap_free(void *ptr) {
    alogf(LogLevels::DEBUG, "memory", "[0x%" PRIxPTR "] free()", (uintptr_t)ptr);
    __real_free(ptr);
}

#endif

#if defined(__SAMD21__) || defined(__SAMD51__)

static void fk_r9_verify() __attribute__((no_instrument_function));

#define FK_EXPECTED_R9        (*(uint32_t *)0x2003fffc)

static void fk_r9_verify() {
    register uint32_t reg_r9 asm("r9");

    if (FK_EXPECTED_R9 != reg_r9) {
        __BKPT(3);
    }
}

#else

static void fk_r9_verify() {
}

#endif

void __cyg_profile_func_enter(void *this_fn, void *call_site) __attribute__((no_instrument_function));

void __cyg_profile_func_exit(void *this_fn, void *call_site) __attribute__((no_instrument_function));

void __cyg_profile_func_enter(void *this_fn, void *call_site) {
    fk_r9_verify();
}

void __cyg_profile_func_exit(void *this_fn, void *call_site) {
    fk_r9_verify();
}

}

namespace fk {

static bool fk_console_attached = false;

bool fk_debugging_initialize() {
    return true;
}

bool fk_debug_is_attached() {
    return false;
}

bool fk_debug_mode() {
    return !os_task_is_running(&scheduler_task);
}

bool fk_debug_get_console_attached() {
    return fk_console_attached;
}

void fk_debug_set_console_attached() {
    if (!fk_console_attached) {
        fk_console_attached = true;
    }
}

static volatile uint32_t fk_debugger_triggered = 0;

void fk_debugger_break() {
    fk_debugger_triggered++;
}

class NoopDebuggerOfLastResort : public DebuggerOfLastResort {
public:
    void message(const char *message) override { }

};

static NoopDebuggerOfLastResort noop;

void DebuggerOfLastResort::message(const char *message) {
    auto display = get_display();
    display->simple({ message });
    fk_delay(500);
}

DebuggerOfLastResort DebuggerOfLastResort::instance_;
DebuggerOfLastResort *DebuggerOfLastResort::selected_{ nullptr };

DebuggerOfLastResort *DebuggerOfLastResort::get() {
    if (selected_ == nullptr) {
        return &noop;
    }
    return selected_;
}

void DebuggerOfLastResort::enable() {
    selected_ = &instance_;
}

void DebuggerOfLastResort::disable() {
    selected_ = &noop;
}

}
