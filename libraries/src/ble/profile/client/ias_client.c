/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    ias_client.c
  * @brief   Ias BLE client source file.
  * @details
  * @author  jane
  * @date    2016-02-18
  * @version v1.0
  ******************************************************************************
  */

/** Add Includes here **/
#include <trace.h>
#include <string.h>
#include <ias_client.h>
#include <os_mem.h>

/********************************************************************************************************
* local static variables defined here, only used in this source file.
********************************************************************************************************/

typedef struct
{
    T_IAS_DISC_STATE disc_state;
    uint16_t          hdl_cache[HDL_IAS_CACHE_LEN];
} T_IAS_LINK, *P_IAS_LINK;

static P_IAS_LINK ias_table;
static uint8_t ias_link_num;

/**<  ias client ID. */
static T_CLIENT_ID ias_client = CLIENT_PROFILE_GENERAL_ID;
/**<  Callback used to send data to app from ias client layer. */
static P_FUN_GENERAL_APP_CB ias_client_cb = NULL;

/**
  * @brief  Used by application, to start the discovery procedure of ias server.
  * @param[in]  conn_id connection ID.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
bool ias_client_start_discovery(uint8_t conn_id)
{
    PROFILE_PRINT_INFO0("ias_client_start_discovery");
    if (conn_id >= ias_link_num)
    {
        PROFILE_PRINT_ERROR1("ias_client_start_discovery: failed invalid conn_id %d", conn_id);
        return false;
    }
    /* First clear handle cache. */
    memset(&ias_table[conn_id], 0, sizeof(T_IAS_LINK));
    ias_table[conn_id].disc_state = DISC_IAS_START;
    if (client_by_uuid_srv_discovery(conn_id, ias_client,
                                     GATT_UUID_IMMEDIATE_ALERT_SERVICE) == GAP_CAUSE_SUCCESS)
    {
        return true;
    }
    return false;
}

