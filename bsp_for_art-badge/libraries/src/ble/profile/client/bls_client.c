/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     bls_client.c
  * @brief    Source file for client of internet protocol support service.
  * @details  Global data and function implement.
  * @author   Jeff_Zheng
  * @date     2017-12-05
  * @version  v1.0
  * *************************************************************************************
  */
#include "string.h"
#include "trace.h"
#include "os_mem.h"
#include "bls_client.h"

/********************************************************************************************************
* local static variables defined here, only used in this source file.
********************************************************************************************************/

typedef struct
{
    T_BLS_DISC_STATE disc_state;
    bool             write_notify_value;
    bool             write_indicate_value;
    uint16_t         properties;
    uint16_t         hdl_cache[HDL_BLS_CACHE_LEN];
} T_BLS_LINK, *P_BLS_LINK;

static P_BLS_LINK bls_table;
static uint8_t bls_link_num;

static T_CLIENT_ID bls_cl_id = CLIENT_PROFILE_GENERAL_ID;
static P_FUN_GENERAL_APP_CB bls_cl_cb = NULL;

bool bls_start_discovery(uint8_t conn_id)
{
    PROFILE_PRINT_INFO0("bls_start_discovery");

    /* First clear handle cache. */
    memset(&bls_table[conn_id], 0, sizeof(T_BLS_LINK));
    bls_table[conn_id].disc_state = DISC_BLS_START;
    if (client_by_uuid_srv_discovery(conn_id, bls_cl_id,
                                     GATT_UUID_BLOOD_PRESSURE) == GAP_CAUSE_SUCCESS)
    {
        return true;
    }
    return false;
}

static bool bls_start_char_discovery(uint8_t conn_id)
{
    uint16_t start_handle;
    uint16_t end_handle;

    PROFILE_PRINT_INFO0("bls_start_char_discovery");
    start_handle = bls_table[conn_id].hdl_cache[HDL_BLS_SRV_START];
    end_handle = bls_table[conn_id].hdl_cache[HDL_BLS_SRV_END];
    if (client_all_char_discovery(conn_id, bls_cl_id, start_handle,
                                  end_handle) == GAP_CAUSE_SUCCESS)
    {
        return true;
    }
    return false;
}

static bool bls_start_char_descriptor_discovery(uint8_t conn_id)
{
    uint16_t start_handle;
    uint16_t end_handle;

    PROFILE_PRINT_INFO0("bls_start_char_descriptor_discovery");
    start_handle = bls_table[conn_id].hdl_cache[HDL_BLS_BLP_MEASUREMENT];
    end_handle = bls_table[conn_id].hdl_cache[HDL_BLS_SRV_END];
    if (client_all_char_descriptor_discovery(conn_id, bls_cl_id, start_handle,
                                             end_handle) == GAP_CAUSE_SUCCESS)
    {
        return true;
    }
    return false;
}

