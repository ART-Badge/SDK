/**
****************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
****************************************************************************************************
* @file      flash_adv_cfg.h
* @brief
* @note      flash advanced functions
* @author    Grace
* @date      2018-04-19
* @version   v0.1
* **************************************************************************************************
*/

#ifndef _FLASH_ADV_CFG_H_
#define _FLASH_ADV_CFG_H_

#include <stdint.h>
#include <stdbool.h>
#include "flash_device.h"
#include "rtl876x_lib_platform.h"

#ifdef  __cplusplus
extern  "C" {
#endif  // __cplusplus

/** @defgroup  FLASH_DEVICE    Flash Device
    * @{
    */

/*============================================================================*
  *                                   Types
  *============================================================================*/
/** @defgroup FLASH_DEVICE_Exported_Types Flash Device Exported Types
    * @brief
    * @{
    */

/** End of FLASH_DEVICE_Exported_Types
  * @}
  */
/*************************************************************************************************
  *                                   Functions
*************************************************************************************************/
/** @defgroup FLASH_DEVICE_Exported_Functions Flash Device Exported Functions
    * @brief
    * @{
    */

/**
* @brief    get block protect level
* @param    *bp_lv  a set of BPx ~ BP0
* @return   success or not
*/
bool flash_get_block_protect_locked(uint8_t *bp_lv);

/**
* @brief    set block protect by map
* @param    bp_lv a set of BPx ~ BP0
* @return   success or not
*/
bool flash_set_block_protect_locked(uint8_t bp_lv);

/**
* @brief    only unlock prefer section by addres
* @param    unlock_addr address section to be unlocked
* @param    *old_bp_lv before unlock
* @return   success or not
*/
bool flash_sw_protect_unlock_by_addr_locked(uint32_t unlock_addr, uint8_t *old_bp_lv);

/**
* @brief    read vendor_id to switch callback
* @return   true if success
*           false if not support
*/
bool flash_otp_init(void);

/**
* @brief    safe version of flash_otp_erase
* @param    type    erase type
* @param    addr    address to erase when erase block or sector
* @return   true if success
*/
bool flash_otp_erase_locked(T_ERASE_TYPE type, uint32_t addr);

/**
* @brief    safe version of flash_otp_write
* @param    start_addr    start_addr address where is going to be flash in flash
* @param    data_len      data length to be program
* @param    data          data buffer to be program into
* @return   true if success
*/
bool flash_otp_write_locked(uint32_t start_addr, uint32_t data_len, uint8_t *data);

/**
* @brief    safe version of flash_otp_read
* @param    start_addr    start_addr address where is going to be read in flash
* @param    data_len      data length to be read
* @param    data          data buffer to be read into
* @return   true if success
*/
bool flash_otp_read_locked(uint32_t start_addr, uint32_t data_len, uint8_t *data);

/**
* @brief    safe version of flash_otp_enter
* @return   true if success
*/
bool flash_otp_enter_locked(void);

/**
* @brief    safe version of flash_otp_exit
* @return   true if success
*/
bool flash_otp_exit_locked(void);

/**
* @brief    safe version of flash_otp_set_lb
* @return   true if success
*/
bool flash_otp_set_lb_locked(void);

/**
* @brief    safe version of flash_otp_get_lb
* @return   true if success
*/
bool flash_otp_get_lb_locked(void);

/**
 * @brief read SFDP data
 * @param start_addr    start_addr address in SFDP table
 * @param data_len      data length to be read
 * @param data          data buffer to be read into
 * @return true if success
*/
bool flash_sfdp_read_locked(uint32_t start_addr, uint32_t data_len, uint8_t *data);

/**
 * @brief The quick user mode read (use split read to improve read speed)
 * @param start_addr    start_addr address where is going to be read in flash (4 byte aligned)
 * @param data_len      data length to be read
 * @param data          data buffer to be read into
 * @return true if success
*/
bool flash_read_quick_locked(uint32_t start_addr, uint32_t data_len, uint8_t *data);

/**
 * @brief  read 8 bytes UUID for WINBOND flash
 * @param  unique_id    UUID
 * @return true if getting UUID successfully
 * @note   WINBOND flash needs calibration under different address mode or system clock.
 *         First 4 bytes of UUID are the same for same flash.
*/
bool flash_read_unique_id_winbond(uint8_t *unique_id);

/**
 * @brief  read 16 bytes UUID for GD flash
 * @param  unique_id    UUID
 * @return true if getting UUID successfully
 * @note   Last 8 bytes of GD flash UUID are all 0xFF. First 4 bytes of UUID are the same for same flash.
*/
bool flash_read_unique_id_gd(uint8_t *unique_id);

/**
 * @brief  read flash ID
 * @param  void
 * @return flash id
*/
uint32_t flash_get_id(void);

/**
 * @brief allow customers to read information from specific eFuse region
 * @param data read out data
 * @param length data length to be read, 8 bytes max
 * @ret   true if success
*/
bool app_read_eFuse(uint8_t *data, uint8_t length);

/**
 * @brief allow customers to write information to specific eFuse region
 * @param data input data
 * @param length data length to be read, 8 bytes max
 * @ret   true if success
*/
bool app_write_eFuse(uint8_t *data, uint8_t length);


/** @} */ /* End of group FLASH_DEVICE_Exported_Functions */


/** @} */ /* End of group FLASH_DEVICE */

#ifdef  __cplusplus
}
#endif // __cplusplus

#endif // _FLASH_ADV_CFG_H_
