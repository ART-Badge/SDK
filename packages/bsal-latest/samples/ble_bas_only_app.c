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
#include "bsal_srv_bas.h"
#include <rtdbg.h>
#define DBG_TAG     "BSAL"
#define DBG_LVL     DBG_LOG

#define BSAL_STACK_NAME PKG_BSAL_STACK_NAME

static void *bsal_stack_ptr = NULL;
uint16_t bsal_app_conn_handle;
static uint8_t battery_flag = 0;
static uint8_t  gap_conn_state = BSAL_GAP_CONN_STATE_DISCONNECTED; /**< GAP connection state */

uint16_t battery_start_handle = 0;
void bsa_app_set_adv_data(void *stack_ptr)
{
    uint8_t tmp_data[32] = {0} ; //must be zero
    bsal_le_adv_data_add_flag(tmp_data, BSAL_GAP_ADTYPE_FLAGS_LIMITED | BSAL_GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED);

    char *adv_name = (char *)bsal_get_device_name(stack_ptr);
    bsal_adv_data_add_name(tmp_data, strlen(adv_name), adv_name);
    //bsal_adv_data_add_uuid16(tmp_data, BSAL_GATT_SERVICE_BATTERY_SERVICE);
    bsal_set_le_adv_data_user(stack_ptr, tmp_data);
}

void bsal_app_all_callback(void *stack_ptr, uint8_t cb_layer, uint16_t cb_sub_event, uint8_t value_length , void *value)
{
    T_BSAL_GAP_MSG_DATA  *bsal_gap_msg_data = (T_BSAL_GAP_MSG_DATA *)value;
    switch (cb_layer)
    {
    case BSAL_CB_LAYER_GAP:
        switch (cb_sub_event)
        {
        case BSAL_CB_STACK_READY:
            LOG_I("BSAL STACK READY");

            bsa_app_set_adv_data(stack_ptr);

            bsal_stack_start_adv(stack_ptr);
            break;
        case BSAL_CB_CONNECT_STATUS:
            if (bsal_gap_msg_data->gap_conn_state_change.new_state == BSAL_GAP_CONN_STATE_CONNECTED)
            {
                LOG_I("REMOTE DEVICE CONNECT ID %d STATUS CONNECTED READY\r\n", bsal_gap_msg_data->gap_conn_state_change.conn_id);
                bsal_app_conn_handle = bsal_gap_msg_data->gap_conn_state_change.conn_id;
            }
            else if (bsal_gap_msg_data->gap_conn_state_change.new_state == BSAL_GAP_CONN_STATE_DISCONNECTED)
            {
                LOG_I("REMOTE DEVICE CONNECT ID %d STATUS DISCONNECT READY\r\n", bsal_gap_msg_data->gap_conn_state_change.conn_id);

                bsal_stack_start_adv(stack_ptr);
                battery_flag = 0;
            }
            bsal_osif_printf_info("conn_id %d old_state %d new_state %d, disc_cause 0x%x",
                                  bsal_gap_msg_data->gap_conn_state_change.conn_id , gap_conn_state, bsal_gap_msg_data->gap_conn_state_change.new_state, bsal_gap_msg_data->gap_conn_state_change.disc_cause);

            break;
        default:
            break;
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




void bsal_app_profile_callback(void *p)
{
    bsal_callbak_data_t *bsal_param = (bsal_callbak_data_t *)p;

    if (bsal_param->msg_type == BSAL_CALLBACK_TYPE_READ_CHAR_VALUE)
    {
         LOG_I("GATT:READ CHAR VALUE OFF_HANDLE %x  UUID:%x\r\n", bsal_param->off_handle, bsal_param->srv_uuid.u16.value);
    }
    else if (bsal_param->msg_type == BSAL_CALLBACK_TYPE_INDIFICATION_NOTIFICATION)
    {
        uint16_t  cccbits = (uint16_t)bsal_param->value;
        LOG_I("GATT: NOTIFY HANDLE %x cccd %x UUID:%x\r\n", bsal_param->off_handle, cccbits, bsal_param->srv_uuid.u16.value);

        if (bsal_param->srv_uuid.u16.value == BSAL_GATT_SERVICE_BATTERY_SERVICE)
        {
            if (cccbits & BSAL_GATT_CCC_NOTIFY)
            {
							 LOG_I("NOTIFY ENABLE from %x data cccd %x  UUID:%x\r\n", bsal_param->off_handle, cccbits, bsal_param->srv_uuid.u16.value);
                battery_flag = 1;
            }
            else
            {
                LOG_I("NOTIFY DISABLE from %x data cccd %x UUID%x\r\n", bsal_param->off_handle, cccbits, bsal_param->srv_uuid.u16.value);
                battery_flag = 0;
            }
        }
    }
    else if (bsal_param->msg_type == BSAL_CALLBACK_TYPE_WRITE_CHAR_VALUE)
    {
        LOG_I("\r\n Recv THE DATA IS :%s\r\n", bsal_param->data);
    }
}

//add the db for


void bsal_ble_loop(void *p_param)
{

    uint8_t count = 0;
    while (1)
    {
        bsal_osif_delay(2000);
        if (battery_flag == 1)
        {
            uint8_t battery_level = 90 - count * 10;
            count++;
            if (battery_level <= 0)
            {
                count = 0;
            }
            bsal_bas_send_notify_level(bsal_stack_ptr, bsal_app_conn_handle, battery_level);
        }
    }
}


int bsal_bas_app(void)
{
    void *stack_ptr = bsal_find_stack_ptr(BSAL_STACK_NAME);
    void *bsal_test_app_task;
    if (stack_ptr == NULL)
    {
        //print error;
        return 1;
    }
    //set iocapability


    bsal_stack_ptr  = stack_ptr;

    //1. stack init
    bsal_stack_init(stack_ptr, bsal_app_all_callback);  // init param not start stack
    char *device_name = "ble_rtt";
    bsal_set_device_name(stack_ptr, strlen(device_name), (uint8_t *)device_name);
    //2. set bond type
    bsal_set_device_le_bond_type(stack_ptr, false, BSAL_NO_INPUT, BSAL_NO_OUTPUT, BSAL_GAP_AUTHEN_BIT_NO_BONDING, false);
    //set the bond flag:
    //3. start add profile service
    bsal_stack_le_srv_begin(stack_ptr, 1, bsal_app_profile_callback); //will add 1 service

    //4. add profile service
    bsal_le_bas_svr_init(stack_ptr, bsal_app_profile_callback); //add battery servcie

    //5. end add profile service
    bsal_stack_le_srv_end(stack_ptr);    //end srv add

    //6. start ble stack
    bsal_stack_startup(stack_ptr);    //start she

    //bsal_ble_loop(NULL);

    bsal_test_app_task =  rt_thread_create("bsal_app", bsal_ble_loop, NULL, 2 * 256, 5, 10);
    if (bsal_test_app_task != RT_NULL)
    {

        rt_thread_startup(bsal_test_app_task);
    }
    return 0;
}
MSH_CMD_EXPORT_ALIAS(bsal_bas_app, bsal_bas_app, "bluetoooth battery service sample");




//CMD

