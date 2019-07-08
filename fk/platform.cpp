#include <cstdarg>

#include "fk.h"
#include "platform.h"

#if defined(__SAMD51__)
#include <Arduino.h>
#include <SEGGER_RTT.h>
#include <os.h>
#else
#include <chrono>
#include <vector>
#include <queue>
#endif

namespace fk {

#if defined(__SAMD51__)

uint32_t fk_uptime() {
    return millis();
}

uint32_t fk_delay(uint32_t ms) {
    return os_delay(ms);
}

void fk_assert(const char *assertion, const char *file, int line) {
    fkb_external_printf("\n\nassertion \"%s\" failed: file \"%s\", line %d\n", assertion, file, line);
    while (true) {
    }
}

extern "C" char *sbrk(int32_t i);

uint32_t fk_free_memory() {
    char stack_dummy = 0;
    return &stack_dummy - sbrk(0);
}

uint32_t fk_serial_number_get(fk_serial_number_t *sn) {
    sn->dwords[0] = *(uint32_t *)0x008061FC;
    sn->dwords[1] = *(uint32_t *)0x00806010;
    sn->dwords[2] = *(uint32_t *)0x00806014;
    sn->dwords[3] = *(uint32_t *)0x00806018;
    return 128;
}

#else

using namespace std::chrono;

std::queue<uint32_t> uptimes;

uint32_t fk_fake_uptime(std::vector<uint32_t> more) {
    for (auto a : more) {
        uptimes.push(a);
    }
    return uptimes.size();
}

uint32_t fk_uptime() {
    if (uptimes.size() > 0) {
        auto time = uptimes.front();
        uptimes.pop();
        return time;
    }
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

uint32_t fk_delay(uint32_t ms) {
    return 0;
}

void fk_assert(const char *assertion, const char *file, int line) {
    fkb_external_printf("\n\nassertion \"%s\" failed: file \"%s\", line %d\n", assertion, file, line);
}

uint32_t fk_free_memory() {
    return 0;
}

uint32_t fk_serial_number_get(fk_serial_number_t *sn) {
    sn->dwords[0] = 0;
    sn->dwords[1] = 0;
    sn->dwords[2] = 0;
    sn->dwords[3] = 0;
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

uint32_t fkb_external_vprintf(const char *str, va_list args) {
    return (uint32_t)SEGGER_RTT_vprintf(0, str, &args);
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

uint32_t fkb_external_vprintf(const char *str, va_list args) {
    return vfprintf(stderr, str, args);
}

#endif

#ifdef __cplusplus
}
#endif
