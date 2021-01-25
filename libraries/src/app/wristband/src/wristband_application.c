/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      peripheral_app.c
   * @brief     This file handles BLE peripheral application routines.
   * @author    jane
   * @date      2017-06-06
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */

/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include <trace.h>
#include <string.h>
#include "os_timer.h"
#include <gap.h>
#include <gap_adv.h>
#include <gap_bond_le.h>
#include <profile_server.h>
#include <gap_msg.h>
#include "os_task.h"
#include <ias.h>
#include "bas.h"
#include "wristband_gap.h"
#include <ancs_client.h>
#include <gap_conn_le.h>
#include "gap_vendor.h"

#include "ota_service.h"
#include "dfu_api.h"
#include "rtl876x_wdg.h"
#include "app_task.h"
#include "wristband_application.h"
#include "communicate_protocol.h"
#include "hub_clock.h"
#include "health_algo.h"
#include "module_gpio_button.h"
#include "module_touch.h"
#include "wristband_global_data.h"
#include "hub_interaction.h"
#include "ancs.h"
#include "ams.h"
#include <gatts_client.h>
#include "app_flash_config.h"
#include "ftl.h"
#include "hub_battery.h"
#include "module_font.h"
#include "wristband_gui.h"
#include "hci_app.h"
#include "patch_header_check.h"
#include "dfu_main.h"
#include "dfu_flash.h"
#if APP_PRIVACY_EN
#include <privacy_mgnt.h>
#endif

#if FEATURE_HID
#include "hids_media.h"
#endif
#if FEATURE_BREEZE
#include "ais.h"
#endif

#if SUPPORT_SILENT_OTA
#include "board.h"
#include "dfu_service.h"
#endif
#if FEATURE_AIRSYNC
#include "wechat_airsync_protocol.h"
#include "airsync_ble_service.h"
#endif

/** @addtogroup  PERIPH_DEMO
    * @{
    */

/** @defgroup  PERIPH_APP Peripheral Application
    * @brief This file handles BLE peripheral application routines.
    * @{
    */
/*============================================================================*
 *                              Variables
 *============================================================================*/
/** @addtogroup  PERIPH_SEVER_CALLBACK Profile Server Callback Event Handler
    * @brief Handle profile server callback event
    * @{
    */

/** @} */ /* End of group PERIPH_SEVER_CALLBACK */
/** @defgroup  PERIPH_GAP_MSG GAP Message Handler
    * @brief Handle GAP Message
    * @{
    */
#if APP_PRIVACY_EN
T_PRIVACY_STATE app_privacy_state = PRIVACY_STATE_INIT;
T_PRIVACY_ADDR_RESOLUTION_STATE app_privacy_resolution_state = PRIVACY_ADDR_RESOLUTION_DISABLED;
T_APP_WORK_MODE app_work_mode = APP_PAIRABLE_MODE;
#endif
bool switch_into_ota_pending = false;
static bool dfu_active_rst_pending = false;
extern void *l1send_task_handle;
bool memu_update_flag = false;
#if SUPPORT_SILENT_OTA
bool dfu_update_param = false;
#endif
/*============================================================================*
 *                              Functions
 *============================================================================*/
void app_handle_gap_msg(T_IO_MSG  *p_gap_msg);
void wristband_handle_io_message(T_IO_MSG *p_wristband_msg);

/**
 * @brief    All the application messages are pre-handled in this function
 * @note     All the IO MSGs are sent to this function, then the event handling
 *           function shall be called according to the MSG type.
 * @param[in] io_msg  IO message data
 * @return   void
 */
void app_handle_io_msg(T_IO_MSG io_msg)
{
    uint16_t msg_type = io_msg.type;

    switch (msg_type)
    {
    case IO_MSG_TYPE_BT_STATUS:
        {
            app_handle_gap_msg(&io_msg);
        }
        break;
    case IO_MSG_TYPE_WRISTBNAD:
        {
            wristband_handle_io_message(&io_msg);
        }
        break;
#if F_BT_ANCS_CLIENT_SUPPORT
    case IO_MSG_TYPE_ANCS:
        {
            ancs_handle_msg(&io_msg);
        }
        break;
#endif
    case IO_MSG_TYPE_RESET_WDG_TIMER:
        {
            //comment this log
            //APP_PRINT_INFO0("[WDG] Watch Dog Rset Timer");
            WDG_Restart();
        }
    default:
        break;
    }
}

void wristband_handle_io_message(T_IO_MSG *p_wristband_msg)
{
    uint8_t msg_type = p_wristband_msg->subtype;
    switch (msg_type)
    {
    case IO_MSG_BWPS_TX_VALUE:
        {
            uint16_t length = ((uint8_t *)(p_wristband_msg->u.buf))[0];
            uint8_t *p_data = (uint8_t *)p_wristband_msg->u.buf + 1;
            L1_receive_data(p_data, length);
        }
        break;
    case IO_MSG_ANCS_DISCOVERY:
        {
            ancs_start_discovery(RtkWristbandSys.wristband_conn_id);
        }
        break;
    case IO_MSG_TYPE_AMS:
        {
            ams_handle_msg(p_wristband_msg);
        }
        break;
    case IO_MSG_UPDATE_CONPARA:
        {
            wristband_update_con_para(true);
        }
        break;
    default:
        break;
    }
}

