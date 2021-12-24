/**
*********************************************************************************************************
*               Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     uartTrans_task.c
* @brief    This module realize HCI event and ack message processing which received from BBPro.
* @details
* @author
* @date     2019-01-11
* @version  v1.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtl8763bf_task.h"
#include <os_msg.h>
#include <os_task.h>
#include "os_sched.h"
#include "os_mem.h"
#include "hci_cmd.h"
#include "os_timer.h"
#include "hci_app.h"
/* Defines -------------------------------------------------------------------*/
#define BBPRO_HCI_SEND_TASK_PRIORITY                1
#define BBPRO_HCI_SEND_TASK_STACK_SIZE              (256*4)
#define BBPRO_MAX_RESEND_COUNT                      8
#define BBPRO_CMD_RECHECK_INTERVAL                  5000



uint8_t bbpro_peer_bd_addr[6];
bool WaitingHCIAckFlag = false;


/* Globals ------------------------------------------------------------------ */
void *bbpro_hci_send_task_handle;
void *bbpro_hci_receive_task_handle;
void *bbppro_hci_send_queue_handle = NULL;
void *bbppro_hci_receive_queue_handle = NULL;
void *bbppro_hci_ack_queue_handle = NULL;



bool bbpro_hci_send(T_HCI_MSG *hci_msg)
{
    switch (hci_msg->subtype)
    {
    case BBPRO_HCI_SEND_WAKE_UP:
        {
            hci_cmd_send_wake_up();
            break;
        }
    case BBPRO_HCI_SEND_POWER_ON_PRESSED:
        {
            hci_app_send_power_on_button_press_cmd();
            break;
        }
    case BBPRO_HCI_SEND_POWER_ON_RELEASED:
        {
            APP_PRINT_INFO0("[talkband] BBPRO_HCI_SEND_POWER_ON_RELEASED!");
            hci_app_send_power_on_button_release_cmd();
            break;
        }
    case BBPRO_HCI_SEND_POWER_OFF_PRESSED:
        {
            hci_app_send_power_off_button_press_cmd();
            break;
        }
    case BBPRO_HCI_SEND_POWER_OFF_RELEASED:
        {
            APP_PRINT_INFO0("[talkband] BBPRO_HCI_SEND_POWER_OFF_RELEASED!");
            hci_app_send_power_off_button_release_cmd();
            break;
        }
    case BBPRO_HCI_SEND_ADD_REMOVE_SCO:
        {
            hci_cmd_add_remove_sco();
            break;
        }
    case BBPRO_HCI_SEND_ENTER_PAIRING_MODE:
        {
            hci_cmd_enter_pairing_mode();
            break;
        }
    case BBPRO_HCI_SEND_EXIT_PAIRING_MODE:
        {
            hci_cmd_exit_pairing_mode();
            break;
        }
    case BBPRO_HCI_CREATE_CONNECTION:
        {
            uint8_t  remote_bd[6];
            memcpy(remote_bd, hci_msg->u.buf, hci_msg->len);
            APP_PRINT_INFO1("[talkband][BBPRO PEER ADDR] create connect by address %b",
                            TRACE_BDADDR(remote_bd));
            hci_cmd_create_connection(CONNECT_WITH_BT_ADDR, A2DP_PROFILE_MSAK, remote_bd);
            break;
        }
    case BBPRO_HCI_GET_BT_ADDR:
        {
            hci_cmd_get_bt_addr();
            break;
        }
    case BBPRO_HCI_GET_BT_STATUS:
        {
            hci_cmd_get_status(LOCAL_APP_STATUS);
            break;
        }
    case BBPRO_HCI_RESET_TO_DEFAULT:
        {
            APP_PRINT_WARN0("[talkband] Prepare Factory Mode!");
            hci_cmd_reset_to_default();
            break;
        }
    case BBPRO_HCI_INITIATE_VOICE_DIAL:
        {
            hci_cmd_initiate_voice_dial();
            break;
        }
    case BBPRO_HCI_CANCEL_VOICE_DIAL:
        {
            hci_cmd_cancel_voice_dial();
            break;
        }
    case BBPRO_HCI_LINK_LAST_DEVICE:
        {
            APP_PRINT_INFO0("[talkband] BBPRO_HCI_LINK_LAST_DEVICE!");
            hci_cmd_link_last_device();
            break;
        }
    case BBPRO_HCI_DISCONNECT_ALL_LINK:
        {
            hci_cmd_disconnect_all_link();
            break;
        }
    case BBPRO_HCI_INCOMMING_CALL_ACCEPT:
        {
            APP_PRINT_WARN0("[talkband] Incoming Call Accept!");
            hci_cmd_answer();
        }
        break;
    case BBPRO_HCI_INCOMMING_CALL_REJECT:
        {
            APP_PRINT_WARN0("[talkband] Incoming Call Reject!");
            hci_cmd_reject_call();
        }
        break;
    case BBPRO_HCI_END_CALL:
        {
            APP_PRINT_WARN0("[talkband] End Call !");
            hci_cmd_end_call();
        }
        break;
    case BBPRO_HCI_PUSH_TONE:
        {
            hci_cmd_push_tone(hci_msg->index, hci_msg->flag);
        }
        break;
    case BBPRO_HCI_DIAL_WITH_NUMBER:
        {
            hci_cmd_hfp_dial_with_number(hci_msg->u.buf, hci_msg->len);
        }
        break;
    case BBPRO_HCI_READ_PAIRED_RECORD:
        {
            hci_cmd_read_paired_record();
        }
        break;
    case BBPRO_HCI_PLAY_MEDIA_BY_NAME:
        {
            hci_cmd_play_media_by_name(hci_msg->u.buf, hci_msg->len);
        }
        break;
    case BBPRO_HCI_GET_MEDIA_INFO:
        {
            hci_cmd_get_media_info();
        }
        break;
    case BBPRO_HCI_GET_PLAY_LIST:
        {
            hci_cmd_get_play_list(hci_msg->u.param);
        }
        break;
    case BBPRO_HCI_SET_PLAY_MODE:
        {
            hci_cmd_set_play_mode(hci_msg->u.param);
        }
        break;
    case BBPRO_HCI_AV_PLAY_PAUSE:
        {
            hci_cmd_av_play_pause();
        }
        break;
    case BBPRO_HCI_AV_STOP:
        {
            hci_cmd_av_stop();
        }
        break;
    case BBPRO_HCI_VOL_UP:
        {
            hci_cmd_vol_up();
        }
        break;
    case BBPRO_HCI_VOL_DOWN:
        {
            hci_cmd_vol_down();
        }
        break;
    case BBPRO_HCI_AV_FWD:
        {
            hci_cmd_av_fwd();
        }
        break;
    case BBPRO_HCI_AV_BWD:
        {
            hci_cmd_av_bwd();
        }
        break;
    case BBPRO_HCI_INQUIRY_START:
        {
            hci_cmd_inquiry_start(hci_msg->u.inquiry_param.filter, hci_msg->u.inquiry_param.rssi,
                                  hci_msg->u.inquiry_param.inquiry_time);
        }
        break;
    case BBPRO_HCI_INQUIRY_STOP:
        {
            hci_cmd_inquiry_stop();
        }
        break;
    default:
        APP_PRINT_INFO0("[talkband]send error cmd fail!");
        break;
    }

    uint8_t event;
    static uint32_t retry_count = 0;
    WaitingHCIAckFlag = true;
    if (os_msg_recv(bbppro_hci_ack_queue_handle, &event, 500) == true)
    {
        APP_PRINT_INFO0("[talkband]bbp hci receive ACK success!");
        retry_count = 0;
        WaitingHCIAckFlag = false;
        return true;
    }
    else
    {
        retry_count++;
        APP_PRINT_INFO1("[talkband]bbp hci receive ACK time out! do retry count = %d", retry_count);
        if (retry_count < BBPRO_MAX_RESEND_COUNT)
        {
            extern HCI_TransInfoTypeDef hci_trans_info;
            hci_trans_info.hci_tx_seqn--;
            bbpro_hci_send(hci_msg);
        }
        retry_count = 0;
        WaitingHCIAckFlag = false;
        return false;
    }
}

