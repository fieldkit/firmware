#include <loading.h>
#include <os.h>
#include <tiny_printf.h>

#include "logging.h"
#include "platform.h"
#include "config.h"
#include "circular_buffer.h"
#include "hal/sd_card.h"

namespace fk {

static static_log_buffer<InMemoryLogBufferSize> logs __attribute__((section (".noinit")));
static bool logs_buffer_free = true;

#if defined(__SAMD51__)

static bool logs_rtt_enabled = true;

static void write_logs_buffer(char c, void *arg) {
    auto app = reinterpret_cast<log_buffer::appender *>(arg);

    // This causes some weird issues. Need a way to do this w/o
    // clearing the buffer.
    if (false) {
        if (logs.full()) {
            logs_buffer_free = false;
            get_sd_card()->append_logs(logs);
            logs.zero();
            logs_buffer_free = true;
        }
    }

    if (c != 0) {
        app->append(c);
    }
}

size_t write_log(LogMessage const *m, const char *fstring, va_list args) {
    // No reason being here if we aren't going to log anything.
    if (!logs_rtt_enabled && !logs_buffer_free) {
        return true;
    }

    auto level = alog_get_log_level((LogLevels)m->level);
    auto plain_fs = "";
    auto color_fs = "";
    auto task = os_task_name();
    if (task == nullptr) {
        task = "startup";
    }

    if ((LogLevels)m->level == LogLevels::ERROR) {
        color_fs = RTT_CTRL_TEXT_GREEN "%08" PRIu32 RTT_CTRL_TEXT_CYAN " %-10s " RTT_CTRL_TEXT_RED "%-7s %s: ";
        plain_fs = "%08" PRIu32 " %-10s %-7s %s: ";
    }
    else if ((LogLevels)m->level == LogLevels::WARN) {
        color_fs = RTT_CTRL_TEXT_GREEN "%08" PRIu32 RTT_CTRL_TEXT_CYAN " %-10s " RTT_CTRL_TEXT_MAGENTA "%-7s %s: ";
        plain_fs = "%08" PRIu32 " %-10s %-7s %s: ";
    }
    else {
        color_fs = RTT_CTRL_TEXT_GREEN "%08" PRIu32 RTT_CTRL_TEXT_CYAN " %-10s " RTT_CTRL_TEXT_YELLOW "%-7s %s" RTT_CTRL_RESET ": ";
        plain_fs = "%08" PRIu32 " %-10s %-7s %s: ";
    }

    SEGGER_RTT_LOCK();

    if (logs_rtt_enabled) {
        SEGGER_RTT_printf(0, color_fs, m->uptime, task, level, m->facility);
        SEGGER_RTT_vprintf(0, fstring, &args);
        SEGGER_RTT_WriteString(0, RTT_CTRL_RESET "\n");
    }

    if (logs_buffer_free) {
        auto app = logs.start();

        tiny_fctprintf(write_logs_buffer, &app, plain_fs, m->uptime, task, level, m->facility);
        tiny_vfctprintf(write_logs_buffer, &app, fstring, args);
        tiny_fctprintf(write_logs_buffer, &app, "\n");

        app.append((char)0);
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
    logs.zero();

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

            fk_logging_dump_buffer();

            break;
        }
    }

    logs_rtt_enabled = has_rtt_reader;

    return true;
}

bool fk_logging_dump_buffer() {
    SEGGER_RTT_LOCK();

    SEGGER_RTT_WriteString(0, RTT_CTRL_RESET "\n");

    for (auto c : logs) {
        if (c != 0) {
            SEGGER_RTT_PutChar(0, c);
        }
    }

    SEGGER_RTT_WriteString(0, RTT_CTRL_RESET "\n");

    SEGGER_RTT_UNLOCK();

    return true;
}

#endif

bool fk_log_buffer_try_lock() {
    auto success = false;

    SEGGER_RTT_LOCK();

    if (logs_buffer_free) {
        logs_buffer_free = false;
        success = true;
    }

    SEGGER_RTT_UNLOCK();

    return success;
}

void fk_log_buffer_unlock() {
    logs_buffer_free = true;
}

log_buffer &fk_log_buffer() {
    return logs;
}

}
