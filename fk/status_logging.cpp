#include <malloc.h>

#include "status_logging.h"
#include "tasks/tasks.h"
#include "hal/hal.h"
#include "clock.h"
#include "state_manager.h"
#include "state_ref.h"

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

    FormattedTime formatted{ now };
    loginfo("%s '%s' (%d.%d.%d.%d) memory(%" PRIu32 ", %zd, %zd) pages(%zd / %zd / %zd)",
            formatted.cstr(), name, ip.u.bytes[0], ip.u.bytes[1], ip.u.bytes[2], ip.u.bytes[3],
            fk_free_memory(), (size_t)mi.arena, (size_t)mi.uordblks,
            spmi.total - spmi.free, spmi.highwater, spmi.total);
}

void fk_status_log() {
    log_status();
}

}
