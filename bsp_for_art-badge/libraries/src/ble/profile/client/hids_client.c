/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    hids_client.c
  * @brief
  * @details
  * @author  jane
  * @date    2016-02-18
  * @version v1.0
  ******************************************************************************
  */

/** Add Includes here **/
#include <string.h>
#include <hids_client.h>
#include <trace.h>
#include <os_mem.h>

/********************************************************************************************************
* local static variables defined here, only used in this source file.
********************************************************************************************************/

typedef enum
{
    HDIS_REPORT_VALUE_HANDLE,
    HDIS_REPORT_CCCD_HANDLE,
    HDIS_REPORT_REF_HANDLE
} T_HIDS_HANDLE_TYPE;

/**
 * @brief  HID Host Link control block definition.
 */
typedef struct
{
    T_HIDS_CLIENT_DISC_STATE disc_state;
    bool             enable_cccd_proc;
    uint8_t          *p_report_map; //allocate mem
    uint16_t         report_map_len;
    uint8_t          report_idx;
    uint8_t          read_proc_flag;
    uint8_t          read_flags;
    uint8_t          protocol_mode;
    uint8_t          report_num;
    HID_INFO_ATTRB   hids_info;
    uint16_t         hdl_cache[HDL_HIDS_CACHE_LEN];
    T_HIDS_CLIENT_REPORT report[HIDS_CLIENT_MAX_REPORT_NUM];
} T_HIDS_CLIENT_LINK, *P_HIDS_CLIENT_LINK;

static P_HIDS_CLIENT_LINK hids_client_table;
static uint8_t hids_client_link_num;

/**<  HID client ID. */
static T_CLIENT_ID hids_client_id = CLIENT_PROFILE_GENERAL_ID;
/**<  Callback used to send data to app from HID client layer. */
static P_FUN_GENERAL_APP_CB hids_client_cb = NULL;


T_HIDS_CLIENT_REPORT *hids_report_add(uint8_t conn_id, uint16_t hdl_report_value)
{
    T_HIDS_CLIENT_LINK *p_hids_link;
    T_HIDS_CLIENT_REPORT *p_report = NULL;
    PROFILE_PRINT_INFO2("[HIDS]: hids_report_add: conn_id %d, reprot value handle 0x%x", conn_id,
                        hdl_report_value);
    if (conn_id >= hids_client_link_num)
    {
        PROFILE_PRINT_ERROR1("[HIDS]: hids_report_add: failed, invalid conn_id %d", conn_id);
        return NULL;
    }
    p_hids_link = &hids_client_table[conn_id];
    if (p_hids_link->report_num >= HIDS_CLIENT_MAX_REPORT_NUM)
    {
        PROFILE_PRINT_ERROR0("[HIDS]: hids_report_add: failed, exceed HIDS_CLIENT_MAX_REPORT_NUM");
        return NULL;
    }
    p_report = &p_hids_link->report[p_hids_link->report_num];
    p_report->hdl_report_value = hdl_report_value;
    p_hids_link->report_num++;

    PROFILE_PRINT_INFO1("[HIDS]: hids_report_add: report_num 0x%x", p_hids_link->report_num);

    return p_report;
}

T_HIDS_CLIENT_REPORT *hids_report_find_by_handle(uint8_t conn_id, T_HIDS_HANDLE_TYPE type,
                                                 uint16_t handle)
{
    uint8_t i;
    T_HIDS_CLIENT_LINK *p_hids_link;
    T_HIDS_CLIENT_REPORT *p_report = NULL;
    PROFILE_PRINT_INFO1("[HIDS]: hids_report_find_by_handle: conn_id %d", conn_id);
    if (conn_id >= hids_client_link_num)
    {
        PROFILE_PRINT_ERROR1("[HIDS]: hids_report_find_by_handle: failed, invalid conn_id %d", conn_id);
        return NULL;
    }
    p_hids_link = &hids_client_table[conn_id];

    for (i = 0; i < p_hids_link->report_num; i++)
    {
        uint16_t compare_handle;
        p_report = &p_hids_link->report[i];
        if (type == HDIS_REPORT_VALUE_HANDLE)
        {
            compare_handle = p_report->hdl_report_value;
        }
        else if (type == HDIS_REPORT_CCCD_HANDLE)
        {
            compare_handle = p_report->hdl_report_cccd;
        }
        else
        {
            compare_handle = p_report->hdl_report_reference;
        }
        if (compare_handle == handle)
        {
            return p_report;
        }
    }
    GAP_PRINT_ERROR0("[HIDS]: hids_report_find_by_handle: not found");
    return NULL;
}

T_HIDS_CLIENT_REPORT *hids_report_find_by_id(uint8_t conn_id, uint8_t report_id,
                                             T_HIDS_REPORT_TYPE type)
{
    uint8_t i;
    T_HIDS_CLIENT_LINK *p_hids_link;
    T_HIDS_CLIENT_REPORT *p_report = NULL;
    PROFILE_PRINT_INFO1("[HIDS]: hids_report_find_by_id: conn_id %d", conn_id);
    if (conn_id >= hids_client_link_num)
    {
        PROFILE_PRINT_ERROR1("[HIDS]: hids_report_find_by_id: failed, invalid conn_id %d", conn_id);
        return NULL;
    }
    p_hids_link = &hids_client_table[conn_id];

    for (i = 0; i < p_hids_link->report_num; i++)
    {
        p_report = &p_hids_link->report[i];

        if ((p_report->report_ref.report_id == report_id)
            && ((p_report->report_ref.report_type == type) || type == HIDS_RESERVED_REPORT))
        {
            return p_report;
        }
    }
    GAP_PRINT_ERROR0("[HIDS]: hids_report_find_by_id: not found");
    return NULL;
}

