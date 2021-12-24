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
int dcm_pool_reset(struct dcm_pool* pool);
struct dcm_pool *dcm_pool_find(const char *name);
dcm_hook_t dcm_pool_sethook(struct dcm_pool* pool, dcm_hook_t hook);

/* Data cache management cache node operation */
struct dcm_notify* dcm_cache_bind(struct dcm_pool* pool, const char* key, dcm_cb_type_t type, dcm_notify_fn_t callback, void* user_data);
int dcm_cache_unbind(struct dcm_pool *pool, const char* key, struct dcm_notify* notify);
int dcm_cache_put(struct dcm_pool *pool, const char *key, struct dcm_blob *blob);
const struct dcm_blob *dcm_cache_get(struct dcm_pool *pool, const char *key);
int dcm_cache_clone(struct dcm_pool *pool, const char *key, struct dcm_blob *blob);
int dcm_cache_delete(struct dcm_pool *pool, const char *key);
struct dcm_cache_node *dcm_cache_node_find(struct dcm_pool *pool, const char * key);
int dcm_cache_blob_update(struct dcm_pool *pool, const char *key, struct dcm_blob *blob /* It must be alloc blob */);

/* Blob format data operation */
struct dcm_blob *dcm_blob_make(struct dcm_blob *blob, void *buf, size_t len);
struct dcm_blob *dcm_blob_alloc(rt_size_t len);
void dcm_blob_free(struct dcm_blob *blob);

struct dcm_blob *dcm_blob_duplicate(const struct dcm_blob *blob);
struct dcm_blob *dcm_blob_copy(struct dcm_blob *des, const struct dcm_blob *src);
int dcm_blob_compare(const struct dcm_blob *blob1, const struct dcm_blob *blob2);

rt_err_t dcm_pool_lock(struct dcm_pool* pool);
void dcm_pool_unlock(struct dcm_pool* pool);

#ifdef DCM_USING_STORAGE
/* Data cache management data storage */
int dcm_cache_save(struct dcm_pool *pool, const char *key);
int dcm_cache_reload(struct dcm_pool* pool, const char* key);
int dcm_cache_set_and_save(struct dcm_pool *pool, const char *key, struct dcm_blob *blob);
int dcm_cache_del_and_save(struct dcm_pool *pool, const char *key);
#endif /* DCM_USING_STORAGE */

#ifdef __cplusplus
}
#endif

#endif /* __DCM_H__ */
