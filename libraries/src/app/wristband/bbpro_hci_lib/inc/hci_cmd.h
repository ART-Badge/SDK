/**
*********************************************************************************************************
*               Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      hci_cmd.h
* @brief
* @details
* @author    elliot chen
* @date      2018-11-16
* @version   v1.0
* *********************************************************************************************************
*/

#ifndef __HCI_CMD_H
#define __HCI_CMD_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "dataTrans_hci.h"

/* Defines ------------------------------------------------------------------*/

/**
  * @brief Connection type which used for
    HCI_CMD_CREATE_CONNECTION and HCI_CMD_DISCONNECT command
  */
typedef enum
{
    CONNECT_TO_LAST_DEVICE = 0x00,
    CONNECT_WITH_BT_ADDR   = 0x04,
} CONNECTION_TYPE;

/**
  * @brief Profiles type which used for
     HCI_CMD_CREATE_CONNECTION and HCI_CMD_DISCONNECT command
  */
typedef enum
{
    A2DP_PROFILE_MSAK               = 0x01,
    AVRCP_PROFILE_MSAK              = 0x02,
    HFHS_PROFILE_MSAK               = 0x04,
    MULTI_SPEAKER_PROFILE_MSAK      = 0x08,
    SPP_PROFILE_MSAK                = 0x10,
    IAP_PROFILE_MSAK                = 0x20,
    PBAP_PROFILE_MSAK               = 0x40,
    RESERVED_PROFILE_MSAK           = 0x80
} PROFILE_TYPE;

/**
  * @brief MMI type which used for HCI_CMD_MMI command
  */
typedef enum
{
    /* HFP Commands*/
    AU_MMI_ADD_REMOVE_SCO           = 0x01,
    AU_MMI_ANSWER                   = 0x03,
    AU_MMI_REJECT_CALL              = 0x04,
    AU_MMI_END_CALL                 = 0x05,
    AU_MMI_MIC_MUTE_TOGGLE          = 0x06,
    AU_MMI_MIC_MUTE                 = 0x07,
    AU_MMI_MIC_UNMUTE               = 0x08,
    AU_MMI_INITIATE_VOICE_DIAL      = 0x09,
    AU_MMI_CANCEL_VOICE_DIAL        = 0x0A,
    AU_MMI_LAST_NUMBER_REDIAL       = 0x0B,

    /* A2DP Commands*/
    AU_MMI_VOL_UP                   = 0x30,
    AU_MMI_VOL_DOWN                 = 0x31,
    AU_MMI_AV_PLAY_PAUSE            = 0x32,
    AU_MMI_AV_STOP                  = 0x33,
    AU_MMI_AV_FWD                   = 0x34,
    AU_MMI_AV_BWD                   = 0x35,
    AU_MMI_DISCONNECT_AV_LINK       = 0x3A,
    AU_MMI_ENTER_PAIRING_MODE       = 0x51,
    AU_MMI_EXIT_PAIRING_MODE        = 0x52,
    AU_MMI_LINK_LAST_DEVICE         = 0x53,
    AU_MMI_POWER_ON_BUTTON_PRESS    = 0x54,
    AU_MMI_POWER_ON_BUTTON_RELEASE  = 0x55,
    AU_MMI_POWER_OFF_BUTTON_PRESS   = 0x56,
    AU_MMI_POWER_OFF_BUTTON_RELEASE = 0x57,
    AU_MMI_RESET_TO_DEFAULT         = 0x58,
    AU_MMI_DISCONNECT_ALL_LINK      = 0x59,
    AU_MMI_AUDIO_EFFECT_NEXT        = 0x63,
    AU_MMI_AUDIO_EFFECT_PREVIOU     = 0x64,
    AU_MMI_AUDIO_PASS_THROUGH       = 0x65,
} MMI_ACTION_TYPE;

/**
  * @brief Connection type which used for HCI_CMD_GET_STATUS
  */
typedef enum
{
    LOCAL_APP_STATUS            = 0x04,
} STATUS_TYPE;

/**
  * @brief HCI cmd type
  */