static void bls_client_discover_state_cb(uint8_t conn_id,  T_DISCOVERY_STATE discovery_state)
{
    bool cb_flag = false;
    T_BLS_CLIENT_CB_DATA cb_data;
    cb_data.cb_type = BLS_CLIENT_CB_TYPE_DISC_STATE;

    PROFILE_PRINT_INFO1("bls_client_discover_state_cb: discovery_state = %d", discovery_state);
    if (bls_table[conn_id].disc_state == DISC_BLS_START)
    {
        switch (discovery_state)
        {
        case DISC_STATE_SRV_DONE:
            /* Indicate that service handle found. Start discover characteristic. */
            if ((bls_table[conn_id].hdl_cache[HDL_BLS_SRV_START] != 0)
                || (bls_table[conn_id].hdl_cache[HDL_BLS_SRV_END] != 0))
            {
                if (bls_start_char_discovery(conn_id) == false)
                {
                    bls_table[conn_id].disc_state = DISC_BLS_FAILED;
                    cb_flag = true;
                }
            }
            /* No BLS handle found. Discover procedure complete. */
            else
            {
                bls_table[conn_id].disc_state = DISC_BLS_FAILED;
                cb_flag = true;
            }
            break;
        case DISC_STATE_CHAR_DONE:
            if (bls_table[conn_id].properties & GATT_CHAR_PROP_READ)
            {
                //discovery cccd
                if (bls_start_char_descriptor_discovery(conn_id) == false)
                {
                    bls_table[conn_id].disc_state = DISC_BLS_FAILED;
                    cb_flag = true;
                }
            }
            else
            {
                bls_table[conn_id].disc_state = DISC_BLS_DONE;
                cb_flag = true;
            }
            break;

        case DISC_STATE_CHAR_DESCRIPTOR_DONE:
            bls_table[conn_id].disc_state = DISC_BLS_DONE;
            cb_flag = true;
            break;

        case DISC_STATE_FAILED:
            bls_table[conn_id].disc_state = DISC_BLS_FAILED;
            cb_flag = true;
            break;

        default:
            PROFILE_PRINT_ERROR0("Invalid Discovery State!");
            break;
        }
    }

    /* Send discover state to application if needed. */
    if (cb_flag && bls_cl_cb)
    {
        cb_data.cb_content.disc_state = bls_table[conn_id].disc_state;
        (*bls_cl_cb)(bls_cl_id, conn_id, &cb_data);
    }
    return;
}


static void bls_client_discover_result_cb(uint8_t conn_id,  T_DISCOVERY_RESULT_TYPE result_type,
                                          T_DISCOVERY_RESULT_DATA result_data)
{
    PROFILE_PRINT_INFO1("bls_client_discover_result_cb: result_type = %d", result_type);
    if (bls_table[conn_id].disc_state == DISC_BLS_START)
    {
        switch (result_type)
        {
        case DISC_RESULT_SRV_DATA:
            bls_table[conn_id].hdl_cache[HDL_BLS_SRV_START] =
                result_data.p_srv_disc_data->att_handle;
            bls_table[conn_id].hdl_cache[HDL_BLS_SRV_END] =
                result_data.p_srv_disc_data->end_group_handle;
            break;

        case DISC_RESULT_CHAR_UUID16:
            {
                uint16_t handle;
                handle = result_data.p_char_uuid16_disc_data->value_handle;
                if (result_data.p_char_uuid16_disc_data->uuid16 == GATT_UUID_CHAR_BLP_MEASUREMENT)
                {
                    bls_table[conn_id].hdl_cache[HDL_BLS_BLP_MEASUREMENT] = handle;
                    bls_table[conn_id].properties = result_data.p_char_uuid16_disc_data->properties;
                }
#if BLS_INTERMEDIATE_CUFF_PRESSURE_SUPPORT
                else if (result_data.p_char_uuid16_disc_data->uuid16 == GATT_UUID_CHAR_INTERMEDIATE_CUFF_PRESSURE)
                {
                    bls_table[conn_id].hdl_cache[HDL_BLS_INTERMEDIATE_CUFF_PRESSURE] = handle;
                    bls_table[conn_id].properties = result_data.p_char_uuid16_disc_data->properties;
                }
#endif
                else if (result_data.p_char_uuid16_disc_data->uuid16 == GATT_UUID_CHAR_BLP_FEATURE)
                {
                    bls_table[conn_id].hdl_cache[HDL_BLS_BLP_FEATURE] = handle;
                    bls_table[conn_id].properties = result_data.p_char_uuid16_disc_data->properties;
                }
            }
            break;

        case DISC_RESULT_CHAR_DESC_UUID16:
            if (result_data.p_char_desc_uuid16_disc_data->uuid16 == GATT_UUID_CHAR_CLIENT_CONFIG)
            {
                uint16_t handle;
                handle = result_data.p_char_desc_uuid16_disc_data->handle;
                if ((handle > bls_table[conn_id].hdl_cache[HDL_BLS_BLP_MEASUREMENT])
                    && (handle < bls_table[conn_id].hdl_cache[HDL_BLS_INTERMEDIATE_CUFF_PRESSURE]))
                {
                    bls_table[conn_id].hdl_cache[HDL_BLS_BLP_MEASUREMENT_CCCD] = handle;
                }
#if BLS_INTERMEDIATE_CUFF_PRESSURE_SUPPORT
                else if ((handle > bls_table[conn_id].hdl_cache[HDL_BLS_INTERMEDIATE_CUFF_PRESSURE])
                         && (handle < bls_table[conn_id].hdl_cache[HDL_BLS_BLP_FEATURE]))
                {
                    bls_table[conn_id].hdl_cache[HDL_BLS_INTERMEDIATE_CUFF_PRESSURE_CCCD] = handle;
                }
#endif
            }
            break;

        default:
            PROFILE_PRINT_ERROR0("Invalid Discovery Result Type!");
            break;
        }
    }

    return;
}

