/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    dfu_client.c
  * @brief   dfu service client source file.
  * @details
  * @author  bill
  * @date    2017-8-28
  * @version v1.0
  ******************************************************************************
  * @attention
  * <h2><center>&copy; COPYRIGHT 2015 Realtek Semiconductor Corporation</center></h2>
  ******************************************************************************
  */
#include <string.h>
#include <stdbool.h>
#include "gap_conn_le.h"
#include "gap_scan.h"
#include "aes_api.h"
#include "mem_types.h"
#include "os_mem.h"
#include "os_sched.h"
#include "otp.h"
#include "dfu_api.h"
#include "patch_header_check.h"
#include "trace.h"
#include "dfu_client.h"
#include "mem_config.h"
#include "board.h" //include OTA config


/*============================================================================*
 *                              Macros
 *============================================================================*/

/**
 * @brief  dfu client Link control block definition.
 */
typedef struct
{
    T_DFU_DISC_STATE disc_state;
    uint16_t         properties; //for what
    uint16_t         hdl_cache[HDL_DFU_CACHE_LEN];
} T_DFU_LINK, *P_DFU_LINK;


/*============================================================================*
 *                              Local varibles
 *============================================================================*/
static P_DFU_LINK dfu_table;
static uint8_t dfu_link_num;

/**<  Callback used to send data to app from Dfu client layer. */
static P_FUN_GENERAL_APP_CB pf_dfu_client_cb = NULL;
static P_FUN_GENERAL_APP_CB pf_dfu_client_app_info_cb = NULL;

P_FUN_DFU_CLIENT_INIT_APP_CB pf_dfu_client_init __attribute__((weak)) = NULL;
/**<  Dfu client ID. */
static T_CLIENT_ID dfu_client = CLIENT_PROFILE_GENERAL_ID;

/* for record data that target send by notification*/
static uint8_t dfu_temp_cp_notify_msg[20];  //for cache control point masg
static uint8_t dfu_temp_cp_notify_msg_len;

static T_DFU_CTX dfu_client_ctx;
static T_DFU_CLIENT_FSM_WRITE dfu_fsm_write = DFU_FSM_WRITE_IDLE;
static uint8_t buf_check_retry_cnt = 0;

const uint8_t dfu_data_uuid[16] = {GATT_UUID128_DFU_DATA};
const uint8_t dfu_control_point_uuid[16] = {GATT_UUID128_DFU_CONTROL_POINT};
const uint8_t dfu_service_uuid[16] = {GATT_UUID128_DFU_SERVICE};


/*============================================================================*
 *                             Functions
 *============================================================================*/

static void swap_buf(uint8_t *dst, const uint8_t *src, uint16_t len)
{
    int i;
    if (dst == NULL || src == NULL)
    {
        return;
    }

    for (i = 0; i < len; i++)
    {
        dst[len - 1 - i] = src[i];
    }
}

/**
 * @brief  get 16bit data swapped.
 *
 * @param  val          16bit data to be swapped.
 * @return value after being swapped.
*/
static uint16_t swap_16(uint16_t val)
{
    uint16_t result;

    /* Idiom Recognition for REV16 */
    result = ((val & 0xff) << 8) | ((val & 0xff00) >> 8);

    return result;
}

bool dfu_encrypt(uint8_t image[16])
{
    bool ret;
    uint8_t image_tmp[16] = {0};
    swap_buf(image_tmp, image, 16);
    ret = aes256_ecb_encrypt_msb2lsb(image_tmp, (uint8_t *)OTP->aes_key, image_tmp);
    swap_buf(image, image_tmp, 16);
    return ret;
}

uint16_t dfu_client_temp_crc_cal(uint8_t *p_temp, uint32_t length, uint16_t init_cksum16)
{
    uint16_t temp_checksum16 = init_cksum16;
    uint16_t *p16 = (uint16_t *)p_temp;

    for (uint32_t i = 0; i < length / 2; ++i)
    {
        temp_checksum16 = temp_checksum16 ^ (*p16);
        ++p16;
    }
    return temp_checksum16;
}

uint16_t dfu_client_buf_crc_cal(uint8_t *buf, uint32_t length)
{
    uint16_t checksum16 = dfu_client_temp_crc_cal(buf, length, 0);
    return swap_16(checksum16);
}

void dfu_client_set_dfu_ctx_value(T_DFU_SET_CLIENT_CTX_TYPE type, void *p_value)
{
    DFU_PRINT_INFO1("dfu_client_set_dfu_ctx_value: type=%d", type);
    if (p_value)
    {
        switch (type)
        {
        case DFU_SET_FEATURE:
            dfu_client_ctx.dfu_feature = *(T_DFU_CLIENT_FEATURE_TYPE *)p_value;
            break;
        case DFU_SET_MODE_VALUE:
            dfu_client_ctx.ota_mode.value = *(uint8_t *)p_value;
            break;
        case DFU_SET_MAX_BUF_SIZE:
            dfu_client_ctx.max_buffer_size = *(uint32_t *)p_value;
            break;
        case DFU_SET_DFU_IMG_ADDR:
            dfu_client_ctx.dfu_img_start_addr = *(uint32_t *)p_value;
            DFU_PRINT_INFO1("dfu_client_set_dfu_ctx_value: dfu_img_start_addr=0x%x",
                            dfu_client_ctx.dfu_img_start_addr);
            break;
        case DFU_SET_DFU_IMG_VER:
            dfu_client_ctx.dfu_img_version = *(uint32_t *)p_value;
            break;
        case DFU_SET_NORMAL_OTA_FLAG:
            dfu_client_ctx.is_normal_ota = *(bool *)p_value;
            break;
        default:
            break;
        }
    }
}

/**
  * @brief  Used by application, to set the handles in Dfu handle cache.
  * @param  handle_type: handle types of this specific profile.
  * @param  handle_value: handle value to set.
  * @retval true--set success.
  *         flase--set failed.
  */
bool dfu_client_set_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len)
{
    if (conn_id >= dfu_link_num)
    {
        DFU_PRINT_ERROR1("dfu_client_set_hdl_cache: failed invalid conn_id %d", conn_id);
        return false;
    }
    if (dfu_table[conn_id].disc_state != DISC_DFU_IDLE)
    {
        DFU_PRINT_ERROR1("dfu_client_set_hdl_cache: failed invalid state %d",
                         dfu_table[conn_id].disc_state);
        return false;
    }
    if (len != sizeof(uint16_t) * HDL_DFU_CACHE_LEN)
    {
        DFU_PRINT_ERROR1("dfu_client_set_hdl_cache: failed invalid len %d", len);
        return false;
    }
    memcpy(dfu_table[conn_id].hdl_cache, p_hdl_cache, len);
    dfu_table[conn_id].disc_state = DISC_DFU_DONE;
    return true;
}

/**
  * @brief  Used by application, to get handle cache.
  * @param[in]  conn_id connection ID.
  * @param[in]  p_hdl_cache pointer of the handle cache table
  * @param[in]  len the length of handle cache table
  * @retval true success.
  * @retval false failed.
  */
