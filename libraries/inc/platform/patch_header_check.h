/**
  ******************************************************************************
  * @file    patch_header_check.h
  * @author
  * @version V0.0.1
  * @date    2017-09-01
  * @brief   This file contains all the functions regarding patch header check.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef _PATCH_HEADER_CHECK_H_
#define _PATCH_HEADER_CHECK_H_
#include <stdbool.h>
#include <stdint.h>
#include "rtl876x.h"
#include "flash_device.h"
#include "app_define.h"
/** @addtogroup  FLASH_DEVICE    Flash Device
    * @{
    */

/*============================================================================*
  *                                   Constants
  *============================================================================*/
/** @defgroup FLASH_DEVICE_Exported_Constants Flash Device Exported Constants
  * @brief
  * @{
  */

#define SIGNATURE_APP_CB         0x0e85d101 /**< patch callback app signature definition*/
#define UUID_SIZE                16

#define DEFINED_IC_TYPE          0x09

#define DFU_HEADER_SIZE          12  /*currently, first 12 byte only will be treated as image header*/
#define IMG_HEADER_SIZE          1024
#define OTA_HEADER_IMAGE_SIZE    0x1000

/** End of FLASH_DEVICE_Exported_Constants
  * @}
  */



/*============================================================================*
  *                                   Types
  *============================================================================*/
/** @defgroup FLASH_DEVICE_Exported_Types Flash Device Exported Types
  * @brief
  * @{
  */

typedef enum
{
    SCCD        = 0x278D,
    OCCD        = 0x278E,
    FactoryCode = 0x278F,
    OTA         = 0x2790, /**< OTA header */
    SecureBoot  = 0x2791,
    RomPatch    = 0x2792,
    AppPatch    = 0x2793,
    AppData1    = 0x2794,
    AppData2    = 0x2795,
    AppData3    = 0x2796,
    AppData4    = 0x2797,
    AppData5    = 0x2798,
    AppData6    = 0x2799,
#ifdef SUPPORT_ALONE_UPPERSTACK_IMG
    UpperStack  = 0x279a,
    IMAGE_MAX   = 0x279b,
#else
    IMAGE_MAX   = 0x279a,
#endif
    IMAGE_USER_DATA = 0xFFFE,  /**<the image only support unsafe single bank ota*/
} T_IMG_ID;

typedef enum
{
    ENC_KEY_SCEK = 0,
    ENC_KEY_SCEK_WITH_RTKCONST,
    ENC_KEY_OCEK,
    ENC_KEY_OCEK_WITH_OEMCONST,
    ENC_KEY_ON_FLASH,
    ENC_KEY_MAX,
} T_ENC_KEY_SELECT;

typedef struct
{
    uint8_t N[256];
    uint8_t E[4];
} T_RSA_PUBLIC_KEY;

typedef enum _APP_IMAGE_TYPE
{
    IMAGE_NORMAL              = 0,
    IMAGE_COMPRESSED          = 1,
    //2-7 are reserved
} T_IMAGE_TYPE;

typedef struct
{
    uint8_t ic_type; //!< indicate different ic, bee2 is 5
    uint8_t secure_version; //!< secure version info
    union
    {
        uint16_t value;
        struct
        {
            uint16_t xip: 1; //!< payload is executed on flash
            uint16_t enc: 1; //!< all the payload is encrypted
            uint16_t load_when_boot: 1; //!< load image when boot
            uint16_t enc_load: 1; //!< encrypt load part or not
            uint16_t enc_key_select: 3; //!< referenced to ENC_KEY_SELECT
            uint16_t not_ready : 1; //!< for copy image in ota
            uint16_t not_obsolete : 1; //!< for copy image in ota
            uint16_t integrity_check_en_in_boot : 1; //!< enable image integrity check in boot flow
            uint16_t compressed_not_ready: 1;
            uint16_t compressed_not_obsolete: 1;
            uint16_t rsvd: 1;
            uint16_t image_type: 3; /*for app 000b: normal image, 001b:compressed image, other for more types
            for patch in temp bank consist of 001b: patch+fsbl, 010b: patch+app, 011b: patch+fsbl+app*/
        } flag_value;
    } ctrl_flag;
    uint16_t image_id; //!< indicate different image
    uint16_t crc16; //!<when enable image integrity check in boot flow, crc16 check image integrity if crc16 is not zero, otherwise SHA256 check.
    uint32_t payload_len; //!< image length in bytes exclude 1KB image header
} T_IMG_CTRL_HEADER_FORMAT;


typedef struct _COMPRESS_IMG_HEADER_FORMAT //96 bytes
{
    T_IMG_CTRL_HEADER_FORMAT ctrl_header; //12bytes
    uint8_t uuid[16];
    uint32_t version;
    uint32_t compress_algo;
    uint8_t sha256[32];
    uint8_t reverved[28];
} T_COMPRESS_IMG_HEADER_FORMAT;

