#pragma once

#include <samd51_common.h>
#include <string.h>
#include <cortex.h>
#include <loading.h>
#include <SEGGER_RTT.h>

void memory_initialize(void);

void board_minimal_initialize(void);

void board_initialize();

uint32_t millis();

void delay(uint32_t ms);

#include "flash.h"
#include "qspi.h"
#include "utilities.h"
