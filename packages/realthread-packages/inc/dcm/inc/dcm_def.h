/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-13     chenyong      the first version
 */

#ifndef __DCM_DEF_H__
#define __DCM_DEF_H__

#include <stdlib.h>
#include <string.h>

#include <rthw.h>
#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DCM_SW_VERSION           "0.0.1"
#define DCM_SW_VERSION_NUM       0x000001L

/* magic word(`D`, `C`, `M`, `0`) */
#define DCM_MAGIC_NUM            0x44434d30L

struct dcm_pool;
typedef void (*dcm_load_cb_t)(struct dcm_pool *pool, void *user_data);
typedef void (*dcm_notify_fn_t)(void *user_data);

typedef enum {
    DCM_OK = 0,
    DCM_FAILED,
    DCM_NO_MEM,
    DCM_NAME_ERR,
    DCM_INIT_ERR,
    DCM_PARAM_ERR,
} dcm_err_t;

typedef enum {
    DCM_CB_TYPE_CHANGE = 0,
    DCM_CB_TYPE_GET,
    DCM_CB_TYPE_DELETE,
} dcm_cb_type_t;

struct dcm_blob
{
    void *buf;                            /* blob data buffer */
    size_t len;                           /* blob data length */
};
typedef struct dcm_blob dcm_blob_t;

struct dcm_notify
{
    dcm_cb_type_t type;                   /* callback type */
    dcm_notify_fn_t callback;             /* value change callback function */
    void *user_data;                      /* callback user-data */
    rt_slist_t list;                      /* callback list */
};

struct dcm_cache_node
{
    const char *name;                       /* cache node name */
    struct dcm_blob *blob;                  /* cache blob data */
    rt_slist_t list;                        /* cache node list */
    rt_slist_t cb_list;                     /* cache node callback list */
};
typedef struct dcm_cache_node dcm_cache_node_t;

struct dcm_pool
{
    rt_uint32_t magic;                      /* magic word(`D`, `C`, `M`, `0`) */
    const char *name;                       /* cache pool name */
    rt_uint16_t max_num;                    /* the maximum number of cache node */
    rt_bool_t init_ok;                      /* initialized successfully */

    rt_mutex_t lock;                        /* cache pool operation lock */
    void *tpool;                            /* value change callback thread pool */
    rt_slist_t *clist;                      /* cache node list array */
    rt_slist_t list;                        /* cache pool list */
    void *db_handle;                        /* db handle */
};
typedef struct dcm_pool dcm_pool_t;

#ifdef __cplusplus
}
#endif

#endif /* __DCM_DEF_H__ */