/**
 * @brief    Handle msg LE_GAP_MSG_TYPE_DEV_STATE_CHANGE
 * @note     All the gap device state events are pre-handled in this function.
 *           Then the event handling function shall be called according to the new_state
 * @param[in] new_state  New gap device state
 * @param[in] cause GAP device state change cause
 * @return   void
 */
void app_handle_dev_state_evt(T_GAP_DEV_STATE new_state, uint16_t cause)
{
    APP_PRINT_INFO3("app_handle_dev_state_evt: init state %d, adv state %d, cause 0x%x",
                    new_state.gap_init_state, new_state.gap_adv_state, cause);
#if APP_PRIVACY_EN
    if ((new_state.gap_init_state == GAP_INIT_STATE_STACK_READY)
        && (new_state.gap_adv_state == GAP_ADV_STATE_IDLE)
        && (new_state.gap_conn_state == GAP_CONN_DEV_STATE_IDLE))
    {
        privacy_handle_resolv_list();
    }
#endif
    if (RtkWristbandSys.gap_dev_state.gap_init_state != new_state.gap_init_state)
    {
        if (new_state.gap_init_state == GAP_INIT_STATE_STACK_READY)
        {
            APP_PRINT_INFO0("GAP stack ready");
            extern T_GAP_CAUSE  le_vendor_more_precise_32k_option(bool enable);
            T_GAP_CAUSE cause;
            cause =  le_vendor_more_precise_32k_option(true);
            if (cause == GAP_CAUSE_SUCCESS)
            {
                APP_PRINT_INFO0("precise 32k option cmd ok! only for wristband!");
            }
            /*stack ready*/
            /*wristband start adv, it will set bd address in the adv data*/
            wristband_start_adv();
        }
    }

    if (RtkWristbandSys.gap_dev_state.gap_adv_state != new_state.gap_adv_state)
    {
        if (new_state.gap_adv_state == GAP_ADV_STATE_IDLE)
        {
            if (new_state.gap_adv_sub_state == GAP_ADV_TO_IDLE_CAUSE_CONN)
            {
                APP_PRINT_INFO0("GAP adv stoped: because connection created");
            }
            else
            {
                APP_PRINT_INFO0("GAP adv stoped");
            }
            if (RtkWristbandSys.gap_conn_state == GAP_CONN_STATE_CONNECTED)
            {

            }
            else if (RtkWristbandSys.gap_conn_state == GAP_CONN_STATE_DISCONNECTED)
            {
                wristband_start_adv();
            }
        }
        else if (new_state.gap_adv_state == GAP_ADV_STATE_ADVERTISING)
        {
            APP_PRINT_INFO0("GAP adv start");
        }
    }

    RtkWristbandSys.gap_dev_state = new_state;
}

/**
 * @brief    Handle msg LE_GAP_MSG_TYPE_CONN_STATE_CHANGE
 * @note     All the gap conn state events are pre-handled in this function.
 *           Then the event handling function shall be called according to the new_state
 * @param[in] conn_id Connection ID
 * @param[in] new_state  New gap connection state
 * @param[in] cause Use this cause when new_state is GAPSTATE_CONN_DISCONNECTED
 * @return   void
 */
