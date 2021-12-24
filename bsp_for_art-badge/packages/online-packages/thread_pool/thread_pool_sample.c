/*
 * This file is part of the EasyDataManager Library.
 *
 * Copyright (C) 2019 by Armink <armink.ztl@gmail.com>
 *
 * Function: a thread pool base on RT-Thread
 * Created on: 2019-03-29
 */

#include <stdlib.h>

#define DBG_SECTION_NAME               "thread_pool.sample"
#define DBG_LEVEL                      DBG_LOG
#include <rtdbg.h>

#ifdef PKG_USING_THREAD_POOL

#include <finsh.h>
#include "thread_pool.h"

static void task(void *arg) {
    LOG_I("The task on thread %.*s is running.", RT_NAME_MAX, rt_thread_self()->name);
    rt_thread_delay(rt_tick_from_millisecond((uint32_t)arg));
    LOG_I("The task on thread %.*s will finish.", RT_NAME_MAX, rt_thread_self()->name);
}

static void thread_pool_sample(uint8_t argc, char **argv) {
    thread_pool pool;

    init_thread_pool(&pool, "sam", 3, 1024);
    /* add 5 task to thread pool */
    pool.add_task(&pool, task, (void *)(rand() % 5000));
    pool.add_task(&pool, task, (void *)(rand() % 5000));
    pool.add_task(&pool, task, (void *)(rand() % 5000));
    pool.add_task(&pool, task, (void *)(rand() % 5000));
    pool.add_task(&pool, task, (void *)(rand() % 5000));
    /* wait 10S */
    rt_thread_delay(rt_tick_from_millisecond(10 * 1000));
    /* delete all task */
    pool.del_all(&pool);
    /* destroy the thread pool */
    pool.destroy(&pool);
}
MSH_CMD_EXPORT(thread_pool_sample, Run thread pool sample);

#endif /* PKG_USING_THREAD_POOL */
