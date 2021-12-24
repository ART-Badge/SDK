/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-09     armink       the first version
 */

#include <stdbool.h>
#include "event_recorder.h"
#include <rtthread.h>

#define LOG_TAG "event_recorder"
#define LOG_LVL DBG_INFO
#include <rtdbg.h>

struct event_source {
    uint8_t id;
    const char *name;
    int (*replay)(uint8_t id, const uint8_t *value, size_t value_len);
};

struct er_event {
    union {
        struct {
            uint8_t id;
            uint8_t value_len;
            rt_tick_t interval;
            uint8_t value[EVENT_VALUE_MAX_SIZE];
        } element;
        uint8_t raw[8 + EVENT_VALUE_MAX_SIZE];
    } content;
};

static struct event_source source_tabel[EVENT_SOURCE_MAX_NUM] = { 0 };
static struct rt_mutex locker;
static bool is_init = false, is_recording = false, is_replaing = false;
static rt_tick_t last_trigger_tick = 0, repaly_start = 0, replay_setting_duration = 0;

#ifdef EVENT_RECORDER_USING_FLASHDB
#include <flashdb.h>
/* TSDB object */
struct fdb_tsdb tsdb = { 0 };
#define TSDB_TSL_LEN    1024
#define TSDB_SEC        32768
#define TSDB_SIZE       (TSDB_SEC * 4)
#endif /* EVENT_RECORDER_USING_FLASHDB */

#define LOCK()   rt_mutex_take(&locker, RT_WAITING_FOREVER)
#define UNLOCK() rt_mutex_release(&locker)

int event_recorder_init(void)
{
    if (is_init)
    {
        return 0;
    }

    rt_mutex_init(&locker, "ev_reco", RT_IPC_FLAG_FIFO);
    is_init = true;

    return 0;
}

int event_recorder_control(void)
{
    return 0;
}

static fdb_time_t get_time(void)
{
    static fdb_time_t counts = 0;
    return ++counts;
}

#ifdef EVENT_RECORDER_USING_FLASHDB
static int open_tsdb(const char *dir, const char *name)
{
    bool file_mode = true, rollver = false;
    uint32_t tsdb_len = TSDB_TSL_LEN;
    uint32_t tsdb_sec = TSDB_SEC, tsdb_size = TSDB_SIZE;

    fdb_tsdb_control(&tsdb, FDB_TSDB_CTRL_SET_SEC_SIZE, &tsdb_sec);
    fdb_tsdb_control(&tsdb, FDB_TSDB_CTRL_SET_FILE_MODE, &file_mode);
    fdb_tsdb_control(&tsdb, FDB_TSDB_CTRL_SET_MAX_SIZE, &tsdb_size);
    if (fdb_tsdb_init(&tsdb, name, dir, get_time, tsdb_len, NULL) != FDB_NO_ERR)
    {
        LOG_E("tsdb(%s:%s) create failed", dir, name);
        return -1;
    }
    fdb_tsdb_control(&tsdb, FDB_TSDB_CTRL_SET_ROLLOVER, &rollver);
    return 0;
}
#endif /* EVENT_RECORDER_USING_FLASHDB */

int event_recorder_start(const char *dir, const char *name)
{
    int result = 0;

    RT_ASSERT(is_init);

    LOCK();
    if (!is_recording)
    {
#ifdef EVENT_RECORDER_USING_FLASHDB
        if (open_tsdb(dir, name) != 0)
        {
            result = -1;
        }
        else
#endif /* EVENT_RECORDER_USING_FLASHDB */
        {
            is_recording = true;
        }
    }
    else
    {
        LOG_W("The recorder is already starting. Please STOP before you want to restart.");
    }
    UNLOCK();

    return result;
}

int event_recorder_stop(void)
{
    RT_ASSERT(is_init);

    LOCK();
    if (is_recording)
    {
        is_recording = false;
        last_trigger_tick = 0;
#ifdef EVENT_RECORDER_USING_FLASHDB
        /* close database */
        fdb_tsdb_deinit(&tsdb);
#endif /* EVENT_RECORDER_USING_FLASHDB */
    }
    UNLOCK();

    return 0;
}

int event_source_register(uint8_t id, const char *name,
        int (*replay)(uint8_t id, const uint8_t *value, size_t value_len))
{
    off_t i = 0, empty_index = -1;
    int result = -1;

    RT_ASSERT(is_init);
    RT_ASSERT(id != 0);
    RT_ASSERT(name);
    RT_ASSERT(replay);

    LOCK();
    //TODO 检查是否处于录制或回放中
    for (i = 0; i < EVENT_SOURCE_MAX_NUM; i++)
    {
        /* check the same source */
        if (source_tabel[i].id == id)
        {
            source_tabel[i].name = name;
            source_tabel[i].replay = replay;
            result = 0;
            goto __exit;
        }
        else if (source_tabel[i].id == 0 && empty_index == -1)
        {
            empty_index = i;
        }
    }
    /* not found the same source */
    if (empty_index != -1)
    {
        source_tabel[empty_index].id = id;
        source_tabel[empty_index].name = name;
        source_tabel[empty_index].replay = replay;
        result = 0;
    }

__exit:

    UNLOCK();
    return result;
}

