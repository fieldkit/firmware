/**
 * \file
 *
 * \brief Quad SPI related functionality implementation.
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

#include "hal_qspi_sync.h"
#include <utils_assert.h>
#include <utils.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Driver version
 */
#define QSPI_SYNC_DRIVER_VERSION 0x00000001u

int32_t qspi_sync_init(struct qspi_sync_descriptor *qspi, void *const hw)
{
	ASSERT(qspi && hw);

	return _qspi_sync_init(&qspi->dev, hw);
}

int32_t qspi_sync_deinit(struct qspi_sync_descriptor *qspi)
{
	ASSERT(qspi);

	return _qspi_sync_deinit(&qspi->dev);
}

int32_t qspi_sync_enable(struct qspi_sync_descriptor *qspi)
{
	ASSERT(qspi);

	return _qspi_sync_enable(&qspi->dev);
}

int32_t qspi_sync_disable(struct qspi_sync_descriptor *qspi)
{
	ASSERT(qspi);

	return _qspi_sync_disable(&qspi->dev);
}

int32_t qspi_sync_serial_run_command(struct qspi_sync_descriptor *qspi, const struct _qspi_command *cmd)
{
	ASSERT(qspi && cmd);

	return _qspi_sync_serial_run_command(&qspi->dev, cmd);
}

uint32_t qspi_sync_get_version(void)
{
	return QSPI_SYNC_DRIVER_VERSION;
}

#ifdef __cplusplus
}
#endif
