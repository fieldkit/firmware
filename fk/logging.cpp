#include <loading.h>
#include <os.h>
#include <tiny_printf.h>
#include <ctype.h>

#include "logging.h"
#include "platform.h"
#include "config.h"
#include "circular_buffer.h"
#include "hal/sd_card.h"
#include "memory.h"

namespace fk {

static static_log_buffer<InMemoryLogBufferSize> logs __attribute__((section (".noinit")));
static log_buffer::iterator sd_card_iterator;
static bool logs_buffer_free = true;

#if defined(__SAMD51__)
static_assert(InMemoryLogBufferSize % StandardPageSize == 0, "log size should be divisible by standard page size.");
#endif

constexpr size_t StandardPagesForLogs = InMemoryLogBufferSize / StandardPageSize;

#if defined(__SAMD51__)

static bool logs_rtt_enabled = true;

typedef struct saved_logs_t {
    uint8_t *pages[StandardPagesForLogs];
} saved_logs_t;

static saved_logs_t saved_logs = { .pages = { nullptr, nullptr, nullptr, nullptr } };

#define FK_LOGS_LOCK       SEGGER_RTT_LOCK
#define FK_LOGS_UNLOCK     SEGGER_RTT_UNLOCK

static void write_logs_buffer(char c, void *arg) {
    auto app = reinterpret_cast<log_buffer::appender *>(arg);

    if (logs.size(sd_card_iterator) >= InMemoryLogBufferSize - 1024) {
        fk_logs_flush();
    }

    if (c != 0) {
        app->append(c);
    }
}

bool fk_logs_flush() {
    logs_buffer_free = false;
    get_sd_card()->append_logs(logs, sd_card_iterator);
    sd_card_iterator = logs.end();
    logs_buffer_free = true;

    return true;
}

void fk_logs_vprintf(const char *f, va_list args) {
    auto app = logs.start();
    SEGGER_RTT_vprintf(0, f, &args);
    tiny_vfctprintf(write_logs_buffer, &app, f, args);
    app.append((char)0);
}

void fk_logs_saved_capture() {
    auto source = logs.buffer();
    for (auto i = 0u; i < StandardPagesForLogs; ++i) {
        saved_logs.pages[i] = (uint8_t *)fk_standard_page_malloc(StandardPageSize, "saved-logs");
        memcpy(saved_logs.pages[i], source, StandardPageSize);
        source += StandardPageSize;

        /**
         * We'll be called very early and so the odds of us being
         * given non-consecutive pages is zero. It's also not
         * important but I figured I'd keep this in here just in case
         * I ever forget that fact.
         */
        FK_ASSERT(i == 0 || saved_logs.pages[i] == saved_logs.pages[i - 1] + StandardPageSize);
    }

    for (auto i = 0u; i < StandardPagesForLogs; ++i) {
        auto page = saved_logs.pages[i];
        if (page != nullptr) {
            for (auto p = page; p < page + StandardPageSize; ) {
                if (*p == 0) {
                    *p = '\n';
                } else if (!isprint(*p)) {
                    if (*p == '\n') {
                        *p = ' ';
                    } else {
                        *p = '?';
                    }
                }
                p++;
            }
        }
    }
}

void fk_logs_saved_write(bool echo) {
    auto begin_header = "\n\n=================== raw log memory begin: remember buffer is circular!\n\n";
    auto end_footer = "\n\n=================== raw log memory end: remember buffer is circular!\n\n";

    if (echo) {
        FK_LOGS_LOCK();

        fk_logs_printf(begin_header);

        for (auto i = 0u; i < StandardPagesForLogs; ++i) {
            auto page = saved_logs.pages[i];
            if (page != nullptr) {
                SEGGER_RTT_Write(0, page, StandardPageSize);
            }
        }

        fk_logs_printf(end_footer);

        FK_LOGS_UNLOCK();
    }

    get_sd_card()->append_logs((uint8_t *)begin_header, strlen(begin_header));

    for (auto i = 0u; i < StandardPagesForLogs; ++i) {
        if (saved_logs.pages[i] != nullptr) {
            get_sd_card()->append_logs(saved_logs.pages[i], StandardPageSize);
        }
    }

    get_sd_card()->append_logs((uint8_t *)end_footer, strlen(end_footer));
}

void fk_logs_saved_free() {
    for (auto i = 0u; i < StandardPagesForLogs; ++i) {
        if (saved_logs.pages[i] != nullptr) {
            fk_standard_page_free(saved_logs.pages[i]);
            saved_logs.pages[i] = nullptr;
        }
    }
}

size_t write_log(LogMessage const *m, const char *fstring, va_list args) {
    // No reason being here if we aren't going to log anything.
    if (!logs_rtt_enabled && !logs_buffer_free) {
        return true;
    }

    auto level = alog_get_log_level((LogLevels)m->level);
    auto plain_fs = "%08" PRIu32 " %-10s %-7s %s: %s";
    auto color_fs = "";
    auto task = os_task_name();
    if (task == nullptr) {
        task = "startup";
    }

    if ((LogLevels)m->level == LogLevels::ERROR) {
        color_fs = RTT_CTRL_TEXT_GREEN "%08" PRIu32 RTT_CTRL_TEXT_CYAN " %-10s " RTT_CTRL_TEXT_RED "%-7s %s%s: ";
    }
    else if ((LogLevels)m->level == LogLevels::WARN) {
        color_fs = RTT_CTRL_TEXT_GREEN "%08" PRIu32 RTT_CTRL_TEXT_CYAN " %-10s " RTT_CTRL_TEXT_MAGENTA "%-7s %s%s: ";
    }
    else {
        color_fs = RTT_CTRL_TEXT_GREEN "%08" PRIu32 RTT_CTRL_TEXT_CYAN " %-10s " RTT_CTRL_TEXT_YELLOW "%-7s %s%s" RTT_CTRL_RESET ": ";
    }

    FK_LOGS_LOCK();

    if (logs_rtt_enabled) {
        SEGGER_RTT_printf(0, color_fs, m->uptime, task, level, m->scope, m->facility);
        SEGGER_RTT_vprintf(0, fstring, &args);
        SEGGER_RTT_WriteString(0, RTT_CTRL_RESET "\n");
    }

    if (logs_buffer_free) {
        auto app = logs.start();

        tiny_fctprintf(write_logs_buffer, &app, plain_fs, m->uptime, task, level, m->scope, m->facility);
        tiny_vfctprintf(write_logs_buffer, &app, fstring, args);
        tiny_fctprintf(write_logs_buffer, &app, "\n");

        app.append((char)0);
    }

    FK_LOGS_UNLOCK();

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
    fk_logs_saved_capture();

    fk_logs_clear();

    sd_card_iterator = logs.end();

    log_configure_writer(write_log);
    log_configure_level(LogLevels::DEBUG);
    log_configure_time(fk_uptime, nullptr);

    OS_CHECK(os_configure_hooks(task_logging_hook, nullptr));

    auto has_rtt_reader = false;
    auto waiting_until = fk_uptime() + OneSecondMs;
    while (fk_uptime() < waiting_until) {
        if (SEGGER_RTT_HasData(0) || !SEGGER_RTT_HasDataUp(0)) {
            fk_debug_set_console_attached();

            SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);

            has_rtt_reader = true;

            break;
        }
    }

