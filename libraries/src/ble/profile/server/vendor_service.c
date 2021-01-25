/**
*********************************************************************************************************
*               Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     vendor_service.c
* @brief    Vendor service source file.
* @details  Interfaces to access Vendor service.
* @author
* @date
* @version  v1.0
*********************************************************************************************************
*/

#include "board.h"
#include "string.h"
#include "trace.h"
#include "gatt.h"
#include "profile_server.h"
#include "vendor_service.h"
#include "gap.h"
#include "mem_config.h"
#include "app_section.h"
#include "overlay_mgr.h"

/********************************************************************************************************
* local static variables defined here, only used in this source file.
********************************************************************************************************/
static P_FUN_SERVER_GENERAL_CB pfn_vendor_svc_cb = NULL;
static const uint8_t GATT_UUID_VENDOR_SERVICE[16] = {0x12, 0xA2, 0x4D, 0x2E, 0xFE, 0x14, 0x48, 0x8e, 0x93, 0xD2, 0x17, 0x3C, 0xFF, 0xD1, 0x00, 0x00};

/********************************************************************************************************
* global variables
********************************************************************************************************/
uint8_t vendor_svc_handshake_values[16] = {0};

/**< @brief  profile/service definition.
*   here is an example of OTA service table
*   including Write
*/
const T_ATTRIB_APPL vendor_svc_bas_attr_tbl[] =
{
    /*--------------------------Vendor Service ---------------------------*/
    /* <<Primary Service>>, .. 0*/
    {
        (ATTRIB_FLAG_VOID | ATTRIB_FLAG_LE),  /* wFlags     */
        {
            LO_WORD(GATT_UUID_PRIMARY_SERVICE),
            HI_WORD(GATT_UUID_PRIMARY_SERVICE),     /* bTypeValue */
        },
        UUID_128BIT_SIZE,                     /* bValueLen     */
        (void *)GATT_UUID_VENDOR_SERVICE,        /* pValueContext */
        GATT_PERM_READ                              /* wPermissions  */
    },

    /* <<Characteristic>> 1 */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_WRITE_NO_RSP | GATT_CHAR_PROP_NOTIFY,                  /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },

    /* characteristic value 2 */
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_VENDOR_HANDSHAKE),
            HI_WORD(GATT_UUID_CHAR_VENDOR_HANDSHAKE),
        },
        1,                                          /* variable size */
        (void *)NULL,
        GATT_PERM_READ | GATT_PERM_WRITE            /* wPermissions */
    },

    /* client characteristic configuration 3 */
    {
        ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_CCCD_APPL,                   /* wFlags */
        {   /* bTypeValue */
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
    },

    /* <<Characteristic>> 4 */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* flags */
        {                                           /* type_value */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            (GATT_CHAR_PROP_WRITE | GATT_CHAR_PROP_WRITE_NO_RSP) /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* permissions */
    },

    /* characteristic value 5 */
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* flags */
        {                                          /* type_value */
            LO_WORD(GATT_UUID_CHAR_VENDOR_TEST_MODE),
            HI_WORD(GATT_UUID_CHAR_VENDOR_TEST_MODE)
        },
        0,                                          /* bValueLen */
        NULL,
        GATT_PERM_WRITE                             /* permissions */
    },
};

