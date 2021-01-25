enum { __FILE_NUM__ = 0 };

/*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     profile_init.c
* @brief
* @details
* @author
* @date     2017-12-27
* @version  v0.1
*********************************************************************************************************
*/

#include "rtl876x.h"
#include "wristband_private_service.h"
#include "string.h"
#include "trace.h"
#include "gap.h"
#include "gap_adv.h"
#include "ftl.h"
#include "os_timer.h"
#include "app_task.h"
#include "app_flash_config.h"
#include "wristband_gap.h"
#include "wristband_global_data.h"
#if APP_PRIVACY_EN
#include <privacy_mgnt.h>
#include "wristband_application.h"
#endif

void *con_parameter_update_timer = NULL;
void *ancs_rediscovery_timer = NULL;
//extern T_GAP_DEV_STATE gap_dev_state;
extern uint8_t adv_data[ADVERTISING_PACKET_LENGTH];
uint8_t con_update_count = 0;

extern uint8_t bbpro_bd_addr[];
extern bool bbpro_bd_addr_got_flag;




void bee2_magic_adv(void)
{
    uint8_t magic_adv_data[21] =
    {
        0x14, 0xFF, 0x4C, 0x00, 0x07, 0x0F, 0x00, 0x02, 0x20, 0x00, 0xE0, 0x4C, 0x87, 0x01, 0x24,
        0xF5, 0xD5, 0x55, 0x5A, 0x01, 0x00
    };
    uint8_t  adv_evt_type = GAP_ADTYPE_ADV_NONCONN_IND;
    uint16_t adv_int_min = 0x50;
    uint16_t adv_int_max = 0x50;
    if ((bbpro_bd_addr_got_flag == true) &&
        (RtkWristbandSys.gap_conn_state == GAP_CONN_STATE_CONNECTED))
    {
        for (int i = 0; i < 6; i++)
        {
            magic_adv_data[9 + i] = bbpro_bd_addr[5 - i];
        }
        le_adv_set_param(GAP_PARAM_ADV_EVENT_TYPE, sizeof(adv_evt_type), &adv_evt_type);
        le_adv_set_param(GAP_PARAM_ADV_DATA, sizeof(magic_adv_data), magic_adv_data);
        le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MIN, sizeof(adv_int_min), &adv_int_min);
        le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MAX, sizeof(adv_int_max), &adv_int_max);
        le_adv_start();
    }
}

#if APP_PRIVACY_EN
extern T_PRIVACY_STATE app_privacy_state;
extern T_PRIVACY_ADDR_RESOLUTION_STATE app_privacy_resolution_state;
extern T_APP_WORK_MODE app_work_mode;
#endif
void wristband_stop_adv(void)
{
    le_adv_stop();
}
void wristband_start_adv(void)
{
    uint8_t advdata_len = 29;
    uint8_t addr_offset = 3 + adv_data[3] + 1 + 8;
    uint8_t addr[6];
    uint8_t  adv_evt_type = GAP_ADTYPE_ADV_IND;
    uint8_t  adv_filter_policy = GAP_ADV_FILTER_ANY;
    uint16_t adv_int_min = 0x640;
    uint16_t adv_int_max = 0x640;
#if APP_PRIVACY_EN
    T_LE_KEY_ENTRY *p_entry;
    p_entry = le_get_high_priority_bond();

    if (p_entry == NULL)
    {
        /* No bonded device, send connectable undirected advertisement without using whitelist*/
        app_work_mode = APP_PAIRABLE_MODE;
        adv_filter_policy = GAP_ADV_FILTER_ANY;
        if (app_privacy_resolution_state == PRIVACY_ADDR_RESOLUTION_ENABLED)
        {
            privacy_set_addr_resolution(false);
        }
    }
//    else
//    {
//        app_work_mode = APP_RECONNECTION_MODE;
//        adv_filter_policy = GAP_ADV_FILTER_WHITE_LIST_ALL;
//        if (app_privacy_resolution_state == PRIVACY_ADDR_RESOLUTION_DISABLED)
//        {
//            privacy_set_addr_resolution(true);
//        }
//    }
#endif
    gap_get_param(GAP_PARAM_BD_ADDR, addr);

    for (int i = 0; i < 6; i++)
    {
        adv_data[addr_offset + i] = addr[5 - i];
    }
    advdata_len = addr_offset + 6;

    le_adv_set_param(GAP_PARAM_ADV_DATA, advdata_len, adv_data);
    le_adv_set_param(GAP_PARAM_ADV_EVENT_TYPE, sizeof(adv_evt_type), &adv_evt_type);
    le_adv_set_param(GAP_PARAM_ADV_FILTER_POLICY, sizeof(adv_filter_policy), &adv_filter_policy);
    le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MIN, sizeof(adv_int_min), &adv_int_min);
    le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MAX, sizeof(adv_int_max), &adv_int_max);

    if (RtkWristbandSys.flag_field.power_off_flag == false &&
        RtkWristbandSys.flag_field.low_power_flag == false)
    {
        le_adv_start();
    }
}

