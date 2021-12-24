/**
*********************************************************************************************************
*               Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     simple_ble_service.c
* @brief    simple BLE profile source file.
* @details  Demonstration of how to implement a self-definition profile.
* @author
* @date
* @version
*********************************************************************************************************
*/

#include <string.h>
#include <trace.h>
#include "aps.h"
#include <gap.h>
#include "app_task.h"


#define BLE_UUID_APS_COMM_4A02          0xFB, 0x34, 0x9B, 0x7F, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x02, 0x4A, 0x18, 0x16

const uint8_t GATT_UUID128_APS[16] = {0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x02, 0x38, 0x00, 0x00};


static P_FUN_SERVER_GENERAL_CB pfn_aps_cb = NULL;


/**< @brief  profile/service definition.  */
const T_ATTRIB_APPL aps_attr_tbl[] =
{
    /* <<Primary Service>>, .. 0*/
    {
        (ATTRIB_FLAG_VOID | ATTRIB_FLAG_LE),        /* wFlags     */
        {
            LO_WORD(GATT_UUID_PRIMARY_SERVICE),
            HI_WORD(GATT_UUID_PRIMARY_SERVICE),
        },
        UUID_128BIT_SIZE,                           /* bValueLen     */
        (void *)GATT_UUID128_APS,                  /* pValueContext */
        GATT_PERM_READ                              /* wPermissions  */
    },
    /* <<Characteristic>> 0x3802 index 1*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_WRITE | GATT_CHAR_PROP_NOTIFY | GATT_CHAR_PROP_READ,                   /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*  Tx characteristic value 2*/
    {
        ATTRIB_FLAG_VALUE_APPL | ATTRIB_FLAG_UUID_128BIT,                     /* wFlags */
        {   /* bTypeValue */
            BLE_UUID_APS_COMM_4A02
        },
        0,                                          /* variable size */
        NULL,
        GATT_PERM_WRITE | GATT_PERM_NOTIF_IND | GATT_PERM_READ                           /* wPermissions */
    },
    /* client characteristic configuration 3*/
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
};

T_APP_RESULT  aps_attr_read_cb(uint8_t conn_id, T_SERVER_ID service_id,
                               uint16_t attrib_index, uint16_t offset, uint16_t *p_length, uint8_t **pp_value)
{
    T_APP_RESULT  cause  = APP_RESULT_SUCCESS;

    switch (attrib_index)
    {
    default:
        APP_PRINT_ERROR1("aps_attr_read_cb, Attr not found, index %d", attrib_index);
        cause = APP_RESULT_ATTR_NOT_FOUND;
        break;
    case GATT_APS_COMM_4B02_INDEX:
        {
//            TSIMP_CALLBACK_DATA callback_data;
//            callback_data.msg_type = SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE;
//            callback_data.msg_data.read_value_index = SIMP_READ_V1;
//            callback_data.conn_id = conn_id;
//            if (pfn_simp_ble_service_cb)
//            {
//                pfn_simp_ble_service_cb(service_id, (void *)&callback_data);
//            }
//            *pp_value = simple_char_read_value;
//            *p_length = simple_char_read_len;
        }
        break;

    }

    return (cause);
}

T_APP_RESULT aps_attr_write_cb(uint8_t conn_id, T_SERVER_ID service_id,
                               uint16_t attrib_index, T_WRITE_TYPE write_type, uint16_t length, uint8_t *p_value,
                               P_FUN_WRITE_IND_POST_PROC *p_write_ind_post_proc)
{
    T_APP_RESULT  wCause  = APP_RESULT_SUCCESS;

    if (p_value == NULL)
    {
        APP_PRINT_ERROR0("aps_service_attr_write_cb, p_value is NULL");
        wCause = APP_RESULT_INVALID_VALUE_SIZE;
        return wCause;
    }

    switch (attrib_index)
    {
    default:
        wCause = APP_RESULT_ATTR_NOT_FOUND;
        APP_PRINT_ERROR2("aps_attr_write_cb Error: attrib_index 0x%x, length %d",
                         attrib_index,
                         length);
        break;
    case GATT_APS_COMM_4B02_INDEX:
        APP_PRINT_INFO0("APS: GATT_APS_COMM_4B02_INDEX");
        break;
    }
    return wCause;
}

void aps_cccd_update_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t index,
                        uint16_t cccbits)
{
    bool is_handled = false;
    APP_PRINT_INFO2("aps_cccd_update_cb: index = %d, cccbits 0x%x", index, cccbits);

    switch (index)
    {
    case GATT_APS_COMM_4B02_CCCD_INDEX:
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
    if (pfn_aps_cb && (is_handled == true))
    {
        pfn_aps_cb(service_id, NULL);
    }
}


const T_FUN_GATT_SERVICE_CBS aps_cbs =
{
    aps_attr_read_cb,  // Read callback function pointer
    aps_attr_write_cb, // Write callback function pointer
    aps_cccd_update_cb  // CCCD update callback function pointer
};

/**
  * @brief Add simple BLE service to the BLE stack database.
  *
  * @param[in]   p_func  Callback when service attribute was read, write or cccd update.
  * @return Service id generated by the BLE stack: @ref T_SERVER_ID.
  * @retval 0xFF Operation failure.
  * @retval others Service id assigned by stack.
  *
  */

T_SERVER_ID aps_add_service(void *p_func)
{
    T_SERVER_ID service_id;
    if (false == server_add_service(&service_id,
                                    (uint8_t *)aps_attr_tbl,
                                    sizeof(aps_attr_tbl),
                                    aps_cbs))
    {
        APP_PRINT_ERROR0("aps_add_service: fail");
        service_id = 0xff;
        return service_id;
    }

    pfn_aps_cb = (P_FUN_SERVER_GENERAL_CB)p_func;
    return service_id;
}

