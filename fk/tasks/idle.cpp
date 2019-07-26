#include "tasks/tasks.h"
#include "config.h"

namespace fk {

FK_DECLARE_LOGGER("idle");

static void log_task_info() {
    __disable_irq();
    for (auto iter = osg.tasks; iter != nullptr; iter = iter->np) {
        logverbose("task: %10s %-30s %dus", iter->name, os_task_status_str(iter->status), iter->runtime);
    }
    __enable_irq();
}

void task_handler_idle(void *params) {
    while (true) {
        fk_delay(FiveSecondsMs);

        log_task_info();
    }
}

}
