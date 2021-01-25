/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-28     Supperthomas the first version
 */


#include "bsal.h"
#include <stdio.h>
#include <string.h>
#include "bsal_osif.h"
#include "bsal_srv_blufi.h"
#include "bsal_srv_bas.h"
#include "gap.h"
#include <cJSON.h>


#define BSAL_STACK_NAME PKG_BSAL_STACK_NAME

static void *bsal_stack_ptr = NULL;
uint16_t bsal_app_conn_handle;
static uint8_t battery_flag = 0;
static uint8_t  gap_conn_state = BSAL_GAP_CONN_STATE_DISCONNECTED; /**< GAP connection state */

RT_WEAK void ble_info_get_callback(char *dev_name)
{
    rt_kprintf("ble_name:%s\n", dev_name);
    ui_ble_mac_update(dev_name);
}

void bsa_app_set_adv_data(void *stack_ptr)
{
    uint8_t tmp_data[32] = {0} ; //must be zero
    char tmp_name[20] = {0};
    bsal_le_adv_data_add_flag(tmp_data, BSAL_GAP_ADTYPE_FLAGS_LIMITED | BSAL_GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED);

    char *adv_name = (char *)bsal_get_device_name(stack_ptr);
    
    //bsal_adv_data_add_uuid16(tmp_data, BSAL_GATT_SERVICE_BATTERY_SERVICE);
    uint8_t bt_addr[6];
    gap_get_param(GAP_PARAM_BD_ADDR, bt_addr);
    sprintf(tmp_name,"%s_%02x%02x","ART-Badge",bt_addr[1],bt_addr[0]);
    ble_info_get_callback(tmp_name);
    bsal_adv_data_add_name(tmp_data, strlen(tmp_name), tmp_name);
    bsal_set_le_adv_data_user(stack_ptr, tmp_data);
}

void bsal_app_all_callback(void *stack_ptr, uint8_t cb_layer, uint16_t cb_sub_event, uint8_t value_length , void *value)
{
    T_BSAL_GAP_MSG_DATA  *bsal_gap_msg_data = (T_BSAL_GAP_MSG_DATA *)value;
    uint8_t bd_addr[6];
    switch (cb_layer)
    {
    case BSAL_CB_LAYER_GAP:
        switch (cb_sub_event)
        {
        case BSAL_CB_STACK_READY:
            //get mac address

            bsal_osif_printf_info("============stack ready===========\r\n");
            bsa_app_set_adv_data(stack_ptr);
            bsal_stack_start_adv(stack_ptr);
            break;
        case BSAL_CB_CONNECT_STATUS:
            bsal_osif_printf_info("============stack connect id %d===========\r\n", bsal_gap_msg_data->gap_conn_state_change.conn_id);
            if (bsal_gap_msg_data->gap_conn_state_change.new_state == BSAL_GAP_CONN_STATE_CONNECTED)
            {
                bsal_app_conn_handle = bsal_gap_msg_data->gap_conn_state_change.conn_id;
            }
            else if (bsal_gap_msg_data->gap_conn_state_change.new_state == BSAL_GAP_CONN_STATE_DISCONNECTED)
            {
                bsal_stack_start_adv(stack_ptr);
                battery_flag = 0;
            }
            bsal_osif_printf_info("BSAL: conn_id %d old_state %d new_state %d, disc_cause 0x%x",
                                  bsal_gap_msg_data->gap_conn_state_change.conn_id , gap_conn_state, bsal_gap_msg_data->gap_conn_state_change.new_state, bsal_gap_msg_data->gap_conn_state_change.disc_cause);

            break;
        default:
            break;
        }

        if (cb_sub_event == BSAL_CB_STACK_READY)
        {
            //stack ready
            //   APP_PRINT_INFO0("GAP stack ready");

        }

        break;
    case BSAL_CB_LAYER_GATT_PROFILE:
        switch (cb_sub_event)
        {
            //save the service start_handle
            //case uuid profile save start_handle
            //case SRV_CALLBACK66
            //save the identity
        }
        break;
    case BSAL_CB_LAYER_SM:
        break;
    case BSAL_CB_LAYER_COMMON:
        //connected save the connect id

        break;
    case BSAL_CB_LAYER_UNKNOWN:
        break;
    default:
        break;
    }

}
//#include <stdio.h>
rt_err_t rtc_set_time(time_t timestamp)
{
    /* converts the local time in time to calendar time. */
    rt_device_t rtc_device;
    rt_err_t ret = -RT_ERROR;

    rtc_device = rt_device_find("rtc");

    if (rtc_device == RT_NULL)
    {
        return -RT_ERROR;
    }

    /* update to RTC device. */
    ret = rt_device_control(rtc_device, RT_DEVICE_CTRL_RTC_SET_TIME, &timestamp);

    return ret;
}

RT_WEAK void card_info_get_callback(char *name, char* duty, double timestamp)
{
    rt_kprintf("name:%s\n", name);
    rt_kprintf("duty:%s\n", duty);
    ui_name_update(name);
    ui_post_update(duty);
    rtc_set_time((time_t)(timestamp/1000) + 8 * 3600);
}

