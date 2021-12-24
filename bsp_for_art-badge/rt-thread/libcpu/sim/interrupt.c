/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-18     tyx          first implementation
 */

#include <rthw.h>
#include <rtthread.h>

static struct rt_irq_desc isr_table[256];

/**
 * This function will initialize hardware interrupt
 */
void rt_hw_interrupt_init(void)
{
}

/**
 * This function will mask a interrupt.
 * @param vector the interrupt number
 */
void rt_hw_interrupt_mask(int vector)
{
}

/**

 * This function will un-mask a interrupt.
 * @param vector the interrupt number
 */
void rt_hw_interrupt_umask(int vector)
{
}

/**
 * This function will install a interrupt service routine to a interrupt.
 * @param vector the interrupt number
 * @param handler the interrupt service routine to be installed
 * @param param the interrupt service function parameter
 * @param name the interrupt name
 * @return old handler
 */
rt_isr_handler_t rt_hw_interrupt_install(int vector, rt_isr_handler_t handler,
        void *param, const char *name)
{
    rt_isr_handler_t old_handler = RT_NULL;
    rt_base_t level;

    if ((vector < 0) || (vector > sizeof(isr_table) / sizeof(isr_table[0])))
    {
        return old_handler;
    }
    level = rt_hw_interrupt_disable();
    old_handler = isr_table[vector].handler;
#ifdef RT_USING_INTERRUPT_INFO
    rt_strncpy(isr_table[vector].name, name, RT_NAME_MAX);
#endif /* RT_USING_INTERRUPT_INFO */
    isr_table[vector].handler = handler;
    isr_table[vector].param = param;
    rt_hw_interrupt_enable(level);
    return old_handler;
}

void sim_interrupt_dispatch(long vector)
{
    void *param;
    rt_isr_handler_t isr_func;

    isr_func = isr_table[vector].handler;
    param = isr_table[vector].param;
    /* jump to fun */
    rt_interrupt_enter();
    if (isr_func != RT_NULL)
    {
        isr_func(vector, param);
    }
    rt_interrupt_leave();
#ifdef RT_USING_INTERRUPT_INFO
    isr_table[vector].counter ++;
#endif
}

void sim_interrupt_systick(void)
{
    rt_interrupt_enter();
    rt_tick_increase();
    rt_interrupt_leave();
}
