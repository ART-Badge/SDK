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
#include "os_mem.h"
#include "os_sched.h"


void *bsal_osif_malloc(uint32_t len)
{
    void *p = NULL;
    p = os_mem_alloc(RAM_TYPE_DATA_ON, len);
    memset(p, 0, len);
    return p;
}

void bsal_osif_free(void *p)
{
    os_mem_free(p);
}

void bsal_osif_delay(uint32_t ms)
{
    os_delay(ms);
}


