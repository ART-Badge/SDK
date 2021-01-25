/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    tps_client.c
  * @brief
  * @details
  * @author  ken
  * @date    2017-12-06
  * @version v1.0
  ******************************************************************************
  */

/** Add Includes here **/
#include <string.h>
#include <tps_client.h>
#include <trace.h>
#include <os_mem.h>

/********************************************************************************************************
* local static variables defined here, only used in this source file.
********************************************************************************************************/

#define GATT_UUID_TX_POWER_SERVICE              0x1804
#define GATT_UUID_CHAR_TX_LEVEL                 0x2A07
/**
 * @brief  TPS client Link control block defination.
 */
typedef struct
{
    T_TPS_DISC_STATE disc_state;
    uint16_t         hdl_cache[HDL_TPS_CACHE_LEN];
} T_TPS_LINK, *P_TPS_LINK;

static P_TPS_LINK tps_table;
static uint8_t tps_link_num;

/**<  TPS client ID. */
static T_CLIENT_ID tps_client = CLIENT_PROFILE_GENERAL_ID;
/**<  Callback used to send data to app from TPS client layer. */
static P_FUN_GENERAL_APP_CB tps_client_cb = NULL;

/**
  * @brief  Used by application, to start the discovery procedure of battery service.
  * @param[in]  conn_id connection ID.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  *
  */
bool tps_start_discovery(uint8_t conn_id)
{
    PROFILE_PRINT_INFO0("tps_start_discovery");
    if (conn_id >= tps_link_num)
    {
        PROFILE_PRINT_ERROR1("bas_start_discovery: failed invalid conn_id %d", conn_id);
        return false;
    }
    /* First clear handle cache. */
    memset(&tps_table[conn_id], 0, sizeof(T_TPS_LINK));
    tps_table[conn_id].disc_state = DISC_TPS_START;
    if (client_by_uuid_srv_discovery(conn_id, tps_client,
                                     GATT_UUID_TX_POWER_SERVICE) == GAP_CAUSE_SUCCESS)
    {
        return true;
    }
    return false;
}

/**
  * @brief  Used by application, to read tx power level.
  * @param[in]  conn_id connection ID.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  *
  */
bool tps_read_power_level(uint8_t conn_id)
{
    if (conn_id >= tps_link_num)
    {
        PROFILE_PRINT_ERROR1("tps_read_battery_level: failed invalid conn_id %d", conn_id);
        return false;
    }
    if (tps_table[conn_id].hdl_cache[HDL_TPS_PARA])
    {
        uint16_t handle = tps_table[conn_id].hdl_cache[HDL_TPS_PARA];
        if (client_attr_read(conn_id, tps_client, handle) == GAP_CAUSE_SUCCESS)
        {
            return true;
        }
    }
    PROFILE_PRINT_ERROR0("tps_read_battery_level: false handle = 0");
    return false;
}

/**
  * @brief  Used by application, to get handle cache.
  * @param[in]  conn_id connection ID.
  * @param[in]  p_hdl_cache pointer of the handle cache table
  * @param[in]  len the length of handle cache table
  * @retval true success.
  * @retval false failed.
  *
  */
bool tps_get_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len)
{
    if (conn_id >= tps_link_num)
    {
        PROFILE_PRINT_ERROR1("tps_get_hdl_cache: failed invalid conn_id %d", conn_id);
        return false;
    }
    if (tps_table[conn_id].disc_state != DISC_TPS_DONE)
    {
        PROFILE_PRINT_ERROR1("tps_get_hdl_cache: failed invalid state %d", tps_table[conn_id].disc_state);
        return false;
    }
    if (len != sizeof(uint16_t) * HDL_TPS_CACHE_LEN)
    {
        PROFILE_PRINT_ERROR1("tps_get_hdl_cache: failed invalid len %d", len);
        return false;
    }
    memcpy(p_hdl_cache, tps_table[conn_id].hdl_cache, len);
    return true;
}

/**
  * @brief  Used by application, to set handle cache.
  * @param[in]  conn_id connection ID.
  * @param[in]  p_hdl_cache pointer of the handle cache table
  * @param[in]  len the length of handle cache table
  * @retval true success.
  * @retval false failed.
  *
  */
