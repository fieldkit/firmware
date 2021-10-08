#include <cstdarg>

#include <os.h>

#include "fk.h"
#include "platform.h"
#include "logging.h"

#if defined(__SAMD51__)
#include <Arduino.h>
#include <SEGGER_RTT.h>
#include <loading.h>

#include <Adafruit_SleepyDog.h>
#undef min
#undef max

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
#if defined(__SAMD21__) || defined(__SAMD51__)
    NVIC_SystemReset();
#endif // defined(__SAMD21__) || defined(__SAMD51__)
    os_panic(OS_PANIC_ASSERTION);
}

#else // __SAMD51__

#endif // __SAMD51__

#ifdef __cplusplus
}
#endif

namespace fk {

#if defined(__SAMD51__)

static uint32_t adjusted = 0;

uint32_t fk_uptime() {
    return millis() + adjusted;
}

uint32_t fk_uptime_adjust_after_sleep(uint32_t delta) {
    adjusted += delta;
    return fk_uptime();
}

uint32_t fk_delay(uint32_t ms) {
    return os_delay(ms);
}

uint32_t fk_deep_sleep(uint32_t ms) {
    return Watchdog.sleep(ms);
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
    std::queue<uint32_t> empty;
    std::swap(uptimes, empty);
    for (auto a : more) {
        uptimes.push(a);
    }
    return uptimes.size();
}

uint32_t fk_uptime_adjust_after_sleep(uint32_t delta) {
    return fk_uptime();
}

static uint32_t started = 0;

uint32_t fk_uptime() {
    if (uptimes.size() > 0) {
        return uptimes.front();
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

uint32_t fk_deep_sleep(uint32_t ms) {
    return ms;
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
    if (false) {
        alogf(LogLevels::DEBUG, "memory", "[0x%" PRIxPTR "] malloc(%zd) %s:%" PRId32 "", (uintptr_t)ptr, size, file, line);
    }
    if (false) {
        printf("[0x%" PRIxPTR "] malloc(%zd) %s:%" PRId32 "\n", (uintptr_t)ptr, size, file, line);
    }
    return ptr;
}

void fk_free_internal(void *ptr, const char *file, int32_t line) {
    if (false) {
        alogf(LogLevels::DEBUG, "memory", "[0x%" PRIxPTR "] free %s:%" PRId32 "", (uintptr_t)ptr, file, line);
    }
    if (false) {
        printf("[0x%" PRIxPTR "] free %s:%" PRId32 "\n", (uintptr_t)ptr, file, line);
    }
    return ::free(ptr);
}

void osi_debug_dump(os_panic_kind_t code) {
    alogf(LogLevels::ERROR, "error", "panic! (%s)", os_panic_kind_str(code));

    if (osg.scheduled != NULL) {
        alogf(LogLevels::ERROR, "error", "osg.scheduled '%s' status(%s) (0x%" PRIx32 ")", osg.scheduled->name,
              os_task_status_str(osg.scheduled->status), osg.scheduled->priority);
    }

    if (osg.running != NULL) {
        alogf(LogLevels::ERROR, "error", "osg.running '%s' status(%s) (0x%" PRIx32 ")", osg.running->name,
              os_task_status_str(osg.running->status), osg.running->priority);
    }

    for (os_task_t *iter = osg.runqueue; iter != NULL; iter = iter->nrp) {
        alogf(LogLevels::ERROR, "error", "rq '%s' status(%s) (0x%" PRIx32 ")", iter->name, os_task_status_str(iter->status),
              iter->priority);
    }

    for (os_task_t *iter = osg.waitqueue; iter != NULL; iter = iter->nrp) {
        alogf(LogLevels::ERROR, "error", "wq '%s' status(%s) (0x%" PRIx32 ")", iter->name, os_task_status_str(iter->status),
              iter->priority);
    }

    fk::fk_logs_flush();
}

void osi_panic(os_panic_kind_t code) {
    osi_debug_dump(code);

#if defined(__SAMD21__) || defined(__SAMD51__)
    NVIC_SystemReset();
#endif // defined(__SAMD21__) || defined(__SAMD51__)
}

void osi_hard_fault_report(uintptr_t *stack, uint32_t lr, cortex_hard_fault_t *hfr) {
#if defined(__SAMD21__) || defined(__SAMD51__)
    alogf(LogLevels::ERROR, "error", "hard fault! stack= 0x%" PRIx32 " lr=0x%" PRIx32 "", (uint32_t)stack, lr);
    alogf(LogLevels::ERROR, "error", "hard fault! mfsr=  0x%" PRIx32 "", (uint32_t)hfr->mfsr.byte);
    alogf(LogLevels::ERROR, "error", "hard fault! bfsr=  0x%" PRIx32 "", (uint32_t)hfr->bfsr.byte);
    alogf(LogLevels::ERROR, "error", "hard fault! bfar=  0x%" PRIx32 "", (uint32_t)hfr->bfar);
    alogf(LogLevels::ERROR, "error", "hard fault! ufsr=  0x%" PRIx32 "", (uint32_t)hfr->ufsr.byte);
    alogf(LogLevels::ERROR, "error", "hard fault! hfsr=  0x%" PRIx32 "", (uint32_t)hfr->hfsr.byte);
    alogf(LogLevels::ERROR, "error", "hard fault! dfsr=  0x%" PRIx32 "", (uint32_t)hfr->dfsr.byte);
    alogf(LogLevels::ERROR, "error", "hard fault! afsr=  0x%" PRIx32 "", (uint32_t)hfr->afsr);

    alogf(LogLevels::ERROR, "error", "hard fault! r0=    0x%" PRIx32 "", (uint32_t)hfr->registers.R0);
    alogf(LogLevels::ERROR, "error", "hard fault! r1=    0x%" PRIx32 "", (uint32_t)hfr->registers.R1);
    alogf(LogLevels::ERROR, "error", "hard fault! r2=    0x%" PRIx32 "", (uint32_t)hfr->registers.R2);
    alogf(LogLevels::ERROR, "error", "hard fault! r3=    0x%" PRIx32 "", (uint32_t)hfr->registers.R3);
    alogf(LogLevels::ERROR, "error", "hard fault! r12=   0x%" PRIx32 "", (uint32_t)hfr->registers.R12);
    alogf(LogLevels::ERROR, "error", "hard fault! lr=    0x%" PRIx32 "", (uint32_t)hfr->registers.LR);
    alogf(LogLevels::ERROR, "error", "hard fault! pc=    0x%" PRIx32 "", (uint32_t)hfr->registers.PC);
    alogf(LogLevels::ERROR, "error", "hard fault! psr=   0x%" PRIx32 "", (uint32_t)hfr->registers.psr.byte);
#endif // defined(__SAMD21__) || defined(__SAMD51__)

#if defined(__SAMD21__) || defined(__SAMD51__)
    NVIC_SystemReset();
#endif // defined(__SAMD21__) || defined(__SAMD51__)
}

#if defined(__SAMD21__) || defined(__SAMD51__)

void osi_assert(const char *assertion, const char *file, int line) {
    alogf(LogLevels::ERROR, "error", "assertion \"%s\" failed: file \"%s\", line %d", assertion, file, line);
    osi_panic(OS_PANIC_ASSERTION);
}

#endif // defined(__SAMD21__) || defined(__SAMD51__)
}
