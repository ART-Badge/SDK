/**
*****************************************************************************************
*     Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    ams_client.c
  * @brief
  * @details
  * @author
  * @date
  * @version v1.0
  ***************************************************************************************
*/
#include <string.h>
#include "ams_client.h"
#include "os_mem.h"
#include "trace.h"

/**
 * @brief  AMS Link control block defination.
 */
typedef struct
{
    T_AMS_DISC_STATE    disc_state;
    bool                remote_cmd_notify;
    bool                entity_upd_notify;
    uint16_t            hdl_cache[AMS_HDL_CACHE_LEN];
} T_AMS_LINK, *P_AMS_LINK;

//AMS service UUID 89D3502B-0F36-433A-8EF4-C502AD55F8DC
uint8_t AMS_SRV_UUID128[16] = {0xdc, 0xf8, 0x55, 0xad, 0x02, 0xc5, 0xf4, 0x8e, 0x3a, 0x43, 0x36, 0x0f, 0x2b, 0x50, 0xd3, 0x89};
//Remote Command UUID 9B3C81D8-57B1-4A8A-B8DF-0E56F7CA51C2 (writeable, notifiable)
const uint8_t AMS_CHAR_REMOTE_CMD_UUID128[16] = {0xc2, 0x51, 0xca, 0xf7, 0x56, 0x0e, 0xdf, 0xb8, 0x8a, 0x4a, 0xb1, 0x57, 0xd8, 0x81, 0x3c, 0x9b};
//Entity Update UUID 2F7CABCE-808D-411F-9A0C-BB92BA96C102 (writeable with response, notifiable)
const uint8_t AMS_CHAR_ENTITY_UPD_UUID128[16] = {0x02, 0xc1, 0x96, 0xba, 0x92, 0xbb, 0x0c, 0x9a, 0x1f, 0x41, 0x8d, 0x80, 0xce, 0xab, 0x7c, 0x2f};
//Entity Attribute UUID C6B2F38C-23AB-46D8-A6AB-A3A870BBD5D7 (readable, writeable)
const uint8_t AMS_CHAR_ENTITY_ATTR_UUID128[16] = {0xd7, 0xd5, 0xbb, 0x70, 0xa8, 0xa3, 0xab, 0xa6, 0xd8, 0x46, 0xab, 0x23, 0x8c, 0xf3, 0xb2, 0xc6};

P_AMS_LINK ams_table;
static uint8_t ams_link_num;
T_CLIENT_ID ams_client_id = CLIENT_PROFILE_GENERAL_ID;
static P_FUN_GENERAL_APP_CB ams_client_cb = NULL;

uint16_t ams_search_handle(uint8_t conn_id, T_AMS_HANDLE_TYPE handle_type)
{
    uint16_t handle = 0;
    P_AMS_LINK link_info = ams_table;

    if (handle_type >= AMS_HDL_CACHE_LEN)
    {
        APP_PRINT_ERROR1("ams_search_handle: no handle_type %d", handle_type);
        return AMS_HDL_CACHE_LEN;
    }

    handle = link_info[conn_id].hdl_cache[handle_type];

    APP_PRINT_INFO1("ams_search_handle: ams handle 0x%08x", handle);

    return handle;
}



/**
 * @brief  start AMS service discovery procedure.
 */
bool ams_start_discovery(uint8_t conn_id)
{
    if (conn_id >= ams_link_num)
    {
        PROFILE_PRINT_ERROR2("ams_start_discovery: failed invalid conn_id %d, ams_link_num = %d", conn_id,
                             ams_link_num);
        return false;
    }

    //clear handle cache first
    memset(ams_table[conn_id].hdl_cache, 0, 2 * AMS_HDL_CACHE_LEN);
    ams_table[conn_id].disc_state = AMS_DISC_START;
    APP_PRINT_INFO0("ams_start_discovery");
    if (client_by_uuid128_srv_discovery(conn_id, ams_client_id, AMS_SRV_UUID128) == GAP_CAUSE_SUCCESS)
    {
        return true;
    }
    return false;
}

/**
 * @brief  start AMS characteristic discovery procedure.
 */
