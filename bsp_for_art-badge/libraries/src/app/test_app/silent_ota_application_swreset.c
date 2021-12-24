/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      silent_ota_application.c
* @brief     silent ota application implementation
* @details   silent ota application implementation
* @author    Grace
* @date
* @version
* *********************************************************************************************************
*/
#include <trace.h>
#include <string.h>
#include "gap.h"
#include "gap_adv.h"
#include "gap_msg.h"
#include "gap_bond_le.h"
#include "gap_conn_le.h"
#include "profile_server.h"
#include "app_msg.h"
#include "patch_header_check.h"
#include "rtl876x_wdg.h"
#include "dfu_flash.h"
#include "dfu_api.h"
#include "bas.h"
#include "dis.h"
#include "dfu_service.h"
#include "ota_service.h"
#include "silent_ota_application_swreset.h"
#include "board.h"
#include "test_app.h"
#if (DEBUG_TEST_WDG_APP_ENABLE == 1)

#include "power_test.h"

T_POWER_SETTINGS power_settings =
{
    .power_test_signature = {'P', 'O', 'W', 'E', 'R', 'T', 'S', 'T'},
    .power_scenario = POWER_ADV_CONN,
    .power_supply_1p2v = POWER_SUPPLY_BUCK,
    .active_clock = CPU_CLOCK_20MHZ,
    .slow_clock = CPU_CLOCK_625KHZ,
    .adv_interval = 0x640,//interval = adv_interval * 0.625ms==>0x20:20ms  0x140 200ms 0x1e0: 300ms 0x640:1000ms
    .conn_interval = 0x50,//interval = conn_interval * 1.25ms==>0x50:100ms
    .tx_power = TX_0_DBM,
    .guard_time_tune_en = false,
};
#endif

#if (DEBUG_TEST_WDG_APP_ENABLE == 1)
#include "power_test.h"
extern T_GAP_CAUSE le_scan_start(void);
void delay_us_asm(uint32_t us);
#endif
/*============================================================================*
 *                              Macros
 *============================================================================*/


/*============================================================================*
 *                              external Variables
 *============================================================================*/
extern T_SERVER_ID g_bas_service_id;
extern T_SERVER_ID g_dis_service_id;
extern T_SERVER_ID g_ota_service_id;
extern T_SERVER_ID g_dfu_service_id;

uint16_t g_bat_vol;
uint16_t g_bas_battery_level;

/*============================================================================*
 *                              Local Variables
 *============================================================================*/
T_GAP_DEV_STATE gap_cur_state = {0, 0, 0, 0};
T_GAP_CONN_STATE gap_conn_state = GAP_CONN_STATE_DISCONNECTED;

static bool switch_to_ota_mode_pending = false;
static bool dfu_active_rst_pending = false;
LantencyStatus g_lantency_status = LANTENCY_OFF;

void peripheral_handle_gap_msg(T_IO_MSG  *p_gap_msg);

/*
 * @fn          app_handle_io_msg
 * @brief      All the application events are pre-handled in this function.
 *                All the IO MSGs are sent to this function, Then the event handling function
 *                shall be called according to the MSG type.
 *
 * @param    io_driver_msg_recv  - bee io msg data
 * @return     void
 */
