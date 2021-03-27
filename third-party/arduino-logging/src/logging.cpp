#include "alogging/sprintf.h"
#include "logging.h"
#include "platform.h"
#include "assert.h"

#ifdef __cplusplus
extern "C" {
#endif

extern size_t platform_write_fn(const LogMessage *m, const char *fstring, va_list args);

static uint32_t always_zero();

static log_message_uptime_fn_t log_uptime_fn = millis;
static log_message_time_fn_t log_time_fn = always_zero;
static log_message_write_fn_t write_fn = platform_write_fn;

static log_message_hook_fn_t log_hook_fn = nullptr;
static void *log_hook_arg = nullptr;
static bool log_hook_enabled = false;
static uint32_t log_counter = 0;
static uint8_t log_level = (uint8_t)LogLevels::INFO;

static uint32_t always_zero() {
    return 0;
}

void log_configure_writer(log_message_write_fn_t new_fn) {
    write_fn = new_fn;
}

void log_configure_hook_register(log_message_hook_fn_t hook, void *arg) {
    log_hook_fn = hook;
    log_hook_arg = arg;
    log_hook_enabled = true;
}

void log_configure_hook(bool enabled) {
    log_hook_enabled = enabled;
}

void log_configure_level(LogLevels level) {
    log_level = (uint8_t)level;
}

uint8_t log_get_level(void) {
    return log_level;
}

bool log_is_trace(void) {
    return log_level <= (uint8_t)LogLevels::TRACE;
}

bool log_is_debug(void) {
    return log_level <= (uint8_t)LogLevels::DEBUG;
}

void log_configure_time(log_message_uptime_fn_t uptime_fn, log_message_time_fn_t time_fn) {
    log_uptime_fn = uptime_fn == nullptr ? always_zero : uptime_fn;
    log_time_fn = time_fn == nullptr ? always_zero : time_fn;
}

void log_raw(const LogMessage *m, const char *fstring, va_list args) {
    if (write_fn != nullptr) {
        write_fn(m, fstring, args);
    }

    if (log_hook_fn != nullptr) {
        if (log_hook_enabled) {
            log_hook_enabled = false;
            log_hook_fn(m, fstring, args, log_hook_arg);
            log_hook_enabled = true;
        }
    }
}

void valogf(LogLevels level, const char *facility, const char *f, va_list args) {
    return valogfs(level, facility, "", f, args);
}

void valogfs(LogLevels level, const char *facility, const char *scope, const char *f, va_list args) {
    if ((uint8_t)level < log_get_level()) {
        return;
    }

    LogMessage m;
    m.uptime = log_uptime_fn();
    m.number = log_counter++;
    m.time = log_time_fn();
    m.level = (uint8_t)level;
    m.facility = facility;
    m.scope = scope;
    m.message = nullptr;

    log_raw(&m, f, args);
}

void alogs(LogLevels level, const char *facility, const char *str) {
    va_list args;
    valogf(level, facility, str, args);
}

void alogf(LogLevels level, const char *facility, const char *f, ...) {
    va_list args;
    va_start(args, f);
    valogf(level, facility, f, args);
    va_end(args);
}

void alogfs(LogLevels level, const char *facility, const char *scope, const char *f, ...) {
    va_list args;
    va_start(args, f);
    valogfs(level, facility, scope, f, args);
    va_end(args);
}

void logerrorf(const char *facility, const char *f, ...) {
    va_list args;
    va_start(args, f);
    valogf(LogLevels::ERROR, facility, f, args);
    va_end(args);
}

void logtracef(const char *facility, const char *f, ...) {
    va_list args;
    va_start(args, f);
    valogf(LogLevels::TRACE, facility, f, args);
    va_end(args);
}

void loginfof(const char *facility, const char *f, ...) {
    va_list args;
    va_start(args, f);
    valogf(LogLevels::INFO, facility, f, args);
    va_end(args);
}

void loginfofv(const char *facility, const char *f, va_list args) {
    valogf(LogLevels::INFO, facility, f, args);
}

const char *alog_get_log_level(LogLevels level) {
    switch (level) {
    case LogLevels::INFO: return "info";
    case LogLevels::TRACE: return "trace";
    case LogLevels::DEBUG: return "debug";
    case LogLevels::WARN: return "warn";
    case LogLevels::ERROR: return "error";
    case LogLevels::VERBOSE: return "verbose";
    default: return "unknown";
    }
}

#ifdef __cplusplus
}
#endif
