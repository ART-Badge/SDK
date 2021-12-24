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
    DCM_CB_TYPE_RELEASE             /* notice of release */
} dcm_cb_type_t;

typedef enum {
    DCM_HOOK_POOL_DEINIT = 0,
    DCM_HOOK_POOL_RESET,
    DCM_HOOK_CACHE_BIND,
    DCM_HOOK_CACHE_UNBIND,
    DCM_HOOK_CACHE_PUT,
    DCM_HOOK_CACHE_GET,
    DCM_HOOK_CACHE_CLONE,
    DCM_HOOK_CACHE_DELETE,
    DCM_HOOK_CACHE_NODE_FIND,
    DCM_HOOK_CACHE_BLOB_UPDATE,
}dcm_hook_type_t;

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
    char *name;                             /* cache node name */
    struct dcm_blob *blob;                  /* cache blob data */
    rt_slist_t list;                        /* cache node list */
    rt_slist_t cb_list;                     /* cache node callback list */
};
typedef struct dcm_cache_node dcm_cache_node_t;

struct dcm_pool;
typedef void (*dcm_hook_t)(struct dcm_pool* pool, dcm_hook_type_t type, const char* key);

struct dcm_pool
{
    rt_uint32_t magic;                      /* magic word(`D`, `C`, `M`, `0`) */
    char *name;                             /* cache pool name */
    rt_uint16_t max_num;                    /* the maximum number of cache node */
    rt_bool_t init_ok;                      /* initialized successfully */
    dcm_hook_t hook;                        /* hook function */
    struct rt_mutex lock;                   /* cache pool operation lock */
    void *tpool;                            /* value change callback thread pool */
    rt_slist_t *clist;                      /* cache node list array */
    rt_slist_t list;                        /* cache pool list */
    void *db_handle;                        /* db handle */
    const char *db_name;                    /* db name */
};
typedef struct dcm_pool dcm_pool_t;

#ifdef __cplusplus
}
#endif

#endif /* __DCM_DEF_H__ */