bool ams_discovery_char(uint8_t conn_id)
{
    uint16_t start_handle = ams_table[conn_id].hdl_cache[AMS_HDL_SRV_START];
    uint16_t end_handle = ams_table[conn_id].hdl_cache[AMS_HDL_SRV_END];

    APP_PRINT_INFO0("ams_start_char_discovery");
    if (client_all_char_discovery(conn_id, ams_client_id, start_handle,
                                  end_handle) == GAP_CAUSE_SUCCESS)
    {
        return true;
    }
    return false;
}

/**
 * @brief  AMS remote command subscribe.
 */
bool ams_subscribe_remote_cmd(uint8_t conn_id, bool subscribe)
{
    uint16_t handle;
    if (conn_id >= ams_link_num)
    {
        PROFILE_PRINT_ERROR1("ams_subscribe_remote_cmd: failed invalid conn_id %d", conn_id);
        return false;
    }
    handle = ams_table[conn_id].hdl_cache[AMS_HDL_REMOTE_CMD_CCCD];
    if (handle)
    {
        uint16_t cccd = subscribe ? 1 : 0;
        APP_PRINT_INFO0("ams_subscribe_remote_cmd");
        if (client_attr_write(conn_id, ams_client_id, GATT_WRITE_TYPE_REQ, handle, sizeof(uint16_t),
                              (uint8_t *)&cccd) == GAP_CAUSE_SUCCESS)
        {
            ams_table[conn_id].remote_cmd_notify = 1;
            return true;
        }
    }
    return false;
}

/**
 * @brief  AMS write remote command.
 */
bool ams_write_remote_cmd(uint8_t conn_id, T_AMS_REMOTE_CMD_ID cmd_id)
{
    uint16_t handle = ams_table[conn_id].hdl_cache[AMS_HDL_REMOTE_CMD_VALUE];
    if (handle)
    {
        APP_PRINT_INFO0("ams_write_remote_cmd");
        if (client_attr_write(conn_id, ams_client_id, GATT_WRITE_TYPE_REQ, handle,
                              1, (uint8_t *)&cmd_id) == GAP_CAUSE_SUCCESS)
        {
            return true;
        }
    }
    return false;
}


/**
 * @brief  AMS entity update subscribe.
 */
bool ams_subscribe_entity_upd(uint8_t conn_id, bool subscribe)
{
    uint16_t handle;
    if (conn_id >= ams_link_num)
    {
        PROFILE_PRINT_ERROR1("ams_subscribe_entity_upd: failed invalid conn_id %d", conn_id);
        return false;
    }
    handle = ams_table[conn_id].hdl_cache[AMS_HDL_ENTITY_UPD_CCCD];
    if (handle)
    {
        uint16_t cccd = subscribe ? 1 : 0;
        APP_PRINT_INFO0("ams_subscribe_entity_upd");
        if (client_attr_write(conn_id, ams_client_id, GATT_WRITE_TYPE_REQ, handle, sizeof(uint16_t),
                              (uint8_t *)&cccd) == GAP_CAUSE_SUCCESS)
        {
            ams_table[conn_id].entity_upd_notify = 1;
            return true;
        }
    }
    return false;
}

/**
 * @brief  AMS write entity update command.
    entity_attr_id, first byte is entity id, the other bytes are attribute id list.
 */
bool ams_write_entity_upd_cmd(uint8_t conn_id, uint8_t *p_value, uint8_t value_len)
{
    uint16_t handle = ams_table[conn_id].hdl_cache[AMS_HDL_ENTITY_UPD_VALUE];
    if (handle)
    {
        APP_PRINT_INFO0("ams_write_entity_upd_cmd");
        if (client_attr_write(conn_id, ams_client_id, GATT_WRITE_TYPE_REQ, handle,
                              value_len, p_value) == GAP_CAUSE_SUCCESS)
        {
            return true;
        }
    }
    return false;
}


/**
 * @brief  AMS write entity attribute.
 */
bool ams_write_entity_attr(uint8_t conn_id, T_AMS_ENTITY_ATTR entity_attr)
{
    uint16_t handle = ams_table[conn_id].hdl_cache[AMS_HDL_ENTITY_ATTR_VALUE];
    if (handle)
    {
        APP_PRINT_INFO0("ams_write_entity_attr");
        if (client_attr_write(conn_id, ams_client_id, GATT_WRITE_TYPE_REQ, handle, sizeof(entity_attr),
                              (uint8_t *)&entity_attr) == GAP_CAUSE_SUCCESS)
        {
            return true;
        }
    }
    return false;
}


