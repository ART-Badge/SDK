enum { __FILE_NUM__ = 0 };

/**
*********************************************************************************************************
*               Copyright(c) 2014, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     atvv_service.c
* @brief    ATV voice service source file.
* @details  Interfaces to access ATV voice service.
* @author   Chenjie Jin
* @date     2017-12-8
* @version  v0.1
*********************************************************************************************************
*/
#include "board.h"
#include "stdint.h"
#include "stddef.h"
#include "string.h"
#include "trace.h"
#include "atvv_service.h"

#if (VOICE_FLOW_SEL == ATV_GOOGLE_VOICE_FLOW)

/********************************************************************************************************
* local static variables defined here, only used in this source file.
********************************************************************************************************/
static P_FUN_SERVER_GENERAL_CB pfn_atvv_cb = NULL;

T_ATVV_GLOBAL_DATA atvv_global_data;

const uint8_t GATT_UUID_ATVV_SERVICE[16] = {0x64, 0xB6, 0x17, 0xF6, 0x01, 0xAF, 0x7D, 0xBC, 0x05, 0x4F, 0x21, 0x5A, 0x01, 0x00, 0x5E, 0xAB};

/**< @brief  profile/service definition.  */
const T_ATTRIB_APPL atvv_attr_tbl[] =
{
    /* <<Primary Service>>, .. 0*/
    {
        (ATTRIB_FLAG_VOID | ATTRIB_FLAG_LE),  /* wFlags     */
        {
            LO_WORD(GATT_UUID_PRIMARY_SERVICE),
            HI_WORD(GATT_UUID_PRIMARY_SERVICE),     /* bTypeValue */
        },
        UUID_128BIT_SIZE,                     /* bValueLen     */
        (void *)GATT_UUID_ATVV_SERVICE,        /* pValueContext */
        GATT_PERM_READ                              /* wPermissions  */
    },

    /* ATVV_CHAR_TX */
    /* Characteristic Definition, 1 */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_WRITE,                   /* characteristic properties */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },

    /* Characteristic Value, 2 */
    {
        ATTRIB_FLAG_VALUE_APPL | ATTRIB_FLAG_UUID_128BIT,                              /* wFlags */
        {   /* bTypeValue */
            GATT_UUID_ATV_CHAR_TX
        },
        0,                                                 /* bValueLen */
        NULL,
        GATT_PERM_WRITE                 /* wPermissions */
    },

    /* ATVV_CHAR_RX */
    /* Characteristic, 3 */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ | GATT_CHAR_PROP_NOTIFY,                    /* characteristic properties */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },

    /* Characteristic Value, 4 */
    {
        ATTRIB_FLAG_VALUE_APPL | ATTRIB_FLAG_UUID_128BIT,                              /* wFlags */
        {   /* bTypeValue */
            GATT_UUID_ATV_CHAR_RX
        },
        0,                                                 /* bValueLen */
        NULL,
        GATT_PERM_READ                 /* wPermissions */
    },

    /* client characteristic configuration, 5 */
    {
        (ATTRIB_FLAG_VALUE_INCL |                   /* wFlags */
         ATTRIB_FLAG_CCCD_APPL),
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

    /* ATVV_CHAR_CTL */
    /* Characteristic, 6 */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ | GATT_CHAR_PROP_NOTIFY,                    /* characteristic properties */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },

    /* Characteristic Value, 7 */
    {
        ATTRIB_FLAG_VALUE_APPL | ATTRIB_FLAG_UUID_128BIT,                              /* wFlags */
        {   /* bTypeValue */
            GATT_UUID_ATV_CHAR_CTL
        },
        0,                                                 /* bValueLen */
        NULL,
        GATT_PERM_READ                 /* wPermissions */
    },

    /* client characteristic configuration, 8 */
    {
        (ATTRIB_FLAG_VALUE_INCL |                   /* wFlags */
         ATTRIB_FLAG_CCCD_APPL),
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
    }
};
/**< @brief  ATV Voice service size definition.  */
const int atvv_attr_tbl_size = sizeof(atvv_attr_tbl);