/**
* @brief
*
*
* @param   void
* @return  the length of advert data
*/
uint8_t wristband_adv_data_init(uint8_t *device_name)
{
    uint8_t advertDaraForAirSync[0x3 + 1 + 0x9 + 1] =
    {
        /* Service */
        0x03,           /* length     */
        0x03,           /* type="More 16-bit UUIDs available, service uuid 0xFEE7 0xA00A" */
        0xE7,
        0xFE,
        /* Manufacture specified data*/
        0x09,           /* length     */
        0xFF,           /* type: manufacture specific data*/
        0xC5, 0xFE,     /* company id */
        0, 0, 0, 0, 0, 0, /* mac address*/
    };
    uint8_t addr[6];
    gap_get_param(GAP_PARAM_BD_ADDR, addr);

    for (int i = 0; i < 6; i++)
    {
        advertDaraForAirSync[8 + i] = addr[5 - i];
    }
    memcpy(adv_data + 3, device_name, device_name[0] + 1);
    memcpy(adv_data + 3 + device_name[0] + 1, advertDaraForAirSync, sizeof(advertDaraForAirSync));

    return (3 + device_name[0] + 1 + sizeof(advertDaraForAirSync));
}

void wristband_gap_dev_change_cb(T_GAP_DEV_STATE cb_dev_state, T_GAP_CONN_STATE cb_con_state)
{
    APP_PRINT_INFO1("[WRISTBAND GAP] [change cb] gap_adv_state = %d", cb_dev_state.gap_adv_state);
    if (cb_dev_state.gap_adv_state == GAP_ADV_STATE_IDLE)
    {
        APP_PRINT_INFO0("[WRISTBAND GAP] [change cb] -->GAP_ADV_STATE_IDLE");
        if (cb_con_state == GAP_CONN_STATE_CONNECTED)
        {

        }
        else if (cb_con_state == GAP_CONN_STATE_DISCONNECTED)
        {

        }
    }
    else if (cb_dev_state.gap_adv_state == GAP_ADV_STATE_START)
    {
        APP_PRINT_INFO0("[WRISTBAND GAP] [change cb] -->GAP_ADV_STATE_START");
    }
    else if (cb_dev_state.gap_adv_state == GAP_ADV_STATE_ADVERTISING)
    {
        APP_PRINT_INFO0("[WRISTBAND GAP] [change cb] -->GAP_ADV_STATE_ADVERTISING");
    }
    else if (cb_dev_state.gap_adv_state == GAP_ADV_STATE_STOP)
    {
        APP_PRINT_INFO0("[WRISTBAND GAP] [change cb] -->GAP_ADV_STATE_STOP");
    }
}

void wristband_gap_con_change_cb(T_GAP_CONN_STATE cb_con_state, T_GAP_DEV_STATE cb_dev_state)
{
    if (cb_con_state == GAP_CONN_STATE_DISCONNECTED)
    {
        if (cb_dev_state.gap_adv_state == GAP_ADV_STATE_IDLE)
        {
            wristband_start_adv();
        }
        else if (cb_dev_state.gap_adv_state == GAP_ADV_STATE_ADVERTISING)
        {
            wristband_stop_adv();
        }
    }
    else if (cb_con_state == GAP_CONN_STATE_CONNECTED)
    {

    }
}

void con_parameter_update_callback(void *pxTimer)
{
    //con_update_count == 0 means not update before, ==> reconnect case
    if (con_update_count == 0)
    {
        T_IO_MSG con_para_msg;
        con_para_msg.type = IO_MSG_TYPE_WRISTBNAD;
        con_para_msg.subtype = IO_MSG_UPDATE_CONPARA;
        app_send_msg_to_apptask(&con_para_msg);
        start_connection_paramter_timer();//start timer again to check if we get the desired parameter.
    }
    else
    {
        //update before, but the parameter is not desired. ==>ERROR
        if ((RtkWristbandSys.conn_latency + 1) * RtkWristbandSys.conn_interval < 100)
        {
            APP_PRINT_INFO0("connection parameter update error!");
        }
    }

}