/**
  * @brief  Used by application, to read data from server by using handles.
  * @param[in]  conn_id connection ID.
  * @param[in]  read_type one of characteristic that has the readable property.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
//bool ias_client_read_by_handle(uint8_t conn_id, T_IAS_READ_TYPE read_type)//~~~~~~~~~~~~~~~~~~will delete
//{
//    bool hdl_valid = false;
//    uint16_t  handle;
//    if (conn_id >= ias_link_num)
//    {
//        PROFILE_PRINT_ERROR1("ias_client_read_by_handle: failed invalid conn_id %d", conn_id);
//        return false;
//    }

//    switch (read_type)
//    {
//    case IAS_READ_V1_READ:
//        if (ias_table[conn_id].hdl_cache[HDL_IAS_V1_READ])
//        {
//            handle = ias_table[conn_id].hdl_cache[HDL_IAS_V1_READ];
//            hdl_valid = true;
//        }
//        break;
//    default:
//        return false;
//    }

//    if (hdl_valid)
//    {
//        if (client_attr_read(conn_id, ias_client, handle) == GAP_CAUSE_SUCCESS)
//        {
//            return true;
//        }
//    }

//    APP_PRINT_WARN0("ias_client_read_by_handle: Request fail! Please check!");
//    return false;
//}

/**
  * @brief  Used by application, to read data from server by using UUIDs.
  * @param[in]  conn_id connection ID.
  * @param[in]  read_type one of characteristic that has the readable property.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
//bool ias_client_read_by_uuid(uint8_t conn_id, T_IAS_READ_TYPE read_type)//~~~~~~~~~~~~~~~~~~will delete
//{
//    uint16_t start_handle;
//    uint16_t end_handle;
//    uint16_t  uuid16;
//    if (conn_id >= ias_link_num)
//    {
//        PROFILE_PRINT_ERROR1("ias_client_read_by_uuid: failed invalid conn_id %d", conn_id);
//        return false;
//    }

//    switch (read_type)
//    {
//    case IAS_READ_V1_READ:
//        start_handle = ias_table[conn_id].hdl_cache[HDL_IAS_SRV_START];
//        end_handle = ias_table[conn_id].hdl_cache[HDL_IAS_SRV_END];
//        uuid16 = GATT_UUID_CHAR_ALERT_LEVEL;
//        break;
//    default:
//        return false;
//    }

//    if (client_attr_read_using_uuid(conn_id, ias_client, start_handle, end_handle,
//                                    uuid16, NULL) == GAP_CAUSE_SUCCESS)
//    {
//        return true;
//    }
//    return false;
//}

/**
  * @brief  Used by application, to enable or disable the notification of peer server's V3 Notify Characteristic.
  * @param[in]  conn_id connection ID.
  * @param[in]  notify 0--disable the notification, 1--enable the notification.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
//bool simp_ble_client_set_v3_notify(uint8_t conn_id, bool notify)
//{
//    if (conn_id >= simp_link_num)
//    {
//        PROFILE_PRINT_ERROR1("simp_ble_client_set_v3_notify: failed invalid conn_id %d", conn_id);
//        return false;
//    }

//    if (simp_table[conn_id].hdl_cache[HDL_SIMBLE_V3_NOTIFY_CCCD])
//    {
//        uint16_t handle = simp_table[conn_id].hdl_cache[HDL_SIMBLE_V3_NOTIFY_CCCD];
//        uint16_t length = sizeof(uint16_t);
//        uint16_t cccd_bits = notify ? 1 : 0;
//        if (client_attr_write(conn_id, simp_client, GATT_WRITE_TYPE_REQ, handle,
//                              length, (uint8_t *)&cccd_bits) == GAP_CAUSE_SUCCESS)
//        {
//            return true;
//        }
//    }

//    APP_PRINT_WARN0("simp_ble_client_set_v3_notify: Request fail! Please check!");
//    return false;
//}

/**
  * @brief  Used by application, to enable or disable the indication of peer server's V4 Indicate Characteristic.
  * @param[in]  conn_id connection ID.
  * @param[in]  ind 0--disable the indication, 1--enable the indication.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
//bool simp_ble_client_set_v4_ind(uint8_t conn_id, bool ind)
//{
//    if (conn_id >= simp_link_num)
//    {
//        PROFILE_PRINT_ERROR1("simp_ble_client_set_v4_ind: failed invalid conn_id %d", conn_id);
//        return false;
//    }

//    if (simp_table[conn_id].hdl_cache[HDL_SIMBLE_V4_INDICATE_CCCD])
//    {
//        uint16_t handle = simp_table[conn_id].hdl_cache[HDL_SIMBLE_V4_INDICATE_CCCD];
//        uint16_t length = sizeof(uint16_t);
//        uint16_t cccd_bits = ind ? 2 : 0;
//        if (client_attr_write(conn_id, simp_client, GATT_WRITE_TYPE_REQ, handle,
//                              length, (uint8_t *)&cccd_bits) == GAP_CAUSE_SUCCESS)
//        {
//            return true;
//        }
//    }

//    APP_PRINT_WARN0("simp_ble_client_set_v4_ind: Request fail! Please check!");
//    return false;
//}

/**
  * @brief  Used by application, to write data of V2 write Characteristic.
  * @param[in]  conn_id connection ID.
  * @param[in]  length  write data length
  * @param[in]  p_value point the value to write
  * @param[in]  type    write type.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
bool ias_client_write_char(uint8_t conn_id, uint16_t length, uint8_t *p_value,
                           T_GATT_WRITE_TYPE type)
{
    if (conn_id >= ias_link_num)
    {
        PROFILE_PRINT_ERROR1("ias_client_write_v2_char: failed invalid conn_id %d", conn_id);
        return false;
    }

    if (ias_table[conn_id].hdl_cache[HDL_IAS_WRITE])
    {
        uint16_t handle = ias_table[conn_id].hdl_cache[HDL_IAS_WRITE];
        if (client_attr_write(conn_id, ias_client, type, handle,
                              length, p_value) == GAP_CAUSE_SUCCESS)
        {
            return true;
        }
    }

    APP_PRINT_WARN0("ias_ble_client_write_char: Request fail! Please check!");
    return false;
}

/**
  * @brief  Used by application, to get handle cache.
  * @param[in]  conn_id connection ID.
  * @param[in]  p_hdl_cache pointer of the handle cache table
  * @param[in]  len the length of handle cache table
  * @retval true success.
  * @retval false failed.
  */
bool ias_client_get_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len)
{
    if (conn_id >= ias_link_num)
    {
        PROFILE_PRINT_ERROR1("ias_client_get_hdl_cache: failed invalid conn_id %d", conn_id);
        return false;
    }
    if (ias_table[conn_id].disc_state != DISC_IAS_DONE)
    {
        PROFILE_PRINT_ERROR1("ias_client_get_hdl_cache: failed invalid state %d",
                             ias_table[conn_id].disc_state);
        return false;
    }
    if (len != sizeof(uint16_t) * HDL_IAS_CACHE_LEN)
    {
        PROFILE_PRINT_ERROR1("ias_client_get_hdl_cache: failed invalid len %d", len);
        return false;
    }
    memcpy(p_hdl_cache, ias_table[conn_id].hdl_cache, len);
    return true;
}

/**
  * @brief  Used by application, to set handle cache.
  * @param[in]  conn_id connection ID.
  * @param[in]  p_hdl_cache pointer of the handle cache table
  * @param[in]  len the length of handle cache table
  * @retval true success.
  * @retval false failed.
  */