void app_handle_conn_state_evt(uint8_t conn_id, T_GAP_CONN_STATE new_state, uint16_t disc_cause)
{
    APP_PRINT_INFO4("app_handle_conn_state_evt: conn_id %d old_state %d new_state %d, disc_cause 0x%x",
                    conn_id, RtkWristbandSys.gap_conn_state, new_state, disc_cause);
    switch (new_state)
    {
    case GAP_CONN_STATE_DISCONNECTED:
        {
            if ((disc_cause != (HCI_ERR | HCI_ERR_REMOTE_USER_TERMINATE))
                && (disc_cause != (HCI_ERR | HCI_ERR_LOCAL_HOST_TERMINATE)))
            {
                APP_PRINT_ERROR1("app_handle_conn_state_evt: connection lost cause 0x%x", disc_cause);
            }
            /* stop parameter update timer */
            stop_connection_paramter_timer();


            /* clear flags used in connection state */
            RtkWristbandSys.flag_field.is_paired = false;
            RtkWristbandSys.flag_field.device_had_logged = false;
            RtkWristbandSys.flag_field.phone_camera_status = false;
            RtkWristbandSys.flag_field.auto_sync_enable = false;
            if (RtkWristbandSys.flag_field.is_calling_action_alive)
            {
                wristband_system_interact(INTERACT_CALLING_REFUSE, 0, NULL);
            }


            //Tx task will ignore the send event if there is no connection;
            //We'd better replace this to reset the queue, it will update connnection state in the end of the function;
            //wait for the platform add reset api;
            RtkWristbandSys.gap_conn_state = new_state;

            if (switch_into_ota_pending)
            {
#if (SUPPORT_NORMAL_OTA == 1)
                switch_into_ota_pending = false;
                dfu_switch_to_ota_mode();
#endif
            }
            else
            {
                if (dfu_active_rst_pending)
                {
                    dfu_active_rst_pending = false;
                    //unlock_flash_bp_all();   //GRACE TO CHECK
                    dfu_fw_reboot(true);
                }
            }
            if (RtkWristbandSys.gap_dev_state.gap_adv_state == GAP_ADV_STATE_IDLE)
            {
                wristband_start_adv();
            }
            else if (RtkWristbandSys.gap_dev_state.gap_adv_state == GAP_ADV_STATE_ADVERTISING)
            {
                wristband_stop_adv();
            }

        }
        break;

    case GAP_CONN_STATE_CONNECTED:
        {
            uint8_t  remote_bd[6];
            T_GAP_REMOTE_ADDR_TYPE remote_bd_type;
            le_get_conn_param(GAP_PARAM_CONN_INTERVAL, &RtkWristbandSys.conn_interval, conn_id);
            le_get_conn_param(GAP_PARAM_CONN_LATENCY, &RtkWristbandSys.conn_latency, conn_id);
            le_get_conn_param(GAP_PARAM_CONN_TIMEOUT, &RtkWristbandSys.conn_superv_tout, conn_id);
            le_get_conn_addr(conn_id, remote_bd, (uint8_t *)&remote_bd_type);

            APP_PRINT_INFO6("GAPSTATE_CONN_CONNECTED:, conn_id 0x%x, conn_interval 0x%x, conn_latency 0x%x, \
                            conn_supervision_timeout 0x%x, remote_bd %s, remote_addr_type %d",
                            conn_id, RtkWristbandSys.conn_interval, RtkWristbandSys.conn_latency,
                            RtkWristbandSys.conn_superv_tout, TRACE_BDADDR(remote_bd), remote_bd_type);

            start_connection_paramter_timer();
            RtkWristbandSys.wristband_conn_id = conn_id;

            g_dfu_para.dfu_conn_para_update_in_progress = false;
            g_dfu_para.dfu_conn_interval = RtkWristbandSys.conn_interval;
            g_dfu_para.dfu_conn_lantency = RtkWristbandSys.conn_latency;
        }
        break;

    default:
        break;
    }
    RtkWristbandSys.gap_conn_state = new_state;
}

/**
 * @brief    Handle msg LE_GAP_MSG_TYPE_AUTHEN_STATE_CHANGE
 * @note     All the gap authentication state events are pre-handled in this function.
 *           Then the event handling function shall be called according to the new_state
 * @param[in] conn_id Connection ID
 * @param[in] new_state  New authentication state
 * @param[in] cause Use this cause when new_state is GAPSEC_AUTHEN_STATE_COMPLETE
 * @return   void
 */
void app_handle_authen_state_evt(uint8_t conn_id, uint8_t new_state, uint16_t cause)
{
    APP_PRINT_INFO2("app_handle_authen_state_evt:conn_id %d, cause 0x%x", conn_id, cause);

    switch (new_state)
    {
    case GAP_AUTHEN_STATE_STARTED:
        {
            APP_PRINT_INFO0("app_handle_authen_state_evt: GAPSEC_AUTHEN_STATE_STARTED");
        }
        break;

    case GAP_AUTHEN_STATE_COMPLETE:
        {
            if (cause == 0)
            {
                RtkWristbandSys.flag_field.is_paired = true;
                gatts_start_discovery(conn_id);

                APP_PRINT_INFO0("app_handle_authen_state_evt: GAPSEC_AUTHEN_STATE_COMPLETE pair success");
                uint8_t tmp_bd_addr[6];
                if (wristband_get_peer_public_bd_addr(conn_id, tmp_bd_addr))
                {
                    APP_PRINT_INFO1("[GAP_AUTHEN_STATE_COMPLETE] Get peer public bd address %b",
                                    TRACE_BDADDR(tmp_bd_addr));
                    extern uint8_t bbpro_peer_bd_addr[];
                    memcpy(bbpro_peer_bd_addr, tmp_bd_addr, 6);
                }
            }
            else
            {
                RtkWristbandSys.flag_field.is_paired = false;
                APP_PRINT_INFO0("app_handle_authen_state_evt: GAPSEC_AUTHEN_STATE_COMPLETE pair failed");

                if (RtkWristbandSys.msg_switch.data)
                {
                    APP_PRINT_INFO0("clear msg switch bit");
                    ancs_set_notification_source_notify(conn_id, false);
                    RtkWristbandSys.msg_switch.data = 0;
                    uint32_t temp = RtkWristbandSys.msg_switch.data;
                    ftl_save(&temp, MSG_SWITCH_OFFSET, MSG_SWITCH_SIZE);
                }

            }
        }
        break;

    default:
        {
            APP_PRINT_ERROR1("app_handle_authen_state_evt: unknown newstate %d", new_state);
        }
        break;
    }
}

