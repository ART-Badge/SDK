/*
 * Copyright (c) 2006-202, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-05-15     chenyong     first version
 */

#ifndef __WATCH_DCM_SVC_H__
#define __WATCH_DCM_SVC_H__

#include <watch_dcm_cfg.h>

struct watch_dcm_item
{
    const char *pool;
    const char *key;
    union
    {
        rt_int32_t v_int32;
        double v_double;
        float v_float;
        rt_bool_t v_bool;
        rt_int64_t v_int64;
        char *v_str;
    } value;
    rt_bool_t need_strg;
    dt_type_t type;
};
typedef struct watch_dcm_item *watch_dcm_item_t;

/* Initialize watch dcm data service */
int watch_dcm_svc_init(const struct watch_dcm_item *items_table, rt_size_t table_size);

#ifdef DCM_USING_STORAGE
int watch_dcm_strg_add(struct dcm_pool *pool, const char *key);
int watch_dcm_strg_del(struct dcm_pool *pool, const char *key);
#endif

#endif /* __WATCH_DCM_SVC_H__ */
