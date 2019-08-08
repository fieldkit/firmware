#pragma once

#include "sidecar.h"

int32_t board_initialize();

int32_t board_timer_setup(struct timer_task *const timer_task, uint32_t interval, timer_cb_t cb);


