#pragma once

#include <os.h>

#include "common.h"
#include "platform.h"
#include "config.h"
#include "task_stack.h"

namespace fk {

struct fk_task_data_t {
    task_stack log_stack{ 10 };
};

void task_handler_idle(void *params);
void task_handler_scheduler(void *params);
void task_handler_display(void *params);
void task_handler_network(void *params);
void task_handler_worker(void *params);

extern os_task_t idle_task;
extern os_task_t scheduler_task;
extern os_task_t display_task;
extern os_task_t network_task;
extern os_task_t worker_tasks[NumberOfWorkerTasks];
extern fk_task_data_t task_data[4 + NumberOfWorkerTasks];

bool fk_can_start_task(os_task_t *task);

bool fk_start_task_if_necessary(os_task_t *task);

bool fk_task_stop_requested(uint32_t *checked);

os_task_t **fk_tasks_all();

}
