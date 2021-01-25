/**
*********************************************************************************************************
*               Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     hci_app.c
* @brief
* @details
* @author
* @date
* @version
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "hci_app.h"
#include "rtl8763bf_task.h"
#include "rtl876x_pinmux.h"
#include "wristband_gap.h"
#include "gap_adv.h"
#include "app_task.h"
#include "board.h"
#include "communicate_protocol.h"
#include "wristband_global_data.h"
#include "os_timer.h"
#include "hub_task.h"
#include "localplay.h"
#include "headset.h"


uint8_t bbpro_bd_addr[6];
bool bbpro_bd_addr_got_flag = false;
HCI_EVENT_CB bbpro_hci_event_cb = NULL;
static bool pairing_key_missing_flag = false;

bool allowed_bbpro_hci_enter_dlps = true;
bool hci_module_has_installed = false;

const T_IO_MODULE_CBS bbpro_hci_cbs =
{
    bbpro_hci_module_init,
    bbpro_hci_module_enter_dlps,
    bbpro_hci_module_exit_dlps,
    bbpro_hci_module_allowed_enter_dlps_check,
    bbpro_hci_module_system_wakeup_dlps_check,
};

void bbpro_hci_add_hub_task(void)
{
    hci_module_has_installed = true;
    bbpro_hci_module_init();
    driver_modules[wristband_sensor_hub_count++] = (T_IO_MODULE_CBS *)&bbpro_hci_cbs;
}

/**
  * @brief  Initialize HCI protocol data struct and physical layer.
  * @param  None.
  * @retval None.
  */
void bbpro_hci_module_init(void)
{
    /* Initialize HCI protocol layer data struct */
    DataTrans_hci_data_struct_init();
    /* Initialize HCI physical layer */
    DataTrans_InitQueue();
    DataTrans_Board_UART_Init(BBPRO_HCI_RX, BBPRO_HCI_TX);
    System_WakeUpPinEnable(BBPRO_HCI_RX, PAD_WAKEUP_POL_LOW, PAD_WK_DEBOUNCE_DISABLE);
    bbpro_hci_task_init();
    bbpro_hci_event_cb = talkband_bbpro_hci_event_cb;
    pairing_key_missing_flag = false;
    /* Initialize HCI protocol layer data struct */
    DataTrans_hci_data_struct_init();
    /* Initialize HCI physical layer */
    DataTrans_InitQueue();
    DataTrans_Board_UART_Init(BBPRO_HCI_RX, BBPRO_HCI_TX);
    System_WakeUpPinEnable(BBPRO_HCI_RX, PAD_WAKEUP_POL_LOW, PAD_WK_DEBOUNCE_DISABLE);

#if 0
    T_HCI_MSG msg;
    msg.type = HCI_MSG_TYPE_8763BF;

    msg.subtype = BBPRO_HCI_SEND_EXIT_PAIRING_MODE;
    bbpro_hci_msg_queue_send(&msg);
    msg.subtype = BBPRO_HCI_SEND_POWER_OFF_PRESSED;
    bbpro_hci_msg_queue_send(&msg);
    msg.subtype = BBPRO_HCI_SEND_POWER_OFF_RELEASED;
    bbpro_hci_msg_queue_send(&msg);
    msg.subtype = BBPRO_HCI_GET_BT_ADDR;
    bbpro_hci_msg_queue_send(&msg);
    msg.subtype = BBPRO_HCI_READ_PAIRED_RECORD;
    bbpro_hci_msg_queue_send(&msg);
#if 0
    msg.subtype = BBPRO_HCI_SEND_POWER_ON_PRESSED;
    bbpro_hci_msg_queue_send(&msg);
    msg.subtype = BBPRO_HCI_SEND_POWER_ON_RELEASED;
    bbpro_hci_msg_queue_send(&msg);
    msg.subtype = BBPRO_HCI_SEND_ENTER_PAIRING_MODE;
    bbpro_hci_msg_queue_send(&msg);
#endif
#endif
//#if BBPRO_BO
#if 1
    T_HCI_MSG msg;

    msg.type = HCI_MSG_TYPE_8763BF;
    msg.subtype = BBPRO_HCI_SEND_POWER_ON_PRESSED;
    bbpro_hci_msg_queue_send(&msg);
    msg.subtype = BBPRO_HCI_SET_PLAY_MODE;
    msg.u.param = BOM_PLAY_MODE_LOCAL_PLAYBACK;
    bbpro_hci_msg_queue_send(&msg);

#endif
}