void app_handle_io_msg(T_IO_MSG io_driver_msg_recv)
{
    uint16_t msg_type = io_driver_msg_recv.type;

    switch (msg_type)
    {
    case IO_MSG_TYPE_BT_STATUS:
        {
            peripheral_handle_gap_msg(&io_driver_msg_recv);
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
void periph_handle_dev_state_evt(T_GAP_DEV_STATE new_state, uint16_t cause)
{
    APP_PRINT_INFO4("periph_handle_dev_state_evt: init state %d, adv state %d, conn state %d, cause 0x%x",
                    new_state.gap_init_state, new_state.gap_adv_state,
                    new_state.gap_conn_state, cause);
    if (gap_cur_state.gap_init_state != new_state.gap_init_state)
    {
        if (new_state.gap_init_state == GAP_INIT_STATE_STACK_READY)
        {
            /*stack ready*/
#if (DEBUG_TEST_WDG_APP_ENABLE == 0)
            DBG_DIRECT("ADV START!");
            le_adv_start();
#else
            if (power_settings.power_scenario == POWER_ADV_CONN)
            {
                DBG_DIRECT("ADV START!");
                le_adv_start();

                extern void reset_test_ble_adv(void);
                reset_test_ble_adv();
            }
            if (power_settings.power_scenario == POWER_SCAN)
            {
                DBG_DIRECT("SCAN START!");

                /* connect */
//                uint8_t addr[6] = {0x87,0x56,0x34,0x12,0xA0,0x00};
//                uint8_t addr_len;
//                uint8_t addr_type = GAP_REMOTE_ADDR_LE_PUBLIC;
                T_GAP_LE_CONN_REQ_PARAM conn_req_param;

                conn_req_param.scan_interval = 0x10;
                conn_req_param.scan_window = 0x10;
                conn_req_param.conn_interval_min = 80;     // 1.25ms * N
                conn_req_param.conn_interval_max = 80;
                conn_req_param.conn_latency = 0;
                conn_req_param.supv_tout = 1000;
                conn_req_param.ce_len_min = 2 * (conn_req_param.conn_interval_min - 1);
                conn_req_param.ce_len_max = 2 * (conn_req_param.conn_interval_max - 1);
                le_set_conn_param(GAP_CONN_PARAM_1M, &conn_req_param);
//                le_connect(GAP_PHYS_CONN_INIT_1M_BIT, addr, (T_GAP_REMOTE_ADDR_TYPE)addr_type,
//                       GAP_LOCAL_ADDR_LE_PUBLIC,
//                       1000);

                le_scan_start();

                extern void reset_test_ble_scan(void);
                reset_test_ble_scan();
            }
#endif
        }
    }

    if (gap_cur_state.gap_adv_state != new_state.gap_adv_state)
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
        }
        else if (new_state.gap_adv_state == GAP_ADV_STATE_ADVERTISING)
        {

            APP_PRINT_INFO0("GAP adv start");
        }
    }

    if (gap_cur_state.gap_conn_state != new_state.gap_conn_state)
    {
        APP_PRINT_INFO2("conn state: %d -> %d",
                        gap_cur_state.gap_conn_state,
                        new_state.gap_conn_state);
    }
    gap_cur_state = new_state;
}

void periph_handle_conn_state_evt(uint8_t conn_id, T_GAP_CONN_STATE new_state, uint16_t disc_cause)
{
    APP_PRINT_INFO3("periph_handle_conn_state_evt: conn_id = %d old_state = %d new_state = %d",
                    conn_id, gap_conn_state, new_state);
    switch (new_state)
    {
    case GAP_CONN_STATE_DISCONNECTED:
        {
            if ((disc_cause != (HCI_ERR | HCI_ERR_REMOTE_USER_TERMINATE))
                && (disc_cause != (HCI_ERR | HCI_ERR_LOCAL_HOST_TERMINATE)))
            {
                APP_PRINT_ERROR1("connection lost: cause 0x%x", disc_cause);
            }

            if (switch_to_ota_mode_pending)
            {
#if (SUPPORT_NORMAL_OTA == 1)
                switch_to_ota_mode_pending = false;
                extern void dfu_switch_to_ota_mode(void);
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
                else
                {
                    le_adv_start();
                }
            }

            //le_adv_start();
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

#if (ENABLE_SLAVE_REQUEST_UPDATE_COON_PARA ==1)
            T_GAP_CAUSE update_conn_para_cause = le_update_conn_param(0, CONNECT_INTERVAL_MIN,
                                                                      CONNECT_INTERVAL_MAX, CONNECT_LATENCY,
                                                                      SUPERVISION_TIMEOUT, 2 * (CONNECT_INTERVAL_MIN - 1), 2 * (CONNECT_INTERVAL_MAX - 1));
            if (GAP_CAUSE_SUCCESS != update_conn_para_cause)
            {
                DFU_PRINT_INFO1("le_update_conn_param: send HCI command failed! cause=%d", update_conn_para_cause);
            }
#endif


        }
        break;

    default:
        break;
    }
    gap_conn_state = new_state;
}

/******************************************************************
 * @fn          peripheral_HandleBtGapAuthenStateChangeEvt
 * @brief      All the bonding state change  events are pre-handled in this function.
 *                Then the event handling function shall be called according to the newState.
 *
 * @param    newState  - new bonding state
 * @return     void
 */
void periph_handle_authen_state_evt(uint8_t conn_id, uint8_t new_state, uint16_t cause)
{
    APP_PRINT_INFO1("periph_handle_authen_state_evt:conn_id %d", conn_id);

    switch (new_state)
    {
    case GAP_AUTHEN_STATE_STARTED:
        {
            APP_PRINT_INFO0("GAPSEC_AUTHEN_STATE_STARTED");
        }
        break;

    case GAP_AUTHEN_STATE_COMPLETE:
        {
            APP_PRINT_INFO0("GAPSEC_AUTHEN_STATE_COMPLETE");
            if (cause == 0)
            {
                g_lantency_status = LANTENCY_UPDATING;
                APP_PRINT_INFO0("LE_GAP_MSG_TYPE_AUTHEN_STATE_CHANGE pair success");
            }
            else
            {
                APP_PRINT_INFO0("LE_GAP_MSG_TYPE_AUTHEN_STATE_CHANGE pair failed");
            }
        }
        break;

    default:
        {
            APP_PRINT_INFO1("LE_GAP_MSG_TYPE_AUTHEN_STATE_CHANGE:(unknown newstate: %d)", new_state);
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
void periph_conn_param_update_evt(uint8_t conn_id, uint8_t status, uint16_t cause)
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
            if (conn_slave_latency)
            {
                g_lantency_status = LANTENCY_ON;
            }
            g_dfu_para.dfu_conn_interval = conn_interval;
            g_dfu_para.dfu_conn_lantency = conn_slave_latency;
            if (g_dfu_para.dfu_conn_para_update_in_progress == true)
            {
                g_dfu_para.dfu_conn_para_update_in_progress = false;

                uint8_t notif_data[9] = {0};
                notif_data[0] = DFU_OPCODE_NOTIFICATION;
                notif_data[1] = DFU_OPCODE_CONN_PARA_TO_UPDATE_REQ;
                notif_data[2] = DFU_ARV_SUCCESS;
                LE_UINT16_TO_ARRAY(notif_data + 3, conn_interval);
                LE_UINT16_TO_ARRAY(notif_data + 5, conn_slave_latency);
                LE_UINT16_TO_ARRAY(notif_data + 7, conn_supervision_timeout);
                /* Send connection parameter result to remote device. */
                server_send_data(0, g_dfu_service_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                                 notif_data, DFU_LENGTH_CONN_PARA_TO_UPDATE_REQ, GATT_PDU_TYPE_NOTIFICATION);
            }
            APP_PRINT_INFO3("LE_GAP_MSG_TYPE_CONN_PARA_UPDATE_CHANGE update success:conn_interval 0x%x, conn_slave_latency 0x%x, conn_supervision_timeout 0x%x",
                            conn_interval, conn_slave_latency, conn_supervision_timeout);
        }
        break;

    case GAP_CONN_PARAM_UPDATE_STATUS_FAIL:
        {
            APP_PRINT_ERROR1("LE_GAP_MSG_TYPE_CONN_PARA_UPDATE_CHANGE failed: cause 0x%x", cause);
            uint8_t notif_data[3] = {0};
            notif_data[0] = DFU_OPCODE_NOTIFICATION;
            notif_data[1] = DFU_OPCODE_CONN_PARA_TO_UPDATE_REQ;
            notif_data[2] = DFU_ARV_FAIL_OPERATION;
            /* Connection Param Update rejected, we should notify the fail result to remote device. */
            server_send_data(0, g_dfu_service_id, INDEX_DFU_CONTROL_POINT_CHAR_VALUE, \
                             notif_data, 3, GATT_PDU_TYPE_NOTIFICATION);
        }
        break;

    case GAP_CONN_PARAM_UPDATE_STATUS_PENDING:
        {
            APP_PRINT_INFO0("LE_GAP_MSG_TYPE_CONN_PARA_UPDATE_CHANGE request success.");
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
void periph_handle_conn_mtu_info_evt(uint8_t conn_id, uint16_t mtu_size)
{
    APP_PRINT_INFO2("app_handle_conn_mtu_info_evt: conn_id %d, mtu_size %d", conn_id, mtu_size);
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
void peripheral_handle_gap_msg(T_IO_MSG *p_gap_msg)
{
    T_LE_GAP_MSG gap_msg;
    uint8_t conn_id;
    memcpy(&gap_msg, &p_gap_msg->u.param, sizeof(p_gap_msg->u.param));

    APP_PRINT_TRACE1("app_handle_gap_msg: subtype %d", p_gap_msg->subtype);
    switch (p_gap_msg->subtype)
    {
    case GAP_MSG_LE_DEV_STATE_CHANGE:
        {
            periph_handle_dev_state_evt(gap_msg.msg_data.gap_dev_state_change.new_state,
                                        gap_msg.msg_data.gap_dev_state_change.cause);
        }
        break;

    case GAP_MSG_LE_CONN_STATE_CHANGE:
        {
            periph_handle_conn_state_evt(gap_msg.msg_data.gap_conn_state_change.conn_id,
                                         (T_GAP_CONN_STATE)gap_msg.msg_data.gap_conn_state_change.new_state,
                                         gap_msg.msg_data.gap_conn_state_change.disc_cause);
        }
        break;

    case GAP_MSG_LE_CONN_MTU_INFO:
        {
            periph_handle_conn_mtu_info_evt(gap_msg.msg_data.gap_conn_mtu_info.conn_id,
                                            gap_msg.msg_data.gap_conn_mtu_info.mtu_size);
            g_dfu_para.mtu_size = gap_msg.msg_data.gap_conn_mtu_info.mtu_size;
        }
        break;

    case GAP_MSG_LE_CONN_PARAM_UPDATE:
        {
            periph_conn_param_update_evt(gap_msg.msg_data.gap_conn_param_update.conn_id,
                                         gap_msg.msg_data.gap_conn_param_update.status,
                                         gap_msg.msg_data.gap_conn_param_update.cause);
        }
        break;

    case GAP_MSG_LE_AUTHEN_STATE_CHANGE:
        {
            periph_handle_authen_state_evt(gap_msg.msg_data.gap_authen_state.conn_id,
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

    case GAP_MSG_LE_BOND_MODIFY_INFO:
        APP_PRINT_INFO1("GAP_MSG_LE_BOND_MODIFY_INFO: type 0x%x",
                        p_data->p_le_bond_modify_info->type);
        break;

    case GAP_MSG_LE_MODIFY_WHITE_LIST:
        APP_PRINT_INFO2("GAP_MSG_LE_MODIFY_WHITE_LIST: operation %d, cause 0x%x",
                        p_data->p_le_modify_white_list_rsp->operation,
                        p_data->p_le_modify_white_list_rsp->cause);
        break;

    default:
        APP_PRINT_INFO1("app_gap_callback: unhandled cb_type 0x%x", cb_type);
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
            APP_PRINT_INFO5("PROFILE_EVT_SEND_DATA_COMPLETE: conn_id %d, cause 0x%x, service_id %d, attrib_idx 0x%x, credits = %d",
                            p_param->event_data.send_data_result.conn_id,
                            p_param->event_data.send_data_result.cause,
                            p_param->event_data.send_data_result.service_id,
                            p_param->event_data.send_data_result.attrib_idx,
                            p_param->event_data.send_data_result.credits);
            if (p_param->event_data.send_data_result.cause == GAP_SUCCESS)
            {
                APP_PRINT_INFO0("PROFILE_EVT_SEND_DATA_COMPLETE success");
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
    else if (service_id == g_bas_service_id)
    {
        T_BAS_CALLBACK_DATA *pBasCallbackData = (T_BAS_CALLBACK_DATA *)p_data;
        switch (pBasCallbackData->msg_type)
        {
        case SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION:
            {
                if (pBasCallbackData->msg_data.notification_indification_index == BAS_NOTIFY_BATTERY_LEVEL_ENABLE)
                {
                    APP_PRINT_INFO0("Battery level notification enable");
                }
                else if (pBasCallbackData->msg_data.notification_indification_index ==
                         BAS_NOTIFY_BATTERY_LEVEL_DISABLE)
                {
                    APP_PRINT_INFO0("Battery level notification disable");
                }
            }
            break;
        case SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE:
            {
                /* update RCU battery level */
                g_bat_vol = 150;
                g_bas_battery_level = 100;

                APP_PRINT_INFO2("Battery_Level_Update BatVol is %d , BASBatteryLevel is %d", g_bat_vol,
                                g_bas_battery_level);

                bas_set_parameter(BAS_PARAM_BATTERY_LEVEL, 1, (uint8_t *)&g_bas_battery_level);
            }
            break;
        default:
            break;
        }
    }
    else if (service_id == g_dis_service_id)
    {
        T_DIS_CALLBACK_DATA *pDisCallbackData = (T_DIS_CALLBACK_DATA *)p_data;
        switch (pDisCallbackData->msg_type)
        {
        case SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE:
            {
                if (pDisCallbackData->msg_data.read_value_index == DIS_READ_MANU_NAME_INDEX)
                {
                    const uint8_t DISManufacturerName[] = "Realtek BT";
                    dis_set_parameter(DIS_PARAM_MANUFACTURER_NAME,
                                      sizeof(DISManufacturerName),
                                      (void *)DISManufacturerName);
                }
                else if (pDisCallbackData->msg_data.read_value_index == DIS_READ_MODEL_NUM_INDEX)
                {
                    const uint8_t DISModelNumber[] = "Model Nbr 0.9";
                    dis_set_parameter(DIS_PARAM_MODEL_NUMBER,
                                      sizeof(DISModelNumber),
                                      (void *)DISModelNumber);
                }
                else if (pDisCallbackData->msg_data.read_value_index == DIS_READ_SERIAL_NUM_INDEX)
                {
                    const uint8_t DISSerialNumber[] = "RTKBeeSerialNum";
                    dis_set_parameter(DIS_PARAM_SERIAL_NUMBER,
                                      sizeof(DISSerialNumber),
                                      (void *)DISSerialNumber);
                }
                else if (pDisCallbackData->msg_data.read_value_index == DIS_READ_HARDWARE_REV_INDEX)
                {
                    const uint8_t DISHardwareRev[] = "RTKBeeHardwareRev";
                    dis_set_parameter(DIS_PARAM_HARDWARE_REVISION,
                                      sizeof(DISHardwareRev),
                                      (void *)DISHardwareRev);
                }
                else if (pDisCallbackData->msg_data.read_value_index == DIS_READ_FIRMWARE_REV_INDEX)
                {
                    const uint8_t DISFirmwareRev[] = "RTKBeeFirmwareRev";
                    dis_set_parameter(DIS_PARAM_FIRMWARE_REVISION,
                                      sizeof(DISFirmwareRev),
                                      (void *)DISFirmwareRev);
                }
                else if (pDisCallbackData->msg_data.read_value_index == DIS_READ_SOFTWARE_REV_INDEX)
                {
                    const uint8_t DISSoftwareRev[] = "RTKBeeSoftwareRev";
                    dis_set_parameter(DIS_PARAM_SOFTWARE_REVISION,
                                      sizeof(DISSoftwareRev),
                                      (void *)DISSoftwareRev);
                }
                else if (pDisCallbackData->msg_data.read_value_index == DIS_READ_SYSTEM_ID_INDEX)
                {
                    const uint8_t DISSystemID[DIS_SYSTEM_ID_LENGTH] = {0, 1, 2, 0, 0, 3, 4, 5};
                    dis_set_parameter(DIS_PARAM_SYSTEM_ID,
                                      sizeof(DISSystemID),
                                      (void *)DISSystemID);
                }
                else if (pDisCallbackData->msg_data.read_value_index == DIS_READ_IEEE_CERT_STR_INDEX)
                {
                    const uint8_t DISIEEEDataList[] = "RTKBeeIEEEDatalist";
                    dis_set_parameter(DIS_PARAM_IEEE_DATA_LIST,
                                      sizeof(DISIEEEDataList),
                                      (void *)DISIEEEDataList);
                }
                else if (pDisCallbackData->msg_data.read_value_index == DIS_READ_PNP_ID_INDEX)
                {
                    //uint8_t DISPnpID[DIS_PNP_ID_LENGTH] = {0x01,0xDA,0x0B,0x62,0x87,0x01,0x00};
                    uint16_t version = 0x03; //VERSION_BUILD;

                    uint8_t DISPnpID[DIS_PNP_ID_LENGTH] = {0x01, 0x5D, 0x00, 0x01, 0x00, (uint8_t)version, (uint8_t)(version >> 8)}; //VID_005D&PID_0001?

                    dis_set_parameter(DIS_PARAM_PNP_ID,
                                      sizeof(DISPnpID),
                                      DISPnpID);
                }
            }
            break;
        default:
            break;
        }
    }
    else if (service_id == g_dfu_service_id)
    {
        T_DFU_CALLBACK_DATA *p_dfu_cb_data = (T_DFU_CALLBACK_DATA *)p_data;
        switch (p_dfu_cb_data->msg_type)
        {
        case SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION:
            {
                if (p_dfu_cb_data->msg_data.notification_indification_index == DFU_NOTIFY_ENABLE)
                {
                    APP_PRINT_INFO0("dfu notification enable");
                }
                else if (p_dfu_cb_data->msg_data.notification_indification_index ==
                         DFU_NOTIFY_DISABLE)
                {
                    APP_PRINT_INFO0("dfu notification disable");
                }
            }
            break;
        case SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE:
            {
                uint8_t dfu_write_opcode = p_dfu_cb_data->msg_data.write.opcode;
                if (dfu_write_opcode == DFU_WRITE_ATTR_EXIT)
                {
                    if (p_dfu_cb_data->msg_data.write.write_attrib_index == INDEX_DFU_CONTROL_POINT_CHAR_VALUE)
                    {
                        uint8_t control_point_opcode = *p_dfu_cb_data->msg_data.write.p_value;
                        switch (control_point_opcode)
                        {
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
#endif
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
                break;
            default:
                break;

            }
        }

        else if (service_id == g_ota_service_id)
        {

            T_OTA_CALLBACK_DATA *pOTACallbackData = (T_OTA_CALLBACK_DATA *)p_data;
            switch (pOTACallbackData->msg_type)
            {
            case SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE:

                if (OTA_WRITE_CHAR_VAL == pOTACallbackData->msg_data.write.opcode &&
                    OTA_VALUE_ENTER == pOTACallbackData->msg_data.write.u.value)
                {

                    /*battery level is above 60 percent*/
                    APP_PRINT_INFO0("Preparing switch into OTA mode");
                    /*prepare to enter OTA mode, before switch action, we should disconnect first.*/
                    switch_to_ota_mode_pending = true;
                    le_disconnect(0);
                }
                else if (OTA_WRITE_IMAGE_COUNT_VAL == pOTACallbackData->msg_data.write.opcode)
                {

                }

                break;

            default:

                break;
            }
        }
        return app_result;
    }


