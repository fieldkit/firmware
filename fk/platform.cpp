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
    vfprintf(stderr, str, args);
    va_end(args);
}

uint32_t fkb_external_println(const char *str, ...) {
    va_list args;
    va_start(args, str);
    vfprintf(stderr, str, args);
    fprintf(stderr, "\n");
    va_end(args);
}

#endif

#ifdef __cplusplus
}
#endif