/**
 * @brief    Handle msg LE_GAP_MSG_TYPE_CONN_MTU_INFO
 * @note     This msg is used to inform APP that exchange mtu procedure is completed.
 * @param[in] conn_id Connection ID
 * @param[in] mtu_size  New mtu size
 * @return   void
 */
void app_handle_conn_mtu_info_evt(uint8_t conn_id, uint16_t mtu_size)
{
    APP_PRINT_INFO2("app_handle_conn_mtu_info_evt: conn_id %d, mtu_size %d", conn_id, mtu_size);
    RtkWristbandSys.wristband_mtu = mtu_size;
#if SUPPORT_SILENT_OTA
    g_dfu_para.mtu_size = mtu_size;
#endif
}

/**
 * @brief    Handle msg LE_GAP_MSG_TYPE_CONN_PARA_UPDATE_CHANGE
 * @note     All the connection parameter update change  events are pre-handled in this function.
 * @param[in] conn_id Connection ID
 * @param[in] status  New update state
 * @param[in] cause Use this cause when status is GAP_CONN_PARAM_UPDATE_STATUS_FAIL
 * @return   void
 */
void app_handle_conn_param_update_evt(uint8_t conn_id, uint8_t status, uint16_t cause)
{
    switch (status)
    {
    case GAP_CONN_PARAM_UPDATE_STATUS_SUCCESS:
        {
            le_get_conn_param(GAP_PARAM_CONN_INTERVAL, &RtkWristbandSys.conn_interval, conn_id);
            le_get_conn_param(GAP_PARAM_CONN_LATENCY, &RtkWristbandSys.conn_latency, conn_id);
            le_get_conn_param(GAP_PARAM_CONN_TIMEOUT, &RtkWristbandSys.conn_superv_tout, conn_id);


            APP_PRINT_INFO3("app_handle_conn_param_update_evt update success:conn_interval 0x%x, conn_slave_latency 0x%x, conn_supervision_timeout 0x%x",
                            RtkWristbandSys.conn_interval, RtkWristbandSys.conn_latency,
                            RtkWristbandSys.conn_superv_tout);
        }
        break;

    case GAP_CONN_PARAM_UPDATE_STATUS_FAIL:
        {
            APP_PRINT_ERROR1("app_handle_conn_param_update_evt update failed: cause 0x%x", cause);
#if SUPPORT_SILENT_OTA
            if (dfu_update_param == false)
#endif
            {
                wristband_update_con_para(false);
            }

        }
        break;

    case GAP_CONN_PARAM_UPDATE_STATUS_PENDING:
        {
            APP_PRINT_INFO0("app_handle_conn_param_update_evt update pending.");
        }
        break;

    default:
        break;
    }
#if SUPPORT_SILENT_OTA
    if (dfu_update_param)
    {
        uint8_t notif_data[3];
        notif_data[0] = DFU_OPCODE_NOTIFICATION;
        notif_data[1] = DFU_OPCODE_START_DFU;
        notif_data[2] = DFU_ARV_SUCCESS;
        server_send_data(0, dfu_ser_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                         notif_data, 3, GATT_PDU_TYPE_NOTIFICATION);
        dfu_update_param = false;
    }
#endif
}

/**
 * @brief    All the BT GAP MSG are pre-handled in this function. @ref T_BT_STATUS_UPDATE_SUBTYPE
 * @note     Then the event handling function shall be called according to the
 *           subtype of T_IO_MSG
 * @param[in] p_gap_msg Pointer to GAP msg
 * @return   void
 */
