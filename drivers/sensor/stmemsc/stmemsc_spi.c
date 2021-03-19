/* ST Microelectronics STMEMS hal i/f
 *
 * Copyright (c) 2021 STMicroelectronics
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * zephyrproject-rtos/modules/hal/st/sensor/stmemsc/
 */

#include <stmemsc.h>

#define IIS2ICLX_SPI_READ		(1 << 7)

int stmemsc_spi_read(const struct stmemsc_cfg *stmemsc,
		     uint8_t reg_addr, uint8_t *value, uint8_t len)
{
	const struct spi_config *spi_cfg = &stmemsc->bus_cfg.spi_cfg;
	uint8_t buffer_tx[2] = { reg_addr | IIS2ICLX_SPI_READ, 0 };
	const struct spi_buf tx_buf = {
			.buf = buffer_tx,
			.len = 2,
	};
	const struct spi_buf_set tx = {
		.buffers = &tx_buf,
		.count = 1
	};
	const struct spi_buf rx_buf[2] = {
		{
			.buf = NULL,
			.len = 1,
		},
		{
			.buf = value,
			.len = len,
		}
	};
	const struct spi_buf_set rx = {
		.buffers = rx_buf,
		.count = 2
	};

	if (spi_transceive(stmemsc->bus, spi_cfg, &tx, &rx)) {
		return -EIO;
	}

	return 0;
}

int stmemsc_spi_write(const struct stmemsc_cfg *stmemsc,
		      uint8_t reg_addr, uint8_t *value, uint8_t len)
{
	const struct spi_config *spi_cfg = &stmemsc->bus_cfg.spi_cfg;
	uint8_t buffer_tx[1] = { reg_addr & ~IIS2ICLX_SPI_READ };
	const struct spi_buf tx_buf[2] = {
		{
			.buf = buffer_tx,
			.len = 1,
		},
		{
			.buf = value,
			.len = len,
		}
	};
	const struct spi_buf_set tx = {
		.buffers = tx_buf,
		.count = 2
	};

	if (spi_write(stmemsc->bus, spi_cfg, &tx)) {
		return -EIO;
	}

	return 0;
}
