/**
 * \file
 *
 * \brief AT30TSE75X driver
 *
 * Copyright (c) 2016-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include <at30tse75x.h>
#include <hal_i2c_m_sync.h>
#include <temperature_sensor.h>
#include <at30tse75x_config.h>

/** Register addresses of at30tse75x */
#define AT30TSE_TEMPERATURE_REG 0
#define AT30TSE_CONFIGURATION_REG 1

/** Register types of at30tse75x */
#define AT30TSE_NON_VOLATILE_REG_TYPE 0

/** Address of temperature sensor */
#define AT30TSE_SENSOR_ADDRESS 0x4F

/** Offset of resolution bit-field */
#define AT30TSE_CONFIGURATION_REG_RESOLUTION_BF_OFFSET 13

/** AT30TSE75x temperature sensor's interface */
static const struct temperature_sensor_interface at30tse75x_interface = {at30tse75x_read};

/**
 * \brief Construct at30tse75x temperature sensor
 */
struct temperature_sensor *at30tse75x_construct(struct temperature_sensor *const me, void *const io,
                                                const uint8_t resolution)
{
	uint8_t                 data[3];
	struct i2c_m_sync_desc *descr;
	struct at30tse75x *     at30 = (struct at30tse75x *)me;

	temperature_sensor_construct(me, io, &at30tse75x_interface);
	descr            = (struct i2c_m_sync_desc *)(me->io);
	at30->resolution = resolution;

	data[0] = AT30TSE_CONFIGURATION_REG | AT30TSE_NON_VOLATILE_REG_TYPE;
	data[1] = (uint16_t)(at30->resolution) << (AT30TSE_CONFIGURATION_REG_RESOLUTION_BF_OFFSET - 8);
	data[2] = 0;
	i2c_m_sync_set_slaveaddr(descr, AT30TSE_SENSOR_ADDRESS, I2C_M_SEVEN);
	io_write(&descr->io, data, 3);

	return me;
}

/**
 * \brief Read temperature from the given sensor
 */
float at30tse75x_read(const struct temperature_sensor *const me)
{
	uint8_t                 buffer[2];
	struct at30tse75x *     at30  = (struct at30tse75x *)me;
	struct i2c_m_sync_desc *descr = (struct i2c_m_sync_desc *)(me->io);
	uint16_t                data;
	int8_t                  sign = 1;

	/* Read the 16-bit temperature register. */
	buffer[0] = AT30TSE_TEMPERATURE_REG | AT30TSE_NON_VOLATILE_REG_TYPE;
	buffer[1] = 0;
	i2c_m_sync_set_slaveaddr(descr, AT30TSE_SENSOR_ADDRESS, I2C_M_SEVEN);
	io_write(&descr->io, buffer, 1);
	io_read(&descr->io, buffer, 2);

	data = (buffer[0] << 8) | buffer[1];
	sign -= (bool)(data & (1 << 15)) << 1;
	data &= ~(1 << 15);

	/* Convert to temperature */
	data = data >> (7 - at30->resolution);
	return (float)data * sign * (0.5 / (1 << at30->resolution));
}