bool dfu_client_get_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len)
{
    if (conn_id >= dfu_link_num)
    {
        DFU_PRINT_ERROR1("dfu_client_get_hdl_cache: failed invalid conn_id %d", conn_id);
        return false;
    }
    if (dfu_table[conn_id].disc_state != DISC_DFU_DONE)
    {
        DFU_PRINT_ERROR1("dfu_client_get_hdl_cache: failed invalid state %d",
                         dfu_table[conn_id].disc_state);
        return false;
    }
    if (len != sizeof(uint16_t) * HDL_DFU_CACHE_LEN)
    {
        DFU_PRINT_ERROR1("dfu_client_get_hdl_cache: failed invalid len %d", len);
        return false;
    }
    memcpy(p_hdl_cache, dfu_table[conn_id].hdl_cache, len);
    return true;
}


/**
  * @brief  Used by application, to start the discovery procedure of Dfu server.
  * @retval true--send request to upper stack success.
  *         flase--send request to upper stack failed.
  */
bool dfu_client_start_discovery(uint8_t conn_id)
{
    if (conn_id >= dfu_link_num)
    {
        DFU_PRINT_ERROR1("dfu_client_start_discovery: failed invalid conn_id %d", conn_id);
        return false;
    }

    DFU_PRINT_INFO0("dfu_client_start_discovery!");
    /* First clear handle cache. */
    memset(&dfu_table[conn_id], 0, sizeof(T_DFU_LINK));
    dfu_table[conn_id].disc_state = DISC_DFU_START;
    if (client_by_uuid128_srv_discovery(conn_id, dfu_client,
                                        (uint8_t *)dfu_service_uuid) == GAP_CAUSE_SUCCESS)
    {
        return true;
    }
    return false;
}


/**
  * @brief  Used by application, read data from server by using handles.
  * @param  readCharType: one of characteristic that has the readable property.
  * @retval true--send request to upper stack success.
  *         flase--send request to upper stack failed.
  */
bool dfu_client_read_by_handle(uint8_t conn_id, T_DFU_READ_TYPE type)
{
    bool hdl_valid = false;
    uint16_t handle = 0;

    if (conn_id >= dfu_link_num)
    {
        DFU_PRINT_ERROR1("dfu_client_read_by_handle: failed invalid conn_id %d", conn_id);
        return false;
    }
    switch (type)
    {
    case DFU_READ_CP_CCCD:
        if (dfu_table[conn_id].hdl_cache[DFU_READ_CP_CCCD])
        {
            handle = dfu_table[conn_id].hdl_cache[DFU_READ_CP_CCCD];
            hdl_valid = true;
        }
        break;
    default:
        return false;
    }

    if (hdl_valid)
    {
        if (client_attr_read(conn_id, dfu_client, handle) == GAP_CAUSE_SUCCESS)
        {
            return true;
        }
    }

    DFU_PRINT_INFO1("dfu_client_read_by_handle fail! type=%d", type);
    return false;
}

/**
  * @brief  Used by application, read data from server by using UUIDs.
  * @param  readCharType: one of characteristic that has the readable property.
  * @retval true--send request to upper stack success.
  *         false--send request to upper stack failed.
  */
bool dfu_client_read_by_uuid(uint8_t conn_id, T_DFU_READ_TYPE type)
{
    uint16_t start_handle;
    uint16_t end_handle;
    uint16_t uuid16;
    switch (type)
    {
    case DFU_READ_CP_CCCD:
        start_handle = dfu_table[conn_id].hdl_cache[HDL_DFU_CP] + 1;
        end_handle = dfu_table[conn_id].hdl_cache[HDL_DFU_SRV_END];
        uuid16 = GATT_UUID_CHAR_CLIENT_CONFIG;
        break;
    default:
        return false;
    }

    if (client_attr_read_using_uuid(conn_id, dfu_client, start_handle, end_handle, uuid16,
                                    NULL) == GAP_CAUSE_SUCCESS)
    {
        return true;
    }
    DFU_PRINT_INFO1("dfu_client_read_by_uuid fail! type=%d", type);
    return false;
}

/**
  * @brief  Used by application, to enable or disable the notification of peer server's V3 Notify Characteristic.
  * @param  command: 0--disable the notification, 1--enable the notification.
  * @retval true--send request to upper stack success.
  *         flase--send request to upper stack failed.
  */
bool dfu_client_cp_cccd_set(uint8_t conn_id, bool command)
{
    uint16_t handle;
    uint16_t length;
    uint8_t *pdata;
    uint16_t cccd_value;
    handle = dfu_table[conn_id].hdl_cache[HDL_DFU_CP_CCCD];
    if (handle)
    {
        cccd_value = command ? 1 : 0;
        length = sizeof(uint16_t);
        pdata = (uint8_t *)&cccd_value;
        if (client_attr_write(conn_id, dfu_client, GATT_WRITE_TYPE_REQ, handle, length,
                              pdata) == GAP_CAUSE_SUCCESS)
        {
            return true;
        }
    }

    DFU_PRINT_INFO1("dfu_client_cp_cccd_set fail! handle=0x%x", handle);
    return false;
}

/**
  * @brief  Used internal, to send write request to peer server's V5 Control Point Characteristic.
  * @param  ctl_pnt_ptr: pointer of control point data to write.
  * @retval true--send request to upper stack success.
  *         flase--send request to upper stack failed.
  */
bool dfu_client_cp_write(uint8_t conn_id, uint8_t *pdata, uint16_t length)
{
    uint16_t handle;
    dfu_fsm_write = DFU_FSM_WRITE_WAIT_WRITE_RESP;
    handle = dfu_table[conn_id].hdl_cache[HDL_DFU_CP];
    if (handle)
    {
        if (client_attr_write(conn_id, dfu_client, GATT_WRITE_TYPE_REQ, handle, length,
                              pdata) == GAP_CAUSE_SUCCESS)
        {
            DFU_PRINT_INFO3("==>dfu_client_cp_write success: conn_id=%d, opcode=0x%x, len=%d",
                            conn_id, pdata[0], length);
            return true;
        }
    }

    DFU_PRINT_INFO1("==>dfu_client_cp_write fail! handle=0x%x", handle);
    return false;
}

/**
  * @brief  Used internal, to send write request to peer server's V5 Control Point Characteristic.
  * @param  ctl_pnt_ptr: pointer of control point data to write.
  * @retval true--send request to upper stack success.
  *         flase--send request to upper stack failed.
  */
bool dfu_client_data_write(uint8_t conn_id, uint8_t *pdata, uint16_t length)
{
    uint16_t handle;
    dfu_fsm_write = DFU_FSM_WRITE_DFU_DATA;
    handle = dfu_table[conn_id].hdl_cache[HDL_DFU_DATA];
    if (handle)
    {
        if (client_attr_write(conn_id, dfu_client, GATT_WRITE_TYPE_CMD, handle, length,
                              pdata) == GAP_CAUSE_SUCCESS)
        {
            DFU_PRINT_INFO2("==>dfu_client_data_write success: conn_id=%d, len=%d", conn_id, length);
            return true;
        }
    }

    DFU_PRINT_INFO1("dfu_client_data_write fail! handle=0x%x", handle);
    return false;
}

/**
  * @brief  Used internal, start the discovery of Dfu characteristics.
  *         NOTE--user can offer this interface for application if required.
  * @retval true--send request to upper stack success.
  *         flase--send request to upper stack failed.
  */
