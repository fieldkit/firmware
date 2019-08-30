#include "common.h"
#include "debugging.h"
#include "config.h"

#include <SEGGER_RTT.h>

namespace fk {

static bool console_attached = false;

bool fk_debug_is_attached() {
    return false;
}

bool fk_debug_get_console_attached() {
    return console_attached;
}

void fk_debug_set_console_attached() {
    if (!console_attached) {
        auto buffer = fk_malloc(DefaultWorkerPoolSize);
        SEGGER_RTT_ConfigUpBuffer(0, "default", buffer, DefaultWorkerPoolSize, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);
        console_attached = true;
    }
}

}
