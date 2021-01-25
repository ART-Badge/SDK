/**
*********************************************************************************************************
*               Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file      hci_cmd.c
* @brief     This file provides HCI command APIs for hci protocol communication.
* @details
* @author  elliot_chen
* @date     2018-11-16
* @version  v1.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "hci_cmd.h"

/* Defines -------------------------------------------------------------------*/

/* Gloabls -------------------------------------------------------------------*/

/**
  * @brief  Ack the received event.
  * @param  pPacket: point to HCI event packet struct.
  * @param  status: MCU can handle this command or not.
  * @retval None.
  */
void hci_cmd_send_ack(HCI_PackTypeDef *pPkt, uint8_t status)
{
    uint8_t param[3];
    uint8_t ack_packet[6 + sizeof(param) + 1];

    param[0] = pPkt->opcode & 0xFF;
    param[1] = (pPkt->opcode >> 8) & 0xFF;
    param[2] = status;
    DataTrans_AssemblePacket(ack_packet, HCI_CMD_ACK, param, sizeof(param));

    return DataTrans_send(ack_packet, sizeof(ack_packet));
}

/**
  * @brief  Send create connection cmd to control BBPro connect to peer device.
  * @param  conn_type: the specify connection type, refer to CONNECTION_TYPE data structure type.
  * @param  profile_type: the specify profile type, refer to PROFILE_TYPE data structure type.
  * @param  bt_addr[6]: the peer device BT address which BBPro need to connect.
  * @retval None.
  */
void hci_cmd_create_connection(CONNECTION_TYPE conn_type, PROFILE_TYPE profile_type,
                               uint8_t bt_addr[6])
{
    uint8_t param[8];
    uint8_t cmd_packet[6 + sizeof(param) + 1];

    param[0] = conn_type;
    param[1] = profile_type;
    memcpy(&param[2], bt_addr, 6);
    DataTrans_AssemblePacket(cmd_packet, HCI_CMD_CREATE_CONNECTION, param, sizeof(param));

    return DataTrans_send(cmd_packet, sizeof(cmd_packet));
}

/**
  * @brief  Send disconnect cmd to control BBPro disconnect to peer device.
  * @param  link_index: the link index.
  * @param  profile_mask: the specify profile mask bit.
  * @retval None.
  */
void hci_cmd_send_disconnect(uint8_t link_index, uint8_t profile_mask)
{
    uint8_t param[2];
    uint8_t cmd_packet[6 + sizeof(param) + 1];

    param[0] = link_index;
    param[1] = profile_mask;
    DataTrans_AssemblePacket(cmd_packet, HCI_CMD_DISCONNECT, param, sizeof(param));

    return DataTrans_send(cmd_packet, sizeof(cmd_packet));
}

/**
  * @brief  send mmi action.
  * @param  action_type: action type.
  * @retval None.
  */
void hci_cmd_send_mmi_action(MMI_ACTION_TYPE action_type)
{
    uint8_t param[2];
    uint8_t ack_packet[6 + sizeof(param) + 1];

    param[0] = 0;
    param[1] = action_type;
    DataTrans_AssemblePacket(ack_packet, HCI_CMD_MMI, param, sizeof(param));

    return DataTrans_send(ack_packet, sizeof(ack_packet));
}

/**
  * @brief  Get status.
  * @param  status_index: local app status.
  * @retval None.
  */
void hci_cmd_get_status(STATUS_TYPE status_index)
{
    uint8_t param[1];
    uint8_t cmd_packet[6 + sizeof(param) + 1];

    param[0] = status_index;
    DataTrans_AssemblePacket(cmd_packet, HCI_CMD_GET_STATUS, param, sizeof(param));

    return DataTrans_send(cmd_packet, sizeof(cmd_packet));
}

/**
  * @brief  Send wake up command to BBPro if it in DLPS mode.
  * @param  None.
  * @retval None.
  */
void hci_cmd_send_wake_up(void)
{
    uint8_t cmd_packet[6 + 1];

    DataTrans_AssemblePacket(cmd_packet, HCI_CMD_WAKE_UP, NULL, 0);

    DataTrans_send(cmd_packet, sizeof(cmd_packet));
    APP_PRINT_INFO0("[talkband][-->hci_cmd_send_wake_up] wake up!");
}

/**
  * @brief  HFP dial with number.
  * @param  phone_number[24]: string of destination phone number.
  * @param  phone_len: length of destination phone number, max length is 24.
  * @retval None.
  */