void ancs_rediscovery_callback(void *pxTimer)
{
    APP_PRINT_INFO0("do ANCS DISCOVERY for service change!");
    T_IO_MSG ancs_msg;
    ancs_msg.type = IO_MSG_TYPE_WRISTBNAD;
    ancs_msg.subtype = IO_MSG_ANCS_DISCOVERY;
    app_send_msg_to_apptask(&ancs_msg);

}

void wristband_update_con_para(bool use_latency)
{
    if (RtkWristbandSys.gap_conn_state != GAP_CONN_STATE_CONNECTED)
    {
        return;
    }

    APP_PRINT_INFO1("wristband_update_con_para, con_update_count = %d", con_update_count);
    if ((RtkWristbandSys.conn_latency + 1) * RtkWristbandSys.conn_interval < 100)
    {
        uint16_t desired_interval_min;
        uint16_t desired_interval_max;
        uint16_t desired_latency;
        uint16_t desired_superv_tout;

        if (use_latency)
        {
            desired_interval_min = 0x18;
            desired_interval_max = 0x24;
            desired_latency = 9;
            desired_superv_tout = 5000;
        }
        else
        {
            desired_interval_min = 240;
            desired_interval_max = 256;
            desired_latency = 0;
            desired_superv_tout = 5000;
        }
        desired_interval_min = 80;
        desired_interval_max = 80;
        desired_latency = 0;
        desired_superv_tout = 5000;

        APP_PRINT_INFO1("send connection parameter update request, use_latency = %d!", use_latency);
        le_update_conn_param(RtkWristbandSys.wristband_conn_id, desired_interval_min, desired_interval_max,
                             desired_latency,
                             desired_superv_tout / 10, desired_interval_min * 2 - 2, desired_interval_max * 2 - 2);
        con_update_count++;
    }

}

void wristband_gap_timer_init(void)
{
    os_timer_create(&con_parameter_update_timer, "con paramter update timer", 1, 20000, false,
                    con_parameter_update_callback);
    if (con_parameter_update_timer == NULL)
    {
        APP_PRINT_ERROR0("con paramter update timer create fail");
    }
    os_timer_create(&ancs_rediscovery_timer, "con paramter update timer", 1, 5000, false,
                    ancs_rediscovery_callback);
    if (ancs_rediscovery_timer == NULL)
    {
        APP_PRINT_ERROR0("ancs rediscovery timer create fail");
    }
}

void start_connection_paramter_timer(void)
{
    if (con_parameter_update_timer != NULL)
    {
        con_update_count = 0;
        os_timer_restart(&con_parameter_update_timer, 5000);
    }
}

void start_ancs_rediscovery_timer(void)
{
    if (ancs_rediscovery_timer != NULL)
    {
        os_timer_restart(&ancs_rediscovery_timer, 20000);
    }
}

void stop_connection_paramter_timer(void)
{
    if (con_parameter_update_timer != NULL)
    {
        con_update_count = 0;
        os_timer_stop(&con_parameter_update_timer);
    }
}
/**
 * @brief get public bd address of peer device
 * @param conn_id - connection index
 * @param p_bd_addr - point of result bd address
 * @return none
 */
bool wristband_get_peer_public_bd_addr(uint8_t conn_id, uint8_t *p_bd_addr)
{
    bool result = false;

    T_LE_KEY_ENTRY *p_entry;
    p_entry = le_find_key_entry_by_idx(conn_id);

    if (p_entry != NULL && p_entry->is_used)
    {
        if (p_entry->flags & LE_KEY_STORE_REMOTE_IRK_BIT)
        {
            APP_PRINT_INFO2("privacy_init_resolving_list: add BD=%s type=%d",
                            TRACE_BDADDR(p_entry->resolved_remote_bd.addr),
                            p_entry->resolved_remote_bd.remote_bd_type);

            if (p_entry->resolved_remote_bd.remote_bd_type == GAP_IDENT_ADDR_PUBLIC)
            {
                memcpy(p_bd_addr, p_entry->resolved_remote_bd.addr, 6);
                result = true;
            }
        }
        else
        {
            uint8_t remote_bd[6];
            uint8_t remote_bd_type;
            le_get_conn_addr(conn_id, remote_bd, &remote_bd_type);
            if (remote_bd_type == GAP_REMOTE_ADDR_LE_PUBLIC)
            {
                memcpy(p_bd_addr, remote_bd, 6);
                result = true;
            }
        }
    }

    return result;
}

