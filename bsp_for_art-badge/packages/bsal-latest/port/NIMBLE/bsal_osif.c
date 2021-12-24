/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-28     Supperthomas the first version
 */


#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>

#include "bsal_osif.h"

void *bsal_osif_malloc(uint32_t len)
{
    void *p = NULL;
    p = rt_malloc(len);
    memset(p, 0, len);
    return p;
}

void bsal_osif_free(void *p)
{
    rt_free(p);
}

#if 0
int bsal_osif_printf_warn(const char *fmt, ...)
{
    rt_kprintf("\r\n [BSAL][WARN]:");
    va_list args;
    va_start(args, fmt);
    rt_kprintf(fmt, args);
    va_end(args);
    return 0;
}

int bsal_osif_printf_info(const char *fmt, ...)
{
    rt_kprintf("\r\n [BSAL][INFO]:");
    va_list args;
    va_start(args, fmt);
    rt_kprintf(fmt, args);
    va_end(args);
    return 0;
}

int bsal_osif_printf_dbg(const char *fmt, ...)
{
    rt_kprintf("\r\n [BSAL][DEBUG]:");
    va_list args;
    va_start(args, fmt);
    rt_kprintf(fmt, args);
    va_end(args);
    rt_kprintf("\r\n");
    return 0;
}


int bsal_osif_printf_err(const char *fmt, ...)
{
    rt_kprintf("\r\n [BSAL][ERROR]:");
    va_list args;
    va_start(args, fmt);
    rt_kprintf(fmt, args);
    va_end(args);
    rt_kprintf("\r\n");
    return 0;
}
#endif

void bsal_osif_delay(uint32_t ms)
{
    rt_thread_mdelay(ms);
}









