/**
*****************************************************************************************
*     Copyright(c) 2019, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    gatts_client.c
  * @brief   GATTS client source file.
  * @details
  * @author  berni
  * @date    2019-05-27
  * @version v0.1
  ******************************************************************************
  */

/** Add Includes here **/
#include <trace.h>
#include <string.h>
#include <gatts_client.h>
#include <os_mem.h>

/********************************************************************************************************
* local static variables defined here, only used in this source file.
********************************************************************************************************/

/**
 * @brief  GATTS Client Link control block definition.
 */
typedef struct
{
    T_GATTS_DISC_STATE disc_state;
    bool               write_ind_value;
    uint16_t           hdl_cache[HDL_GATTS_CACHE_LEN];
} T_GATTS_LINK, *P_GATTS_LINK;

/** @brief  GATTS Client link table */
static P_GATTS_LINK gatts_table;
static uint8_t gatts_link_num;

/**<  GATTS client ID. */
static T_CLIENT_ID gatts_client = CLIENT_PROFILE_GENERAL_ID;
/**<  Callback used to send data to app from GATTS client layer. */
static P_FUN_GENERAL_APP_CB gatts_client_cb = NULL;

/**
  * @brief  Used by application, to start the discovery procedure of GATT service.
  * @param[in]  conn_id connection ID.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    static T_USER_CMD_PARSE_RESULT cmd_gattsdis(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        uint8_t conn_id = p_parse_value->dw_param[0];
        bool ret = gatts_start_discovery(conn_id);
        ......
    }
 * \endcode
  */
bool gatts_start_discovery(uint8_t conn_id)
{
    PROFILE_PRINT_INFO0("gatts_start_discovery");
    /* First clear handle cache. */
    if (conn_id >= gatts_link_num)
    {
        PROFILE_PRINT_ERROR1("gatts_start_discovery: failed invalid conn_id %d", conn_id);
        return false;
    }
    memset(&gatts_table[conn_id], 0, sizeof(T_GATTS_LINK));
    gatts_table[conn_id].disc_state = DISC_GATTS_START;
    if (client_by_uuid_srv_discovery(conn_id, gatts_client,
                                     GATT_UUID_GATT) == GAP_CAUSE_SUCCESS)
    {
        return true;
    }
    return false;
}

/**
  * @brief  Used by application, to enable or disable the indication of peer server's Service Changed Characteristic.
  * @param[in]  conn_id connection ID.
  * @param[in]  ind 0--disable the indication, 1--enable the indication.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
bool gatts_client_set_service_changed_ind(uint8_t conn_id, bool ind)
{
    if (conn_id >= gatts_link_num)
    {
        PROFILE_PRINT_ERROR1("gatts_client_set_service_changed_ind: failed invalid conn_id %d", conn_id);
        return false;
    }

    if (gatts_table[conn_id].hdl_cache[HDL_GATTS_SERVICE_CHANGED_CCCD])
    {
        uint16_t handle = gatts_table[conn_id].hdl_cache[HDL_GATTS_SERVICE_CHANGED_CCCD];
        uint16_t length = sizeof(uint16_t);
        uint16_t cccd_bits = ind ? 2 : 0;
        if (client_attr_write(conn_id, gatts_client, GATT_WRITE_TYPE_REQ, handle,
                              length, (uint8_t *)&cccd_bits) == GAP_CAUSE_SUCCESS)
        {
            gatts_table[conn_id].write_ind_value = ind;
            return true;
        }
    }

    APP_PRINT_WARN0("gatts_client_set_service_changed_ind: Request fail! Please check!");
    return false;
}

/**
  * @brief  Used by application, to get handle cache.
  * @param[in]  conn_id connection ID.
  * @param[in,out]  p_hdl_cache pointer of the handle cache table
  * @param[in]  len the length of handle cache table
  * @retval true success.
  * @retval false failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    static T_USER_CMD_PARSE_RESULT cmd_gattshdl(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        uint8_t conn_id = p_parse_value->dw_param[0];
        uint16_t hdl_cache[HDL_GATTS_CACHE_LEN];
        uint8_t hdl_idx;
        bool ret = gatts_get_hdl_cache(conn_id, hdl_cache, sizeof(uint16_t) * HDL_GATTS_CACHE_LEN);
        ......
    }
 * \endcode
  */
