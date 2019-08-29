#include "tasks/tasks.h"

namespace fk {

os_task_t idle_task;
os_task_t scheduler_task;
os_task_t display_task;
os_task_t network_task;
os_task_t gps_task;
os_task_t readings_task;
os_task_t worker_tasks[NumberOfWorkerTasks];

}
