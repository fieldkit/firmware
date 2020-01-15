#pragma once

#include <os.h>

#include "common.h"
#include "platform.h"
#include "config.h"

namespace fk {

void task_handler_idle(void *params);
void task_handler_scheduler(void *params);
void task_handler_display(void *params);
void task_handler_network(void *params);
void task_handler_gps(void *params);
void task_handler_worker(void *params);

extern os_task_t idle_task;
extern os_task_t scheduler_task;
extern os_task_t display_task;
extern os_task_t network_task;
extern os_task_t gps_task;
extern os_task_t worker_tasks[NumberOfWorkerTasks];

bool fk_can_start_task(os_task_t *task);

bool fk_start_task_if_necessary(os_task_t *task);

}