static bool dfu_client_start_all_char_discovery(uint8_t conn_id)
{
    uint16_t start_handle;
    uint16_t end_handle;

    DFU_PRINT_INFO0("dfu_client_start_all_char_discovery");
    start_handle = dfu_table[conn_id].hdl_cache[HDL_DFU_SRV_START];
    end_handle = dfu_table[conn_id].hdl_cache[HDL_DFU_SRV_END];
    return (client_all_char_discovery(conn_id, dfu_client, start_handle,
                                      end_handle) == GAP_CAUSE_SUCCESS);
}

/**
  * @brief  Used internal, start the discovery of Dfu characteristics descriptor.
  *         NOTE--user can offer this interface for application if required.
  * @retval true--send request to upper stack success.
  *         flase--send request to upper stack failed.
  */
static bool dfu_client_start_all_char_descriptor_discovery(uint8_t conn_id)
{
    uint16_t start_handle;
    uint16_t end_handle;
    DFU_PRINT_INFO0("dfu_client_start_all_char_descriptor_discovery");
    start_handle = dfu_table[conn_id].hdl_cache[HDL_DFU_CP];
    end_handle = dfu_table[conn_id].hdl_cache[HDL_DFU_SRV_END];
    return (client_all_char_descriptor_discovery(conn_id, dfu_client, start_handle,
                                                 end_handle) == GAP_CAUSE_SUCCESS);
}


/**
  * @brief  Called by profile client layer, when discover state of discovery procedure changed.
  * @param  discoveryState: current service discovery state.
  * @retval ProfileResult_Success--procedure OK.
  *         other--procedure exception.
  */
static void dfu_client_disc_state_cb(uint8_t conn_id, T_DISCOVERY_STATE disc_state)
{
    bool cb_flag = false;

    T_DFU_CLIENT_CB_DATA cb_data;
    cb_data.cb_type = DFU_CLIENT_CB_TYPE_DISC_STATE;

    DFU_PRINT_INFO1("dfu_client_disc_state_cb: disc_state = %d", disc_state);
    if (dfu_table[conn_id].disc_state == DISC_DFU_START)
    {
        switch (disc_state)
        {
        case DISC_STATE_SRV_DONE:
            /* Indicate that service handle found. Start discover characteristic. */
            if ((dfu_table[conn_id].hdl_cache[HDL_DFU_SRV_START] != 0)
                || (dfu_table[conn_id].hdl_cache[HDL_DFU_SRV_END] != 0))
            {
                DFU_PRINT_INFO0("GG2 dfu_client_disc_state_cb: Start Find all characteristic!");
                if (dfu_client_start_all_char_discovery(conn_id) == false)
                {
                    cb_data.cb_content.disc_state = DISC_DFU_FAIL;
                    cb_flag = true;
                }
            }
            /* No dfu service handle found. Discover procedure complete. */
            else
            {
                cb_data.cb_content.disc_state = DISC_DFU_FAIL;
                cb_flag = true;
            }
            break;
        case DISC_STATE_CHAR_DONE:

            //discovery cccd
            if (dfu_client_start_all_char_descriptor_discovery(conn_id) == false)
            {
                dfu_table[conn_id].disc_state = DISC_DFU_FAIL;
                cb_flag = true;
            }
            break;
        case DISC_STATE_CHAR_DESCRIPTOR_DONE:
            /* Find the next descriptor to be discovered. */
            cb_data.cb_content.disc_state = DISC_DFU_DONE;
            cb_flag = true;
            break;
        default:
            DFU_PRINT_INFO0("dfu_client_disc_state_cb: Invalid Discovery State!");
            break;
        }
    }

    if (cb_data.cb_content.disc_state == DISC_DFU_DONE)
    {
        /* Discovery Simple BLE service procedure successfully done. */
        DFU_PRINT_INFO0("==>dfu_client_disc_state_cb: discover procedure done.");
        if (dfu_client_cp_cccd_set(conn_id, 1)) //enable server notification
        {
            dfu_fsm_write = DFU_FSM_WRITE_CCCD_ENABLE;
            DFU_PRINT_INFO0("==>dfu_client_disc_state_cb: Enable CCCD.");
        }
    }
    else if (cb_data.cb_content.disc_state == DISC_DFU_FAIL)
    {
        /* Discovery Request failed. */
        DFU_PRINT_INFO0("==>dfu client hdl msg: discover request failed.");
    }
    else
    {
        //don't handle type
    }

    /* Send discover state to application if needed. */
    if (cb_flag && pf_dfu_client_cb)
    {
        (*pf_dfu_client_cb)(dfu_client, conn_id, &cb_data);
    }
}

/**
  * @brief  Called by profile client layer, when discover result fetched.
  * @param  resultType: indicate which type of value discovered in service discovery procedure.
  * @param  resultData: value discovered.
  * @retval ProfileResult_Success--procedure OK.
  *         other--procedure exception.
  */
static void dfu_client_disc_result_cb(uint8_t conn_id, T_DISCOVERY_RESULT_TYPE result_type,
                                      T_DISCOVERY_RESULT_DATA result_data)
{
    DFU_PRINT_INFO1("==>dfu_client_disc_result_cb: result_type=%d", result_type);
    switch (result_type)
    {
    case DISC_RESULT_SRV_DATA:
        /* send service handle range to application. */
        DFU_PRINT_INFO2("==>dfu_client_disc_result_cb: Primary service Found! Set dfu start_handle=0x%4x, end_handle=0x%4x",
                        result_data.p_srv_disc_data->att_handle, result_data.p_srv_disc_data->end_group_handle);
        dfu_table[conn_id].hdl_cache[HDL_DFU_SRV_START] = result_data.p_srv_disc_data->att_handle;
        dfu_table[conn_id].hdl_cache[HDL_DFU_SRV_END] = result_data.p_srv_disc_data->end_group_handle;
        break;
    case DISC_RESULT_CHAR_UUID128:
        /* we should inform intrested handles to upper application. */
        if (0 == memcmp(result_data.p_char_uuid128_disc_data->uuid128, dfu_data_uuid, 16))
        {
            DFU_PRINT_INFO1("==>dfu_client_disc_result_cb: Set dfu_data handle=0x%4x",
                            result_data.p_char_uuid128_disc_data->value_handle);
            dfu_table[conn_id].hdl_cache[HDL_DFU_DATA] = result_data.p_char_uuid128_disc_data->value_handle;
        }
        else if (0 == memcmp(result_data.p_char_uuid128_disc_data->uuid128, dfu_control_point_uuid, 16))
        {
            DFU_PRINT_INFO1("==>dfu_client_disc_result_cb: Set dfu_control_point handle=0x%4x",
                            result_data.p_char_uuid128_disc_data->value_handle);
            dfu_table[conn_id].hdl_cache[HDL_DFU_CP] = result_data.p_char_uuid128_disc_data->value_handle;
        }
        break;
    case DISC_RESULT_CHAR_DESC_UUID16:
        /* When use clientAPI_AllCharDescriptorDiscovery. */
        if (result_data.p_char_desc_uuid16_disc_data->uuid16 ==
            GATT_UUID_CHAR_CLIENT_CONFIG)  //GATT Client Characteristic Configuration descriptors uuid
        {
            uint16_t temp_handle = result_data.p_char_desc_uuid16_disc_data->handle;
            DFU_PRINT_INFO2("==>dfu_client_disc_result_cb: Set dfu_cccd handle=0x%4x, properties=0x%x",
                            temp_handle, result_data.p_char_uuid16_disc_data->properties);
            dfu_table[conn_id].hdl_cache[HDL_DFU_CP_CCCD] = temp_handle;
            dfu_table[conn_id].properties = result_data.p_char_uuid16_disc_data->properties;
        }
        break;
    default:
        DFU_PRINT_INFO0("dfu_client_disc_result_cb: Invalid Discovery Result Type!");
        break;
    }
}

