/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    dis_client.c
  * @brief   Dis BLE client source file.
  * @details
  * @author  ken
  * @date    2017-12-05
  * @version v1.0
  ******************************************************************************
  */

/** Add Includes here **/
#include <trace.h>
#include <string.h>
#include <dis_client.h>
#include <os_mem.h>

/********************************************************************************************************
* local static variables defined here, only used in this source file.
********************************************************************************************************/

typedef struct
{
    T_DIS_DISC_STATE disc_state;
    uint16_t          hdl_cache[HDL_DIS_CACHE_LEN];
} T_DIS_LINK, *P_DIS_LINK;

static P_DIS_LINK dis_table;
static uint8_t dis_link_num;

/**<  dis client ID. */
static T_CLIENT_ID dis_client = CLIENT_PROFILE_GENERAL_ID;
/**<  Callback used to send data to app from dis client layer. */
static P_FUN_GENERAL_APP_CB dis_client_cb = NULL;

/**
  * @brief  Used by application, to start the discovery procedure of lls server.
  * @param[in]  conn_id connection ID.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
bool dis_client_start_discovery(uint8_t conn_id)
{
    PROFILE_PRINT_INFO0("dis_client_start_discovery");
    if (conn_id >= dis_link_num)
    {
        PROFILE_PRINT_ERROR1("dis_client_start_discovery: failed invalid conn_id %d", conn_id);
        return false;
    }
    /* First clear handle cache. */
    memset(&dis_table[conn_id], 0, sizeof(T_DIS_LINK));
    dis_table[conn_id].disc_state = DISC_DIS_START;
    if (client_by_uuid_srv_discovery(conn_id, dis_client,
                                     GATT_UUID_DEVICE_INFORMATION_SERVICE) == GAP_CAUSE_SUCCESS)
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
bool dis_client_read_by_handle(uint8_t conn_id, T_DIS_READ_TYPE read_type)
{
    bool hdl_valid = false;
    uint16_t  handle;
    if (conn_id >= dis_link_num)
    {
        PROFILE_PRINT_ERROR1("dis_client_read_by_handle: failed invalid conn_id %d", conn_id);
        return false;
    }

    switch (read_type)
    {
    case DIS_READ_SYSTEM_ID:
        if (dis_table[conn_id].hdl_cache[HDL_DIS_SYSTEM_ID])
        {
            handle = dis_table[conn_id].hdl_cache[HDL_DIS_SYSTEM_ID];
            hdl_valid = true;
        }
        break;
    case DIS_READ_MODEL_NUMBER:
        if (dis_table[conn_id].hdl_cache[HDL_DIS_MODEL_NUMBER])
        {
            handle = dis_table[conn_id].hdl_cache[HDL_DIS_MODEL_NUMBER];
            hdl_valid = true;
        }
        break;
    case DIS_READ_SERIAL_NUMBER:
        if (dis_table[conn_id].hdl_cache[HDL_DIS_SERIAL_NUMBER])
        {
            handle = dis_table[conn_id].hdl_cache[HDL_DIS_SERIAL_NUMBER];
            hdl_valid = true;
        }
        break;
    case DIS_READ_FIRMWARE_REVISION:
        if (dis_table[conn_id].hdl_cache[HDL_DIS_FIRMWARE_REVISION])
        {
            handle = dis_table[conn_id].hdl_cache[HDL_DIS_FIRMWARE_REVISION];
            hdl_valid = true;
        }
        break;
    case DIS_READ_HARDWARE_REVISION:
        if (dis_table[conn_id].hdl_cache[HDL_DIS_HARDWARE_REVISION])
        {
            handle = dis_table[conn_id].hdl_cache[HDL_DIS_HARDWARE_REVISION];
            hdl_valid = true;
        }
        break;
    case DIS_READ_SOFTWARE_REVISION:
        if (dis_table[conn_id].hdl_cache[HDL_DIS_SOFTWARE_REVISION])
        {
            handle = dis_table[conn_id].hdl_cache[HDL_DIS_SOFTWARE_REVISION];
            hdl_valid = true;
        }
        break;
    case DIS_READ_MANUFACTURER_NAME:
        if (dis_table[conn_id].hdl_cache[HDL_DIS_MANUFACTURER_NAME])
        {
            handle = dis_table[conn_id].hdl_cache[HDL_DIS_MANUFACTURER_NAME];
            hdl_valid = true;
        }
        break;
    case DIS_READ_IEEE_CERTIF_DATA_LIST:
        if (dis_table[conn_id].hdl_cache[HDL_DIS_IEEE_CERTIF_DATA_LIST])
        {
            handle = dis_table[conn_id].hdl_cache[HDL_DIS_IEEE_CERTIF_DATA_LIST];
            hdl_valid = true;
        }
        break;
    case DIS_READ_PNP_ID:
        if (dis_table[conn_id].hdl_cache[HDL_DIS_PNP_ID])
        {
            handle = dis_table[conn_id].hdl_cache[HDL_DIS_PNP_ID];
            hdl_valid = true;
        }
        break;
    default:
        return false;
    }

    if (hdl_valid)
    {
        if (client_attr_read(conn_id, dis_client, handle) == GAP_CAUSE_SUCCESS)
        {
            return true;
        }
    }

    APP_PRINT_WARN0("dis_client_read_by_handle: Request fail! Please check!");
    return false;
}

