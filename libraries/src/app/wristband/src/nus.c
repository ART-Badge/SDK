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
#include "nus.h"
#include <gap.h>
#include "app_task.h"


#define BLE_UUID_NUS_TX_7F01            0xFB, 0x34, 0x9B, 0x7F, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x01, 0x7F, 0x18, 0x16
#define BLE_UUID_NUS_RX_7F02            0xFB, 0x34, 0x9B, 0x7F, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x02, 0x7F, 0x18, 0x16
#define BLE_UUID_NUS_FITNESS_7F03       0xFB, 0x34, 0x9B, 0x7F, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x03, 0x7F, 0x18, 0x16
#define BLE_UUID_NUS_MASS_7F04          0xFB, 0x34, 0x9B, 0x7F, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x04, 0x7F, 0x18, 0x16



const uint8_t GATT_UUID128_NUS[16] = {0xFB, 0x34, 0x9B, 0x7F, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x00, 0x7F, 0x18, 0x16};


static P_FUN_SERVER_GENERAL_CB pfn_nus_cb = NULL;

static uint8_t nus_receive_buf[245];


/**< @brief  profile/service definition.  */
const T_ATTRIB_APPL simple_ble_service_tbl[] =
{
    /* <<Primary Service>>, .. 0*/
    {
        (ATTRIB_FLAG_VOID | ATTRIB_FLAG_LE),        /* wFlags     */
        {
            LO_WORD(GATT_UUID_PRIMARY_SERVICE),
            HI_WORD(GATT_UUID_PRIMARY_SERVICE),
        },
        UUID_128BIT_SIZE,                           /* bValueLen     */
        (void *)GATT_UUID128_NUS,                  /* pValueContext */
        GATT_PERM_READ                              /* wPermissions  */
    },
    /* <<Characteristic>> 0x7F01 index 1*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_WRITE | GATT_CHAR_PROP_NOTIFY | GATT_CHAR_PROP_WRITE_NO_RSP,                   /* characteristic properties */
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
            BLE_UUID_NUS_TX_7F01
        },
        0,                                          /* variable size */
        NULL,
        GATT_PERM_WRITE | GATT_PERM_NOTIF_IND                           /* wPermissions */
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
    /* <<Characteristic>>, .. 4*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_WRITE | GATT_CHAR_PROP_NOTIFY | GATT_CHAR_PROP_WRITE_NO_RSP,                  /* characteristic properties */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },

    /* <<RX Characteristic>>, .. 5*/
    {
        ATTRIB_FLAG_VALUE_APPL | ATTRIB_FLAG_UUID_128BIT,                     /* wFlags */
        {   /* bTypeValue */
            BLE_UUID_NUS_RX_7F02
        },
        0,                                          /* variable size */
        NULL,
        GATT_PERM_WRITE | GATT_PERM_NOTIF_IND                       /* wPermissions */
    },

    /* client characteristic configuration 6*/
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
    /* <<Characteristic>> 0x7F03 index 7*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_WRITE | GATT_CHAR_PROP_NOTIFY | GATT_CHAR_PROP_WRITE_NO_RSP,                   /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*  Tx characteristic value index 8*/
    {
        ATTRIB_FLAG_VALUE_APPL | ATTRIB_FLAG_UUID_128BIT,                     /* wFlags */
        {   /* bTypeValue */
            BLE_UUID_NUS_FITNESS_7F03
        },
        0,                                          /* variable size */
        NULL,
        GATT_PERM_WRITE | GATT_PERM_NOTIF_IND                           /* wPermissions */
    },
    /* client characteristic configuration 9*/
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
    /* <<Characteristic>> 0x7F04 index 10*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_WRITE | GATT_CHAR_PROP_NOTIFY | GATT_CHAR_PROP_WRITE_NO_RSP,                   /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*  Tx characteristic value index 11*/
    {
        ATTRIB_FLAG_VALUE_APPL | ATTRIB_FLAG_UUID_128BIT,                     /* wFlags */
        {   /* bTypeValue */
            BLE_UUID_NUS_MASS_7F04
        },
        0,                                          /* variable size */
        NULL,
        GATT_PERM_WRITE | GATT_PERM_NOTIF_IND                           /* wPermissions */
    },
    /* client characteristic configuration 12*/
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