typedef enum
{
    HCI_CMD_ACK                 = 0x00,
    HCI_CMD_GET_PAIDED_RECORD   = 0x01,
    HCI_CMD_CREATE_CONNECTION   = 0x02,
    HCI_CMD_DISCONNECT          = 0x03,
    HCI_CMD_MMI                 = 0x04,
    HCI_CMD_GET_STATUS          = 0x18,
    HCI_CMD_WAKE_UP             = 0x1A,
    HCI_CMD_HFP_DIAL_WITH_NUMBER = 0x001B,
    HCI_CMD_GET_BT_ADDR         = 0x0302,
    HCI_CMD_PUSH_TONE           = 0x0008,
    /* next for BO */
    HCI_CMD_GET_PLAY_LIST       = 0x1000,
    HCI_CMD_PLAY_MEDIA_BY_NAME      = 0x1001,
    HCI_CMD_PLAY_MEDIA_BY_INDEX      = 0x1002,
    HCI_CMD_GET_MEDIA_INFO      = 0x1003,
    HCI_CMD_GET_PLAY_STATUS     = 0x1004,
    HCI_CMD_SET_PLAY_MODE       = 0x1005,

    HCI_CMD_INQUIRY_START             = 0x1100,
    HCI_CMD_INQUIRY_STOP             = 0x1101,
} HCI_CMD_TypeDef;

/**
  * @brief Command ack statds
  */
typedef enum
{
    CMD_ACK_COMPLETE              = 0x00,
    CMD_ACK_DISALLOW              = 0x01,
    CMD_ACK_UNKNOWN_EVT           = 0x02,
} CMD_ACK_STATUS;

typedef enum
{
    BOM_PLAY_MODE_A2DP_SINK              = 0x00,
    BOM_PLAY_MODE_A2DP_SOURCE              = 0x01,
    BOM_PLAY_MODE_LOCAL_PLAYBACK           = 0x02,
} BOM_MODE_STATE;

typedef enum
{
    BOM_MEDIA_TYPE_TITLE              = 0x00,
    BOM_MEDIA_TYPE_INDEX              = 0x01,
    BOM_MEDIA_TYPE_TIME               = 0x02,
    BOM_MEDIA_TYPE_TPE1               = 0x03,
    BOM_MEDIA_TYPE_TALB               = 0x04,
} BOM_MEDIA_INFO_TYPE;

/**
  * @brief Exported function
  */
void hci_cmd_send_ack(HCI_PackTypeDef *pPkt, uint8_t status);
void hci_cmd_create_connection(CONNECTION_TYPE conn_type, PROFILE_TYPE profile_type,
                               uint8_t bt_addr[6]);
void hci_cmd_send_disconnect(uint8_t link_index, uint8_t profile_mask);
void hci_cmd_send_mmi_action(MMI_ACTION_TYPE action_type);
void hci_cmd_get_status(STATUS_TYPE status_index);
void hci_cmd_send_wake_up(void);
void hci_cmd_hfp_dial_with_number(uint8_t phone_number[24], uint8_t phone_len);
void hci_cmd_get_bt_addr(void);
void hci_cmd_reconnect(uint8_t bt_addr[6]);

void hci_cmd_add_remove_sco(void);
void hci_cmd_answer(void);
void hci_cmd_reject_call(void);
void hci_cmd_end_call(void);
void hci_cmd_mic_mute_toggle(void);
void hci_cmd_mic_mute(void);
void hci_cmd_mic_unmute(void);
void hci_cmd_initiate_voice_dial(void);
void hci_cmd_cancel_voice_dial(void);
void hci_cmd_last_number_redial(void);

void hci_cmd_vol_up(void);
void hci_cmd_vol_down(void);
void hci_cmd_av_play_pause(void);
void hci_cmd_av_stop(void);
void hci_cmd_av_fwd(void);
void hci_cmd_av_bwd(void);
void hci_cmd_disconnect_av_link(void);
void hci_cmd_enter_pairing_mode(void);
void hci_cmd_exit_pairing_mode(void);
void hci_cmd_link_last_device(void);
void hci_cmd_reset_to_default(void);
void hci_cmd_disconnect_all_link(void);
void hci_cmd_audio_effect_next(void);
void hci_cmd_audio_effect_previou(void);
void hci_cmd_audio_pass_through(void);
void hci_cmd_read_paired_record(void);
void hci_app_send_power_on_button_press_cmd(void);
void hci_app_send_power_on_button_release_cmd(void);
void hci_app_send_power_off_button_press_cmd(void);
void hci_app_send_power_off_button_release_cmd(void);
void hci_cmd_push_tone(uint8_t index, uint8_t flag);


void hci_cmd_get_play_list(uint8_t type);
void hci_cmd_play_media_by_name(uint8_t *name, uint8_t len);
void hci_cmd_play_media_by_index(uint16_t index);
void hci_cmd_get_media_info(void);
void hci_cmd_get_play_status(void);
void hci_cmd_set_play_mode(uint8_t mode);

void hci_cmd_inquiry_start(uint8_t filter, int8_t rssi, uint8_t inquiry_time);
void hci_cmd_inquiry_stop(void);

#ifdef __cplusplus
}
#endif

#endif /*__HCI_CMD_H*/

/******************* (C) COPYRIGHT 2018 Realtek Semiconductor Corporation *****END OF FILE****/

