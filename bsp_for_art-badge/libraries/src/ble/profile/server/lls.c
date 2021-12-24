/**
*********************************************************************************************************
*               Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     lls.c
* @brief    link loss service source file.
* @details  Interfaces to access link loss service.
* @author
* @date
* @version  v1.0
*********************************************************************************************************
*/
#include "stdint.h"
#include "string.h"
#include "trace.h"
#include "profile_server.h"
#include "lls.h"

/********************************************************************************************************
* local static variables defined here, only used in this source file.
********************************************************************************************************/

///@cond
/** @brief LLS profile related services UUIDs. */
#define GATT_UUID_LINK_LOSS_SERVICE             0x1803
#define GATT_UUID_CHAR_ALERT_LEVEL              0x2A06

/** @brief LLS Attribute Index */
#define GATT_SVC_PXP_LINK_LOSS_ALERT_LEVEL_INDEX   2
///@endcond

static P_FUN_SERVER_GENERAL_CB pfn_lls_cb = NULL;
static uint8_t lls_alert_level = 0;

/**< @brief profile/service definition.  */
const T_ATTRIB_APPL lls_attr_tbl[] =
{
    /*----------------- Link Loss Service -------------------*/
    {
        (ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_LE),  /* wFlags     */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_PRIMARY_SERVICE),
            HI_WORD(GATT_UUID_PRIMARY_SERVICE),
            LO_WORD(GATT_UUID_LINK_LOSS_SERVICE),    /* service UUID */
            HI_WORD(GATT_UUID_LINK_LOSS_SERVICE)
        },
        UUID_16BIT_SIZE,                            /* bValueLen     */
        NULL,                                       /* pValueContext */
        GATT_PERM_READ                              /* wPermissions  */
    },

    /* Alert Level Characteristic */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ | GATT_CHAR_PROP_WRITE, /* characteristic properties */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },

    /* Alert Level Characteristic value  */
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_ALERT_LEVEL),
            HI_WORD(GATT_UUID_CHAR_ALERT_LEVEL)
        },
        0,                                          /* variable size */
        NULL,
        GATT_PERM_READ | GATT_PERM_WRITE            /* wPermissions */
    }
};
/**< @brief  LLS service size definition.  */
const int lls_attr_tbl_size = sizeof(lls_attr_tbl);


/**
 * @brief       Set a link loss service parameter.
 *
 *              NOTE: You can call this function with a link loss service parameter type and it will set the
 *                      link loss service parameter. Link loss service parameters are defined in @ref T_LLS_PARAM_TYPE.
 *                      If the "len" field sets to the size of a "uint16_t" ,the
 *                      "p_value" field must point to a data with type of "uint16_t".
 *
 * @param[in]   param_type   Link loss service parameter type: @ref T_LLS_PARAM_TYPE
 * @param[in]   length       Length of data to write
 * @param[in]   p_value Pointer to data to write.  This is dependent on
 *                      the parameter type and WILL be cast to the appropriate
 *                      data type (For example: if data type of param is uint16_t, p_value will be cast to
 *                      pointer of uint16_t).
 *
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 */
bool lls_set_parameter(T_LLS_PARAM_TYPE param_type, uint8_t length, void *p_value)
{
    bool ret = true;

    switch (param_type)
    {
    default:
        ret = false;
        break;
    case LLS_PARAM_LINK_LOSS_ALERT_LEVEL:
        lls_alert_level = *(uint8_t *)p_value;
        break;
    }

    if (!ret)
    {
        PROFILE_PRINT_ERROR0("lls_set_parameter: lls alert level parameter set failed");
    }

    return ret;
}


