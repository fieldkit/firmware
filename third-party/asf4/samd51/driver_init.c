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

struct aes_sync_descriptor   CRYPTOGRAPHY_0;
struct crc_sync_descriptor   CRC_0;
struct sha_sync_descriptor   HASH_ALGORITHM_0;
struct spi_m_sync_descriptor SPI_0;
struct spi_m_sync_descriptor SPI_1;
struct spi_m_sync_descriptor SPI_2;
struct timer_descriptor      TIMER_0;

struct flash_descriptor FLASH_0;

struct qspi_sync_descriptor QUAD_SPI_0;

struct calendar_descriptor CALENDAR_0;

struct i2c_m_sync_desc I2C_0;

struct i2c_m_sync_desc I2C_1;

struct i2c_m_sync_desc I2C_2;

struct rand_sync_desc RAND_0;

struct wdt_descriptor WDT_0;

/**
 * \brief AES initialization function
 *
 * Enables AES peripheral, clocks and initializes AES driver
 */
void CRYPTOGRAPHY_0_init(void)
{
	hri_mclk_set_APBCMASK_AES_bit(MCLK);
	aes_sync_init(&CRYPTOGRAPHY_0, AES);
}

/**
 * \brief CRC initialization function
 *
 * Enables CRC peripheral, clocks and initializes CRC driver
 */
void CRC_0_init(void)
{
	hri_mclk_set_APBBMASK_DSU_bit(MCLK);
	crc_sync_init(&CRC_0, DSU);
}

void EXTERNAL_IRQ_0_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, EIC_GCLK_ID, CONF_GCLK_EIC_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_mclk_set_APBAMASK_EIC_bit(MCLK);

	ext_irq_init();
}

void EVENT_SYSTEM_0_init(void)
{

	hri_mclk_set_APBBMASK_EVSYS_bit(MCLK);

	event_system_init();
}

void HASH_ALGORITHM_0_CLOCK_init(void)
{
	hri_mclk_set_AHBMASK_ICM_bit(MCLK);
	hri_mclk_set_APBCMASK_ICM_bit(MCLK);
}

/**
 * \brief SHA initialization function
 *
 * Enables SHA peripheral, clocks and initializes SHA driver
 */
void HASH_ALGORITHM_0_init(void)
{
	HASH_ALGORITHM_0_CLOCK_init();
	sha_sync_init(&HASH_ALGORITHM_0, ICM);
}

void FLASH_0_CLOCK_init(void)
{

	hri_mclk_set_AHBMASK_NVMCTRL_bit(MCLK);
}

void FLASH_0_init(void)
{
	FLASH_0_CLOCK_init();
	flash_init(&FLASH_0, NVMCTRL);
}

void QUAD_SPI_0_PORT_init(void)
{
}

void QUAD_SPI_0_CLOCK_init(void)
{
	hri_mclk_set_AHBMASK_QSPI_bit(MCLK);
	hri_mclk_set_AHBMASK_QSPI_2X_bit(MCLK);
	hri_mclk_set_APBCMASK_QSPI_bit(MCLK);
}

void QUAD_SPI_0_init(void)
{
	QUAD_SPI_0_CLOCK_init();
	qspi_sync_init(&QUAD_SPI_0, QSPI);
	QUAD_SPI_0_PORT_init();
}

void CALENDAR_0_CLOCK_init(void)
{
	hri_mclk_set_APBAMASK_RTC_bit(MCLK);
}

void CALENDAR_0_init(void)
{
	CALENDAR_0_CLOCK_init();
	calendar_init(&CALENDAR_0, RTC);
}

void I2C_0_PORT_init(void)
{

	gpio_set_pin_pull_mode(PA08,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA08, PINMUX_PA08C_SERCOM0_PAD0);

	gpio_set_pin_pull_mode(PA09,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA09, PINMUX_PA09C_SERCOM0_PAD1);
}

void I2C_0_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM0_GCLK_ID_CORE, CONF_GCLK_SERCOM0_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM0_GCLK_ID_SLOW, CONF_GCLK_SERCOM0_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBAMASK_SERCOM0_bit(MCLK);
}

void I2C_0_init(void)
{
	I2C_0_CLOCK_init();
	i2c_m_sync_init(&I2C_0, SERCOM0);
	I2C_0_PORT_init();
}

void I2C_1_PORT_init(void)
{

	gpio_set_pin_pull_mode(PA16,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA16, PINMUX_PA16C_SERCOM1_PAD0);

	gpio_set_pin_pull_mode(PA17,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA17, PINMUX_PA17C_SERCOM1_PAD1);
}

void I2C_1_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM1_GCLK_ID_CORE, CONF_GCLK_SERCOM1_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM1_GCLK_ID_SLOW, CONF_GCLK_SERCOM1_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBAMASK_SERCOM1_bit(MCLK);
}

void I2C_1_init(void)
{
	I2C_1_CLOCK_init();
	i2c_m_sync_init(&I2C_1, SERCOM1);
	I2C_1_PORT_init();
}

