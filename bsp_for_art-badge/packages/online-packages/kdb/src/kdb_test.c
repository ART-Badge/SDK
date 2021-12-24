/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-06-09     tyx          first version.
 */

#include <stdlib.h>
#include <rtthread.h>
#include <rtdevice.h>
#include "kdb_core.h"
#include "kdb_type.h"

#define KDB_TEST_TIME           1000 * 10
#define KDB_TEST_BUFF_SIZE      2047
#define KDB_TEST_THREAD         4

#define KDB_TYPE_TEST           0x66

struct test_info
{
    int check;
    char buf[KDB_MAX_DATA_LEN - 4];
};

struct test_obj
{
    rt_bool_t run;
    struct kdb kdb;
    rt_uint8_t *buff;
    struct rt_work work;
    struct rt_semaphore sem;
};

#ifdef KDB_TEST_ENABLE

static int checknum(int crc, const char *buf, int len)
{
    unsigned int byte;
    unsigned char k;
    unsigned short ACC,TOPBIT;
    unsigned short remainder = crc;

    TOPBIT = 0x8000;
    for (byte = 0; byte < len; ++byte)
    {
        ACC = buf[byte];
        remainder ^= (ACC <<8);
        for (k = 8; k > 0; --k)
        {
            if (remainder & TOPBIT)
            {
                remainder = (remainder << 1) ^0x8005;
            }
            else
            {
                remainder = (remainder << 1);
            }
        }
    }
    remainder=remainder ^ 0x0000;

    return remainder;
}

static void kdb_test_thread(void *p)
{
    int len, i;
    struct test_info data;
    struct test_obj *t = p;

    while (t->run)
    {
        len = rand() % (KDB_MAX_DATA_LEN - 4);
        for (i = 0; i < len; i++)
        {
            data.buf[i] = rand() & 0xff;
        }
        data.check = checknum(0, data.buf, len);
        kdb_record(&t->kdb, KDB_TYPE_TEST, &data, len + 4);
    }
    rt_sem_release(&t->sem);
}

static void kdb_test_dump(kdb_head_t head, void *buff)
{
    struct test_info *data = buff;

    if (data->check == checknum(0, data->buf, head->len - KDB_HEAD_SIZE - 4))
    {
        rt_kprintf("check ok\n");
    }
    else
    {
        rt_kprintf("check err\n");
    }
}

static void kdb_test_work(struct rt_work *work, void *p)
{
    struct test_obj *t = p;
    int i = KDB_TEST_THREAD;

    t->run = RT_FALSE;
    kdb_stop(&t->kdb);
    while (i--)
    {
        rt_sem_take(&t->sem, RT_WAITING_FOREVER);
    }
    rt_sem_detach(&t->sem);
    kdb_trace(&t->kdb);
    rt_free(t->buff);
    rt_free(t);
    rt_kprintf("test end\n");
}

void kdb_test(void)
{
    rt_thread_t tid;
    int i;
    char strbuf[16];
    struct test_obj *t;

    srand(rt_tick_get());
    t = rt_malloc(sizeof(struct test_obj));
    t->buff = rt_malloc(KDB_TEST_BUFF_SIZE);

    kdb_type_register(KDB_TYPE_TEST, kdb_test_dump);
    kdb_init(&t->kdb, t->buff, KDB_TEST_BUFF_SIZE);
    kdb_start(&t->kdb);
    t->run = RT_TRUE;
    rt_sem_init(&t->sem, "kdb_sem", 0, RT_IPC_FLAG_FIFO);

    for (i = 0; i < KDB_TEST_THREAD; i++)
    {
        rt_sprintf(strbuf, "kdb%d", i);
        tid = rt_thread_create(strbuf, kdb_test_thread, t, 2048, 27, 2);
        rt_thread_startup(tid);
    }
    rt_work_init(&t->work, kdb_test_work, t);
    rt_work_submit(&t->work, KDB_TEST_TIME);
    rt_kprintf("Wait for %d ms to end automatically\n", KDB_TEST_TIME);
}
MSH_CMD_EXPORT(kdb_test, kdb_test);

#endif
