#if defined(linux)

#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <string.h>

#include "logging_linux.h"
#include "logging.h"

static uint64_t epoch = 0;

#ifdef __cplusplus
extern "C" {
#endif

uint32_t millis() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    auto now  = (uint64_t)ts.tv_sec * (uint64_t)1000 + (uint64_t)(ts.tv_nsec / 1000000L);
    if (epoch == 0) {
        epoch = now;
    }
    return (uint32_t)(now - epoch);
}

#define COLOR_YELLOW "\033[0;33m"
#define COLOR_GREEN  "\033[0;32m"
#define COLOR_RED    "\033[0;31m"
#define COLOR_RESET  "\033[0m"

size_t platform_write_fn(const LogMessage *m, const char *fstring, va_list args) {
    auto level = alog_get_log_level((LogLevels)m->level);
    if ((LogLevels)m->level == LogLevels::ERROR) {
        fprintf(stderr, COLOR_GREEN "%08" PRIu32 COLOR_RED " %-7s %s: ", m->uptime, level, m->facility);
    }
    else {
        fprintf(stderr, COLOR_GREEN "%08" PRIu32 COLOR_YELLOW " %-7s %s" COLOR_RESET ": ", m->uptime, level, m->facility);
    }
    fprintf(stderr, "%s", m->scope);
    vfprintf(stderr, fstring, args);
    auto l = strlen(fstring);
    if (fstring[l - 1] != '\n') {
        fprintf(stderr, COLOR_RESET "\n");
    }
    return 0;
}

void alogging_platform_abort() {
    abort();
}

#ifdef __cplusplus
}
#endif

#endif // !defined(ARDUINO)
