/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2020-11-25     tyustli       first version
 */

#include "rtthread.h"
#include "rtdevice.h"
#include "drv_lcd_8080.h"

#ifdef PKG_USING_PERSIMMON_LIB
void rtthread_persimmon_startup(void)
{
    extern int js_app_ready_init(void);
    js_app_ready_init();

    extern int rtgui_system_server_init(void);
    rtgui_system_server_init();

    extern int js_persim_thread_init(void);
    js_persim_thread_init();

    extern int js_app_ready_sem_release(void);
    js_app_ready_sem_release();
}
#endif /* PKG_USING_PERSIMMON_LIB*/

/********************** end of file *********************/
