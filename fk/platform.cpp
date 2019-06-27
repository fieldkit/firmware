#include <cstdarg>

#include "fk.h"
#include "platform.h"

#if defined(ARDUINO)
#include <Arduino.h>
#endif

namespace fk {

#if defined(ARDUINO)

uint32_t fk_uptime() {
    return millis();
}

#else
#endif

}

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__SAMD51__)

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
