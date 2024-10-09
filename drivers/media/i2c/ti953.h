/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (C) 2020 Intel Corporation */

#ifndef TI953_H
#define TI953_H

struct ti953_register_write {
	u8 reg;
	u8 val;
};

struct ti953_register_devid {
	u8 reg;
	u8 val_expected;
};

/* register definition */
#define TI953_RESET_CTL		0x1
#define TI953_GENERAL_CFG	0x2
#define TI953_LOCAL_GPIO_DATA	0xd
#define TI953_GPIO_INPUT_CTRL	0xe
#define TI953_SCL_HIGH_TIME		0xbU
#define TI953_SCL_LOW_TIME		0xcU

/* register value definition */
#define TI953_DIGITAL_RESET_1	0x2
#define TI953_GPIO0_RMTEN	0x10
#define TI953_GPIO0_OUT		0x1
#define TI953_GPIO1_OUT		(0x1 << 1)
#define TI953_GPIO_OUT_EN	0xf0
#define TI953_I2C_SCL_HIGH_TIME_STANDARD	0x7F
#define TI953_I2C_SCL_LOW_TIME_STANDARD		0x7F
#define TI953_I2C_SCL_HIGH_TIME_FAST		0x13
#define TI953_I2C_SCL_LOW_TIME_FAST		0x26
#define TI953_I2C_SCL_HIGH_TIME_FAST_PLUS	0x06
#define TI953_I2C_SCL_LOW_TIME_FAST_PLUS	0x0b

#define TI953_I2C_SPEED_STANDARD	0x1U
#define TI953_I2C_SPEED_FAST		0x2U
#define TI953_I2C_SPEED_HIGH		0x3U
#define TI953_I2C_SPEED_FAST_PLUS	0x4U

/*
 * TI953_GENERAL_CFG:
 * bit 0: I2C Strap Mode: I2C Voltage level
 * bit 1: CRC_TX_GEN_ENABLE: Transmitter CRC Generator
 * bit 4 - 5:
 * CSI-2 Data lane configuration.
 * 00: 1-lane configuration
 * 01: 2-lane configuration
 * 11: 4-lane configuration
 * bit 6:
 * CONTS_CLK:
 * CSI-2 Clock Lane Configuration.
 * 0 : Non Continuous Clock
 * 1 : Continuous Clock
 */
#define TI953_CONTS_CLK		0x40
#define TI953_CSI_1LANE		0x00
#define TI953_CSI_2LANE		0x10
#define TI953_CSI_4LANE		0x30
#define TI953_CSI_LANE_MASK	~(0x30)
#define TI953_CRC_TX_GEN_ENABLE	0x2
#define TI953_I2C_STRAP_MODE	0x1

static const struct ti953_register_write ti953_init_settings[] = {
	{0x4c, 0x01}, /* ox03a10 init sequence */
	{0xb0, 0x04},
	{0xb1, 0x03},
	{0xb2, 0x25},
	{0xb1, 0x13},
	{0xb2, 0x25},
	{0xb0, 0x04},
	{0xb1, 0x04},
	{0xb2, 0x30},
	{0xb1, 0x14},
	{0xb2, 0x30},
	{0xb0, 0x04},
	{0xb1, 0x06},
	{0xb2, 0x40},
	{0x42, 0x01},
	{0x41, 0x93},
	{0x4c, 0x12},
	{0xb0, 0x08},
	{0xb1, 0x03},
	{0xb2, 0x25},
	{0xb1, 0x13},
	{0xb2, 0x25},
	{0xb0, 0x08},
	{0xb1, 0x04},
	{0xb2, 0x30},
	{0xb1, 0x14},
	{0xb2, 0x30},
	{0xb0, 0x08},
	{0xb1, 0x06},
	{0xb2, 0x40},
	{0x42, 0x01},
	{0x41, 0x93},
	{0x4c, 0x24},
	{0xb0, 0x0c},
	{0xb1, 0x03},
	{0xb2, 0x25},
	{0xb1, 0x13},
	{0xb2, 0x25},
	{0xb0, 0x0c},
	{0xb1, 0x04},
	{0xb2, 0x30},
	{0xb1, 0x14},
	{0xb2, 0x30},
	{0xb0, 0x0c},
	{0xb1, 0x06},
	{0xb2, 0x40},
	{0x42, 0x01},
	{0x41, 0x93},
	{0x4c, 0x38},
	{0xb0, 0x10},
	{0xb1, 0x03},
	{0xb2, 0x25},
	{0xb1, 0x13},
	{0xb2, 0x25},
	{0xb0, 0x10},
	{0xb1, 0x04},
	{0xb2, 0x30},
	{0xb1, 0x14},
	{0xb2, 0x30},
	{0xb0, 0x10},
	{0xb1, 0x06},
	{0xb2, 0x40},
	{0x42, 0x01},
	{0x41, 0x93},
};

static const struct ti953_register_write ti953_init_settings_clk[] = {
	{0x06, 0x41},
	/* WA: set N to 0x25 for 30 fps */
	{0x07, 0x25},
};

static const struct ti953_register_devid ti953_FPD3_RX_ID[] = {
	{0xf0, 0x5f},
	{0xf1, 0x55},
	{0xf2, 0x42},
	{0xf3, 0x39},
	{0xf4, 0x35},
	{0xf5, 0x33},
};

int ti953_reg_write(struct v4l2_subdev *sd, unsigned short rx_port,
		unsigned short ser_alias, unsigned char reg, unsigned char val);

int ti953_reg_read(struct v4l2_subdev *sd, unsigned short rx_port,
		unsigned short ser_alias, unsigned char reg, unsigned char *val);

bool ti953_detect(struct v4l2_subdev *sd, unsigned short rx_port, unsigned short ser_alias);

int ti953_init(struct v4l2_subdev *sd, unsigned short rx_port, unsigned short ser_alias);
int ti953_init_clk(struct v4l2_subdev *sd, unsigned short rx_port, unsigned short ser_alias);
int32_t ti953_bus_speed(struct v4l2_subdev *sd, uint16_t rx_port, uint16_t ser_alias, uint8_t i2c_speed);

#endif