/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author         Notes
 * 2020-04-22     supperthomas   first version  
 */

#include <rtconfig.h>
#include "rtconfig.h"
#include <rtthread.h>
#include <rtdevice.h>
#include <string.h>

#ifdef BSP_USING_PSRAM
#include "board.h"
#include "psram_platform.h"
#include <stdio.h>
#include <string.h>
#include "drv_psram.h"
#include "drv_lcd_8080.h"

#define PSRAM_PIN_ENABLE                 P2_0

#ifndef PSRAM_AS_MAIN_HEAP
struct rt_memheap psram_heap;
#endif /* PSRAM_AS_MAIN_HEAP */

static void psram_pin_enable(rt_base_t pin, rt_base_t mode, rt_base_t value)
{
    RCC_PeriphClockCmd(APBPeriph_GPIO, APBPeriph_GPIO_CLOCK, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_GetPin(pin);

    Pad_Config(pin, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_HIGH);
    Pinmux_Config(pin, DWGPIO);

    /* output setting */
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_ITCmd  = DISABLE;
    GPIO_Init(&GPIO_InitStruct);

    /* output value */
    GPIO_WriteBit(GPIO_GetPin(pin), (BitAction)(value));
}

int drv_psram_init(void)
{
#define FLASH_MODE_4BIT 2
    /* enable psram pin */
    psram_pin_enable(PSRAM_PIN_ENABLE, PIN_MODE_OUTPUT, PIN_HIGH);
    psram_init();
    psram_try_high_speed(FLASH_MODE_4BIT);
#ifdef PSRAM_AS_MAIN_HEAP
    rt_system_heap_init((void *)PSRAM_BEGIN_ADDR + (LCD_WIDTH * LCD_HIGHT * PIXEL_BYTES), (void *)(PSRAM_BEGIN_ADDR + PSRAM_SIZE - (LCD_WIDTH * LCD_HIGHT * PIXEL_BYTES)));
#else
    rt_memheap_init(&psram_heap, "psram", (void *)(PSRAM_BEGIN_ADDR), PSRAM_SIZE);
#endif

    return 0;
}

#endif /* BSP_USING_PSRAM */

/*********** end of file **********/
