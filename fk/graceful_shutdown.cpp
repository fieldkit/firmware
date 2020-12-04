#include "graceful_shutdown.h"
#include "logging.h"
#include "tasks/tasks.h"

namespace fk {

FK_DECLARE_LOGGER("system");

bool fk_graceful_shutdown() {
    auto all_tasks = fk_tasks_all();
    auto self = os_task_self();

    loginfo("graceful shutdown");

    for (auto iter = all_tasks; *iter != nullptr; iter++) {
        if (*iter != self && *iter != &display_task) {
            if (os_task_is_running(*iter)) {
                loginfo("stop %s", (*iter)->name);
                os_signal(*iter, 9);
            }
        }
    }

    auto give_up = fk_uptime() + FiveSecondsMs;
    while (fk_uptime() < give_up) {
        fk_delay(100);

        auto running = false;
        for (auto iter = all_tasks; *iter != nullptr; iter++) {
            if (*iter != self && *iter != &display_task) {
                if (os_task_is_running(*iter)) {
                    loginfo("waiting on %s", (*iter)->name);
                    running = true;
                }
            }
        }

        if (!running) {
            break;
        }
    }

    auto gave_up = fk_uptime() > give_up;
    if (gave_up) {
        loginfo("still have procs running, gave up");
    }

    loginfo("graceful shutdown done, flushing");

    fk_logs_flush();

    return true;
}

} // namespace fk
