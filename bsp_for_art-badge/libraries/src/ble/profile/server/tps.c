/**
*********************************************************************************************************
*               Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     tps.c
* @brief    Tx Power service source file.
* @details  Interfaces to access Tx Power service.
* @author
* @date
* @version  v1.0
*********************************************************************************************************
*/
#include "string.h"
#include "trace.h"
#include "profile_server.h"
#include "tps.h"


#define GATT_UUID_TX_POWER_SERVICE              0x1804
#define GATT_UUID_CHAR_TX_LEVEL                 0x2A07

#define TPS_TX_POWER_VALUE_INDEX          2



static uint8_t tx_power_value = 0;

static P_FUN_SERVER_GENERAL_CB pfn_tps_cb = NULL;

/**< @brief  profile/service definition.  */
const T_ATTRIB_APPL tps_attr_tbl[] =
{
    /*----------------- TX Power Service -------------------*/
    {
        (ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_LE),  /* wFlags     */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_PRIMARY_SERVICE),
            HI_WORD(GATT_UUID_PRIMARY_SERVICE),
            LO_WORD(GATT_UUID_TX_POWER_SERVICE),    /* service UUID */
            HI_WORD(GATT_UUID_TX_POWER_SERVICE)
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
            GATT_CHAR_PROP_READ,           /* characteristic properties */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },

    /* Alert Level Characteristic value  */
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_TX_LEVEL),
            HI_WORD(GATT_UUID_CHAR_TX_LEVEL)
        },
        0,                                          /* variable size */
        NULL,
        GATT_PERM_READ                             /* wPermissions */
    }
};

/**< @brief  TPS profile related services size definition.  */
const static int tps_attr_tbl_size = sizeof(tps_attr_tbl);

/**
 * @brief       Set a Tx power service parameter.
 *
 *              NOTE: You can call this function with a tx power service parameter type and it will set the
 *                      tx power service parameter.  Tx power service parameters are defined in @ref T_TPS_PARAM_TYPE.
 *                      If the "len" field sets to the size of a "uint16_t" ,the
 *                      "p_value" field must point to a data with type of "uint16_t".
 *
 * @param[in]   param_type   Tx power service parameter type: @ref T_TPS_PARAM_TYPE
 * @param[in]   len       Length of data to write
 * @param[in]   p_value Pointer to data to write.  This is dependent on
 *                      the parameter type and WILL be cast to the appropriate
 *                      data type (example: data type of uint16 will be cast to
 *                      uint16 pointer).
 *
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint8_t tx_power = 0;
        tps_set_parameter(TPS_PARAM_TX_POWER, 1, &tx_power);
    }
 * \endcode
 */
bool tps_set_parameter(T_TPS_PARAM_TYPE param_type, uint8_t len, void *p_value)
{
    bool ret = true;

    switch (param_type)
    {
    default:
        /* invalid param to set. */
        ret = false;
        break;

    case TPS_PARAM_TX_POWER:
        tx_power_value = *(uint8_t *)p_value;
        break;
    }

    if (!ret)
    {
        PROFILE_PRINT_ERROR0("tps_set_parameter: TPS parameter set failed");
    }

    return ret;
}


/**
 * @brief read characteristic data from service.
 *
 * @param[in] conn_id   Connection id.
 * @param[in] service_id          ServiceID to be read.
 * @param[in] attrib_index          Attribute index of getting characteristic data.
 * @param[in] offset                offset of characteritic to be read.
 * @param[in,out] length_ptr            length of getting characteristic data.
 * @param[in,out] pp_value            pointer to pointer of characteristic value to be read.
 * @return TProfileResult
*/
T_APP_RESULT tps_attr_read_cb(uint8_t conn_id, uint8_t service_id, uint16_t attrib_index,
                              uint16_t offset, uint16_t *length_ptr, uint8_t **pp_value)
{
    T_APP_RESULT  cause  = APP_RESULT_SUCCESS;
    *length_ptr = 0;
    T_TPS_CALLBACK_DATA callback_data;
    switch (attrib_index)
    {
    default:
        PROFILE_PRINT_ERROR1("tps_attr_read_cb: attrib_index %d", attrib_index);
        cause = APP_RESULT_ATTR_NOT_FOUND;
        break;

    case TPS_TX_POWER_VALUE_INDEX:
        callback_data.conn_id = conn_id;
        callback_data.msg_type = SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE;
        callback_data.msg_data.read_value_index = TPS_READ_TX_POWER_VALUE;
        pfn_tps_cb(service_id, (void *)&callback_data);

        *pp_value   = (uint8_t *)&tx_power_value;
        *length_ptr = sizeof(tx_power_value);
        break;
    }

    PROFILE_PRINT_INFO2("tps_attr_read_cb: attrib_index %d, *length_ptr %d", attrib_index, *length_ptr);

    return (cause);
}


// TPS related Service Callbacks
const T_FUN_GATT_SERVICE_CBS tps_cbs =
{
    tps_attr_read_cb,  // Read callback function pointer
    NULL,           // Write callback function pointer
    NULL            // CCCD update callback function pointer
};

/**
  * @brief Add tx power service to the BLE stack database.
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
         tps_id = tps_add_service(app_handle_profile_message);
     }
  * \endcode
  */
T_SERVER_ID tps_add_service(void *p_func)
{
    T_SERVER_ID service_id;
    if (false == server_add_service(&service_id,
                                    (uint8_t *)tps_attr_tbl,
                                    tps_attr_tbl_size,
                                    tps_cbs))
    {
        PROFILE_PRINT_ERROR1("tps_add_service: service_id %d", service_id);
        service_id = 0xff;
    }

    pfn_tps_cb = (P_FUN_SERVER_GENERAL_CB)p_func;
    return service_id;
}

