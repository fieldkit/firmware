#pragma once

#include "sidecar.h"

typedef struct board_register_map_t {
    uint8_t *registers;
    uint16_t size;
    uint16_t position;
    void (*before_read)(void *ptr);
} board_register_map_t;

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

int32_t board_subordinate_initialize(board_register_map_t *regmap);
