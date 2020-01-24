#include "tasks/tasks.h"
#include "status_logging.h"
#include "config.h"
#include "memory.h"

namespace fk {

void task_handler_idle(void *params) {
    auto counter = 0u;

    while (true) {
        fk_delay(FiveSecondsMs);
        fk_status_log();

        if (counter++ == 10) {
            fk_standard_page_log();
            counter = 0;
        }

        if (SEGGER_RTT_GetKey() >= 0) {
            while (SEGGER_RTT_GetKey() >= 0) {
                // Read all incoming data.
            }

            if (false) {
                fk_logging_dump_buffer();
            }
        }
    }
}

} // namespace fk
