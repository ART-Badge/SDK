/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-06-09     tyx          first version.
 */

#include <string.h>
#include "kdb_core.h"
#include "kdb_type.h"

rt_inline rt_uint32_t kdb_walk(kdb_t kdb, int len)
{
    register rt_base_t level;
    rt_uint32_t next_idx;
    rt_uint32_t res;

    level = kdb_enter_critical();
    res = kdb->curr_idx;
    next_idx = kdb->curr_idx + len;
    if (next_idx < kdb->blen)
        kdb->curr_idx = next_idx;
    else
        kdb->curr_idx = next_idx - kdb->blen;
    kdb_exit_critical(level);
    return res;
}

rt_inline rt_uint32_t _write(kdb_t kdb, rt_uint32_t idx,
    void *buf, rt_uint16_t len)
{
    rt_uint8_t *tmp = buf;

    if (idx + len < kdb->blen)
    {
        KDB_MEMCPY(&kdb->buff[idx], tmp, len);
        idx += len;
    }
    else
    {
        KDB_MEMCPY(&kdb->buff[idx], tmp, kdb->blen - idx);
        tmp += kdb->blen - idx;
        idx = len - (kdb->blen - idx);
        KDB_MEMCPY(&kdb->buff[0], tmp, idx);
    }
    return idx;
}

rt_inline rt_uint32_t _read(kdb_t kdb, rt_uint32_t idx, void *buf, rt_uint16_t len)
{
    rt_uint8_t *tmp = buf;
    rt_uint32_t size;

    if (idx >= len)
    {
        KDB_MEMCPY(tmp, &kdb->buff[idx - len], len);
        idx -= len;
    }
    else
    {
        size = len - idx;
        KDB_MEMCPY(&tmp[size], &kdb->buff[0], idx);
        KDB_MEMCPY(tmp, &kdb->buff[kdb->blen - size], size);
        idx = kdb->blen - size;
    }
    return idx;
}

void kdb_record(kdb_t kdb, rt_uint16_t type, void *buff, rt_uint16_t len)
{
    rt_uint32_t idx;
    struct kdb_head head;
    rt_uint16_t size = len + KDB_HEAD_SIZE;

    if (!kdb_isactive(kdb) || len > KDB_MAX_DATA_LEN)
        return;

    idx = kdb_walk(kdb, size);
    head.magic = KDB_ITEM_MAGIC;
    head.type = type;
    head.len = size;
    head.tick = rt_tick_get();
    idx = _write(kdb, idx, buff, len);
    _write(kdb, idx, &head, KDB_HEAD_SIZE);
}

void kdb_trace(kdb_t kdb)
{
    rt_uint8_t *buff = kdb->data_buff;
    rt_uint32_t last_idx;
    struct kdb_head head;
    int len;

    if (kdb_isactive(kdb))
        return;
    len = kdb->blen;
    last_idx = kdb->curr_idx;
    while (len > 0)
    {
        last_idx = _read(kdb, last_idx, &head, KDB_HEAD_SIZE);
        if (head.len > KDB_MAX_DATA_LEN + KDB_HEAD_SIZE ||
            head.magic != KDB_ITEM_MAGIC ||
            head.len > len)
        {
            break;
        }
        last_idx = _read(kdb, last_idx, buff, head.len - KDB_HEAD_SIZE);
        kdb_type_dump(&head, buff);
        len -= head.len;
    }
}

static rt_uint32_t _ccode(rt_uint16_t ctx, rt_uint8_t *data, int len)
{
    int i;

    for(i = 0; i < len; i++)
    {
        ctx = (ctx * 16777619) ^ data[i];
    }
    return ctx;
}

rt_uint32_t kdb_ccode(kdb_t kdb)
{
    rt_uint32_t ctx = 2166136261;

    ctx = _ccode(ctx, (rt_uint8_t *)kdb->buff, kdb->blen);
    ctx = _ccode(ctx, (rt_uint8_t *)&kdb->curr_idx, sizeof(kdb->curr_idx));
    ctx = _ccode(ctx, (rt_uint8_t *)&kdb->blen, sizeof(kdb->blen));
    return ctx;
}

int kdb_isvalid(kdb_t kdb)
{
    return kdb_ccode(kdb) == kdb->ccode;
}