void bbpro_hci_msg_queue_send(T_HCI_MSG *p_msg)
{
    extern bool hci_module_has_installed;
    if (hci_module_has_installed == false)
    {
        return;
    }
    if (os_msg_send(bbppro_hci_send_queue_handle, p_msg, 0) == false)
    {
        APP_PRINT_ERROR0("[talkband]bbpro hci send msgfail");
    }
}

/**
 * @brief      bbp hci send task to handle events & messages
 * @param[in]  p_param  Parameters sending to the task
 * @return     void
 */
void bbpro_hci_send_task(void *p_param)
{
    T_HCI_MSG hci_msg;

    while (true)
    {
        if (os_msg_recv(bbppro_hci_send_queue_handle, &hci_msg, 0xFFFFFFFF) == true)
        {
            bbpro_hci_send(&hci_msg);
        }
    }
}


void bbpro_hci_receive_task(void *p_param)
{
    uint8_t event;

    while (true)
    {
        if (os_msg_recv(bbppro_hci_receive_queue_handle, &event, 0xFFFFFFFF) == true)
        {
            switch (event)
            {
            case IO_MSG_UART_RX_PKT:
                {
                    //APP_PRINT_INFO0("[talkband]bbppro hci receive uart interrupt task");
                    DataTrans_HandleRX();
                    break;
                }
            default:
                break;
            }
        }
    }
}

/**
 * @brief  Initialize data trans task
 * @return void
 */
extern void *app_task_handle;   //!< APP Task handle
void bbpro_hci_task_init(void)
{
    os_msg_queue_create(&bbppro_hci_receive_queue_handle, 0x10, sizeof(uint8_t));
    os_msg_queue_create(&bbppro_hci_send_queue_handle, 0x10, sizeof(T_HCI_MSG));
    os_msg_queue_create(&bbppro_hci_ack_queue_handle, 0x10, sizeof(uint8_t));
    DataTrans_Driver_UART_Init();

    os_task_create(&bbpro_hci_send_task_handle, "B3 S task", bbpro_hci_send_task, 0, \
                   BBPRO_HCI_SEND_TASK_STACK_SIZE, BBPRO_HCI_SEND_TASK_PRIORITY);
    os_task_create(&bbpro_hci_receive_task_handle, "B3 R task", bbpro_hci_receive_task, 0, \
                   BBPRO_HCI_SEND_TASK_STACK_SIZE, BBPRO_HCI_SEND_TASK_PRIORITY);
#if 1
    DBG_DIRECT("gggg task TCB timer:0x%x, lower:0x%x, upper:0x%x, app:0x%x, send: 0x%x, receive: 0x%x",
               *(uint32_t *)0x00200e5c, *(uint32_t *)0x0020176c,
               *(uint32_t *)0x00201494, app_task_handle, bbpro_hci_send_task_handle,
               bbpro_hci_receive_task_handle);
#endif

}

/******************* (C) COPYRIGHT 2018 Realtek Semiconductor Corporation *****END OF FILE****/