/**
  * @brief  Used by application, to read data from server by using UUIDs.
  * @param[in]  conn_id connection ID.
  * @param[in]  read_type one of characteristic that has the readable property.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
bool dis_client_read_by_uuid(uint8_t conn_id,
                             T_DIS_READ_TYPE read_type)//~~~~~~~~~~~~~~~~~~will delete
{
    uint16_t start_handle;
    uint16_t end_handle;
    uint16_t  uuid16;
    if (conn_id >= dis_link_num)
    {
        PROFILE_PRINT_ERROR1("dis_client_read_by_uuid: failed invalid conn_id %d", conn_id);
        return false;
    }

    switch (read_type)
    {
    case DIS_READ_SYSTEM_ID:
        start_handle = dis_table[conn_id].hdl_cache[HDL_DIS_SRV_START];
        end_handle = dis_table[conn_id].hdl_cache[HDL_DIS_SRV_END];
        uuid16 = GATT_UUID_CHAR_SYSTEM_ID;
        break;
    case DIS_READ_MODEL_NUMBER:
        start_handle = dis_table[conn_id].hdl_cache[HDL_DIS_SRV_START];
        end_handle = dis_table[conn_id].hdl_cache[HDL_DIS_SRV_END];
        uuid16 = GATT_UUID_CHAR_MODEL_NUMBER;
        break;
    case DIS_READ_SERIAL_NUMBER:
        start_handle = dis_table[conn_id].hdl_cache[HDL_DIS_SRV_START];
        end_handle = dis_table[conn_id].hdl_cache[HDL_DIS_SRV_END];
        uuid16 = GATT_UUID_CHAR_SERIAL_NUMBER;
        break;
    case DIS_READ_FIRMWARE_REVISION:
        start_handle = dis_table[conn_id].hdl_cache[HDL_DIS_SRV_START];
        end_handle = dis_table[conn_id].hdl_cache[HDL_DIS_SRV_END];
        uuid16 = GATT_UUID_CHAR_FIRMWARE_REVISION;
        break;
    case DIS_READ_HARDWARE_REVISION:
        start_handle = dis_table[conn_id].hdl_cache[HDL_DIS_SRV_START];
        end_handle = dis_table[conn_id].hdl_cache[HDL_DIS_SRV_END];
        uuid16 = GATT_UUID_CHAR_HARDWARE_REVISION;
        break;
    case DIS_READ_SOFTWARE_REVISION:
        start_handle = dis_table[conn_id].hdl_cache[HDL_DIS_SRV_START];
        end_handle = dis_table[conn_id].hdl_cache[HDL_DIS_SRV_END];
        uuid16 = GATT_UUID_CHAR_SOFTWARE_REVISION;
        break;
    case DIS_READ_MANUFACTURER_NAME:
        start_handle = dis_table[conn_id].hdl_cache[HDL_DIS_SRV_START];
        end_handle = dis_table[conn_id].hdl_cache[HDL_DIS_SRV_END];
        uuid16 = GATT_UUID_CHAR_MANUFACTURER_NAME;
        break;
    case DIS_READ_IEEE_CERTIF_DATA_LIST:
        start_handle = dis_table[conn_id].hdl_cache[HDL_DIS_SRV_START];
        end_handle = dis_table[conn_id].hdl_cache[HDL_DIS_SRV_END];
        uuid16 = GATT_UUID_CHAR_IEEE_CERTIF_DATA_LIST;
        break;
    case DIS_READ_PNP_ID:
        start_handle = dis_table[conn_id].hdl_cache[HDL_DIS_SRV_START];
        end_handle = dis_table[conn_id].hdl_cache[HDL_DIS_SRV_END];
        uuid16 = GATT_UUID_CHAR_PNP_ID;
        break;

    default:
        return false;
    }

    if (client_attr_read_using_uuid(conn_id, dis_client, start_handle, end_handle,
                                    uuid16, NULL) == GAP_CAUSE_SUCCESS)
    {
        return true;
    }
    return false;
}

///**
//  * @brief  Used by application, to enable or disable the notification of peer server's V3 Notify Characteristic.
//  * @param[in]  conn_id connection ID.
//  * @param[in]  notify 0--disable the notification, 1--enable the notification.
//  * @retval true send request to upper stack success.
//  * @retval false send request to upper stack failed.
//  */
//bool lls_client_set_v3_notify(uint8_t conn_id, bool notify)
//{
//    if (conn_id >= kns_link_num)
//    {
//        PROFILE_PRINT_ERROR1("kns_client_set_v3_notify: failed invalid conn_id %d", conn_id);
//        return false;
//    }

