#include "tasks/tasks.h"
#include "status_logging.h"
#include "config.h"

#include <Arduino.h>

namespace fk {

void task_handler_idle(void *params) {
    while (true) {
        fk_delay(FiveSecondsMs);
        fk_status_log();

        if (SEGGER_RTT_GetKey() >= 0) {
            while (SEGGER_RTT_GetKey() >= 0) {
                // Read all incoming data.
            }

            fk_logging_dump_buffer();
        }
    }
}

} // namespace fk
