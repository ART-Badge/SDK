/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     ota_service.h
* @brief
* @details
* @author
* @date
* @version  v1.0.0
******************************************************************************
* @attention
* <h2><center>&copy; COPYRIGHT 2015 Realtek Semiconductor Corporation</center></h2>
******************************************************************************
*/

#ifndef _OTA_SERVICE_H_
#define _OTA_SERVICE_H_


#ifdef __cplusplus
extern "C"  {
#endif      /* __cplusplus */

#include "profile_server.h"
/** @brief  Demo Profile service related UUIDs. */

#define GATT_UUID_CHAR_OTA                          0xFFD1               //1
#define GATT_UUID_CHAR_MAC                          0xFFD2               //2
#define GATT_UUID_CHAR_PATCH                        0xFFD3               //3
#define GATT_UUID_CHAR_APP_VERSION                  0xFFD4               //4
#define GATT_UUID_CHAR_PATCH_EXTENSION              0xFFD5               //5 
#define GATT_UUID_CHAR_TEST_MODE                    0xFFD8               //6

#define GATT_UUID_CHAR_DEVICE_INFO                  0xFFF1               //7
#define GATT_UUID_CHAR_IMAGE_COUNT_TO_UPDATE        0xFFF2               //8  
#define GATT_UUID_CHAR_IMAGE_VERSION                0xFFE0


//vendor featture for some customers

#define EFLASH_SAVE_OTA_PROCESSING_OFFSET           0x208




/** @brief  Index of each characteristic in Demo Profile service database. */
#define BLE_SERVICE_CHAR_OTA_INDEX                      0x02
#define BLE_SERVICE_CHAR_MAC_ADDRESS_INDEX              0x04
#define BLE_SERVICE_CHAR_PATCH_INDEX                    0x06
#define BLE_SERVICE_CHAR_APP_VERSION_INDEX              0x08
#define BLE_SERVICE_CHAR_PATCH_EXTENSION_INDEX          0x0a
#define BLE_SERVICE_CHAR_TEST_MODE_INDEX                0x0c
#define BLE_SERVICE_CHAR_DEVICE_INFO_INDEX              0x0e
#define BLE_SERVICE_CHAR_IMAGE_COUNT_INDEX              0x10
#define BLE_SERVICE_CHAR_IMAGE_VERSION_INDEX            0x12

/** @brief OTA Write callback data type definition.*/
#define OTA_WRITE_CHAR_VAL                        0x01
#define OTA_WRITE_TEST_MODE_CHAR_VAL              0x02

/** @brief OTA Write callback data type definition.*/
#define OTA_WRITE_CHAR_VAL      0x01
#define OTA_VALUE_ENTER      0x01

#define OTA_WRITE_IMAGE_COUNT_VAL      0x02
#define OTA_VALUE_IMAGE_COUNT_APP_ONLY          0x01
#define OTA_VALUE_IMAGE_COUNT_PATCH_ONLY        0x02
#define OTA_VALUE_IMAGE_COUNT_APP_AND_PATCH     0x03

typedef struct _TOTA_UPDATE_IMAGE_INFO
{
    uint8_t image_count;
    uint16_t update_patch_version;
    uint16_t update_app_version;
} TOTA_UPDATE_IMAGE_INFO;
#define OTA_UPDATE_IMAGE_INFO_LEN 5

#define CHAR2SHORT(p) (((*(p)) & 0xff) + ((*((p)+1)) << 8))

typedef struct _TOTA_WRITE_MSG
{
    uint8_t opcode;
    union
    {
        uint8_t value;
        TOTA_UPDATE_IMAGE_INFO update_image_info;
    } u;
} TOTA_WRITE_MSG;

typedef union _TOTA_UPSTREAM_MSG_DATA
{
    uint8_t notification_indification_index;
    uint8_t read_value_index;
    TOTA_WRITE_MSG write;
} TOTA_UPSTREAM_MSG_DATA;

/**
 * @brief OTA servic data struct for notification data to application.
 *
 * OTA service data to inform application.
*/
typedef struct _TOTA_CALLBACK_DATA
{
    T_SERVICE_CALLBACK_TYPE     msg_type;                    /**<  @brief EventId defined upper */
    TOTA_UPSTREAM_MSG_DATA  msg_data;
} TOTA_CALLBACK_DATA;


extern uint8_t ota_add_service(void *pFunc);

typedef enum _BANK_NUM
{
    BANK0 = 0,
    BANK1 = 1,
    BANK_UNKNOWN = 2
} BANK_NUM;

typedef void (*APPEntryFunc)(void);
extern APPEntryFunc getAPPEntryFunc(BANK_NUM bank_num, uint16_t *pAppVersion);
extern BANK_NUM getAPPFreeBank(void);

#ifdef __cplusplus
}
#endif

#endif
