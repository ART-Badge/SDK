/**
*********************************************************************************************************
*               Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     sps.c
* @brief    SPS service source file.
* @details  Interfaces to access SPS service.
* @author
* @date
* @version  v1.0
*********************************************************************************************************
*/
#include "stdint.h"
#include "string.h"
#include "trace.h"
#include "profile_server.h"
#include "sps.h"
#include "sps_config.h"


///@cond
/** @brief  ScanParameters service related UUIDs. */
#define GATT_UUID_SPS                           0x1813
#define GATT_UUID_CHAR_SCAN_INTERVAL_WINDOW     0x2A4F
#define GATT_UUID_CHAR_SCAN_REFRESH             0x2A31

/** @brief  defines for Characteristic Index in Scan parameters Service */
#define GATT_SVC_SPS_SCAN_INTERVAL_INDEX        2 /**<  @brief  Index for Scan Interval chars's value      */
#define GATT_SVC_SPS_SCAN_REFRESH_INDEX         4 /**<  @brief  Index for Scan refresh chars's value      */
#define GATT_SVC_SPS_SCAN_REFRESH_CCCD_INDEX    5/**<  @brief  CCCD Index for Scan Interval chars's value      */
///@endcond


/**<  SPS Refresh Value. */
static uint8_t sps_refresh_value __attribute__((unused)) = 0;
/**<  Function pointer used to send event to application from sps profile. */
static P_FUN_SERVER_GENERAL_CB pfn_sps_cb = NULL;

/**< @brief  profile/service definition.  */
const T_ATTRIB_APPL sps_attr_tbl[] =
{
    /*--------------------------ScanParameters Service------------------*/
    /* <<Primary Service>>, .. */
    {
        (ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_LE),   /* wFlags     */
        {                                         /* bTypeValue */
            LO_WORD(GATT_UUID_PRIMARY_SERVICE),
            HI_WORD(GATT_UUID_PRIMARY_SERVICE),
            LO_WORD(GATT_UUID_SPS),              /* service UUID */
            HI_WORD(GATT_UUID_SPS)
        },
        UUID_16BIT_SIZE,                            /* bValueLen     */
        NULL,                                       /* pValueContext */
        GATT_PERM_READ                              /* wPermissions  */
    },

    /* <<Characteristic>>, .. */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                         /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_WRITE_NO_RSP                  /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },

    /* Scan Interval Window Char value */
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                         /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_SCAN_INTERVAL_WINDOW),
            HI_WORD(GATT_UUID_CHAR_SCAN_INTERVAL_WINDOW)
        },
        0,                                          /* bValueLen */
        NULL,
        GATT_PERM_NONE                              /* wPermissions */
    },
    /* <<Characteristic>>, .. */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                         /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            (GATT_CHAR_PROP_NOTIFY                    /* characteristic properties */
            )
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    }
#if SPS_CHAR_SCAN_REFRESH_SUPPORT
    ,
    /* Scan Refresh Char value */
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                         /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_SCAN_REFRESH),
            HI_WORD(GATT_UUID_CHAR_SCAN_REFRESH)
        },
        0,                                          /* bValueLen */
        NULL,
        GATT_PERM_NONE                              /* wPermissions */
    },
    /* client characteristic configuration */
    {
        ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_CCCD_APPL,                 /* wFlags */
        {                                         /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            /* NOTE: this value has an instantiation for each client, a write to */
            /* this attribute does not modify this default value:                */
            LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT), /* client char. config. bit field */
            HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)
        },
        2,                                          /* bValueLen */
        NULL,
        (GATT_PERM_READ | GATT_PERM_WRITE)          /* wPermissions */
    }
#endif
};
/**< @brief  SPS service size definition.  */
const uint16_t sps_attr_tbl_size = sizeof(sps_attr_tbl);

/**
 * @brief       Set a scan paramter service parameter.
 *
 *              NOTE: You can call this function with a scan paramter service parameter type and it will set the
 *                      scan paramter service parameter.  Scan paramter service parameters are defined in @ref T_SPS_PARAM_TYPE.
 *                      If the "len" field sets to the size of a "uint16_t" ,the
 *                      "p_value" field must point to a data with type of "uint16_t".
 *
 * @param[in]   param_type   Scan paramter service parameter type: @ref T_SPS_PARAM_TYPE
 * @param[in]   len       Length of data to write
 * @param[in]   p_value Pointer to data to write.  This is dependent on
 *                      the parameter type and WILL be cast to the appropriate
 *                      data type (For example: if data type of param is uint16_t, p_value will be cast to
 *                      pointer of uint16_t).
 *
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint8_t refresh_value = 10;
        sps_set_parameter(SPS_PARAM_SCAN_REFRESH, 1, &refresh_value);
    }
 * \endcode
 */
bool sps_set_parameter(T_SPS_PARAM_TYPE param_type, uint8_t len, void *p_value)
{
    bool ret = true;

    switch (param_type)
    {
    default:
        /* invalid param to set. */
        ret = false;
        break;
    case SPS_PARAM_SCAN_REFRESH:
        sps_refresh_value = *((uint8_t *)p_value);
        break;
    }

    if (!ret)
    {
        PROFILE_PRINT_ERROR0("sps_set_parameter: SPS parameter set failed");
    }

    return ret;
}


