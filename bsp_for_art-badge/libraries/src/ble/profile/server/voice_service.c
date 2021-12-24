/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     voice_service.c
* @brief  This file provides a vendor defined voice profile.
* @details
* @author
* @date
* @version  v1.1
******************************************************************************
* @attention
* <h2><center>&copy; COPYRIGHT 2015 Realtek Semiconductor Corporation</center></h2>
******************************************************************************
*/

/*============================================================================*
  *                                 Header Files
  *============================================================================*/

#include "trace.h"
#include "gatt.h"
#include "gap.h"
#include "string.h"
#include "gap_conn_le.h"
#include "gap_adv.h"
#include "gap_msg.h"
#include "voice_service.h"

/*============================================================================*
  *                                 External Function
  *============================================================================*/


/*============================================================================*
  *                                 Constants
  *============================================================================*/
const uint8_t GATT_UUID128_VOICE_SRV[16] = { 0x12, 0xA2, 0x4D, 0x2E, 0xFE, 0x14, 0x48, 0x8e, 0x93, 0xD2, 0x17, 0x3C, 0xFD, 0x03, 0x00, 0x00};

static P_FUN_SERVER_GENERAL_CB pfn_voice_cb = NULL;

/**@brief  profile/service definition.
*   here is the Voice Service table
*/
const T_ATTRIB_APPL VOICE_SERVICE_GATT_TABLE[] =
{
    /*--------------------------Voice Service ---------------------------*/
    /* <<Primary Service>>, 0 */
    {
        (ATTRIB_FLAG_VOID | ATTRIB_FLAG_LE),  /* wFlags     */
        {
            LO_WORD(GATT_UUID_PRIMARY_SERVICE),
            HI_WORD(GATT_UUID_PRIMARY_SERVICE),     /* bTypeValue */
        },
        UUID_128BIT_SIZE,                     /* bValueLen     */
        (void *)GATT_UUID128_VOICE_SRV,        /* pValueContext */
        GATT_PERM_READ                              /* wPermissions  */
    },

    /* VOICE_CHAR_TX */
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
        ATTRIB_FLAG_VALUE_APPL,                     /* flags */
        {                                          /* type_value */
            LO_WORD(GATT_UUID_VOICE_CHAR_CTL),
            HI_WORD(GATT_UUID_VOICE_CHAR_CTL)
        },
        0,                                          /* bValueLen */
        NULL,
        GATT_PERM_WRITE                             /* permissions */
    },

    /* VOICE_CHAR_DATA */
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
        ATTRIB_FLAG_VALUE_APPL,                     /* flags */
        {                                          /* type_value */
            LO_WORD(GATT_UUID_VOICE_CHAR_DATA),
            HI_WORD(GATT_UUID_VOICE_CHAR_DATA)
        },
        0,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                             /* permissions */
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

    /* VOICE_CHAR_CMD */
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
        ATTRIB_FLAG_VALUE_APPL,                     /* flags */
        {                                          /* type_value */
            LO_WORD(GATT_UUID_VOICE_CHAR_CMD),
            HI_WORD(GATT_UUID_VOICE_CHAR_CMD)
        },
        0,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                             /* permissions */
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
    },
};

