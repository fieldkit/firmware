#pragma once

#include <os.h>

#include "common.h"
#include "platform.h"

namespace fk {

/**
 * Number of worker tasks to allow.
 */
constexpr size_t NumberOfWorkerTasks = 2;

void task_handler_idle(void *params);
void task_handler_scheduler(void *params);
void task_handler_display(void *params);
void task_handler_network(void *params);
void task_handler_gps(void *params);
void task_handler_readings(void *params);
void task_handler_worker(void *params);

extern os_task_t idle_task;
extern os_task_t scheduler_task;
extern os_task_t display_task;
extern os_task_t network_task;
extern os_task_t gps_task;
extern os_task_t readings_task;
extern os_task_t worker_tasks[NumberOfWorkerTasks];

}