/**
  * @brief  Called by profile client layer, when read request responsed.
  * @param  reqResult: read request from peer device success or not.
  * @param  wHandle: handle of the value in read response.
  * @param  iValueSize: size of the value in read response.
  * @param  pValue: pointer to the value in read response.
  * @retval ProfileResult_Success--procedure OK.
  *         other--procedure exception.
  */
static void dfu_client_read_result_cb(uint8_t conn_id,  uint16_t cause, uint16_t handle,
                                      uint16_t len, uint8_t *pvalue)
{
    T_DFU_CLIENT_CB_DATA cb_data;
    cb_data.cb_type = DFU_CLIENT_CB_TYPE_READ_RESULT;
    cb_data.cb_content.read_result.cause = cause;
    DFU_PRINT_INFO3("==>dfu_client_read_result_cb: cause=%d, handle=0x%x, size=%d", cause,
                    handle, len);
    /* If read req success, branch to fetch value and send to application. */
    if (handle == dfu_table[conn_id].hdl_cache[HDL_DFU_CP_CCCD])
    {
        cb_data.cb_content.read_result.type = DFU_READ_CP_CCCD;
        if (cause == GAP_SUCCESS && len == sizeof(uint16_t))
        {
            uint16_t cccd_value;
            LE_ARRAY_TO_UINT16(cccd_value, pvalue);
            cb_data.cb_content.read_result.data.dfu_cp_cccd = cccd_value &
                                                              GATT_CLIENT_CHAR_CONFIG_NOTIFY;
        }
    }

    /* Inform application the read result. */
    if (pf_dfu_client_cb)
    {
        (*pf_dfu_client_cb)(dfu_client, conn_id, &cb_data);
    }
}

/**
  * @brief  Called by profile client layer, when write request complete.
  * @param  reqResult: write request send success or not.
  * @retval ProfileResult_Success--procedure OK.
  *         other--procedure exception.
  */
static void dfu_client_write_result_cb(uint8_t conn_id, T_GATT_WRITE_TYPE type, uint16_t handle,
                                       uint16_t cause, uint8_t credits)
{
    T_DFU_CLIENT_CB_DATA cb_data;
    cb_data.cb_type = DFU_CLIENT_CB_TYPE_WRITE_RESULT;
    cb_data.cb_content.write_result.cause = cause;

    DFU_PRINT_INFO4("==>dfu_client_write_result_cb: cause=%d, handle=0x%x, type=%d, credits=%d",
                    cause, handle, type, credits);
    /* If write req success, branch to fetch value and send to application. */
    if (handle == dfu_table[conn_id].hdl_cache[HDL_DFU_CP_CCCD])

    {
        cb_data.cb_content.write_result.type = DFU_WRITE_CP_CCCD;
    }
    else if (handle == dfu_table[conn_id].hdl_cache[HDL_DFU_CP])
    {
        cb_data.cb_content.write_result.type = DFU_WRITE_CP;
    }
    else if (handle == dfu_table[conn_id].hdl_cache[HDL_DFU_DATA])
    {
        cb_data.cb_content.write_result.type = DFU_WRITE_DATA;
    }
    else
    {
        DFU_PRINT_ERROR1("==>dfu_client_write_result_cb: invalid handle=0x%x!", handle);
        return;
    }

    /* Inform application the write result. */
    if (pf_dfu_client_cb)
    {
        (*pf_dfu_client_cb)(dfu_client, conn_id, &cb_data);
    }
}

/**
  * @brief  Called by profile client layer, when notification or indication arrived.
  * @param  wHandle: handle of the value in received data.
  * @param  iValueSize: size of the value in received data.
  * @param  pValue: pointer to the value in received data.
  * @retval ProfileResult_Success--procedure OK.
  *         other--procedure exception.
  */
static T_APP_RESULT dfu_client_notify_indicate_cb(uint8_t conn_id, bool notify,
                                                  uint16_t handle, uint16_t len, uint8_t *pvalue)
{
    T_APP_RESULT app_result = APP_RESULT_REJECT;
    T_DFU_CLIENT_CB_DATA cb_data;

    DFU_PRINT_INFO3("==>dfu_client_notify_indicate_cb: handle=0x%x, len=%d, is notify=%d",
                    handle, len, notify);
    if (handle == dfu_table[conn_id].hdl_cache[HDL_DFU_CP])
    {
        cb_data.cb_type = DFU_CLIENT_CB_TYPE_NOTIF_IND_RESULT;
        cb_data.cb_content.notif_ind_data.type = DFU_RECEIVE_CP_NOTIFY;
        cb_data.cb_content.notif_ind_data.value.len = len;
        cb_data.cb_content.notif_ind_data.value.pdata = pvalue;

        /* Inform application the notif/ind result. */
        if (pf_dfu_client_cb)
        {
            (*pf_dfu_client_cb)(dfu_client, conn_id, &cb_data);
        }
        app_result = APP_RESULT_SUCCESS;
    }

    return app_result;
}

static void dfu_client_disconnect_cb(uint8_t conn_id)
{
    DFU_PRINT_INFO0("dfu_client_disc_cb");
    if (conn_id >= dfu_link_num)
    {
        DFU_PRINT_ERROR1("dfu_client_disc_cb: failed invalid conn_id %d", conn_id);
        return;
    }
    memset(&dfu_table[conn_id], 0, sizeof(T_DFU_LINK));
    return;
}

/**
  * @brief  procedure that client send control point cmd to server.
  * @param  conn_id: connection id.
  * @param  opcode: control point opcode.
  */