bool tps_set_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len)
{
    if (conn_id >= tps_link_num)
    {
        PROFILE_PRINT_ERROR1("tps_set_hdl_cache: failed invalid conn_id %d", conn_id);
        return false;
    }
    if (tps_table[conn_id].disc_state != DISC_TPS_IDLE)
    {
        PROFILE_PRINT_ERROR1("tps_set_hdl_cache: failed invalid state %d", tps_table[conn_id].disc_state);
        return false;
    }
    if (len != sizeof(uint16_t) * HDL_TPS_CACHE_LEN)
    {
        PROFILE_PRINT_ERROR1("tps_set_hdl_cache: failed invalid len %d", len);
        return false;
    }
    memcpy(tps_table[conn_id].hdl_cache, p_hdl_cache, len);
    tps_table[conn_id].disc_state = DISC_TPS_DONE;
    return true;
}

static bool tps_start_char_discovery(uint8_t conn_id)
{
    uint16_t start_handle;
    uint16_t end_handle;

    PROFILE_PRINT_INFO0("tps_start_char_discovery");
    start_handle = tps_table[conn_id].hdl_cache[HDL_TPS_SRV_START];
    end_handle = tps_table[conn_id].hdl_cache[HDL_TPS_SRV_END];
    if (client_all_char_discovery(conn_id, tps_client, start_handle,
                                  end_handle) == GAP_CAUSE_SUCCESS)
    {
        return true;
    }
    return false;
}

//static bool bas_start_char_descriptor_discovery(uint8_t conn_id)
//{
//    uint16_t start_handle;
//    uint16_t end_handle;

//    PROFILE_PRINT_INFO0("bas_start_char_descriptor_discovery");
//    start_handle = bas_table[conn_id].hdl_cache[HDL_BAS_BATTERY_LEVEL];
//    end_handle = bas_table[conn_id].hdl_cache[HDL_BAS_SRV_END];
//    if (client_all_char_descriptor_discovery(conn_id, bas_client, start_handle,
//                                             end_handle) == GAP_CAUSE_SUCCESS)
//    {
//        return true;
//    }
//    return false;
//}

static void tps_client_discover_state_cb(uint8_t conn_id,  T_DISCOVERY_STATE discovery_state)
{
    bool cb_flag = false;
    T_TPS_CLIENT_CB_DATA cb_data;
    cb_data.cb_type = TPS_CLIENT_CB_TYPE_DISC_STATE;

    PROFILE_PRINT_INFO1("tps_client_discover_state_cb: discovery_state = %d", discovery_state);
    if (tps_table[conn_id].disc_state == DISC_TPS_START)
    {
        switch (discovery_state)
        {
        case DISC_STATE_SRV_DONE:
            /* Indicate that service handle found. Start discover characteristic. */
            if ((tps_table[conn_id].hdl_cache[HDL_TPS_SRV_START] != 0)
                || (tps_table[conn_id].hdl_cache[HDL_TPS_SRV_END] != 0))
            {
                if (tps_start_char_discovery(conn_id) == false)
                {
                    tps_table[conn_id].disc_state = DISC_TPS_FAILED;
                    cb_flag = true;
                }
            }
            /* No TPS handle found. Discover procedure complete. */
            else
            {
                tps_table[conn_id].disc_state = DISC_TPS_FAILED;
                cb_flag = true;
            }
            break;
        case DISC_STATE_CHAR_DONE:
            tps_table[conn_id].disc_state = DISC_TPS_DONE;
            cb_flag = true;
//            if (tps_table[conn_id].properties & GATT_CHAR_PROP_NOTIFY)
//            {
//                //discovery cccd
//                if (tps_start_char_descriptor_discovery(conn_id) == false)
//                {
//                    tps_table[conn_id].disc_state = DISC_TPS_FAILED;
//                    cb_flag = true;
//                }
//            }
//            else
//            {
//                tps_table[conn_id].disc_state = DISC_TPS_DONE;
//                cb_flag = true;
//            }
            break;

        case DISC_STATE_CHAR_DESCRIPTOR_DONE:
//            bas_table[conn_id].disc_state = DISC_BAS_DONE;
//            cb_flag = true;
            break;

        case DISC_STATE_FAILED:
            tps_table[conn_id].disc_state = DISC_TPS_FAILED;
            cb_flag = true;
            break;

        default:
            PROFILE_PRINT_ERROR0("Invalid Discovery State!");
            break;
        }
    }

    /* Send discover state to application if needed. */
    if (cb_flag && tps_client_cb)
    {
        cb_data.cb_content.disc_state = tps_table[conn_id].disc_state;
        (*tps_client_cb)(tps_client, conn_id, &cb_data);
    }
    return;
}


