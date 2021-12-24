/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      dfu_application.c
* @brief     dfu task application implementation
* @details   dfu task application implementation
* @author
* @date
* @version
* *********************************************************************************************************
*/
#include <trace.h>
#include <string.h>
#include "app_msg.h"
#include "gap.h"
#include "gap_adv.h"
#include "gap_bond_le.h"
#include "gap_conn_le.h"
#include "gap_msg.h"
#include "profile_server.h"
#include "os_timer.h"
#include "otp.h"
#include "dfu_api.h"
#include "dfu_service.h"
#include "ota_service.h"
#include "dfu_main.h"
#include "dfu_flash.h"
#include "dfu_application.h"
#include "dfu_task.h"
#include "board.h"

#if (SUPPORT_NORMAL_OTA == 1)
/*============================================================================*
 *                              Macros
 *============================================================================*/


/*============================================================================*
 *                              Local Variables
 *============================================================================*/
T_GAP_DEV_STATE dfu_gap_cur_state = {0, 0, 0, 0};
T_GAP_CONN_STATE dfu_gap_conn_state = GAP_CONN_STATE_DISCONNECTED;
static bool rtk_dfu_active_reset_pending = false;

void dfu_peripheral_handle_gap_msg(T_IO_MSG  *p_gap_msg);

/*============================================================================*
 *                              External Variables
 *============================================================================*/


/*============================================================================*
 *                              Functions
 *============================================================================*/
/*
 * @fn          app_handle_io_msg
 * @brief      All the application events are pre-handled in this function.
 *                All the IO MSGs are sent to this function, Then the event handling function
 *                shall be called according to the MSG type.
 *
 * @param    io_msg  - bee io msg data
 * @return     void
 */
void dfu_handle_io_msg(T_IO_MSG io_msg)
{
    uint16_t msg_type = io_msg.type;

    switch (msg_type)
    {
    case IO_MSG_TYPE_BT_STATUS:
        {
            dfu_peripheral_handle_gap_msg(&io_msg);
        }
        break;
    case IO_MSG_TYPE_DFU_VALID_FW:
        {
            APP_PRINT_INFO0("IO_MSG_TYPE_DFU_VALID_FW");
            dfu_service_handle_valid_fw(io_msg.u.param);
        }
        break;
    default:
        break;
    }
}

/******************************************************************
 * @fn          peripheral_HandleBtDevStateChangeEvt
 * @brief      All the gaprole_States_t events are pre-handled in this function.
 *                Then the event handling function shall be called according to the newState.
 *
 * @param    newState  - new gap state
 * @return     void
 */
void dfu_periph_handle_dev_state_evt(T_GAP_DEV_STATE new_state, uint16_t cause)
{
    DFU_PRINT_INFO4("dfu_periph_handle_dev_state_evt: init state %d, adv state %d, conn state %d, cause 0x%x",
                    new_state.gap_init_state, new_state.gap_adv_state,
                    new_state.gap_conn_state, cause);
    if (dfu_gap_cur_state.gap_init_state != new_state.gap_init_state)
    {
        if (new_state.gap_init_state == GAP_INIT_STATE_STACK_READY)
        {
            dfu_set_rand_addr();

            /*stack ready*/
            le_adv_start();
        }
    }

    if (dfu_gap_cur_state.gap_adv_state != new_state.gap_adv_state)
    {
        if (new_state.gap_adv_state == GAP_ADV_STATE_IDLE)
        {
            if (new_state.gap_adv_sub_state == GAP_ADV_TO_IDLE_CAUSE_CONN)
            {
                DFU_PRINT_INFO0("GAP adv stoped: because connection created");
            }
            else
            {
                DFU_PRINT_INFO0("GAP adv stoped");
            }
        }
        else if (new_state.gap_adv_state == GAP_ADV_STATE_ADVERTISING)
        {
            DFU_PRINT_INFO0("GAP adv start");
        }
    }

    if (dfu_gap_cur_state.gap_conn_state != new_state.gap_conn_state)
    {
        DFU_PRINT_INFO2("conn state: %d -> %d",
                        dfu_gap_cur_state.gap_conn_state,
                        new_state.gap_conn_state);
    }
    dfu_gap_cur_state = new_state;
}