T_APP_RESULT nus_attr_write_cb(uint8_t conn_id, T_SERVER_ID service_id,
                               uint16_t attrib_index, T_WRITE_TYPE write_type, uint16_t length, uint8_t *p_value,
                               P_FUN_WRITE_IND_POST_PROC *p_write_ind_post_proc)
{
    T_APP_RESULT  wCause  = APP_RESULT_SUCCESS;

    if (p_value == NULL)
    {
        APP_PRINT_ERROR0("nus_service_attr_write_cb, p_value is NULL");
        wCause = APP_RESULT_INVALID_VALUE_SIZE;
        return wCause;
    }

    switch (attrib_index)
    {
    default:
        wCause = APP_RESULT_ATTR_NOT_FOUND;
        APP_PRINT_ERROR2("nus_attr_write_cb Error: attrib_index 0x%x, length %d",
                         attrib_index,
                         length);
        break;
    case GATT_NUS_TX_7F01_INDEX:
        APP_PRINT_INFO0("NUS: GATT_NUS_TX_7F01_INDEX");
        nus_receive_buf[0] = nus_service_proto_rx_state;
        break;
    case GATT_NUS_RX_7F02_INDEX:
        APP_PRINT_INFO0("NUS: GATT_NUS_RX_7F02_INDEX");
        nus_receive_buf[0] = nus_service_proto_tx_state;
        break;
    case GATT_NUS_FITNESS_7F03_INDEX:
        APP_PRINT_INFO0("NUS: GATT_NUS_FITNESS_7F03_INDEX");
        nus_receive_buf[0] = nus_service_sportdata_tx_state;
        break;
    case GATT_NUS_MASS_7F04_INDEX:
        APP_PRINT_INFO0("NUS: GATT_NUS_MASS_7F04_INDEX");
        nus_receive_buf[0] = nus_service_bigdata_rx_state;
        break;
    }
    T_IO_MSG bwps_msg;
    nus_receive_buf[1] = length;
    if (length <= 244)
    {
        memcpy(nus_receive_buf + 2, p_value, length);
    }
    else
    {
        APP_PRINT_INFO0(" TOO LONG MESSAGE.\n");
    }

    bwps_msg.type = IO_MSG_TYPE_WRISTBNAD;
    bwps_msg.subtype = IO_MSG_TYPE_NUS_WRITE;
    bwps_msg.u.buf = nus_receive_buf;
    app_send_msg_to_apptask(&bwps_msg);
    return wCause;
}

void nus_cccd_update_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t index,
                        uint16_t cccbits)
{
    bool is_handled = false;
    APP_PRINT_INFO2("bwps_service_cccd_update_cb: index = %d, cccbits 0x%x", index, cccbits);

    switch (index)
    {
    case GATT_NUS_RX_7F02_CCCD_INDEX:
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
    if (pfn_nus_cb && (is_handled == true))
    {
        pfn_nus_cb(service_id, NULL);
    }
}


const T_FUN_GATT_SERVICE_CBS nus_cbs =
{
    NULL,  // Read callback function pointer
    nus_attr_write_cb, // Write callback function pointer
    nus_cccd_update_cb  // CCCD update callback function pointer
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

T_SERVER_ID nus_add_service(void *p_func)
{
    T_SERVER_ID service_id;
    if (false == server_add_service(&service_id,
                                    (uint8_t *)simple_ble_service_tbl,
                                    sizeof(simple_ble_service_tbl),
                                    nus_cbs))
    {
        APP_PRINT_ERROR0("nus_add_service: fail");
        service_id = 0xff;
        return service_id;
    }

    pfn_nus_cb = (P_FUN_SERVER_GENERAL_CB)p_func;
    return service_id;
}

