/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-28     Supperthomas the first version
 */

#include <stdio.h>
#include <stdint.h>
#include "bsal.h"
#include "bsal_osif.h"
#include "bsal_srv_bas.h"


static uint8_t bsal_bas_battery_level = 50;

static P_SRV_GENERAL_CB pfn_bas_cb = NULL;
static void profile_callback(void *p)
{
    bsal_callbak_data_t *p_param = (bsal_callbak_data_t *)p;
    bool is_app_cb = false;
    if (p_param->msg_type == BSAL_CALLBACK_TYPE_READ_CHAR_VALUE)
    {
        if (GATT_SVC_BAS_BATTERY_LEVEL_INDEX == p_param->off_handle)
        {
            is_app_cb = true;
            //write db the battery_level
            bsal_srv_write_data(p_param->stack_ptr, p_param->start_handle, p_param->off_handle, sizeof(uint8_t), &bsal_bas_battery_level);
        }

    }
    else if (p_param->msg_type == BSAL_CALLBACK_TYPE_WRITE_CHAR_VALUE)
    {
        //do nothing
        is_app_cb = true;
    }
    else if (p_param->msg_type == BSAL_CALLBACK_TYPE_INDIFICATION_NOTIFICATION)
    {
        if (GATT_SVC_BAS_CHAR_CCCD_INDEX == p_param->off_handle)
        {
            if (p_param->length == 2)
            {
                //uint16_t ccdbit = (uint16_t)p_param->data;
                is_app_cb = true;
            }
        }
    }
    else if (p_param->msg_type == BSAL_CALLBACK_TYPE_HANDLE_TABLE)
    {
        //save the could notify u_handle
        // bsal_start_handle = p_param->start_handle;
    }

    if (is_app_cb && (pfn_bas_cb != NULL))
    {
        pfn_bas_cb(p_param);
    }

    //read write the datebase

}

void bsal_le_bas_svr_init(void *stack_ptr, void *app_callback)
{
    struct bsal_gatt_app_srv_def ble_svc_bas_defs[] =
    {
        {
            /*** Battery Service. */
            .type = BSAL_GATT_UUID_PRIMARY_SERVICE,
            .uuid = BSAL_UUID16_DECLARE(GATT_UUID_BATTERY),//(bsal_uuid_any_t *)&battery_srv,//BSAL_UUID16_DECLARE(GATT_UUID_BATTERY),
            .characteristics = (bsal_gatt_chr_def_t[])
            {
                {
                    /*** Battery level characteristic */
                    .uuid = BSAL_UUID16_DECLARE(GATT_UUID_CHAR_BAS_LEVEL),//(bsal_uuid_any_t *)&bas_char_bas_level,//BSAL_UUID16_DECLARE(GATT_UUID_CHAR_BAS_LEVEL),
                    .properties = BSAL_ATT_P_READ
                    | BSAL_ATT_P_NOTIFY
                    ,
                    .permission = BSAL_GATT_PERM_READ_NONE,
                    .value_length = 1,
                },
                {
                    0, /* No more characteristics in this service. */
                }
            },
        },

        {
            0, /* No more services. */
        },
    };
    bsal_stack_le_srv_reg_func(stack_ptr, &ble_svc_bas_defs, (P_SRV_GENERAL_CB *)profile_callback);
    pfn_bas_cb = (P_SRV_GENERAL_CB)app_callback;
}
void bsal_le_bas_svr_deinit(void *stack_ptr)
{
    //delete the service
}


//no notify
void bsal_set_battery_level_static(uint8_t battery_level)
{
    bsal_bas_battery_level = battery_level;
}

void bsal_bas_send_notify_level(void *stack_ptr, uint16_t conn_id,  uint8_t battery_level)
{
    /*TODO*/
    if (battery_level > 100)
    {
        bsal_osif_printf_err("The battery is invalid:%d", battery_level);
        return;
    }
    bsal_uuid_any_t uuid_srv;
    uuid_srv.u_type = 16;
    uuid_srv.u16.value = GATT_UUID_BATTERY;
    uint16_t start_handle = bsal_srv_get_start_handle(stack_ptr, uuid_srv);
    bsal_bas_battery_level = battery_level;
    bsal_srv_send_notify_data(stack_ptr, conn_id, start_handle, GATT_SVC_BAS_BATTERY_LEVEL_INDEX, sizeof(uint8_t), &battery_level);
}


