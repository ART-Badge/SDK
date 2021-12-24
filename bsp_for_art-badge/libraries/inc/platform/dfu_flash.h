#ifndef DFU_FLASH_H
#define DFU_FLASH_H

#include <stdint.h>
#include <stdbool.h>
#include "patch_header_check.h"
#include "dfu_api.h"

/** @defgroup DFU_API DFU API Sets
  * @brief API sets for device firmware update implementation
  * @{
  */
/*============================================================================*
  *                                   Types
 *============================================================================*/
/** @defgroup DFU_API_Exported_Types DFU API Sets Exported Types
    * @brief
    * @{
    */

typedef enum
{
    DFU_START = 0,
    DFU_DOING,
    DFU_END,
} T_DFU_STATUS;

typedef union
{
    T_IMG_CTRL_HEADER_FORMAT *p_header; //for DFU_START
    uint32_t dfu_length;                //for DFU_DOING
    bool image_check_result;            //for DFU_END
} T_DFU_DATA;

typedef void (*P_FUNC_DFU_STATUS_CB)(T_DFU_STATUS status, T_DFU_DATA data);

/**
  * @}
  */

/*============================================================================*
  *                                   Variables
 *============================================================================*/
/** @defgroup DFU_API_Exported_Functions DFU API Sets Exported Variables
    * @brief
    * @{
    */



/**
  * @}
  */

/*============================================================================*
  *                                Functions
  *============================================================================*/
/** @defgroup DFU_API_Exported_Functions DFU API Sets Exported Functions
    * @{
    */
/**
 * @brief check whether in OTA mode.
*/
bool dfu_check_ota_mode_flag(void);

/**
 * @brief set aon reg value means whether in OTA mode or not.
*/
void dfu_set_ota_mode_flag(bool enable);

/**
 * @brief switch to the OTA mode, if support normal ota app need call it.
*/
void dfu_switch_to_ota_mode(void);

/**
 * @brief OTA procedure do wdg system reset.
 * @param  is_active_fw true means dfu success! otherwise fail.
*/
void dfu_fw_reboot(bool is_active_fw);


/**
 * @brief OTA image default ecb mode aes decrypt, last less then 16bytes no encrypted.
*/
void dfu_hw_aes_decrypt_image(uint8_t *input, uint8_t *output, uint32_t length);

/**
 * @brief  report specified target ic type.
 * @param  image_id     image_id to identify FW.
 * @param  p_ic_type     To store ic type.
 * @return 0 if report ic type info successfully, error line number otherwise
*/
uint32_t dfu_report_target_ic_type(uint16_t image_id, uint8_t *p_ic_type);

/**
 * @brief  report specified FW info and current OTA offset.
 * @param  image_id          image_id to identify FW.
 * @param  p_origin_fw_version     To store current FW version.
 * @param  p_offset            To store current file offset.
 * @return 0 if report FW info successfully, error line number otherwise
*/
uint32_t dfu_report_target_fw_info(uint16_t image_id, uint32_t *p_origin_fw_version,
                                   uint32_t *p_offset);

/**
* @brief calculate checksum of lenth of buffer in flash.
* @param  image_id          image_id to identify FW.
* @param  offset             offset of the image.
* @param  length             length of data.
* @param  crc_val            ret crc value point.
* @return  0 if buffer checksum calcs successfully, error line number otherwise
*/
uint32_t dfu_check_buf_crc(uint8_t *buf, uint32_t length, uint16_t crc_val);


/**
 * @brief  unlock flash is need when erase or write flash.
*/
bool unlock_flash_bp_all(void);

/**
 * @brief  lock flash after erase or write flash.
*/
void lock_flash_bp(void);
/**
 * @brief erase a sector of the flash, will retry three times at most
 * @param  image_id          image_id to identify FW.
 * @param  offset             offset of the image.
 * @return  0 if erase successfully, error line number otherwise,
*/
uint32_t dfu_flash_erase_sector_with_retry(uint16_t image_id, uint32_t offset);


/**
 * @brief  write specified image data with specified length to flash
 * @param  image_id          image_id to identify FW.
 * @param  offset             offset of the image.
 * @param  length             length of data.
 * @param  p_void             pointer to data.
 * @return 0 if write FW image successfully, error line number otherwise
*/
uint32_t dfu_update(uint16_t image_id, uint32_t offset, uint32_t length,
                    uint32_t *p_void);


/**
 * @brief calculate checksum of the image and compare with given checksum value.
 * @param   image_id     image_id to identify image.
 * @return  true if the image integrity check passes, false otherwise
*/
bool dfu_check_checksum(uint16_t image_id);


/**
 * @brief   copy appdata from active bank to updating bank.
 * @param   image_id    image_id to identify image.
 * @param   dlAddress   address the img copy to.
 * @param   dlSize      copy size.
 * @return  true if the image copied success, false otherwise
*/
bool dfu_copy_img(uint16_t image_id, uint32_t dlAddress, uint32_t dlSize);
/**
  * @}
  */

/**
  * @}
  */




#endif //DFU_FLASH_H
