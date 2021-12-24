/**
****************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
****************************************************************************************************
* @file      ftl.h
* @brief     flash transport layer is used as abstraction layer for user application to save read/write
*            parameters in flash.
* @note      ftl is dedicate block in flash, only used for save read/write value, and offset here is
*            logical offset which is defined for whole ftl section.If value is only for one time read,
*            refer to fs_load_app_data_8 or other APIs in flash_device.h
* @author    Brenda_li
* @date      2016-12-27
* @version   v1.0
* **************************************************************************************************
*/

#ifndef _FTL_H_
#define _FTL_H_

#include <stdint.h>

#ifdef  __cplusplus
extern  "C" {
#endif  // __cplusplus

/** @defgroup  FTL    Flash Transport Layer
    * @brief simple implementation of file system for flash
    * @{
    */

/*============================================================================*
  *                                   Macros
  *============================================================================*/
/** @defgroup FTL_Exported_Macros Flash Transport Layer Exported Macros
    * @brief
    * @{
    */

/** @defgroup FTL_WRITE_ERROR_CODE FTL Write Error Code
 * @{
 */
#define FTL_WRITE_SUCCESS                   (0x00)
#define FTL_WRITE_ERROR_IN_INTR             (0x01)  /**< can't write ftl in a interrupt */
#define FTL_WRITE_ERROR_INVALID_ADDR        (0x02)  /**< logical addr is not 4byte align or exceed the max logical address */
#define FTL_WRITE_ERROR_OUT_OF_SPACE        (0x03)  /**< ftl have no space for store new data */
#define FTL_WRITE_ERROR_READ_BACK           (0x04)  /**< ftl write to flash error */
#define FTL_WRITE_ERROR_VERIFY              (0x05)
#define FTL_WRITE_ERROR_INVALID_SIZE        (0x06)  /**< write length is not 4byte align */
#define FTL_WRITE_ERROR_ERASE_FAIL          (0x07)  /**< flash erase fail result in ftl write fail */
#define FTL_WRITE_ERROR_NOT_INIT            (0x08)  /**< write ftl too early before ftl init */
#define FTL_WRITE_ERROR_NEED_GC             (0x09)  /**< ftl_only_gc_in_idle is opened */
/**
 * @}
 */

/** @defgroup FTL_READ_ERROR_CODE FTL Read Error Code
* @{
*/
#define FTL_READ_SUCCESS                    (0x00)
#define FTL_READ_ERROR_INVALID_LOGICAL_ADDR (0x01)  /**< logical addr is not 4byte align or exceed the max logical address */
#define FTL_READ_ERROR_READ_NOT_FOUND       (0x02)  /**< logical addr never be writed so that can't found */
#define FTL_READ_ERROR_PARSE_ERROR          (0x03)  /**< recorded logical addr is changed even though crc check pass*/
#define FTL_READ_ERROR_INVALID_SIZE         (0x04)  /**< read length is not 4byte align */
#define FTL_READ_ERROR_NOT_INIT             (0x05)  /**< read ftl too early before ftl init */
#define FTL_READ_ERROR_CRC_CHECK_FAIL       (0x06) /**< crc check fail */
/**
 * @}
 */

#define FTL_INIT_ERROR_ERASE_FAIL           (0x01)  /**< flash erase fail result in ftl init fail */
/** End of FTL_Exported_Macros
    * @}
    */
/*============================================================================*
  *                                   Types
  *============================================================================*/
/** @defgroup FTL_Exported_Types Flash Transport Layer Exported Types
    * @brief
    * @{
    */
typedef enum
{
    FTL_IOCTL_DEBUG = 0,                /**< IO code for ftl debug */
    FTL_IOCTL_CLEAR_ALL = 2,            /**< IO code for clear ftl section*/
    FTL_IOCTL_ERASE_INVALID_PAGE = 3,   /**< IO code to erase invalid page*/
    FTL_IOCTL_ENABLE_GC_IN_IDLE = 4,    /**< IO code to enable garbage collection in idle task*/
    FTL_IOCTL_DISABLE_GC_IN_IDLE = 5,   /**< IO code to disable garbage collection in idle task*/
    FTL_IOCTL_DO_GC_IN_APP = 6,         /**< IO code to do garbage collection in app*/
} T_FTL_IOCTL_CODE;

/** End of FTL_Exported_Types
    * @}
    */


/*============================================================================*
  *                                Functions
  *============================================================================*/
/** @defgroup FTL_Exported_Functions Flash Transport Layer Exported Functions
    * @brief
    * @{
    */
/**
    * @brief    Save specified value to specified ftl offset
    * @param    pdata  specify data buffer
    * @param    offset specify FTL offset to store
    *     @arg  Min: 0
    *     @arg  Max: depend on configuried ftl size
    * @param    size   size to store
    *     @arg  Min: 4
    *     @arg  Max: depend on configuried ftl size
    * @return   status
    * @retval   0  status successful
    * @retval   otherwise fail
    * @note     FTL offset is pre-defined and no conflict
    */
uint32_t ftl_save(void *pdata, uint16_t offset, uint16_t size);

/**
    * @brief    Load specified ftl offset parameter to specified buffer
    * @param    pdata  specify data buffer
    * @param    offset specify FTL offset to load
    *     @arg  Min: 0
    *     @arg  Max: depend on configuried ftl size
    * @param    size   size to load
    *     @arg  Min: 4
    *     @arg  Max: depend on configuried ftl size
    * @return   status
    * @retval   0  status successful
    * @retval   otherwise fail
    * @note     FTL offset is pre-defined and no conflict
    */
uint32_t ftl_load(void *pdata, uint16_t offset, uint16_t size);


/**
    * @brief    Control function entry for ftl
    * @param    cmd    command code for different operation
    * @param    p1     command parameter @ref T_FTL_IOCTL_CODE
    * @param    p2     extended command parameters
    * @return   results of control
    * @retval   0  status successful
    * @retval   otherwise fail
    */
uint32_t ftl_ioctl(uint32_t cmd, uint32_t p1, uint32_t p2);

/** @} */ /* End of group FTL_Exported_Functions */


/** @} */ /* End of group FTL */

#ifdef  __cplusplus
}
#endif // __cplusplus

#endif // _FTL_H_