void hci_cmd_hfp_dial_with_number(uint8_t phone_number[24], uint8_t phone_len)
{
    uint8_t param[24];
    uint8_t cmd_packet[6 + sizeof(param) + 1];

    memcpy(&param[0], phone_number, phone_len);
    DataTrans_AssemblePacket(cmd_packet, HCI_CMD_HFP_DIAL_WITH_NUMBER, param, phone_len);

    return DataTrans_send(cmd_packet, 6 + phone_len + 1);
}

/**
  * @brief  Get BBPro BT addrss.
  * @param  None.
  * @retval None.
  */
void hci_cmd_get_bt_addr(void)
{
    uint8_t cmd_packet[6 + 1];

    DataTrans_AssemblePacket(cmd_packet, HCI_CMD_GET_BT_ADDR, NULL, 0);
    APP_PRINT_INFO0("[talkband][-->hci_cmd_get_bt_addr] get bt addr!");
    return DataTrans_send(cmd_packet, sizeof(cmd_packet));
}

/**
  * @brief  push tone.
  * @param  None.
  * @retval None.
  */
void hci_cmd_push_tone(uint8_t index, uint8_t flag)
{
    uint8_t param[2];
    param[0] = index;
    param[1] = flag;
    uint8_t cmd_packet[6 + sizeof(param) + 1];

    DataTrans_AssemblePacket(cmd_packet, HCI_CMD_PUSH_TONE, param, sizeof(param));
    if (flag == 0x00)
    {
        APP_PRINT_INFO1("[talkband][-->hci_cmd_push tone] play tone after previous tone finished index = 0x%x!",
                        index);
    }
    else
    {
        APP_PRINT_INFO1("[talkband][-->hci_cmd_push tone] clear unfinished tone index = 0x%x!", index);
    }
    DataTrans_send(cmd_packet, sizeof(cmd_packet));
}

/**
  * @brief  Send reconnection command to BBPro.
  * @param  bt_addr[6]: the peer device BT address which BBPro need to connect.
  * @retval None.
  */
void hci_cmd_reconnect(uint8_t bt_addr[6])
{
    return hci_cmd_create_connection(CONNECT_WITH_BT_ADDR, A2DP_PROFILE_MSAK, bt_addr);
}

/**
  * @brief  add or remove sco.
  * @param  None.
  * @retval None.
  */
void hci_cmd_add_remove_sco(void)
{
    return hci_cmd_send_mmi_action(AU_MMI_ADD_REMOVE_SCO);
}

/**
  * @brief  answer call.
  * @param  None.
  * @retval None.
  */
void hci_cmd_answer(void)
{
    return hci_cmd_send_mmi_action(AU_MMI_ANSWER);
}

/**
  * @brief  reject call.
  * @param  None.
  * @retval None.
  */
void hci_cmd_reject_call(void)
{
    return hci_cmd_send_mmi_action(AU_MMI_REJECT_CALL);
}

/**
  * @brief  end call.
  * @param  None.
  * @retval None.
  */
void hci_cmd_end_call(void)
{
    return hci_cmd_send_mmi_action(AU_MMI_END_CALL);
}

/**
  * @brief  mic mute toggle.
  * @param  None.
  * @retval None.
  */
void hci_cmd_mic_mute_toggle(void)
{
    return hci_cmd_send_mmi_action(AU_MMI_MIC_MUTE_TOGGLE);
}

/**
  * @brief  mic mute.
  * @param  None.
  * @retval None.
  */
void hci_cmd_mic_mute(void)
{
    return hci_cmd_send_mmi_action(AU_MMI_MIC_MUTE);
}

/**
  * @brief  mic unmute.
  * @param  None.
  * @retval None.
  */
void hci_cmd_mic_unmute(void)
{
    return hci_cmd_send_mmi_action(AU_MMI_MIC_UNMUTE);
}

/**
  * @brief  initiate voice dial.
  * @param  None.
  * @retval None.
  */
void hci_cmd_initiate_voice_dial(void)
{
    return hci_cmd_send_mmi_action(AU_MMI_INITIATE_VOICE_DIAL);
}

/**
  * @brief  cancel voice dial.
  * @param  None.
  * @retval None.
  */
void hci_cmd_cancel_voice_dial(void)
{
    return hci_cmd_send_mmi_action(AU_MMI_CANCEL_VOICE_DIAL);
}

/**
  * @brief  last number redial.
  * @param  None.
  * @retval None.
  */
void hci_cmd_last_number_redial(void)
{
    return hci_cmd_send_mmi_action(AU_MMI_LAST_NUMBER_REDIAL);
}

/**
  * @brief  volume up control.
  * @param  None.
  * @retval None.
  */
void hci_cmd_vol_up(void)
{
    return hci_cmd_send_mmi_action(AU_MMI_VOL_UP);
}

