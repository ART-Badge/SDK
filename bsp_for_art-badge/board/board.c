/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-06-29     supperthomas first version
 */
#include <rtthread.h>
#include "board.h"
#include "osif_for_rtl876x.h"

struct rt_memheap buffer_heap;
struct rt_memheap data_heap;
struct rt_memheap pm_heap;

void rt_hw_us_delay(rt_uint32_t us)
{
    rt_uint32_t delta;
    us = us * (SysTick->LOAD / (1000000 / RT_TICK_PER_SECOND));
    delta = SysTick->VAL;

    while (delta - SysTick->VAL < us);
}

uint64_t rtgui_micro_sec_get(void)
{
    rt_tick_t ms, us;

    do
    {
        ms = rt_tick_get();
        us = SysTick->VAL;
    }
    while (ms != rt_tick_get());

    return (uint64_t)ms * 1000u + (999u - us * 999u / SysTick->LOAD);
}

void rt_hw_board_init(void)
{
    /* Set the clock frequency at 100MHz */
    set_system_clock(SYSTEM_90MHZ);

    /* Config systick */
    extern void SetSystemTickInterrupt_RTT(void);
    SetSystemTickInterrupt_RTT();

    #if defined(RT_USING_HEAP)
    /* Heap initialization */
    rt_system_heap_init((void *)(OTP->appDataAddr + OTP->appDataSize + 30 * 1024), \
                        (void *)(OTP->appDataAddr + OTP->appDataSize + OTP->heapDataONSize));

    rt_memheap_init(&buffer_heap, "bufferheap", BUFFER_RAM_ADDR, 18 * 1024);
    rt_memheap_init(&data_heap, "dataheap", BUFFER_RAM_ADDR + 18 * 1024, 12 * 1024);
    //rt_memheap_init(&pm_heap, "pmheap", (void *)(OTP->appDataAddr + OTP->appDataSize), 30 * 1024);
    #endif /* RT_USING_HEAP */

    #ifdef BSP_USING_PSRAM
    /* init psram */
    extern int drv_psram_init(void);
    drv_psram_init();
    #endif /* BSP_USING_PSRAM */

    #ifdef PKG_USING_SYSTEMVIEW
    extern int hw_segger_init(void);
    hw_segger_init();
    #endif /* PKG_USING_SYSTEMVIEW */

    #ifdef BSP_USING_UART
    /* USART driver initialization is open by default */
#include "drv_usart.h"
    rt_hw_usart_init();
    #endif /* BSP_USING_UART */

    #ifdef RT_USING_CONSOLE
    /* Set the shell console output device */
    rt_console_set_device("segger");
    #endif /* RT_USING_CONSOLE */

    #ifdef RT_USING_COMPONENTS_INIT
    /* Board underlying hardware initialization */
    rt_components_board_init();
    #endif /* RT_USING_COMPONENTS_INIT */
}

/************************ end of file ****************************/