bool gatts_get_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len)
{
    if (conn_id >= gatts_link_num)
    {
        PROFILE_PRINT_ERROR1("gatts_get_hdl_cache: failed invalid conn_id %d", conn_id);
        return false;
    }
    if (gatts_table[conn_id].disc_state != DISC_GATTS_DONE)
    {
        PROFILE_PRINT_ERROR1("gatts_get_hdl_cache: failed invalid state %d",
                             gatts_table[conn_id].disc_state);
        return false;
    }
    if (len != sizeof(uint16_t) * HDL_GATTS_CACHE_LEN)
    {
        PROFILE_PRINT_ERROR1("gatts_get_hdl_cache: failed invalid len %d", len);
        return false;
    }
    memcpy(p_hdl_cache, gatts_table[conn_id].hdl_cache, len);
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
  * <b>Example usage</b>
  * \code{.c}
    void app_discov_services(uint8_t conn_id, bool start)
    {
        ......
        if (app_srvs_table.srv_found_flags & APP_DISCOV_GATTS_FLAG)
        {
            gatts_set_hdl_cache(conn_id, app_srvs_table.gatts_hdl_cache, sizeof(uint16_t) * HDL_GATTS_CACHE_LEN);
        }
        ......
    }
 * \endcode
  */
bool gatts_set_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len)
{
    if (conn_id >= gatts_link_num)
    {
        PROFILE_PRINT_ERROR1("gatts_set_hdl_cache: failed invalid conn_id %d", conn_id);
        return false;
    }
    if (gatts_table[conn_id].disc_state != DISC_GATTS_IDLE)
    {
        PROFILE_PRINT_ERROR1("gatts_set_hdl_cache: failed invalid state %d",
                             gatts_table[conn_id].disc_state);
        return false;
    }
    if (len != sizeof(uint16_t) * HDL_GATTS_CACHE_LEN)
    {
        PROFILE_PRINT_ERROR1("gatts_set_hdl_cache: failed invalid len %d", len);
        return false;
    }
    memcpy(gatts_table[conn_id].hdl_cache, p_hdl_cache, len);
    gatts_table[conn_id].disc_state = DISC_GATTS_DONE;
    return true;
}

static bool gatts_client_start_char_discovery(uint8_t conn_id)
{
    uint16_t start_handle;
    uint16_t end_handle;

    PROFILE_PRINT_INFO0("gatts_client_start_gap_char_discovery");
    start_handle = gatts_table[conn_id].hdl_cache[HDL_GATTS_SRV_START];
    end_handle = gatts_table[conn_id].hdl_cache[HDL_GATTS_SRV_END];
    if (client_all_char_discovery(conn_id, gatts_client, start_handle,
                                  end_handle) == GAP_CAUSE_SUCCESS)
    {
        return true;
    }
    return false;
}

static bool gatts_client_start_char_descriptor_discovery(uint8_t conn_id)
{
    uint16_t start_handle;
    uint16_t end_handle;

    PROFILE_PRINT_INFO0("gatts_client_start_char_descriptor_discovery");
    start_handle = gatts_table[conn_id].hdl_cache[HDL_GATTS_SERVICE_CHANGED];
    end_handle = gatts_table[conn_id].hdl_cache[HDL_GATTS_SRV_END];
    if (client_all_char_descriptor_discovery(conn_id, gatts_client, start_handle,
                                             end_handle) == GAP_CAUSE_SUCCESS)
    {
        return true;
    }
    return false;
}

