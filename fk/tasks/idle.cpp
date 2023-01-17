#include "hal/clock.h"
#include "config.h"
#include "memory.h"
#include "status_logging.h"
#include "tasks/tasks.h"

namespace fk {

void task_handler_idle(void *params) {
    auto counter = 0u;

    while (true) {
        fk_delay(FiveSecondsMs);
        fk_status_log();

        if (counter++ == 10) {
            get_clock()->compare();
            fk_standard_page_log();
            counter = 0;
        }

        if (SEGGER_RTT_GetKey() >= 0) {
            fk_log_debugging("key");
        }
    }
}

} // namespace fk
