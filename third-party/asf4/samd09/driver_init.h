/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef DRIVER_INIT_INCLUDED
#define DRIVER_INIT_INCLUDED

#include "atmel_start_pins.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <hal_atomic.h>
#include <hal_delay.h>
#include <hal_gpio.h>
#include <hal_init.h>
#include <hal_io.h>
#include <hal_sleep.h>

#include <hal_crc_sync.h>
#include <hal_ext_irq.h>

#include <hal_flash.h>

#include <hal_i2c_m_sync.h>

#include <hal_i2c_m_sync.h>

#include <hal_delay.h>

#include <hal_calendar.h>
#include <hal_timer.h>
#include <hpl_tc_base.h>

#include <hal_evsys.h>

#include <hal_wdt.h>

extern struct crc_sync_descriptor CRC_0;

extern struct flash_descriptor FLASH_0;

extern struct i2c_m_sync_desc I2C_0;

extern struct i2c_m_sync_desc I2C_1;

extern struct calendar_descriptor CALENDAR_0;
extern struct timer_descriptor    TIMER_0;

extern struct wdt_descriptor WDT_0;

void FLASH_0_init(void);
void FLASH_0_CLOCK_init(void);

void I2C_0_CLOCK_init(void);
void I2C_0_init(void);
void I2C_0_PORT_init(void);

void I2C_1_CLOCK_init(void);
void I2C_1_init(void);
void I2C_1_PORT_init(void);

void delay_driver_init(void);

void CALENDAR_0_CLOCK_init(void);
void CALENDAR_0_init(void);

void WDT_0_CLOCK_init(void);
void WDT_0_init(void);

void TIMER_0_init(void);

/**
 * \brief Perform system initialization, initialize pins and clocks for
 * peripherals
 */
void system_init(void);

#ifdef __cplusplus
}
#endif
#endif // DRIVER_INIT_INCLUDED
