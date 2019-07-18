/**
 * \file
 *
 * \brief SAM Peripheral Access Controller
 *
 * Copyright (c) 2015-2018 Microchip Technology Inc. and its subsidiaries.
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
#include <compiler.h>
#include <utils_assert.h>
#include <hpl_pac.h>

#define CONF_PAC0_PERIPH_MIN_Pos 0
#define CONF_PAC0_PERIPH_MAX_Pos 5
#define CONF_PAC1_PERIPH_MIN_Pos 0
#define CONF_PAC1_PERIPH_MAX_Pos 5
#define CONF_PAC2_PERIPH_MIN_Pos 0
#define CONF_PAC2_PERIPH_MAX_Pos 10
static int32_t _pac_get_pac1_instance(const void *const module)
{
	if ((uint32_t)module == (uint32_t)DSU) {
		return 1;
	} else if ((uint32_t)module == (uint32_t)NVMCTRL) {
		return 2;
	} else if ((uint32_t)module == (uint32_t)PORT) {
		return 3;
	}
#if defined(DMAC)
	else if ((uint32_t)module == (uint32_t)DMAC) {
		return 4;
	}
#endif
#if defined(USB)
	else if ((uint32_t)module == (uint32_t)USB) {
		return 5;
	}
#endif
#if defined(MTB)
	else if ((uint32_t)module == (uint32_t)MTB) {
		return 6;
	}
#endif
	return ERR_INVALID_DATA;
}

/**
 * \brief Enable write protect for the given hardware module
 */
int32_t _periph_lock(const void *const module)
{
	ASSERT(module);
	ASSERT((((uint32_t)module) > (uint32_t)PAC0));

	int32_t timeout;
	int32_t peripheral = (((uint32_t)module & 0x0000ff00) >> 10) - 1;

	if ((uint32_t)module < (uint32_t)PAC1) {
		if ((peripheral >= CONF_PAC0_PERIPH_MIN_Pos) && (peripheral <= CONF_PAC0_PERIPH_MAX_Pos)) {
			timeout = 1000;
			hri_pac_set_WP_WP_bf(PAC0, 1 << peripheral);
			while (hri_pac_get_WP_WP_bf(PAC0, 1 << peripheral) && timeout--) {
			}
		} else {
			return ERR_INVALID_DATA;
		}
	} else if (((uint32_t)module) < ((uint32_t)PAC2)) {
		peripheral = _pac_get_pac1_instance(module);
		if (peripheral != ERR_INVALID_DATA) {
			peripheral -= 1;
			timeout = 1000;
			hri_pac_set_WP_WP_bf(PAC1, 1 << peripheral);
			while (hri_pac_get_WP_WP_bf(PAC1, 1 << peripheral) && timeout--) {
			}
		} else {
			return ERR_INVALID_DATA;
		}
	} else {
		if ((peripheral >= CONF_PAC2_PERIPH_MIN_Pos) && (peripheral <= CONF_PAC2_PERIPH_MAX_Pos)) {
			timeout = 1000;
			hri_pac_set_WP_WP_bf(PAC2, 1 << peripheral);
			while (hri_pac_get_WP_WP_bf(PAC2, 1 << peripheral) && timeout--) {
			}
		} else {
			return ERR_INVALID_DATA;
		}
	}

	if (timeout < 0) {
		return ERR_TIMEOUT;
	}

	return ERR_NONE;
}

/**
 * \brief Disable write protect for the given hardware module
 */
int32_t _periph_unlock(const void *const module)
{
	ASSERT(module);
	ASSERT((((uint32_t)module) > (uint32_t)PAC0));

	int32_t timeout;

	int32_t peripheral = (((uint32_t)module & 0x0000ff00) >> 10) - 1;

	if ((uint32_t)module < (uint32_t)PAC1) {
		if ((peripheral >= CONF_PAC0_PERIPH_MIN_Pos) && (peripheral <= CONF_PAC0_PERIPH_MAX_Pos)) {
			timeout = 1000;
			hri_pac_clear_WP_WP_bf(PAC0, 1 << peripheral);
			while (hri_pac_get_WP_WP_bf(PAC0, 1 << peripheral) && timeout--) {
			}
		} else {
			return ERR_INVALID_DATA;
		}
	} else if (((uint32_t)module) < ((uint32_t)PAC2)) {
		peripheral = _pac_get_pac1_instance(module);
		if (peripheral != ERR_INVALID_DATA) {
			peripheral -= 1;
			timeout = 1000;
			hri_pac_clear_WP_WP_bf(PAC1, 1 << peripheral);
			while (hri_pac_get_WP_WP_bf(PAC1, 1 << peripheral) && timeout--) {
			}
		} else {
			return ERR_INVALID_DATA;
		}
	} else {
		if ((peripheral >= CONF_PAC2_PERIPH_MIN_Pos) && (peripheral <= CONF_PAC2_PERIPH_MAX_Pos)) {
			timeout = 1000;
			hri_pac_clear_WP_WP_bf(PAC2, 1 << peripheral);
			while (hri_pac_get_WP_WP_bf(PAC2, 1 << peripheral) && timeout--) {
			}
		} else {
			return ERR_INVALID_DATA;
		}
	}

	if (timeout < 0) {
		return ERR_TIMEOUT;
	}

	return ERR_NONE;
}

/**
 * \brief Get write protect for the given hardware module
 */
int32_t _periph_get_lock_state(const void *const module, bool *const state)
{
	ASSERT(module);
	ASSERT((((uint32_t)module) > (uint32_t)PAC0));

	int32_t peripheral = (((uint32_t)module & 0x0000ff00) >> 10) - 1;

	if ((uint32_t)module < (uint32_t)PAC1) {
		if ((peripheral >= CONF_PAC0_PERIPH_MIN_Pos) && (peripheral <= CONF_PAC0_PERIPH_MAX_Pos)) {
			*state = hri_pac_get_WP_WP_bf(PAC0, 1 << peripheral);
		} else {
			return ERR_INVALID_DATA;
		}
	} else if (((uint32_t)module) < ((uint32_t)PAC2)) {
		peripheral = _pac_get_pac1_instance(module);
		if (peripheral != ERR_INVALID_DATA) {
			peripheral -= 1;
			*state = hri_pac_get_WP_WP_bf(PAC1, 1 << peripheral);
		} else {
			return ERR_INVALID_DATA;
		}
	} else {
		if ((peripheral >= CONF_PAC2_PERIPH_MIN_Pos) && (peripheral <= CONF_PAC2_PERIPH_MAX_Pos)) {
			*state = hri_pac_get_WP_WP_bf(PAC2, 1 << peripheral);
		} else {
			return ERR_INVALID_DATA;
		}
	}
	return ERR_NONE;
}
