/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-13     chenyong      the first version
 */

#ifndef __DCM_H__
#define __DCM_H__

#include <dcm_def.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Data cache management pool operation */
int dcm_pool_init(struct dcm_pool *pool, dcm_load_cb_t cb, void *user_data);
void dcm_pool_deinit(struct dcm_pool *pool);
struct dcm_pool *dcm_pool_find(const char *name);

/* Data cache management cache node operation */
struct dcm_notify* dcm_cache_bind(struct dcm_pool* pool, const char* key, dcm_cb_type_t type, dcm_notify_fn_t callback, void* user_data);
int dcm_cache_unbind(struct dcm_pool *pool, const char* key, struct dcm_notify* notify);
int dcm_cache_put(struct dcm_pool *pool, const char *key, struct dcm_blob *blob);
struct dcm_blob *dcm_cache_get(struct dcm_pool *pool, const char *key);
int dcm_cache_clone(struct dcm_pool *pool, const char *key, struct dcm_blob *blob);
int dcm_cache_delete(struct dcm_pool *pool, const char *key);
struct dcm_cache_node *dcm_cache_node_find(struct dcm_pool *pool, const char *name);

/* Blob format data operation */
struct dcm_blob *dcm_blob_make(struct dcm_blob *blob, void *buf, size_t len);
struct dcm_blob *dcm_blob_make_form_bson(struct dcm_blob *blob, rt_uint8_t *bson_data);

#ifdef DCM_USING_STORAGE
/* Data cache management data storage */
int dcm_cache_save(struct dcm_pool *pool, const char *key);
int dcm_cache_set_and_save(struct dcm_pool *pool, const char *key, struct dcm_blob *blob);
int dcm_cache_del_and_save(struct dcm_pool *pool, const char *key);
#endif /* DCM_USING_STORAGE */

#ifdef __cplusplus
}
#endif

#endif /* __DCM_H__ */
