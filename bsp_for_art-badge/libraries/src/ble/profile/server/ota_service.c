/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     ota_service.c
* @brief
* @details
* @author   Ken_mei
* @date     14-May-2018
* @version  v1.0.0
******************************************************************************
* @attention
* <h2><center>&copy; COPYRIGHT 2015 Realtek Semiconductor Corporation</center></h2>
******************************************************************************
*/

#include <string.h>
#include "gatt.h"
#include "patch_header_check.h"
#include "flash_map.h"
#include "ota_service.h"
#include "otp.h"
#include "trace.h"
#include "board.h"

/*============================================================================*
 *                              Micro
 *============================================================================*/
#define SOCV_CFG    0
#define SYS_CFG     1
#define OTA_HEADER  2
#define SECURE_BOOT 3
#define ROM_PATCH   4
#define APP_IMG     5
#define APP_DATA1   6
#define APP_DATA2   7
#define APP_DATA3   8
#define APP_DATA4   9
#define APP_DATA5   10
#define APP_DATA6   11

#define IMAGE_NOEXIST         0
#define IMAGE_LOCATION_BANK0  1
#define IMAGE_LOCATION_BANK1  2
#define IMAGE_FIX_BANK_EXIST  3

/*============================================================================*
 *                              External Variables
 *============================================================================*/

/*============================================================================*
 *                              Local Variables
 *============================================================================*/
/**<  Function pointer used to send event to application from BWPS extended profile. */
P_FUN_SERVER_GENERAL_CB pfn_ota_service_cb = NULL;
uint8_t mac_addr[6];
uint32_t patch_version = 0;
uint32_t patch_ext_version = 0;
uint32_t app_version = 0;
#if (SUPPORT_OTA_PROTOCOL_TYPE_CHARACTERISTIC == 1)
uint16_t protocol_type = 0x0;
#endif

const uint8_t GATT_UUID_OTA_SERVICE[16] = { 0x12, 0xA2, 0x4D, 0x2E, 0xFE, 0x14, 0x48, 0x8e, 0x93, 0xD2, 0x17, 0x3C, 0xFF, 0xD0, 0x00, 0x00};