static void bls_client_write_result_cb(uint8_t conn_id, T_GATT_WRITE_TYPE type,
                                       uint16_t handle,
                                       uint16_t cause,
                                       uint8_t credits)
{
    T_BLS_CLIENT_CB_DATA cb_data;
    uint16_t *hdl_cache;
    hdl_cache = bls_table[conn_id].hdl_cache;
    cb_data.cb_type = BLS_CLIENT_CB_TYPE_WRITE_RESULT;

    PROFILE_PRINT_INFO2("bls_client_write_result_cb: handle 0x%x, cause 0x%x", handle, cause);
    cb_data.cb_content.write_result.cause = cause;

    if (handle == hdl_cache[HDL_BLS_BLP_MEASUREMENT_CCCD])
    {
        if (bls_table[conn_id].write_indicate_value)
        {
            cb_data.cb_content.write_result.type = BLS_WRITE_INDICATE_BLOOD_PRESS_MEAS_ENABLE;
        }
        else
        {
            cb_data.cb_content.write_result.type = BLS_WRITE_INDICATE_BLOOD_PRESS_MEAS_DISABLE;
        }
    }
#if BLS_INTERMEDIATE_CUFF_PRESSURE_SUPPORT
    else if (handle == hdl_cache[HDL_BLS_INTERMEDIATE_CUFF_PRESSURE_CCCD])
    {
        if (bls_table[conn_id].write_notify_value)
        {
            cb_data.cb_content.write_result.type = BLS_WRITE_NOTIFY_INTER_CUFF_PRESS_ENABLE;
        }
        else
        {
            cb_data.cb_content.write_result.type = BLS_WRITE_NOTIFY_INTER_CUFF_PRESS_DISABLE;
        }
    }
#endif
    else
    {
        return;
    }

    if (bls_cl_cb)
    {
        (*bls_cl_cb)(bls_cl_id, conn_id, &cb_data);
    }
    return;
}

static void bls_client_read_result_cb(uint8_t conn_id,  uint16_t cause,
                                      uint16_t handle, uint16_t value_size, uint8_t *p_value)
{
    T_BLS_CLIENT_CB_DATA cb_data;
    uint16_t *hdl_cache;
    hdl_cache = bls_table[conn_id].hdl_cache;
    cb_data.cb_type = BLS_CLIENT_CB_TYPE_READ_RESULT;

    PROFILE_PRINT_INFO2("bls_client_read_result_cb: handle 0x%x, cause 0x%x", handle, cause);
    if (handle == hdl_cache[HDL_BLS_BLP_FEATURE])
    {
        cb_data.cb_content.read_result.type = BLS_READ_BLP_FEATURE;
        if (cause == GAP_SUCCESS)
        {
            if (value_size != 2)
            {
                PROFILE_PRINT_ERROR1("bls_client_read_result_cb: invalid battery value len %d", value_size);
                return;
            }
            uint16_t blp_feature;
            LE_STREAM_TO_UINT16(blp_feature, p_value);
            cb_data.cb_content.read_result.data.blood_pressure_feature = blp_feature;
        }
    }
    else
    {
        return;
    }

    if (bls_cl_cb)
    {
        (*bls_cl_cb)(bls_cl_id, conn_id, &cb_data);
    }
    return;
}

