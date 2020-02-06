/**
 * \file
 *
 * \brief I2C Slave related functionality implementation.
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
#include <hal_i2c_s_async.h>
#include <hal_atomic.h>
#include <utils_ringbuffer.h>
#include <utils.h>

/**
 * \brief Driver version
 */
#define DRIVER_VERSION 0x00000001u

static int32_t i2c_s_async_write(struct io_descriptor *const io_descr, const uint8_t *const buf, const uint16_t length);
static int32_t i2c_s_async_read(struct io_descriptor *const io_descr, uint8_t *const buf, const uint16_t length);

static void i2c_s_async_tx(struct _i2c_s_async_device *const device);
static void i2c_s_async_byte_received(struct _i2c_s_async_device *const device, const uint8_t data);
static void i2c_s_async_error(struct _i2c_s_async_device *const device);
static void i2c_s_async_stop(struct _i2c_s_async_device *const device);

/**
 * \brief Initialize asynchronous i2c slave interface
 */
int32_t i2c_s_async_init(struct i2c_s_async_descriptor *const descr, void *const hw, uint8_t *const rx_buffer,
                         const uint16_t rx_buffer_length)
{
	int32_t init_status;

	ASSERT(descr && hw && rx_buffer && rx_buffer_length);

	if (ERR_NONE != ringbuffer_init(&descr->rx, rx_buffer, rx_buffer_length)) {
		return ERR_INVALID_ARG;
	}

	init_status = _i2c_s_async_init(&descr->device, hw);
	if (init_status) {
		return init_status;
	}

	descr->io.read  = i2c_s_async_read;
	descr->io.write = i2c_s_async_write;

	descr->device.cb.error   = i2c_s_async_error;
	descr->device.cb.tx      = i2c_s_async_tx;
	descr->device.cb.rx_done = i2c_s_async_byte_received;
	descr->device.cb.stop    = i2c_s_async_stop;

	descr->tx_por           = 0;
	descr->tx_buffer_length = 0;

	return ERR_NONE;
}

/**
 * \brief Deinitialize asynchronous i2c slave interface
 */
int32_t i2c_s_async_deinit(struct i2c_s_async_descriptor *const descr)
{
	ASSERT(descr);

	return _i2c_s_async_deinit(&descr->device);
}

/**
 * \brief Enable I2C slave communication
 */
int32_t i2c_s_async_enable(struct i2c_s_async_descriptor *const descr)
{
	int32_t rc;

	ASSERT(descr);

	rc = _i2c_s_async_enable(&descr->device);
	if (rc == ERR_NONE) {
		_i2c_s_async_set_irq_state(&descr->device, I2C_S_DEVICE_TX, true);
		_i2c_s_async_set_irq_state(&descr->device, I2C_S_DEVICE_RX_COMPLETE, true);
		_i2c_s_async_set_irq_state(&descr->device, I2C_S_DEVICE_ERROR, true);
	}
	return rc;
}

/**
 * \brief Disable I2C slave communication
 */
int32_t i2c_s_async_disable(struct i2c_s_async_descriptor *const descr)
{
	int32_t rc;

	ASSERT(descr);

	rc = _i2c_s_async_disable(&descr->device);
	if (rc == ERR_NONE) {
		_i2c_s_async_set_irq_state(&descr->device, I2C_S_DEVICE_TX, false);
		_i2c_s_async_set_irq_state(&descr->device, I2C_S_DEVICE_RX_COMPLETE, false);
		_i2c_s_async_set_irq_state(&descr->device, I2C_S_DEVICE_ERROR, false);
	}
	return rc;
}

/**
 * \brief Set the device address
 */
int32_t i2c_s_async_set_addr(struct i2c_s_async_descriptor *const descr, const uint16_t address)
{
	ASSERT(descr);

	if (!_i2c_s_async_is_10bit_addressing_on(&descr->device)) {
		return _i2c_s_async_set_address(&descr->device, address & 0x7F);
	}

	return _i2c_s_async_set_address(&descr->device, address);
}

/**
 * \brief Register callback function
 */
int32_t i2c_s_async_register_callback(struct i2c_s_async_descriptor *const descr,
                                      const enum i2c_s_async_callback_type type, i2c_s_async_cb_t func)
{
	ASSERT(descr);

	switch (type) {
	case I2C_S_ERROR:
		descr->cbs.error = func;
		break;
	case I2C_S_TX_PENDING:
		descr->cbs.tx_pending = func;
		break;
	case I2C_S_TX_COMPLETE:
		descr->cbs.tx = func;
		break;
	case I2C_S_RX_COMPLETE:
		descr->cbs.rx = func;
		break;
	case I2C_S_RX_STOP:
		descr->cbs.stop = func;
		break;
	default:
		return ERR_INVALID_DATA;
	}
	return ERR_NONE;
}

/**
 * \brief Retrieve I/O descriptor
 */
int32_t i2c_s_async_get_io_descriptor(struct i2c_s_async_descriptor *const descr, struct io_descriptor **io)
{
	ASSERT(descr && io);

	*io = &descr->io;

	return ERR_NONE;
}

/**
 * \brief Return the number of received bytes in the buffer
 */
