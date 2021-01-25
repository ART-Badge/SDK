/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-25     chenyong     first version
 */

#ifndef __URPC_H__
#define __URPC_H__

#include <stdlib.h>
#include <string.h>

#include <rtthread.h>
#include <rthw.h>

#ifdef URPC_USING_CJSON
#include <cJSON.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif


#define URPC_SW_VER                    "0.1.0"
#define URPC_SW_VER_NUM                0x000100

#define URPC_SVC_VER_NUM               0x1

#define URPC_SVC_FLAG_NEED_RSP         0x01
#define URPC_SVC_FLAG_NEED_ACK         0x02

/* uRPC data object */
struct urpc_blob
{
    void *buf;                         /* raw buffer */
    size_t len;                        /* current data length */
    size_t size;                       /* maximum data size */
};

/* uRPC service execute function */
typedef struct urpc_blob *(*urpc_svc_fn)(struct urpc_blob *input);

/* uRPC service object */
struct urpc_svc
{
    uint8_t ver;                       /* version */
    const char *name;                  /* service name */
    urpc_svc_fn fn;                    /* service function*/
    rt_slist_t list;                   /* service list */
};

/* uRPC object */
struct urpc_object
{
    uint8_t id;                        /* device ID */
    rt_thread_t parser;                /* receive service parser thread */

    void *user_data;                   /* user-specific data */
};

/* uRPC initialize and start */
int urpc_init(uint8_t id);
void urpc_start(void);

/* uRPC service static register */
#define URPC_SVC_STATIC_REGISTER(name)                             \
    const char urpc_svc_##name##_name[] = #name;                   \
    const struct urpc_svc urpc_svc_##name SECTION("uRPCSvcTab")=   \
    {                                                              \
        URPC_SVC_VER_NUM,                                          \
        urpc_svc_##name##_name,                                    \
        (urpc_svc_fn)&name                                         \
    };

#define URPC_FFI_FUNC_STATIC_REGISTER(name)                        \
    const char urpc_svc_##name##_name[] = "ffi."#name;               \
    const struct urpc_svc urpc_svc_##name SECTION("uRPCSvcTab")=   \
    {                                                              \
        URPC_SVC_VER_NUM,                                          \
        urpc_svc_##name##_name,                                    \
        (urpc_svc_fn)&name                                         \
    };

/* uRPC service dynamic register */
int urpc_svc_register(const char *name, urpc_svc_fn svc_fn);
int urpc_ffi_func_register(const char *func_name, void *func_addr);

/* uRPC service execute */
int urpc_svc_exec(uint8_t dst_id, const char *name, uint16_t flag,
        struct urpc_blob *input, struct urpc_blob *output, int32_t timeout);

/* uRPC blob object operates */
struct urpc_blob *urpc_blob_make(struct urpc_blob *blob, void *buf, size_t len, size_t size);
struct urpc_blob *urpc_blob_alloc(size_t size);
void urpc_blob_free(struct urpc_blob *blob);

/* uRPC create error code blob object */
struct urpc_blob *urpc_create_result_blob(uint8_t result_code);
uint8_t urpc_result_code_get(struct urpc_blob* blob);

#ifdef URPC_USING_CJSON
/* cJSON object and blob object change */
struct urpc_blob *urcp_json_to_blob(cJSON *json, struct urpc_blob *blob);
cJSON *urpc_blob_to_json(struct urpc_blob *blob, cJSON *json);
#endif /* URPC_USING_CJSON */

#ifdef  __cplusplus
    }
#endif

#endif /* __URPC_H__ */

