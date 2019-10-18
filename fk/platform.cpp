#include <cstdarg>

#include "fk.h"
#include "platform.h"

#if defined(__SAMD51__)
#include <Arduino.h>
#include <SEGGER_RTT.h>
#include <os.h>
#include <loading.h>
#else // __SAMD51__
#include <chrono>
#include <vector>
#include <queue>
#endif // __SAMD51__

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__SAMD51__)

void fk_assert(const char *assertion, const char *file, int32_t line, const char *f, ...) {
    logerrorf("assertion", "\"%s\" failed: file \"%s\", line %" PRIu32, assertion, file, line);
    os_panic(OS_PANIC_ASSERTION);
}

#else // __SAMD51__

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

#endif // __SAMD51__

#ifdef __cplusplus
}
#endif

namespace fk {

#if defined(__SAMD51__)

uint32_t fk_uptime() {
    return millis();
}

uint32_t fk_delay(uint32_t ms) {
    return os_delay(ms);
}

extern "C" char *sbrk(int32_t i);

uint32_t fk_free_memory() {
    return os_free_memory();
}

uint32_t fk_serial_number_get(fk_serial_number_t *sn) {
    sn->dwords[0] = __builtin_bswap32(*(uint32_t *)0x008061FC);
    sn->dwords[1] = __builtin_bswap32(*(uint32_t *)0x00806010);
    sn->dwords[2] = __builtin_bswap32(*(uint32_t *)0x00806014);
    sn->dwords[3] = __builtin_bswap32(*(uint32_t *)0x00806018);
    return 128;
}

#else // __SAMD51__

using namespace std::chrono;

std::queue<uint32_t> uptimes;

uint32_t fk_fake_uptime(std::vector<uint32_t> more) {
    for (auto a : more) {
        uptimes.push(a);
    }
    return uptimes.size();
}

static uint32_t started = 0;

uint32_t fk_uptime() {
    if (uptimes.size() > 0) {
        auto time = uptimes.front();
        uptimes.pop();
        return time;
    }
    auto machine_uptime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    if (started == 0) {
        started = machine_uptime;
    }
    return machine_uptime - started;
}

uint32_t fk_delay(uint32_t ms) {
    return 0;
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

void fk_restart() {
}

void fk_nvm_swap_banks() {
}

uint8_t fk_nvm_get_active_bank() {
    return 0;
}

#endif // __SAMD51__

fk_serial_number_t::fk_serial_number_t() {
    fk_serial_number_get(this);
}

} // namespace fk

extern "C" {

void *fk_malloc_internal(size_t size, const char *file, int32_t line) {
    auto ptr = ::malloc(size);
    alogf(LogLevels::DEBUG, "memory", "[0x%" PRIxPTR "] malloc(%zd) %s:%" PRId32 "", (uintptr_t)ptr, size, file, line);
    return ptr;
}

void fk_free_internal(void *ptr, const char *file, int32_t line) {
    alogf(LogLevels::DEBUG, "memory", "[0x%" PRIxPTR "] free %s:%" PRId32 "", (uintptr_t)ptr, file, line);
    return ::free(ptr);
}

}