T_HIDS_CLIENT_REPORT *hids_report_check_desc_handle(uint8_t conn_id, uint16_t handle)
{
    uint8_t i;
    T_HIDS_CLIENT_LINK *p_hids_link;
    T_HIDS_CLIENT_REPORT *p_report = NULL;
    PROFILE_PRINT_INFO2("[HIDS]: hids_report_check_desc_handle: conn_id %d, handle 0x%x", conn_id,
                        handle);
    if (conn_id >= hids_client_link_num)
    {
        PROFILE_PRINT_ERROR1("[HIDS]: hids_report_check_desc_handle: failed, invalid conn_id %d", conn_id);
        return NULL;
    }
    p_hids_link = &hids_client_table[conn_id];

    for (i = 0; i < p_hids_link->report_num; i++)
    {
        p_report = &p_hids_link->report[i];
        if ((handle == p_report->hdl_report_value + 1) ||
            (handle == p_report->hdl_report_value + 2))
        {
            return p_report;
        }
    }
    GAP_PRINT_ERROR0("[HIDS]: hids_report_check_desc_handle: not found");
    return NULL;
}

/**
  * @brief  Used by application, to start the discovery procedure of battery service.
  * @param[in]  conn_id connection ID.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    static T_USER_CMD_PARSE_RESULT cmd_hiddis(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        uint8_t conn_id = p_parse_value->dw_param[0];
        bool ret = hid_start_discovery(conn_id);
        ......
    }
  * \endcode
  */
bool hids_start_discovery(uint8_t conn_id)
{
    PROFILE_PRINT_INFO0("[HIDS]: hids_start_discovery");
    if (conn_id >= hids_client_link_num)
    {
        PROFILE_PRINT_ERROR1("[HIDS]: hid_start_discovery: failed invalid conn_id %d", conn_id);
        return false;
    }
    /* First clear handle cache. */
    if (hids_client_table[conn_id].p_report_map != NULL)
    {
        os_mem_free(hids_client_table[conn_id].p_report_map);
    }
    memset(&hids_client_table[conn_id], 0, sizeof(T_HIDS_CLIENT_LINK));
    hids_client_table[conn_id].disc_state = DISC_HIDS_START;
    if (client_by_uuid_srv_discovery(conn_id, hids_client_id,
                                     GATT_UUID_HID) == GAP_CAUSE_SUCCESS)
    {
        return true;
    }
    return false;
}

bool hids_enable_all_cccd(uint8_t conn_id)
{
    uint8_t i;
    uint16_t cccd_bits = 1;
    PROFILE_PRINT_INFO0("[HIDS]: hids_enable_all_cccd");
    if (conn_id >= hids_client_link_num)
    {
        PROFILE_PRINT_ERROR1("[HIDS]: hids_enable_all_cccd: failed invalid conn_id %d", conn_id);
        return false;
    }
    if (hids_client_table[conn_id].disc_state != DISC_HIDS_DONE ||
        hids_client_table[conn_id].enable_cccd_proc)
    {
        PROFILE_PRINT_ERROR1("[HIDS]: hids_enable_all_cccd: failed invalid state %d",
                             hids_client_table[conn_id].disc_state);
        return false;
    }

    for (i = 0; i < hids_client_table[conn_id].report_num; i++)
    {
        if (hids_client_table[conn_id].report[i].hdl_report_cccd != 0)
        {
            if (client_attr_write(conn_id, hids_client_id, GATT_WRITE_TYPE_REQ,
                                  hids_client_table[conn_id].report[i].hdl_report_cccd,
                                  sizeof(uint16_t), (uint8_t *)&cccd_bits) == GAP_CAUSE_SUCCESS)
            {
                hids_client_table[conn_id].report_idx = i;
                hids_client_table[conn_id].enable_cccd_proc = true;
                return true;
            }
            else
            {
                PROFILE_PRINT_ERROR0("[HIDS]: hids_enable_all_cccd: call client_attr_write failed");
                return false;
            }
        }
    }
    return true;
}

bool hids_write_report(uint8_t conn_id, uint8_t report_id, T_HIDS_REPORT_TYPE report_type,
                       uint16_t length, uint8_t *p_value, T_GATT_WRITE_TYPE type)
{
    T_HIDS_CLIENT_REPORT *p_report;
    PROFILE_PRINT_INFO0("[HIDS]: hids_write_report");
    if (conn_id >= hids_client_link_num)
    {
        PROFILE_PRINT_ERROR1("[HIDS]: hids_write_report: failed invalid conn_id %d", conn_id);
        return false;
    }

    p_report = hids_report_find_by_id(conn_id, report_id, report_type);
    if (p_report != NULL)
    {

        if (client_attr_write(conn_id, hids_client_id, type,
                              p_report->hdl_report_value,
                              length, p_value) == GAP_CAUSE_SUCCESS)
        {
            return true;
        }
        else
        {
            PROFILE_PRINT_ERROR0("[HIDS]: hids_write_report: call client_attr_write failed");
            return false;
        }
    }

    return false;
}

