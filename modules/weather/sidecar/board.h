#pragma once

#include "sidecar.h"

#define MCP2803_RAIN_I2C_ADDRESS        (0x20 + 0x1)
#define MCP2803_RAIN_IODIR              (0b11111111)

#define MCP2803_WIND_I2C_ADDRESS        (0x20 + 0x3)
#define MCP2803_WIND_IODIR              (0b11111111)

#define MCP2803_CONTROL_I2C_ADDRESS     (0x20 + 0x2)
#define MCP2803_CONTROL_IODIR           (0b10001000)

/**
 * Control:
 * CCLR-CLEAR
 * RCLK-UPDATE-OUTPUT
 * OUTPUT-ENABLE
 */

int32_t board_initialize();
