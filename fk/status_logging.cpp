#include <malloc.h>
#include <loading.h>

#include "status_logging.h"
#include "storage/storage.h"
#include "tasks/tasks.h"
#include "hal/hal.h"
#include "clock.h"
#include "state_manager.h"
#include "state_ref.h"

#if defined(__SAMD51__)

extern const struct fkb_header_t fkb_header;

extern uint32_t __cm_ram_origin__;
extern uint32_t __cm_ram_end__;
extern uint32_t __heap_start__;
extern uint32_t __heap_end__;
extern uint32_t __data_start__;
extern uint32_t __data_end__;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;
extern uint32_t __noinit_start__;
extern uint32_t __noinit_end__;

#endif

namespace fk {

FK_DECLARE_LOGGER("status");

static void log_status() {
    auto gs = try_get_global_state_ro();
    if (!gs) {
        logwarn("failed to get gs");
        return;
    }

    auto now = get_clock_now();
    auto mi = mallinfo();
    auto name = gs.get()->general.name;
    ip4_address ip{ gs.get()->network.state.ip };
    auto spmi = fk_standard_page_meminfo();
    auto memory_oddity = mi.arena != mi.uordblks;
    auto percentage = (float)spmi.used / spmi.total * 100.0f;
    auto rssi = get_network()->rssi();

    FormattedTime formatted{ now };
    loginfo("%s '%s' (%d.%d.%d.%d) (%" PRId32 ") memory(%" PRIu32 " / %zd%s) pages(%zd / %zd / %zd, %.2f%% used)",
            formatted.cstr(), name, ip.u.bytes[0], ip.u.bytes[1], ip.u.bytes[2], ip.u.bytes[3],
            rssi, fk_free_memory(), (size_t)mi.arena, (memory_oddity ? " ERR" : ""),
            spmi.total - spmi.free, spmi.highwater, spmi.total, percentage);
}

void fk_status_log() {
    log_status();
}

bool fk_log_diagnostics() {
    #if defined(__SAMD51__)
    uint8_t stack_dummy = 0;
    auto in_stack = (uint8_t *)&__cm_ram_end__ - &stack_dummy;
    auto available = (unsigned long)fk_free_memory();
    auto data = (&__data_end__ - &__data_start__) * sizeof(uint32_t);
    auto bss = (&__bss_end__ - &__bss_start__) * sizeof(uint32_t);
    auto heap = (&__heap_end__ - &__heap_start__) * sizeof(uint32_t);
    auto noi = (&__noinit_end__ - &__noinit_start__) * sizeof(uint32_t);
    auto used = (&__heap_end__ - &__cm_ram_origin__) * sizeof(uint32_t);

    loginfo("hello (memory = %lu) (data + bss + noi + heap = %zd + %zd + %zd + %zd = %zd) (used = %zd) (stack-left-now ~ %zd)",
            available, data, bss, noi, heap, noi + data + bss + heap, used, in_stack);

    fk_serial_number_t sn;
    loginfo("serial = %08" PRIx32 "-%08" PRIx32 "-%08" PRIx32 "-%08" PRIx32,
            (uint32_t)__builtin_bswap32(sn.dwords[0]), (uint32_t)__builtin_bswap32(sn.dwords[1]),
            (uint32_t)__builtin_bswap32(sn.dwords[2]), (uint32_t)__builtin_bswap32(sn.dwords[3]));

    loginfo("fw = %s (#%" PRIu32 ")", fkb_header.firmware.version, fkb_header.firmware.number);
    char hash_string[128];
    bytes_to_hex_string(hash_string, sizeof(hash_string), fkb_header.firmware.hash, fkb_header.firmware.hash_size);
    loginfo("hash = %s", hash_string);
    #endif

    return true;
}

}