/**
  * @brief  volume down control.
  * @param  None.
  * @retval None.
  */
void hci_cmd_vol_down(void)
{
    return hci_cmd_send_mmi_action(AU_MMI_VOL_DOWN);
}

/**
  * @brief  av play pause player.
  * @param  None.
  * @retval None.
  */
void hci_cmd_av_play_pause(void)
{
    return hci_cmd_send_mmi_action(AU_MMI_AV_PLAY_PAUSE);
}

/**
  * @brief  av stop player.
  * @param  None.
  * @retval None.
  */
void hci_cmd_av_stop(void)
{
    return hci_cmd_send_mmi_action(AU_MMI_AV_STOP);
}

/**
  * @brief  next song.
  * @param  None.
  * @retval None.
  */
void hci_cmd_av_fwd(void)
{
    return hci_cmd_send_mmi_action(AU_MMI_AV_FWD);
}

/**
  * @brief  previous song
  * @param  None.
  * @retval None.
  */
void hci_cmd_av_bwd(void)
{
    return hci_cmd_send_mmi_action(AU_MMI_AV_BWD);
}

/**
  * @brief  disconnect a2dp /avrcp link
  * @param  None.
  * @retval None.
  */
void hci_cmd_disconnect_av_link(void)
{
    return hci_cmd_send_mmi_action(AU_MMI_DISCONNECT_AV_LINK);
}

/**
  * @brief  enter pairing mode.
  * @param  None.
  * @retval None.
  */
void hci_cmd_enter_pairing_mode(void)
{
    hci_cmd_send_mmi_action(AU_MMI_ENTER_PAIRING_MODE);
    APP_PRINT_INFO0("[talkband][-->hci_cmd_send_mmi_action] enter pairing mode");
}

/**
  * @brief  exit pairing mode.
  * @param  None.
  * @retval None.
  */
void hci_cmd_exit_pairing_mode(void)
{
    hci_cmd_send_mmi_action(AU_MMI_EXIT_PAIRING_MODE);
    APP_PRINT_INFO0("[talkband][-->hci_cmd_send_mmi_action] exit pairing mode");
}

/**
  * @brief  initialte connection to last device
  * @param  None.
  * @retval None.
  */
void hci_cmd_link_last_device(void)
{
    hci_cmd_send_mmi_action(AU_MMI_LINK_LAST_DEVICE);
    APP_PRINT_INFO0("[talkband][-->hci_cmd_send_mmi_action] link last device");
}

/**
  * @brief  Erase link key.
  * @param  None.
  * @retval None.
  */
void hci_cmd_reset_to_default(void)
{
    hci_cmd_send_mmi_action(AU_MMI_RESET_TO_DEFAULT);
    APP_PRINT_INFO0("[talkband][-->hci_cmd_send_reset] reset to default!");
}

/**
  * @brief  To control BBPro disconnect with the peer device.
  * @param  NOne.
  * @retval None.
  */
void hci_cmd_disconnect_all_link(void)
{
    hci_cmd_send_mmi_action(AU_MMI_DISCONNECT_ALL_LINK);
    APP_PRINT_INFO0("[talkband][-->hci_cmd_send_disconnect] disconnect all!");
}

/**
  * @brief  change DSP audio effect to previous mode
  * @param  None.
  * @retval None.
  */
void hci_cmd_audio_effect_next(void)
{
    return hci_cmd_send_mmi_action(AU_MMI_AUDIO_EFFECT_NEXT);
}

/**
  * @brief  change DSP audio effect to previous mode
  * @param  None.
  * @retval None.
  */
void hci_cmd_audio_effect_previou(void)
{
    return hci_cmd_send_mmi_action(AU_MMI_AUDIO_EFFECT_PREVIOU);
}

/**
  * @brief  toggle DSP passthrough feature
  * @param  None.
  * @retval None.
  */
void hci_cmd_audio_pass_through(void)
{
    return hci_cmd_send_mmi_action(AU_MMI_AUDIO_PASS_THROUGH);
}

/**
  * @brief  Send Power on button press command.
  * @param  None.
  * @retval None.
  */
void hci_app_send_power_on_button_press_cmd(void)
{
    hci_cmd_send_mmi_action(AU_MMI_POWER_ON_BUTTON_PRESS);
    APP_PRINT_INFO0("[talkband][-->hci_cmd_send_mmi_action] power on button press");
}

/**
  * @brief  Send Power on button release command.
  * @param  None.
  * @retval None.
  */
void hci_app_send_power_on_button_release_cmd(void)
{
    hci_cmd_send_mmi_action(AU_MMI_POWER_ON_BUTTON_RELEASE);
    APP_PRINT_INFO0("[talkband][-->hci_cmd_send_mmi_action] power on button release");
}