static T_APP_RESULT bls_client_notify_indicate_cb(uint8_t conn_id, bool notify, uint16_t handle,
                                                  uint16_t value_size, uint8_t *p_value)
{
    T_APP_RESULT app_result = APP_RESULT_SUCCESS;
    T_BLS_CLIENT_CB_DATA cb_data;
    uint16_t *hdl_cache;

    hdl_cache = bls_table[conn_id].hdl_cache;

    if (handle == hdl_cache[HDL_BLS_BLP_MEASUREMENT])
    {
        cb_data.cb_type = BLS_CLIENT_CB_TYPE_IND_BLOOD_PRESS_MEAS;
    }
#if BLS_INTERMEDIATE_CUFF_PRESSURE_SUPPORT
    else if (handle == hdl_cache[HDL_BLS_INTERMEDIATE_CUFF_PRESSURE])
    {
        cb_data.cb_type = BLS_CLIENT_CB_TYPE_NIFY_INTER_CUFF_PRESS;
    }
#endif

    if ((handle == hdl_cache[HDL_BLS_BLP_MEASUREMENT])
#if BLS_INTERMEDIATE_CUFF_PRESSURE_SUPPORT
        || (handle == hdl_cache[HDL_BLS_INTERMEDIATE_CUFF_PRESSURE])
#endif
       )
    {
        T_BLOOD_PRESSURE_MEASURMENT blp_meas_value;

        LE_STREAM_TO_UINT8(blp_meas_value.bp_meas_flag, p_value);

        uint8_t bp_meas_systolic_value[2];
        uint8_t bp_meas_diastolic_value[2];
        uint8_t bp_meas_map_value[2];
        STREAM_TO_ARRAY(bp_meas_systolic_value, p_value, 2);
        STREAM_TO_ARRAY(bp_meas_diastolic_value, p_value, 2);
        STREAM_TO_ARRAY(bp_meas_map_value, p_value, 2);

        memcpy(blp_meas_value.bp_meas_compound_value.bp_meas_systolic_value, bp_meas_systolic_value, 2);
        memcpy(blp_meas_value.bp_meas_compound_value.bp_meas_diastolic_value, bp_meas_diastolic_value, 2);
        memcpy(blp_meas_value.bp_meas_compound_value.bp_meas_map_value, bp_meas_map_value, 2);

        if (blp_meas_value.bp_meas_flag & BLS_FLAG_MEASUREMENT_TIMESTAMP_PRESENT_BIT)
        {
            LE_STREAM_TO_UINT16(blp_meas_value.time_stamp.year, p_value);
            LE_STREAM_TO_UINT8(blp_meas_value.time_stamp.month, p_value);
            LE_STREAM_TO_UINT8(blp_meas_value.time_stamp.day, p_value);
            LE_STREAM_TO_UINT8(blp_meas_value.time_stamp.hours, p_value);
            LE_STREAM_TO_UINT8(blp_meas_value.time_stamp.minutes, p_value);
            LE_STREAM_TO_UINT8(blp_meas_value.time_stamp.seconds, p_value);
        }

        if (blp_meas_value.bp_meas_flag & BLS_FLAG_MEASUREMENT_PULSE_RATE_BIT)
        {
            uint8_t bp_meas_pulse_rate[2];
            STREAM_TO_ARRAY(bp_meas_pulse_rate, p_value, 2);
            memcpy(blp_meas_value.bp_meas_pulse_rate, bp_meas_pulse_rate, sizeof(SFLOAT));
        }

        if (blp_meas_value.bp_meas_flag & BLS_FLAG_MEASUREMENT_USER_ID_BIT)
        {
            LE_STREAM_TO_UINT8(blp_meas_value.bp_meas_user_id, p_value);
        }

        if (blp_meas_value.bp_meas_flag & BLS_FLAG_MEASUREMENT_STATUS_BIT)
        {
            uint16_t bp_meas_status;
            LE_STREAM_TO_UINT16(bp_meas_status, p_value);
            memcpy(&blp_meas_value.bp_meas_status, &bp_meas_status, sizeof(T_BLOOD_PRESSUREE_MEAS_STATUS));
        }

        memcpy(&cb_data.cb_content.blp_meas_notify_indicate_data.blood_presure_mesaurment, &blp_meas_value,
               sizeof(T_BLOOD_PRESSURE_MEASURMENT));
    }

    else
    {
        return APP_RESULT_SUCCESS;
    }
    if (bls_cl_cb)
    {
        app_result = (*bls_cl_cb)(bls_cl_id, conn_id, &cb_data);
    }

    return app_result;
}

