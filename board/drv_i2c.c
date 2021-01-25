/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-07-14     tyustli   first version
 */

#include "drv_i2c.h"
#include "board.h"
#include "rtl876x_i2c.h"

#ifdef BSP_USING_I2C

#if !defined(BSP_USING_I2C1) && !defined(BSP_USING_I2C2) && !defined(BSP_USING_I2C3) && !defined(BSP_USING_I2C4)
#error "Please define at least one BSP_USING_I2Cx"
/* this driver can be disabled at menuconfig  RT-Thread Components  Device Drivers */
#endif

struct realtek_i2c_bus
{
    struct rt_i2c_bus_device parent;
    I2C_TypeDef *based;
    char *device_name;
};

static struct realtek_i2c_bus realtek_i2c_device[] = {
#ifdef BSP_USING_I2C1
    {
        .based = I2C1,
        .device_name = "i2c1"
    },
#endif
};

static rt_size_t realtek_master_xfer(struct rt_i2c_bus_device *bus, struct rt_i2c_msg msgs[], rt_uint32_t num)
{
    rt_uint32_t send_num;
    I2C_Status abort_status = I2C_ERR_TIMEOUT;
    struct realtek_i2c_bus *i2c_bus;
    struct rt_i2c_msg *msg;

    RT_ASSERT(bus != RT_NULL);
    i2c_bus = rt_list_entry(bus, struct realtek_i2c_bus, parent);
    RT_ASSERT(i2c_bus != RT_NULL);

    for (send_num = 0; send_num < num; send_num++)
    {
        msg = &msgs[send_num];
        if (msg->flags & RT_I2C_RD)
        {
            I2C_SetSlaveAddress(i2c_bus->based, msg->addr);
            abort_status = I2C_MasterRead(i2c_bus->based, msg->buf, msg->len);
            if (abort_status != I2C_Success)
            {
                return 0;
            }
        }
        else
        {
            I2C_SetSlaveAddress(i2c_bus->based, msg->addr);
            abort_status = I2C_MasterWrite(i2c_bus->based, msg->buf, msg->len);
            if (abort_status != I2C_Success)
            {
                return 0;
            }
        }
    }

    return send_num;
}

static rt_size_t realtek_slave_xfer(struct rt_i2c_bus_device *bus, struct rt_i2c_msg msgs[], rt_uint32_t num)
{
    return 0;
}
static rt_err_t realtek_i2c_bus_control(struct rt_i2c_bus_device *bus, rt_uint32_t cmd, rt_uint32_t arg)
{
    return 0;
}

static const struct rt_i2c_bus_device_ops realtek_ops = {
    .master_xfer = realtek_master_xfer,
    .slave_xfer = realtek_slave_xfer,
    .i2c_bus_control = realtek_i2c_bus_control,
};

static void hw_i2c_init(void)
{
    /* init gpio */
    Pinmux_Config(MPU6XXX_I2C_SCL, MPU6XXX_I2C_FUNC_SCL);
    Pinmux_Config(MPU6XXX_I2C_SDA, MPU6XXX_I2C_FUNC_SDA);

    Pad_Config(MPU6XXX_I2C_SCL, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_Config(MPU6XXX_I2C_SDA, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);

    /* Enable GPIO and hardware timer's clock */
    RCC_PeriphClockCmd(APBPeriph_GPIO,  APBPeriph_GPIO_CLOCK,  ENABLE);

    RCC_I2CClkDivConfig(MPU6XXX_I2C_BUS, I2C_CLOCK_DIV_2);
    RCC_PeriphClockCmd(MPU6XXX_I2C_APBPeriph, MPU6XXX_I2C_APBClock, ENABLE);
    I2C_InitTypeDef  I2C_InitStructure;
    I2C_InitStructure.I2C_Clock = 20000000;
    I2C_InitStructure.I2C_ClockSpeed   = 400000;
    I2C_InitStructure.I2C_DeviveMode   = I2C_DeviveMode_Master;
    I2C_InitStructure.I2C_AddressMode  = I2C_AddressMode_7BIT;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_SDA_Pin = MPU6XXX_I2C_SDA;
    I2C_InitStructure.I2C_SCL_Pin = MPU6XXX_I2C_SCL;
    I2C_Init(MPU6XXX_I2C_BUS, &I2C_InitStructure);
    I2C_Cmd(MPU6XXX_I2C_BUS, ENABLE);
}

int rt_hw_i2c_init(void)
{
    rt_uint8_t i = 0;
    rt_uint8_t i2c_bus_num = 0;

    hw_i2c_init();

    i2c_bus_num = sizeof(realtek_i2c_device) / sizeof(realtek_i2c_device[0]);

    for (i = 0; i < i2c_bus_num; i++)
    {
        realtek_i2c_device[i].parent.ops = &realtek_ops;
        rt_i2c_bus_device_register(&realtek_i2c_device[i].parent, realtek_i2c_device[i].device_name);
    }

    return 1;
}
INIT_BOARD_EXPORT(rt_hw_i2c_init);

#endif /* BSP_USING_I2C */