/**< @brief  profile/service definition.
*   here is an example of OTA service table
*   including Write
*/
const T_ATTRIB_APPL gatt_Ota_service_table[] =
{
    /*--------------------------OTA Service ---------------------------*/
    /* <<Primary Service>>, .. 0*/
    {
        (ATTRIB_FLAG_VOID | ATTRIB_FLAG_LE),  /* wFlags     */
        {
            LO_WORD(GATT_UUID_PRIMARY_SERVICE),
            HI_WORD(GATT_UUID_PRIMARY_SERVICE),     /* bTypeValue */
        },
        UUID_128BIT_SIZE,                     /* bValueLen     */
        (void *)GATT_UUID_OTA_SERVICE,        /* pValueContext */
        GATT_PERM_READ                              /* wPermissions  */
    },

    /* <<Characteristic1>>, .. 1*/                         //----------------------OTA CMD        1
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_WRITE_NO_RSP,                    /* characteristic properties */
            //XXXXMJMJ GATT_CHAR_PROP_INDICATE,                  /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*  OTA characteristic value 2*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_OTA),
            HI_WORD(GATT_UUID_CHAR_OTA),
        },
        2,                                          /* variable size */
        (void *)NULL,
        GATT_PERM_READ | GATT_PERM_WRITE            /* wPermissions */
    },

    /* <<Characteristic2>>, .. 3, MAC Address*/              //------------------------MAC Address  2
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ,                    /* characteristic properties */
            //XXXXMJMJ GATT_CHAR_PROP_INDICATE,                  /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*  OTA characteristic value 4*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_MAC),
            HI_WORD(GATT_UUID_CHAR_MAC),
        },
        1,                                          /* variable size */
        (void *)NULL,
        GATT_PERM_READ            /* wPermissions */
    },

    /* <<Characteristic3>>, .. 5, Patch version*/            //-------------------------Patch Version  3
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ,                    /* characteristic properties */
            //XXXXMJMJ GATT_CHAR_PROP_INDICATE,                  /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*  OTA characteristic value 6*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_PATCH),
            HI_WORD(GATT_UUID_CHAR_PATCH),
        },
        1,                                          /* variable size */
        (void *)NULL,
        GATT_PERM_READ            /* wPermissions */
    },
    /* <<Characteristic4>>, .. 7 App version*/            //-----------------------------APP Version   4
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ,                    /* characteristic properties */
            //XXXXMJMJ GATT_CHAR_PROP_INDICATE,                  /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*  OTA characteristic value 8*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_APP_VERSION),
            HI_WORD(GATT_UUID_CHAR_APP_VERSION),
        },
        1,                                          /* variable size */
        (void *)NULL,
        GATT_PERM_READ            /* wPermissions */
    },
    /* <<Characteristic5>>, .. 9 Patch extension version*/  //--------------------Patch Ext Version    5
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ,                    /* characteristic properties */
            //XXXXMJMJ GATT_CHAR_PROP_INDICATE,                  /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*  OTA characteristic value 0x0A*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_PATCH_EXTENSION),
            HI_WORD(GATT_UUID_CHAR_PATCH_EXTENSION),
        },
        1,                                          /* variable size */
        (void *)NULL,
        GATT_PERM_READ            /* wPermissions */
    },

    /* <<Characteristic6>>, .. 0xB TEST MODE*/                 //------------------------MP TEST MODE     6
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_WRITE_NO_RSP,                    /* characteristic properties */
            //XXXXMJMJ GATT_CHAR_PROP_INDICATE,                  /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*  OTA characteristic value 0x0C*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_TEST_MODE),
            HI_WORD(GATT_UUID_CHAR_TEST_MODE),
        },
        0,                                          /* variable size */
        (void *)NULL,
        GATT_PERM_WRITE            /* wPermissions */
    },

    /* <<Characteristic7>>, .. 0x0D OTA Device info*/    //---------------------------Device info    7
    {
        ATTRIB_FLAG_VALUE_INCL,
        {
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ,
        },
        1,
        NULL,
        GATT_PERM_READ
    },
    /*  OTA Device info characteristic value 0x0E*/
    {
        ATTRIB_FLAG_VALUE_APPL,
        {
            LO_WORD(GATT_UUID_CHAR_DEVICE_INFO),
            HI_WORD(GATT_UUID_CHAR_DEVICE_INFO),
        },
        1,
        (void *)NULL,
        GATT_PERM_READ
    },
    /* <<Characteristic8>>, .. 0x0F OTA IMAGE COUNT TO UPDATE*/ //----------------------NUM OF IMG TO UPDATA  8
    {
        ATTRIB_FLAG_VALUE_INCL,
        {
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_WRITE,
        },
        1,
        NULL,
        GATT_PERM_READ
    },
    /*  OTA IMAGE COUNT TO UPDATE characteristic value 0x10*/
    {
        ATTRIB_FLAG_VALUE_APPL,
        {
            LO_WORD(GATT_UUID_CHAR_IMAGE_COUNT_TO_UPDATE),
            HI_WORD(GATT_UUID_CHAR_IMAGE_COUNT_TO_UPDATE),
        },
        5,
        (void *)NULL,
        GATT_PERM_WRITE
    },

    /* <<Characteristic9>>, .. 0x11,ota pack img version*/            //-------------------------ota pack image version  9
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ,                    /* characteristic properties */
            //XXXXMJMJ GATT_CHAR_PROP_INDICATE,                  /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*  OTA characteristic value 0x12*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_IMAGE_VERSION),
            HI_WORD(GATT_UUID_CHAR_IMAGE_VERSION),
        },
        1,                                          /* variable size */
        (void *)NULL,
        GATT_PERM_READ            /* wPermissions */
    },

#if (SUPPORT_OTA_PROTOCOL_TYPE_CHARACTERISTIC == 1)
    /* <<Characteristic10>>, .. 0x13,ota pack img version*/            //-------------------------ota protocol type  10
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ,                    /* characteristic properties */
            //XXXXMJMJ GATT_CHAR_PROP_INDICATE,                  /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*  OTA characteristic value 0x14*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_PROTOCOL_TYPE),
            HI_WORD(GATT_UUID_CHAR_PROTOCOL_TYPE),
        },
        1,                                          /* variable size */
        (void *)NULL,
        GATT_PERM_READ            /* wPermissions */
    },
#endif
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

