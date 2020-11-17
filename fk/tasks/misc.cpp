#include "tasks/tasks.h"
#include "hal/hal.h"

#include "storage/storage.h"
#include "readings_taker.h"
#include "state_ref.h"
#include "config.h"

namespace fk {

FK_DECLARE_LOGGER("misc");

void fk_verify_state() {
    auto gs = get_global_state_ro();
    FK_ASSERT(gs.get()->modules->nmodules >= 0 && gs.get()->modules->nmodules < 10);
}

void task_handler_misc(void *params) {
    loginfo("misc: started, waiting");

    fk_delay(5000);

    loginfo("misc: ready");

    while (true) {
        fk_delay(10);
        fk_verify_state();
        SEGGER_RTT_printf(0, ".");
    }
}

}