void dfu_client_process_control_point(uint8_t conn_id, T_DFU_CP_OPCODE opcode)
{
    uint8_t data[DFU_LENGTH_MAX];
    data[0] = opcode;

    DFU_PRINT_INFO1("===>dfu_client_process_control_point: opcode=0x%x", opcode);
    switch (opcode)
    {
    case DFU_OPCODE_START_DFU: //0x1
        {
            uint8_t *pheader = (uint8_t *)dfu_client_ctx.dfu_img_start_addr;
            dfu_client_ctx.dfu_img_total_length = ((T_IMG_CTRL_HEADER_FORMAT *)pheader)->payload_len +
                                                  IMG_HEADER_SIZE;
            DFU_PRINT_INFO3("==>dfu_client_process_control_point: START_DFU img_id=0x%x, ic_type=0x%x, dfu_img_total_len=%d",
                            dfu_client_ctx.dfu_ctrl_header.image_id,
                            dfu_client_ctx.dfu_ctrl_header.ic_type,
                            dfu_client_ctx.dfu_img_total_length);

            memcpy(data + 1, pheader, DFU_HEADER_SIZE);
            memset(data + 1 + DFU_HEADER_SIZE, 0, 4);
            //aes encrypt when ota
            if (dfu_client_ctx.ota_mode.mode_flag.aesflg)
            {
                DFU_PRINT_INFO1("===>Ctrl Header before encryped: %b", TRACE_BINARY(16, data + 1));
                dfu_encrypt(data + 1);
                DFU_PRINT_INFO1("===>Ctrl Header after encryped: %b", TRACE_BINARY(16, data + 1));
            }
            dfu_client_cp_write(conn_id, data, DFU_LENGTH_START_DFU);
            //update offset
            dfu_client_ctx.curr_offset = DFU_HEADER_SIZE;
        }
        break;
    case DFU_OPCODE_RECEIVE_FW_IMAGE_INFO: //0x2
        {
            LE_UINT16_TO_ARRAY(data + 1, dfu_client_ctx.dfu_ctrl_header.image_id);
            LE_UINT32_TO_ARRAY(data + 3, dfu_client_ctx.curr_offset);
            dfu_client_cp_write(conn_id, data, DFU_LENGTH_RECEIVE_FW_IMAGE_INFO);
        }
        break;
    case DFU_OPCODE_VALID_FW: //0x3
        {
            DFU_PRINT_INFO1("===>DFU_OPCODE_VALID_FW: image_id=0x%x", dfu_client_ctx.dfu_ctrl_header.image_id);
            LE_UINT16_TO_ARRAY(data + 1, dfu_client_ctx.dfu_ctrl_header.image_id);
            dfu_client_cp_write(conn_id, data, DFU_LENGTH_VALID_FW);
        }
        break;
    case DFU_OPCODE_ACTIVE_IMAGE_RESET: //0x4
        {
            dfu_client_cp_write(conn_id, data, DFU_LENGTH_ACTIVE_IMAGE_RESET);
        }
        break;
    case DFU_OPCODE_SYSTEM_RESET: //0x5
        {
            dfu_client_cp_write(conn_id, data, DFU_LENGTH_SYSTEM_RESET);
        }
        break;
    case DFU_OPCODE_REPORT_TARGET_INFO: //0x6
        {
            DFU_PRINT_INFO1("===>DFU_OPCODE_REPORT_TARGET_INFO: image_id=0x%x",
                            dfu_client_ctx.dfu_ctrl_header.image_id);
            LE_UINT16_TO_ARRAY(data + 1, dfu_client_ctx.dfu_ctrl_header.image_id);
            dfu_client_cp_write(conn_id, data, DFU_LENGTH_REPORT_TARGET_INFO);
        }
        break;
    case DFU_OPCODE_CONN_PARA_TO_UPDATE_REQ: //0x7
        {
        }
        break;
    case DFU_OPCODE_BUFFER_CHECK_EN: //0x9
        {
            dfu_client_cp_write(conn_id, data, DFU_LENGTH_BUFFER_CHECK_EN);
        }
        break;
    case DFU_OPCODE_REPORT_BUFFER_CRC: //0xa
        {
            DFU_PRINT_INFO2("===>DFU_OPCODE_REPORT_BUFFER_CRC: dfu_buf_check_size=0x%x, dfu_buf_check_crc_val",
                            dfu_client_ctx.dfu_buf_check_size,
                            dfu_client_ctx.dfu_buf_check_crc_val);

            LE_UINT16_TO_ARRAY(data + 1, dfu_client_ctx.dfu_buf_check_size);
            LE_UINT16_TO_ARRAY(data + 3, dfu_client_ctx.dfu_buf_check_crc_val);
            dfu_client_cp_write(conn_id, data, DFU_LENGTH_REPORT_BUFFER_CRC);
        }
        break;
    case DFU_OPCODE_COPY_IMG:
        {
        }
        break;
    default:
        break;
    }
}

/**
  * @brief  procedure that client send image data to server.
  * @param  conn_id: connection id.
  */
bool dfu_client_push_image(uint8_t conn_id)
{
    uint16_t mtu_size = 0;
    uint16_t push_img_size = 0;
    uint32_t image_size = 0;
    uint8_t *p_dfu_header;
    uint8_t *pdata;

    p_dfu_header = (uint8_t *)dfu_client_ctx.dfu_img_start_addr;
    //DFU_PRINT_INFO1("==>dfu_client_push_image: p_dfu_header=0x%x", p_dfu_header);

    if (dfu_fsm_write == DFU_FSM_WRITE_WAIT_WRITE_RESP)
    {
        // in case that the credit is bigger then the buffer check packet num
        DFU_PRINT_INFO0("==>dfu_client_push_image fail! Waiting for control point response!");
        return false;
    }

    mtu_size = (dfu_client_ctx.mtu_size - 3) & 0xfff0; //mtu size down align 16 bytes
    image_size = dfu_client_ctx.dfu_img_total_length - dfu_client_ctx.curr_offset;
    push_img_size = (image_size > mtu_size ? mtu_size : image_size); //get min value
    DFU_PRINT_INFO3("==>dfu_client_push_image: MIN(mtu_size=%d, img_size=%d)= push_img_size %d",
                    mtu_size, image_size, push_img_size);

    if (dfu_client_ctx.dfu_pkt_counter == (dfu_client_ctx.dfu_pkt_num - 1) &&
        !dfu_client_ctx.ota_mode.mode_flag.buf_check_en)
    {
        push_img_size = dfu_client_ctx.dfu_last_pkt_size;
    }
    DFU_PRINT_INFO3("==>dfu_client_push_image: push_img_size=%d, curr_offset=%d, dfu_img_total_length=%d",
                    push_img_size, dfu_client_ctx.curr_offset, dfu_client_ctx.dfu_img_total_length);

    pdata = os_mem_alloc(RAM_TYPE_DATA_ON, push_img_size);  //send one packet(mtu size) or left img size
    if (pdata == NULL)
    {
        return false;
    }

    memcpy(pdata, p_dfu_header + dfu_client_ctx.curr_offset, push_img_size);
    if (dfu_client_ctx.ota_mode.mode_flag.aesflg && dfu_client_ctx.ota_mode.mode_flag.aesmode)
    {
        uint8_t *ptmp = pdata;
        for (uint16_t loop = 0; loop < push_img_size / 16; loop++)  //only encrypt 16 bytes align
        {
            dfu_encrypt(ptmp);
            ptmp += 16;
        }
    }

    if (dfu_client_data_write(conn_id, pdata, push_img_size))
    {
        dfu_client_ctx.curr_offset += push_img_size;

        /*Target Enable Buffer Check Case*/
        if (dfu_client_ctx.ota_mode.mode_flag.buf_check_en) //enable buffer check
        {
            if (!dfu_client_ctx.dfu_pkt_counter)
            {
                dfu_client_ctx.dfu_buf_check_crc_val = 0;
            }
            dfu_client_ctx.dfu_pkt_counter++;
            dfu_client_ctx.dfu_buf_check_crc_val = dfu_client_temp_crc_cal(pdata, push_img_size,
                                                                           dfu_client_ctx.dfu_buf_check_crc_val);
            /*can do buffer check or not*/
            if (dfu_client_ctx.dfu_pkt_counter == dfu_client_ctx.dfu_pkt_num ||
                dfu_client_ctx.curr_offset == dfu_client_ctx.dfu_img_total_length)
            {
                dfu_client_ctx.dfu_buf_check_crc_val = swap_16(dfu_client_ctx.dfu_buf_check_crc_val);
                dfu_client_ctx.dfu_buf_check_size = push_img_size +
                                                    (dfu_client_ctx.dfu_pkt_counter - 1) * mtu_size;
                /*send opcode 0xa*/
                dfu_client_process_control_point(conn_id, DFU_OPCODE_REPORT_BUFFER_CRC);
                dfu_client_ctx.dfu_pkt_counter = 0;
            }
        }
        else //disable buffer check
        {
            DFU_PRINT_INFO0("==>dfu_client_push_image: buffer check disable");
            dfu_client_ctx.dfu_pkt_counter++;
            if (dfu_client_ctx.dfu_pkt_counter == dfu_client_ctx.dfu_pkt_num)
            {
                DFU_PRINT_INFO0("==>dfu_client_push_image: buffer check disable, target need write flash");
                //wait for slave write image to flash
                os_delay(200);
                dfu_client_ctx.dfu_pkt_counter = 0;
            }
        }
        os_mem_free(pdata);
        return true;
    }
    else
    {
        DFU_PRINT_INFO0("==>dfu_client_push_image: fail write data");

        os_mem_free(pdata);
        return false;
    }
}


