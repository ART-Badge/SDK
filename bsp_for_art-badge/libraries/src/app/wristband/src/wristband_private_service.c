enum { __FILE_NUM__ = 0 };

/**
*********************************************************************************************************
*               Copyright(c) 2014, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      wristband_private_service.c
* @brief     wristband_private_service implementation
* @details   wristband_private_service implementation
* @author
* @date
* @version   v0.1
* *********************************************************************************************************
*/


#include "wristband_private_service.h"
#include <string.h>
#include <trace.h>
#include <gap.h>
#include "app_task.h"
#include "ftl.h"
#include "app_flash_config.h"
#include "gap_adv.h"
#include "wristband_gap.h"

/********************************************************************************************************
* local static variables defined here, only used in this source file.
********************************************************************************************************/
/**<  Function pointer used to send event to application from BWPS extended profile. */
/**<  Initiated in BWPSExtended_AddService. */
const uint8_t GATT_UUID128_BWPS[16] = {0x12, 0xA2, 0x4D, 0x2E, 0xFE, 0x14, 0x48, 0x8e, 0x93, 0xD2, 0x17, 0x3C, 0xFF, 0x01, 0x00, 0x00};

/**<  Function pointer used to send event to application from wristband profile. Initiated in bwps_service_add_service. */
static P_FUN_SERVER_GENERAL_CB pfn_wristband_service_cb = NULL;

/* Receive data from client */
uint8_t receive_temp[245];

const T_ATTRIB_APPL bwps_service_tbl[] =
{
    /*--------------------------BWPS(bee wristband private protocol) Service ---------------------------*/
    /* <<Primary Service>>, .. 0*/
    {
        (ATTRIB_FLAG_VOID | ATTRIB_FLAG_LE),        /* wFlags     */
        {
            LO_WORD(GATT_UUID_PRIMARY_SERVICE),
            HI_WORD(GATT_UUID_PRIMARY_SERVICE),
        },
        UUID_128BIT_SIZE,                           /* bValueLen     */
        (void *)GATT_UUID128_BWPS,                  /* pValueContext */
        GATT_PERM_READ                              /* wPermissions  */
    },

    /* <<Characteristic>>, .. 1*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_WRITE,                   /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },

    /*  Tx characteristic value 2*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(BLE_UUID_BWPS_TX_CHARACTERISTIC),
            HI_WORD(BLE_UUID_BWPS_TX_CHARACTERISTIC),
        },
        0,                                          /* variable size */
        NULL,
        GATT_PERM_WRITE                             /* wPermissions */
    },

    /* <<Characteristic>>, .. 3*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_NOTIFY,                  /* characteristic properties */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },

    /* <<RX Characteristic>>, .. 4*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(BLE_UUID_BWPS_RX_CHARACTERISTIC),
            HI_WORD(BLE_UUID_BWPS_RX_CHARACTERISTIC),
        },
        0,                                          /* variable size */
        NULL,
        GATT_PERM_NOTIF_IND                         /* wPermissions */
    },

    /* client characteristic configuration 5*/
    {
        (ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_CCCD_APPL),    /* wFlags */
        {
            LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT), /* client char. config. bit field */
            HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)
        },
        2,                                          /* bValueLen */
        NULL,
        (GATT_PERM_READ | GATT_PERM_WRITE)          /* wPermissions */
    },

    /* used for device name update */
    /* <<Characteristic>>, .. 6*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_WRITE | GATT_CHAR_PROP_READ,                 /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },

    /*<<Characteristic>>, .. 7*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(BLE_UUID_BWPS_DEVICE_NAME),
            HI_WORD(BLE_UUID_BWPS_DEVICE_NAME),
        },
        0,                                          /* variable size */
        NULL,
        GATT_PERM_READ | GATT_PERM_WRITE            /* wPermissions */
    },

};


/**
 * @brief read characteristic data from service.
 *
 * @param ServiceId          ServiceID of characteristic data.
 * @param iAttribIndex       Attribute index of getting characteristic data.
 * @param iOffset              Used for Blob Read.
 * @param piLength           length of getting characteristic data.
 * @param ppValue            data got from service.
 * @return  Profile procedure result
*/
T_APP_RESULT   bwps_service_attr_read_cb(uint8_t conn_id, T_SERVER_ID service_id,
                                         uint16_t attrib_index, uint16_t offset, uint16_t *p_length, uint8_t **pp_value)
{
    T_APP_RESULT  wCause  = APP_RESULT_SUCCESS;

    switch (attrib_index)
    {
    default:
        APP_PRINT_ERROR1("bwps_service_attr_read_cb, Attr not found, index %d", attrib_index);
        wCause = APP_RESULT_ATTR_NOT_FOUND;
        break;
    case GATT_SRV_BWPS_DEVNAME_INDEX:
        {
            //return the device name information.
            extern uint8_t adv_data[ADVERTISING_PACKET_LENGTH];
            *pp_value  = adv_data + 5;
            *p_length = adv_data[3] - 1;
        }
        break;

    }

    return (wCause);
}