/**
 * @brief  AMS read entity attribute.
 */
bool ams_read_entity_attr(uint8_t conn_id, T_AMS_ENTITY_ATTR entity_attr)
{
    uint16_t handle = ams_table[conn_id].hdl_cache[AMS_HDL_ENTITY_ATTR_VALUE];
    if (handle)
    {
        APP_PRINT_INFO0("ams_read_entity_attr");
        if (client_attr_read(conn_id, ams_client_id, handle) == GAP_CAUSE_SUCCESS)
        {
            return true;
        }
    }
    return false;
}


/**
  * @brief  Called by profile client layer, when discover state of discovery procedure changed.
  */
static void ams_client_cb_discover_state(uint8_t conn_id,  T_DISCOVERY_STATE discovery_state)
{
    bool cb_flag = false;
    T_AMS_CB_DATA cb_data;
    cb_data.cb_type = AMS_CLIENT_CB_TYPE_DISC_STATE;

    APP_PRINT_INFO1("ams_client_cb_discover_state: discovery state = %d", discovery_state);
    if (ams_table[conn_id].disc_state == AMS_DISC_START)
    {
        switch (discovery_state)
        {
        case DISC_STATE_SRV_DONE:
            if ((ams_table[conn_id].hdl_cache[AMS_HDL_SRV_START] != 0) ||
                (ams_table[conn_id].hdl_cache[AMS_HDL_SRV_END] != 0))
            {
                // start discover characteristic.
                if (ams_discovery_char(conn_id) == false)
                {
                    ams_table[conn_id].disc_state = AMS_DISC_FAILED;
                    cb_flag = true;
                }
            }
            else // No AMS handle found.
            {
                ams_table[conn_id].disc_state = AMS_DISC_FAILED;
                cb_flag = true;
            }
            break;

        case DISC_STATE_CHAR_DONE:
            ams_table[conn_id].disc_state = AMS_DISC_DONE;
            cb_flag = true;
            break;

        case DISC_STATE_FAILED:
            ams_table[conn_id].disc_state = AMS_DISC_FAILED;
            cb_flag = true;
            break;

        default:
            APP_PRINT_ERROR0("ams_client_cb_discover_state: invalid discovery state!");
            break;
        }
    }

    // send discover state to app if need
    if (cb_flag && ams_client_cb)
    {
        cb_data.cb_content.disc_state = ams_table[conn_id].disc_state;
        (*ams_client_cb)(ams_client_id, conn_id, &cb_data);
    }
    return;
}

/**
  * @brief  Called by profile client layer, when discover result fetched.
  */
static void ams_client_cb_discover_result(uint8_t conn_id,  T_DISCOVERY_RESULT_TYPE result_type,
                                          T_DISCOVERY_RESULT_DATA result_data)
{
    APP_PRINT_INFO1("ams_client_discover_result_cb: result type = %d", result_type);
    if (ams_table[conn_id].disc_state == AMS_DISC_START)
    {
        switch (result_type)
        {
        case DISC_RESULT_SRV_DATA:
            {
                ams_table[conn_id].hdl_cache[AMS_HDL_SRV_START] = result_data.p_srv_disc_data->att_handle;
                ams_table[conn_id].hdl_cache[AMS_HDL_SRV_END] = result_data.p_srv_disc_data->end_group_handle;
            }
            break;

        case DISC_RESULT_CHAR_UUID128:
            {
                uint16_t handle;
                handle = result_data.p_char_uuid128_disc_data->value_handle;
                if (0 == memcmp(AMS_CHAR_REMOTE_CMD_UUID128, result_data.p_char_uuid128_disc_data->uuid128, 16))
                {
                    ams_table[conn_id].hdl_cache[AMS_HDL_REMOTE_CMD_VALUE] = handle;
                    ams_table[conn_id].hdl_cache[AMS_HDL_REMOTE_CMD_CCCD] = handle + 2;
                }
                else if (0 == memcmp(AMS_CHAR_ENTITY_UPD_UUID128, result_data.p_char_uuid128_disc_data->uuid128,
                                     16))
                {
                    ams_table[conn_id].hdl_cache[AMS_HDL_ENTITY_UPD_VALUE] = handle;
                    ams_table[conn_id].hdl_cache[AMS_HDL_ENTITY_UPD_CCCD] = handle + 2;
                }
                else if (0 == memcmp(AMS_CHAR_ENTITY_ATTR_UUID128, result_data.p_char_uuid128_disc_data->uuid128,
                                     16))
                {
                    ams_table[conn_id].hdl_cache[AMS_HDL_ENTITY_ATTR_VALUE] = handle;
                }
            }
            break;

        default:
            APP_PRINT_ERROR0("ams_client_discover_result_cb: invalid discovery result type");
            break;
        }
    }

    return;
}