void I2C_2_PORT_init(void)
{

	gpio_set_pin_pull_mode(PA12,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA12, PINMUX_PA12C_SERCOM2_PAD0);

	gpio_set_pin_pull_mode(PA13,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA13, PINMUX_PA13C_SERCOM2_PAD1);
}

void I2C_2_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM2_GCLK_ID_CORE, CONF_GCLK_SERCOM2_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM2_GCLK_ID_SLOW, CONF_GCLK_SERCOM2_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBBMASK_SERCOM2_bit(MCLK);
}

void I2C_2_init(void)
{
	I2C_2_CLOCK_init();
	i2c_m_sync_init(&I2C_2, SERCOM2);
	I2C_2_PORT_init();
}

void SPI_0_PORT_init(void)
{

	gpio_set_pin_level(PC23,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PC23, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PC23, PINMUX_PC23D_SERCOM3_PAD0);

	gpio_set_pin_level(PC22,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PC22, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PC22, PINMUX_PC22D_SERCOM3_PAD1);

	// Set pin direction to input
	gpio_set_pin_direction(PA18, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(PA18,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA18, PINMUX_PA18D_SERCOM3_PAD2);
}

void SPI_0_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM3_GCLK_ID_CORE, CONF_GCLK_SERCOM3_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM3_GCLK_ID_SLOW, CONF_GCLK_SERCOM3_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBBMASK_SERCOM3_bit(MCLK);
}

void SPI_0_init(void)
{
	SPI_0_CLOCK_init();
	spi_m_sync_init(&SPI_0, SERCOM3);
	SPI_0_PORT_init();
}

void SPI_1_PORT_init(void)
{

	gpio_set_pin_level(PB08,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PB08, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PB08, PINMUX_PB08D_SERCOM4_PAD0);

	gpio_set_pin_level(PB09,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PB09, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PB09, PINMUX_PB09D_SERCOM4_PAD1);

	// Set pin direction to input
	gpio_set_pin_direction(PB10, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(PB10,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PB10, PINMUX_PB10D_SERCOM4_PAD2);
}

void SPI_1_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM4_GCLK_ID_CORE, CONF_GCLK_SERCOM4_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM4_GCLK_ID_SLOW, CONF_GCLK_SERCOM4_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBDMASK_SERCOM4_bit(MCLK);
}

void SPI_1_init(void)
{
	SPI_1_CLOCK_init();
	spi_m_sync_init(&SPI_1, SERCOM4);
	SPI_1_PORT_init();
}

void SPI_2_PORT_init(void)
{

	gpio_set_pin_level(PB16,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PB16, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PB16, PINMUX_PB16C_SERCOM5_PAD0);

	gpio_set_pin_level(PB17,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PB17, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PB17, PINMUX_PB17C_SERCOM5_PAD1);

	// Set pin direction to input
	gpio_set_pin_direction(PB18, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(PB18,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PB18, PINMUX_PB18C_SERCOM5_PAD2);
}

void SPI_2_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM5_GCLK_ID_CORE, CONF_GCLK_SERCOM5_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM5_GCLK_ID_SLOW, CONF_GCLK_SERCOM5_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBDMASK_SERCOM5_bit(MCLK);
}

void SPI_2_init(void)
{
	SPI_2_CLOCK_init();
	spi_m_sync_init(&SPI_2, SERCOM5);
	SPI_2_PORT_init();
}

void delay_driver_init(void)
{
	delay_init(SysTick);
}

/**
 * \brief Timer initialization function
 *
 * Enables Timer peripheral, clocks and initializes Timer driver
 */
static void TIMER_0_init(void)
{
	hri_mclk_set_APBAMASK_TC0_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, TC0_GCLK_ID, CONF_GCLK_TC0_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	timer_init(&TIMER_0, TC0, _tc_get_timer());
}

void RAND_0_CLOCK_init(void)
{
	hri_mclk_set_APBCMASK_TRNG_bit(MCLK);
}

void RAND_0_init(void)
{
	RAND_0_CLOCK_init();
	rand_sync_init(&RAND_0, TRNG);
}

void WDT_0_CLOCK_init(void)
{
	hri_mclk_set_APBAMASK_WDT_bit(MCLK);
}

void WDT_0_init(void)
{
	WDT_0_CLOCK_init();
	wdt_init(&WDT_0, WDT);
}

void system_init(void)
{
	init_mcu();

	CRYPTOGRAPHY_0_init();

	CRC_0_init();
	EXTERNAL_IRQ_0_init();

	EVENT_SYSTEM_0_init();

	HASH_ALGORITHM_0_init();

	FLASH_0_init();

	QUAD_SPI_0_init();

	CALENDAR_0_init();

	I2C_0_init();

	I2C_1_init();

	I2C_2_init();

	SPI_0_init();

	SPI_1_init();

	SPI_2_init();

	delay_driver_init();

	TIMER_0_init();
	RAND_0_init();

	WDT_0_init();
}