static void bls_client_disc_cb(uint8_t conn_id)
{
    PROFILE_PRINT_INFO0("bls_client_disc_cb.");
    memset(&bls_table[conn_id], 0, sizeof(T_BLS_LINK));
    return;
}

bool bls_read_blp_feature(uint8_t conn_id)
{
    if (conn_id >= bls_link_num)
    {
        PROFILE_PRINT_ERROR1("bls_read_blp_feature: failed invalid conn_id %d", conn_id);
        return false;
    }
    if (bls_table[conn_id].hdl_cache[HDL_BLS_BLP_FEATURE])
    {
        uint16_t handle = bls_table[conn_id].hdl_cache[HDL_BLS_BLP_FEATURE];
        if (client_attr_read(conn_id, bls_cl_id, handle) == GAP_CAUSE_SUCCESS)
        {
            return true;
        }
    }
    PROFILE_PRINT_ERROR0("bls_read_blp_feature: false handle = 0");
    return false;
}

bool bls_write_notify_indicate(uint8_t conn_id, T_BLS_WRTIE_TYPE write_type)
{
    if (conn_id >= bls_link_num)
    {
        PROFILE_PRINT_ERROR1("bls_set_notify_indicate: failed invalid conn_id %d", conn_id);
        return false;
    }
    switch (write_type)
    {
    case BLS_WRITE_NOTIFY_INTER_CUFF_PRESS_ENABLE:
        {
            if (bls_table[conn_id].hdl_cache[HDL_BLS_INTERMEDIATE_CUFF_PRESSURE_CCCD])
            {
                uint16_t handle = bls_table[conn_id].hdl_cache[HDL_BLS_INTERMEDIATE_CUFF_PRESSURE_CCCD];
                PROFILE_PRINT_ERROR1("bls_set_notify_indicate: handle %d", handle);

                uint16_t length = sizeof(uint16_t);
                uint16_t cccd_bits = 1;
                if (client_attr_write(conn_id, bls_cl_id, GATT_WRITE_TYPE_REQ, handle,
                                      length, (uint8_t *)&cccd_bits) == GAP_CAUSE_SUCCESS)
                {
                    bls_table[conn_id].write_notify_value = 1;
                    return true;
                }
            }
            PROFILE_PRINT_ERROR0("bls_set_notify: false handle = 0");
            return false;
        }

    case BLS_WRITE_NOTIFY_INTER_CUFF_PRESS_DISABLE:
        {
            if (bls_table[conn_id].hdl_cache[HDL_BLS_INTERMEDIATE_CUFF_PRESSURE_CCCD])
            {
                uint16_t handle = bls_table[conn_id].hdl_cache[HDL_BLS_INTERMEDIATE_CUFF_PRESSURE_CCCD];
                uint16_t length = sizeof(uint16_t);
                uint16_t cccd_bits = 0;
                if (client_attr_write(conn_id, bls_cl_id, GATT_WRITE_TYPE_REQ, handle,
                                      length, (uint8_t *)&cccd_bits) == GAP_CAUSE_SUCCESS)
                {
                    bls_table[conn_id].write_notify_value = 0;
                    return true;
                }
            }
            PROFILE_PRINT_ERROR0("bls_set_notify: false handle = 0");
            return false;
        }

    case BLS_WRITE_INDICATE_BLOOD_PRESS_MEAS_ENABLE:
        {
            if (bls_table[conn_id].hdl_cache[HDL_BLS_BLP_MEASUREMENT_CCCD])
            {
                uint16_t handle = bls_table[conn_id].hdl_cache[HDL_BLS_BLP_MEASUREMENT_CCCD];
                PROFILE_PRINT_ERROR1("bls_set_notify_indicate: handle %d", handle);

                uint16_t length = sizeof(uint16_t);
                uint16_t cccd_bits = 2;
                if (client_attr_write(conn_id, bls_cl_id, GATT_WRITE_TYPE_REQ, handle,
                                      length, (uint8_t *)&cccd_bits) == GAP_CAUSE_SUCCESS)
                {
                    bls_table[conn_id].write_indicate_value = 1;
                    return true;
                }
            }
            PROFILE_PRINT_ERROR0("bls_set_notify: false handle = 0");
            return false;
        }

    case BLS_WRITE_INDICATE_BLOOD_PRESS_MEAS_DISABLE:
        {
            if (bls_table[conn_id].hdl_cache[HDL_BLS_BLP_MEASUREMENT_CCCD])
            {
                uint16_t handle = bls_table[conn_id].hdl_cache[HDL_BLS_BLP_MEASUREMENT_CCCD];
                uint16_t length = sizeof(uint16_t);
                uint16_t cccd_bits = 0;
                if (client_attr_write(conn_id, bls_cl_id, GATT_WRITE_TYPE_REQ, handle,
                                      length, (uint8_t *)&cccd_bits) == GAP_CAUSE_SUCCESS)
                {
                    bls_table[conn_id].write_indicate_value = 1;
                    return true;
                }
            }
            PROFILE_PRINT_ERROR0("bls_set_notify: false handle = 0");
            return false;
        }

    default:
        return false;

    }
}