void dfu_client_handle_cp_notify_msg(uint8_t conn_id, uint8_t *p_notif_data, uint16_t len)
{
    /*Notification Format: notify_opcode(1 byte)+cp opcode(1 byte)+error code(1 byte)+ payload(different length)*/
    uint8_t notify_opcode = p_notif_data[0];
    uint8_t opcode = p_notif_data[1];
    T_DFU_ARV_ERROR_CODE error_code = (T_DFU_ARV_ERROR_CODE)p_notif_data[2];
    if (notify_opcode != DFU_OPCODE_NOTIFICATION)
    {
        DFU_PRINT_INFO1("==>dfu_client_handle_cp_notify_msg: invalid notify_opcode=0x%x!", notify_opcode);
        return;
    }

    DFU_PRINT_INFO2("==>dfu_client_handle_cp_notify_msg: opcode=0x%x, len=%d", opcode, len);
    switch (opcode)
    {
    case DFU_OPCODE_BUFFER_CHECK_EN: //0x9
        {
            if (len == DFU_NOTIFY_LENGTH_BUFFER_CHECK_EN)
            {
                LE_ARRAY_TO_UINT16(dfu_client_ctx.max_buffer_size, p_notif_data + 3);
                LE_ARRAY_TO_UINT16(dfu_client_ctx.mtu_size, p_notif_data + 5);
                DFU_PRINT_INFO2("==>dfu_client_handle_cp_notify_msg: max_buffer_size=0x%x, mtu_size=0x%x",
                                dfu_client_ctx.max_buffer_size, dfu_client_ctx.mtu_size);

                if (error_code == DFU_ARV_SUCCESS)
                {
                    //target enable buffer check

                    dfu_client_ctx.dfu_pkt_num = dfu_client_ctx.max_buffer_size /
                                                 ((dfu_client_ctx.mtu_size - 3) & 0xfff0);
                    dfu_client_ctx.dfu_pkt_counter = 0;
                    DFU_PRINT_INFO1("==>dfu_client_handle_cp_notify_msg: Target buffer check enable, dfu_pkt_num=%d",
                                    dfu_client_ctx.dfu_pkt_num);
                }
                else
                {
                    uint32_t max_buffer_size = dfu_client_ctx.max_buffer_size - (dfu_client_ctx.max_buffer_size % 20);
                    uint16_t mtu_size = (dfu_client_ctx.mtu_size - 3) & 0xfff0;
                    dfu_client_ctx.dfu_pkt_num = (max_buffer_size + mtu_size - 1) / mtu_size;
                    dfu_client_ctx.dfu_last_pkt_size = (max_buffer_size % mtu_size) ? (max_buffer_size % mtu_size) :
                                                       mtu_size;
                    dfu_client_ctx.dfu_pkt_counter = 0;
                    DFU_PRINT_INFO2("==>dfu_client_handle_cp_notify_msg: Target buffer check disable, max_buffer_size=%d, dfu_pkt_num=%d",
                                    max_buffer_size, dfu_client_ctx.dfu_pkt_num);

                }

                dfu_client_process_control_point(conn_id, DFU_OPCODE_REPORT_TARGET_INFO);
            }
        }
        break;
    case DFU_OPCODE_REPORT_TARGET_INFO: //0x6
        {
            if (len == DFU_NOTIFY_LENGTH_REPORT_TARGET_INFO)
            {
                LE_ARRAY_TO_UINT32(dfu_client_ctx.target_img_version, p_notif_data + 3);
                LE_ARRAY_TO_UINT32(dfu_client_ctx.curr_offset, p_notif_data + 7);
                DFU_PRINT_INFO2("dfu_client_handle_cp_notify_msg: target_version=0x%x, img_id=0x%x",
                                dfu_client_ctx.target_img_version,
                                dfu_client_ctx.dfu_ctrl_header.image_id);

                DFU_PRINT_INFO5("dfu_client_handle_cp_notify_msg: target_version=%d.%d.%d.%d, dfu_client_ctx.curr_offset=0x%x",
                                MAJOR_IMG_VER(dfu_client_ctx.target_img_version),
                                MINOR_IMG_VER(dfu_client_ctx.target_img_version),
                                REVISION_IMG_VER(dfu_client_ctx.target_img_version),
                                RESERVE_IMG_VER(dfu_client_ctx.target_img_version),
                                dfu_client_ctx.curr_offset);
                if (DFU_FEATURE_AUTO_COPY_DFU == dfu_client_ctx.dfu_feature)
                {
                    uint32_t le_target_ver = IMAGE_VERSION_TO_LE_UINT32(dfu_client_ctx.target_img_version);
                    uint32_t le_dfu_ver = 0;
                    if (dfu_client_ctx.dfu_ctrl_header.image_id == AppPatch)
                    {
                        //init dfu_client_ctx.dfu_img_version in format T_IMAGE_VERSION
                        get_active_bank_image_version(AppPatch, (T_IMAGE_VERSION *)&dfu_client_ctx.dfu_img_version);

                        le_dfu_ver = IMAGE_VERSION_TO_LE_UINT32(dfu_client_ctx.dfu_img_version);
                        DFU_PRINT_INFO2("dfu_client_handle_cp_notify_msg: le_dfu_ver=0x%x, le_target_ver=0x%x",
                                        le_dfu_ver, le_target_ver);
                        //compare target image version with dfu img version to determine continue dfu procedure or not
                        if (le_dfu_ver <= le_target_ver)
                        {
                            DBG_DIRECT("auto Copy OTA: app no need ota!");

                            dfu_client_ctx.dfu_ctrl_header.image_id = RomPatch;
                            dfu_client_ctx.dfu_img_start_addr = get_header_addr_by_img_id(RomPatch);
                            /*get target patch image version*/
                            dfu_client_process_control_point(conn_id, DFU_OPCODE_REPORT_TARGET_INFO);
                            return;
                        }
                    }
                    else if (dfu_client_ctx.dfu_ctrl_header.image_id == RomPatch)
                    {
                        get_active_bank_image_version(RomPatch, (T_IMAGE_VERSION *)&dfu_client_ctx.dfu_img_version);

                        le_dfu_ver = IMAGE_VERSION_TO_LE_UINT32(dfu_client_ctx.dfu_img_version);

                        DFU_PRINT_INFO2("dfu_client_handle_cp_notify_msg: le_dfu_ver=0x%x, le_target_ver=0x%x",
                                        le_dfu_ver, le_target_ver);
                        //if dfu patch version less then and equal target img version, don't OTA
                        if (le_dfu_ver <= le_target_ver)
                        {
                            DBG_DIRECT("auto Copy OTA: patch no need OTA!");
                            return;
                        }
                    }
                    else
                    {
                        return;
                    }

                    dfu_client_process_control_point(conn_id, DFU_OPCODE_START_DFU);
                }
                else // not auto copy dfu
                {
                    dfu_client_ctx.dfu_img_version = ((T_IMG_HEADER_FORMAT *)
                                                      dfu_client_ctx.dfu_img_start_addr)->git_ver.ver_info.version;
                    /*if only support upgrade, need compare version value in litte endian */
                    dfu_client_process_control_point(conn_id, DFU_OPCODE_START_DFU);
                }


                /*notify application*/
                if (pf_dfu_client_app_info_cb)
                {
                    T_DFU_CLIENT_CB_MSG cb_msg;
                    cb_msg.conn_id = conn_id;
                    cb_msg.type = DFU_CB_START;
                    pf_dfu_client_app_info_cb(dfu_client, conn_id, &cb_msg);
                }
            }
        }
        break;
    case DFU_OPCODE_START_DFU: //0x1
        {
            if (len == DFU_NOTIFY_LENGTH_START_DFU && error_code == DFU_ARV_SUCCESS)
            {
                //start push image, make use of credits
                for (uint8_t loop = 0; loop < 8; loop++)
                {
                    dfu_client_push_image(conn_id);
                }
            }
            else
            {
                if (pf_dfu_client_app_info_cb)
                {
                    T_DFU_CLIENT_CB_MSG cb_msg;
                    cb_msg.conn_id = conn_id;
                    cb_msg.type = DFU_CB_FAIL;
                    cb_msg.data.dfu_fail_reason = DFU_FAIL_START_DFU;
                    pf_dfu_client_app_info_cb(dfu_client, conn_id, &cb_msg);
                }
            }
        }
        break;
    case DFU_OPCODE_REPORT_BUFFER_CRC: //0xa
        {
            if (len == DFU_NOTIFY_LENGTH_REPORT_BUFFER_CRC)
            {
                uint32_t target_offset;
                LE_ARRAY_TO_UINT32(target_offset, p_notif_data + 3);
                DFU_PRINT_INFO2("==>dfu_client_handle_cp_notify_msg: target_offset=%d, dfu_client_ctx.curr_offset=%d",
                                target_offset, dfu_client_ctx.curr_offset);

                /*if buffer check fail, only retry 3 times*/
                if (error_code != DFU_ARV_SUCCESS)
                {
                    DFU_PRINT_INFO0("==>dfu_client_handle_cp_notify_msg: target buffer check fail!");
                    buf_check_retry_cnt++;
                    if (buf_check_retry_cnt >= 3)
                    {
                        DFU_PRINT_INFO0("==>dfu_client_handle_cp_notify_msg: one packet retry buffer check 3 times fail!");
                        dfu_client_process_control_point(conn_id, DFU_OPCODE_SYSTEM_RESET);

                        if (pf_dfu_client_app_info_cb)
                        {
                            T_DFU_CLIENT_CB_MSG cb_msg;
                            cb_msg.conn_id = conn_id;
                            cb_msg.type = DFU_CB_FAIL;
                            cb_msg.data.dfu_fail_reason = DFU_FAIL_BUF_CHECK;
                            pf_dfu_client_app_info_cb(dfu_client, conn_id, &cb_msg);
                        }

                        return;
                    }
                }
                else
                {
                    DFU_PRINT_INFO0("==>dfu_client_handle_cp_notify_msg: target buffer check success!");
                    buf_check_retry_cnt = 0;
                }

                dfu_client_ctx.curr_offset = target_offset;
                if (dfu_client_ctx.dfu_img_total_length > dfu_client_ctx.curr_offset)
                {
                    DFU_PRINT_INFO0("==>dfu_client_handle_cp_notify_msg: Image transmitting (Buffer Check Enable)");
                    dfu_client_push_image(conn_id);
                }
                else
                {
                    //buffer check enable image trans done
                    DFU_PRINT_INFO0("==>dfu_client_handle_cp_notify_msg: Image trans done (Buffer Check Enable)");
                    dfu_client_process_control_point(conn_id, DFU_OPCODE_VALID_FW);
                }
            }
        }
        break;
    case DFU_OPCODE_VALID_FW: //0x3
        {
            if (len == DFU_NOTIFY_LENGTH_VALID_FW)
            {
                T_DFU_CP_OPCODE next_opcode;
                next_opcode = (error_code == DFU_ARV_SUCCESS) ? DFU_OPCODE_ACTIVE_IMAGE_RESET :
                              DFU_OPCODE_SYSTEM_RESET;
                dfu_client_process_control_point(conn_id, next_opcode);

                if (pf_dfu_client_app_info_cb)
                {
                    T_DFU_CLIENT_CB_MSG cb_msg;
                    cb_msg.conn_id = conn_id;
                    cb_msg.type = (error_code == DFU_ARV_SUCCESS) ? DFU_CB_END : DFU_CB_FAIL;
                    cb_msg.data.dfu_fail_reason = (error_code == DFU_ARV_SUCCESS) ? DFU_SUCCESS : DFU_FAIL_VALIDATE_FW;
                    pf_dfu_client_app_info_cb(dfu_client, conn_id, &cb_msg);
                }
            }
        }
        break;
    default: //other opcode no ack
        break;
    }
}


