#ifndef ALOGGING_LOGGING_H_INCLUDED
#define ALOGGING_LOGGING_H_INCLUDED

#include <cinttypes>
#include <cstddef>
#include <cstdarg>
#include <cstdio>

constexpr size_t ArduinoLoggingLineMax = 255;

enum class LogLevels : uint8_t {
    VERBOSE,
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    NONE,
};

typedef struct LogMessage {
    uint32_t number;
    uint32_t uptime;
    uint32_t time;
    uint8_t level;
    const char *facility;
    const char *scope;
    const char *message;
} LogMessage;

typedef size_t (*log_message_write_fn_t)(const LogMessage *m, const char *fstring, va_list args);

typedef size_t (*log_message_hook_fn_t)(const LogMessage *m, const char *fstring, va_list args, void *arg);

typedef uint32_t (*log_message_time_fn_t)();

typedef uint32_t (*log_message_uptime_fn_t)();

typedef const char *(*log_message_scope_fn_t)();

#ifdef __cplusplus
extern "C" {
#endif

void log_configure_writer(log_message_write_fn_t writer);

void log_configure_hook_register(log_message_hook_fn_t hook, void *arg);

void log_configure_hook(bool enabled);

void log_configure_time(log_message_uptime_fn_t uptime_fn, log_message_time_fn_t time_fn);

void log_configure_level(LogLevels level);

void log_configure_scope(log_message_scope_fn_t scope_fn);

uint8_t log_get_level(void);

bool log_is_trace(void);

bool log_is_debug(void);

void log_raw(const LogMessage *m, const char *fstring, va_list args);

void valogf(LogLevels level, const char *facility, const char *f, va_list args);

void valogfs(LogLevels level, const char *facility, const char *scope, const char *f, va_list args);

void alogs(LogLevels level, const char *facility, const char *str);

void alogf(LogLevels level, const char *facility, const char *f, ...) __attribute__((format(printf, 3, 4)));

void alogfs(LogLevels level, const char *facility, const char *scope, const char *f, ...) __attribute__((format(printf, 4, 5)));

void logerrorf(const char *facility, const char *f, ...) __attribute__((format(printf, 2, 3)));

void logtracef(const char *facility, const char *f, ...) __attribute__((format(printf, 2, 3)));

void loginfof(const char *facility, const char *f, ...) __attribute__((format(printf, 2, 3)));

void loginfofv(const char *facility, const char *f, va_list args);

const char *alog_get_log_level(LogLevels level);

#ifdef __cplusplus
}
#endif

#endif
