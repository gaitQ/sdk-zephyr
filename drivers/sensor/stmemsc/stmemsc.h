/* ST Microelectronics STMEMS hal i/f
 *
 * Copyright (c) 2021 STMicroelectronics
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * zephyrproject-rtos/modules/hal/st/sensor/stmemsc/
 */

#ifndef ZEPHYR_DRIVERS_SENSOR_STMEMSC_STMEMSC_H_
#define ZEPHYR_DRIVERS_SENSOR_STMEMSC_STMEMSC_H_

#include <drivers/i2c.h>
#include <drivers/spi.h>

union stmemsc_bus_cfg {
	/* bus is I2C */
	uint16_t i2c_slv_addr;

	/* bus is SPI */
	struct spi_config spi_cfg;
};

struct stmemsc_cfg {
	const struct device *bus;
	const union stmemsc_bus_cfg bus_cfg;
};

int stmemsc_i2c_read(const struct stmemsc_cfg *stmemsc,
		     uint8_t reg_addr, uint8_t *value, uint8_t len);
int stmemsc_i2c_write(const struct stmemsc_cfg *stmemsc,
		      uint8_t reg_addr, uint8_t *value, uint8_t len);
int stmemsc_spi_read(const struct stmemsc_cfg *stmemsc,
		     uint8_t reg_addr, uint8_t *value, uint8_t len);
int stmemsc_spi_write(const struct stmemsc_cfg *stmemsc,
		      uint8_t reg_addr, uint8_t *value, uint8_t len);
#endif /* ZEPHYR_DRIVERS_SENSOR_STMEMSC_STMEMSC_H_ */