T_APP_RESULT dfu_client_handle_read_cb(uint8_t conn_id, T_DFU_READ_RESULT *pcb_read_data)
{
    T_APP_RESULT result = APP_RESULT_SUCCESS;
    switch (pcb_read_data->type)
    {
    case DFU_READ_CP_CCCD:
        DFU_PRINT_INFO1("==>dfu_client_handle_read_cb: dfu_cp_cccd=%d(1:CCCD Enable)",
                        pcb_read_data->data.dfu_cp_cccd);
        break;
    default:
        break;
    }
    return result;
}

T_APP_RESULT dfu_client_handle_write_cb(uint8_t conn_id, T_DFU_WRITE_RESULT *pcb_write_data)
{
    T_APP_RESULT result = APP_RESULT_SUCCESS;
    DFU_PRINT_INFO1("==>dfu_client_handle_write_cb: type=%d(0: write cp, 1: write cp ccd, 2: write data)",
                    pcb_write_data->type);
    DFU_PRINT_INFO1("==>dfu_client_handle_write_cb: dfu_fsm_write=%d(0:idle, 1:cccd_enabled, 2: writed_data, 3: wait for rsp )",
                    dfu_client_ctx.dfu_fsm_write);
    if (pcb_write_data->cause == 0)
    {
        switch (pcb_write_data->type)
        {
        case DFU_WRITE_CP_CCCD:
            DFU_PRINT_INFO1("==>dfu_client_handle_write_cb: DFU_WRITE_CP_CCCD",
                            dfu_fsm_write);
            if (dfu_fsm_write == DFU_FSM_WRITE_CCCD_ENABLE)
            {
                T_IMG_HEADER_FORMAT *p_header = (T_IMG_HEADER_FORMAT *)dfu_client_ctx.dfu_img_start_addr;
                dfu_client_ctx.dfu_ctrl_header.image_id = p_header->ctrl_header.image_id;
                dfu_client_process_control_point(conn_id, DFU_OPCODE_BUFFER_CHECK_EN);
            }
            break;
        case DFU_WRITE_DATA:
            DFU_PRINT_INFO1("==>dfu_client_handle_write_cb: DFU_WRITE_DATA, dfu_fsm_write=%d.",
                            dfu_fsm_write);
            if (dfu_fsm_write == DFU_FSM_WRITE_DFU_DATA)
            {
                if (dfu_client_ctx.dfu_img_total_length > dfu_client_ctx.curr_offset)
                {
                    DFU_PRINT_INFO0("==>dfu_client_handle_write_cb: DFU_WRITE_DATA, continue push image!");
                    dfu_client_push_image(conn_id);
                }
                else
                {
                    if (!dfu_client_ctx.ota_mode.mode_flag.buf_check_en)
                    {
                        /*when not enable buf check, img trans done, verify img*/
                        DFU_PRINT_INFO0("==>dfu_client_handle_write_cb: Img trans done(BufCheckDisable), validate fw");
                        dfu_client_process_control_point(conn_id, DFU_OPCODE_VALID_FW);
                    }
                }
            }
            break;
        case DFU_WRITE_CP:
            DFU_PRINT_INFO1("==>dfu_client_handle_write_cb: DFU_WRITE_CP, dfu_fsm_write=%d.",
                            dfu_fsm_write);
            if (dfu_fsm_write == DFU_FSM_WRITE_WAIT_WRITE_RESP)
            {
                //means received write response
                dfu_fsm_write = DFU_FSM_WRITE_IDLE;
                if (dfu_temp_cp_notify_msg_len)
                {
                    //means first received notification then received write response
                    DFU_PRINT_INFO2("==>dfu_client_handle_write_cb: DFU_WRITE_CP, dfu_fsm_write=%d, dfu_cp_temp_msg_len=%d",
                                    dfu_fsm_write, dfu_temp_cp_notify_msg_len);
                    dfu_client_handle_cp_notify_msg(conn_id, dfu_temp_cp_notify_msg, dfu_temp_cp_notify_msg_len);
                    dfu_temp_cp_notify_msg_len = 0;
                }
            }
            break;
        default:
            break;
        }
    }
    else
    {
        DFU_PRINT_INFO3("==>dfu_client_handle_write_cb: write data fail! type=%d, cause=0x%x, dfu_fsm_write=%d",
                        pcb_write_data->type, pcb_write_data->cause, dfu_fsm_write);
        DFU_PRINT_INFO0("==>dfu_client_handle_write_cb: le_disconnect");
        le_disconnect(conn_id);
    }

    return result;
}