void bsal_app_profile_callback(void *p)
{
    bsal_callbak_data_t *bsal_param = (bsal_callbak_data_t *)p;

    if (bsal_param->msg_type == BSAL_CALLBACK_TYPE_READ_CHAR_VALUE)
    {
        bsal_osif_printf_info("========callback read from %x====%x=======\r\n", bsal_param->off_handle, bsal_param->srv_uuid.u16.value);
    }
    else if (bsal_param->msg_type == BSAL_CALLBACK_TYPE_INDIFICATION_NOTIFICATION)
    {
        uint16_t  cccbits = bsal_param->value;
        bsal_osif_printf_info("======callback notify from %x===data cccd %x====%x=====\r\n", bsal_param->off_handle, cccbits, bsal_param->srv_uuid.u16.value);
        if (bsal_param->srv_uuid.u16.value == BSAL_GATT_SERVICE_BATTERY_SERVICE)
        {
            if (cccbits & BSAL_GATT_CCC_NOTIFY)
            {
                bsal_osif_printf_info("=========NOTIFY ENABLE from %x===data cccd %x====%x=====\r\n", bsal_param->off_handle, cccbits, bsal_param->srv_uuid.u16.value);
                battery_flag = 1;
            }
            else
            {
                bsal_osif_printf_info("========NOTIFY DISABLE from %x===data cccd %x====%x=====\r\n", bsal_param->off_handle, cccbits, bsal_param->srv_uuid.u16.value);
                battery_flag = 0;
            }
        }
        else if (bsal_param->srv_uuid.u16.value == GATT_UUID_BLUFI_PROFILE)
        {
            if (cccbits & BSAL_GATT_CCC_NOTIFY)
            {
                bsal_osif_printf_info("=========NOTIFY ENABLE from %x===data cccd %x====%x=====\r\n", bsal_param->off_handle, cccbits, bsal_param->srv_uuid.u16.value);
            }
            else
            {
                bsal_osif_printf_info("========NOTIFY DISABLE from %x===data cccd %x====%x=====\r\n", bsal_param->off_handle, cccbits, bsal_param->srv_uuid.u16.value);
            }
        }
    }
    else if (bsal_param->msg_type == BSAL_CALLBACK_TYPE_WRITE_CHAR_VALUE)
    {
        cJSON *root = NULL;
        cJSON *name_item = NULL, *duty_item = NULL, *time_item = NULL;

        bsal_osif_printf_info("\r\n BSAL: THE DATA IS :%s\r\n", bsal_param->data);
        root = cJSON_Parse(bsal_param->data); 
        if (!root) 
        {
            rt_kprintf("Error before: [%s]\n",cJSON_GetErrorPtr());
        }
        else
        {
            name_item = cJSON_GetObjectItem(root, "name");
            duty_item = cJSON_GetObjectItem(root, "duty");
            time_item = cJSON_GetObjectItem(root, "time");
            card_info_get_callback(name_item->valuestring, duty_item->valuestring, time_item->valuedouble);
        }
        cJSON_Delete(root);
    }
}

//add the db for


void bsal_ble_loop(void *p_param)
{

    uint8_t count = 0;
    battery_flag = 1;
    while (1)
    {
        bsal_osif_delay(20000);
        bsal_osif_printf_info("====hello world===%d=\r\n", battery_flag);
        if (battery_flag == 1)
        {
            uint8_t battery_level = 90 - count * 10;
            count++;
            bsal_bas_send_notify_level(bsal_stack_ptr, bsal_app_conn_handle, battery_level);
        }
        bsal_osif_delay(2000);
    }
}


int bsal_bas_blufi_app(void)
{
    void *stack_ptr = bsal_find_stack_ptr(BSAL_STACK_NAME);
    if (stack_ptr == NULL)
    {
        //print error;
        return 1;
    }
    //set iocapability


    bsal_stack_ptr  = stack_ptr;
    //1. init stack
    bsal_stack_init(stack_ptr, bsal_app_all_callback);  // init param not start stack
    // set device name
    char device_name[20];
    uint8_t bt_addr[6];
    gap_get_param(GAP_PARAM_BD_ADDR, bt_addr);
    sprintf(device_name,"%s_%02x%02x","ART-Badge",bt_addr[1],bt_addr[0]);
    bsal_set_device_name(stack_ptr, strlen(device_name), (uint8_t *)device_name);
    //2. bond type
    bsal_set_device_le_bond_type(stack_ptr, false, BSAL_NO_INPUT, BSAL_NO_OUTPUT, BSAL_GAP_AUTHEN_BIT_NO_BONDING, false);
    //set the bond flag:

    //3. service begin
    bsal_stack_le_srv_begin(stack_ptr, 2, bsal_app_profile_callback);  //will add 1 service
    //4. bas_init
    bsal_le_bas_svr_init(stack_ptr, bsal_app_profile_callback); //add battery servcie

    //5. blufi_init
    bsal_le_blufi_svr_init(stack_ptr, bsal_app_profile_callback);

    //6. srv_end
    bsal_stack_le_srv_end(stack_ptr);    //end srv add

    //start stack
    bsal_stack_startup(stack_ptr);    //start she

    return 0;
}
MSH_CMD_EXPORT_ALIAS(bsal_bas_blufi_app, bsal_bas_blufi_app, "bluetoooth blufi and bas sample");

//CMD