typedef struct
{
    union
    {
        uint32_t version;
        struct
        {
            uint32_t _version_reserve: 8;   //!< reserved
            uint32_t _version_revision: 8; //!< revision version
            uint32_t _version_minor: 8;     //!< minor version
            uint32_t _version_major: 8;     //!< major version
        } header_sub_version; //!< ota header sub version
        struct
        {
            uint32_t _version_major: 4;     //!< major version
            uint32_t _version_minor: 8;     //!< minor version
            uint32_t _version_revision: 15; //!< revision version
            uint32_t _version_reserve: 5;   //!< reserved
        } img_sub_version; //!< other image sub version including patch, app, app data1-6

    } ver_info;
} T_IMAGE_VERSION;

typedef struct
{
    union
    {
        uint32_t version;
        struct
        {
            uint32_t _version_major: 4;     //!< major version
            uint32_t _version_minor: 8;     //!< minor version
            uint32_t _version_revision: 15; //!< revision version
            uint32_t _version_reserve: 5;   //!< reserved
        } sub_version;
    } ver_info;
    uint32_t _version_commitid;     //!< git commit id
    uint8_t _customer_name[8];      //!< branch name for customer patch
} T_VERSION_FORMAT;

typedef struct
{
    T_IMG_CTRL_HEADER_FORMAT ctrl_header;
    uint8_t uuid[16];
    uint32_t exe_base;
    uint32_t load_base;
    uint32_t load_len;
    uint32_t image_base;
    uint8_t rsvd0[4];
    uint32_t magic_pattern;
    uint8_t dec_key[16];
    uint8_t rsvd1[28];
    T_VERSION_FORMAT git_ver;
    T_RSA_PUBLIC_KEY rsaPubKey;
    uint8_t sha256[32];
    uint8_t rsvd2[68];
    uint32_t app_cb_signature;
    uint32_t app_cb_table_base_address;
} T_IMG_HEADER_FORMAT;

typedef struct
{
    uint8_t payload_signature[256];
    uint8_t payload_mac[16];
    uint8_t header_signature[256];
    uint8_t header_mac[16];

} T_AUTH_HEADER_FORMAT;

typedef struct
{
    T_IMG_CTRL_HEADER_FORMAT ctrl_header;
    uint8_t uuid[16];
    uint32_t exe_base;
    uint32_t load_base;
    uint8_t rsvd0[8];
    uint16_t auto_enter_dfu_mode_pattern;
    uint16_t single_bank_ota_pattern;
    uint32_t magic_pattern;
    uint8_t rsvd1[44];
    T_VERSION_FORMAT git_ver;  //!< offst: 0x60
    T_RSA_PUBLIC_KEY rsaPubKey;
    uint8_t sha256[32];
    uint32_t ver_val;
    uint32_t secure_boot_addr;
    uint32_t secure_boot_size;
    uint32_t rom_patch_addr;
    uint32_t rom_patch_size;
    uint32_t app_addr;
    uint32_t app_size;
    uint32_t app_data1_addr;
    uint32_t app_data1_size;
    uint32_t app_data2_addr;
    uint32_t app_data2_size;
    uint32_t app_data3_addr;
    uint32_t app_data3_size;
    uint32_t app_data4_addr;
    uint32_t app_data4_size;
    uint32_t app_data5_addr;
    uint32_t app_data5_size;
    uint32_t app_data6_addr;
    uint32_t app_data6_size;
    uint32_t upperstack_addr;  //!< offst:0x1E0
    uint32_t upperstack_size;
} T_OTA_HEADER_FORMAT;


/** End of FLASH_DEVICE_Exported_Types
  * @}
  */

/*************************************************************************************************
*                          functions
*************************************************************************************************/
/** @defgroup FLASH_DEVICE_Exported_Functions Flash Device Exported Functions
    * @brief
    * @{
    */
/**
 * @brief  get start address of active ota bank
 * @param  none
 * @return start address of active ota bank
*/
extern uint32_t get_active_ota_bank_addr(void);

/**
 * @brief  check configurated flash layout support bank switch
 * @param  none
 * @retval true support bank switch
 * @retval false otherwise
*/
extern bool is_ota_support_bank_switch(void);

/**
 * @brief  calculated checksum(CRC16 or SHA256 determined by image) over the image, and compared
 *         with given checksum value.
 * @param  p_header image header info of the given image.
 * @retval true if image integrity check pass via checksum compare
 * @retval false otherwise
*/
extern bool check_image_chksum(T_IMG_CTRL_HEADER_FORMAT *p_header);
/**
 * @brief  Check the validity of the specified image
 * @param  image_id specify the image
 * @param  header_addr specify image header address
 * @retval true if image check pass
 * @retval false otherwise
*/
extern bool check_header_valid(uint32_t header_addr, T_IMG_ID image_id);
/**
 * @brief  get specified image header address in active bank
 * @param  image_id specify the image
 * @return specified image header address
*/
extern uint32_t get_header_addr_by_img_id(T_IMG_ID image_id);

