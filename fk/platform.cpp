#include <cstdarg>

#include "fk.h"
#include "platform.h"

#if defined(__SAMD51__)
#include <Arduino.h>
#include <SEGGER_RTT.h>
#endif

namespace fk {

#if defined(__SAMD51__)

uint32_t fk_uptime() {
    return millis();
}

uint32_t fk_delay(uint32_t ms) {
    delay(ms);
    return 0;
}

void fk_assert(const char *assertion, const char *file, int line) {
    fkb_external_printf("Assertion \"%s\" failed: file \"%s\", line %d\n", assertion, file, line);
    while (true) {
    }
}

#else

void fk_assert(const char *assertion, const char *file, int line) {
    fkb_external_printf("Assertion \"%s\" failed: file \"%s\", line %d\n", assertion, file, line);
}

uint32_t fk_delay(uint32_t ms) {
    return 0;
}

#endif

}

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__SAMD51__)

uint32_t fkb_external_printf(const char *str, ...) {
    va_list args;
    va_start(args, str);
    uint32_t r = (uint32_t)SEGGER_RTT_vprintf(0, str, &args);
    va_end(args);
    return r;
}

uint32_t fkb_external_println(const char *str, ...) {
    va_list args;
    va_start(args, str);
    uint32_t r = (uint32_t)SEGGER_RTT_vprintf(0, str, &args);
    SEGGER_RTT_WriteString(0, "\n");
    va_end(args);
    return r;
}

#else

uint32_t fkb_external_printf(const char *str, ...) {
    va_list args;
    va_start(args, str);
    auto n = vfprintf(stderr, str, args);
    va_end(args);
    return n;
}

uint32_t fkb_external_println(const char *str, ...) {
    va_list args;
    va_start(args, str);
    vfprintf(stderr, str, args);
    auto n = fprintf(stderr, "\n");
    va_end(args);
    return n;
}

#endif

#ifdef __cplusplus
}
#endif
