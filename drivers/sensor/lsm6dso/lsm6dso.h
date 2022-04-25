/* ST Microelectronics LSM6DSO 6-axis IMU sensor driver
 *
 * Copyright (c) 2019 STMicroelectronics
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Datasheet:
 * https://www.st.com/resource/en/datasheet/lsm6dso.pdf
 */

#ifndef ZEPHYR_DRIVERS_SENSOR_LSM6DSO_LSM6DSO_H_
#define ZEPHYR_DRIVERS_SENSOR_LSM6DSO_LSM6DSO_H_

#include <zephyr/drivers/sensor.h>
#include <zephyr/types.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/sys/util.h>
#include <stmemsc.h>
#include "lsm6dso_reg.h"

#if DT_ANY_INST_ON_BUS_STATUS_OKAY(spi)
#include <zephyr/drivers/spi.h>
#endif /* DT_ANY_INST_ON_BUS_STATUS_OKAY(spi) */

#if DT_ANY_INST_ON_BUS_STATUS_OKAY(i2c)
#include <zephyr/drivers/i2c.h>
#endif /* DT_ANY_INST_ON_BUS_STATUS_OKAY(i2c) */

#define LSM6DSO_EN_BIT					0x01
#define LSM6DSO_DIS_BIT					0x00

/* Accel sensor sensitivity grain is 61 ug/LSB */
#define GAIN_UNIT_XL				(61LL)

/* Gyro sensor sensitivity grain is 4.375 udps/LSB */
#define GAIN_UNIT_G				(4375LL)

#define SENSOR_PI_DOUBLE			(SENSOR_PI / 1000000.0)
#define SENSOR_DEG2RAD_DOUBLE			(SENSOR_PI_DOUBLE / 180)
#define SENSOR_G_DOUBLE				(SENSOR_G / 1000000.0)

struct lsm6dso_config {
	stmdev_ctx_t ctx;
	union {
#if DT_ANY_INST_ON_BUS_STATUS_OKAY(i2c)
		const struct i2c_dt_spec i2c;
#endif
#if DT_ANY_INST_ON_BUS_STATUS_OKAY(spi)
		const struct spi_dt_spec spi;
#endif
	} stmemsc_cfg;
	uint8_t accel_pm;
	uint8_t accel_odr;
#define ACCEL_RANGE_DOUBLE	BIT(7)
#define ACCEL_RANGE_MASK	BIT_MASK(6)
	uint8_t accel_range;
	uint8_t gyro_pm;
	uint8_t gyro_odr;
	uint8_t gyro_range;
	uint8_t drdy_pulsed;
#ifdef CONFIG_LSM6DSO_TRIGGER
	bool fifo_active;
	const struct gpio_dt_spec gpio_intr;
	uint8_t int_pin;
	bool trig_enabled;
#endif /* CONFIG_LSM6DSO_TRIGGER */
#ifdef CONFIG_LSM6DSO_ENABLE_FIFO
	uint16_t batch_cnt_thr;
	uint8_t accel_bdr;
	uint8_t gyro_bdr;
#endif /* CONFIG_LSM6DSO_ENABLE_FIFO */
};

union samples {
	uint8_t raw[6];
	struct {
		int16_t axis[3];
	};
} __aligned(2);

#define LSM6DSO_SHUB_MAX_NUM_SLVS			2

struct lsm6dso_data {
	const struct device *dev;
	int16_t acc[3];
	uint32_t acc_gain;
	int16_t gyro[3];
	uint32_t gyro_gain;
#if defined(CONFIG_LSM6DSO_ENABLE_TEMP)
	int16_t temp_sample;
#endif
#if defined(CONFIG_LSM6DSO_SENSORHUB)
	uint8_t ext_data[2][6];
	uint16_t magn_gain;

	struct hts221_data {
		int16_t x0;
		int16_t x1;
		int16_t y0;
		int16_t y1;
	} hts221;
	bool shub_inited;
	uint8_t num_ext_dev;
	uint8_t shub_ext[LSM6DSO_SHUB_MAX_NUM_SLVS];
#endif /* CONFIG_LSM6DSO_SENSORHUB */

	uint16_t accel_freq;
	uint8_t accel_fs;
	uint16_t gyro_freq;
	uint8_t gyro_fs;

#ifdef CONFIG_LSM6DSO_TRIGGER
	struct gpio_callback gpio_cb;
	sensor_trigger_handler_t handler_drdy_acc;
	sensor_trigger_handler_t handler_drdy_gyr;
	sensor_trigger_handler_t handler_drdy_temp;
#if defined(CONFIG_LSM6DSO_ENABLE_FIFO)
	sensor_trigger_handler_t handler_fifo_bdr_cnt;
#endif

#if defined(CONFIG_LSM6DSO_TRIGGER_OWN_THREAD)
	K_KERNEL_STACK_MEMBER(thread_stack, CONFIG_LSM6DSO_THREAD_STACK_SIZE);
	struct k_thread thread;
	struct k_sem gpio_sem;
#elif defined(CONFIG_LSM6DSO_TRIGGER_GLOBAL_THREAD)
	struct k_work work;
#endif
#endif /* CONFIG_LSM6DSO_TRIGGER */
};

#if defined(CONFIG_LSM6DSO_SENSORHUB)
int lsm6dso_shub_init(const struct device *dev);
int lsm6dso_shub_fetch_external_devs(const struct device *dev);
int lsm6dso_shub_get_idx(const struct device *dev, enum sensor_channel type);
int lsm6dso_shub_config(const struct device *dev, enum sensor_channel chan,
			enum sensor_attribute attr,
			const struct sensor_value *val);
#endif /* CONFIG_LSM6DSO_SENSORHUB */

#ifdef CONFIG_LSM6DSO_TRIGGER
int lsm6dso_trigger_set(const struct device *dev,
			const struct sensor_trigger *trig,
			sensor_trigger_handler_t handler);

int lsm6dso_init_interrupt(const struct device *dev);
#endif

#endif /* ZEPHYR_DRIVERS_SENSOR_LSM6DSO_LSM6DSO_H_ */
