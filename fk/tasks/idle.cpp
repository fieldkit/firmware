#include "tasks/tasks.h"
#include "status_logging.h"
#include "config.h"

#include <Arduino.h>

namespace fk {

void task_handler_idle(void *params) {
    while (true) {
        fk_delay(FiveSecondsMs);
        // fk_status_log();
    }
}

}
