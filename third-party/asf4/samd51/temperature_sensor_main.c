/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file or main.c
 * to avoid loosing it when reconfiguring.
 */
#include "atmel_start.h"
#include "temperature_sensor_main.h"

static struct at30tse75x TEMPERATURE_SENSOR_0_descr;

struct temperature_sensor *TEMPERATURE_SENSOR_0;

/**
 * \brief Initialize Temperature Sensors
 */
void temperature_sensors_init(void)
{

	i2c_m_sync_enable(&I2C_INSTANCE);
	TEMPERATURE_SENSOR_0
	    = at30tse75x_construct(&TEMPERATURE_SENSOR_0_descr.parent, &I2C_INSTANCE, CONF_TEMPERATURE_SENSOR_0_RESOLUTION);
}