    logs_rtt_enabled = has_rtt_reader;

    return true;
}

bool fk_logging_dump_buffer() {
    FK_LOGS_LOCK();

    SEGGER_RTT_WriteString(0, RTT_CTRL_RESET "\n");

    for (auto c : logs) {
        if (c != 0) {
            SEGGER_RTT_PutChar(0, c);
        }
    }

    SEGGER_RTT_WriteString(0, RTT_CTRL_RESET "\n");

    FK_LOGS_UNLOCK();

    return true;
}

void fk_log_debugging(const char *source) {
    for (os_task_t *iter = osg.runqueue; iter != NULL; iter = iter->nrp) {
        alogf(LogLevels::DEBUG, source, "rq '%s' status(%s) (0x%" PRIx32 ")", iter->name,
              os_task_status_str(iter->status), iter->priority);
    }

    for (os_task_t *iter = osg.waitqueue; iter != NULL; iter = iter->nrp) {
        alogf(LogLevels::DEBUG, source, "wq '%s' status(%s) (0x%" PRIx32 ")", iter->name,
              os_task_status_str(iter->status), iter->priority);
    }

    fk_standard_page_log();
}

#else

void fk_logs_vprintf(const char *f, va_list args) {
    vprintf(f, args);
}

bool fk_logs_flush() {
    return true;
}

void fk_logs_saved_write(bool echo) {
}

void fk_logs_saved_free() {
}

void fk_log_debugging(const char *source) {
}

#define FK_LOGS_LOCK()

#define FK_LOGS_UNLOCK()

#endif

void fk_logs_printf(const char *f, ...) {
    va_list args;
    va_start(args, f);

    FK_LOGS_LOCK();
    fk_logs_vprintf(f, args);
    FK_LOGS_UNLOCK();

    va_end(args);
}

void fk_logs_dump_memory(const char *prefix, uint8_t const *ptr, size_t size, ...) {
    va_list args;
    va_start(args, size);

    #if defined(__SAMD51__)
    FK_LOGS_LOCK();
    #endif

    // Prewrite the prefix into the line. We force this to max 32
    // characters here.
    char line[(32) + (32 * 2) + 1];
    auto prefix_length = tiny_vsnprintf(line, 32, prefix, args);
    if (prefix_length > 32) {
        prefix_length = 32;
    }
    auto p = (char *)nullptr;

    for (auto i = (size_t)0; i < size; ++i) {
        if (p == nullptr) {
            p = line + prefix_length;
        }
        tiny_sprintf(p, "%02x ", ptr[i]);
        p += 3;
        if ((i + 1) % 32 == 0) {
            *p = 0;
            fk_logs_printf("%s\n", line);
            p = nullptr;
        }
    }
    if (p != nullptr) {
        *p = 0;
        fk_logs_printf("%s\n", line);
        p = nullptr;
    }
    #if defined(__SAMD51__)
    FK_LOGS_UNLOCK();
    #endif

    va_end(args);
}

void fk_logs_clear() {
    logs.zero();
}

bool fk_log_buffer_try_lock() {
    auto success = false;

    FK_LOGS_LOCK();

    if (logs_buffer_free) {
        logs_buffer_free = false;
        success = true;
    }

    FK_LOGS_UNLOCK();

    return success;
}

void fk_log_buffer_unlock() {
    logs_buffer_free = true;
}

log_buffer &fk_log_buffer() {
    return logs;
}

}

namespace fk {

void fk_logf(LogLevels level, const char *logger, const char *f, ...) {
    va_list args;
    va_start(args, f);
    valogf(level, logger, f, args);
    va_end(args);
}

} // namespace fk
