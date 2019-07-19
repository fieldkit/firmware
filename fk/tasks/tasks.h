#pragma once

#include <os.h>

#include "common.h"
#include "platform.h"

namespace fk {

void task_handler_idle(void *params);
void task_handler_scheduler(void *params);
void task_handler_display(void *params);
void task_handler_network(void *params);
void task_handler_gps(void *params);
void task_handler_readings(void *params);
void task_handler_data(void *params);

extern os_task_t idle_task;
extern os_task_t scheduler_task;
extern os_task_t display_task;
extern os_task_t network_task;
extern os_task_t gps_task;
extern os_task_t readings_task;
extern os_task_t data_task;

}
