#include <SEGGER_RTT.h>

#include "common.h"
#include "debugging.h"
#include "platform.h"
#include "config.h"
#include "hal/display.h"
#include "tasks/tasks.h"

#if defined(__SAMD51__)
#include <Arduino.h>
#endif

extern "C" {

#if defined(__SAMD51__)

extern void *__real_malloc(size_t size);
extern void __real_free(void *ptr);

void *__wrap_malloc(size_t size) {
    auto ptr = __real_malloc(size);
    alogf(LogLevels::VERBOSE, "memory", "[0x%" PRIxPTR "] malloc(%zd)", (uintptr_t)ptr, size);
    return ptr;
}

void __wrap_free(void *ptr) {
    alogf(LogLevels::VERBOSE, "memory", "[0x%" PRIxPTR "] free()", (uintptr_t)ptr);
    __real_free(ptr);
}

#endif

void __cyg_profile_func_enter(void *this_fn, void *call_site) __attribute__((no_instrument_function));

void __cyg_profile_func_exit(void *this_fn, void *call_site) __attribute__((no_instrument_function));

static int32_t dbg_stack_paint(uint32_t *stack, size_t size) {
    for (size_t i = 0u; i < size; ++i) {
        *stack = OSH_STACK_MAGIC_WORD;
        stack++;
    }
    return 0;
}

int32_t dbg_stack_highwater(uint32_t *stack, size_t size) {
    for (size_t i = 0u; i < size; ++i) {
        if (*stack != OSH_STACK_MAGIC_WORD) {
            return i;
        }
        stack++;
    }
    return size;
}

void __cyg_profile_func_enter(void *this_fn, void *call_site) {
    os_task_t *self = os_task_self();
    if (self == nullptr) {
        return;
    }
    uint32_t *stack = (uint32_t *)self->stack;
    uint32_t size_dwords = self->stack_size / sizeof(uint32_t);
    uint32_t stack_check = 0;
    uint32_t *sp = &stack_check; // Is clobbered
    if (sp >= stack && sp < stack + size_dwords) {
        uint32_t depth = (sp - stack);
        dbg_stack_paint(stack, depth);
        /*
        if (depth > 6000) {
            SEGGER_RTT_printf(0, "hello %" PRIu32 "\n", depth);
            if (osi_in_task()) {
                __BKPT(3);
            }
        }
        */
    }

    (void)self;
    (void)sp;
}

void __cyg_profile_func_exit(void *this_fn, void *call_site) {
    os_task_t *self = os_task_self();
    if (self == nullptr) {
        return;
    }
    uint32_t *stack = (uint32_t *)self->stack;
    uint32_t size_dwords = self->stack_size / sizeof(uint32_t);
    dbg_stack_highwater(stack, size_dwords);
}

}

namespace fk {

static bool fk_console_attached = false;
static const char *debug_mode_type = nullptr;

bool fk_debugging_initialize() {
    return true;
}

bool fk_debug_is_attached() {
    return false;
}

void fk_debug_mode_configure(const char *type) {
    debug_mode_type = type;
}

const char *fk_debug_mode() {
    if (os_task_is_running(&scheduler_task)) {
        return nullptr;
    }
    return debug_mode_type;
}

bool fk_debug_get_console_attached() {
    return fk_console_attached;
}

bool fk_debug_override_schedules() {
    return false;
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
    display->simple(SimpleScreen{ message });
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
