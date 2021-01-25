/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2018-11-06     balanceTWK        first version
 * 2019-04-23     WillianChan       Fix GPIO serial number disorder
 */

#include <board.h>
#include "drv_gpio.h"

#ifdef BSP_USING_PIN

static void mcu_pin_mode(rt_device_t dev, rt_base_t pin, rt_base_t mode)
{
    const struct pin_index *index;

    RCC_PeriphClockCmd(APBPeriph_GPIO, APBPeriph_GPIO_CLOCK, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_StructInit(&GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_GetPin(pin);

    if (index == RT_NULL)
    {
        return;
    }

    Pad_Config(pin, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE,
               PAD_OUT_HIGH);
    Pinmux_Config(pin, DWGPIO);

    if (mode == PIN_MODE_OUTPUT)
    {
        /* output setting */
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    }
    else if (mode == PIN_MODE_INPUT)
    {
        /* input setting: not pull. */
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
        Pad_Config(pin, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_ENABLE,
                   PAD_OUT_HIGH);
        Pinmux_Config(pin, DWGPIO);
    }
    else if (mode == PIN_MODE_OUTPUT_OD)
    {
        Pad_Config(pin, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE, PAD_OUT_LOW);
        Pinmux_Config(pin, DWGPIO);
    }
    GPIO_InitStruct.GPIO_ITCmd  = DISABLE;
    GPIO_Init(&GPIO_InitStruct);
}
static void mcu_pin_write(rt_device_t dev, rt_base_t pin, rt_base_t value)
{
    GPIO_WriteBit(GPIO_GetPin(pin), (BitAction)(value));
}
static int mcu_pin_read(rt_device_t dev, rt_base_t pin)
{
    int value;

    value = PIN_LOW;

    value = GPIO_ReadInputDataBit(GPIO_GetPin(pin));

    return value;
}
const static struct rt_pin_ops mcu_pin_ops =
{
    mcu_pin_mode,
    mcu_pin_write,
    mcu_pin_read,
};


int rt_hw_pin_init(void)
{
    return rt_device_pin_register("pin", &mcu_pin_ops, RT_NULL);
}
INIT_BOARD_EXPORT(rt_hw_pin_init);
#endif /* RT_USING_PIN */
