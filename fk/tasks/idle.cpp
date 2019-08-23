#include "tasks/tasks.h"
#include "config.h"
#include "debugging.h"

namespace fk {

FK_DECLARE_LOGGER("idle");

static void log_task_info() {
    __disable_irq();
    for (auto iter = osg.tasks; iter != nullptr; iter = iter->np) {
        logverbose("task: %10s %-30s %" PRIu32 "us", iter->name, os_task_status_str(iter->status), iter->runtime);
    }
    __enable_irq();
}

void task_handler_idle(void *params) {
    while (true) {
        fk_delay(FiveSecondsMs);

        if (SEGGER_RTT_HasData(0)) {
            char buffer[8] = { 0 };
            while (SEGGER_RTT_Read(0, buffer, sizeof(buffer)) > 0);

            logwarn("detected console");
            fk_debug_set_console_attached();
        }

        log_task_info();
    }
}

}
