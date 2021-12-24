/**
*****************************************************************************************
*     Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      ams.c
   * @brief     This file handles AMS Client routines.
   * @author
   * @date
   * @version
   **************************************************************************************
*/
#include "app_flags.h"
#if F_BT_AMS_CLIENT_SUPPORT
#include <trace.h>
#include <string.h>
#include <os_msg.h>
#include <os_mem.h>
#include <ams.h>
#include <ams_client.h>

T_CLIENT_ID     ams_client;

/**
 * @brief  ams clinet callback handle message from upperstack
 */
T_APP_RESULT ams_client_cb(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data)
{
    T_APP_RESULT  result = APP_RESULT_SUCCESS;
    T_AMS_CB_DATA *p_cb_data = (T_AMS_CB_DATA *)p_data;

    switch (p_cb_data->cb_type)
    {
    case AMS_CLIENT_CB_TYPE_DISC_STATE:
        switch (p_cb_data->cb_content.disc_state)
        {
        case AMS_DISC_DONE:
            APP_PRINT_INFO0("ams_client_cb: discover procedure done.");
            ams_subscribe_remote_cmd(conn_id, true);
            break;
        case AMS_DISC_FAILED:
            APP_PRINT_ERROR0("ams_client_cb: discover request failed.");
            break;
        default:
            break;
        }
        break;

    case AMS_CLIENT_CB_TYPE_NOTIF_IND_RESULT:
        switch (p_cb_data->cb_content.notify_data.type)
        {
        case AMS_NOTIFY_FROM_REMOTE_CMD:
            APP_PRINT_INFO2("AMS_NOTIFY_FROM_REMOTE_CMD: data[%d]: %b",
                            p_cb_data->cb_content.notify_data.value_size,
                            TRACE_BINARY(p_cb_data->cb_content.notify_data.value_size,
                                         p_cb_data->cb_content.notify_data.p_value));
            break;
        case AMS_NOTIFY_FROM_ENTITY_UPD:
            APP_PRINT_INFO2("AMS_NOTIFY_FROM_ENTITY_UPD: data[%d]: %b",
                            p_cb_data->cb_content.notify_data.value_size,
                            TRACE_BINARY(p_cb_data->cb_content.notify_data.value_size,
                                         p_cb_data->cb_content.notify_data.p_value));
            break;
        default:
            break;
        }
        break;

    case AMS_CLIENT_CB_TYPE_READ_RESULT:
        switch (p_cb_data->cb_content.read_data.type)
        {
        case AMS_READ_FROM_ENTITY_UPD:
            APP_PRINT_INFO3("AMS_READ_FROM_ENTITY_UPD: cause 0x%x, data[%d]: %b",
                            p_cb_data->cb_content.read_data.cause,
                            p_cb_data->cb_content.read_data.value_size,
                            TRACE_BINARY(p_cb_data->cb_content.read_data.value_size, p_cb_data->cb_content.read_data.p_value));
            break;
        default:
            break;
        }
        break;

    case AMS_CLIENT_CB_TYPE_WRITE_RESULT:
        {
            if (p_cb_data->cb_content.write_result.cause != ATT_SUCCESS)
            {
                APP_PRINT_ERROR1("AMS_CLIENT_CB_TYPE_WRITE_RESULT: Failed, cause 0x%x",
                                 p_cb_data->cb_content.write_result.cause);
            }
            switch (p_cb_data->cb_content.write_result.type)
            {
            case AMS_WRITE_REMOTE_CMD_NOTIFY_ENABLE:
                APP_PRINT_INFO0("AMS_WRITE_REMOTE_CMD_NOTIFY_ENABLE");
                ams_subscribe_entity_upd(conn_id, true);
                break;

            case AMS_WRITE_REMOTE_CMD_NOTIFY_DISABLE:
                APP_PRINT_INFO0("AMS_WRITE_REMOTE_CMD_NOTIFY_DISABLE");
                break;

            case AMS_WRITE_ENTITY_UPD_NOTIFY_ENABLE:
                APP_PRINT_INFO0("AMS_WRITE_ENTITY_UPD_NOTIFY_ENABLE");

                break;
            case AMS_WRITE_ENTITY_UPD_NOTIFY_DISABLE:
                APP_PRINT_INFO0("AMS_WRITE_ENTITY_UPD_NOTIFY_DISABLE");
                break;

            case AMS_WRITE_ENTITY_ATTR_VALUE:
                APP_PRINT_INFO0("AMS_WRITE_ENTITY_ATTR_VALUE");
                break;

            case AMS_WRITE_REMOTE_CMD_VALUE:
                APP_PRINT_INFO0("AMS_WRITE_REMOTE_CMD_VALUE");
                break;

            default:
                break;
            }
        }
        break;

    case AMS_CLIENT_CB_TYPE_DISCONNECT_INFO:
        {
            APP_PRINT_INFO1("AMS_CLIENT_CB_TYPE_DISCONNECT_INFO: conn_id = 0x%x", conn_id);
        }
        break;

    default:
        break;
    }
    return result;
}

/**
 * @brief  App register ams client to upperstack.
 */
void ams_init(uint8_t link_num)
{
    ams_client = ams_add_client(ams_client_cb, link_num);
}

#endif

