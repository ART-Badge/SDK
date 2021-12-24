/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-10-12     tyx          first implementation
 */

#ifndef __DATATYPE_H__
#define __DATATYPE_H__

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
    DT_TYPE_FLOAT  = 0x02,
    DT_TYPE_BOOL   = 0x03,
    DT_TYPE_INT32  = 0x04,
    DT_TYPE_INT64  = 0x05,
    DT_TYPE_STRING = 0x06,
    DT_TYPE_JSON   = 0x07,
    DT_TYPE_BSON   = 0x08,
    DT_TYPE_UBJSON = 0x09,
    DT_TYPE_ARRAY  = 0x7E,
    DT_TYPE_OBJECT = 0x7F,
    DT_TYPE_UNDEFINED = 0xFF,
} dt_type_t;

#define DT_MEM_TYPE_UNKNOWN 0
#define DT_MEM_TYPE_INLINE  1
#define DT_MEM_TYPE_ALLOC   2
#define DT_MEM_TYPE_RDONLY  3
#define DT_MEM_TYPE_MAKE    4

struct _dt_value_attr
{
    rt_uint64_t memtype:8;
    rt_uint64_t type:8;
    rt_uint64_t memlen:24;
    rt_uint64_t len:24;
};

typedef rt_base_t dt_doc_t;

typedef struct _dt_value_t
{
    struct _dt_value_attr attr;
    union
    {
        rt_int64_t v_int64;
        rt_int32_t v_int32;
        double v_double;
        float v_float;
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
        struct
        {
            rt_uint8_t ubjson[1];
        } v_ubjson;
        dt_doc_t doc;
        rt_uint8_t inline_data[DT_INLINE_DATA_LEN];
    } value;
} dt_value_t;

struct dt_document_t
{
    rt_base_t item;
    dt_value_t *value;
    struct dt_document_t *next;
};

#define DT_HEAD_LENGTH  ((rt_uint32_t)(&((dt_value_t *)0)->value))

#define DT_IS_BOOL(_v)      ((_v)->attr.type == DT_TYPE_BOOL)
#define DT_IS_DOUBLE(_v)    ((_v)->attr.type == DT_TYPE_DOUBLE)
#define DT_IS_INT32(_v)     ((_v)->attr.type == DT_TYPE_INT32)
#define DT_IS_INT64(_v)     ((_v)->attr.type == DT_TYPE_INT64)
#define DT_IS_STRING(_v)    ((_v)->attr.type == DT_TYPE_STRING)
#define DT_IS_BSON(_v)      ((_v)->attr.type == DT_TYPE_BSON)
#define DT_IS_JSON(_v)      ((_v)->attr.type == DT_TYPE_JSON)
#define DT_IS_UBJSON(_v)    ((_v)->attr.type == DT_TYPE_UBJSON)

dt_value_t *dt_value_init(dt_value_t *dt_value);
dt_value_t *dt_value_new(rt_size_t size);
void dt_value_free(dt_value_t *dt_value);

dt_value_t *dt_value_clone(const dt_value_t *v);
dt_value_t *dt_value_make(void *buff, size_t size);
dt_value_t *dt_value_copy(dt_value_t *des, const dt_value_t *src);
int dt_value_compare(const dt_value_t *v1, const dt_value_t *v2);

rt_uint8_t dt_memtype_get(const dt_value_t *v);
rt_size_t dt_get_len(const dt_value_t *v);
dt_type_t dt_get_type(const dt_value_t *v);

rt_bool_t dt_pack_bool(dt_value_t *dt_value, rt_bool_t v);
rt_bool_t dt_pack_double(dt_value_t *dt_value, double v);
rt_bool_t dt_pack_float(dt_value_t *dt_value, float v);
rt_bool_t dt_pack_int32(dt_value_t *dt_value, rt_int32_t v);
rt_bool_t dt_pack_int64(dt_value_t *dt_value, rt_int64_t v);
rt_bool_t dt_pack_string(dt_value_t *dt_value, const char *v);
rt_bool_t dt_pack_json(dt_value_t *dt_value, const char *v);
rt_bool_t dt_pack_bson(dt_value_t *dt_value, const rt_uint8_t *v);
rt_bool_t dt_pack_ubjson(dt_value_t *dt_value, const void *v, rt_size_t len);