T_APP_RESULT dfu_client_handle_notif_ind_cb(uint8_t conn_id,
                                            T_DFU_NOTIFY_INDICATE_DATA *pcb_notif_ind_data)
{
    T_APP_RESULT result = APP_RESULT_SUCCESS;
    DFU_PRINT_INFO1("==>dfu_client_handle_notif_ind_cb: type=%d(0: DFU_RECEIVE_CP_NOTIFY)",
                    pcb_notif_ind_data->type);
    switch (pcb_notif_ind_data->type)
    {
    case DFU_RECEIVE_CP_NOTIFY:
        {
            DFU_PRINT_INFO1("==>dfu_client_handle_notif_ind_cb: dfu_fsm_write=%d(if 0 hdl cp msg, else cache msg)",
                            dfu_fsm_write);
            if (dfu_fsm_write == DFU_FSM_WRITE_IDLE)
            {
                dfu_client_handle_cp_notify_msg(conn_id, pcb_notif_ind_data->value.pdata,
                                                pcb_notif_ind_data->value.len);
            }
            else
            {
                //cache
                dfu_temp_cp_notify_msg_len = pcb_notif_ind_data->value.len;
                memcpy(dfu_temp_cp_notify_msg, pcb_notif_ind_data->value.pdata,
                       dfu_temp_cp_notify_msg_len > 20 ? 20 : dfu_temp_cp_notify_msg_len);
                DFU_PRINT_INFO1("==>dfu_client_handle_notif_ind_cb: cached received notification, len=%d",
                                dfu_temp_cp_notify_msg_len);
            }
        }
        break;
    default:
        break;
    }
    return result;
}

/*profile callback to handle msg*/
T_APP_RESULT dfu_client_handle_cb(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data)
{
    T_APP_RESULT result = APP_RESULT_SUCCESS;
    T_DFU_CLIENT_CB_DATA *pcb_data = (T_DFU_CLIENT_CB_DATA *)p_data;
    DFU_PRINT_INFO1("==>dfu_client_handle_cb: cb_type=%d(0: disc, 1: read, 2: write, 3: notify)",
                    pcb_data->cb_type);
    switch (pcb_data->cb_type)
    {
    case DFU_CLIENT_CB_TYPE_DISC_STATE:
        break;
    case DFU_CLIENT_CB_TYPE_READ_RESULT:
        result = dfu_client_handle_read_cb(conn_id, &pcb_data->cb_content.read_result);
        break;
    case DFU_CLIENT_CB_TYPE_WRITE_RESULT:
        result = dfu_client_handle_write_cb(conn_id, &pcb_data->cb_content.write_result);
        break;
    case DFU_CLIENT_CB_TYPE_NOTIF_IND_RESULT:
        result = dfu_client_handle_notif_ind_cb(conn_id, &pcb_data->cb_content.notif_ind_data);
        break;
    default:
        break;
    }

    return result;
}

/**
 * @brief Dfu Client Callbacks.
*/
const T_FUN_CLIENT_CBS dfu_client_cbs =
{
    dfu_client_disc_state_cb,   //!< Discovery State callback function pointer
    dfu_client_disc_result_cb,  //!< Discovery result callback function pointer
    dfu_client_read_result_cb,      //!< Read response callback function pointer
    dfu_client_write_result_cb,     //!< Write result callback function pointer
    dfu_client_notify_indicate_cb,  //!< Notify Indicate callback function pointer
    dfu_client_disconnect_cb       //!< Link disconnection callback function pointer
};

/**
  * @brief  add Dfu client to application.
  * @param  appCB: pointer of app callback function to handle specific client module data.
  * @retval Client ID of the specific client module.
  */
T_CLIENT_ID dfu_add_client(P_FUN_GENERAL_APP_CB app_cb, uint8_t link_num)
{
    uint16_t size;
    if (link_num > DFU_MAX_LINKS)
    {
        DFU_PRINT_ERROR1("dfu_add_client: invalid link_num %d", link_num);
        return 0xff;
    }
    if (false == client_register_spec_client_cb(&dfu_client, &dfu_client_cbs))
    {
        dfu_client = CLIENT_PROFILE_GENERAL_ID;
        DFU_PRINT_ERROR0("dfu_client_add: fail!");
        return dfu_client;
    }
    DFU_PRINT_INFO1("dfu_client_add: client ID = %d", dfu_client);

    dfu_link_num = link_num;
    size = dfu_link_num * sizeof(T_DFU_LINK);
    dfu_table = os_mem_zalloc(RAM_TYPE_DATA_ON, size);

    /* register callback for profile to inform application that some events happened. */
    pf_dfu_client_cb = dfu_client_handle_cb;
    pf_dfu_client_app_info_cb = app_cb;

    if (pf_dfu_client_init)
    {
        pf_dfu_client_init();
    }

    return dfu_client;
}