T_APP_RESULT ota_attr_write_cb(uint8_t conn_id, uint8_t service_id, uint16_t attrib_index,
                               T_WRITE_TYPE write_type,
                               uint16_t length, uint8_t *p_value, P_FUN_WRITE_IND_POST_PROC *p_write_ind_post_proc)
{
    T_OTA_CALLBACK_DATA callback_data;
    T_APP_RESULT  wCause = APP_RESULT_SUCCESS;

    if (BLE_SERVICE_CHAR_OTA_INDEX == attrib_index)
    {
        /* Make sure written value size is valid. */
        if ((length != sizeof(uint8_t)) || (p_value == NULL))
        {
            wCause  = APP_RESULT_INVALID_VALUE_SIZE;
        }
        else
        {
            /* Notify Application. */
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE;
            callback_data.msg_data.write.opcode = OTA_WRITE_CHAR_VAL;
            callback_data.msg_data.write.u.value = p_value[0];

            if (pfn_ota_service_cb)
            {
                pfn_ota_service_cb(service_id, (void *)&callback_data);
            }
        }
    }
    else if (BLE_SERVICE_CHAR_IMAGE_COUNT_INDEX == attrib_index)
    {
        /* Make sure written value size is valid. */
        if ((length != OTA_CHAR_IMAGE_COUNT_LEN) || (p_value == NULL))
        {
            wCause  = APP_RESULT_INVALID_VALUE_SIZE;
        }
        else
        {
            /* Notify Application. */
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE;
            callback_data.msg_data.write.opcode = OTA_WRITE_IMAGE_COUNT_VAL;
            callback_data.msg_data.write.u.update_image_info.image_count = p_value[0];
            callback_data.msg_data.write.u.update_image_info.update_patch_version = CHAR2SHORT(&p_value[1]);
            callback_data.msg_data.write.u.update_image_info.update_app_version = CHAR2SHORT(&p_value[3]);

            if (pfn_ota_service_cb)
            {
                pfn_ota_service_cb(service_id, (void *)&callback_data);
            }
        }
    }
    else if (BLE_SERVICE_CHAR_TEST_MODE_INDEX == attrib_index)
    {
        /* Make sure written value size is valid. */
        if ((length != sizeof(uint8_t)) || (p_value == NULL))
        {
            wCause  = APP_RESULT_INVALID_VALUE_SIZE;
        }
        else
        {
            /* Notify Application. */
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE;
            callback_data.msg_data.write.opcode = OTA_WRITE_TEST_MODE_CHAR_VAL;
            callback_data.msg_data.write.u.value = p_value[0];

            if (pfn_ota_service_cb)
            {
                pfn_ota_service_cb(service_id, (void *)&callback_data);
            }
        }
    }
    else
    {
        DFU_PRINT_INFO2("==>[OTA]ota_attr_write_cb Error! attrib_index=0x%x, length=%d",
                        attrib_index,
                        length);
        wCause = APP_RESULT_ATTR_NOT_FOUND;
    }
    return wCause;

}


