/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-07-14     tyustli   first version
 */

#ifndef __DRV_I2C_H
#define __DRV_I2C_H

#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"

#define MPU6XXX_I2C_SCL                                   P2_1
#define MPU6XXX_I2C_SDA                                   P2_2
#define MPU6XXX_I2C_BUS                                   I2C1
#define MPU6XXX_I2C_FUNC_SCL                              I2C1_CLK
#define MPU6XXX_I2C_FUNC_SDA                              I2C1_DAT
#define MPU6XXX_I2C_APBPeriph                             APBPeriph_I2C1
#define MPU6XXX_I2C_APBClock                              APBPeriph_I2C1_CLOCK

int rt_hw_i2c_init(void);

#endif
