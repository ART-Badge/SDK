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

#include "rtthread.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DT_INLINE_DATA_LEN
#define DT_INLINE_DATA_LEN 128
#endif

typedef enum
{
    DT_TYPE_NULL   = 0x00,
    DT_TYPE_DOUBLE = 0x01,
    DT_TYPE_BOOL   = 0x02,
    DT_TYPE_INT32  = 0x03,
    DT_TYPE_INT64  = 0x04,
    DT_TYPE_STRING = 0x05,
    DT_TYPE_JSON   = 0x06,
    DT_TYPE_BSON   = 0x07,
} dt_type_t;

typedef enum
{
    DT_FLAG_ALLOC  = 0,
    DT_FLAG_INLINE = 1,
    DT_FLAG_RDONLY = 2,
    DT_FLAG_UNKNOWN = 0x7FFFFFFF,   /* align 4 */
} dt_flag_t;

typedef struct _dt_value_t
{
    dt_flag_t flags;
    rt_uint32_t type:8;
    rt_uint32_t len:24;
    union
    {
        rt_int64_t v_int64;
        rt_int32_t v_int32;
        double v_double;
        rt_bool_t v_bool;
        struct
        {
            char str[1];
        } v_str;
        struct
        {
            char json[1];
        } v_json;
        struct
        {
            rt_uint8_t bson[1];
        } v_bson;
        rt_uint8_t inline_data[DT_INLINE_DATA_LEN];
    } value;
} dt_value_t;

typedef struct
{
    dt_flag_t flags;
    rt_uint32_t type:8;
    rt_uint32_t len:24;
    rt_uint8_t data[DT_INLINE_DATA_LEN];
} dt_inline_t;

typedef struct
{
    dt_flag_t flags;
    rt_uint32_t type:8;
    rt_uint32_t len:24;
    rt_uint8_t buff[];
} dt_alloc_t;

#define DT_IS_BOOL(_v)      (_v)->type == DT_TYPE_BOOL
#define DT_IS_DOUBLE(_v)    (_v)->type == DT_TYPE_DOUBLE
#define DT_IS_INT32(_v)     (_v)->type == DT_TYPE_INT32
#define DT_IS_INT64(_v)     (_v)->type == DT_TYPE_INT64
#define DT_IS_STRING(_v)    (_v)->type == DT_TYPE_STRING
#define DT_IS_BSON(_v)      (_v)->type == DT_TYPE_BSON
#define DT_IS_JSON(_v)      (_v)->type == DT_TYPE_JSON

dt_value_t *dt_value_init(dt_value_t *dt_value);
dt_value_t *dt_value_new(rt_size_t size);
void dt_value_free(dt_value_t *dt_value);
rt_size_t dt_get_len(dt_value_t *v);
dt_type_t dt_get_type(dt_value_t *v);

rt_bool_t dt_pack_bool(dt_value_t *dt_value, rt_bool_t v);
rt_bool_t dt_pack_double(dt_value_t *dt_value, double v);
rt_bool_t dt_pack_int32(dt_value_t *dt_value, rt_int32_t v);
rt_bool_t dt_pack_int64(dt_value_t *dt_value, rt_int64_t v);
rt_bool_t dt_pack_string(dt_value_t *dt_value, const char *v);
rt_bool_t dt_pack_json(dt_value_t *dt_value, const char *v);
rt_bool_t dt_pack_bson(dt_value_t *dt_value, const rt_uint8_t *v);

rt_bool_t dt_unpack_bool(dt_value_t *v);
double dt_unpack_double(dt_value_t *v);
rt_int32_t dt_unpack_int32(dt_value_t *v);
rt_int64_t dt_unpack_int64(dt_value_t *v);
const char *dt_unpack_string(dt_value_t *v);
const rt_uint8_t *dt_unpack_bson(dt_value_t *v);
const char *dt_unpack_json(dt_value_t *v);

#ifdef PKG_USING_DCM
#include "dcm.h"
int dcm_value_set(dcm_pool_t *pool, const char *key, dt_value_t *v);
dt_value_t *dcm_value_get(dcm_pool_t *pool, const char *key);
#ifdef DCM_USING_STORAGE
int dcm_value_set_and_save(dcm_pool_t *pool, const char *key, dt_value_t *v);
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