static void tps_client_discover_result_cb(uint8_t conn_id,  T_DISCOVERY_RESULT_TYPE result_type,
                                          T_DISCOVERY_RESULT_DATA result_data)
{
    PROFILE_PRINT_INFO1("tps_client_discover_result_cb: result_type = %d", result_type);
    if (tps_table[conn_id].disc_state == DISC_TPS_START)
    {
        switch (result_type)
        {
        case DISC_RESULT_SRV_DATA:
            tps_table[conn_id].hdl_cache[HDL_TPS_SRV_START] =
                result_data.p_srv_disc_data->att_handle;
            tps_table[conn_id].hdl_cache[HDL_TPS_SRV_END] =
                result_data.p_srv_disc_data->end_group_handle;
            break;

        case DISC_RESULT_CHAR_UUID16:
            {
                uint16_t handle;
                handle = result_data.p_char_uuid16_disc_data->value_handle;
                if (result_data.p_char_uuid16_disc_data->uuid16 == GATT_UUID_CHAR_TX_LEVEL)
                {
                    tps_table[conn_id].hdl_cache[HDL_TPS_PARA] = handle;
                }
            }
            break;

        case DISC_RESULT_CHAR_DESC_UUID16:
//            if (result_data.p_char_desc_uuid16_disc_data->uuid16 == GATT_UUID_CHAR_CLIENT_CONFIG)
//            {
//                bas_table[conn_id].hdl_cache[HDL_BAS_BATTERY_LEVEL_CCCD] =
//                    result_data.p_char_desc_uuid16_disc_data->handle;
//            }
            break;

        default:
            PROFILE_PRINT_ERROR0("Invalid Discovery Result Type!");
            break;
        }
    }

    return;
}

//static void bas_client_write_result_cb(uint8_t conn_id, T_GATT_WRITE_TYPE type,
//                                       uint16_t handle,
//                                       uint16_t cause,
//                                       uint8_t credits)
//{
//    T_BAS_CLIENT_CB_DATA cb_data;
//    uint16_t *hdl_cache;
//    hdl_cache = bas_table[conn_id].hdl_cache;
//    cb_data.cb_type = BAS_CLIENT_CB_TYPE_WRITE_RESULT;

//    PROFILE_PRINT_INFO2("bas_client_write_result_cb: handle 0x%x, cause 0x%x", handle, cause);
//    cb_data.cb_content.write_result.cause = cause;

//    if (handle == hdl_cache[HDL_BAS_BATTERY_LEVEL_CCCD])
//    {
//        if (bas_table[conn_id].write_notify_value)
//        {
//            cb_data.cb_content.write_result.type = BAS_WRITE_NOTIFY_ENABLE;
//        }
//        else
//        {
//            cb_data.cb_content.write_result.type = BAS_WRITE_NOTIFY_DISABLE;
//        }
//    }
//    else
//    {
//        return;
//    }

//    if (bas_client_cb)
//    {
//        (*bas_client_cb)(bas_client, conn_id, &cb_data);
//    }
//    return;
//}