void bbpro_hci_module_enter_dlps(void)
{
    Pad_Config(BBPRO_HCI_TX, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_Config(BBPRO_HCI_RX, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
}

void bbpro_hci_module_exit_dlps(void)
{
    Pad_Config(BBPRO_HCI_TX, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE,
               PAD_OUT_LOW);
    Pad_Config(BBPRO_HCI_RX, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
}

bool bbpro_hci_module_allowed_enter_dlps_check(void)
{
    return allowed_bbpro_hci_enter_dlps;
}

bool bbpro_hci_module_system_wakeup_dlps_check(void)
{
    if (System_WakeUpInterruptValue(BBPRO_HCI_RX) == SET)
    {
        APP_PRINT_INFO0("BBPRO HCI RX WakeUp Int");
        Pad_ClearWakeupINTPendingBit(BBPRO_HCI_RX);
        allowed_bbpro_hci_enter_dlps = false;
        return true;
    }
    return false;
}

/**
  * @brief  Handle device state event message.
  * @param  pPacket: point to HCI event packet struct.
  * @retval None.
  */
void HandleBBProDeviceStateEvt(EVENT_DEVICE_STATE status)
{
    /* Update peer device status */
    switch (status)
    {
    case APP_STATE_OFF:
        {
            APP_PRINT_INFO0("[<--HandleBBProDeviceStateEvt] app state off!");
            RtkWristbandSys.bbpro_device_status = status;
            break;
        }
    case APP_STATE_PAIRING:
        {
            APP_PRINT_INFO0("[<--HandleBBProDeviceStateEvt] app state pairing!");
            RtkWristbandSys.bbpro_device_status = status;
            break;
        }
    case APP_STATE_STANDBY:
        {
            APP_PRINT_INFO0("[<--HandleBBProDeviceStateEvt] app state standby!");
            RtkWristbandSys.bbpro_device_status = status;
            break;
        }
    case APP_STATE_CONNECTED:
        {
            APP_PRINT_INFO0("[<--HandleBBProDeviceStateEvt] app state connected!");
            RtkWristbandSys.bbpro_device_status = APP_STATE_CONNECTED;
            break;
        }
    case APP_STANDBY_WITH_LINK_BACK:
        {
            APP_PRINT_INFO0("[<--HandleBBProDeviceStateEvt] app state standby with link back!");
            RtkWristbandSys.bbpro_device_status = APP_STANDBY_WITH_LINK_BACK;
            break;
        }
    case APP_CONNECTED_WITH_LINK_BACK:
        {
            APP_PRINT_INFO0("[<--HandleBBProDeviceStateEvt] app state connected with link back!");
            RtkWristbandSys.bbpro_device_status = APP_CONNECTED_WITH_LINK_BACK;
            break;
        }
    default:
        {
            break;
        }
    }

    return;
}


/**
  * @brief  HCI event message handle callback function.
  * @param  pPacket: point to HCI packet struct.
  * @retval None.
  */
void talkband_bbpro_hci_event_cb(HCI_PackTypeDef *pPacket)
{
    uint8_t status = CMD_ACK_COMPLETE;


    if (pPacket->opcode != HCI_EVENT_ACK)
    {
        APP_PRINT_INFO3("[talkband][<-hci receive raw data seq num %d ACK:false ]: %b-%02x", pPacket->seqn,
                        TRACE_BINARY(pPacket->len + 4, pPacket), pPacket->check_sum);
        /* This command is used to ack the received BBPro's event. */
    }
    else
    {
        APP_PRINT_INFO3("[talkband][<-hci receive raw data seq num %d ACK:true]: %b-%02x", pPacket->seqn,
                        TRACE_BINARY(pPacket->len + 4, pPacket), pPacket->check_sum);
    }

    /* Handle various HCI event */
    switch (pPacket->opcode)
    {
    case HCI_EVENT_ACK:
        {
            uint8_t event = 0;
            if (WaitingHCIAckFlag == false)
            {
                APP_PRINT_INFO0("[talkband][<-hci receive event cb] HCI event ACK, this is a fake ACK!");
            }
            else
            {
                if (os_msg_send(bbppro_hci_ack_queue_handle, &event, 0) == false)
                {
                    APP_PRINT_ERROR0("bbpro hci send msgfail");
                }
                APP_PRINT_INFO0("[talkband][<-hci receive event cb] HCI event ACK!");
            }
            //hci_cmd_send_ack(pPacket, status);
            break;
        }
    case HCI_EVENT_CREATE_CONNECTION:
        {
            APP_PRINT_INFO0("[talkband][<-event cb] HCI event create connection for profile!"); // for profile
            hci_cmd_send_ack(pPacket, status);
            break;
        }
    case HCI_EVENT_DISCONNECT:
        {
            APP_PRINT_INFO0("[talkband][<-event cb] HCI event disconnect for profile!"); // for profile
            hci_cmd_send_ack(pPacket, status);
            break;
        }
    case HCI_EVENT_DEVICE_STATE:
        {
            APP_PRINT_INFO1("[talkband][<-event cb] HCI event device state: %d!",
                            (EVENT_DEVICE_STATE)pPacket->payload[0]);
            HandleBBProDeviceStateEvt((EVENT_DEVICE_STATE)pPacket->payload[0]);
            hci_cmd_send_ack(pPacket, status);
            break;
        }
    case HCI_EVENT_CALL_STATUS:
        {
            APP_PRINT_INFO0("[talkband][<-event cb] recv HCI_EVENT_CALL_STATUS");
            hci_cmd_send_ack(pPacket, status);
            break;
        }
    case HCI_EVENT_PLAYER_STATUS:
        {
            APP_PRINT_INFO0("[talkband][<-event cb] recv HCI_EVENT_PLAYER_STATUS");
            hci_cmd_send_ack(pPacket, status);
            break;
        }
    case HCI_EVENT_LINK_STATUS:
        {
            if (pPacket->payload[0] == 0)
            {
                RtkWristbandSys.bbpro_hci_link_status = BBPRO_HCI_LINK_STATE_CONNECTED;

                APP_PRINT_INFO1("[talkband][<-event cb] recv HCI_EVENT_LINK_STATUS CONNECTED bbpro_hci_link_status= %d",
                                \
                                RtkWristbandSys.bbpro_hci_link_status);
            }
            else if (pPacket->payload[0] == 1)
            {
                APP_PRINT_INFO1("[talkband][<-event cb] recv HCI_EVENT_LINK_STATUS DISCONNECT : %d",
                                pPacket->payload[0]);
                RtkWristbandSys.bbpro_hci_link_status = BBPRO_HCI_LINK_STATE_DISCONNECTED;

                if (pairing_key_missing_flag == true)
                {
                    talkband_create_connection();//todo only for IOS
                    pairing_key_missing_flag = false;
                }
            }
            else
            {
                RtkWristbandSys.bbpro_hci_link_status = BBPRO_HCI_LINK_STATE_DISCONNECTED;
                APP_PRINT_ERROR1("[talkband][<-event cb] recv HCI_EVENT_LINK_STATUS ERROR CODE : %d",
                                 pPacket->payload[0]);
            }
            hci_cmd_send_ack(pPacket, status);
            break;
        }
    case HCI_EVENT_REPORT_STATUS:
        {
            APP_PRINT_INFO0("[talkband][<-event cb] recv HCI_EVENT_REPORT_STATUS");
            HandleBBProDeviceStateEvt((EVENT_DEVICE_STATE)pPacket->payload[1]);
            hci_cmd_send_ack(pPacket, status);
            break;
        }
    case HCI_EVENT_PAIRING_KEY_MISSING:
        {
            APP_PRINT_INFO0("[talkband][<-event cb] recv HCI_EVENT_PAIRING_KEY_MISSING");
            pairing_key_missing_flag = true;
            hci_cmd_send_ack(pPacket, status);
            break;
        }
    case HCI_EVENT_DSP_IDLE:
        {
            APP_PRINT_INFO0("[talkband][<-event cb] recv HCI_EVENT_DSP_IDLE");
            hci_cmd_send_ack(pPacket, status);
            break;
        }
    case HCI_EVENT_PUBLIC_ADDR:
        {
            APP_PRINT_INFO1("[talkband][<-event cb] hci event BT addr: %b", \
                            TRACE_BINARY(6, pPacket->payload));
            memcpy(bbpro_bd_addr, pPacket->payload, 6);
            bbpro_bd_addr_got_flag = true;
            hci_cmd_send_ack(pPacket, status);
            break;
        }
    case HCI_EVENT_GET_MMI:
        {
            APP_PRINT_INFO0("[talkband][<-event cb] recv HCI_EVENT_GET_MMI");
            hci_cmd_send_ack(pPacket, status);
            break;
        }
    case HCI_EVENT_CODEC_SETTING:
        {
            APP_PRINT_INFO0("[talkband][<-event cb] recv HCI_EVENT_CODEC_SETTING");
            break;
        }
    case HCI_EVENT_REPLY_PAIRED_RECORD:
        {
            APP_PRINT_INFO1("[talkband][<-event cb] recv HCI_EVENT_REPLY_PAIRED_RECORD num %d;",
                            pPacket->payload[0]);
            RtkWristbandSys.paired_info.paired_num = pPacket->payload[0];
            memcpy((uint8_t *)RtkWristbandSys.paired_info.newest_addr, pPacket->payload + 3, 6);
            hci_cmd_send_ack(pPacket, status);
            break;
        }
    case HCI_EVENT_INITIATE_CONN_DONE:
        {
            APP_PRINT_INFO0("[talkband][<-event cb] recv HCI_EVENT_INITIATE_CONN_DONE");
            RtkWristbandSys.bbpro_device_status = APP_STATE_CONNECTED;
            RtkWristbandSys.bbpro_hci_link_status = BBPRO_HCI_LINK_STATE_CONNECTED;
            RtkWristbandSys.paired_info.paired_flag = true;
            hci_cmd_send_ack(pPacket, status);
            break;
        }
    case HCI_EVENT_MEDIA_INFO:
        {
            APP_PRINT_INFO0("[talkband][<-event cb] recv HCI_EVENT_MEDIA_INFO");
            //save_media_list(pPacket->payload, pPacket->len - 2);
            save_media_list(pPacket->payload + 2, pPacket->len - 2 - 2);
            hci_cmd_send_ack(pPacket, status);
            break;
        }
    case HCI_EVENT_MEDIA_INFO_SYNC_START:
        {
            APP_PRINT_INFO0("[talkband][<-event cb] recv HCI EVENT MEDIA INFO SYNC START");
            prepare_get_media_list();
            hci_cmd_send_ack(pPacket, status);
            break;
        }
    case HCI_EVENT_MEDIA_INFO_SYNC_END:
        {
            APP_PRINT_INFO0("[talkband][<-event cb] recv HCI EVENT MEDIA INFO SYNC END");
            hci_cmd_send_ack(pPacket, status);
            break;
        }
    case HCI_EVENT_INQURIRY_START:
        {
            APP_PRINT_INFO0("[talkband][<-event cb] recv HCI_EVENT_INQURIRY_START");
            hci_cmd_send_ack(pPacket, status);
            break;
        }
    case HCI_EVENT_INQURIRY_COMPLETE:
        {
            APP_PRINT_INFO0("[talkband][<-event cb] recv HCI_EVENT_INQURIRY_COMPLETE");

            hci_cmd_send_ack(pPacket, status);
            break;
        }
    case HCI_EVENT_INQURIRY_STOP:
        {
            APP_PRINT_INFO0("[talkband][<-event cb] recv HCI_EVENT_INQURIRY_CANCEL");
            hci_cmd_send_ack(pPacket, status);
            break;
        }
    case HCI_EVENT_INQURIRY_RESULT:
        {
            APP_PRINT_INFO0("[talkband][<-event cb] recv HCI_EVENT_INQURIRY_RESULT");
            save_headset_list(pPacket->payload, pPacket->len - 2);
            hci_cmd_send_ack(pPacket, status);
            break;
        }
    case HCI_EVENT_MODE_STATE:
        {
            APP_PRINT_INFO1("[talkband][<-event cb] recv HCI EVENT MODE STATE %b ",
                            TRACE_BINARY(pPacket->len - 2, pPacket->payload));

            hci_cmd_send_ack(pPacket, status);
            T_HCI_MSG msg;
            msg.subtype = BBPRO_HCI_GET_PLAY_LIST;
            msg.u.param = BOM_MEDIA_TYPE_TITLE;
            bbpro_hci_msg_queue_send(&msg);
            break;
        }
    default:
        {
            APP_PRINT_INFO1("[talkband][<-event cb] unhandle opcode: 0x%x!", pPacket->opcode);
            hci_cmd_send_ack(pPacket, status);
            break;
        }
    }
}

void talkband_create_connection(void)
{
    T_HCI_MSG msg;
    msg.type = HCI_MSG_TYPE_8763BF;
    msg.subtype = BBPRO_HCI_SEND_POWER_ON_PRESSED;
    bbpro_hci_msg_queue_send(&msg);
    msg.subtype = BBPRO_HCI_SEND_POWER_ON_RELEASED;
    bbpro_hci_msg_queue_send(&msg);

    extern uint8_t bbpro_peer_bd_addr[];
    msg.subtype = BBPRO_HCI_CREATE_CONNECTION;
    msg.len = 6;
    msg.u.buf = bbpro_peer_bd_addr;
    bbpro_hci_msg_queue_send(&msg);
}



/******************* (C) COPYRIGHT 2018 Realtek Semiconductor Corporation *****END OF FILE****/

