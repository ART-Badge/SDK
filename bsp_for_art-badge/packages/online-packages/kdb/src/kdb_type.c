/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-06-09     tyx          first version.
 */

#include "kdb_type.h"

static struct kdb_type tab[KDB_TYPE_NUM];

static struct kdb_type *_type_find(rt_uint16_t type)
{
    int i = 0;

    for (i = 0; i < KDB_TYPE_NUM; i++)
    {
        if (tab[i].type == type)
            return &tab[i];
    }
    return RT_NULL;
}

rt_err_t kdb_type_register(rt_uint16_t type, kdb_dump_t dump)
{
    int i = 0;
    struct kdb_type *t = RT_NULL;

    for (i = 0; i < KDB_TYPE_NUM; i++)
    {
        if (tab[i].type == type)
        {
            t = &tab[i];
            break;
        }
        else if (tab[i].type == KDB_TYPE_INVALID && t == RT_NULL)
        {
            t = &tab[i];
        }
    }
    if (t)
    {
        t->type = type;
        t->dump = dump;
        return RT_EOK;
    }
    return -RT_ERROR;
}

void kdb_type_unregister(rt_uint16_t type)
{
    struct kdb_type *t = _type_find(type);

    if (t)
    {
        t->type = 0;
        t->dump = 0;
    }
}

void kdb_type_dump(kdb_head_t head, void *data)
{
    struct kdb_type *t = _type_find(head->type);

    if (t && t->dump) t->dump(head, data);
}
