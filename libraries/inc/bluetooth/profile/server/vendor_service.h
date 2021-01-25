/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     vendor_service.h
  * @brief    Head file for using Vendor Service Remote Controller.
  * @details  Vendor service data structs and external functions declaration.
  * @author   Chenjie Jin
  * @date     2018-5-7
  * @version  v1.1
  * *************************************************************************************
  */

#ifndef _VENDOR_SERVICE_H_
#define _VENDOR_SERVICE_H_

#include "profile_server.h"

#ifdef __cplusplus
extern "C"  {
#endif      /* __cplusplus */


/** @brief  Demo Profile service related UUIDs. */

#define GATT_UUID_CHAR_VENDOR_HANDSHAKE                       0xA001
#define GATT_UUID_CHAR_VENDOR_TEST_MODE                       0xA002

/** @brief  Index of each characteristic in Demo Profile service database. */
#define BLE_SERVICE_CHAR_VENDOR_HANDSHAKE_INDEX               0x02
#define GATT_SVC_VENDOR_HANDSHAKE_CHAR_CCCD_INDEX             0x03
#define BLE_SERVICE_CHAR_VENDOR_TEST_MODE_INDEX               0x05

#define VENDOR_WRITE_HANDSHAKE  1
#define VENDOR_WRITE_TEST_MODE  2

#define VENDOR_NOTIFY_ENABLE  1
#define VENDOR_NOTIFY_DISABLE  2

/** Message content */
typedef union
{
    struct
    {
        uint8_t len;
        uint8_t *report;
    } report_data;
} T_VENDOR_WRITE_PARAMETER;

/** @struct _TATVV_WRITE_MSG
  * @brief write message
  */
typedef struct
{
    uint8_t write_type; /**< ref: @ref Vendor_Write_Info */
    T_VENDOR_WRITE_PARAMETER write_parameter;
} T_VENDOR_WRITE_MSG;

typedef union _TVENDOR_UPSTREAM_MSG_DATA
{
    uint8_t notification_indification_index;
    T_VENDOR_WRITE_MSG write_msg;
} TVENDOR_UPSTREAM_MSG_DATA;

/** Vendor service data to inform application */
typedef struct _T_VENDOR_CALLBACK_DATA
{
    T_SERVICE_CALLBACK_TYPE     msg_type;
    TVENDOR_UPSTREAM_MSG_DATA    msg_data;
} T_VENDOR_CALLBACK_DATA;

extern uint8_t vendor_svc_handshake_values[16];

uint8_t vendor_svc_add_service(void *pFunc);

#ifdef __cplusplus
}
#endif

#endif