rt_bool_t dt_unpack_bool(const dt_value_t *v);
double dt_unpack_double(const dt_value_t *v);
float dt_unpack_float(const dt_value_t *v);
rt_int32_t dt_unpack_int32(const dt_value_t *v);
rt_int64_t dt_unpack_int64(const dt_value_t *v);
const char *dt_unpack_string(const dt_value_t *v);
const rt_uint8_t *dt_unpack_bson(const dt_value_t *v);
const char *dt_unpack_json(const dt_value_t *v);
const void *dt_unpack_ubjson(const dt_value_t *v);

void *datatype_malloc(rt_size_t size);
void *datatype_realloc(void *rmem, rt_size_t newsize);
void datatype_free(void *rmem);
char *datatype_strdup(const char *s);

#define DT_DOC_VALUE_NORMAL  (0)
#define DT_DOC_VALUE_ARRAY  (1)
#define DT_DOC_VALUE_OBJECT (2)

void dt_doc_destroy(dt_doc_t doc);
int dt_doc_length(dt_doc_t doc);
dt_type_t dt_doc_type(dt_doc_t doc);
dt_doc_t dt_doc_clone(dt_doc_t doc);
int dt_doc_compare(dt_doc_t v1, dt_doc_t v2);
const dt_value_t *dt_doc_iterator(dt_doc_t doc, rt_base_t *ctx);
const dt_value_t *dt_doc_find(dt_doc_t doc, const char *item);
dt_doc_t dt_doc_remove(dt_doc_t doc, const dt_value_t *value);
/* array */
const dt_value_t *dt_array_get(dt_doc_t doc, int idx);
rt_bool_t dt_array_set(dt_doc_t doc, dt_value_t *value);
rt_bool_t dt_array_add_string(dt_doc_t doc, const char *v);
rt_bool_t dt_array_add_int32(dt_doc_t doc, rt_int32_t v);
rt_bool_t dt_array_add_doc(dt_doc_t doc, dt_doc_t v);
rt_bool_t dt_array_add_bool(dt_doc_t doc, rt_bool_t v);
rt_bool_t dt_array_add_float(dt_doc_t doc, float v);
rt_bool_t dt_array_add_double(dt_doc_t doc, double v);

/* object */
const dt_value_t *dt_object_get(dt_doc_t doc, const char *key);
rt_bool_t dt_object_set(dt_doc_t doc, const char *key, dt_value_t *value);
rt_bool_t dt_object_add_string(dt_doc_t doc, const char *key, const char *v);
rt_bool_t dt_object_add_int32(dt_doc_t doc, const char *key, rt_int32_t v);
rt_bool_t dt_object_add_doc(dt_doc_t doc, const char *key, dt_doc_t v);
rt_bool_t dt_object_add_bool(dt_doc_t doc, const char *key, rt_bool_t v);
rt_bool_t dt_object_add_float(dt_doc_t doc, const char *key, float v);
rt_bool_t dt_object_add_double(dt_doc_t doc, const char *key, double v);

/*  */
dt_value_t *dt_ubjson_to_doc(const void *buff, int length);
int dt_doc_to_ubjson(const dt_value_t *dt_value, void **ubjson, int *length);
dt_value_t *dt_json_to_doc(const char *jstr);
int dt_doc_to_json(const dt_value_t *dt_value, char **json, int *length);

/* internal  */
struct dt_document_t * dt_doc2document(dt_doc_t doc);
dt_doc_t dt_document2doc(struct dt_document_t *document, dt_type_t type);
struct dt_document_t *dt_document_new(void);

rt_bool_t dt_pack_doc(dt_value_t *dt_value, dt_doc_t doc);
dt_doc_t dt_unpack_doc(const dt_value_t *v);

#ifdef __cplusplus
}
#endif

#endif