static void ams_client_cb_read_result(uint8_t conn_id,  uint16_t cause,
                                      uint16_t handle, uint16_t value_size, uint8_t *p_value)
{
    T_AMS_CB_DATA cb_data;
    uint16_t *hdl_cache;
    hdl_cache = ams_table[conn_id].hdl_cache;

    cb_data.cb_type = AMS_CLIENT_CB_TYPE_READ_RESULT;

    APP_PRINT_INFO2("ams_client_cb_read_result: handle 0x%x, cause 0x%x", handle, cause);
    cb_data.cb_content.read_data.cause = cause;

    if (handle == hdl_cache[AMS_HDL_ENTITY_ATTR_VALUE])
    {
        cb_data.cb_content.read_data.type = AMS_READ_FROM_ENTITY_UPD;
        if (cause == GAP_SUCCESS)
        {
            cb_data.cb_content.read_data.p_value = p_value;
            cb_data.cb_content.read_data.value_size = value_size;
        }
        else
        {
            cb_data.cb_content.read_data.value_size = 0;
        }
    }
    else
    {
        return;
    }
    /* Inform application the read result. */
    if (ams_client_cb)
    {
        (*ams_client_cb)(ams_client_id, conn_id, &cb_data);
    }

    return;
}

/**
  * @brief  Called by profile client layer, when write result arrived.
  */
static void ams_client_cb_write_result(uint8_t conn_id, T_GATT_WRITE_TYPE type, uint16_t handle,
                                       uint16_t cause, uint8_t credits)
{
    T_AMS_CB_DATA cb_data;
    uint16_t *hdl_cache;
    hdl_cache = ams_table[conn_id].hdl_cache;
    cb_data.cb_type = AMS_CLIENT_CB_TYPE_WRITE_RESULT;

    APP_PRINT_INFO2("ams_client_write_result_cb: handle=0x%x cause = 0x%x", handle, cause);
    cb_data.cb_content.write_result.cause = cause;

    if (handle == hdl_cache[AMS_HDL_REMOTE_CMD_CCCD])
    {
        if (ams_table[conn_id].remote_cmd_notify == 1)
        {
            cb_data.cb_content.write_result.type = AMS_WRITE_REMOTE_CMD_NOTIFY_ENABLE;
        }
        else
        {
            cb_data.cb_content.write_result.type = AMS_WRITE_REMOTE_CMD_NOTIFY_DISABLE;
        }
    }
    else if (handle == hdl_cache[AMS_HDL_ENTITY_UPD_CCCD])
    {
        if (ams_table[conn_id].entity_upd_notify == 1)
        {
            cb_data.cb_content.write_result.type = AMS_WRITE_ENTITY_UPD_NOTIFY_ENABLE;
        }
        else
        {
            cb_data.cb_content.write_result.type = AMS_WRITE_ENTITY_UPD_NOTIFY_DISABLE;
        }
    }
    else if (handle == hdl_cache[AMS_HDL_REMOTE_CMD_VALUE])
    {
        cb_data.cb_content.write_result.type = AMS_WRITE_REMOTE_CMD_VALUE;
    }
    else if (handle == hdl_cache[AMS_HDL_ENTITY_UPD_VALUE])
    {
        cb_data.cb_content.write_result.type = AMS_WRITE_ENTITY_UPD_VALUE;
    }
    else if (handle == hdl_cache[AMS_HDL_ENTITY_ATTR_VALUE])
    {
        cb_data.cb_content.write_result.type = AMS_WRITE_ENTITY_ATTR_VALUE;
    }
    else
    {
        return;
    }

    if (ams_client_cb)
    {
        (*ams_client_cb)(ams_client_id, conn_id, &cb_data);
    }

    return;
}