/**
 * @brief write characteristic data from stack.
 *
 * @param ServiceId          ServiceId generated when register to upper stack.
 * @param iAttribIndex          Attribute index of getting characteristic data.
 * @param wLength                length of data to be written.
 * @param pValue            pointer of data to be written.
 * @return TProfileResult   profile procedure results.
*/
T_APP_RESULT bwps_service_attr_write_cb(uint8_t conn_id, T_SERVER_ID service_id,
                                        uint16_t attrib_index, T_WRITE_TYPE write_type, uint16_t length, uint8_t *p_value,
                                        P_FUN_WRITE_IND_POST_PROC *p_write_ind_post_proc)
{
    T_APP_RESULT  wCause  = APP_RESULT_SUCCESS;
    T_IO_MSG bwps_msg;

    if (p_value == NULL)
    {
        APP_PRINT_ERROR0("bwps_service_attr_write_cb, p_value is NULL");
        wCause = APP_RESULT_INVALID_VALUE_SIZE;
        return wCause;
    }

    switch (attrib_index)
    {
    default:
        wCause = APP_RESULT_ATTR_NOT_FOUND;
        APP_PRINT_ERROR2("bwps_service_attr_write_cb Error: attrib_index 0x%x, length %d",
                         attrib_index,
                         length);
        break;
    case GATT_SRV_BWPS_TX_INDEX:
        APP_PRINT_INFO0("bwps_service_attr_write_cb, GATT_SRV_BWPS_TX_INDEX");

        /* copy gatt write value in tx_buffer, the first byte is value length*/
        receive_temp[0] = length;
        if (length <= 244)
        {
            memcpy(receive_temp + 1, p_value, length);
        }
        else
        {
            APP_PRINT_INFO0(" TOO LONG MESSAGE.\n");
        }

        bwps_msg.type = IO_MSG_TYPE_WRISTBNAD;
        bwps_msg.subtype = IO_MSG_BWPS_TX_VALUE;
        bwps_msg.u.buf = receive_temp;
        app_send_msg_to_apptask(&bwps_msg);

        break;
    case GATT_SRV_BWPS_DEVNAME_INDEX:
        APP_PRINT_INFO0("BWPS: update device name\n");
        if (length <= DEVICE_NAME_MAX_LENGTH)
        {
            /*update device name to advertising packet array*/
            extern uint8_t adv_data[ADVERTISING_PACKET_LENGTH];
            uint8_t DeviceName[GAP_DEVICE_NAME_LEN] = {0};
            memcpy(DeviceName, p_value, length);
            le_set_gap_param(GAP_PARAM_DEVICE_NAME, GAP_DEVICE_NAME_LEN, DeviceName);

            DeviceName[0] = length + 1;
            DeviceName[1] = 0x09;
            memcpy(DeviceName + 2, p_value, length);
            //save name into flash
            ftl_save(DeviceName, DEVICE_NAME_OFFSET, DEVICE_NAME_SIZE);

            uint8_t advdata_len = wristband_adv_data_init(DeviceName);
            le_adv_set_param(GAP_PARAM_ADV_DATA, advdata_len, adv_data);

        }
        else
        {
            APP_PRINT_INFO0("device name exceed max length.\n");
        }
        break;
    }

    return wCause;
}

/**
 * @brief update CCCD bits from stack.
 *
 * @param ServiceId          ServiceId generated when register to upper stack.
 * @param Index          Attribute index of getting characteristic data.
 * @param wCCCBits                CCCD bits to be updated.
 * @return none.
*/
void bwps_service_cccd_update_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t index,
                                 uint16_t cccbits)
{
    TBWPS_CALLBACK_DATA callback_data;
    bool is_handled = false;
    callback_data.conn_id = conn_id;
    callback_data.msg_type = SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION;
    APP_PRINT_INFO2("bwps_service_cccd_update_cb: index = %d, cccbits 0x%x", index, cccbits);

    switch (index)
    {
    case GATT_SRV_BWPS_CCCD_INDEX:
        {
            if (cccbits & GATT_CLIENT_CHAR_CONFIG_NOTIFY)
            {
                // Enable Indication
            }
            else
            {
                // Disable Indication
            }
        }
        break;

    default:
        break;
    }
    /* Notify Application. */
    if (pfn_wristband_service_cb && (is_handled == true))
    {
        pfn_wristband_service_cb(service_id, (void *)&callback_data);
    }
}



/**< @brief  BWPS Extended Service Callbacks, will be registered to stack.  */
const T_FUN_GATT_SERVICE_CBS bwps_service_cbs =
{
    bwps_service_attr_read_cb,  // Read callback function pointer
    bwps_service_attr_write_cb, // Write callback function pointer
    bwps_service_cccd_update_cb  // CCCD update callback function pointer
};


/**
 * @brief Add BWPS extended service to stack/profile.
 *
 * @param pFunc          pointer of function to send data to application from BWPS extended service.
 * @return ServiceId.   the service ID auto generated by stack when register this service to stack
*/
uint8_t bwps_service_add_service(void *p_func)
{
    uint8_t bwps_service_id;
    /* register service callbacks to stack/profile. */
    if (false == server_add_service(&bwps_service_id,
                                    (uint8_t *)bwps_service_tbl,
                                    sizeof(bwps_service_tbl),
                                    bwps_service_cbs))
    {
        APP_PRINT_ERROR0("bwps_service_add_service: fail");
        bwps_service_id = 0xff;
        return bwps_service_id;
    }

    pfn_wristband_service_cb = (P_FUN_SERVER_GENERAL_CB)p_func;
    return bwps_service_id;
}