int32_t i2c_s_async_get_bytes_received(const struct i2c_s_async_descriptor *const descr)
{
	ASSERT(descr);

	return ringbuffer_num((struct ringbuffer *)&descr->rx);
}

/**
 * \brief Retrieve the number of bytes sent
 */
int32_t i2c_s_async_get_bytes_sent(const struct i2c_s_async_descriptor *const descr)
{
	ASSERT(descr);

	return descr->tx_por;
}

/**
 * \brief Flush received data
 */
int32_t i2c_s_async_flush_rx_buffer(struct i2c_s_async_descriptor *const descr)
{
	ASSERT(descr);

	return ringbuffer_flush(&descr->rx);
}

/**
 * \brief Abort sending
 */
int32_t i2c_s_async_abort_tx(struct i2c_s_async_descriptor *const descr)
{
	ASSERT(descr);

	return _i2c_s_async_abort_transmission(&descr->device);
}

/**
 * \brief Retrieve the current interface status
 */
int32_t i2c_s_async_get_status(const struct i2c_s_async_descriptor *const descr, i2c_s_status_t *const status)
{
	ASSERT(descr && status);

	*status = _i2c_s_async_get_status(&descr->device);

	return ERR_NONE;
}

/**
 * \brief Retrieve the current driver version
 */
uint32_t i2c_s_async_get_version(void)
{
	return DRIVER_VERSION;
}

/**
 * \internal Callback function for data sending
 *
 * \param[in] device The pointer to i2c slave device
 */
static void i2c_s_async_tx(struct _i2c_s_async_device *const device)
{
	struct i2c_s_async_descriptor *descr = CONTAINER_OF(device, struct i2c_s_async_descriptor, device);

	if (!descr->tx_buffer_length) {
		if (descr->cbs.tx_pending) {
			descr->cbs.tx_pending(descr);
		}
	} else if (++descr->tx_por != descr->tx_buffer_length) {
		_i2c_s_async_write_byte(&descr->device, descr->tx_buffer[descr->tx_por]);
	} else {
		descr->tx_por           = 0;
		descr->tx_buffer_length = 0;
		if (descr->cbs.tx) {
			descr->cbs.tx(descr);
		}
	}
}

/**
 * \internal Callback function for data receipt
 *
 * \param[in] device The pointer to i2c slave device
 */
static void i2c_s_async_byte_received(struct _i2c_s_async_device *const device, const uint8_t data)
{
	struct i2c_s_async_descriptor *descr = CONTAINER_OF(device, struct i2c_s_async_descriptor, device);

	ringbuffer_put(&descr->rx, data);

	if (descr->cbs.rx) {
		descr->cbs.rx(descr);
	}
}

static void i2c_s_async_stop(struct _i2c_s_async_device *const device)
{
	struct i2c_s_async_descriptor *descr = CONTAINER_OF(device, struct i2c_s_async_descriptor, device);

	if (descr->cbs.stop) {
		descr->cbs.stop(descr);
	}
}

/**
 * \internal Callback function for error
 *
 * \param[in] device The pointer to i2c slave device
 */
static void i2c_s_async_error(struct _i2c_s_async_device *const device)
{
	struct i2c_s_async_descriptor *descr = CONTAINER_OF(device, struct i2c_s_async_descriptor, device);

	if (descr->cbs.error) {
		descr->cbs.error(descr);
	}
}

/*
 * \internal Read data from i2c slave interface
 *
 * \param[in] descr The pointer to an io descriptor
 * \param[in] buf A buffer to read data to
 * \param[in] length The size of a buffer
 *
 * \return The number of bytes read.
 */
static int32_t i2c_s_async_read(struct io_descriptor *const io, uint8_t *const buf, const uint16_t length)
{
	uint16_t                       was_read = 0;
	uint32_t                       num;
	struct i2c_s_async_descriptor *descr = CONTAINER_OF(io, struct i2c_s_async_descriptor, io);

	ASSERT(io && buf && length);

	CRITICAL_SECTION_ENTER()
	num = ringbuffer_num(&descr->rx);
	CRITICAL_SECTION_LEAVE()

	while ((was_read < num) && (was_read < length)) {
		ringbuffer_get(&descr->rx, &buf[was_read++]);
	}

	return (int32_t)was_read;
}

/*
 * \internal Write the given data to i2c slave interface
 *
 * \param[in] descr The pointer to an io descriptor
 * \param[in] buf Data to write to usart
 * \param[in] length The number of bytes to write
 *
 * \return The number of bytes written or -1 if another write operation is in
 *         progress.
 */
static int32_t i2c_s_async_write(struct io_descriptor *const io, const uint8_t *const buf, const uint16_t length)
{
	struct i2c_s_async_descriptor *descr = CONTAINER_OF(io, struct i2c_s_async_descriptor, io);

	ASSERT(io && buf && length);

	if (descr->tx_por != descr->tx_buffer_length) {
		return ERR_BUSY;
	}

	descr->tx_buffer        = (uint8_t *)buf;
	descr->tx_buffer_length = length;
	_i2c_s_async_write_byte(&descr->device, descr->tx_buffer[0]);

	return (int32_t)length;
}
