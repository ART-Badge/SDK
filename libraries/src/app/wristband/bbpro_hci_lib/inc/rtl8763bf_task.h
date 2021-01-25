/**
*********************************************************************************************************
*               Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      uartTrans_task.h
* @brief    This module realize send data to IO peripheral by task.
* @details
* @author    elliot chen
* @date      2017-05-09
* @version   v1.0
* *********************************************************************************************************
*/

#ifndef __RTL9763BF_TASK_H
#define __RTL9763BF_TASK_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "app_msg.h"
#include "os_msg.h"
#include "dataTrans_uart.h"
#include "dataTrans_hci.h"
#include "hci_cmd.h"

/* Defines ------------------------------------------------------------------*/
/**  @brief IO message definition for communications between tasks*/

typedef struct
{
    uint8_t filter;
    int8_t rssi;
    uint8_t inquiry_time;
    uint8_t rsv_1;
} T_INQUIRY_TYPE;

typedef struct
{
    uint16_t type;
    uint16_t subtype;
    uint16_t len;
    uint8_t index;
    uint8_t flag;
    union
    {
        uint32_t  param;
        void     *buf;
        T_INQUIRY_TYPE inquiry_param;
    } u;
} T_HCI_MSG;

typedef enum
{
    HCI_MSG_TYPE_8763BF,
    HCI_MSG_TYPE_8771,
} T_HCI_MSG_TYPE;

typedef enum
{
    BBPRO_HCI_SEND_WAKE_UP,
    BBPRO_HCI_SEND_POWER_ON_PRESSED,
    BBPRO_HCI_SEND_POWER_ON_RELEASED,
    BBPRO_HCI_SEND_POWER_OFF_PRESSED,
    BBPRO_HCI_SEND_POWER_OFF_RELEASED,
    BBPRO_HCI_SEND_ADD_REMOVE_SCO,
    BBPRO_HCI_SEND_ENTER_PAIRING_MODE,
    BBPRO_HCI_SEND_EXIT_PAIRING_MODE,
    BBPRO_HCI_CREATE_CONNECTION,
    BBPRO_HCI_GET_BT_ADDR,
    BBPRO_HCI_GET_BT_STATUS,
    BBPRO_HCI_RESET_TO_DEFAULT,
    BBPRO_HCI_INITIATE_VOICE_DIAL,
    BBPRO_HCI_CANCEL_VOICE_DIAL,
    BBPRO_HCI_LINK_LAST_DEVICE,
    BBPRO_HCI_DISCONNECT_ALL_LINK,
    BBPRO_HCI_INCOMMING_CALL_ACCEPT,
    BBPRO_HCI_INCOMMING_CALL_REJECT,
    BBPRO_HCI_END_CALL,
    BBPRO_HCI_PUSH_TONE,
    BBPRO_HCI_DIAL_WITH_NUMBER,
    BBPRO_HCI_READ_PAIRED_RECORD,


    BBPRO_HCI_AV_PLAY_PAUSE,
    BBPRO_HCI_AV_STOP,
    BBPRO_HCI_VOL_UP,
    BBPRO_HCI_VOL_DOWN,
    BBPRO_HCI_AV_FWD,
    BBPRO_HCI_AV_BWD,
    //NEXT FOR BOM
    BBPRO_HCI_GET_PLAY_LIST,
    BBPRO_HCI_SET_PLAY_MODE,
    BBPRO_HCI_PLAY_MEDIA_BY_NAME,
    BBPRO_HCI_PLAY_MEDIA_BY_INDEX,
    BBPRO_HCI_GET_MEDIA_INFO,
    BBPRO_HCI_GET_PLAY_STATUS,
    BBPRO_HCI_INQUIRY_START,
    BBPRO_HCI_INQUIRY_STOP,
} T_HCI_MSG_8763BF;

/** @brief BBPRO HCI LINK connection states*/
typedef enum
{
    BBPRO_HCI_LINK_STATE_DISCONNECTED, //!< Disconnected.
    BBPRO_HCI_LINK_STATE_CONNECTING,   //!< Connecting.
    BBPRO_HCI_LINK_STATE_CONNECTED,    //!< Connected.
    BBPRO_HCI_LINK_STATE_DISCONNECTING //!< Disconnecting.
} T_BBPRO_LINK_STATE;


/**
  * @brief
  */
extern void *bbppro_hci_send_queue_handle;
extern void *bbppro_hci_receive_queue_handle;
extern void *bbppro_hci_ack_queue_handle;
extern bool WaitingHCIAckFlag;
/**
  * @brief exported function
  */
void bbpro_hci_task_init(void);
void bbpro_hci_msg_queue_send(T_HCI_MSG *p_msg);
void bbpro_hall_double_check(void);

#ifdef __cplusplus
}
#endif

#endif /*__RTL9763BF_TASK_H*/

/******************* (C) COPYRIGHT 2017 Realtek Semiconductor Corporation *****END OF FILE****/