void app_handle_gap_msg(T_IO_MSG *p_gap_msg)
{
    T_LE_GAP_MSG gap_msg;
    uint8_t conn_id;
    memcpy(&gap_msg, &p_gap_msg->u.param, sizeof(p_gap_msg->u.param));

    APP_PRINT_TRACE1("app_handle_gap_msg: subtype %d", p_gap_msg->subtype);
    switch (p_gap_msg->subtype)
    {
    case GAP_MSG_LE_DEV_STATE_CHANGE:
        {
            app_handle_dev_state_evt(gap_msg.msg_data.gap_dev_state_change.new_state,
                                     gap_msg.msg_data.gap_dev_state_change.cause);
        }
        break;

    case GAP_MSG_LE_CONN_STATE_CHANGE:
        {
            app_handle_conn_state_evt(gap_msg.msg_data.gap_conn_state_change.conn_id,
                                      (T_GAP_CONN_STATE)gap_msg.msg_data.gap_conn_state_change.new_state,
                                      gap_msg.msg_data.gap_conn_state_change.disc_cause);
        }
        break;

    case GAP_MSG_LE_CONN_MTU_INFO:
        {
            app_handle_conn_mtu_info_evt(gap_msg.msg_data.gap_conn_mtu_info.conn_id,
                                         gap_msg.msg_data.gap_conn_mtu_info.mtu_size);
            uint8_t mtu_size = 0;
            le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, &mtu_size,
                              gap_msg.msg_data.gap_conn_mtu_info.conn_id);
            if (mtu_size >= 23)
            {
                /* update mtu size */
                RtkWristbandSys.wristband_mtu = mtu_size;
                APP_PRINT_INFO1("[periph_handle_gap_msg] update mtu size to %d", mtu_size);
            }
        }
        break;

    case GAP_MSG_LE_CONN_PARAM_UPDATE:
        {
            app_handle_conn_param_update_evt(gap_msg.msg_data.gap_conn_param_update.conn_id,
                                             gap_msg.msg_data.gap_conn_param_update.status,
                                             gap_msg.msg_data.gap_conn_param_update.cause);
        }
        break;

    case GAP_MSG_LE_AUTHEN_STATE_CHANGE:
        {
            app_handle_authen_state_evt(gap_msg.msg_data.gap_authen_state.conn_id,
                                        gap_msg.msg_data.gap_authen_state.new_state,
                                        gap_msg.msg_data.gap_authen_state.status);
        }
        break;

    case GAP_MSG_LE_BOND_JUST_WORK:
        {
            conn_id = gap_msg.msg_data.gap_bond_just_work_conf.conn_id;
            le_bond_just_work_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
            APP_PRINT_INFO0("LE_GAP_MSG_TYPE_BOND_JUST_WORK");
        }
        break;

    case GAP_MSG_LE_BOND_PASSKEY_DISPLAY:
        {
            uint32_t display_value = 0;
            conn_id = gap_msg.msg_data.gap_bond_passkey_display.conn_id;
            le_bond_get_display_key(conn_id, &display_value);
            APP_PRINT_INFO1("LE_GAP_MSG_TYPE_BOND_PASSKEY_DISPLAY:passkey %d", display_value);
            le_bond_passkey_display_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
        }
        break;

    case GAP_MSG_LE_BOND_USER_CONFIRMATION:
        {
            uint32_t display_value = 0;
            conn_id = gap_msg.msg_data.gap_bond_user_conf.conn_id;
            le_bond_get_display_key(conn_id, &display_value);
            APP_PRINT_INFO1("LE_GAP_MSG_TYPE_BOND_USER_CONFIRMATION: passkey %d", display_value);
            le_bond_user_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
        }
        break;

    case GAP_MSG_LE_BOND_PASSKEY_INPUT:
        {
            uint32_t passkey = 888888;
            conn_id = gap_msg.msg_data.gap_bond_passkey_input.conn_id;
            APP_PRINT_INFO1("LE_GAP_MSG_TYPE_BOND_PASSKEY_INPUT: conn_id %d", conn_id);
            le_bond_passkey_input_confirm(conn_id, passkey, GAP_CFM_CAUSE_ACCEPT);
        }
        break;

    case GAP_MSG_LE_BOND_OOB_INPUT:
        {
            uint8_t oob_data[GAP_OOB_LEN] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            conn_id = gap_msg.msg_data.gap_bond_oob_input.conn_id;
            APP_PRINT_INFO0("LE_GAP_MSG_TYPE_BOND_OOB_INPUT");
            le_bond_set_param(GAP_PARAM_BOND_OOB_DATA, GAP_OOB_LEN, oob_data);
            le_bond_oob_input_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
        }
        break;

    default:
        APP_PRINT_ERROR1("app_handle_gap_msg: unknown subtype %d", p_gap_msg->subtype);
        break;
    }
}
/** @} */ /* End of group PERIPH_GAP_MSG */

/** @defgroup  PERIPH_GAP_CALLBACK GAP Callback Event Handler
    * @brief Handle GAP callback event
    * @{
    */
/**
  * @brief Callback for gap le to notify app
  * @param[in] cb_type callback msy type @ref GAP_LE_MSG_Types.
  * @param[in] p_cb_data point to callback data @ref T_LE_CB_DATA.
  * @retval result @ref T_APP_RESULT
  */
