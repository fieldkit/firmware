#include <loading.h>
#include <os.h>
#include <tiny_printf.h>

#include "logging.h"
#include "platform.h"
#include "config.h"
#include "circular_buffer.h"
#include "hal/sd_card.h"

namespace fk {

#if defined(__SAMD51__)

static bool logs_rtt_enabled = true;
static char logs_buffer[BUFFER_SIZE_UP];
static bool logs_flushing{ false };
static circular_buffer<char> logs{ logs_buffer, BUFFER_SIZE_UP };

static void write_circular_buffer(char c, void *arg) {
    if (logs.full()) {
        logs_flushing = true;
        get_sd_card()->append_logs(logs);
        logs.clear();
        logs_flushing = false;
    }
    if (c != 0) {
        logs.append(c);
    }
}

size_t write_log(LogMessage const *m, const char *fstring, va_list args) {
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

    auto level = alog_get_log_level((LogLevels)m->level);

    SEGGER_RTT_LOCK();

    if (logs_rtt_enabled) {
        SEGGER_RTT_printf(0, f, m->uptime, task, level, m->facility);
        SEGGER_RTT_vprintf(0, fstring, &args);
        SEGGER_RTT_WriteString(0, RTT_CTRL_RESET "\n");
    }

    if (!logs_flushing) {
        tiny_fctprintf(write_circular_buffer, nullptr, f, m->uptime, task, level, m->facility);
        tiny_vfctprintf(write_circular_buffer, nullptr, fstring, args);
        tiny_fctprintf(write_circular_buffer, nullptr, "\n");
    }

    SEGGER_RTT_UNLOCK();

    return true;
}

void task_logging_hook(os_task_t *task, os_task_status previous_status) {
    auto was_alive = os_task_status_is_alive(previous_status);
    auto is_alive = os_task_status_is_alive(task->status);

    if (was_alive != is_alive) {
        if (is_alive) {
            alogf(LogLevels::INFO, task->name, "alive");
        }
        else {
            alogf(LogLevels::INFO, task->name, "dead");
        }
    }
}

bool fk_logging_initialize() {
    log_configure_writer(write_log);
    log_configure_level(LogLevels::DEBUG);

    OS_CHECK(os_configure_hook(task_logging_hook));

    auto has_rtt_reader = false;
    auto waiting_until = fk_uptime() + OneSecondMs;
    while (fk_uptime() < waiting_until) {
        if (SEGGER_RTT_HasData(0) || !SEGGER_RTT_HasDataUp(0)) {
            fk_debug_set_console_attached();

            SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);

            alogf(LogLevels::INFO, "debug", "debugger detected");

            has_rtt_reader = true;

            break;
        }
    }

    logs_rtt_enabled = has_rtt_reader;

    return true;
}

#endif

}