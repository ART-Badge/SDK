/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-06-09     tyx          first version.
 */

#include <rtthread.h>
#include "kdb_core.h"
#include "kdb_type.h"

extern rt_base_t rt_hw_interrupt_disable(void);
extern void rt_hw_interrupt_enable(rt_base_t level);

void kdb_output(char *buf, int len)
{
    rt_kprintf("%s", buf);
}

rt_base_t kdb_enter_critical(void)
{
    return rt_hw_interrupt_disable();
}

void kdb_exit_critical(rt_base_t level)
{
    rt_hw_interrupt_enable(level);
}