void dfu_periph_handle_conn_state_evt(uint8_t conn_id, T_GAP_CONN_STATE new_state,
                                      uint16_t disc_cause)
{
    DFU_PRINT_INFO3("dfu_periph_handle_conn_state_evt: conn_id = %d old_state = %d new_state = %d",
                    conn_id, dfu_gap_conn_state, new_state);
    switch (new_state)
    {
    case GAP_CONN_STATE_DISCONNECTED:
        {
            if ((disc_cause != (HCI_ERR | HCI_ERR_REMOTE_USER_TERMINATE))
                && (disc_cause != (HCI_ERR | HCI_ERR_LOCAL_HOST_TERMINATE)))
            {
                DFU_PRINT_ERROR1("connection lost: cause 0x%x", disc_cause);
            }

            if (rtk_dfu_active_reset_pending)
            {
                rtk_dfu_active_reset_pending = false;
                //unlock_flash_bp_all();
                dfu_fw_reboot(true);
            }
            else
            {
                if (OTP->ota_link_loss_reset)
                {
                    dfu_fw_reboot(false);
                }
                else
                {
                    le_adv_start();
                }
            }

        }
        break;

    case GAP_CONN_STATE_CONNECTED:
        {
            uint16_t conn_interval;
            uint16_t conn_latency;
            uint16_t conn_supervision_timeout;
            uint8_t  remote_bd[6];
            T_GAP_REMOTE_ADDR_TYPE remote_bd_type;

            le_get_conn_param(GAP_PARAM_CONN_INTERVAL, &conn_interval, conn_id);
            le_get_conn_param(GAP_PARAM_CONN_LATENCY, &conn_latency, conn_id);
            le_get_conn_param(GAP_PARAM_CONN_TIMEOUT, &conn_supervision_timeout, conn_id);
            le_get_conn_addr(conn_id, remote_bd, (unsigned char *)&remote_bd_type);
            DFU_PRINT_INFO5("GAPSTATE_CONN_CONNECTED:remote_bd %s, remote_addr_type %d, conn_interval 0x%x, conn_latency 0x%x, conn_supervision_timeout 0x%x",
                            TRACE_BDADDR(remote_bd), remote_bd_type,
                            conn_interval, conn_latency, conn_supervision_timeout);

            g_dfu_para.dfu_conn_para_update_in_progress = false;
            g_dfu_para.dfu_conn_interval = conn_interval;
            g_dfu_para.dfu_conn_lantency = conn_latency;

            os_timer_stop(&wait4_conn_timer_handle);  //to check?
        }
        break;

    default:
        break;
    }
    dfu_gap_conn_state = new_state;
}

/******************************************************************
 * @fn          peripheral_HandleBtGapAuthenStateChangeEvt
 * @brief      All the bonding state change  events are pre-handled in this function.
 *                Then the event handling function shall be called according to the newState.
 *
 * @param    newState  - new bonding state
 * @return     void
 */
void dfu_periph_handle_authen_state_evt(uint8_t conn_id, uint8_t new_state, uint16_t cause)
{
    DFU_PRINT_INFO1("dfu_periph_handle_authen_state_evt:conn_id=%d", conn_id);

    switch (new_state)
    {
    case GAP_AUTHEN_STATE_STARTED:
        {
            DFU_PRINT_INFO0("GAPSEC_AUTHEN_STATE_STARTED");
        }
        break;

    case GAP_AUTHEN_STATE_COMPLETE:
        {
            DFU_PRINT_INFO0("GAPSEC_AUTHEN_STATE_COMPLETE");
            if (cause == 0)
            {
                DFU_PRINT_INFO0("LE_GAP_MSG_TYPE_AUTHEN_STATE_CHANGE pair success");
            }
            else
            {
                DFU_PRINT_INFO0("LE_GAP_MSG_TYPE_AUTHEN_STATE_CHANGE pair failed");
            }
        }
        break;

    default:
        {
            DFU_PRINT_INFO1("LE_GAP_MSG_TYPE_AUTHEN_STATE_CHANGE: unknown newstate=%d!", new_state);
        }
        break;
    }
}