bool ias_client_set_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len)
{
    if (conn_id >= ias_link_num)
    {
        PROFILE_PRINT_ERROR1("ias_client_set_hdl_cache: failed invalid conn_id %d", conn_id);
        return false;
    }
    if (ias_table[conn_id].disc_state != DISC_IAS_IDLE)
    {
        PROFILE_PRINT_ERROR1("ias_client_set_hdl_cache: failed invalid state %d",
                             ias_table[conn_id].disc_state);
        return false;
    }
    if (len != sizeof(uint16_t) * HDL_IAS_CACHE_LEN)
    {
        PROFILE_PRINT_ERROR1("ias_client_set_hdl_cache: failed invalid len %d", len);
        return false;
    }
    memcpy(ias_table[conn_id].hdl_cache, p_hdl_cache, len);
    ias_table[conn_id].disc_state = DISC_IAS_DONE;
    return true;
}

static bool ias_client_start_char_discovery(uint8_t conn_id)
{
    uint16_t start_handle;
    uint16_t end_handle;

    APP_PRINT_INFO0("ias_client_start_ias_char_discovery");
    start_handle = ias_table[conn_id].hdl_cache[HDL_IAS_SRV_START];
    end_handle = ias_table[conn_id].hdl_cache[HDL_IAS_SRV_END];
    if (client_all_char_discovery(conn_id, ias_client, start_handle,
                                  end_handle) == GAP_CAUSE_SUCCESS)
    {
        return true;
    }
    return false;
}

//static bool ias_client_start_char_descriptor_discovery(uint8_t conn_id)
//{
//    uint16_t start_handle;
//    uint16_t end_handle;

//    PROFILE_PRINT_INFO0("ias_client_start_char_descriptor_discovery");
//    start_handle = simp_table[conn_id].hdl_cache[HDL_SIMBLE_V3_NOTIFY];
//    end_handle = simp_table[conn_id].hdl_cache[HDL_SIMBLE_SRV_END];
//    if (client_all_char_descriptor_discovery(conn_id, simp_client, start_handle,
//                                             end_handle) == GAP_CAUSE_SUCCESS)
//    {
//        return true;
//    }
//    return false;
//}
static void ias_client_discover_state_cb(uint8_t conn_id,  T_DISCOVERY_STATE discovery_state)
{
    bool cb_flag = false;
    T_IAS_CLIENT_CB_DATA cb_data;
    cb_data.cb_type = IAS_CLIENT_CB_TYPE_DISC_STATE;

    APP_PRINT_INFO1("ias_client_discover_state_cb: discovery_state %d", discovery_state);
    if (ias_table[conn_id].disc_state == DISC_IAS_START)
    {
        uint16_t *hdl_cache;
        hdl_cache = ias_table[conn_id].hdl_cache;

        switch (discovery_state)
        {
        case DISC_STATE_SRV_DONE:
            /* Indicate that service handle found. Start discover characteristic. */
            if ((hdl_cache[HDL_IAS_SRV_START] != 0)
                || (hdl_cache[HDL_IAS_SRV_END] != 0))
            {
                if (ias_client_start_char_discovery(conn_id) == false)
                {
                    ias_table[conn_id].disc_state = DISC_IAS_FAILED;
                    cb_flag = true;
                }
            }
            /* No Ias BLE service handle found. Discover procedure complete. */
            else
            {
                ias_table[conn_id].disc_state = DISC_IAS_FAILED;
                cb_flag = true;
            }
            break;
        case DISC_STATE_CHAR_DONE:
            ias_table[conn_id].disc_state = DISC_IAS_DONE;
            cb_flag = true;
            break;
        case DISC_STATE_FAILED:
            ias_table[conn_id].disc_state = DISC_IAS_FAILED;
            cb_flag = true;
            break;
        default:
            APP_PRINT_ERROR0("ias_handle_discover_state: Invalid Discovery State!");
            break;
        }
    }

    /* Send discover state to application if needed. */
    if (cb_flag && ias_client_cb)
    {
        cb_data.cb_content.disc_state = ias_table[conn_id].disc_state;
        (*ias_client_cb)(ias_client, conn_id, &cb_data);
    }
    return;
}

/**
  * @brief  Called by profile client layer, when discover result fetched.
  * @param  conn_id: connection ID.
  * @param  result_type: indicate which type of value discovered in service discovery procedure.
  * @param  result_data: value discovered.
  * @retval None
  */
