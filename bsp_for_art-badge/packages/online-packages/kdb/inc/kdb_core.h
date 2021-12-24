/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-06-09     tyx          first version.
 */

#ifndef __KDB_CORE_H__
#define __KDB_CORE_H__

#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#define KDB_HEAD_SIZE           (sizeof(struct kdb_head))
#define KDB_ITEM_MAGIC          (0x5A)
#define KDB_FLAG_START          (0x01 << 0)

#ifndef KDB_MAX_DATA_LEN
#define KDB_MAX_DATA_LEN        (64)
#endif

#ifndef KDB_MEMCPY
#define KDB_MEMCPY              rt_memcpy
#endif

#define KDB_CCODE(_kdb)         (_kdb)->ccode = kdb_ccode(_kdb)

/*
* +-----------------+------+------+-------+-----+
* |       data      | tick | type | magic | len |
* +-----------------+------+------+-------+-----+
* data  - save user data
* tick  - tick stamp, 4bytes
* type  - 1bytes, flag
* magic - 1bytes, type id
* len   - 2bytes, the length of data + header.
*/

struct kdb_head
{
    rt_uint32_t tick;
    rt_uint8_t  type;
    rt_uint8_t  magic;
    rt_uint16_t len;
};

typedef struct kdb_head *kdb_head_t;

struct kdb
{
    rt_uint32_t ccode;
    rt_uint32_t curr_idx;
    rt_uint32_t flags;
    rt_int32_t blen;
    rt_uint8_t *buff;
    void *data_buff;
};
typedef struct kdb *kdb_t;

void kdb_record(kdb_t kdb, rt_uint16_t type, void *buff, rt_uint16_t len);
void kdb_trace(kdb_t kdb);
rt_uint32_t kdb_ccode(kdb_t kdb);
int kdb_isvalid(kdb_t kdb);

//kdb port

rt_base_t kdb_enter_critical(void);
void kdb_exit_critical(rt_base_t level);

rt_inline void kdb_init(kdb_t kdb, void *buff, rt_int32_t len)
{
    if (kdb)
    {
        rt_memset(buff, 0, len);
        kdb->curr_idx = 0;
        kdb->buff = buff;
        kdb->blen = len - KDB_MAX_DATA_LEN;
        kdb->flags = 0;
        kdb->data_buff = ((rt_uint8_t *)buff) + len - KDB_MAX_DATA_LEN;
    }
}

rt_inline void kdb_start(kdb_t kdb)
{
    if (kdb)
        kdb->flags |= KDB_FLAG_START;
}

rt_inline void kdb_stop(kdb_t kdb)
{
    if (kdb)
        kdb->flags &= ~KDB_FLAG_START;
}

rt_inline int kdb_isactive(kdb_t kdb)
{
    if (kdb)
        return !!(kdb->flags & KDB_FLAG_START);
    else
        return 0;
}

#ifdef __cplusplus
}
#endif

#endif