T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
{
    T_APP_RESULT result = APP_RESULT_SUCCESS;
    T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;

    switch (cb_type)
    {
    case GAP_MSG_LE_DATA_LEN_CHANGE_INFO:
        APP_PRINT_INFO3("GAP_MSG_LE_DATA_LEN_CHANGE_INFO: conn_id %d, tx octets 0x%x, max_tx_time 0x%x",
                        p_data->p_le_data_len_change_info->conn_id,
                        p_data->p_le_data_len_change_info->max_tx_octets,
                        p_data->p_le_data_len_change_info->max_tx_time);
        break;

    case GAP_MSG_LE_MODIFY_WHITE_LIST:
        APP_PRINT_INFO2("GAP_MSG_LE_MODIFY_WHITE_LIST: operation %d, cause 0x%x",
                        p_data->p_le_modify_white_list_rsp->operation,
                        p_data->p_le_modify_white_list_rsp->cause);
        break;
#if APP_PRIVACY_EN
    case GAP_MSG_LE_BOND_MODIFY_INFO:
        APP_PRINT_INFO1("GAP_MSG_LE_BOND_MODIFY_INFO: type 0x%x",
                        p_data->p_le_bond_modify_info->type);
        privacy_handle_bond_modify_msg(p_data->p_le_bond_modify_info->type,
                                       p_data->p_le_bond_modify_info->p_entry, true);
        break;
#endif
    case GAP_MSG_LE_VENDOR_MORE_PRECISE_32K:
        APP_PRINT_INFO1("GAP_MSG_LE_VENDOR_MORE_PRECISE_32K: cause 0x%x",
                        p_data->le_cause.cause);
        break;
    default:
        APP_PRINT_ERROR1("app_gap_callback: unhandled cb_type 0x%x", cb_type);
        break;
    }
    return result;
}
/** @} */ /* End of group PERIPH_GAP_CALLBACK */

/** @defgroup  PERIPH_SEVER_CALLBACK Profile Server Callback Event Handler
    * @brief Handle profile server callback event
    * @{
    */
/**
 * @brief    All the BT Profile service callback events are handled in this function
 * @note     Then the event handling function shall be called according to the
 *           service_id.
 * @param[in] service_id  Profile service ID
 * @param[in] p_data      Pointer to callback data
 * @return   Indicates the function call is successful or not
 * @retval   result @ref T_APP_RESULT
 */