static void gatts_client_discover_state_cb(uint8_t conn_id,  T_DISCOVERY_STATE discovery_state)
{
    bool cb_flag = false;
    T_GATTS_CLIENT_CB_DATA cb_data;
    cb_data.cb_type = GATTS_CLIENT_CB_TYPE_DISC_STATE;

    PROFILE_PRINT_INFO1("gatts_client_discover_state_cb: discovery_state %d", discovery_state);

    switch (discovery_state)
    {
    case DISC_STATE_SRV_DONE:
        /* Indicate that service handle found. Start discover characteristic. */
        if ((gatts_table[conn_id].hdl_cache[HDL_GATTS_SRV_START] != 0)
            || (gatts_table[conn_id].hdl_cache[HDL_GATTS_SRV_END] != 0))
        {
            if (gatts_client_start_char_discovery(conn_id) == false)
            {
                gatts_table[conn_id].disc_state = DISC_GATTS_FAILED;
                cb_flag = true;
            }
        }
        /* No GAP service handle found. Discover procedure complete. */
        else
        {
            gatts_table[conn_id].disc_state = DISC_GATTS_FAILED;
            cb_flag = true;
        }
        break;

    case DISC_STATE_CHAR_DONE:
        if (gatts_table[conn_id].hdl_cache[HDL_GATTS_SERVICE_CHANGED] != 0)
        {
            if (gatts_client_start_char_descriptor_discovery(conn_id) == false)
            {
                gatts_table[conn_id].disc_state = DISC_GATTS_FAILED;
                cb_flag = true;
            }
        }
        else
        {
            gatts_table[conn_id].disc_state = DISC_GATTS_FAILED;
            cb_flag = true;
        }
        break;

    case DISC_STATE_CHAR_DESCRIPTOR_DONE:
        gatts_table[conn_id].disc_state = DISC_GATTS_DONE;
        cb_flag = true;
        break;

    case DISC_STATE_FAILED:
        PROFILE_PRINT_ERROR0("DISC_STATE_FAILED");
        gatts_table[conn_id].disc_state = DISC_GATTS_FAILED;
        cb_flag = true;
        break;
    default:
        PROFILE_PRINT_ERROR0("Invalid Discovery State!");
        break;
    }
    /* Send discover state to application if needed. */
    if (cb_flag && gatts_client_cb)
    {
        cb_data.cb_content.disc_state = gatts_table[conn_id].disc_state;
        (*gatts_client_cb)(gatts_client, conn_id, &cb_data);
    }
    return;
}

static void gatts_client_discover_result_cb(uint8_t conn_id,  T_DISCOVERY_RESULT_TYPE result_type,
                                            T_DISCOVERY_RESULT_DATA result_data)
{
    PROFILE_PRINT_INFO1("gatts_client_discover_result_cb: result_type = %d", result_type);
    if (gatts_table[conn_id].disc_state == DISC_GATTS_START)
    {
        uint16_t handle;
        switch (result_type)
        {
        case DISC_RESULT_SRV_DATA:
            gatts_table[conn_id].hdl_cache[HDL_GATTS_SRV_START] =
                result_data.p_srv_disc_data->att_handle;
            gatts_table[conn_id].hdl_cache[HDL_GATTS_SRV_END] =
                result_data.p_srv_disc_data->end_group_handle;
            break;

        case DISC_RESULT_CHAR_UUID16:
            handle = result_data.p_char_uuid16_disc_data->value_handle;
            /* we should inform intrested handles to upper application. */
            switch (result_data.p_char_uuid16_disc_data->uuid16)
            {
            case GATT_UUID_CHAR_SERVICE_CHANGED:
                gatts_table[conn_id].hdl_cache[HDL_GATTS_SERVICE_CHANGED] = handle;
                break;
            default:
                /* have no intrest on this handle. */
                break;
            }
            break;

        case DISC_RESULT_CHAR_DESC_UUID16:
            if (result_data.p_char_desc_uuid16_disc_data->uuid16 == GATT_UUID_CHAR_CLIENT_CONFIG)
            {
                handle = result_data.p_char_desc_uuid16_disc_data->handle;
                if ((handle > gatts_table[conn_id].hdl_cache[HDL_GATTS_SERVICE_CHANGED])
                    && (gatts_table[conn_id].hdl_cache[HDL_GATTS_SERVICE_CHANGED_CCCD] == 0))
                {
                    gatts_table[conn_id].hdl_cache[HDL_GATTS_SERVICE_CHANGED_CCCD] = handle;
                }
            }
            break;


        default:
            PROFILE_PRINT_ERROR0("Invalid Discovery Result Type!");
            break;
        }
    }
    return;
}


