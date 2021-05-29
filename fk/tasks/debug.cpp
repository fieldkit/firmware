#include "tasks/tasks.h"
#include "hal/hal.h"

#include "storage/storage.h"
#include "readings_taker.h"
#include "state_ref.h"
#include "config.h"

namespace fk {

FK_DECLARE_LOGGER("debug");

void fk_verify_state() {
    get_ipc()->verify();
    get_network()->verify();
}

void task_handler_debug(void *params) {
    loginfo("started, waiting");

    fk_delay(5000);

    loginfo("ready");

    while (true) {
        fk_delay(10);
        SEGGER_RTT_printf(0, ".");
        fk_verify_state();
    }
}

}
