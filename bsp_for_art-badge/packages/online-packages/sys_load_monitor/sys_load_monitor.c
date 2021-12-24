/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-03-03     armink       the first version
 */

#include <rtthread.h>
#include <stdlib.h>

/* monitor frequency (Hz) */
#ifndef SYS_LOAD_MONITOR_FREQ
#define SYS_LOAD_MONITOR_FREQ          (1)
#endif
/* monitor recording time (seconds) */
#ifndef SYS_LOAD_MONITOR_TIME
#define SYS_LOAD_MONITOR_TIME          (20)
#endif
/* monitor recording threads number */
#ifndef SYS_LOAD_MONITOR_MAX_THREAD
#define SYS_LOAD_MONITOR_MAX_THREAD    (5)
#endif

#if RT_THREAD_PRIORITY_MAX > 32
#error "Not support, I hope you can fix it"
#endif

#define MAX_RECORD                     (SYS_LOAD_MONITOR_FREQ * SYS_LOAD_MONITOR_TIME)

/* thread record information */
static rt_thread_t thread_record[MAX_RECORD][SYS_LOAD_MONITOR_MAX_THREAD] = { 0 };
static rt_uint32_t prio_record[MAX_RECORD] = { 0 };
static rt_bool_t record_is_full = RT_FALSE;
static rt_size_t record_tail = 0;
/* monitor timer */
static struct rt_timer timer;

static void sys_load_record(void *parameter)
{
    extern rt_list_t rt_thread_priority_table[RT_THREAD_PRIORITY_MAX];
    extern rt_uint32_t rt_thread_ready_priority_group;
    rt_size_t i, thread_index = 0;
//    rt_kprintf("sys_load_record %08x\n", rt_thread_ready_priority_group);

    rt_thread_t thread;
    struct rt_list_node *node;

    /* clean the old state */
    rt_memset(&thread_record[record_tail], 0, SYS_LOAD_MONITOR_MAX_THREAD * sizeof(rt_thread_t));
    /* current ready priority group */
    prio_record[record_tail] = rt_thread_ready_priority_group;
    /* first thread is running state */
    thread_record[record_tail][thread_index++] = rt_thread_self();
    /* record other ready threads */
    for (i = 0; i < RT_THREAD_PRIORITY_MAX; i ++)
    {
        if (rt_list_isempty(&rt_thread_priority_table[i])) continue;
        rt_list_for_each(node, &rt_thread_priority_table[i])
        {
            thread = rt_list_entry(node, struct rt_thread, tlist);
//            rt_kprintf("add a thread %.*s %d %d\n", RT_NAME_MAX, thread->name, thread->current_priority, i);
            thread_record[record_tail][thread_index ++] = thread;

            if (thread_index >= SYS_LOAD_MONITOR_MAX_THREAD)
                goto __exit;
        }
    }

__exit:

    record_tail++;
    if (record_tail >= MAX_RECORD)
    {
        if (!record_is_full)
            record_is_full = RT_TRUE;

        record_tail = 0;
    }
}

int sys_load_monitor_init(void)
{
    static rt_bool_t is_init = RT_FALSE;

    if (is_init)
    {
        return RT_EOK;
    }

    /* using hardware system tick timer */
    rt_timer_init(&timer, "sys_load", sys_load_record, RT_NULL, rt_tick_from_millisecond(1000 / SYS_LOAD_MONITOR_FREQ),
            RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_HARD_TIMER);
    rt_timer_start(&timer);

    is_init = RT_TRUE;
    return RT_EOK;
}
INIT_PREV_EXPORT(sys_load_monitor_init);

/**
 * dump the system load history
 */
void sys_load_monitor_dump(void)
{
    //TODO call stack
    rt_size_t thread_index = 0;
    rt_off_t i = 0;
    rt_size_t record_head = 0, record_tail_bak = 0;
    rt_thread_t thread;
    rt_uint32_t priority = 0;

    /* append a new record */
    sys_load_record(RT_NULL);

    record_tail_bak = record_tail;
    if (record_is_full)
    {
        record_head = record_tail_bak + 1;
        if (record_head == MAX_RECORD)
            record_head = 0;
    }

    rt_kprintf("%-*.s | 31      23      15      7      0 | ready thread list..\n", RT_NAME_MAX, "RUNNING");
    rt_kprintf("-------- | -------------------------------- | -------------------\n");
    while (record_head != record_tail_bak)
    {
        thread_index = 0;
        thread = thread_record[record_head][thread_index++];
        rt_kprintf("%-*.*s | ", RT_NAME_MAX, RT_NAME_MAX, thread->name);
        /* dump priority group */
        priority = prio_record[record_head];
        for (i = RT_THREAD_PRIORITY_MAX - 1; i >= 0; i--)
        {
            if (i == thread->current_priority)  rt_kprintf("$");
            else if ((priority & 0x80000000) == 0) rt_kprintf(" ");
            else rt_kprintf("*");
            priority <<= 1;
        }
        rt_kprintf(" | ");
        /* dump ready thread info */
        for (; thread_index < SYS_LOAD_MONITOR_MAX_THREAD; thread_index ++)
        {
            thread = thread_record[record_head][thread_index];
            if (thread)
            {
                rt_kprintf("%-*.*s(%02d) ", RT_NAME_MAX, RT_NAME_MAX, thread->name, thread->current_priority);
            }
        }
        rt_kprintf("\n");
        /* calculate next record index */
        record_head++;
        if (record_head >= MAX_RECORD)
        {
            record_head = 0;
        }
    }

//    rt_ubase_t level;
//    //TODO reduce the interrupt disable code
//    level = rt_hw_interrupt_disable();
//
//    rt_hw_interrupt_enable(level);
}
MSH_CMD_EXPORT_ALIAS(sys_load_monitor_dump, sys_load, dump current system running load information);

int sys_load_monitor_deinit(void)
{
    //TODO disable timer
    //TODO free data
    return RT_EOK;
}

/* ================= system load test code ========================== */
static void entry(void *para)
{
    rt_tick_t tick;

    while(1)
    {
        tick = rt_tick_get() + rt_tick_from_millisecond(rand()%10);
        while(rt_tick_get() < tick);
        rt_thread_delay(10);
    }
}

static void start_load_test(void)
{
    rt_thread_t thread;
    int i;
    char name[] = "thread-a";

#define MAX_THREAD 15

    for (i = 0; i < MAX_THREAD; i++)
    {
        name[7] = 'a' + i;
        thread = rt_thread_create(name, entry, RT_NULL, 512, i * 2, 10);
        if (thread)
            rt_thread_startup(thread);
    }
}
MSH_CMD_EXPORT(start_load_test, start system load test);
