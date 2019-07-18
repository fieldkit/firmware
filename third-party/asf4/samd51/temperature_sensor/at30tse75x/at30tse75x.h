/**
 * \file
 *
 * \brief AT30TSE75X temperature declaration
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

#ifndef _AT30TSE75X_H_INCLUDED
#define _AT30TSE75X_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <temperature_sensor.h>

/**
 * \brief AT30TSE75x temperature sensor
 */
struct at30tse75x {
	/** The parent abstract temperature sensor */
	struct temperature_sensor parent;
	/** Resolution used by internal temperature converter */
	uint8_t resolution;
};

/**
 * \brief Construct at30tse75x temperature sensor
 *
 * \param[in] me The pointer to temperature sensor to initialize
 * \param[in] io The pointer to instance of interface to actual sensor
 * \param[in] resolution The resolution of temperature sensor
 *
 * \return pointer to initialized sensor
 */
struct temperature_sensor *at30tse75x_construct(struct temperature_sensor *const me, void *const io,
                                                const uint8_t resolution);

/**
 * \brief Read temperature from the given sensor
 *
 * \param[in] me The pointer to temperature sensor to read temperature from
 *
 * \return temperature
 */
float at30tse75x_read(const struct temperature_sensor *const me);

#ifdef __cplusplus
}
#endif

#endif /* _AT30TSE75X_H_INCLUDED */