/**
 * @brief write characteristic data from service.
 *
 * @param ServiceID          ServiceID to be written.
 * @param iAttribIndex       Attribute index of characteristic.
 * @param wLength            length of value to be written.
 * @param pValue             value to be written.
 * @return Profile procedure result
*/
T_APP_RESULT  vendor_svc_attr_write_cb(uint8_t conn_id, uint8_t service_id, uint16_t attrib_index,
                                       T_WRITE_TYPE write_type,
                                       uint16_t length, uint8_t *p_value, P_FUN_WRITE_IND_POST_PROC *p_write_ind_post_proc)
{
    bool handle = true;
    T_VENDOR_CALLBACK_DATA callback_data;
    T_APP_RESULT  cause = APP_RESULT_SUCCESS;

    APP_PRINT_INFO1("[vendor_svc_attr_write_cb] attrib_index is %d", attrib_index);

    if (BLE_SERVICE_CHAR_VENDOR_HANDSHAKE_INDEX == attrib_index)
    {
        /* Make sure written value size is valid. */
        if ((length != 16) || (p_value == NULL))
        {
            handle = false;
            cause  = APP_RESULT_APP_ERR;
        }
        else
        {
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE;
            callback_data.msg_data.write_msg.write_type = VENDOR_WRITE_HANDSHAKE;
            callback_data.msg_data.write_msg.write_parameter.report_data.len = length;
            callback_data.msg_data.write_msg.write_parameter.report_data.report = p_value;
        }
    }
    else if (BLE_SERVICE_CHAR_VENDOR_TEST_MODE_INDEX == attrib_index)
    {
        callback_data.msg_type = SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE;
        callback_data.msg_data.write_msg.write_type = VENDOR_WRITE_TEST_MODE;
        callback_data.msg_data.write_msg.write_parameter.report_data.len = length;
        callback_data.msg_data.write_msg.write_parameter.report_data.report = p_value;
    }
    else
    {
        handle = false;
        cause = APP_RESULT_ATTR_NOT_FOUND;
    }

    if ((pfn_vendor_svc_cb) && (handle == true))
    {
        pfn_vendor_svc_cb(service_id, (void *)&callback_data);
    }

    return cause;

}

/**
 * @brief update CCCD bits from stack.
 *
 * @param ServiceId          Service ID.
 * @param Index          Attribute index of characteristic data.
 * @param wCCCBits         CCCD bits from stack.
 * @return None
*/
void vendor_svc_cccd_update_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t index,
                               uint16_t ccc_bits)
{
    bool handle = true;
    T_VENDOR_CALLBACK_DATA callback_data;
    callback_data.msg_type = SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION;

    APP_PRINT_INFO2("[vendor_svc_cccd_update_cb] index = %d, ccc_bits %x", index, ccc_bits);
    switch (index)
    {
    case GATT_SVC_VENDOR_HANDSHAKE_CHAR_CCCD_INDEX:
        {
            if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_NOTIFY)
            {
                callback_data.msg_type = SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION;
                callback_data.msg_data.notification_indification_index = VENDOR_NOTIFY_ENABLE;
            }
            else
            {
                callback_data.msg_type = SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION;
                callback_data.msg_data.notification_indification_index = VENDOR_NOTIFY_DISABLE;
            }
            break;
        }
    default:
        {
            handle = false;
            break;
        }
    }

    if (pfn_vendor_svc_cb && (handle == true))
    {
        /* Notify Application */
        pfn_vendor_svc_cb(service_id, (void *)&callback_data);
    }

    return;
}

/**
 * @brief Vendor Service Callbacks.
*/
const T_FUN_GATT_SERVICE_CBS vendor_svc_cbs =
{
    NULL,   // Read callback function pointer
    vendor_svc_attr_write_cb,  // Write callback function pointer
    vendor_svc_cccd_update_cb                    // CCCD update callback function pointer
};

/**
  * @brief       Add vendor service to the BLE stack database.
  *
  *
  * @param[in]   p_func  Callback when service attribute was read, write or cccd update.
  * @return Service id generated by the BLE stack: @ref T_SERVER_ID.
  * @retval 0xFF Operation failure.
  * @retval Others Service id assigned by stack.
  *
  * <b>Example usage</b>
  * \code{.c}
     void profile_init()
     {
         server_init(1);
         bas_id = vendor_svc_add_service(app_handle_profile_message);
     }
  * \endcode
  */
uint8_t vendor_svc_add_service(void *p_func)
{
    uint8_t service_id;
    if (false == server_add_service(&service_id,
                                    (uint8_t *)vendor_svc_bas_attr_tbl,
                                    sizeof(vendor_svc_bas_attr_tbl),
                                    vendor_svc_cbs))
    {
        APP_PRINT_INFO1("vendor_svc_add_service: service_id %d", service_id);
        service_id = 0xff;
        return service_id;
    }
    pfn_vendor_svc_cb = (P_FUN_SERVER_GENERAL_CB)p_func;
    return service_id;
}