//    if (kns_table[conn_id].hdl_cache[HDL_KNS_NOTIFY_KEY_CCCD])
//    {
//        uint16_t handle = kns_table[conn_id].hdl_cache[HDL_KNS_NOTIFY_KEY_CCCD];
//        uint16_t length = sizeof(uint16_t);
//        uint16_t cccd_bits = notify ? 1 : 0;
//        if (client_attr_write(conn_id, kns_client, GATT_WRITE_TYPE_REQ, handle,
//                              length, (uint8_t *)&cccd_bits) == GAP_CAUSE_SUCCESS)
//        {
//            return true;
//        }
//    }

//    APP_PRINT_WARN0("kns_client_set_v3_notify: Request fail! Please check!");
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
  * @brief  Used by application, to write data of  write Characteristic.
  * @param[in]  conn_id connection ID.
  * @param[in]  length  write data length
  * @param[in]  p_value point the value to write
  * @param[in]  type    write type.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
//bool lls_client_write_char(uint8_t conn_id, uint16_t length, uint8_t *p_value,
//                                   T_GATT_WRITE_TYPE type)
//{
//    if (conn_id >= lls_link_num)
//    {
//        PROFILE_PRINT_ERROR1("lls_client_write_char: failed invalid conn_id %d", conn_id);
//        return false;
//    }

//    if (lls_table[conn_id].hdl_cache[HDL_LLS_PARA])
//    {
//        uint16_t handle = lls_table[conn_id].hdl_cache[HDL_LLS_PARA];
//        if (client_attr_write(conn_id, lls_client, type, handle,
//                              length, p_value) == GAP_CAUSE_SUCCESS)
//        {
//            return true;
//        }
//    }

//    APP_PRINT_WARN0("lls_ble_client_write_char: Request fail! Please check!");
//    return false;
//}

/**
  * @brief  Used by application, to get handle cache.
  * @param[in]  conn_id connection ID.
  * @param[in]  p_hdl_cache pointer of the handle cache table
  * @param[in]  len the length of handle cache table
  * @retval true success.
  * @retval false failed.
  */