T_APP_RESULT app_profile_callback(T_SERVER_ID service_id, void *p_data)
{
    T_APP_RESULT app_result = APP_RESULT_SUCCESS;
    if (service_id == SERVICE_PROFILE_GENERAL_ID)
    {
        T_SERVER_APP_CB_DATA *p_param = (T_SERVER_APP_CB_DATA *)p_data;
        switch (p_param->eventId)
        {
        case PROFILE_EVT_SRV_REG_COMPLETE:// srv register result event.
            APP_PRINT_INFO1("PROFILE_EVT_SRV_REG_COMPLETE: result %d",
                            p_param->event_data.service_reg_result);
            break;

        case PROFILE_EVT_SEND_DATA_COMPLETE:
            APP_PRINT_INFO5("PROFILE_EVT_SEND_DATA_COMPLETE: conn_id %d, cause 0x%x, service_id %d, attrib_idx 0x%x, credits %d",
                            p_param->event_data.send_data_result.conn_id,
                            p_param->event_data.send_data_result.cause,
                            p_param->event_data.send_data_result.service_id,
                            p_param->event_data.send_data_result.attrib_idx,
                            p_param->event_data.send_data_result.credits);
            if (p_param->event_data.send_data_result.cause == GAP_SUCCESS)
            {
                APP_PRINT_INFO0("PROFILE_EVT_SEND_DATA_COMPLETE success");
#if FEATURE_AIRSYNC
                extern void airsync_tx_complete(void);
                airsync_tx_complete();
#endif
            }
            else
            {
                APP_PRINT_ERROR0("PROFILE_EVT_SEND_DATA_COMPLETE failed");
            }
            break;

        default:
            break;
        }
    }
    else if (service_id == ias_ser_id)
    {
        T_IAS_CALLBACK_DATA *p_ias_cb_data = (T_IAS_CALLBACK_DATA *)p_data;
        switch (p_ias_cb_data->msg_type)
        {
        case SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE:
            {
                if (p_ias_cb_data->msg_data.write_alert_level)
                {
                    wristband_system_interact(INTERACT_FIND_ME, 0, NULL);
                }
            }
            break;

        default:
            break;
        }
    }

    else if (service_id == ota_ser_id)
    {
        TOTA_CALLBACK_DATA *pOTACallbackData = (TOTA_CALLBACK_DATA *)p_data;
        switch (pOTACallbackData->msg_type)
        {
        case SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE:

            if (OTA_WRITE_CHAR_VAL == pOTACallbackData->msg_data.write.opcode &&
                OTA_VALUE_ENTER == pOTACallbackData->msg_data.write.u.value)
            {

                /*battery level is above 60 percent*/
                APP_PRINT_INFO0("Preparing switch into OTA mode\n");
                /*prepare to enter OTA mode, before switch action, we should disconnect first.*/
                switch_into_ota_pending = true;
                le_disconnect(RtkWristbandSys.wristband_conn_id);
            }
            else if (OTA_WRITE_IMAGE_COUNT_VAL == pOTACallbackData->msg_data.write.opcode)
            {

            }

            break;

        default:

            break;
        }
    }
#if SUPPORT_SILENT_OTA
    else if (service_id == dfu_ser_id)
    {
        T_DFU_CALLBACK_DATA *pDfuCallbackData = (T_DFU_CALLBACK_DATA *)p_data;
        if (pDfuCallbackData->msg_type == SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION)
        {
            if (pDfuCallbackData->msg_data.notification_indification_index == DFU_NOTIFY_ENABLE)
            {
                APP_PRINT_INFO0("dfu notification enable");
            }
            else if (pDfuCallbackData->msg_data.notification_indification_index ==
                     DFU_NOTIFY_DISABLE)
            {
                APP_PRINT_INFO0("dfu notification disable");
            }
        }
        else if (pDfuCallbackData->msg_type == SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE)
        {
            uint8_t dfu_write_opcode = pDfuCallbackData->msg_data.write.opcode;
            if (dfu_write_opcode == DFU_WRITE_ATTR_EXIT)
            {
                if (pDfuCallbackData->msg_data.write.write_attrib_index == INDEX_DFU_CONTROL_POINT_CHAR_VALUE)
                {
                    uint8_t control_point_opcode = *pDfuCallbackData->msg_data.write.p_value;
                    switch (control_point_opcode)
                    {
                    case DFU_OPCODE_START_DFU:
                        {
                            stop_connection_paramter_timer();
                            le_update_conn_param(RtkWristbandSys.wristband_conn_id, 0x08, 0x11, 0,
                                                 5000 / 10, 0x08 * 2 - 2, 0x11 * 2 - 2);
                            dfu_update_param = true;
                        }
                        break;
                    case DFU_OPCODE_ACTIVE_IMAGE_RESET:
                        {
                            if (g_dfu_para.ctrl_header.image_id == IMAGE_USER_DATA)
                            {
                                start_connection_paramter_timer();
                            }
                            le_disconnect(0);
                            dfu_active_rst_pending = true;
                        }
                        break;

                    default:
                        break;
                    }
                }
            }
            else if (dfu_write_opcode == DFU_WRITE_FAIL)
            {
                DBG_DIRECT("DFU FAIL!");
            }
        }
    }
#endif
    else if (service_id == bas_ser_id)
    {
        T_BAS_CALLBACK_DATA *pBasCallbackData = (T_BAS_CALLBACK_DATA *)p_data;
        switch (pBasCallbackData->msg_type)
        {
        case SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION:
            {
                if (pBasCallbackData->msg_data.notification_indification_index == BAS_NOTIFY_BATTERY_LEVEL_ENABLE)
                {
                    APP_PRINT_INFO0("profile callback BAS_NOTIFY_BATTERY_LEVEL_ENABLE\n");
                }
                else if (pBasCallbackData->msg_data.notification_indification_index ==
                         BAS_NOTIFY_BATTERY_LEVEL_DISABLE)
                {
                    APP_PRINT_INFO0("profile callback BAS_NOTIFY_BATTERY_LEVEL_DISABLE\n");
                }
                break;
            }
        case SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE:
            {
                break;
            }
        default:
            break;
        }
    }
#if FEATURE_HID
    else  if (service_id == hid_kb_ser_id)
    {
        T_HID_CALLBACK_DATA *p_hid_cb_data = (T_HID_CALLBACK_DATA *)p_data;
        //extern bool report_cccd_enable;

        switch (p_hid_cb_data->msg_type)
        {
        case SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION:
            {
                switch (p_hid_cb_data->msg_data.not_ind_data.index)
                {
                case GATT_SVC_HID_MM_IN_REPORT_CCCD_INDEX:
                    {
                        if (p_hid_cb_data->msg_data.not_ind_data.value == NOTIFY_ENABLE)
                        {
                            //report_cccd_enable = true;
                        }
                        else
                        {
                            //report_cccd_enable = false;
                        }
                    }
                    break;
                default:
                    break;
                }
            }
            break;
        default:
            break;
        }
    }
#endif
#if FEATURE_AIRSYNC
    else if (service_id == airsync_ser_id)
    {
        T_WAS_CALLBACK_DATA *pWasCallback_data = (T_WAS_CALLBACK_DATA *)p_data;
        switch (pWasCallback_data->msg_type)
        {
        case SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE:
            {
                APP_PRINT_INFO0("SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE... ");
                switch ((pWasCallback_data->msg_data).write.opcode)
                {
                case GATT_UUID_AIRSYNC_CHAR_WRITE_INDEX:
                    {
                        APP_PRINT_INFO0("do nothing SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE... ");
                    }
                    break;
                default:
                    break;
                }
                break;
            }
        case SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION:
            {
                if ((pWasCallback_data->msg_data).notification_indification_index ==
                    GATT_UUID_AIRSYNC_CHAR_INDICATION_ENABLE)
                {
                    T_IO_MSG enable_indicate_msg;
                    enable_indicate_msg.type = IO_MSG_TYPE_WRISTBNAD;
                    enable_indicate_msg.subtype = IO_MSG_WAS_ENABLE_CCCD;
                    /* Send MSG to APP task */
                    app_send_msg_to_apptask(&enable_indicate_msg);
                }
                else
                {
                    //do nothing
                }
                break;
            }
        default:
            break;
        }
    }
#endif

    return app_result;
}
#if APP_PRIVACY_EN