/**
 * @brief read characteristic data from service.
 *
 * @param conn_id           Connection ID.
 * @param service_id        ServiceID to be read.
 * @param attrib_index      Attribute index of getting characteristic data.
 * @param offset            Offset of characteritic to be read.
 * @param p_length          Length of getting characteristic data.
 * @param pp_value          Pointer to pointer of characteristic value to be read.
 * @return T_APP_RESULT
*/
T_APP_RESULT  attv_attr_read_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                                uint16_t offset, uint16_t *p_length, uint8_t **pp_value)
{
    T_APP_RESULT cause = APP_RESULT_SUCCESS;
    *p_length = 0;

    PROFILE_PRINT_INFO2("bas_attr_read_cb attrib_index = %d offset %x", attrib_index, offset);

    switch (attrib_index)
    {
    case GATT_SVC_ATVV_CHAR_RX_VALUE_INDEX:
        {
            T_ATVV_CALLBACK_DATA callback_data;
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE;
            callback_data.msg_data.read_value_index = ATVV_READ_CHAR_RX_INDEX;
            if (pfn_atvv_cb)
            {
                cause = pfn_atvv_cb(service_id, (void *)&callback_data);
            }

            if (cause != APP_RESULT_SUCCESS)
            {
                memset(atvv_global_data.char_rx_data_buff, 0, ATVV_CHAR_RX_DATA_LEN);
            }
            *pp_value = atvv_global_data.char_rx_data_buff;
            *p_length = ATVV_CHAR_RX_DATA_LEN;
        }
        break;

    case GATT_SVC_ATVV_CHAR_CTL_VALUE_INDEX:
        {
            T_ATVV_CALLBACK_DATA callback_data;
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE;
            callback_data.msg_data.read_value_index = ATVV_READ_CHAR_CTL_INDEX;
            if (pfn_atvv_cb)
            {
                cause = pfn_atvv_cb(service_id, (void *)&callback_data);
            }

            if (cause != APP_RESULT_SUCCESS)
            {
                memset(atvv_global_data.char_ctl_data_buff, 0, ATVV_CHAR_CTL_DATA_LEN);
            }

            *pp_value = atvv_global_data.char_ctl_data_buff;
            *p_length = ATVV_CHAR_CTL_DATA_LEN;
        }
        break;

    default:
        {
            PROFILE_PRINT_ERROR1("attv_attr_read_cb attrib_index = %d not found", attrib_index);
            cause  = APP_RESULT_ATTR_NOT_FOUND;
        }
        break;
    }
    return (cause);
}

void atvv_write_post_callback(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                              uint16_t length, uint8_t *p_value)
{
    T_ATVV_CALLBACK_DATA callback_data;

    APP_PRINT_INFO4("atvv_write_post_callback: conn_id %d, service_id %d, attrib_index 0x%x, length %d",
                    conn_id, service_id, attrib_index, length);

    if (GATT_SVC_ATVV_CHAR_TX_VALUE_INDEX == attrib_index)
    {
        /* Notify Application. */
        callback_data.msg_type = SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE;
        callback_data.msg_data.write.write_type = ATVV_WRITE_CHAR_TX_INDEX;
        callback_data.msg_data.write.write_parameter.report_data.len = length;
        callback_data.msg_data.write.write_parameter.report_data.report = p_value;

        if (pfn_atvv_cb)
        {
            pfn_atvv_cb(service_id, (void *)&callback_data);
        }
    }
}

