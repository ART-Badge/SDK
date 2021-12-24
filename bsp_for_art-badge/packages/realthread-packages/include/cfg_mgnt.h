/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-09-15     tyx          first implementation
 */

#ifndef __CFG_MGNT_H__
#define __CFG_MGNT_H__

#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CFGM_STORAGE_POOL_NAME
#define CFGM_STORAGE_POOL_NAME  ("cfgmpool")
#endif

typedef enum {
    CFGM_SYS_TYPE = 0,
    CFGM_APP_TYPE = 1,
    CFGM_UNKNOWN = 0xFFFF,
}cfgm_type_t;

/* 配置项 */
struct cfgm_object
{
    char *item_ptr;
};

struct cfgm_items
{
    const char *item;
    const char *value;
    rt_err_t (*callback)(const struct cfgm_items *, const char *);
};

struct cfgm_table
{
    rt_slist_t node;
    const struct cfgm_items *items;
    rt_uint16_t cnt;
    cfgm_type_t type;
};

rt_err_t cfgm_set_system_config(const char *item, const char *pstr);
rt_err_t cfgm_get_system_config(const char *item, char *pstr , rt_size_t size);
long cfgm_system_config_length(const char *item);

struct cfgm_object *cfgm_app_get(const char *url);
void cfgm_app_put(struct cfgm_object *app);
rt_err_t cfgm_set_app_config(struct cfgm_object *app, const char *item, char *value);
rt_err_t cfgm_get_app_config(struct cfgm_object *app, const char *item, char *value, rt_size_t size);
long cfgm_app_config_length(struct cfgm_object* app, const char* item);

int cfgm_init(struct cfgm_table *default_table);
void cfgm_table_append(struct cfgm_table *table);


#ifdef __cplusplus
}
#endif

#endif
