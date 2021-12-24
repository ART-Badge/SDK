/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-28     Supperthomas the first version
 */

#ifndef __BSAL_COMMON_H__
#define __BSAL_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

#define  BIT(x)                             (1<<x)

typedef enum
{
    BSAL_RESULT_SUCCESS                   = 0,
    BSAL_RESULT_FAIL                      = 0xff,
    BSAL_SRV_READ_PENDING                 = 0xf1,
} BSAL_STATUS;

#define BSAL_UUID_TYPE_16BIT  16
#define BSAL_UUID_TYPE_32BIT  32
#define BSAL_UUID_TYPE_128BIT 128

/** 16-bit UUID */
struct bsal_uuid16
{
    uint8_t u_type;
    uint16_t value;
};

typedef struct bsal_uuid16 bsal_uuid16_t;

/** 32-bit UUID */
struct bsal_uuid32
{
    uint8_t u_type;
    uint32_t value;
} ;
typedef struct bsal_uuid32 bsal_uuid32_t;

/** 128-bit UUID */
struct bsal_uuid128
{
    uint8_t u_type;
    uint8_t value[16];
};
typedef struct bsal_uuid128 bsal_uuid128_t;

typedef union
{
    uint8_t u_type;
    bsal_uuid16_t u16;
    bsal_uuid32_t u32;
    bsal_uuid128_t u128;
} bsal_uuid_any_t;

#define BSAL_UUID16_INIT(uuid16)         \
    {                                   \
        .u_type = BSAL_UUID_TYPE_16BIT,     \
        .value = (uuid16),              \
    }

#define BSAL_UUID32_INIT(uuid32)         \
    {                                   \
        .u_type = BSAL_UUID_TYPE_32BIT,     \
        .value = (uuid32),              \
    }

#define BSAL_UUID128_INIT(uuid128...)    \
    {                                   \
        .u_type = BSAL_UUID_TYPE_128BIT,    \
        .value = { uuid128 },           \
    }

#define BSAL_UUID16_DECLARE(uuid16) \
    ((bsal_uuid_any_t *) (&(bsal_uuid16_t) BSAL_UUID16_INIT(uuid16)))

#define BSAL_UUID32_DECLARE(uuid32) \
    ((bsal_uuid_any_t *) (&(bsal_uuid32_t) BSAL_UUID32_INIT(uuid32)))

#define BSAL_UUID128_DECLARE(uuid128...) \
    ((bsal_uuid_any_t *) (&(bsal_uuid128_t) BSAL_UUID128_INIT(uuid128)))

struct bsal_gatt_chr_def
{
    bsal_uuid_any_t *uuid;
    struct bsal_gatt_dsc_def *descriptors;
    uint16_t properties;
    uint16_t permission;
    uint8_t value_length;
    uint16_t *val_handle;
    uint32_t bsal_flag;  //self define flag for future
};
typedef struct bsal_gatt_chr_def bsal_gatt_chr_def_t;

//SERVICE DEF
struct bsal_gatt_app_srv_def
{
    uint16_t type;
    bsal_uuid_any_t *uuid;
    struct bsal_gatt_app_srv_def **includes;
    bsal_gatt_chr_def_t *characteristics;
};
typedef struct bsal_gatt_app_srv_def  bsal_gatt_app_srv_def_t;

struct bsal_gatt_dsc_def
{
    bsal_uuid_any_t *uuid;
    uint8_t att_flags;
    uint8_t min_key_size;
};
typedef struct bsal_gatt_dsc_def bsal_gatt_dsc_def_t;

struct bsal_gatt_res
{
    uint16_t svcs;
    void     *srvc_addr;
    uint16_t incs;
    void     *incs_addr;
    uint16_t chrs;
    void     *chrs_addr;
    uint16_t dscs;
    void     *dscs_addr;
    uint16_t cccds;
    void     *cccds_addr;
    uint16_t attrs;
};

typedef struct bsal_gatt_res bsal_gatt_res_t;
#define BSAL_MAX_NAME_LENGTH 40
#define BSAL_MAX_ADV_SCAN_DATA_LENGTH 30
#define BSAL_SRV_UUID_GENERAL  0xEEEE

#ifdef __cplusplus
}
#endif
#endif