bool hids_read_saved_data(uint8_t conn_id, T_HIDS_READ_TYPE read_type, T_HIDS_READ_VALUE *p_value)
{
    bool ret = true;
    if (conn_id >= hids_client_link_num)
    {
        PROFILE_PRINT_ERROR1("[HIDS]: hids_read_saved_data: failed invalid conn_id %d", conn_id);
        return false;
    }
    switch (read_type)
    {
    case HIDS_READ_PROTOCOL_MODE:
        {
            if (hids_client_table[conn_id].read_flags & HIDS_READ_PROTOCOL_MODE_BIT)
            {
                p_value->value_size = 1;
                p_value->p_value = &hids_client_table[conn_id].protocol_mode;
            }
            else
            {
                ret = false;
            }
        }
        break;

    case HIDS_READ_REPORT_MAP:
        {
            if (hids_client_table[conn_id].read_flags & HIDS_READ_REPORT_MAP_BIT)
            {
                p_value->value_size = hids_client_table[conn_id].report_map_len;
                p_value->p_value = hids_client_table[conn_id].p_report_map;
            }
            else
            {
                ret = false;
            }
        }
        break;

    case HIDS_READ_HID_INFO:
        {
            if (hids_client_table[conn_id].read_flags & HIDS_READ_HID_INFO_BIT)
            {
                p_value->value_size = sizeof(HID_INFO_ATTRB);
                p_value->p_value = (uint8_t *)&hids_client_table[conn_id].hids_info;
            }
            else
            {
                ret = false;
            }

        }
        break;

    default:
        {
            ret = false;

        }
        break;
    }

    if (ret == false)
    {
        PROFILE_PRINT_ERROR1("[HIDS]: hids_read_saved_data failed: read_type %d", read_type);
    }

    return ret;
}

void hids_print_infos(uint8_t conn_id)
{
    int8_t i;
    PROFILE_PRINT_INFO1("[HIDS]: hids_print_infos: conn_id %d", conn_id);
    if (conn_id >= hids_client_link_num)
    {
        return;
    }
    for (i = 0; i < HDL_HIDS_CACHE_LEN; i++)
    {
        PROFILE_PRINT_INFO2("[HIDS]: hdl_cache[%d]: 0x%x", i, hids_client_table[conn_id].hdl_cache[i]);
    }
    for (i = 0; i < hids_client_table[conn_id].report_num; i++)
    {
        PROFILE_PRINT_INFO3("[HIDS]: report[%d]: report_id 0x%x, report_type 0x%x",
                            i, hids_client_table[conn_id].report[i].report_ref.report_id,
                            hids_client_table[conn_id].report[i].report_ref.report_type);
        PROFILE_PRINT_INFO3("            hdl_report_value 0x%x, hdl_report_cccd 0x%x, hdl_report_reference 0x%x",
                            hids_client_table[conn_id].report[i].hdl_report_value,
                            hids_client_table[conn_id].report[i].hdl_report_cccd,
                            hids_client_table[conn_id].report[i].hdl_report_reference);
    }
    PROFILE_PRINT_INFO5("[HIDS]: read_flags 0x%x, protocol_mode 0x%x, hids_info %b, report_map[%d] %b",
                        hids_client_table[conn_id].read_flags,
                        hids_client_table[conn_id].protocol_mode,
                        TRACE_BINARY(4, &hids_client_table[conn_id].hids_info),
                        hids_client_table[conn_id].report_map_len,
                        TRACE_BINARY(hids_client_table[conn_id].report_map_len, hids_client_table[conn_id].p_report_map));

}

bool hids_read(uint8_t conn_id, T_HIDS_READ_TYPE read_type)
{
    uint16_t handle = 0;
    PROFILE_PRINT_INFO1("[HIDS]: hids_read: type %d", read_type);
    if (conn_id >= hids_client_link_num)
    {
        PROFILE_PRINT_ERROR1("[HIDS]: hids_read: failed invalid conn_id %d", conn_id);
        return false;
    }

    if (read_type == HIDS_READ_PROTOCOL_MODE)
    {
        handle = hids_client_table[conn_id].hdl_cache[HDL_HIDS_CHAR_PROTOCOL_MODE];
    }
    else if (read_type == HIDS_READ_REPORT_MAP)
    {
        handle = hids_client_table[conn_id].hdl_cache[HDL_HIDS_CHAR_REPORT_MAP];
    }
    else if (read_type == HIDS_READ_HID_INFO)
    {
        handle = hids_client_table[conn_id].hdl_cache[HDL_HIDS_CHAR_HID_INFO];
    }

    if (handle)
    {
        if (client_attr_read(conn_id, hids_client_id, handle) == GAP_CAUSE_SUCCESS)
        {
            return true;
        }
    }
    PROFILE_PRINT_ERROR0("[HIDS]: hids_read: failed");
    return false;
}

