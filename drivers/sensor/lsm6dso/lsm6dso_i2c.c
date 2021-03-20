/* ST Microelectronics LSM6DSO 6-axis IMU sensor driver
 *
 * Copyright (c) 2019 STMicroelectronics
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Datasheet:
 * https://www.st.com/resource/en/datasheet/lsm6dso.pdf
 */

#define DT_DRV_COMPAT st_lsm6dso

#include <logging/log.h>
#include "lsm6dso.h"

#if DT_ANY_INST_ON_BUS_STATUS_OKAY(i2c)

LOG_MODULE_DECLARE(LSM6DSO, CONFIG_SENSOR_LOG_LEVEL);

int lsm6dso_i2c_init(const struct device *dev)
{
	struct lsm6dso_data *data = dev->data;
	const struct lsm6dso_config *cfg = dev->config;

	/* Use generic stmemsc routine for read/write I2C bus */
	data->ctx_i2c.read_reg = (stmdev_read_ptr) stmemsc_i2c_read,
	data->ctx_i2c.write_reg = (stmdev_write_ptr) stmemsc_i2c_write,

	data->ctx = &data->ctx_i2c;
	data->ctx->handle = (void *)&cfg->stmemsc;

	return 0;
}
#endif /* DT_ANY_INST_ON_BUS_STATUS_OKAY(i2c) */