static void tps_client_read_result_cb(uint8_t conn_id,  uint16_t cause,
                                      uint16_t handle, uint16_t value_size, uint8_t *p_value)
{
    T_TPS_CLIENT_CB_DATA cb_data;
    uint16_t *hdl_cache;
    hdl_cache = tps_table[conn_id].hdl_cache;
    cb_data.cb_type = TPS_CLIENT_CB_TYPE_READ_RESULT;

    PROFILE_PRINT_INFO2("tps_client_read_result_cb: handle 0x%x, cause 0x%x", handle, cause);
    cb_data.cb_content.read_result.cause = cause;

    if (handle == hdl_cache[HDL_TPS_PARA])
    {
        cb_data.cb_content.read_result.type = TPS_READ_PARA;
        if (cause == GAP_SUCCESS)
        {
            if (value_size != 1)
            {
                PROFILE_PRINT_ERROR1("tps_client_read_result_cb: invalid battery value len %d", value_size);
                return;
            }
            cb_data.cb_content.read_result.data.txpower_level = *p_value;
        }
    }
    else
    {
        return;
    }

    if (tps_client_cb)
    {
        (*tps_client_cb)(tps_client, conn_id, &cb_data);
    }
    return;
}

//static T_APP_RESULT tps_client_notify_ind_cb(uint8_t conn_id, bool notify, uint16_t handle,
//                                             uint16_t value_size, uint8_t *p_value)
//{
//    T_APP_RESULT app_result = APP_RESULT_SUCCESS;
//    T_BAS_CLIENT_CB_DATA cb_data;
//    uint16_t *hdl_cache;

//    hdl_cache = bas_table[conn_id].hdl_cache;
//    cb_data.cb_type = BAS_CLIENT_CB_TYPE_NOTIF_IND_RESULT;

//    if (handle == hdl_cache[HDL_BAS_BATTERY_LEVEL])
//    {
//        cb_data.cb_content.notify_data.battery_level = *p_value;
//    }
//    else
//    {
//        return APP_RESULT_SUCCESS;
//    }

//    if (bas_client_cb)
//    {
//        app_result = (*bas_client_cb)(bas_client, conn_id, &cb_data);
//    }

//    return app_result;
//}

static void tps_client_disc_cb(uint8_t conn_id)
{
    PROFILE_PRINT_INFO0("tps_client_disc_cb.");
    if (conn_id >= tps_link_num)
    {
        PROFILE_PRINT_ERROR1("tps_client_disc_cb: failed invalid conn_id %d", conn_id);
        return;
    }
    memset(&tps_table[conn_id], 0, sizeof(T_TPS_LINK));
    return;
}
/**
 * @brief BAS Client Callbacks.
*/
const T_FUN_CLIENT_CBS tps_client_cbs =
{
    tps_client_discover_state_cb,   //!< Discovery State callback function pointer
    tps_client_discover_result_cb,  //!< Discovery result callback function pointer
    tps_client_read_result_cb,      //!< Read response callback function pointer
    NULL,//bas_client_write_result_cb,     //!< Write result callback function pointer
    NULL,//bas_client_notify_ind_cb,       //!< Notify Indicate callback function pointer
    tps_client_disc_cb              //!< Link disconnection callback function pointer
};

/**
  * @brief      Add tps client to application.
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
        tps_client_id = tps_add_client(app_client_callback, APP_MAX_LINKS);
    }
  * \endcode
  */
T_CLIENT_ID tps_add_client(P_FUN_GENERAL_APP_CB app_cb, uint8_t link_num)
{
    uint16_t size;
    if (link_num > TPS_MAX_LINKS)
    {
        PROFILE_PRINT_ERROR1("tps_add_client: invalid link_num %d", link_num);
        return 0xff;
    }
    if (false == client_register_spec_client_cb(&tps_client, &tps_client_cbs))
    {
        tps_client = CLIENT_PROFILE_GENERAL_ID;
        PROFILE_PRINT_ERROR0("tps_add_client:register fail");
        return tps_client;
    }
    PROFILE_PRINT_INFO1("tps_add_client: client id %d", tps_client);

    /* register callback for profile to inform application that some events happened. */
    tps_client_cb = app_cb;
    tps_link_num = link_num;
    size = tps_link_num * sizeof(T_TPS_LINK);
    tps_table = os_mem_zalloc(RAM_TYPE_DATA_ON, size);

    return tps_client;
}

