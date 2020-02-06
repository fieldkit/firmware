/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hal_init.h>
#include <hpl_gclk_base.h>
#include <hpl_pm_base.h>

struct crc_sync_descriptor CRC_0;
struct timer_descriptor    TIMER_0;

struct flash_descriptor FLASH_0;

struct i2c_m_sync_desc I2C_0_m;

struct i2c_s_async_descriptor I2C_0_s;

uint8_t SERCOM0_i2c_s_buffer[SERCOM0_I2CS_BUFFER_SIZE];

struct i2c_m_sync_desc I2C_1;

struct calendar_descriptor CALENDAR_0;

struct wdt_descriptor WDT_0;

/**
 * \brief CRC initialization function
 *
 * Enables CRC peripheral, clocks and initializes CRC driver
 */
void CRC_0_initialize(void)
{
	_pm_enable_bus_clock(PM_BUS_AHB, DSU);
	_pm_enable_bus_clock(PM_BUS_APBB, PAC1);
	crc_sync_init(&CRC_0, DSU);
}

void EXTERNAL_IRQ_0_initialize(void)
{
    _gclk_enable_channel(EIC_GCLK_ID, CONF_GCLK_EIC_SRC);

    // Set pin direction to input
    gpio_set_pin_direction(PA25, GPIO_DIRECTION_IN);

    gpio_set_pin_pull_mode(PA25,
                           // <y> Pull configuration
                           // <id> pad_pull_config
                           // <GPIO_PULL_OFF"> Off
                           // <GPIO_PULL_UP"> Pull-up
                           // <GPIO_PULL_DOWN"> Pull-down
                           GPIO_PULL_OFF);

    gpio_set_pin_function(PA25, PINMUX_PA25A_EIC_EXTINT5);

    ext_irq_init();
}

void FLASH_0_CLOCK_initialize(void)
{

	_pm_enable_bus_clock(PM_BUS_APBB, NVMCTRL);
}

void FLASH_0_initialize(void)
{
	FLASH_0_CLOCK_initialize();
	flash_init(&FLASH_0, NVMCTRL);
}

void I2C_0_PORT_initialize(void)
{

	gpio_set_pin_pull_mode(PA14,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA14, PINMUX_PA14C_SERCOM0_PAD0);

	gpio_set_pin_pull_mode(PA15,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA15, PINMUX_PA15C_SERCOM0_PAD1);
}

void I2C_0_CLOCK_initialize(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM0);
	_gclk_enable_channel(SERCOM0_GCLK_ID_CORE, CONF_GCLK_SERCOM0_CORE_SRC);
	_gclk_enable_channel(SERCOM0_GCLK_ID_SLOW, CONF_GCLK_SERCOM0_SLOW_SRC);
}

void I2C_0_master_initialize(void)
{
	I2C_0_CLOCK_initialize();
	i2c_m_sync_init(&I2C_0_m, SERCOM0);
	I2C_0_PORT_initialize();
}

void I2C_0_async_subordinate_initialize(void)
{
	I2C_0_CLOCK_initialize();
	i2c_s_async_init(&I2C_0_s, SERCOM0, SERCOM0_i2c_s_buffer, SERCOM0_I2CS_BUFFER_SIZE);
	I2C_0_PORT_initialize();
}

void I2C_1_PORT_initialize(void)
{

	gpio_set_pin_pull_mode(PA22,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA22, PINMUX_PA22C_SERCOM1_PAD0);

	gpio_set_pin_pull_mode(PA23,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA23, PINMUX_PA23C_SERCOM1_PAD1);
}

void I2C_1_CLOCK_initialize(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM1);
	_gclk_enable_channel(SERCOM1_GCLK_ID_CORE, CONF_GCLK_SERCOM1_CORE_SRC);
	_gclk_enable_channel(SERCOM1_GCLK_ID_SLOW, CONF_GCLK_SERCOM1_SLOW_SRC);
}

void I2C_1_initialize(void)
{
	I2C_1_CLOCK_initialize();
	i2c_m_sync_init(&I2C_1, SERCOM1);
	I2C_1_PORT_initialize();
}

void delay_driver_initialize(void)
{
	delay_init(SysTick);
}

void CALENDAR_0_CLOCK_initialize(void)
{
	_pm_enable_bus_clock(PM_BUS_APBA, RTC);
	_gclk_enable_channel(RTC_GCLK_ID, CONF_GCLK_RTC_SRC);
}

void CALENDAR_0_initialize(void)
{
	CALENDAR_0_CLOCK_initialize();
	calendar_init(&CALENDAR_0, RTC);
}

/**
 * \brief Timer initialization function
 *
 * Enables Timer peripheral, clocks and initializes Timer driver
 */
void TIMER_0_initialize(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, TC1);
	_gclk_enable_channel(TC1_GCLK_ID, CONF_GCLK_TC1_SRC);

	timer_init(&TIMER_0, TC1, _tc_get_timer());
}

void EVENT_SYSTEM_0_initialize(void)
{

	hri_pm_set_APBCMASK_EVSYS_bit(PM);

	event_system_init();
}

void WDT_0_CLOCK_initialize(void)
{
	_pm_enable_bus_clock(PM_BUS_APBA, WDT);
	_gclk_enable_channel(WDT_GCLK_ID, CONF_GCLK_WDT_SRC);
}

void WDT_0_initialize(void)
{
	WDT_0_CLOCK_initialize();
	wdt_init(&WDT_0, WDT);
}

void system_initialize(void)
{
	init_mcu();

	// CRC_0_initialize();
	EXTERNAL_IRQ_0_initialize();

	// FLASH_0_initialize();

	delay_driver_initialize();

	// CALENDAR_0_initialize();

	TIMER_0_initialize();

	// EVENT_SYSTEM_0_initialize();

	WDT_0_initialize();
}