/**
  * @brief Callback for BLE privacy management module to notify app
  * @param[in] type     callback msy type @ref T_PRIVACY_CB_TYPE.
  * @param[in] cb_data  callback data.
  * @retval void
  */
void app_privacy_callback(T_PRIVACY_CB_TYPE type, T_PRIVACY_CB_DATA cb_data)
{
    APP_PRINT_INFO1("app_privacy_callback: type %d", type);
    switch (type)
    {
    case PRIVACY_STATE_MSGTYPE:
        app_privacy_state = cb_data.privacy_state;
        APP_PRINT_INFO1("PRIVACY_STATE_MSGTYPE: status %d", app_privacy_state);
        break;

    case PRIVACY_RESOLUTION_STATUS_MSGTYPE:
        app_privacy_resolution_state = cb_data.resolution_state;
        APP_PRINT_INFO1("PRIVACY_RESOLUTION_STATUS_MSGTYPE: status %d", app_privacy_resolution_state);
        break;

    default:
        break;
    }
}
/**
 * @brief  Callback will be called when data sent from profile client layer.
 * @param  client_id the ID distinguish which module sent the data.
 * @param  conn_id connection ID.
 * @param  p_data  pointer to data.
 * @retval   result @ref T_APP_RESULT
 */
T_APP_RESULT app_client_callback(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data)
{
    T_APP_RESULT  result = APP_RESULT_SUCCESS;
    APP_PRINT_INFO2("app_client_callback: client_id %d, conn_id %d",
                    client_id, conn_id);
    if (client_id == gatts_client_id)
    {
        T_GATTS_CLIENT_CB_DATA *p_gatts_client_cb_data = (T_GATTS_CLIENT_CB_DATA *)p_data;
        uint16_t value_size;
        uint8_t *p_value;
        uint16_t start_handle = 0;
        uint16_t end_handle = 0;
        switch (p_gatts_client_cb_data->cb_type)
        {
        case GATTS_CLIENT_CB_TYPE_DISC_STATE:
            switch (p_gatts_client_cb_data->cb_content.disc_state)
            {
            case DISC_GATTS_DONE:
                APP_PRINT_INFO0("app_client_callback: discover gatt service procedure done.");
                gatts_client_set_service_changed_ind(conn_id, true);
                break;
            case DISC_GATTS_FAILED:
                /* Discovery Request failed. */
                APP_PRINT_INFO0("app_client_callback: discover gatt service simp request failed.");
                break;
            default:
                break;
            }
            break;

        case GATTS_CLIENT_CB_TYPE_WRITE_RESULT:
            switch (p_gatts_client_cb_data->cb_content.write_result.type)
            {
            case GATTS_WRITE_SERVICE_CHANGED_IND_ENABLE:
                APP_PRINT_INFO1("GATTS_WRITE_SERVICE_CHANGED_IND_ENABLE: write result 0x%x",
                                p_gatts_client_cb_data->cb_content.write_result.cause);
                ancs_start_discovery(conn_id);
                break;
            case GATTS_WRITE_SERVICE_CHANGED_IND_DISABLE:
                APP_PRINT_INFO1("GATTS_WRITE_SERVICE_CHANGED_IND_DISABLE: write result 0x%x",
                                p_gatts_client_cb_data->cb_content.write_result.cause);
                break;
            default:
                break;
            }
            break;

        case GATTS_CLIENT_CB_TYPE_NOTIF_IND_RESULT:
            value_size = p_gatts_client_cb_data->cb_content.notif_ind_data.data.value_size;
            p_value = p_gatts_client_cb_data->cb_content.notif_ind_data.data.p_value;
            switch (p_gatts_client_cb_data->cb_content.notif_ind_data.type)
            {
            case GATTS_SERVICE_CHANGED_INDICATE:
                start_handle = (*p_value) + ((*(p_value + 1)) << 8);
                end_handle = (*(p_value + 2)) + ((*(p_value + 3)) << 8);
                APP_PRINT_INFO3("GATTS_SERVICE_CHANGED_INDICATE: value_size %d, start handle 0x%x, end handle 0x%x",
                                value_size, start_handle, end_handle);
                start_ancs_rediscovery_timer();
                break;
            default:
                break;
            }
            break;

        default:
            break;
        }
    }

    return result;
}
#endif
/** @} */ /* End of group PERIPH_SEVER_CALLBACK */
/** @} */ /* End of group PERIPH_APP */
/** @} */ /* End of group PERIPH_DEMO */