/**
  * @brief  Send Power off button press command.
  * @param  None.
  * @retval None.
  */
void hci_app_send_power_off_button_press_cmd(void)
{
    hci_cmd_send_mmi_action(AU_MMI_POWER_OFF_BUTTON_PRESS);
    APP_PRINT_INFO0("[talkband][-->hci_cmd_send_mmi_action] power off button press");
}

/**
  * @brief  Send Power off button release command.
  * @param  None.
  * @retval None.
  */
void hci_app_send_power_off_button_release_cmd(void)
{
    hci_cmd_send_mmi_action(AU_MMI_POWER_OFF_BUTTON_RELEASE);
    APP_PRINT_INFO0("[talkband][-->hci_cmd_send_mmi_action] power off button release");
}

void hci_cmd_read_paired_record(void)
{
    uint8_t param[1];
    uint8_t cmd_packet[6 + sizeof(param) + 1];

    param[0] = 0x00;
    DataTrans_AssemblePacket(cmd_packet, HCI_CMD_GET_PAIDED_RECORD, param, sizeof(param));

    APP_PRINT_INFO0("[talkband][-->hci_cmd_read_paired_record]!");
    return DataTrans_send(cmd_packet, sizeof(cmd_packet));
}


void hci_cmd_get_play_list(uint8_t type)
{
    uint8_t cmd_packet[6 + 1 + 1];

    DataTrans_AssemblePacket(cmd_packet, HCI_CMD_GET_PLAY_LIST, &type, 1);

    APP_PRINT_INFO0("[talkband][-->hci_cmd_get_play_list]!");
    return DataTrans_send(cmd_packet, sizeof(cmd_packet));
}

void hci_cmd_play_media_by_name(uint8_t *name, uint8_t len)
{
    uint8_t cmd_packet[6 + len + 1];

    DataTrans_AssemblePacket(cmd_packet, HCI_CMD_PLAY_MEDIA_BY_NAME, name, len);

    APP_PRINT_INFO0("[talkband][-->hci_cmd_play_media_by_name]!");
    return DataTrans_send(cmd_packet, sizeof(cmd_packet));
}

void hci_cmd_play_media_by_index(uint16_t index)
{

}


void hci_cmd_get_media_info(void)
{
    uint8_t cmd_packet[6 + 1];

    DataTrans_AssemblePacket(cmd_packet, HCI_CMD_GET_MEDIA_INFO, NULL, 0);

    APP_PRINT_INFO0("[talkband][-->hci_cmd_get_media_info]!");
    return DataTrans_send(cmd_packet, sizeof(cmd_packet));
}

void hci_cmd_get_play_status(void)
{
    uint8_t cmd_packet[6 + 1];

    DataTrans_AssemblePacket(cmd_packet, HCI_CMD_GET_PLAY_STATUS, NULL, 0);

    APP_PRINT_INFO0("[talkband][-->hci_cmd_get_play_status]!");
    return DataTrans_send(cmd_packet, sizeof(cmd_packet));
}

void hci_cmd_set_play_mode(uint8_t mode)
{
    uint8_t cmd_packet[6 + 1 + 1];

    DataTrans_AssemblePacket(cmd_packet, HCI_CMD_SET_PLAY_MODE, &mode, 1);
    APP_PRINT_INFO0("[talkband][-->hci_cmd_set_play_mode]!");
    return DataTrans_send(cmd_packet, sizeof(cmd_packet));
}

void hci_cmd_inquiry_start(uint8_t filter, int8_t rssi, uint8_t inquiry_time)
{
    uint8_t param[3];
    param[0] = filter;
    param[1] = rssi;
    param[2] = inquiry_time;
    uint8_t cmd_packet[6 + sizeof(param) + 1];
    DataTrans_AssemblePacket(cmd_packet, HCI_CMD_INQUIRY_START, param, sizeof(param));

    APP_PRINT_INFO0("[talkband][-->hci_cmd_inquiry_start]!");
    return DataTrans_send(cmd_packet, sizeof(cmd_packet));
}

void hci_cmd_inquiry_stop(void)
{
    uint8_t cmd_packet[6 + 1];
    DataTrans_AssemblePacket(cmd_packet, HCI_CMD_INQUIRY_STOP, NULL, 0);

    APP_PRINT_INFO0("[talkband][-->hci_cmd_inquiry_stop]!");
    return DataTrans_send(cmd_packet, sizeof(cmd_packet));
}

/******************* (C) COPYRIGHT 2018 Realtek Semiconductor Corporation *****END OF FILE****/

