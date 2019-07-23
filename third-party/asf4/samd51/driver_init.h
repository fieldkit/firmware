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

#include <hal_aes_sync.h>

#include <hal_crc_sync.h>
#include <hal_ext_irq.h>

#include <hal_evsys.h>

#include <hal_sha_sync.h>

#include <hal_flash.h>

#include <hal_qspi_sync.h>

#include <hal_calendar.h>

#include <hal_i2c_m_sync.h>

#include <hal_i2c_m_sync.h>

#include <hal_i2c_m_sync.h>
#include <hal_spi_m_sync.h>
#include <hal_spi_m_sync.h>
#include <hal_spi_m_sync.h>

#include <hal_delay.h>
#include <hal_timer.h>
#include <hpl_tc_base.h>

#include <hal_rand_sync.h>

#include <hal_wdt.h>

extern struct aes_sync_descriptor CRYPTOGRAPHY_0;
extern struct crc_sync_descriptor CRC_0;
extern struct sha_sync_descriptor HASH_ALGORITHM_0;

extern struct flash_descriptor FLASH_0;

extern struct qspi_sync_descriptor QUAD_SPI_0;

extern struct calendar_descriptor CALENDAR_0;

extern struct i2c_m_sync_desc I2C_0;

extern struct i2c_m_sync_desc I2C_1;

extern struct i2c_m_sync_desc       I2C_2;
extern struct spi_m_sync_descriptor SPI_0;
extern struct spi_m_sync_descriptor SPI_1;
extern struct spi_m_sync_descriptor SPI_2;

extern struct timer_descriptor TIMER_0;

extern struct rand_sync_desc RAND_0;

extern struct wdt_descriptor WDT_0;

void FLASH_0_init(void);
void FLASH_0_CLOCK_init(void);

void QUAD_SPI_0_PORT_init(void);
void QUAD_SPI_0_CLOCK_init(void);
void QUAD_SPI_0_init(void);

void CALENDAR_0_CLOCK_init(void);
void CALENDAR_0_init(void);

void I2C_0_CLOCK_init(void);
void I2C_0_init(void);
void I2C_0_PORT_init(void);

void I2C_1_CLOCK_init(void);
void I2C_1_init(void);
void I2C_1_PORT_init(void);

void I2C_2_CLOCK_init(void);
void I2C_2_init(void);
void I2C_2_PORT_init(void);

void SPI_0_PORT_init(void);
void SPI_0_CLOCK_init(void);
void SPI_0_init(void);

void SPI_1_PORT_init(void);
void SPI_1_CLOCK_init(void);
void SPI_1_init(void);

void SPI_2_PORT_init(void);
void SPI_2_CLOCK_init(void);
void SPI_2_init(void);

void delay_driver_init(void);

void RAND_0_CLOCK_init(void);
void RAND_0_init(void);

void WDT_0_CLOCK_init(void);
void WDT_0_init(void);

/**
 * \brief Perform system initialization, initialize pins and clocks for
 * peripherals
 */
void system_init(void);

#ifdef __cplusplus
}
#endif
#endif // DRIVER_INIT_INCLUDED