static void ias_client_discover_result_cb(uint8_t conn_id,
                                          T_DISCOVERY_RESULT_TYPE result_type,
                                          T_DISCOVERY_RESULT_DATA result_data)
{
    APP_PRINT_INFO1("ias_client_discover_result_cb: result_type %d", result_type);
    if (ias_table[conn_id].disc_state == DISC_IAS_START)
    {
        uint16_t handle;
        uint16_t *hdl_cache;
        hdl_cache = ias_table[conn_id].hdl_cache;

        switch (result_type)
        {
        case DISC_RESULT_SRV_DATA:
            hdl_cache[HDL_IAS_SRV_START] = result_data.p_srv_disc_data->att_handle;
            hdl_cache[HDL_IAS_SRV_END] = result_data.p_srv_disc_data->end_group_handle;
            break;

        case DISC_RESULT_CHAR_UUID16:
            handle = result_data.p_char_uuid16_disc_data->value_handle;
            switch (result_data.p_char_uuid16_disc_data->uuid16)
            {
            case GATT_UUID_CHAR_ALERT_LEVEL:
                hdl_cache[HDL_IAS_WRITE] = handle;
                break;
            default:
                /* have no intrest on this handle. */
                break;
            }
            break;

        case DISC_RESULT_CHAR_DESC_UUID16:
            /* When use client_all_char_descriptor_discovery. */
            if (result_data.p_char_desc_uuid16_disc_data->uuid16 == GATT_UUID_CHAR_CLIENT_CONFIG)
            {
                handle = result_data.p_char_desc_uuid16_disc_data->handle;
            }
            break;

        default:
            APP_PRINT_ERROR0("ias_handle_discover_result: Invalid Discovery Result Type!");
            break;
        }
    }
    return;
}

static void ias_client_write_result_cb(uint8_t conn_id, T_GATT_WRITE_TYPE type,
                                       uint16_t handle, uint16_t cause,
                                       uint8_t credits)
{
    T_IAS_CLIENT_CB_DATA cb_data;
    uint16_t *hdl_cache;
    hdl_cache = ias_table[conn_id].hdl_cache;
    cb_data.cb_type = IAS_CLIENT_CB_TYPE_WRITE_RESULT;

    PROFILE_PRINT_INFO2("ias_client_write_result_cb: handle 0x%x, cause 0x%x", handle, cause);
    cb_data.cb_content.write_result.cause = cause;

    if (handle == hdl_cache[HDL_IAS_WRITE])
    {
        cb_data.cb_content.write_result.type = IAS_WRITE_ALERT;
    }
    else
    {
        return;
    }
    /* Inform application the write result. */
    if (ias_client_cb)
    {
        (*ias_client_cb)(ias_client, conn_id, &cb_data);
    }

    return;
}


static void ias_client_disconnect_cb(uint8_t conn_id)
{
    APP_PRINT_INFO0("ias_client_disconnect_cb.");
    if (conn_id >= ias_link_num)
    {
        PROFILE_PRINT_ERROR1("ias_client_disconnect_cb: failed invalid conn_id %d", conn_id);
        return;
    }
    memset(&ias_table[conn_id], 0, sizeof(T_IAS_LINK));
    return;
}

/**
 * @brief Ias BLE Client Callbacks.
*/
const T_FUN_CLIENT_CBS ias_client_cbs =
{
    ias_client_discover_state_cb,   //!< Discovery State callback function pointer
    ias_client_discover_result_cb,  //!< Discovery result callback function pointer
    NULL,                           //!< Read response callback function pointer
    ias_client_write_result_cb,     //!< Write result callback function pointer
    NULL,                           //!< Notify Indicate callback function pointer
    ias_client_disconnect_cb        //!< Link disconnection callback function pointer
};

/**
  * @brief      Add ias service client to application.
  * @param[in]  app_cb pointer of app callback function to handle specific client module data.
  * @param[in]  link_num initialize link num.
  * @return Client ID of the specific client module.
  * @retval 0xff failed.
  * @retval other success.
  *
  * <b>Example usage</b>
  * \code{.c}
    void app_le_profile_init(void)
    {
        client_init(1);
        ias_client_id = ias_ble_add_client(app_client_callback, APP_MAX_LINKS);
    }
  * \endcode
  */
T_CLIENT_ID ias_add_client(P_FUN_GENERAL_APP_CB app_cb, uint8_t link_num)
{
    uint16_t size;
    if (link_num > IAS_MAX_LINKS)
    {
        PROFILE_PRINT_ERROR1("ias_add_client: invalid link_num %d", link_num);
        return 0xff;
    }
    if (false == client_register_spec_client_cb(&ias_client, &ias_client_cbs))
    {
        ias_client = CLIENT_PROFILE_GENERAL_ID;
        APP_PRINT_ERROR0("ias_add_client failed");
        return ias_client;
    }
    APP_PRINT_INFO1("ias_add_client: ias_client %d", ias_client);

    /* register callback for profile to inform application that some events happened. */
    ias_client_cb = app_cb;
    ias_link_num = link_num;
    size = ias_link_num * sizeof(T_IAS_LINK);
    ias_table = os_mem_zalloc(RAM_TYPE_DATA_ON, size);

    return ias_client;
}

