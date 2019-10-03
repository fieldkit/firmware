#include <loading.h>
#include <os.h>

#include "logging.h"
#include "platform.h"
#include "config.h"

namespace fk {

#if defined(__SAMD51__)

static size_t log_hook(LogMessage const *m, const char *fstring, va_list args, void *arg) {
    return 0;
}

static size_t write_log(LogMessage const *m, const char *fstring, va_list args) {
    const char *task = os_task_name();
    if (task == nullptr) {
        task = "startup";
    }

    const char *f;
    if ((LogLevels)m->level == LogLevels::ERROR) {
        f = RTT_CTRL_TEXT_GREEN "%08" PRIu32 RTT_CTRL_TEXT_CYAN " %-10s " RTT_CTRL_TEXT_RED "%-7s %s: ";
    }
    else if ((LogLevels)m->level == LogLevels::WARN) {
        f = RTT_CTRL_TEXT_GREEN "%08" PRIu32 RTT_CTRL_TEXT_CYAN " %-10s " RTT_CTRL_TEXT_MAGENTA "%-7s %s: ";
    }
    else {
        f = RTT_CTRL_TEXT_GREEN "%08" PRIu32 RTT_CTRL_TEXT_CYAN " %-10s " RTT_CTRL_TEXT_YELLOW "%-7s %s" RTT_CTRL_RESET ": ";
    }

    SEGGER_RTT_LOCK();

    auto level = alog_get_log_level((LogLevels)m->level);
    auto length = 0;

    length += SEGGER_RTT_printf(0, f, m->uptime, task, level, m->facility);
    length += SEGGER_RTT_vprintf(0, fstring, &args);
    length += SEGGER_RTT_WriteString(0, RTT_CTRL_RESET "\n");

    log_hook(m, fstring, args, nullptr);

    SEGGER_RTT_UNLOCK();

    return true;
}

bool fk_logging_initialize() {
    log_configure_writer(write_log);
    log_configure_level(LogLevels::DEBUG);

    auto waiting_until = fk_uptime() + OneSecondMs;

    while (fk_uptime() < waiting_until) {
        if (SEGGER_RTT_HasData(0) || !SEGGER_RTT_HasDataUp(0)) {
            fk_debug_set_console_attached();
            alogf(LogLevels::INFO, "debug", "debugger detected"); \
            break;
        }
    }

    return true;
}

#endif

}