int event_source_trigger(uint8_t event_id, uint8_t *event_value, size_t value_len)
{
    if (!is_init)
    {
        return -1;
    }

    LOCK();
    if (is_recording)
    {
        struct er_event event;
        struct fdb_blob blob;
        rt_tick_t interval = 0;
        fdb_err_t result = FDB_NO_ERR;
        /* calculate the interval from last record */
        rt_tick_t cur_tick = rt_tick_get();
        if (last_trigger_tick == 0) {
            last_trigger_tick = cur_tick;
        }
        interval = cur_tick - last_trigger_tick;
        last_trigger_tick = cur_tick;
        /* make event object */
        event.content.element.id = event_id;
        event.content.element.interval = interval;
        if (value_len > EVENT_VALUE_MAX_SIZE)
        {
            LOG_W("the event value length (%d) is too long", value_len);
            value_len = EVENT_VALUE_MAX_SIZE;
        }
        event.content.element.value_len = value_len;
        memcpy(event.content.element.value, event_value, value_len);
        LOG_D("Record an event (%d), len %d", event_id, value_len);
        /* append the new record */
        result = fdb_tsl_append(&tsdb, fdb_blob_make(&blob, event.content.raw, sizeof(event.content.raw)));
        if (result == FDB_SAVED_FULL)
        {
            LOG_E("Record log is FULL, please increase the TSDB max size.");
        }
        else if (result != FDB_NO_ERR)
        {
            LOG_E("Record log append failed(%d).", result);
        }
    }
    UNLOCK();

    return 0;
}

static bool query_cb(fdb_tsl_t tsl, void *arg)
{
    int *result = (int *)arg;
    struct fdb_blob blob;
    off_t i;

    struct er_event event;
    /* read the event record */
    fdb_blob_read((fdb_db_t) &tsdb,
            fdb_tsl_to_blob(tsl, fdb_blob_make(&blob, event.content.raw, sizeof(event.content.raw))));
    /* delay some time for every event interval */
    rt_thread_delay(event.content.element.interval);
    if (rt_tick_get() - repaly_start <= replay_setting_duration && is_replaing)
    {
        /* replay the event */
        for (i = 0; i < EVENT_SOURCE_MAX_NUM; i++)
        {
            /* check the same source */
            if (source_tabel[i].id == event.content.element.id)
            {
                LOG_D("Replay an event (%d), len %d", event.content.element.id, event.content.element.value_len);
                if (source_tabel[i].replay(event.content.element.id, event.content.element.value,
                        event.content.element.value_len) < 0)
                {
                    LOG_W("Event source %s replay failed. Now will STOP replay.", source_tabel[i].name);
                    *result = -1;
                    return true;
                }
                break;
            }
        }
        return false;
    }
    else
    {
        return true;
    }
}

static void replay_entry(void *parameter)
{
    int result = 0, count = 0;

    repaly_start = rt_tick_get();
    while (rt_tick_get() - repaly_start <= replay_setting_duration && is_replaing && result >= 0)
    {
        LOG_I("Replay count is [%d].", ++count);
        fdb_tsl_iter(&tsdb, query_cb, &result);
    }
    event_replay_stop();
}

int event_replay_start(const char *dir, const char *name, uint32_t time_s)
{
    int result = 0;

    RT_ASSERT(is_init);

    LOCK();
    if (is_recording)
    {
        LOG_W("Event is recording, please STOP record first.");
        result = -1;
        goto __exit;
    }

    if (!is_replaing)
    {
        rt_thread_t tid = rt_thread_create("replay", replay_entry, NULL, 4096, 20, 15);
        if (tid)
        {
            if (open_tsdb(dir, name) == 0)
            {
                if (time_s)
                {
                    replay_setting_duration = rt_tick_from_millisecond(time_s * 1000);
                }
                else
                {
                    replay_setting_duration = UINT32_MAX;
                }
                is_replaing = true;
                rt_thread_startup(tid);
            }
            else
            {
                rt_thread_delete(tid);
                result = -1;
            }
        }
    }

__exit:

    UNLOCK();
    return result;
}

int event_replay_stop(void)
{
    RT_ASSERT(is_init);

    LOCK();
    if (is_replaing)
    {
        is_replaing = false;
        replay_setting_duration = 0;
        /* close database */
        fdb_tsdb_deinit(&tsdb);
    }
    UNLOCK();

    return 0;
}