static void gatts_client_write_result_cb(uint8_t conn_id, T_GATT_WRITE_TYPE type,
                                         uint16_t handle, uint16_t cause,
                                         uint8_t credits)
{
    T_GATTS_CLIENT_CB_DATA cb_data;
    uint16_t *hdl_cache;
    hdl_cache = gatts_table[conn_id].hdl_cache;
    cb_data.cb_type = GATTS_CLIENT_CB_TYPE_WRITE_RESULT;

    PROFILE_PRINT_INFO2("gatts_client_write_result_cb: handle 0x%x, cause 0x%x", handle, cause);
    cb_data.cb_content.write_result.cause = cause;

    if (handle == hdl_cache[HDL_GATTS_SERVICE_CHANGED_CCCD])
    {
        if (gatts_table[conn_id].write_ind_value)
        {
            cb_data.cb_content.write_result.type = GATTS_WRITE_SERVICE_CHANGED_IND_ENABLE;
        }
        else
        {
            cb_data.cb_content.write_result.type = GATTS_WRITE_SERVICE_CHANGED_IND_DISABLE;
        }
    }
    else
    {
        return;
    }
    /* Inform application the write result. */
    if (gatts_client_cb)
    {
        (*gatts_client_cb)(gatts_client, conn_id, &cb_data);
    }

    return;
}

static T_APP_RESULT gatts_client_notif_ind_result_cb(uint8_t conn_id, bool notify,
                                                     uint16_t handle,
                                                     uint16_t value_size, uint8_t *p_value)
{
    T_APP_RESULT app_result = APP_RESULT_SUCCESS;
    T_GATTS_CLIENT_CB_DATA cb_data;
    uint16_t *hdl_cache;
    hdl_cache = gatts_table[conn_id].hdl_cache;

    cb_data.cb_type = GATTS_CLIENT_CB_TYPE_NOTIF_IND_RESULT;

    if (handle == hdl_cache[HDL_GATTS_SERVICE_CHANGED])
    {
        cb_data.cb_content.notif_ind_data.type = GATTS_SERVICE_CHANGED_INDICATE;
        cb_data.cb_content.notif_ind_data.data.value_size = value_size;
        cb_data.cb_content.notif_ind_data.data.p_value = p_value;
    }
    else
    {
        return app_result;
    }
    /* Inform application the notif/ind result. */
    if (gatts_client_cb)
    {
        app_result = (*gatts_client_cb)(gatts_client, conn_id, &cb_data);
    }

    return app_result;
}

static void gatts_client_disc_cb(uint8_t conn_id)
{
    PROFILE_PRINT_INFO0("gatts_client_disc_cb.");
    if (conn_id >= gatts_link_num)
    {
        PROFILE_PRINT_ERROR1("gatts_client_disc_cb: failed invalid conn_id %d", conn_id);
        return;
    }
    memset(&gatts_table[conn_id], 0, sizeof(T_GATTS_LINK));
    return;
}
/**
 * @brief GATTS Client Callbacks.
*/
const T_FUN_CLIENT_CBS gatts_client_cbs =
{
    gatts_client_discover_state_cb,   //!< Discovery State callback function pointer
    gatts_client_discover_result_cb,  //!< Discovery result callback function pointer
    NULL,      //!< Read response callback function pointer
    gatts_client_write_result_cb,     //!< Write result callback function pointer
    gatts_client_notif_ind_result_cb,  //!< Notify Indicate callback function pointer
    gatts_client_disc_cb       //!< Link disconnection callback function pointer
};

/**
  * @brief  Add gatt service client to application.
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
        gatts_client_id  = gatts_add_client(app_client_callback, APP_MAX_LINKS);
    }
 * \endcode
  */
T_CLIENT_ID gatts_add_client(P_FUN_GENERAL_APP_CB app_cb, uint8_t link_num)
{
    uint16_t size;
    if (link_num > GATTS_MAX_LINKS)
    {
        PROFILE_PRINT_ERROR1("gatts_add_client: invalid link_num %d", link_num);
        return 0xff;
    }
    if (false == client_register_spec_client_cb(&gatts_client, &gatts_client_cbs))
    {
        gatts_client = CLIENT_PROFILE_GENERAL_ID;
        PROFILE_PRINT_ERROR0("gatts_add_client Fail !!!");
        return gatts_client;
    }
    PROFILE_PRINT_INFO1("gatts_add_client: client ID = %d", gatts_client);

    /* register callback for profile to inform application that some events happened. */
    gatts_client_cb = app_cb;
    gatts_link_num = link_num;
    size = gatts_link_num * sizeof(T_GATTS_LINK);
    gatts_table = os_mem_zalloc(RAM_TYPE_DATA_ON, size);

    return gatts_client;
}

