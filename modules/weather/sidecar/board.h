#pragma once

#include "sidecar.h"

enum system_sleepmode {
    SYSTEM_SLEEPMODE_IDLE_0,
    SYSTEM_SLEEPMODE_IDLE_1,
    SYSTEM_SLEEPMODE_IDLE_2,
    SYSTEM_SLEEPMODE_STANDBY,
};

int32_t board_initialize();

int32_t board_timer_setup(struct timer_task *const timer_task, uint32_t interval, timer_cb_t cb);

int32_t board_eeprom_i2c_enable();

int32_t board_eeprom_i2c_disable();

int32_t board_sensors_i2c_enable();

int32_t board_sensors_i2c_disable();

uint32_t board_system_time_get();
