/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2021-01-23     WillianChan   first version
 */

#include <stdlib.h>
#include <string.h>
#include "rtthread.h"
#include "rtdevice.h"
#include "board.h"
#include <hws.h>

#ifdef PKG_USING_GUIENGINE
#include <rtgui/driver.h>
#endif

#define DBG_ENABLE
#define DBG_TAG     "MAIN"
#define DBG_LVL     DBG_LOG
#include <rtdbg.h>

#define APP_TASK_PRIORITY       9
#define APP_TASK_STACK_SIZE     (512 * 4)
#define BACKLIGHT               P1_2

extern int rt_hw_8080_lcd_init(void);

static void app_main_task(void *p_param)
{
    rt_components_init();

    extern void board_uart_init(uint8_t tx_pin, uint8_t rx_pin, UART_TypeDef * uart_index);
    board_uart_init(P3_0, P3_1, UART0);

    /* backlight on */
    rt_pin_mode(BACKLIGHT, PIN_MODE_OUTPUT);
    rt_pin_write(BACKLIGHT, PIN_HIGH);
    rt_hw_8080_lcd_init();
#ifdef PKG_USING_GUIENGINE
    rt_device_t dev;
    rt_uint8_t init_bright = 0x01;
    dev = rt_device_find("lcd");
    rt_device_open(dev, RT_DEVICE_FLAG_RDWR);
    /* set backlight to 1%, for saving electricity */
    rt_device_control(dev, RTGRAPHIC_CTRL_SET_BRIGHTNESS, &init_bright);
    rt_device_close(dev);
#endif /* PKG_USING_GUIENGINE */

#ifdef PKG_USING_PERSIMMON_LIB
    extern int filesystem_mount(void);
    if(filesystem_mount() == 0)
    {
        LOG_I("file system init success, start persimmon ui\r\n");
    }
    else
    {
        LOG_E("init file system failed\r\n");
    }
#endif /* PKG_USING_PERSIMMON_LIB */
    
    /* start smodule */
    extern void* rt_kpi_entry(int no);
    extern void *smodule_math_init(uint32_t smodule_addr, void *kpi_entry);;
    smodule_math_init(0x008d0000, &rt_kpi_entry);
    
    /* start ble */
    extern int bsal_bas_blufi_app(void);
    bsal_bas_blufi_app();
}

static void main_task_entry(void)
{
    static void *app_task_handle;

    app_task_handle = rt_thread_create("main_task", app_main_task, NULL, APP_TASK_STACK_SIZE,
                                       APP_TASK_PRIORITY, 2);
    rt_thread_startup(app_task_handle);
}

int main(void)
{
    if (flash_try_high_speed(FLASH_MODE_4BIT) == 0)
    {
        LOG_E("Flash change to 4 BIT Mode Fail!");
    }
    main_task_entry();
    rt_system_scheduler_start();

    return RT_EOK;
}