/**
  * @brief send notification.
  *
  * @param[in] conn_id   Connection id.
  * @param[in] service_id   Service ID of service.
  * @param[in] sps_refresh_value   characteristic value to notify
  * @return notification action result
  * @retval 1 TRUE
  * @retval 0 FALSE
  *
  * <b>Example usage</b>
  * \code{.c}
     void test(void)
     {
         uint8_t sps_refresh_value = 10;
         sps_scan_interval_window_value_notify(conn_id, sps_id, sps_refresh_value);
     }
  * \endcode
  */
bool sps_scan_interval_window_value_notify(uint8_t conn_id, uint8_t service_id,
                                           uint8_t sps_refresh_value)
{
    return server_send_data(conn_id, service_id, GATT_SVC_SPS_SCAN_REFRESH_INDEX,
                            (uint8_t *)&sps_refresh_value, sizeof(sps_refresh_value), GATT_PDU_TYPE_ANY);
}


/**
 * @brief write characteristic data from service.
 *
 * @param[in] conn_id           Connection id.
 * @param[in] service_id          ServiceID to be written.
 * @param[in] attrib_index        Attribute index of characteristic.
 * @param[in] write_type          Write type.
 * @param[in] length              Length of value to be written.
 * @param[in] p_value             Value to be written.
 * @return Profile procedure result
*/
T_APP_RESULT sps_attr_write_cb(uint8_t conn_id, uint8_t service_id, uint16_t attrib_index,
                               T_WRITE_TYPE write_type,
                               uint16_t length, uint8_t *p_value, P_FUN_WRITE_IND_POST_PROC *p_write_ind_post_proc)
{

    T_APP_RESULT  cause = APP_RESULT_SUCCESS;
    if ((GATT_SVC_SPS_SCAN_INTERVAL_INDEX == attrib_index) && (length == 4) && p_value)
    {
        uint16_t scan_interval = ((uint16_t)p_value[1] << 8) && (0xff00) + p_value[0];
        uint16_t scan_window = ((uint16_t)p_value[2] << 8) && (0xff00) + p_value[3];

        T_SPS_CALLBACK_DATA callback_data;
        callback_data.msg_type = SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE;
        callback_data.conn_id = conn_id;
        callback_data.msg_data.write.write_type = SPS_WRITE_SCAN_INTERVAL_WINDOW;
        callback_data.msg_data.write.write_parameter.scan.scan_interval = scan_interval;
        callback_data.msg_data.write.write_parameter.scan.scan_window = scan_window;

        if (pfn_sps_cb)
        {
            pfn_sps_cb(service_id, (void *)&callback_data);
        }
    }
    else
    {
        PROFILE_PRINT_ERROR2("sps_attr_write_cb: attrib_index 0x%x, length %d", attrib_index, length);
        cause = APP_RESULT_ATTR_NOT_FOUND;
    }
    return cause;

}

/**
 * @brief update CCCD bits from stack.
 *
 * @param[in] conn_id           Connection id.
 * @param[in] service_id          Service ID.
 * @param[in] index          Attribute index of characteristic data.
 * @param[in] ccc_bits         CCCD bits from stack.
 * @return None
*/
void sps_cccd_update_cb(uint8_t conn_id, uint8_t service_id, uint16_t index, uint16_t ccc_bits)
{
    T_SPS_CALLBACK_DATA callback_data;
    callback_data.msg_type = SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION;
    callback_data.conn_id = conn_id;
    bool handle = false;
    PROFILE_PRINT_INFO2("sps_cccd_update_cb: index %d ccc_bits %x", index, ccc_bits);

    switch (index)
    {
    case GATT_SVC_SPS_SCAN_REFRESH_CCCD_INDEX:
        {
            if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_NOTIFY)
            {
                callback_data.msg_data.notification_indification_index = SPS_NOTIFY_INDICATE_SCAN_REFRESH_ENABLE;
            }
            else
            {
                callback_data.msg_data.notification_indification_index = SPS_NOTIFY_INDICATE_SCAN_REFRESH_DISABLE;
            }
            handle = true;
            break;
        }
    default:
        break;
    }

    if (pfn_sps_cb && (handle == true))
    {
        pfn_sps_cb(service_id, (void *)&callback_data);
    }

    return;
}

/**
 * @brief SPS Service Callbacks.
*/
const T_FUN_GATT_SERVICE_CBS sps_cbs =
{
    NULL,           // Read callback function pointer
    sps_attr_write_cb, // Write callback function pointer
    sps_cccd_update_cb // CCCD update callback function pointer
};

/**
  * @brief Add scan parameters service to the BLE stack database.
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
        sps_id = sps_add_service(app_handle_profile_message);
    }
  * \endcode
  */
T_SERVER_ID sps_add_service(void *p_func)
{
    T_SERVER_ID service_id;
    if (false == server_add_service(&service_id,
                                    (uint8_t *)sps_attr_tbl,
                                    sps_attr_tbl_size,
                                    sps_cbs))
    {
        PROFILE_PRINT_ERROR1("sps_add_service: service_id %d", service_id);
        service_id = 0xff;
        return service_id;
    }
    pfn_sps_cb = (P_FUN_SERVER_GENERAL_CB)p_func;
    return service_id;
}

