/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-28     Supperthomas the first version
 */

#ifndef __BSAL_H__
#define __BSAL_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define BSAL_SW_VERSION "0.1.0"
#define BSAL_SW_VERSION_NUM 0x01000

#include "bsal_sig.h"

#include "bsal_common.h"
#include "bsal_app_api.h"

//CALLBACK_EVENT
typedef enum
{
    BSAL_CB_LAYER_GAP = 0xF1,
    BSAL_CB_LAYER_GATT_PROFILE,
    BSAL_CB_LAYER_SM,
    BSAL_CB_LAYER_COMMON,
    BSAL_CB_LAYER_UNKNOWN
} T_BSAL_APP_CB_LAYER_TYPE;

typedef enum
{
    BSAL_CB_STACK_READY = 0xD1,
    BSAL_CB_CONNECT_STATUS,
    BSAL_CB_STACK_ERROR = 0xFF,
} T_BSAL_GAP_SUBEVENT_TYPE;

typedef enum
{
    BSAL_GAP_CONN_STATE_DISCONNECTED, //!< Disconnected.
    BSAL_GAP_CONN_STATE_CONNECTING,   //!< Connecting.
    BSAL_GAP_CONN_STATE_CONNECTED,    //!< Connected.
    BSAL_GAP_CONN_STATE_DISCONNECTING //!< Disconnecting.
} T_BSAL_GAP_CONN_STATE;

struct bsal_gap_conn_status
{
    uint8_t conn_id;
    uint8_t new_state;
    uint16_t disc_cause;
};
typedef struct bsal_gap_conn_status bsal_gap_conn_status_t;

typedef union
{
    bsal_gap_conn_status_t    gap_conn_state_change;
} T_BSAL_GAP_MSG_DATA;


//bit 0...3 is read permision   bit 4..7 write permission bit 8...12 notify indicate permision

#define BSAL_GATT_PERM_NONE                   0x00
#define BSAL_GATT_PERM_ALL                    0x01
#define BSAL_GATT_PERM_AUTHEN_REQ             0x02
#define BSAL_GATT_PERM_AUTHEN_MITM_REQ        0x03
#define BSAL_GATT_PERM_AUTHOR_REQ             0x04  /**< bits 2 (rd), 6 (wr), 10 (notif/ind) */
#define BSAL_GATT_PERM_ENCRYPTED_REQ          0x08  /**< bits 3 (rd), 7 (wr), 11 (notif/ind) */
#define BSAL_GATT_PERM_AUTHEN_SC_REQ          0x09

#define BSAL_GATT_PERM_READ_NONE              BSAL_GATT_PERM_NONE
#define BSAL_GATT_PERM_READ                   BSAL_GATT_PERM_ALL
#define BSAL_GATT_PERM_READ_AUTHEN_REQ        BSAL_GATT_PERM_AUTHEN_REQ
#define BSAL_GATT_PERM_READ_AUTHEN_MITM_REQ   BSAL_GATT_PERM_AUTHEN_MITM_REQ
#define BSAL_GATT_PERM_READ_AUTHOR_REQ        BSAL_GATT_PERM_AUTHOR_REQ
#define BSAL_GATT_PERM_READ_ENCRYPTED_REQ     BSAL_GATT_PERM_ENCRYPTED_REQ
#define BSAL_GATT_PERM_READ_AUTHEN_SC_REQ     BSAL_GATT_PERM_AUTHEN_SC_REQ

#define BSAL_GATT_PERM_GET_READ(x)              (x & 0x0f)

#define BSAL_GATT_PERM_WRITE_NONE              (BSAL_GATT_PERM_NONE << 4)
#define BSAL_GATT_PERM_WRITE                   (BSAL_GATT_PERM_ALL << 4)
#define BSAL_GATT_PERM_WRITE_AUTHEN_REQ        (BSAL_GATT_PERM_AUTHEN_REQ << 4)
#define BSAL_GATT_PERM_WRITE_AUTHEN_MITM_REQ   (BSAL_GATT_PERM_AUTHEN_MITM_REQ << 4)
#define BSAL_GATT_PERM_WRITE_AUTHOR_REQ        (BSAL_GATT_PERM_AUTHOR_REQ << 4)
#define BSAL_GATT_PERM_WRITE_ENCRYPTED_REQ     (BSAL_GATT_PERM_ENCRYPTED_REQ << 4)
#define BSAL_GATT_PERM_WRITE_AUTHEN_SC_REQ     (BSAL_GATT_PERM_AUTHEN_SC_REQ<<4)

#define BSAL_GATT_PERM_GET_WRITE(x)             ((x>>4) & 0x0f)

#define BSAL_GATT_PERM_NOTIFY_IND_NONE              (BSAL_GATT_PERM_NONE << 8)
#define BSAL_GATT_PERM_NOTIFY_IND                   (BSAL_GATT_PERM_ALL << 8)
#define BSAL_GATT_PERM_NOTIFY_IND_AUTHEN_REQ        (BSAL_GATT_PERM_AUTHEN_REQ << 8)
#define BSAL_GATT_PERM_NOTIFY_IND_AUTHEN_MITM_REQ   (BSAL_GATT_PERM_AUTHEN_MITM_REQ << 8)
#define BSAL_GATT_PERM_NOTIFY_IND_AUTHOR_REQ        (BSAL_GATT_PERM_AUTHOR_REQ << 8)
#define BSAL_GATT_PERM_NOTIFY_IND_ENCRYPTED_REQ     (BSAL_GATT_PERM_ENCRYPTED_REQ << 8)
#define BSAL_GATT_PERM_NOTIFY_IND_AUTHEN_SC_REQ     (BSAL_GATT_PERM_AUTHEN_SC_REQ<<8)

#define BSAL_GATT_PERM_GET_NOTIFY_IND(x)        ((x>>8)& 0xf)


#define BSAL_GATT_GET_ENC_TYPE(x)                     (x & BSAL_GATT_PERM_ALL)
#define BSAL_GATT_GET_ENC_TYPE_AUTHEN_REQ(x)          (x & BSAL_GATT_PERM_AUTHEN_REQ)
#define BSAL_GATT_GET_ENC_TYPE_AUTHEN_MITM_REQ(x)     (x & BSAL_GATT_PERM_AUTHEN_MITM_REQ)
#define BSAL_GATT_GET_ENC_TYPE_AUTHOR_REQ(x)          (x & BSAL_GATT_PERM_AUTHOR_REQ)
#define BSAL_GATT_GET_ENC_TYPE_ENCRYPTED_REQ(x)       (x & BSAL_GATT_PERM_READ_ENCRYPTED_REQ)
#define BSAL_GATT_GET_ENC_AUTHEN_SC_REQ(x)            (x & BSAL_GATT_PERM_READ_AUTHEN_SC_REQ)




#ifdef __cplusplus
}
#endif
#endif