/*============================================================================*
  *                                 Private Functions
  *============================================================================*/
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
T_APP_RESULT voice_attr_read_cb(uint8_t conn_id, uint8_t service_id, uint16_t attrib_index,
                                uint16_t offset, uint16_t *p_length, uint8_t **pp_value)
{
    T_APP_RESULT cause = APP_RESULT_SUCCESS;
    *p_length = 0;

    PROFILE_PRINT_INFO2("voice_attr_read_cb attrib_index = %d offset %x", attrib_index, offset);

    switch (attrib_index)
    {
    default:
        {
            PROFILE_PRINT_ERROR1("voice_attr_read_cb attrib_index = %d not found", attrib_index);
            cause  = APP_RESULT_ATTR_NOT_FOUND;
        }
        break;
    }
    return (cause);
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
T_APP_RESULT voice_attr_write_cb(uint8_t conn_id, uint8_t service_id, uint16_t attrib_index,
                                 T_WRITE_TYPE write_type,
                                 uint16_t length, uint8_t *p_value, P_FUN_WRITE_IND_POST_PROC *p_write_ind_post_proc)
{
    T_APP_RESULT  cause  = APP_RESULT_SUCCESS;
    T_VOICE_CALLBACK_DATA callback_data;
    bool handle = true;

    APP_PRINT_INFO1("voice_attr_write_cb write_type = 0x%x", write_type);

    if (!p_value)
    {
        cause = APP_RESULT_INVALID_PDU;
        handle = false;
    }
    else if (attrib_index == GATT_SRV_VOICE_CHAR_CTL_VALUE_INDEX)
    {
        callback_data.msg_type = SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE;
        callback_data.msg_data.write.write_type = VOICE_WRITE_VOIC_CTL;
        callback_data.msg_data.write.write_parameter.report_data.len = length;
        callback_data.msg_data.write.write_parameter.report_data.report = p_value;
    }
    else
    {
        cause = APP_RESULT_ATTR_NOT_FOUND;
        handle = false;
    }

    if (pfn_voice_cb && (handle == true))
    {
        pfn_voice_cb(service_id, (void *)&callback_data);
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
void voice_cccd_update_cb(uint8_t conn_id, uint8_t service_id, uint16_t index, uint16_t ccc_bits)
{
    T_VOICE_CALLBACK_DATA callback_data;
    callback_data.msg_type = SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION;
    bool handle = true;

    PROFILE_PRINT_INFO2("voice_cccd_update_cb index = %d ccc_bits %x", index, ccc_bits);
    switch (index)
    {
    case GATT_SRV_VOICE_CHAR_DATA_CCCD_INDEX:
        if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_NOTIFY)
        {
            callback_data.msg_data.notification_indification_index = VOICE_NOTIFY_DATA_ENABLE;
        }
        else
        {
            callback_data.msg_data.notification_indification_index = VOICE_NOTIFY_DATA_DISABLE;
        }
        break;

    case GATT_SRV_VOICE_CHAR_CMD_CCCD_INDEX:
        if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_NOTIFY)
        {
            callback_data.msg_data.notification_indification_index = VOICE_NOTIFY_CMD_ENABLE;
        }
        else
        {
            callback_data.msg_data.notification_indification_index = VOICE_NOTIFY_CMD_DISABLE;
        }
        break;

    default:
        handle = false;
        break;
    }
    /* Notify Application. */
    if (pfn_voice_cb && (handle == true))
    {
        pfn_voice_cb(service_id, (void *)&callback_data);
    }

    return;
}

/*============================================================================*
  *                                 Public Functions
  *============================================================================*/
/**
 * @brief Vendor ble Service Callbacks.
*/
const T_FUN_GATT_SERVICE_CBS VOICE_SERVICE_CBS =
{
    voice_attr_read_cb,   /*Read callback function pointer*/
    voice_attr_write_cb,  /*Write callback function pointer*/
    voice_cccd_update_cb,   /*CCCD update callback function pointer*/
};

/**
 * @brief  add Vendor ble service to application.
 *
 * @param  p_func          pointer of app callback function called by profile.
 * @return service ID auto generated by profile layer.
 * @retval service_id
*/
uint8_t voice_service_add_service(void *p_func)
{
    uint8_t service_id;
    if (false == server_add_service(&service_id,
                                    (uint8_t *)VOICE_SERVICE_GATT_TABLE,
                                    sizeof(VOICE_SERVICE_GATT_TABLE),
                                    VOICE_SERVICE_CBS))
    {
        APP_PRINT_ERROR1("[voice_service_add_service]failed, service_id = %d", service_id);
        service_id = 0xff;
        return service_id;
    }
    pfn_voice_cb = (P_FUN_SERVER_GENERAL_CB)p_func;
    APP_PRINT_INFO1("[voice_service_add_service] service_id = %d", service_id);
    return service_id;
}