/**
 * @brief  get image size of specified image which located in ota active bank
 * @param  image_id specify the image which located in ota active bank
 * @return image size of specified image which located in ota active bank
*/
extern uint32_t get_active_bank_image_size_by_img_id(T_IMG_ID image_id);
/**
 * @brief  get start address of specified image which located in ota temp bank
 * @param  image_id specify the image
 * @return start address of specified image which located in ota temp bank
*/
extern uint32_t get_temp_ota_bank_addr_by_img_id(T_IMG_ID image_id);

/**
 * @brief  get size of specified image which located in ota temp bank
 * @param  image_id specify the image
 * @return size of specified image which located in ota temp bank
*/
extern uint32_t get_temp_ota_bank_size_by_img_id(T_IMG_ID image_id);

/**
 * @brief  get version info of specified image which located in active bank
 * @param  image_id specify the image
 * @return version info of specified image which located in active bank
*/
extern bool get_active_bank_image_version(T_IMG_ID image_id, T_IMAGE_VERSION *p_image_version);

#ifdef SUPPORT_ALONE_UPPERSTACK_IMG
static inline uint32_t get_temp_ota_bank_addr_by_img_id_app(T_IMG_ID image_id)
{
    uint32_t image_addr = 0;

    bool enable_old_ota = !is_ota_support_bank_switch();
    if (enable_old_ota)
    {
        if (image_id >= SecureBoot && image_id < IMAGE_MAX)
        {
            image_addr = flash_get_bank_addr(FLASH_OTA_TMP);
        }
        //others will return 0
    }
    else
    {
        uint32_t ota_bank0_addr = flash_get_bank_addr(FLASH_OTA_BANK_0);
        uint32_t temp_bank_addr;
        if (ota_bank0_addr == get_active_ota_bank_addr())
        {
            temp_bank_addr = flash_get_bank_addr(FLASH_OTA_BANK_1);
        }
        else
        {
            temp_bank_addr = ota_bank0_addr;
        }

        if (image_id == OTA)
        {
            image_addr = temp_bank_addr;
        }
        else if (image_id >= SecureBoot && image_id < IMAGE_MAX)
        {
            // auth ota temp bank and get address
            // image_authencation will fail after secure boot, so remove it
            if (!check_header_valid(temp_bank_addr, OTA))
            {
                return image_addr;
            }

            image_addr = HAL_READ32((uint32_t) & ((T_OTA_HEADER_FORMAT *)temp_bank_addr)->secure_boot_addr,
                                    (image_id - SecureBoot) * 8);
        }
        else //others will return 0
        {
        }
    }

    if (image_addr == 0)
    {
        return 0;
    }

    return (image_addr | FLASH_OFFSET_TO_NO_CACHE);
}


static inline uint32_t get_temp_ota_bank_size_by_img_id_app(T_IMG_ID image_id)
{
    uint32_t bank_size = 0;

    bool enable_old_ota = !is_ota_support_bank_switch();
    if (enable_old_ota)
    {
        if (image_id >= SecureBoot && image_id < IMAGE_MAX)
        {
            bank_size = flash_get_bank_size(FLASH_OTA_TMP);
        }
        //others will return 0
    }
    else
    {
        uint32_t ota_bank0_addr = flash_get_bank_addr(FLASH_OTA_BANK_0);
        uint32_t temp_bank_addr;
        if (ota_bank0_addr == get_active_ota_bank_addr())
        {
            temp_bank_addr = flash_get_bank_addr(FLASH_OTA_BANK_1);
        }
        else
        {
            temp_bank_addr = ota_bank0_addr;
        }

        if (image_id == OTA)
        {
            bank_size = OTA_HEADER_IMAGE_SIZE;
        }
        else if (image_id >= SecureBoot && image_id < IMAGE_MAX)
        {
            // auth ota temp bank and get address
            // image_authencation will fail after secure boot, so remove it
            if (!check_header_valid(temp_bank_addr, OTA))
            {
                bank_size = 0;
            }
            else
            {
                bank_size = HAL_READ32((uint32_t) & ((T_OTA_HEADER_FORMAT *)temp_bank_addr)->secure_boot_size,
                                       (image_id - SecureBoot) * 8);
            }
        }
        else //others will return 0
        {
        }
    }

    return bank_size;
}

#endif

/** @} */ /* End of group FLASH_DEVICE_Exported_Functions */
/** @} */ /* End of group FLASH_DEVICE */

#endif // _PATCH_HEADER_CHECK_H_
