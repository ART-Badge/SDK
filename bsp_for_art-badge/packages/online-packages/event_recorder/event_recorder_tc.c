/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-09     armink       the first version
 */

#include <rtthread.h>
#include "event_recorder.h"

#ifdef RT_USING_UTEST
#include <utest.h>
#if !defined(_MSC_VER)
#include <sys/unistd.h>
#ifndef RT_USING_POSIX
#error "please enable the RT_USING_POSIX"
#endif
#endif

#include <dfs_posix.h>

#define RECORD_DIR  "/logs/event_recorder_test/"
#define RECORD_NAME "testcase"

#define EVENT_ID   1
#define EVENT_NAME "test"

#define EVENT_RECORD_TRIGGER_NUM      32
#define EVENT_RECORD_DELAY_MULTIPLE   5
#define EVENT_RECORD_TIME_ERROR_RANGE 5

#define REPLAY_TIME_S 60

static size_t replay_count = 0;
static rt_tick_t replay_last_tick = 0;
static rt_sem_t replay_stop_notice;

static int replay(uint8_t id, const uint8_t *value, size_t value_len)
{
    uint32_t count = *((uint32_t *) value);
    rt_tick_t interval = 0;
    rt_tick_t cur_tick = rt_tick_get();

    if (replay_last_tick == 0)
    {
        replay_last_tick = cur_tick;
    }
    interval = cur_tick - replay_last_tick;
    replay_last_tick = cur_tick;

    replay_count ++;
    /* only test a replay loop */
    if (replay_count <= EVENT_RECORD_TRIGGER_NUM)
    {
        uassert_int_equal(replay_count, count);
        if (interval)
        {
//            LOG_D("%d %d %d", replay_count, interval, replay_count * EVENT_RECORD_DELAY_MULTIPLE);
            uassert_true(interval >= replay_count * EVENT_RECORD_DELAY_MULTIPLE - EVENT_RECORD_TIME_ERROR_RANGE);
        }
    }
    else
    {
        rt_sem_release(replay_stop_notice);
    }

    return 0;
}

static void test_event_recorder_init(void)
{
    uassert_int_equal(event_recorder_init(), 0);
}

static void test_event_source_register(void)
{
    uassert_int_equal(event_source_register(EVENT_ID, EVENT_NAME, replay), 0);
}

static void test_event_recorder_start(void)
{
    uassert_int_equal(event_recorder_start(RECORD_DIR, RECORD_NAME), 0);
}

static void test_event_source_trigger(void)
{
    uint32_t trigger_num = 0;

    while (trigger_num++ < EVENT_RECORD_TRIGGER_NUM)
    {
        uassert_int_equal(event_source_trigger(EVENT_ID, (uint8_t *)&trigger_num, sizeof(trigger_num)), 0);
        rt_thread_delay(trigger_num * EVENT_RECORD_DELAY_MULTIPLE);
    }
}

static void test_event_recorder_stop(void)
{
    uassert_int_equal(event_recorder_stop(), 0);
}

static void test_event_replay_start(void)
{
    uassert_int_equal(event_replay_start(RECORD_DIR, RECORD_NAME, REPLAY_TIME_S), 0);
}

static void test_event_replay_stop(void)
{
    if (rt_sem_take(replay_stop_notice,
            EVENT_RECORD_TRIGGER_NUM * EVENT_RECORD_TRIGGER_NUM * EVENT_RECORD_DELAY_MULTIPLE) == RT_EOK)
    {
        uassert_int_equal(event_replay_stop(), 0);
        /* delay some time for wait stop finish */
        rt_thread_delay(5 * EVENT_RECORD_TRIGGER_NUM * EVENT_RECORD_DELAY_MULTIPLE);
        uassert_true(replay_count <= EVENT_RECORD_TRIGGER_NUM + 2);
    }
    else
    {
        uassert_int_equal(event_replay_stop(), 0);
        uassert_true(RT_FALSE);
    }
}

static rt_err_t utest_tc_init(void)
{
    replay_count = 0;
    replay_last_tick = 0;

    unlink(RECORD_DIR""RECORD_NAME);
    unlink(RECORD_DIR""RECORD_NAME".fdb.0");
    unlink(RECORD_DIR""RECORD_NAME".fdb.1");
    unlink(RECORD_DIR""RECORD_NAME".fdb.2");
    unlink(RECORD_DIR""RECORD_NAME".fdb.3");
    chdir("/");
    mkdir(RECORD_DIR, 0);
    chdir("/");

    replay_stop_notice = rt_sem_create("event_test", 0, RT_IPC_FLAG_FIFO);

    if (replay_stop_notice)
    {
        return RT_EOK;
    }
    else
    {
        return -RT_ENOMEM;
    }
}

static rt_err_t utest_tc_cleanup(void)
{
    unlink(RECORD_DIR""RECORD_NAME);
    unlink(RECORD_DIR""RECORD_NAME".fdb.0");
    unlink(RECORD_DIR""RECORD_NAME".fdb.1");
    unlink(RECORD_DIR""RECORD_NAME".fdb.2");
    unlink(RECORD_DIR""RECORD_NAME".fdb.3");
    unlink(RECORD_DIR);
    return RT_EOK;
}

static void testcase(void)
{
    UTEST_UNIT_RUN(test_event_recorder_init);
    UTEST_UNIT_RUN(test_event_source_register);
    UTEST_UNIT_RUN(test_event_recorder_start);
    UTEST_UNIT_RUN(test_event_source_trigger);
    UTEST_UNIT_RUN(test_event_recorder_stop);
    UTEST_UNIT_RUN(test_event_replay_start);
    UTEST_UNIT_RUN(test_event_replay_stop);
}

UTEST_TC_EXPORT(testcase, "packages.system.event_recorder", utest_tc_init, utest_tc_cleanup, 50);

#endif /* RT_USING_UTEST */