/**
  * @brief IPS service client callback function pointer.
  */
const T_FUN_CLIENT_CBS bls_cl_cbs =
{
    bls_client_discover_state_cb,   //!< Discovery State callback function pointer
    bls_client_discover_result_cb,  //!< Discovery result callback function pointer
    bls_client_read_result_cb,      //!< Read response callback function pointer
    bls_client_write_result_cb,     //!< Write result callback function pointer
    bls_client_notify_indicate_cb,  //!< Notify Indicate callback function pointer
    bls_client_disc_cb              //!< Link disconnection callback function pointer
};

T_CLIENT_ID bls_add_client(P_FUN_GENERAL_APP_CB app_cb, uint8_t link_num)
{
    uint16_t size;
    if (link_num > BLS_MAX_LINKS)
    {
        PROFILE_PRINT_ERROR1("bls_add_client: invalid link_num %d", link_num);
        return 0xff;
    }
    if (false == client_register_spec_client_cb(&bls_cl_id, &bls_cl_cbs))
    {
        bls_cl_id = CLIENT_PROFILE_GENERAL_ID;
        PROFILE_PRINT_ERROR0("bls_add_client: register fail");
        return bls_cl_id;
    }

    PROFILE_PRINT_INFO1("bls_add_client: client id %d", bls_cl_id);
    bls_cl_cb = app_cb;
    bls_link_num = link_num;
    size = bls_link_num * sizeof(T_BLS_LINK);
    bls_table = os_mem_zalloc(RAM_TYPE_DATA_ON, size);

    return bls_cl_id;
}
