/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-13     ChenYong     first version
 */

#include <string.h>

#include <rtthread.h>
#include <logmgr.h>

static int logmgr_test(int argc, char **argv)
{
    volatile int * SCB_CCR = (volatile int *) 0xE000ED14; // SCB->CCR
    static rt_bool_t is_init = RT_FALSE;
    int x, y, z;

    if (argc < 2)
    {
        rt_kprintf("Please input 'logmgr_test <INIT|DIVBYZERO|UNALIGNED|ASSERT>' \n");
        return -1;
    }

    if (!strcmp(argv[1], "INIT"))
    {
        /* logmgr initialized */
        logmgr_init();
        is_init = RT_TRUE;
    }
    else
    {
        if (is_init == RT_FALSE)
        {
            rt_kprintf("logmgr is not initialized, please input 'logmgr_test INIT'\n");
            return -1;
        }

        if (!strcmp(argv[1], "DIVBYZERO"))
        {
            *SCB_CCR |= (1 << 4); /* bit4: DIV_0_TRP. */
            x = 10;
            y = rt_strlen("");
            z = x / y;
            rt_kprintf("z:%d\n", z);

            return 0;
        }
        else if (!strcmp(argv[1], "UNALIGNED"))
        {
            volatile int * p;
            volatile int value;
            *SCB_CCR |= (1 << 3); /* bit3: UNALIGN_TRP. */

            p = (int *) 0x00;
            value = *p;
            rt_kprintf("addr:0x%02X value:0x%08X\r\n", (int) p, value);

            p = (int *) 0x04;
            value = *p;
            rt_kprintf("addr:0x%02X value:0x%08X\r\n", (int) p, value);

            p = (int *) 0x03;
            value = *p;
            rt_kprintf("addr:0x%02X value:0x%08X\r\n", (int) p, value);

            return 0;
        }
        else if (!strcmp(argv[1], "ASSERT"))
        {
            char *p = RT_NULL;
            RT_ASSERT(p);
        }
    }

    return 0;
}
#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(logmgr_test, logmgr feature test);
#endif
