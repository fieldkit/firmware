#include "tasks/tasks.h"

namespace fk {

os_task_t idle_task;
os_task_t scheduler_task;
os_task_t display_task;
os_task_t network_task;
os_task_t gps_task;
os_task_t readings_task;
os_task_t worker_tasks[NumberOfWorkerTasks];

bool fk_can_start_task(os_task_t *task) {
    auto status = os_task_get_status(task);
    return status == OS_TASK_STATUS_SUSPENDED || status == OS_TASK_STATUS_FINISHED || status == OS_TASK_STATUS_PANIC || status == OS_TASK_STATUS_ABORTED;
}

bool fk_start_task_if_necessary(os_task_t *task) {
    if (!fk_can_start_task(task)) {
        return false;
    }
    os_task_start(task);
    return true;
}

}
