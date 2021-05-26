#include "tasks/tasks.h"

namespace fk {

os_task_t idle_task;
os_task_t scheduler_task;
os_task_t display_task;
os_task_t network_task;
os_task_t gps_task;
os_task_t debug_task;
os_task_t readings_task;
os_task_t worker_tasks[NumberOfWorkerTasks];

os_task_t *all_tasks[6 + NumberOfWorkerTasks + 1] = {
    &scheduler_task,
    &display_task,
    &network_task,
    &gps_task,
    &debug_task,
    &readings_task,
    &worker_tasks[0],
    &worker_tasks[1],
    nullptr,
};

bool fk_can_start_task(os_task_t *task) {
    auto status = os_task_get_status(task);
    return status == OS_TASK_STATUS_SUSPENDED || status == OS_TASK_STATUS_FINISHED || status == OS_TASK_STATUS_PANIC || status == OS_TASK_STATUS_ABORTED;
}

bool fk_start_task_if_necessary(os_task_t *task) {
    if (fk_task_stop_requested(nullptr)) {
        return false;
    }
    if (!fk_can_start_task(task)) {
        return false;
    }
    os_task_start(task);
    return true;
}

os_task_t **fk_tasks_all() {
    return all_tasks;
}

bool fk_task_stop_requested(uint32_t *checked) {
    if (checked != nullptr) {
        if (*checked > 0 && fk_uptime() < *checked) {
            return false;
        }

        *checked = fk_uptime() + 100;
    }

    uint32_t signal = 0;
    if (os_signal_check(&signal) == OSS_SUCCESS) {
        if (signal > 0) {
            return true;
        }
    }
    return false;
}

} // namespace fk