bool dis_client_get_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len)
{
    if (conn_id >= dis_link_num)
    {
        PROFILE_PRINT_ERROR1("dis_client_get_hdl_cache: failed invalid conn_id %d", conn_id);
        return false;
    }
    if (dis_table[conn_id].disc_state != DISC_DIS_DONE)
    {
        PROFILE_PRINT_ERROR1("dis_client_get_hdl_cache: failed invalid state %d",
                             dis_table[conn_id].disc_state);
        return false;
    }
    if (len != sizeof(uint16_t) * HDL_DIS_CACHE_LEN)
    {
        PROFILE_PRINT_ERROR1("dis_client_get_hdl_cache: failed invalid len %d", len);
        return false;
    }
    memcpy(p_hdl_cache, dis_table[conn_id].hdl_cache, len);
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
bool dis_client_set_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len)
{
    if (conn_id >= dis_link_num)
    {
        PROFILE_PRINT_ERROR1("dis_client_set_hdl_cache: failed invalid conn_id %d", conn_id);
        return false;
    }
    if (dis_table[conn_id].disc_state != DISC_DIS_IDLE)
    {
        PROFILE_PRINT_ERROR1("lls_client_set_hdl_cache: failed invalid state %d",
                             dis_table[conn_id].disc_state);
        return false;
    }
    if (len != sizeof(uint16_t) * HDL_DIS_CACHE_LEN)
    {
        PROFILE_PRINT_ERROR1("dis_client_set_hdl_cache: failed invalid len %d", len);
        return false;
    }
    memcpy(dis_table[conn_id].hdl_cache, p_hdl_cache, len);
    dis_table[conn_id].disc_state = DISC_DIS_DONE;
    return true;
}

static bool dis_client_start_char_discovery(uint8_t conn_id)
{
    uint16_t start_handle;
    uint16_t end_handle;

    APP_PRINT_INFO0("dis_client_start_ias_char_discovery");
    start_handle = dis_table[conn_id].hdl_cache[HDL_DIS_SRV_START];
    end_handle = dis_table[conn_id].hdl_cache[HDL_DIS_SRV_END];
    if (client_all_char_discovery(conn_id, dis_client, start_handle,
                                  end_handle) == GAP_CAUSE_SUCCESS)
    {
        return true;
    }
    return false;
}

//static bool lls_client_start_char_descriptor_discovery(uint8_t conn_id)
//{
//    uint16_t start_handle;
//    uint16_t end_handle;