void hids_start_read(uint8_t conn_id, uint16_t cause)
{
    bool cb_flag = false;
    T_HIDS_CLIENT_CB_DATA cb_data;
    cb_data.cb_type = HIDS_CLIENT_CB_TYPE_DISC_STATE;

    PROFILE_PRINT_INFO2("[HIDS]: hids_start_read: read_proc_flag 0x%x, cause 0x%x",
                        hids_client_table[conn_id].read_proc_flag, cause);
    if (conn_id >= hids_client_link_num)
    {
        PROFILE_PRINT_ERROR1("[HIDS]: hids_start_read: failed invalid conn_id %d", conn_id);
        return;
    }

    if (hids_client_table[conn_id].report_idx == hids_client_table[conn_id].report_num)
    {
        hids_client_table[conn_id].read_proc_flag &= ~HIDS_READ_REPORT_REF_BIT;
        hids_client_table[conn_id].report_idx = 0;
    }

    if ((cause == GAP_SUCCESS) && (hids_client_table[conn_id].read_proc_flag != 0))
    {
        do
        {
            if (hids_client_table[conn_id].read_proc_flag & HIDS_READ_PROTOCOL_MODE_BIT)
            {
                hids_client_table[conn_id].read_proc_flag &= ~HIDS_READ_PROTOCOL_MODE_BIT;
                if (hids_read(conn_id, HIDS_READ_PROTOCOL_MODE))
                {
                    break;
                }
            }
            if (hids_client_table[conn_id].read_proc_flag & HIDS_READ_HID_INFO_BIT)
            {
                hids_client_table[conn_id].read_proc_flag &= ~HIDS_READ_HID_INFO_BIT;
                if (hids_read(conn_id, HIDS_READ_HID_INFO) == false)
                {
                    cb_flag = true;
                    hids_client_table[conn_id].disc_state = DISC_HIDS_FAILED;
                }
                break;
            }

            if (hids_client_table[conn_id].read_proc_flag & HIDS_READ_REPORT_MAP_BIT)
            {
                hids_client_table[conn_id].read_proc_flag &= ~HIDS_READ_REPORT_MAP_BIT;
                if (hids_read(conn_id, HIDS_READ_REPORT_MAP) == false)
                {
                    cb_flag = true;
                    hids_client_table[conn_id].disc_state = DISC_HIDS_FAILED;
                }
                break;
            }
            if (hids_client_table[conn_id].read_proc_flag & HIDS_READ_REPORT_REF_BIT)
            {
                uint16_t handle =
                    hids_client_table[conn_id].report[hids_client_table[conn_id].report_idx].hdl_report_reference;
                if (handle)
                {
                    if (client_attr_read(conn_id, hids_client_id, handle) == GAP_CAUSE_SUCCESS)
                    {
                        break;
                    }
                }
                cb_flag = true;
                hids_client_table[conn_id].disc_state = DISC_HIDS_FAILED;
            }
        }
        while (0);
    }
    else
    {
        if (cause == GAP_SUCCESS)
        {
            cb_flag = true;
            hids_client_table[conn_id].disc_state = DISC_HIDS_DONE;
        }
        else
        {
            cb_flag = true;
            hids_client_table[conn_id].disc_state = DISC_HIDS_FAILED;
        }
    }

    /* Send discover state to application if needed. */
    if (cb_flag && hids_client_cb)
    {
        cb_data.cb_content.disc_state = hids_client_table[conn_id].disc_state;
        (*hids_client_cb)(hids_client_id, conn_id, &cb_data);
    }
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
    static T_USER_CMD_PARSE_RESULT cmd_hidhdl(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        uint8_t conn_id = p_parse_value->dw_param[0];
        T_HIDS_CLIENT_STORAGE_INFO hids_client_info;
        bool ret = hids_get_hdl_cache(conn_id, (&hids_client_info));
        ......
    }
 * \endcode
  */
bool hids_get_hdl_cache(uint8_t conn_id, T_HIDS_CLIENT_STORAGE_INFO *p_storage_info)
{
    if (conn_id >= hids_client_link_num)
    {
        PROFILE_PRINT_ERROR1("[HIDS]: hids_get_hdl_cache: failed invalid conn_id %d", conn_id);
        return false;
    }
    if (hids_client_table[conn_id].disc_state != DISC_HIDS_DONE)
    {
        PROFILE_PRINT_ERROR1("[HIDS]: hids_get_hdl_cache: failed invalid state %d",
                             hids_client_table[conn_id].disc_state);
        return false;
    }

    memcpy(p_storage_info, (&(hids_client_table[conn_id].read_flags)),
           sizeof(T_HIDS_CLIENT_STORAGE_INFO));
    p_storage_info->read_flags &= ~HIDS_READ_REPORT_MAP_BIT;
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
        if (app_srvs_table.srv_found_flags & APP_DISCOV_HID_FLAG)
        {
            hids_set_hdl_cache(conn_id, (&(app_srvs_table.hids_client_info)));
        }
        if (!(app_srvs_table.hids_client_info.read_flags & HIDS_READ_REPORT_MAP_BIT))
        {
            APP_PRINT_INFO0("app_discov_services: read report map");
            hids_read_report_map(conn_id);
        }
        ......
    }
 * \endcode
  */
bool hids_set_hdl_cache(uint8_t conn_id, T_HIDS_CLIENT_STORAGE_INFO *p_storage_info)
{
    if (conn_id >= hids_client_link_num)
    {
        PROFILE_PRINT_ERROR1("[HIDS]: hids_set_hdl_cache: failed invalid conn_id %d", conn_id);
        return false;
    }
    if (hids_client_table[conn_id].disc_state != DISC_HIDS_IDLE)
    {
        PROFILE_PRINT_ERROR1("[HIDS]: hids_set_hdl_cache: failed invalid state %d",
                             hids_client_table[conn_id].disc_state);
        return false;
    }

    memcpy((&(hids_client_table[conn_id].read_flags)), p_storage_info,
           sizeof(T_HIDS_CLIENT_STORAGE_INFO));
    hids_client_table[conn_id].disc_state = DISC_HIDS_DONE;
    return true;
}

bool hids_read_report_map(uint8_t conn_id)
{
    if (conn_id >= hids_client_link_num)
    {
        PROFILE_PRINT_ERROR1("[HIDS]: hids_read_report_map: failed invalid conn_id %d", conn_id);
        return false;
    }
    if (hids_client_table[conn_id].disc_state != DISC_HIDS_DONE)
    {
        PROFILE_PRINT_ERROR1("[HIDS]: hids_read_report_map: failed invalid state %d",
                             hids_client_table[conn_id].disc_state);
        return false;
    }
    return hids_read(conn_id, HIDS_READ_REPORT_MAP);
}

bool hids_handle_report_map(uint8_t conn_id, uint16_t value_size, uint8_t *p_value)
{
    if (hids_client_table[conn_id].p_report_map != NULL)
    {
        os_mem_free(hids_client_table[conn_id].p_report_map);
    }

    hids_client_table[conn_id].p_report_map = os_mem_zalloc(RAM_TYPE_DATA_ON, value_size);
    if (hids_client_table[conn_id].p_report_map != NULL)
    {
        memcpy(hids_client_table[conn_id].p_report_map, p_value, value_size);
        hids_client_table[conn_id].report_map_len = value_size;
        hids_client_table[conn_id].read_flags |= HIDS_READ_REPORT_MAP_BIT;
        PROFILE_PRINT_INFO0("[HIDS]: hids_handle_report_map: handle report map success");
        return true;
    }
    else
    {
        PROFILE_PRINT_ERROR0("[HIDS]: hids_handle_report_map: handle report map failed, allocate memory failed");
        return false;
    }
}

bool hids_set_report_map(uint8_t conn_id, uint16_t value_size, uint8_t *p_value)
{
    if (conn_id >= hids_client_link_num)
    {
        PROFILE_PRINT_ERROR1("[HIDS]: hids_set_report_map: failed invalid conn_id %d", conn_id);
        return false;
    }
    if (hids_client_table[conn_id].disc_state != DISC_HIDS_DONE)
    {
        PROFILE_PRINT_ERROR1("[HIDS]: hids_set_report_map: failed invalid state %d",
                             hids_client_table[conn_id].disc_state);
        return false;
    }

    if (value_size != 0)
    {
        if (hids_handle_report_map(conn_id, value_size, p_value))
        {
            PROFILE_PRINT_INFO0("[HIDS]: hids_set_report_map: configuration of report map success");
            return true;
        }
        else
        {
            PROFILE_PRINT_ERROR0("[HIDS]: hids_set_report_map: configuration of report map failed");
            return false;
        }
    }
    else
    {
        PROFILE_PRINT_ERROR0("[HIDS]: hids_set_report_map: configuration of report map failed, invalid size");
        return false;
    }
}

static bool hids_client_start_char_discovery(uint8_t conn_id)
{
    uint16_t start_handle;
    uint16_t end_handle;

    PROFILE_PRINT_INFO0("[HIDS]: hids_client_start_char_discovery");
    start_handle = hids_client_table[conn_id].hdl_cache[HDL_HIDS_SRV_START];
    end_handle = hids_client_table[conn_id].hdl_cache[HDL_HIDS_SRV_END];
    if (client_all_char_discovery(conn_id, hids_client_id, start_handle,
                                  end_handle) == GAP_CAUSE_SUCCESS)
    {
        return true;
    }
    return false;
}

static bool hids_client_start_char_descriptor_discovery(uint8_t conn_id)
{
    uint16_t start_handle;
    uint16_t end_handle;

    PROFILE_PRINT_INFO0("[HIDS]: hids_client_start_char_descriptor_discovery");
    start_handle = hids_client_table[conn_id].hdl_cache[HDL_HIDS_SRV_START];
    end_handle = hids_client_table[conn_id].hdl_cache[HDL_HIDS_SRV_END];
    if (client_all_char_descriptor_discovery(conn_id, hids_client_id, start_handle,
                                             end_handle) == GAP_CAUSE_SUCCESS)
    {
        return true;
    }
    return false;
}

static void hids_client_discover_state_cb(uint8_t conn_id,  T_DISCOVERY_STATE discovery_state)
{
    bool cb_flag = false;
    T_HIDS_CLIENT_CB_DATA cb_data;
    cb_data.cb_type = HIDS_CLIENT_CB_TYPE_DISC_STATE;

    PROFILE_PRINT_INFO1("[HIDS]: hids_client_discover_state_cb: discovery_state = %d", discovery_state);
    if (hids_client_table[conn_id].disc_state == DISC_HIDS_START)
    {
        switch (discovery_state)
        {
        case DISC_STATE_SRV_DONE:
            /* Indicate that service handle found. Start discover characteristic. */
            if ((hids_client_table[conn_id].hdl_cache[HDL_HIDS_SRV_START] != 0)
                || (hids_client_table[conn_id].hdl_cache[HDL_HIDS_SRV_END] != 0))
            {
                if (hids_client_start_char_discovery(conn_id) == false)
                {
                    hids_client_table[conn_id].disc_state = DISC_HIDS_FAILED;
                    cb_flag = true;
                }
            }
            /* No HID handle found. Discover procedure complete. */
            else
            {
                hids_client_table[conn_id].disc_state = DISC_HIDS_FAILED;
                cb_flag = true;
            }
            break;
        case DISC_STATE_CHAR_DONE:
            if (hids_client_table[conn_id].hdl_cache[HDL_HIDS_CHAR_REPORT_MAP] != 0)
            {
                //discovery cccd
                if (hids_client_start_char_descriptor_discovery(conn_id) == false)
                {
                    hids_client_table[conn_id].disc_state = DISC_HIDS_FAILED;
                    cb_flag = true;
                }
            }
            else
            {
                hids_client_table[conn_id].disc_state = DISC_HIDS_DONE;
                cb_flag = true;
            }
            break;

        case DISC_STATE_CHAR_DESCRIPTOR_DONE:
            hids_client_table[conn_id].read_proc_flag = HIDS_READ_HID_INFO_BIT | HIDS_READ_PROTOCOL_MODE_BIT |
                                                        HIDS_READ_REPORT_MAP_BIT | HIDS_READ_REPORT_REF_BIT;
            hids_start_read(conn_id, GAP_SUCCESS);
            break;

        case DISC_STATE_FAILED:
            hids_client_table[conn_id].disc_state = DISC_HIDS_FAILED;
            cb_flag = true;
            break;

        default:
            PROFILE_PRINT_ERROR0("Invalid Discovery State!");
            break;
        }
    }

    /* Send discover state to application if needed. */
    if (cb_flag && hids_client_cb)
    {
        cb_data.cb_content.disc_state = hids_client_table[conn_id].disc_state;
        (*hids_client_cb)(hids_client_id, conn_id, &cb_data);
    }
    return;
}


static void hids_client_discover_result_cb(uint8_t conn_id,  T_DISCOVERY_RESULT_TYPE result_type,
                                           T_DISCOVERY_RESULT_DATA result_data)
{
    PROFILE_PRINT_INFO1("[HIDS]: hids_client_discover_result_cb: result_type = %d", result_type);
    if (hids_client_table[conn_id].disc_state == DISC_HIDS_START)
    {
        uint16_t handle;
        uint16_t *hdl_cache;
        hdl_cache = hids_client_table[conn_id].hdl_cache;

        switch (result_type)
        {
        case DISC_RESULT_SRV_DATA:
            hids_client_table[conn_id].hdl_cache[HDL_HIDS_SRV_START] =
                result_data.p_srv_disc_data->att_handle;
            hids_client_table[conn_id].hdl_cache[HDL_HIDS_SRV_END] =
                result_data.p_srv_disc_data->end_group_handle;
            break;

        case DISC_RESULT_CHAR_UUID16:
            {
                handle = result_data.p_char_uuid16_disc_data->value_handle;
                switch (result_data.p_char_uuid16_disc_data->uuid16)
                {
                case GATT_UUID_CHAR_HID_INFO:
                    hdl_cache[HDL_HIDS_CHAR_HID_INFO] = handle;
                    break;

                case GATT_UUID_CHAR_REPORT_MAP:
                    hdl_cache[HDL_HIDS_CHAR_REPORT_MAP] = handle;
                    break;

                case GATT_UUID_CHAR_HID_CONTROL_POINT:
                    hdl_cache[HDL_HIDS_CHAR_HID_CONTROL_POINT] = handle;
                    break;

                case GATT_UUID_CHAR_PROTOCOL_MODE:
                    hdl_cache[HDL_HIDS_CHAR_PROTOCOL_MODE] = handle;
                    break;

                case GATT_UUID_CHAR_REPORT:
                    {
                        hids_report_add(conn_id, handle);
                    }
                    break;

                default:
                    /* have no intrest on this handle. */
                    break;
                }
            }
            break;

        case DISC_RESULT_CHAR_DESC_UUID16:
            handle = result_data.p_char_desc_uuid16_disc_data->handle;
            if (result_data.p_char_desc_uuid16_disc_data->uuid16 == GATT_UUID_CHAR_CLIENT_CONFIG)
            {
                T_HIDS_CLIENT_REPORT *p_report = hids_report_check_desc_handle(conn_id, handle);
                if (p_report != NULL)
                {
                    p_report->hdl_report_cccd = handle;
                    PROFILE_PRINT_INFO1("[HIDS]: Found report cccd handle 0x%x", handle);
                }
            }
            else if (result_data.p_char_desc_uuid16_disc_data->uuid16 == GATT_UUID_CHAR_REPORT_REFERENCE)
            {
                T_HIDS_CLIENT_REPORT *p_report = hids_report_check_desc_handle(conn_id, handle);
                if (p_report != NULL)
                {
                    p_report->hdl_report_reference = handle;
                    PROFILE_PRINT_INFO1("[HIDS]: Found report ref handle 0x%x", handle);

                }
            }
            break;

        default:
            PROFILE_PRINT_ERROR0("[HIDS]: Invalid Discovery Result Type!");
            break;
        }
    }

    return;
}

static void hids_client_write_result_cb(uint8_t conn_id, T_GATT_WRITE_TYPE type,
                                        uint16_t handle,
                                        uint16_t cause,
                                        uint8_t credits)
{
    T_HIDS_CLIENT_CB_DATA cb_data;
    cb_data.cb_type = HIDS_CLIENT_CB_TYPE_WRITE_RESULT;

    PROFILE_PRINT_INFO2("[HIDS]: hids_client_write_result_cb: handle 0x%x, cause 0x%x", handle, cause);
    cb_data.cb_content.write_result.cause = cause;

    if (hids_client_table[conn_id].enable_cccd_proc)
    {
        if (cause == GAP_SUCCESS)
        {
            uint8_t i = hids_client_table[conn_id].report_idx;
            uint16_t cccd_bits = 1;

            if (handle == hids_client_table[conn_id].report[i].hdl_report_cccd)
            {
                i++;
                for (; i < hids_client_table[conn_id].report_num; i++)
                {
                    if (hids_client_table[conn_id].report[i].hdl_report_cccd != 0)
                    {
                        if (client_attr_write(conn_id, hids_client_id, GATT_WRITE_TYPE_REQ,
                                              hids_client_table[conn_id].report[i].hdl_report_cccd,
                                              sizeof(uint16_t), (uint8_t *)&cccd_bits) == GAP_CAUSE_SUCCESS)
                        {
                            hids_client_table[conn_id].report_idx = i;
                            hids_client_table[conn_id].enable_cccd_proc = true;
                            return;
                        }
                        else
                        {
                            cb_data.cb_content.write_result.cause = GATT_ERR | GATT_ERR_INVALID_PARAM;
                            PROFILE_PRINT_ERROR0("[HIDS]: hids_enable_all_cccd: call client_attr_write failed");
                        }
                    }
                }
            }
            else
            {
                cb_data.cb_content.write_result.cause = GATT_ERR | GATT_ERR_INVALID_PARAM;
                PROFILE_PRINT_ERROR2("[HIDS]: hids_client_write_result_cb: invalid cccd handle 0x%x, hande 0x%x",
                                     handle, hids_client_table[conn_id].report[i].hdl_report_cccd);
            }
        }
        hids_client_table[conn_id].report_idx = 0;
        hids_client_table[conn_id].enable_cccd_proc = false;
        cb_data.cb_content.write_result.type = HIDS_WRITE_ALL_REPORT_CCCD;
    }
    else
    {
        T_HIDS_CLIENT_REPORT *p_report = hids_report_find_by_handle(conn_id, HDIS_REPORT_VALUE_HANDLE,
                                                                    handle);
        if (p_report != NULL)
        {
            cb_data.cb_content.write_result.type = HIDS_WRITE_REPORT;
            cb_data.cb_content.write_result.report_ref = p_report->report_ref;
        }
        else
        {
            return;
        }
    }

    if (hids_client_cb)
    {
        (*hids_client_cb)(hids_client_id, conn_id, &cb_data);
    }
    return;
}

static void hids_client_read_result_cb(uint8_t conn_id,  uint16_t cause,
                                       uint16_t handle, uint16_t value_size, uint8_t *p_value)
{
    uint16_t *hdl_cache;
    hdl_cache = hids_client_table[conn_id].hdl_cache;
    if (hids_client_table[conn_id].disc_state == DISC_HIDS_START)
    {
        if (cause == ATT_SUCCESS)
        {
            if (handle == hdl_cache[HDL_HIDS_CHAR_HID_INFO])
            {
                if (value_size == 4)
                {
                    memcpy(&(hids_client_table[conn_id].hids_info), p_value, value_size);
                    hids_client_table[conn_id].read_flags |= HIDS_READ_HID_INFO_BIT;
                    PROFILE_PRINT_INFO0("[HIDS]: Read hid info success");
                }
                else
                {
                    cause = GATT_ERR | GATT_ERR_INVALID_PARAM;
                    PROFILE_PRINT_ERROR0("[HIDS]: Read hid info failed, invalid size");
                }
            }
            else if (handle == hdl_cache[HDL_HIDS_CHAR_REPORT_MAP])
            {
                if (value_size != 0)
                {
                    if (!(hids_handle_report_map(conn_id, value_size, p_value)))
                    {
                        cause = GATT_ERR | GATT_ERR_INVALID_PARAM;
                    }
                }
                else
                {
                    cause = GATT_ERR | GATT_ERR_INVALID_PARAM;
                    PROFILE_PRINT_ERROR0("[HIDS]: Read report map failed, invalid size");
                }
            }
            else if (handle == hdl_cache[HDL_HIDS_CHAR_PROTOCOL_MODE])
            {
                if (value_size == 1)
                {
                    hids_client_table[conn_id].protocol_mode = *p_value;
                    hids_client_table[conn_id].read_flags |= HIDS_READ_PROTOCOL_MODE_BIT;
                    PROFILE_PRINT_INFO0("[HIDS]: Read protocol mode success");
                }
                else
                {
                    cause = GATT_ERR | GATT_ERR_INVALID_PARAM;
                    PROFILE_PRINT_ERROR0("[HIDS]: Read protocol mode failed, invalid size");
                }
            }
            else
            {
                T_HIDS_CLIENT_REPORT *p_report = hids_report_find_by_handle(conn_id, HDIS_REPORT_REF_HANDLE,
                                                                            handle);
                if (p_report != NULL && value_size == 2)
                {
                    p_report->report_ref.report_id = *p_value;
                    p_report->report_ref.report_type = (T_HIDS_REPORT_TYPE) * (p_value + 1);
                }
                else
                {
                    cause = GATT_ERR | GATT_ERR_INVALID_PARAM;
                    PROFILE_PRINT_ERROR0("[HIDS]: Read report ref, invalid size");
                }
                hids_client_table[conn_id].report_idx++;
            }
            hids_start_read(conn_id, cause);
        }
        else
        {
            hids_start_read(conn_id, cause);
        }
    }
    else if (hids_client_table[conn_id].disc_state == DISC_HIDS_DONE)
    {
        if (handle == hdl_cache[HDL_HIDS_CHAR_REPORT_MAP])
        {
            T_HIDS_CLIENT_CB_DATA cb_data;

            if (cause == ATT_SUCCESS)
            {
                if (value_size != 0)
                {
                    if (!(hids_handle_report_map(conn_id, value_size, p_value)))
                    {
                        cause = GATT_ERR | GATT_ERR_INVALID_PARAM;
                    }
                }
                else
                {
                    cause = GATT_ERR | GATT_ERR_INVALID_PARAM;
                    PROFILE_PRINT_ERROR0("[HIDS]: Read report map failed, invalid size");
                }
            }

            cb_data.cb_type = HIDS_CLIENT_CB_TYPE_READ_RESULT;
            cb_data.cb_content.read_result.type = HIDS_READ_RESULT_REPORT_MAP;
            cb_data.cb_content.read_result.cause = cause;
            (*hids_client_cb)(hids_client_id, conn_id, &cb_data);
        }
    }
    return;
}

static T_APP_RESULT hids_client_notify_ind_cb(uint8_t conn_id, bool notify, uint16_t handle,
                                              uint16_t value_size, uint8_t *p_value)
{
    T_APP_RESULT app_result = APP_RESULT_SUCCESS;
    T_HIDS_CLIENT_CB_DATA cb_data;
    T_HIDS_CLIENT_REPORT *p_report;
    cb_data.cb_type = HIDS_CLIENT_CB_TYPE_NOTIF_RESULT;

    p_report = hids_report_find_by_handle(conn_id, HDIS_REPORT_VALUE_HANDLE, handle);
    if (p_report == NULL)
    {
        return APP_RESULT_SUCCESS;
    }
    cb_data.cb_content.notify_data.type = HIDS_REPORT_NOTIFY;
    cb_data.cb_content.notify_data.data.report_id = p_report->report_ref.report_id;
    cb_data.cb_content.notify_data.data.value_size = value_size;
    cb_data.cb_content.notify_data.data.p_value = p_value;

    if (hids_client_cb)
    {
        app_result = (*hids_client_cb)(hids_client_id, conn_id, &cb_data);
    }

    return app_result;
}

static void hids_client_disc_cb(uint8_t conn_id)
{
    PROFILE_PRINT_INFO0("[HIDS]: hids_client_disc_cb.");
    if (conn_id >= hids_client_link_num)
    {
        PROFILE_PRINT_ERROR1("[HIDS]: hids_client_disc_cb: failed invalid conn_id %d", conn_id);
        return;
    }
    if (hids_client_table[conn_id].p_report_map != NULL)
    {
        os_mem_free(hids_client_table[conn_id].p_report_map);
    }
    memset(&hids_client_table[conn_id], 0, sizeof(T_HIDS_CLIENT_LINK));
    return;
}

/**
 * @brief HID Client Callbacks.
*/
const T_FUN_CLIENT_CBS hids_client_cbs =
{
    hids_client_discover_state_cb,   //!< Discovery State callback function pointer
    hids_client_discover_result_cb,  //!< Discovery result callback function pointer
    hids_client_read_result_cb,      //!< Read response callback function pointer
    hids_client_write_result_cb,     //!< Write result callback function pointer
    hids_client_notify_ind_cb,       //!< Notify Indicate callback function pointer
    hids_client_disc_cb              //!< Link disconnection callback function pointer
};

/**
  * @brief      Add hid client to application.
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
        hids_client_id = hid_add_client(app_client_callback, APP_MAX_LINKS);
    }
  * \endcode
  */
T_CLIENT_ID hids_add_client(P_FUN_GENERAL_APP_CB app_cb, uint8_t link_num)
{
    uint16_t size;
    if (link_num > HIDS_CLIENT_MAX_LINKS)
    {
        PROFILE_PRINT_ERROR1("[HIDS]: hids_add_client: invalid link_num %d", link_num);
        return 0xff;
    }
    if (false == client_register_spec_client_cb(&hids_client_id, &hids_client_cbs))
    {
        hids_client_id = CLIENT_PROFILE_GENERAL_ID;
        PROFILE_PRINT_ERROR0("[HIDS]: hids_add_client:register fail");
        return hids_client_id;
    }
    PROFILE_PRINT_INFO1("[HIDS]: hids_add_client: client id %d", hids_client_id);

    /* register callback for profile to inform application that some events happened. */
    hids_client_cb = app_cb;
    hids_client_link_num = link_num;
    size = hids_client_link_num * sizeof(T_HIDS_CLIENT_LINK);
    hids_client_table = os_mem_zalloc(RAM_TYPE_DATA_ON, size);

    return hids_client_id;
}

