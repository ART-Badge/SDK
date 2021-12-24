/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-10-12     tyx          first implementation
 */

#ifndef __DCMTYPE_H__
#define __DCMTYPE_H__

#include "datatype.h"
#include "dcm.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef PKG_USING_DCM
int dcm_value_set(dcm_pool_t *pool, const char *key, const dt_value_t *v);
const dt_value_t *dcm_value_get(dcm_pool_t *pool, const char *key);

int dcm_set_bool(dcm_pool_t* pool, const char* key, rt_bool_t v);
int dcm_set_double(dcm_pool_t* pool, const char* key, double v);
int dcm_set_float(dcm_pool_t* pool, const char* key, float v);
int dcm_set_int32(dcm_pool_t* pool, const char* key, rt_int32_t v);
int dcm_set_int64(dcm_pool_t* pool, const char* key, rt_int64_t v);
int dcm_set_string(dcm_pool_t* pool, const char* key, const char* v);

rt_bool_t dcm_get_bool(dcm_pool_t* pool, const char* key);
double dcm_get_double(dcm_pool_t* pool, const char* key);
float dcm_get_float(dcm_pool_t* pool, const char* key);
rt_int32_t dcm_get_int32(dcm_pool_t* pool, const char* key);
rt_int64_t dcm_get_int64(dcm_pool_t* pool, const char* key);
const char* dcm_get_string(dcm_pool_t* pool, const char* key);

#ifdef DCM_USING_STORAGE
int dcm_value_set_and_save(dcm_pool_t *pool, const char *key, dt_value_t *v);
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