//    PROFILE_PRINT_INFO0("lls_client_start_char_descriptor_discovery");
//    start_handle = lls_table[conn_id].hdl_cache[HDL_LLS_NOTIFY_KEY];
//    end_handle = lls_table[conn_id].hdl_cache[HDL_LLS_SRV_END];
//    if (client_all_char_descriptor_discovery(conn_id, lls_client, start_handle,
//                                             end_handle) == GAP_CAUSE_SUCCESS)
//    {
//        return true;
//    }
//    return false;
//}
static void dis_client_discover_state_cb(uint8_t conn_id,  T_DISCOVERY_STATE discovery_state)
{
    bool cb_flag = false;
    T_DIS_CLIENT_CB_DATA cb_data;
    cb_data.cb_type = DIS_CLIENT_CB_TYPE_DISC_STATE;

    APP_PRINT_INFO1("dis_client_discover_state_cb: discovery_state %d", discovery_state);
    if (dis_table[conn_id].disc_state == DISC_DIS_START)
    {
        uint16_t *hdl_cache;
        hdl_cache = dis_table[conn_id].hdl_cache;

        switch (discovery_state)
        {
        case DISC_STATE_SRV_DONE:
            /* Indicate that service handle found. Start discover characteristic. */
            if ((hdl_cache[HDL_DIS_SRV_START] != 0)
                || (hdl_cache[HDL_DIS_SRV_END] != 0))
            {
                if (dis_client_start_char_discovery(conn_id) == false)
                {
                    dis_table[conn_id].disc_state = DISC_DIS_FAILED;
                    cb_flag = true;
                }
            }
            /* No Ias BLE service handle found. Discover procedure complete. */
            else
            {
                dis_table[conn_id].disc_state = DISC_DIS_FAILED;
                cb_flag = true;
            }
            break;
        case DISC_STATE_CHAR_DONE:
            dis_table[conn_id].disc_state = DISC_DIS_DONE;
            cb_flag = true;
            break;
        case DISC_STATE_FAILED:
            dis_table[conn_id].disc_state = DISC_DIS_FAILED;
            cb_flag = true;
            break;
        default:
            APP_PRINT_ERROR0("dis_handle_discover_state: Invalid Discovery State!");
            break;
        }
    }

    /* Send discover state to application if needed. */
    if (cb_flag && dis_client_cb)
    {
        cb_data.cb_content.disc_state = dis_table[conn_id].disc_state;
        (*dis_client_cb)(dis_client, conn_id, &cb_data);
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
static void dis_client_discover_result_cb(uint8_t conn_id,
                                          T_DISCOVERY_RESULT_TYPE result_type,
                                          T_DISCOVERY_RESULT_DATA result_data)
{
    APP_PRINT_INFO1("dis_client_discover_result_cb: result_type %d", result_type);
    if (dis_table[conn_id].disc_state == DISC_DIS_START)
    {
        uint16_t handle;
        uint16_t *hdl_cache;
        hdl_cache = dis_table[conn_id].hdl_cache;

        switch (result_type)
        {
        case DISC_RESULT_SRV_DATA:
            hdl_cache[HDL_DIS_SRV_START] = result_data.p_srv_disc_data->att_handle;
            hdl_cache[HDL_DIS_SRV_END] = result_data.p_srv_disc_data->end_group_handle;
            break;
        case DISC_RESULT_CHAR_UUID128:
            break;

        case DISC_RESULT_CHAR_UUID16:
            handle = result_data.p_char_uuid16_disc_data->value_handle;
            switch (result_data.p_char_uuid16_disc_data->uuid16)
            {
            case GATT_UUID_CHAR_SYSTEM_ID:
                hdl_cache[HDL_DIS_SYSTEM_ID] = handle;
                break;
            case GATT_UUID_CHAR_MODEL_NUMBER:
                hdl_cache[HDL_DIS_MODEL_NUMBER] = handle;
                break;
            case GATT_UUID_CHAR_SERIAL_NUMBER:
                hdl_cache[HDL_DIS_SERIAL_NUMBER] = handle;
                break;
            case GATT_UUID_CHAR_FIRMWARE_REVISION:
                hdl_cache[HDL_DIS_FIRMWARE_REVISION] = handle;
                break;
            case GATT_UUID_CHAR_HARDWARE_REVISION:
                hdl_cache[HDL_DIS_HARDWARE_REVISION] = handle;
                break;
            case GATT_UUID_CHAR_SOFTWARE_REVISION:
                hdl_cache[HDL_DIS_SOFTWARE_REVISION] = handle;
                break;
            case GATT_UUID_CHAR_MANUFACTURER_NAME:
                hdl_cache[HDL_DIS_MANUFACTURER_NAME] = handle;
                break;
            case GATT_UUID_CHAR_IEEE_CERTIF_DATA_LIST:
                hdl_cache[HDL_DIS_IEEE_CERTIF_DATA_LIST] = handle;
                break;
            case GATT_UUID_CHAR_PNP_ID:
                hdl_cache[HDL_DIS_PNP_ID] = handle;
                break;
            default:
                /* have no intrest on this handle. */
                break;
            }
            break;

        case DISC_RESULT_CHAR_DESC_UUID16:
            /* When use client_all_char_descriptor_discovery. */
            break;

        default:
            APP_PRINT_ERROR0("lls_handle_discover_result: Invalid Discovery Result Type!");
            break;
        }
    }

    return;
}

static void dis_client_read_result_cb(uint8_t conn_id,  uint16_t cause,
                                      uint16_t handle, uint16_t value_size, uint8_t *p_value)
{
    T_DIS_CLIENT_CB_DATA cb_data;
    uint16_t *hdl_cache;
    hdl_cache = dis_table[conn_id].hdl_cache;

    cb_data.cb_type = DIS_CLIENT_CB_TYPE_READ_RESULT;

    APP_PRINT_INFO2("dis_client_read_result_cb: handle 0x%x, cause 0x%x", handle, cause);
    cb_data.cb_content.read_result.cause = cause;

    if (handle == hdl_cache[HDL_DIS_SYSTEM_ID])
    {
        cb_data.cb_content.read_result.type = DIS_READ_SYSTEM_ID;
        if (cause == GAP_SUCCESS)
        {
            cb_data.cb_content.read_result.data.v1_read.p_value = p_value;
            cb_data.cb_content.read_result.data.v1_read.value_size = value_size;
        }
        else
        {
            cb_data.cb_content.read_result.data.v1_read.value_size = 0;
        }
    }
    else if (handle == hdl_cache[HDL_DIS_MODEL_NUMBER])
    {
        cb_data.cb_content.read_result.type = DIS_READ_MODEL_NUMBER;
        if (cause == GAP_SUCCESS)
        {
            cb_data.cb_content.read_result.data.v1_read.p_value = p_value;
            cb_data.cb_content.read_result.data.v1_read.value_size = value_size;
        }
        else
        {
            cb_data.cb_content.read_result.data.v1_read.value_size = 0;
        }
    }
    else if (handle == hdl_cache[HDL_DIS_SERIAL_NUMBER])
    {
        cb_data.cb_content.read_result.type = DIS_READ_SERIAL_NUMBER;
        if (cause == GAP_SUCCESS)
        {
            cb_data.cb_content.read_result.data.v1_read.p_value = p_value;
            cb_data.cb_content.read_result.data.v1_read.value_size = value_size;
        }
        else
        {
            cb_data.cb_content.read_result.data.v1_read.value_size = 0;
        }
    }
    else if (handle == hdl_cache[HDL_DIS_FIRMWARE_REVISION])
    {
        cb_data.cb_content.read_result.type = DIS_READ_FIRMWARE_REVISION;
        if (cause == GAP_SUCCESS)
        {
            cb_data.cb_content.read_result.data.v1_read.p_value = p_value;
            cb_data.cb_content.read_result.data.v1_read.value_size = value_size;
        }
        else
        {
            cb_data.cb_content.read_result.data.v1_read.value_size = 0;
        }
    }
    else if (handle == hdl_cache[HDL_DIS_HARDWARE_REVISION])
    {
        cb_data.cb_content.read_result.type = DIS_READ_HARDWARE_REVISION;
        if (cause == GAP_SUCCESS)
        {
            cb_data.cb_content.read_result.data.v1_read.p_value = p_value;
            cb_data.cb_content.read_result.data.v1_read.value_size = value_size;
        }
        else
        {
            cb_data.cb_content.read_result.data.v1_read.value_size = 0;
        }
    }
    else if (handle == hdl_cache[HDL_DIS_SOFTWARE_REVISION])
    {
        cb_data.cb_content.read_result.type = DIS_READ_SOFTWARE_REVISION;
        if (cause == GAP_SUCCESS)
        {
            cb_data.cb_content.read_result.data.v1_read.p_value = p_value;
            cb_data.cb_content.read_result.data.v1_read.value_size = value_size;
        }
        else
        {
            cb_data.cb_content.read_result.data.v1_read.value_size = 0;
        }
    }
    else if (handle == hdl_cache[HDL_DIS_MANUFACTURER_NAME])
    {
        cb_data.cb_content.read_result.type = DIS_READ_MANUFACTURER_NAME;
        if (cause == GAP_SUCCESS)
        {
            cb_data.cb_content.read_result.data.v1_read.p_value = p_value;
            cb_data.cb_content.read_result.data.v1_read.value_size = value_size;
        }
        else
        {
            cb_data.cb_content.read_result.data.v1_read.value_size = 0;
        }
    }
    else if (handle == hdl_cache[HDL_DIS_IEEE_CERTIF_DATA_LIST])
    {
        cb_data.cb_content.read_result.type = DIS_READ_IEEE_CERTIF_DATA_LIST;
        if (cause == GAP_SUCCESS)
        {
            cb_data.cb_content.read_result.data.v1_read.p_value = p_value;
            cb_data.cb_content.read_result.data.v1_read.value_size = value_size;
        }
        else
        {
            cb_data.cb_content.read_result.data.v1_read.value_size = 0;
        }
    }
    else if (handle == hdl_cache[HDL_DIS_PNP_ID])
    {
        cb_data.cb_content.read_result.type = DIS_READ_PNP_ID;
        if (cause == GAP_SUCCESS)
        {
            cb_data.cb_content.read_result.data.v1_read.p_value = p_value;
            cb_data.cb_content.read_result.data.v1_read.value_size = value_size;
        }
        else
        {
            cb_data.cb_content.read_result.data.v1_read.value_size = 0;
        }
    }
    else
    {
        return;
    }
    /* Inform application the read result. */
    if (dis_client_cb)
    {
        (*dis_client_cb)(dis_client, conn_id, &cb_data);
    }

    return;
}

//static void dis_client_write_result_cb(uint8_t conn_id, T_GATT_WRITE_TYPE type,
//                                            uint16_t handle, uint16_t cause,
//                                            uint8_t credits)
//{
//    T_LLS_CLIENT_CB_DATA cb_data;
//    uint16_t *hdl_cache;
//    hdl_cache = dis_table[conn_id].hdl_cache;
//    cb_data.cb_type = DIS_CLIENT_CB_TYPE_WRITE_RESULT;

//    PROFILE_PRINT_INFO2("dis_client_write_result_cb: handle 0x%x, cause 0x%x", handle, cause);
//    cb_data.cb_content.write_result.cause = cause;

//    if (handle == hdl_cache[HDL_DIS_PARA])
//    {
//        cb_data.cb_content.write_result.type = DIS_WRITE_PARA;
//    }
//    else
//    {
//        return;
//    }
//    /* Inform application the write result. */
//    if (dis_client_cb)
//    {
//        (*dis_client_cb)(lls_client, conn_id, &cb_data);
//    }

//    return;
//}

//static T_APP_RESULT lls_client_notif_ind_result_cb(uint8_t conn_id, bool notify,
//                                                        uint16_t handle,
//                                                        uint16_t value_size, uint8_t *p_value)
//{
//    T_APP_RESULT app_result = APP_RESULT_SUCCESS;
//    T_KNS_CLIENT_CB_DATA cb_data;
//    uint16_t *hdl_cache;
//    hdl_cache = kns_table[conn_id].hdl_cache;

//    cb_data.cb_type = KNS_CLIENT_CB_TYPE_NOTIF_IND_RESULT;

//    if (handle == hdl_cache[HDL_KNS_NOTIFY_KEY])
//    {
//        cb_data.cb_content.notif_ind_data.type = KNS_KEY_NOTIFY;
//        cb_data.cb_content.notif_ind_data.data.value_size = value_size;
//        cb_data.cb_content.notif_ind_data.data.p_value = p_value;
//    }
//    else
//    {
//        return app_result;
//    }
//    /* Inform application the notif/ind result. */
//    if (kns_client_cb)
//    {
//        app_result = (*kns_client_cb)(kns_client, conn_id, &cb_data);
//    }

//    return app_result;
//}

static void dis_client_disconnect_cb(uint8_t conn_id)
{
    APP_PRINT_INFO0("dis_client_disconnect_cb.");
    if (conn_id >= dis_link_num)
    {
        PROFILE_PRINT_ERROR1("dis_client_disconnect_cb: failed invalid conn_id %d", conn_id);
        return;
    }
    memset(&dis_table[conn_id], 0, sizeof(T_DIS_LINK));
    return;
}

/**
 * @brief Ias BLE Client Callbacks.
*/
const T_FUN_CLIENT_CBS dis_client_cbs =
{
    dis_client_discover_state_cb,   //!< Discovery State callback function pointer
    dis_client_discover_result_cb,  //!< Discovery result callback function pointer
    dis_client_read_result_cb,      //!< Read response callback function pointer
    NULL,//lls_client_write_result_cb,     //!< Write result callback function pointer
    NULL,//kns_client_notif_ind_result_cb,  //!< Notify Indicate callback function pointer
    dis_client_disconnect_cb       //!< Link disconnection callback function pointer
};

/**
  * @brief      Add dis service client to application.
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
        dis_client_id = dis_ble_add_client(app_client_callback, APP_MAX_LINKS);
    }
  * \endcode
  */
T_CLIENT_ID dis_add_client(P_FUN_GENERAL_APP_CB app_cb, uint8_t link_num)
{
    uint16_t size;
    if (link_num > DIS_MAX_LINKS)
    {
        PROFILE_PRINT_ERROR1("dis_add_client: invalid link_num %d", link_num);
        return 0xff;
    }
    if (false == client_register_spec_client_cb(&dis_client, &dis_client_cbs))
    {
        dis_client = CLIENT_PROFILE_GENERAL_ID;
        APP_PRINT_ERROR0("dis_add_client failed");
        return dis_client;
    }
    APP_PRINT_INFO1("dis_add_client: dis_client %d", dis_client);

    /* register callback for profile to inform application that some events happened. */
    dis_client_cb = app_cb;
    dis_link_num = link_num;
    size = dis_link_num * sizeof(T_DIS_LINK);
    dis_table = os_mem_zalloc(RAM_TYPE_DATA_ON, size);

    return dis_client;
}

