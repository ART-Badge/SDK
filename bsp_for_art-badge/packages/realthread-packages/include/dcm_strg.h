/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-10-22     tyx          first implementation
 */

#ifndef __DCM_STRG_H__
#define __DCM_STRG_H__

#include "dcm.h"

#ifdef __cplusplus
extern "C" {
#endif

int dcm_bind_with_storage(struct dcm_pool *pool);
int dcm_unbind_with_storage(struct dcm_pool *pool);
int dcm_blob_store(struct dcm_pool *pool, const char *key, const struct dcm_blob *blob);
struct dcm_blob* dcm_blob_load(struct dcm_pool* pool, const char* key);
int dcm_blob_remove(struct dcm_pool *pool, const char *key);
int dcm_blob_clear(struct dcm_pool *pool);

#ifdef __cplusplus
}
#endif

#endif