/**
 * @brief read characteristic data from service.
 *
 * @param ServiceId          ServiceID of characteristic data.
 * @param iAttribIndex       Attribute index of getting characteristic data.
 * @param iOffset            Used for Blob Read.
 * @param piLength           length of getting characteristic data.
 * @param ppValue            data got from service.
 * @return Profile procedure result
*/
T_APP_RESULT ota_attr_read_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                              uint16_t offset, uint16_t *p_length, uint8_t **pp_value)
{
    T_APP_RESULT  wCause = APP_RESULT_SUCCESS;
    T_OTA_CALLBACK_DATA callback_data;
    bool defer_handle = false;

    switch (attrib_index)
    {
    default:
        DFU_PRINT_INFO1("==>ota_attr_read_cb: attrib_index=%d not found!", attrib_index);
        wCause  = APP_RESULT_ATTR_NOT_FOUND;
        break;
    case BLE_SERVICE_CHAR_MAC_ADDRESS_INDEX:
        {
            gap_get_param(GAP_PARAM_BD_ADDR, mac_addr);
            uint8_t addr[6];
            for (int i = 0; i < 6; i++)
            {
                addr[i] = mac_addr[5 - i];
            }
            memcpy(mac_addr, addr, 6);
            *pp_value  = (uint8_t *)mac_addr;
            *p_length = sizeof(mac_addr);
        }
        break;
    case BLE_SERVICE_CHAR_PATCH_INDEX:
        {
            T_IMG_HEADER_FORMAT *p_header;
            uint32_t addr = get_header_addr_by_img_id(RomPatch);
            p_header = (T_IMG_HEADER_FORMAT *)addr;
            patch_version = p_header->git_ver.ver_info.version;
            DFU_PRINT_INFO2("ota_attr_read_cb: Patch addr=0x%x, version=0x%x", addr, patch_version);

            *pp_value  = (uint8_t *)&patch_version;
            *p_length = sizeof(patch_version);
        }
        break;

    case BLE_SERVICE_CHAR_APP_VERSION_INDEX:
        {
            T_IMG_HEADER_FORMAT *p_header;
            uint32_t addr = get_header_addr_by_img_id(AppPatch);
            p_header = (T_IMG_HEADER_FORMAT *)addr;
            app_version = p_header->git_ver.ver_info.version;
            DFU_PRINT_INFO2("ota_attr_read_cb: APP addr=0x%x, version=0x%x", addr, app_version);

            *pp_value  = (uint8_t *)&app_version;
            *p_length = sizeof(app_version);
        }
        break;

    case BLE_SERVICE_CHAR_PATCH_EXTENSION_INDEX:        //not used in bee2
        {

        }
        break;
    case BLE_SERVICE_CHAR_DEVICE_INFO_INDEX:
        {
            static T_DFU_DEVICE_INFO DeviceInfo;
            T_IMG_HEADER_FORMAT *p_header;
            uint32_t addr = get_header_addr_by_img_id(OTA);
            p_header = (T_IMG_HEADER_FORMAT *)addr;
            DeviceInfo.ic_type = DEFINED_IC_TYPE;
            DeviceInfo.ota_version = 0x1;
            DeviceInfo.secure_version = p_header->ctrl_header.secure_version;
            DFU_PRINT_INFO3("<==ota_attr_read_cb: ic_type=0x%x, ota_version=0x%x, OTA Header secure_version=0x%x",
                            DeviceInfo.ic_type, DeviceInfo.ota_version, DeviceInfo.secure_version);

            DeviceInfo.ota_mode.mode_flag.buf_check_en = DFU_BUFFER_CHECK_ENABLE;
            DeviceInfo.max_buffer_size = DFU_TEMP_BUFFER_SIZE;
            DeviceInfo.ota_mode.mode_flag.aesflg = OTP->ota_with_encryption_data;
            DeviceInfo.ota_mode.mode_flag.aesmode = 1;
            DFU_PRINT_INFO4("<==ota_attr_read_cb: bufChkEn=%d, bufsize=0x%x, aesflg=%d, aesmode=%d",
                            DeviceInfo.ota_mode.mode_flag.buf_check_en, DeviceInfo.max_buffer_size,
                            DeviceInfo.ota_mode.mode_flag.aesflg, DeviceInfo.ota_mode.mode_flag.aesmode);

            /* default disable copy app data img when bank switch */
            DeviceInfo.ota_mode.mode_flag.copy_img = 0;
            DeviceInfo.ota_mode.mode_flag.multi_img = is_ota_support_bank_switch() & 0x01;

            /* prepare img_indicator, if modify here need to sync with vaule
                of BLE_SERVICE_CHAR_IMAGE_VERSION_INDEX */
            if (is_ota_support_bank_switch())
            {
                if ((addr & 0xffffff) == OTA_BANK0_ADDR)
                {
                    DeviceInfo.img_indicator = ((IMAGE_LOCATION_BANK0 << (OTA_HEADER * 2))
                                                /*| (IMAGE_LOCATION_BANK0 << (SECURE_BOOT * 2))*/
                                                | (IMAGE_LOCATION_BANK0 << (ROM_PATCH * 2))
                                                | (IMAGE_LOCATION_BANK0 << (APP_IMG * 2))
                                               );
                }
                else
                {
                    DeviceInfo.img_indicator = ((IMAGE_LOCATION_BANK1 << (OTA_HEADER * 2))
                                                /*| (IMAGE_LOCATION_BANK1 << (SECURE_BOOT * 2))*/
                                                | (IMAGE_LOCATION_BANK1 << (ROM_PATCH * 2))
                                                | (IMAGE_LOCATION_BANK1 << (APP_IMG * 2))
                                               );
                }
            }
            else
            {
                DeviceInfo.img_indicator = ((IMAGE_FIX_BANK_EXIST << (OTA_HEADER * 2))
                                            | (IMAGE_FIX_BANK_EXIST << (SECURE_BOOT * 2))
                                            | (IMAGE_FIX_BANK_EXIST << (ROM_PATCH * 2))
                                            | (IMAGE_FIX_BANK_EXIST << (APP_IMG * 2))
                                           );
            }
            DFU_PRINT_INFO3("<==ota_attr_read_cb: copy_img=0x%x, multi_img=0x%x, img_indicator=0x%x",
                            DeviceInfo.ota_mode.mode_flag.copy_img, DeviceInfo.ota_mode.mode_flag.multi_img,
                            DeviceInfo.img_indicator);


            *pp_value  = (uint8_t *)&DeviceInfo;
            *p_length = sizeof(T_DFU_DEVICE_INFO);
        }
        break;
    case BLE_SERVICE_CHAR_IMAGE_VERSION_INDEX:
        {
            T_IMG_HEADER_FORMAT *p_imgheader;
            static uint32_t version[5] = {0};
            uint32_t image_addr;

            T_OTA_HEADER_FORMAT *p_otaheader;
            image_addr = get_header_addr_by_img_id(OTA);
            DFU_PRINT_INFO1("<==ota_attr_read_cb: OTA header addr=0x%x", image_addr);
            p_otaheader = (T_OTA_HEADER_FORMAT *)image_addr;
            if (p_otaheader)
            {
                version[0] = p_otaheader->ver_val;
            }

            if (is_ota_support_bank_switch())
            {
                image_addr = get_header_addr_by_img_id(RomPatch);
                p_imgheader = (T_IMG_HEADER_FORMAT *)image_addr;
                if (p_imgheader)
                {
                    version[1] = p_imgheader->git_ver.ver_info.version;
                }

                image_addr = get_header_addr_by_img_id(AppPatch);
                p_imgheader = (T_IMG_HEADER_FORMAT *)image_addr;
                if (p_imgheader)
                {
                    version[2] = p_imgheader->git_ver.ver_info.version;
                }
                DFU_PRINT_INFO3("<==ota_attr_read_cb: OTA Header Ver=0x%x, Patch Ver=0x%x, APP Ver=0x%x",
                                version[0], version[1], version[2]);

                *pp_value  = (uint8_t *)version;
                *p_length = 12;
            }
            else
            {
                image_addr = get_header_addr_by_img_id(SecureBoot);
                p_imgheader = (T_IMG_HEADER_FORMAT *)image_addr;
                if (p_imgheader)
                {
                    version[1] = p_imgheader->git_ver.ver_info.version;
                }

                image_addr = get_header_addr_by_img_id(RomPatch);
                p_imgheader = (T_IMG_HEADER_FORMAT *)image_addr;
                if (p_imgheader)
                {
                    version[2] = p_imgheader->git_ver.ver_info.version;
                }

                image_addr = get_header_addr_by_img_id(AppPatch);
                p_imgheader = (T_IMG_HEADER_FORMAT *)image_addr;
                if (p_imgheader)
                {
                    version[3] = p_imgheader->git_ver.ver_info.version;
                }
                DFU_PRINT_INFO4("<==ota_attr_read_cb: OTA Header Ver=0x%x, FSBL Ver=0x%x, Patch Ver=0x%x, APP Ver=0x%x",
                                version[0], version[1], version[2], version[3]);

                *pp_value  = (uint8_t *)version;
                *p_length = 16;
            }

            /* Notify Application. */
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE;
            callback_data.msg_data.read_value_index = OTA_READ_CHAR_IMAGE_VERSION_INDEX;
            /*defer processing before ota start*/
            defer_handle = true;
        }
        break;
#if (SUPPORT_OTA_PROTOCOL_TYPE_CHARACTERISTIC == 1)
    case BLE_SERVICE_CHAR_PROTOCOL_TYPE_INDEX:
        {
            protocol_type = 0x0012;
            *pp_value  = (uint8_t *)&protocol_type;
            *p_length = sizeof(protocol_type);
        }
        break;
#endif
    }

    /* Notify Application. */
    if (pfn_ota_service_cb && (defer_handle == true))
    {
        pfn_ota_service_cb(service_id, (void *)&callback_data);
    }
    return (wCause);
}


/**
 * @brief OTA ble Service Callbacks.
*/
const T_FUN_GATT_SERVICE_CBS ota_service_cbs =
{
    ota_attr_read_cb,       // Read callback function pointer
    ota_attr_write_cb,      // Write callback function pointer
    NULL                    // CCCD update callback function pointer
};

/**
 * @brief  add OTA ble service to application.
 *
 * @param  p_func          pointer of app callback function called by profile.
 * @return service ID auto generated by profile layer.
 * @retval ServiceId
*/
uint8_t ota_add_service(void *p_func)
{
    uint8_t service_id;
    if (false == server_add_service(&service_id,
                                    (uint8_t *)gatt_Ota_service_table,
                                    sizeof(gatt_Ota_service_table),
                                    ota_service_cbs))
    {
        DFU_PRINT_ERROR1("<==ota_add_service: service_id=%d", service_id);
        service_id = 0xff;
        return service_id;
    }
    pfn_ota_service_cb = (P_FUN_SERVER_GENERAL_CB)p_func;
    return service_id;
}