/**
  * @brief  Called by profile client layer, when notification or indication arrived.
  */
static T_APP_RESULT ams_client_cb_notify_ind(uint8_t conn_id, bool notify, uint16_t handle,
                                             uint16_t value_size, uint8_t *p_value)
{
    T_APP_RESULT app_result = APP_RESULT_SUCCESS;
    T_AMS_CB_DATA cb_data;
    uint16_t *hdl_cache;

    hdl_cache = ams_table[conn_id].hdl_cache;
    cb_data.cb_type = AMS_CLIENT_CB_TYPE_NOTIF_IND_RESULT;

    if (handle == hdl_cache[AMS_HDL_REMOTE_CMD_VALUE])
    {
        cb_data.cb_content.notify_data.type = AMS_NOTIFY_FROM_REMOTE_CMD;
        cb_data.cb_content.notify_data.value_size = value_size;
        cb_data.cb_content.notify_data.p_value = p_value;
    }
    else if (handle == hdl_cache[AMS_HDL_ENTITY_UPD_VALUE])
    {
        cb_data.cb_content.notify_data.type = AMS_NOTIFY_FROM_ENTITY_UPD;
        cb_data.cb_content.notify_data.value_size = value_size;
        cb_data.cb_content.notify_data.p_value = p_value;
    }
    else
    {
        return APP_RESULT_SUCCESS;
    }

    if (ams_client_cb)
    {
        app_result = (*ams_client_cb)(ams_client_id, conn_id, &cb_data);
    }

    return app_result;
}

/**
  * @brief  Called by profile client layer, when one LE link is disconnected.
  */
static void ams_client_cb_disc(uint8_t conn_id)
{
    APP_PRINT_INFO0("ams_client_disc_cb.");
    memset(&ams_table[conn_id], 0, sizeof(T_AMS_LINK));
    if (ams_client_cb)
    {
        T_AMS_CB_DATA cb_data;
        cb_data.cb_type = AMS_CLIENT_CB_TYPE_DISCONNECT_INFO;
        (*ams_client_cb)(ams_client_id, conn_id, &cb_data);
    }
    return;
}
/**
 * @brief AMS Client Callbacks.
*/
const T_FUN_CLIENT_CBS ams_cbs =
{
    ams_client_cb_discover_state,   //!< Discovery State callback function pointer
    ams_client_cb_discover_result,  //!< Discovery result callback function pointer
    ams_client_cb_read_result,      //!< Read response callback function pointer
    ams_client_cb_write_result,     //!< Write result callback function pointer
    ams_client_cb_notify_ind,       //!< Notify Indicate callback function pointer
    ams_client_cb_disc              //!< Link disconnection callback function pointer
};

/**
 * @brief       Add ams client.
 *
 * @param[in]   app_cb  Callback to notify client read/write/notify/indicate events.
 * @param[in]   link_num Initialize link number
 * @return Client ID of the specific client module.
 * @retval 0xff failed.
 * @retval other success.
 */
T_CLIENT_ID ams_add_client(P_FUN_GENERAL_APP_CB app_cb, uint8_t link_num)
{
    uint16_t size;
    if (link_num > AMS_MAX_LINKS)
    {
        PROFILE_PRINT_ERROR1("ams_add_client: invalid link_num %d", link_num);
        return 0xff;
    }
    if (false == client_register_spec_client_cb(&ams_client_id, &ams_cbs))
    {
        ams_client_id = CLIENT_PROFILE_GENERAL_ID;
        APP_PRINT_ERROR0("ams_add_client Fail !!!");
        return ams_client_id;
    }
    APP_PRINT_INFO1("ams_add_client: client ID = %d", ams_client_id);

    /* register callback for profile to inform application that some events happened. */
    ams_client_cb = app_cb;
    ams_link_num = link_num;
    size = ams_link_num * sizeof(T_AMS_LINK);
    ams_table = os_mem_zalloc(RAM_TYPE_DATA_ON, size);

    return ams_client_id;
}