/**
 * @brief read characteristic data from service.
 *
 * @param conn_id           connection id.
 * @param service_id        ServiceID to be read.
 * @param attrib_index      Attribute index of getting characteristic data.
 * @param offset            offset of characteritic to be read.
 * @param p_length          length of getting characteristic data.
 * @param pp_value          pointer to pointer of characteristic value to be read.
 * @return T_APP_RESULT
*/
T_APP_RESULT lls_attr_read_cb(uint8_t conn_id, uint8_t service_id, uint16_t attrib_index,
                              uint16_t offset, uint16_t *p_length, uint8_t **pp_value)
{
    T_APP_RESULT cause = APP_RESULT_SUCCESS;
    *p_length = 0;
    T_LLS_CALLBACK_DATA callback_data;

    switch (attrib_index)
    {
    default:
        PROFILE_PRINT_ERROR1("lls_attr_read_cb default:attrib_index %d", attrib_index);
        cause = APP_RESULT_ATTR_NOT_FOUND;
        break;

    case GATT_SVC_PXP_LINK_LOSS_ALERT_LEVEL_INDEX:
        callback_data.conn_id = conn_id;
        callback_data.msg_type = SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE;
        callback_data.msg_data.read_value_index = LLS_READ_ALERT_LEVEL;
        pfn_lls_cb(service_id, (void *)&callback_data);
        *pp_value = (uint8_t *)&lls_alert_level;
        *p_length = sizeof(lls_alert_level);
        PROFILE_PRINT_INFO2("lls_attr_read_cb: attrib_index %d, length %d", attrib_index, *p_length);
        break;
    }

    return (cause);
}

/**
 * @brief write characteristic data from service.
 *
 * @param conn_id               connection id.
 * @param service_id            ServiceID to be written.
 * @param attrib_index          Attribute index of characteristic.
 * @param length                length of writing characteristic data.
 * @param p_value               pointer to characteristic data.
 * @param p_write_ind_post_proc function pointer called after lls_attr_write_cb.
 * @return T_APP_RESULT
*/
T_APP_RESULT lls_attr_write_cb(uint8_t conn_id, uint8_t service_id, uint16_t attrib_index,
                               T_WRITE_TYPE write_type,
                               uint16_t length, uint8_t *p_value, P_FUN_WRITE_IND_POST_PROC *p_write_ind_post_proc)
{
    T_APP_RESULT  cause = APP_RESULT_SUCCESS;
    T_LLS_CALLBACK_DATA callback_data;
    if (!p_value)
    {
        PROFILE_PRINT_ERROR2("lls_attr_write_cb: p_value %p, length 0x%x", p_value, length);
        cause = APP_RESULT_INVALID_PDU;
        return cause;
    }

    switch (attrib_index)
    {
    default:
        cause = APP_RESULT_ATTR_NOT_FOUND;
        break;
    case GATT_SVC_PXP_LINK_LOSS_ALERT_LEVEL_INDEX:
        if (length != sizeof(uint8_t))
        {
            cause = APP_RESULT_INVALID_VALUE_SIZE;
        }
        else
        {
            callback_data.conn_id = conn_id;
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE;
            callback_data.msg_data.write_alert_level = p_value[0];
        }
        break;

    }
    if (pfn_lls_cb && (cause == APP_RESULT_SUCCESS))
    {
        pfn_lls_cb(service_id, (void *)&callback_data);
    }

    return cause;

}


// LLS related Service Callbacks
const T_FUN_GATT_SERVICE_CBS lls_cbs =
{
    lls_attr_read_cb,   // Read callback function pointer
    lls_attr_write_cb,  // Write callback function pointer
    NULL                // CCCD update callback function pointer
};

/**
  * @brief Add link loss service to the BLE stack database.
  *
  * @param[in]   p_func  Callback when service attribute was read, write or cccd update.
  * @return Service id generated by the BLE stack: @ref T_SERVER_ID.
  * @retval 0xFF Operation failure.
  * @retval others Service id assigned by stack.
  *
  * <b>Example usage</b>
  * \code{.c}
    void profile_init()
    {
        server_init(1);
        lls_id = lls_add_service(app_handle_profile_message);
    }
  * \endcode
  */
T_SERVER_ID lls_add_service(void *p_func)
{
    T_SERVER_ID service_id;
    if (false == server_add_service(&service_id,
                                    (uint8_t *)lls_attr_tbl,
                                    lls_attr_tbl_size,
                                    lls_cbs))
    {
        PROFILE_PRINT_ERROR1("lls_add_service: service_id %d", service_id);
        service_id = 0xff;
    }

    pfn_lls_cb = (P_FUN_SERVER_GENERAL_CB)p_func;
    return service_id;
}

