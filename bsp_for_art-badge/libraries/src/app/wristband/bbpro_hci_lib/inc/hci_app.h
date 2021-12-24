/**
*********************************************************************************************************
*               Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      hci_app.h
* @brief     This file provides hci application layer code.
* @details
* @author    elliot chen
* @date      2018-11-16
* @version   v1.0
* *********************************************************************************************************
*/

#ifndef __HCI_APP_H
#define __HCI_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hci_cmd.h"
#include "dataTrans_hci.h"

/* Defines ------------------------------------------------------------------*/
/**
  * @brief HCI Event type
  */
typedef enum
{
    HCI_EVENT_ACK                 = 0x00,
    HCI_EVENT_REPLY_PAIRED_RECORD = 0x01,
    HCI_EVENT_CREATE_CONNECTION   = 0x03,
    HCI_EVENT_DISCONNECT          = 0x04,
    HCI_EVENT_DEVICE_STATE        = 0x07,
    HCI_EVENT_CALL_STATUS         = 0x0A,
    HCI_EVENT_PLAYER_STATUS       = 0x0B,
    HCI_EVENT_LINK_STATUS         = 0x14,
    HCI_EVENT_CODEC_SETTING       = 0x15,
    HCI_EVENT_INITIATE_CONN_DONE  = 0x16,
    HCI_EVENT_REPORT_STATUS       = 0x19,
    HCI_EVENT_PAIRING_KEY_MISSING = 0x1B,
    HCI_EVENT_DSP_IDLE            = 0x0201,
    HCI_EVENT_PUBLIC_ADDR         = 0x0903,
    HCI_EVENT_GET_MMI             = 0x090A,

    HCI_EVENT_MEDIA_INFO               = 0x1000,
    HCI_EVENT_MEDIA_INFO_SYNC_START               = 0x1001,
    HCI_EVENT_MEDIA_INFO_SYNC_END               = 0x1002,
    HCI_EVENT_PLAY_STATUS               = 0x1003,
    HCI_EVENT_MODE_STATE            = 0x1004,

    HCI_EVENT_INQURIRY_START             = 0x1100,
    HCI_EVENT_INQURIRY_COMPLETE          = 0x1101,
    HCI_EVENT_INQURIRY_STOP            = 0x1102,
    HCI_EVENT_INQURIRY_RESULT            = 0x1103,

} HCI_EVENT_TypeDef;

/**
  * @brief Event ack state type
  */
typedef enum
{
    EVENT_ACK_COMPLETE              = 0x00,
    EVENT_ACK_DISALLOW              = 0x01,
    EVENT_ACK_UNKNOWN_EVT           = 0x02,
    EVENT_ACK_PARAM_ERROR           = 0x03,
    EVENT_ACK_BUSY                  = 0x04,
    EVENT_ACK_PROECEE_FAIL          = 0x05,
} EVENT_ACK_STATUS;

/**
  * @brief Device state Event type
  */
typedef enum
{
    APP_STATE_OFF                   = 0x00,
    APP_STATE_PAIRING               = 0x01,
    APP_STATE_STANDBY               = 0x02,
    APP_STATE_CONNECTED             = 0x03,
    APP_STANDBY_WITH_LINK_BACK      = 0x04,
    APP_CONNECTED_WITH_LINK_BACK    = 0x05,
    APP_STATE_RESERVED              = 0x06,
} EVENT_DEVICE_STATE;

typedef struct
{
    volatile bool      paired_flag;
    volatile uint8_t   paired_num;
    volatile uint8_t   newest_addr[8];
} T_BBPRO_PAIRED_INFO;

/**
  * @brief Exported function
  */
void bbpro_hci_module_init(void);
void bbpro_hci_module_enter_dlps(void);
void bbpro_hci_module_exit_dlps(void);
bool bbpro_hci_module_allowed_enter_dlps_check(void);
bool bbpro_hci_module_system_wakeup_dlps_check(void);
void talkband_bbpro_hci_event_cb(HCI_PackTypeDef *pPacket);
void talkband_create_connection(void);
void bbpro_hci_add_hub_task(void);

#ifdef __cplusplus
}
#endif

#endif /*__HCI_APP_H*/

/******************* (C) COPYRIGHT 2018 Realtek Semiconductor Corporation *****END OF FILE****/