/**
 * @brief write characteristic data from service.
 *
 * @param[in] conn_id       Connection id.
 * @param[in] service_id          ServiceID to be written.
 * @param[in] attrib_index          Attribute index of characteristic.
 * @param[in] write_type          Write type.
 * @param[in] length            Length of writing characteristic data.
 * @param[in] p_value            Pointer to characteristic data.
 * @param[in] p_write_ind_post_proc      Function pointer after ias_attr_write_cb.
 * @return TProfileResult
*/
T_APP_RESULT atvv_attr_write_cb(uint8_t conn_id, uint8_t service_id, uint16_t attrib_index,
                                T_WRITE_TYPE write_type,
                                uint16_t length, uint8_t *p_value, P_FUN_WRITE_IND_POST_PROC *p_write_ind_post_proc)
{
    T_APP_RESULT  cause  = APP_RESULT_SUCCESS;

    APP_PRINT_INFO1("atvv_attr_write_cb write_type = 0x%x", write_type);

    *p_write_ind_post_proc = atvv_write_post_callback;

    if (!p_value)
    {
        cause = APP_RESULT_INVALID_PDU;
    }
    else if (attrib_index == GATT_SVC_ATVV_CHAR_TX_VALUE_INDEX)
    {
        cause = APP_RESULT_SUCCESS;
    }
    else
    {
        cause = APP_RESULT_ATTR_NOT_FOUND;
    }

    return cause;
}


/**
 * @brief update CCCD bits from stack.
 *
 * @param conn_id           Connection ID.
 * @param service_id        Service ID.
 * @param index             Attribute index of characteristic data.
 * @param ccc_bits          CCCD bits from stack.
 * @return None
*/
void atvv_cccd_update_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t index, uint16_t ccc_bits)
{
    T_ATVV_CALLBACK_DATA callback_data;
    callback_data.msg_type = SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION;
    bool handle = true;

    PROFILE_PRINT_INFO2("atvv_cccd_update_cb index = %d ccc_bits %x", index, ccc_bits);
    switch (index)
    {
    case GATT_SVC_ATVV_CHAR_RX_CCCD_INDEX:
        if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_NOTIFY)
        {
            callback_data.msg_data.notification_indification_index = ATVV_CHAR_RX_NOTIFY_ENABLE;
        }
        else
        {
            callback_data.msg_data.notification_indification_index = ATVV_CHAR_RX_NOTIFY_DISABLE;
        }
        break;

    case GATT_SVC_ATVV_CHAR_CTL_CCCD_INDEX:
        if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_NOTIFY)
        {
            callback_data.msg_data.notification_indification_index = ATVV_CHAR_CTL_NOTIFY_ENABLE;
        }
        else
        {
            callback_data.msg_data.notification_indification_index = ATVV_CHAR_CTL_NOTIFY_DISABLE;
        }
        break;

    default:
        handle = false;
        break;
    }
    /* Notify Application. */
    if (pfn_atvv_cb && (handle == true))
    {
        pfn_atvv_cb(service_id, (void *)&callback_data);
    }

    return;
}

/*********************************************************************
 * SERVICE CALLBACKS
 */
// ATVV related Service Callbacks
const T_FUN_GATT_SERVICE_CBS atvv_cbs =
{
    attv_attr_read_cb,  // Read callback function pointer
    atvv_attr_write_cb, // Write callback function pointer
    atvv_cccd_update_cb  // CCCD update callback function pointer
};

/**
  * @brief add ATV voice service to application.
  *
  * @param[in] pFunc          pointer of app callback function called by profile.
  * @return service ID auto generated by profile layer.
  * @retval  ServiceId
  */
uint8_t atvv_add_service(void *p_func)
{
    uint8_t service_id;
    if (false == server_add_service(&service_id,
                                    (uint8_t *)atvv_attr_tbl,
                                    atvv_attr_tbl_size,
                                    atvv_cbs))
    {
        APP_PRINT_ERROR1("atvv_add_service: ServiceId %d", service_id);
        service_id = 0xff;
    }

    pfn_atvv_cb = (P_FUN_SERVER_GENERAL_CB)p_func;
    return service_id;
}

#endif  /* (VOICE_FLOW_SEL == ATV_GOOGLE_VOICE_FLOW) */
