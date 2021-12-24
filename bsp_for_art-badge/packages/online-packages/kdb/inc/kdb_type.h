/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-06-09     tyx          first version.
 */

#ifndef __KDB_TYPE_H__
#define __KDB_TYPE_H__

#include <rtthread.h>
#include "kdb_core.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef KDB_TYPE_NUM
#define KDB_TYPE_NUM        (8)
#endif

#define KDB_TYPE_INVALID    (0)

typedef void(*kdb_dump_t)(kdb_head_t head, void *data);
struct kdb_type
{
    rt_uint16_t type;
    kdb_dump_t dump;
};

rt_err_t kdb_type_register(rt_uint16_t type, kdb_dump_t dump);
void kdb_type_unregister(rt_uint16_t type);
void kdb_type_dump(kdb_head_t head, void *data);

#ifdef __cplusplus
}
#endif

#endif