/******************************************************************
 * @fn          peripheral_HandleBtGapConnParaChangeEvt
 * @brief      All the connection parameter update change  events are pre-handled in this function.
 *                Then the event handling function shall be called according to the status.
 *
 * @param    status  - connection parameter result, 0 - success, otherwise fail.
 * @return     void
 */
void dfu_periph_conn_param_update_evt(uint8_t conn_id, uint8_t status, uint16_t cause)
{
    switch (status)
    {
    case GAP_CONN_PARAM_UPDATE_STATUS_SUCCESS:
        {
            uint16_t conn_interval;
            uint16_t conn_slave_latency;
            uint16_t conn_supervision_timeout;

            le_get_conn_param(GAP_PARAM_CONN_INTERVAL, &conn_interval, conn_id);
            le_get_conn_param(GAP_PARAM_CONN_LATENCY, &conn_slave_latency, conn_id);
            le_get_conn_param(GAP_PARAM_CONN_TIMEOUT, &conn_supervision_timeout, conn_id);
            DFU_PRINT_INFO3("LE_GAP_MSG_TYPE_CONN_PARA_UPDATE_CHANGE success(Normal OTA): interval=0x%x, slave_latency=0x%x, supervision_timeout=0x%x",
                            conn_interval, conn_slave_latency, conn_supervision_timeout);

            g_dfu_para.dfu_conn_interval = conn_interval;
            g_dfu_para.dfu_conn_lantency = conn_slave_latency;
            dfu_notify_conn_para_update_req(conn_id, DFU_ARV_SUCCESS);
        }
        break;

    case GAP_CONN_PARAM_UPDATE_STATUS_FAIL:
        {
            DFU_PRINT_ERROR1("LE_GAP_MSG_TYPE_CONN_PARA_UPDATE_CHANGE failed(Normal OTA): cause 0x%x", cause);
            dfu_notify_conn_para_update_req(conn_id, DFU_ARV_FAIL_OPERATION);
        }
        break;

    case GAP_CONN_PARAM_UPDATE_STATUS_PENDING:
        {
            DFU_PRINT_INFO0("LE_GAP_MSG_TYPE_CONN_PARA_UPDATE_CHANGE Request success but pending(Normal OTA).");
        }
        break;

    default:
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
void dfu_periph_handle_conn_mtu_info_evt(uint8_t conn_id, uint16_t mtu_size)
{
    DFU_PRINT_INFO2("dfu_periph_handle_conn_mtu_info_evt: conn_id %d, mtu_size %d", conn_id, mtu_size);
}
/******************************************************************
 * @fn          peripheral_HandleBtGapMessage
 * @brief      All the bt gap msg  events are pre-handled in this function.
 *                Then the event handling function shall be called according to the subType
 *                of BEE_IO_MSG.
 *
 * @param    pBeeIoMsg  - pointer to bee io msg
 * @return     void
 */
void dfu_peripheral_handle_gap_msg(T_IO_MSG *p_gap_msg)
{
    T_LE_GAP_MSG gap_msg;
    uint8_t conn_id;
    memcpy(&gap_msg, &p_gap_msg->u.param, sizeof(p_gap_msg->u.param));

    DFU_PRINT_TRACE1("dfu_peripheral_handle_gap_msg: subtype %d", p_gap_msg->subtype);
    switch (p_gap_msg->subtype)
    {
    case GAP_MSG_LE_DEV_STATE_CHANGE:
        {
            dfu_periph_handle_dev_state_evt(gap_msg.msg_data.gap_dev_state_change.new_state,
                                            gap_msg.msg_data.gap_dev_state_change.cause);
        }
        break;

    case GAP_MSG_LE_CONN_STATE_CHANGE:
        {
            dfu_periph_handle_conn_state_evt(gap_msg.msg_data.gap_conn_state_change.conn_id,
                                             (T_GAP_CONN_STATE)gap_msg.msg_data.gap_conn_state_change.new_state,
                                             gap_msg.msg_data.gap_conn_state_change.disc_cause);
        }
        break;

    case GAP_MSG_LE_CONN_MTU_INFO:
        {
            dfu_periph_handle_conn_mtu_info_evt(gap_msg.msg_data.gap_conn_mtu_info.conn_id,
                                                gap_msg.msg_data.gap_conn_mtu_info.mtu_size);
        }
        break;

    case GAP_MSG_LE_CONN_PARAM_UPDATE:
        {
            dfu_periph_conn_param_update_evt(gap_msg.msg_data.gap_conn_param_update.conn_id,
                                             gap_msg.msg_data.gap_conn_param_update.status,
                                             gap_msg.msg_data.gap_conn_param_update.cause);
        }
        break;

    case GAP_MSG_LE_AUTHEN_STATE_CHANGE:
        {
            dfu_periph_handle_authen_state_evt(gap_msg.msg_data.gap_authen_state.conn_id,
                                               gap_msg.msg_data.gap_authen_state.new_state,
                                               gap_msg.msg_data.gap_authen_state.status);
        }
        break;

    case GAP_MSG_LE_BOND_JUST_WORK:
        {
            conn_id = gap_msg.msg_data.gap_bond_just_work_conf.conn_id;
            le_bond_just_work_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
            DFU_PRINT_INFO0("LE_GAP_MSG_TYPE_BOND_JUST_WORK");
        }
        break;

    case GAP_MSG_LE_BOND_PASSKEY_DISPLAY:
        {
            uint32_t display_value = 0;
            conn_id = gap_msg.msg_data.gap_bond_passkey_display.conn_id;
            le_bond_get_display_key(conn_id, &display_value);
            DFU_PRINT_INFO1("LE_GAP_MSG_TYPE_BOND_PASSKEY_DISPLAY:passkey %d", display_value);
            le_bond_passkey_display_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
        }
        break;

    case GAP_MSG_LE_BOND_USER_CONFIRMATION:
        {
            uint32_t display_value = 0;
            conn_id = gap_msg.msg_data.gap_bond_user_conf.conn_id;
            le_bond_get_display_key(conn_id, &display_value);
            DFU_PRINT_INFO1("LE_GAP_MSG_TYPE_BOND_USER_CONFIRMATION: passkey %d", display_value);
#ifndef SUPPORT_ALONE_UPPERSTACK_IMG
            le_bond_user_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
#endif
        }
        break;

    case GAP_MSG_LE_BOND_PASSKEY_INPUT:
        {
            uint32_t passkey = 888888;
            conn_id = gap_msg.msg_data.gap_bond_passkey_input.conn_id;
            DFU_PRINT_INFO1("LE_GAP_MSG_TYPE_BOND_PASSKEY_INPUT: conn_id %d", conn_id);
            le_bond_passkey_input_confirm(conn_id, passkey, GAP_CFM_CAUSE_ACCEPT);
        }
        break;

    case GAP_MSG_LE_BOND_OOB_INPUT:
        {
            DFU_PRINT_INFO0("LE_GAP_MSG_TYPE_BOND_OOB_INPUT");
            conn_id = gap_msg.msg_data.gap_bond_oob_input.conn_id;
#ifndef SUPPORT_ALONE_UPPERSTACK_IMG
            uint8_t oob_data[GAP_OOB_LEN] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            le_bond_set_param(GAP_PARAM_BOND_OOB_DATA, GAP_OOB_LEN, oob_data);
            le_bond_oob_input_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
#endif
        }
        break;

    default:
        DFU_PRINT_ERROR1("dfu_peripheral_handle_gap_msg: unknown subtype %d", p_gap_msg->subtype);
        break;
    }
}
/** @defgroup  DFU_GAP_CALLBACK GAP Callback Event Handler
    * @brief Handle GAP callback event
    * @{
    */
/**
  * @brief Callback for gap le to notify app
  * @param[in] cb_type callback msy type @ref GAP_LE_MSG_Types.
  * @param[in] p_cb_data point to callback data @ref T_LE_CB_DATA.
  * @retval result @ref T_APP_RESULT
  */
T_APP_RESULT dfu_gap_callback(uint8_t cb_type, void *p_cb_data)
{
    T_APP_RESULT result = APP_RESULT_SUCCESS;
    T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;

    switch (cb_type)
    {
    case GAP_MSG_LE_DATA_LEN_CHANGE_INFO:
        DFU_PRINT_INFO3("GAP_MSG_LE_DATA_LEN_CHANGE_INFO: conn_id %d, tx octets 0x%x, max_tx_time 0x%x",
                        p_data->p_le_data_len_change_info->conn_id,
                        p_data->p_le_data_len_change_info->max_tx_octets,
                        p_data->p_le_data_len_change_info->max_tx_time);
        break;

    case GAP_MSG_LE_BOND_MODIFY_INFO:
        DFU_PRINT_INFO1("GAP_MSG_LE_BOND_MODIFY_INFO: type 0x%x",
                        p_data->p_le_bond_modify_info->type);
        break;

    case GAP_MSG_LE_MODIFY_WHITE_LIST:
        DFU_PRINT_INFO2("GAP_MSG_LE_MODIFY_WHITE_LIST: operation %d, cause 0x%x",
                        p_data->p_le_modify_white_list_rsp->operation,
                        p_data->p_le_modify_white_list_rsp->cause);
        break;
    case GAP_MSG_LE_SET_RAND_ADDR:
        DFU_PRINT_INFO1("GAP_MSG_LE_SET_RAND_ADDR: cause 0x%x",
                        p_data->p_le_set_rand_addr_rsp->cause);

    default:
        DFU_PRINT_INFO1("app_gap_callback: unhandled cb_type 0x%x", cb_type);
        break;
    }
    return result;
}

/******************************************************************
 * @fn          app_profile_callback
 * @brief      All the bt profile callbacks are handled in this function.
 *                Then the event handling function shall be called according to the serviceID
 *                of BEE_IO_MSG.
 *
 * @param    serviceID  -  service id of profile
 * @param    pData  - pointer to callback data
 * @return     void
 */

T_APP_RESULT dfu_profile_callback(T_SERVER_ID service_id, void *p_data)
{
    T_APP_RESULT app_result = APP_RESULT_SUCCESS;
    if (service_id == SERVICE_PROFILE_GENERAL_ID)
    {
        T_SERVER_APP_CB_DATA *p_param = (T_SERVER_APP_CB_DATA *)p_data;
        switch (p_param->eventId)
        {
        case PROFILE_EVT_SRV_REG_COMPLETE:// srv register result event.
            DFU_PRINT_INFO1("PROFILE_EVT_SRV_REG_COMPLETE: result %d",
                            p_param->event_data.service_reg_result);
            break;

        case PROFILE_EVT_SEND_DATA_COMPLETE:
            DFU_PRINT_INFO5("PROFILE_EVT_SEND_DATA_COMPLETE: conn_id %d, cause 0x%x, service_id %d, attrib_idx 0x%x, credits = %d",
                            p_param->event_data.send_data_result.conn_id,
                            p_param->event_data.send_data_result.cause,
                            p_param->event_data.send_data_result.service_id,
                            p_param->event_data.send_data_result.attrib_idx,
                            p_param->event_data.send_data_result.credits);
            if (p_param->event_data.send_data_result.cause == GAP_SUCCESS)
            {
                DFU_PRINT_INFO0("PROFILE_EVT_SEND_DATA_COMPLETE success");
            }
            else
            {
                DFU_PRINT_ERROR0("PROFILE_EVT_SEND_DATA_COMPLETE failed");
            }
            break;

        default:
            break;
        }
    }
    else if (service_id == rtk_dfu_service_id)
    {
        T_DFU_CALLBACK_DATA *p_dfu_cb_data = (T_DFU_CALLBACK_DATA *)p_data;
        switch (p_dfu_cb_data->msg_type)
        {
        case SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION:
            {
                if (p_dfu_cb_data->msg_data.notification_indification_index == DFU_NOTIFY_ENABLE)
                {
                    DFU_PRINT_INFO0("dfu notification enable");
                }
                else if (p_dfu_cb_data->msg_data.notification_indification_index ==
                         DFU_NOTIFY_DISABLE)
                {
                    DFU_PRINT_INFO0("dfu notification disable");
                }
            }
            break;
        case SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE:
            {
                uint8_t dfu_write_opcode = p_dfu_cb_data->msg_data.write.opcode;
                if (DFU_WRITE_ATTR_EXIT == dfu_write_opcode)
                {
                    if (p_dfu_cb_data->msg_data.write.write_attrib_index == INDEX_DFU_CONTROL_POINT_CHAR_VALUE)
                    {
                        uint8_t control_point_opcode = *p_dfu_cb_data->msg_data.write.p_value;
                        switch (control_point_opcode)
                        {
                        case DFU_OPCODE_VALID_FW:
                            {
                                T_IO_MSG dfu_valid_fw_msg;
                                dfu_valid_fw_msg.type = IO_MSG_TYPE_DFU_VALID_FW;
                                dfu_valid_fw_msg.u.param = p_dfu_cb_data->conn_id;
                                if (app_send_msg_to_dfutask(&dfu_valid_fw_msg) == false)
                                {
                                    DBG_DIRECT("DFU send Valid FW msg fail!");
                                }
                            }
                            break;
                        case DFU_OPCODE_ACTIVE_IMAGE_RESET:
                            {
#if (ENABLE_AUTO_BANK_SWITCH == 1)
                                if (is_ota_support_bank_switch())
                                {
                                    uint32_t ota_addr;
                                    ota_addr = get_header_addr_by_img_id(OTA);
                                    DFU_PRINT_INFO1("DFU_OPCODE_ACTIVE_IMAGE_RESET: Bank switch erase ota_addr=0x%x", ota_addr);
                                    unlock_flash_bp_all();
                                    flash_erase_locked(FLASH_ERASE_SECTOR, ota_addr);
                                    lock_flash_bp();
                                }
#endif
                                le_disconnect(0);
                                rtk_dfu_active_reset_pending = true;
                            }
                            break;
                        default:
                            break;
                        }
                    }
                }
                else if (DFU_WRITE_START == dfu_write_opcode)
                {
                    T_IMG_CTRL_HEADER_FORMAT *p_header = (T_IMG_CTRL_HEADER_FORMAT *)
                                                         p_dfu_cb_data->msg_data.write.p_value;
                    if (p_header->image_id >= OTA && p_header->image_id < IMAGE_MAX &&
                        p_header->payload_len < flash_get_bank_size(FLASH_OTA_BANK_0))
                    {
                        uint32_t total_period = timeout_value_total * ((p_header->payload_len + 1024) / 102400 + 1);
                        os_timer_restart(&total_timer_handle,
                                         total_period);
                        uint32_t transfer_period = timeout_value_image_transfer * ((p_header->payload_len + 1024) / 102400 +
                                                                                   1);
                        os_timer_restart(&image_transfer_timer_handle, transfer_period);
                        DBG_DIRECT("[Normal OTA] restart timer, total=%dms, transfer=%dms!", total_period, transfer_period);
                    }
                }
                else if (DFU_WRITE_FAIL == dfu_write_opcode)
                {
                    DBG_DIRECT("DFU FAIL! reason=%d", *p_dfu_cb_data->msg_data.write.p_value);
                }
                else
                {
                }
            }
            break;
        default:
            break;
        }
    }
    else
    {
    }


    return app_result;
}

#endif //end SUPPORT_NORMAL_OTA
