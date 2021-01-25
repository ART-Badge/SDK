/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-28     Supperthomas the first version
 */



#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "bsal.h"
#include "bsal_osif.h"
#include "bsal_srv_blufi.h"


#define GATT_UUID_BLUFI_PROFILE                   0xFFFF
#define GATT_UUID_CHAR_BLUFI_V2_WRITE             0xFF01
#define GATT_UUID_CHAR_BLUFI_V4_INDICATE          0xFF02


#define BLUEFI_BLE_SERVICE_CHAR_V2_WRITE_INDEX          0x02
#define BLUEFI_BLE_SERVICE_CHAR_V3_INDICATE_INDEX         0x04
#define BLUEFI_BLE_SERVICE_CHAR_INDICATE_CCCD_INDEX     (BLUEFI_BLE_SERVICE_CHAR_V3_INDICATE_INDEX + 1)

#define BLUFI_RECV_BUFF_SIZE  200

struct basl_blufi_recv_data
{
    uint8_t buf[BLUFI_RECV_BUFF_SIZE];
    uint16_t alloc_len;
    uint8_t recv_offset;
};


static struct basl_blufi_recv_data blufi_data;
static P_SRV_GENERAL_CB pfn_bas_cb = NULL;
uint8_t bsal_blufi_push_data(struct basl_blufi_recv_data *blufi_data, uint8_t length, uint8_t *data)
{
    if (blufi_data->recv_offset + length > blufi_data->alloc_len)
    {
        //error
        return 1;
    }
    if (data[0] == '{')
    {
        //first entry
        blufi_data->recv_offset = 0;
        memset(blufi_data->buf, 0, blufi_data->alloc_len);
    }
    memcpy(&blufi_data->buf[blufi_data->recv_offset], data, length);
    blufi_data->recv_offset += length;
    //check the data
    if (data[length - 1] == '}')
    {
        return 0xff;
    }
    return 0;
}


static void profile_callback(void *p)
{
    bsal_callbak_data_t *p_param = (bsal_callbak_data_t *)p;
    bool is_app_cb = false;
    //BSAL_STATUS bsal_result = BSAL_RESULT_SUCCESS;
    if (p_param->msg_type == BSAL_CALLBACK_TYPE_READ_CHAR_VALUE)
    {
        //NO DEAL
    }
    else if (p_param->msg_type == BSAL_CALLBACK_TYPE_WRITE_CHAR_VALUE)
    {
        //do nothing
        if (BLUEFI_BLE_SERVICE_CHAR_V2_WRITE_INDEX == p_param->off_handle)
        {
            uint8_t ret =  bsal_blufi_push_data(&blufi_data, p_param->length, p_param->data);
            if (ret == 0xff)
            {
                //the data is ready
                is_app_cb = true;
                p_param->data = blufi_data.buf;
                p_param->length = blufi_data.recv_offset;
                //bsal_osif_printf("\r\n BLUFI: THE RECEIVE DATA IS :%s \r\n", p_param->data);
            }
        }
    }
    else if (p_param->msg_type == BSAL_CALLBACK_TYPE_INDIFICATION_NOTIFICATION)
    {
        if (BLUEFI_BLE_SERVICE_CHAR_INDICATE_CCCD_INDEX == p_param->off_handle)
        {
            if (p_param->length == 2)
            {
                // uint16_t ccdbit = (uint32_t)p_param->data;
                is_app_cb = true;
            }
        }
    }
    if (is_app_cb && (pfn_bas_cb != NULL))
    {
        pfn_bas_cb(p_param);
    }
}

void bsal_le_blufi_svr_init(void *stack_ptr, void *app_callback)
{
    struct bsal_gatt_app_srv_def ble_svc_blufi_defs[] =
    {
        {
            /*** Battery Service. */
            .type = BSAL_GATT_UUID_PRIMARY_SERVICE,
            .uuid = BSAL_UUID16_DECLARE(GATT_UUID_BLUFI_PROFILE),
            .characteristics = (bsal_gatt_chr_def_t[])
            {
                {
                    /*** Battery level characteristic */
                    .uuid = BSAL_UUID16_DECLARE(GATT_UUID_CHAR_BLUFI_V2_WRITE),
                    .properties = BSAL_ATT_P_WRITE
                    ,
                    .permission = GATT_PERM_WRITE,
                    .value_length = 1,
                },
                {
                    /*** Battery level characteristic */
                    .uuid = BSAL_UUID16_DECLARE(GATT_UUID_CHAR_BLUFI_V4_INDICATE),
                    .properties = BSAL_ATT_P_INDICATE
                    ,
                    .permission = GATT_PERM_READ | GATT_PERM_WRITE,
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
    bsal_stack_le_srv_reg_func(stack_ptr, &ble_svc_blufi_defs, (P_SRV_GENERAL_CB *)profile_callback);

    pfn_bas_cb = app_callback;
    blufi_data.alloc_len = BLUFI_RECV_BUFF_SIZE;
    blufi_data.recv_offset = 0;
}

void bsal_blufi_ble_send_v4_indicate(void *stack_ptr, uint16_t conn_id,  void *p_value, uint16_t length)
{
    bsal_uuid_any_t uuid_srv;
    uuid_srv.u_type = BSAL_UUID_TYPE_16BIT;
    uuid_srv.u16.value = GATT_UUID_BLUFI_PROFILE;
    uint16_t  start_handle = bsal_srv_get_start_handle(stack_ptr, uuid_srv);
    bsal_srv_send_notify_data(stack_ptr, conn_id, start_handle, BLUEFI_BLE_SERVICE_CHAR_INDICATE_CCCD_INDEX, length, p_value);
}


void bsal_le_blufi_svr_deinit(void *stack_ptr)
{
    //deinit
}


