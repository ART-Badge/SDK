/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      gap_config.h
* @brief     Header file for configuration of bt stack related features
* @details   This file defines configuration of bt stack related features related API.
* @author    ranhui
* @date      2016-02-18
* @version   v1.0
* *********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef GAP_CONFIG_H
#define GAP_CONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include "gap_le_types.h"
#include "app_section.h"

/** @addtogroup GAP GAP Module
  * @{
  */

/** @addtogroup GAP_LE GAP LE Module
  * @{
  */

/** @addtogroup GAP_LE_CONFIG GAP LE Config Module
  * @{
  */


/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_LE_CONFIG_Exported_Types GAP LE Config Exported Types
  * @{
  */

/** @brief GAP gatt cccd not check. */
typedef enum
{
    CONFIG_GATT_CCCD_CHECK,         /**< Check cccd when server sends notification or indication. */
    CONFIG_GATT_CCCD_NOT_CHECK,     /**< Not check cccd when server sends notification or indication. */
} T_GAP_CONFIG_GATT_CCCD_NOT_CHECK;

/** @brief GAP set minimum remote SCA automatically in GAP initialization. */
typedef enum
{
    CONFIG_LE_MIN_REM_SCA = 1,      /**< Set minimum remote SCA automatically in GAP initialization. */
} T_GAP_CONFIG_LE_MIN_REM_SCA_INIT;

/** End of GAP_LE_CONFIG_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup GAP_LE_CONFIG_Exported_Functions GAP LE Config Exported Functions
  * @brief
  * @{
  */

/**
 * @brief   Configure advertising extensions parameter.
 *
 *          NOTE: This function is only used for project with gap_bt5.lib.
 *
 * <b>Example usage</b>
 * \code{.c}
    APP_FLASH_TEXT_SECTION void bt_stack_config_init(void)
    {
        gap_config_ae_parameter();
    }
 * \endcode
 */
APP_FLASH_TEXT_SECTION void gap_config_ae_parameter(void);

/**
 * @brief   Configure whether to check cccd when server sends notification or indication.
 *
 * @param[in]      cccd_not_check_flag   Whether to check cccd: @ref T_GAP_CONFIG_GATT_CCCD_NOT_CHECK
 *
 * <b>Example usage</b>
 * \code{.c}
    APP_FLASH_TEXT_SECTION void bt_stack_config_init(void)
    {
        gap_config_cccd_not_check(CONFIG_GATT_CCCD_NOT_CHECK);
    }
 * \endcode
 */
APP_FLASH_TEXT_SECTION void gap_config_cccd_not_check(T_GAP_CONFIG_GATT_CCCD_NOT_CHECK
                                                      cccd_not_check_flag);

/**
 * @brief   Configure whether to set minimum remote SCA automatically in GAP initialization.
 *
 * @param[in]      le_min_rem_sca_flag       whether to set minimum remote SCA automatically in GAP initialization, @ref T_GAP_CONFIG_LE_MIN_REM_SCA_INIT
 * @param[in]      le_min_rem_sca_encoding   SCA field encoding, @ref T_GAP_SCA_FIELD_ENCODING
                                             Note: The value is valid if le_min_rem_sca_flag is set to CONFIG_LE_MIN_REM_SCA, and will be replaced
                                                   if app calls @ref le_set_gap_param() with parameter @ref GAP_PARAM_SET_REM_MIN_SCA.
 *
 * <b>Example usage</b>
 * \code{.c}
    APP_FLASH_TEXT_SECTION void bt_stack_config_init(void)
    {
        gap_config_le_min_rem_sca(CONFIG_LE_MIN_REM_SCA, GAP_SCA_31_TO_50_PPM);
    }
 * \endcode
 */
APP_FLASH_TEXT_SECTION void gap_config_le_min_rem_sca(T_GAP_CONFIG_LE_MIN_REM_SCA_INIT
                                                      le_min_rem_sca_flag,
                                                      T_GAP_SCA_FIELD_ENCODING le_min_rem_sca_encoding);

/**
 * @brief   Configure bte pool size.
 *          NOTE: Default value of size is 4 * 1024.
 *
 * @param[in]      bte_pool_size   bte pool size in units of 1024, range: 4 ~ 10. NOTE: The range may be changed in future version.
 *
 * <b>Example usage</b>
 * \code{.c}
    APP_FLASH_TEXT_SECTION void bt_stack_config_init(void)
    {
        //bte pool size is 6 * 1024
        gap_config_bte_pool_size(6);
    }
 * \endcode
 */
APP_FLASH_TEXT_SECTION void gap_config_bte_pool_size(uint8_t bte_pool_size);

/**
 * @brief   Configure bt report buffer number for observer role or central role.
 *          NOTE: Default value is 16.
 *
 * @param[in]      bt_report_buf_num   bt report buffer number
 *
 * <b>Example usage</b>
 * \code{.c}
    APP_FLASH_TEXT_SECTION void bt_stack_config_init(void)
    {
        gap_config_bt_report_buf_num(17);
    }
 * \endcode
 */
APP_FLASH_TEXT_SECTION void gap_config_bt_report_buf_num(uint8_t bt_report_buf_num);

/**
 * @brief   Configure maximum server CCCD number and maximum storage CCCD number.
 *
 * @param[in]      gatt_server_ccc_bits_count    maximum server CCCD number, default value is 16.
 * @param[in]      gatt_storage_ccc_bits_count   maximum storage CCCD number, default value is 16.
 *
 * <b>Example usage</b>
 * \code{.c}
    APP_FLASH_TEXT_SECTION void bt_stack_config_init(void)
    {
        gap_config_ccc_bits_count(18, 18);
    }
 * \endcode
 */
APP_FLASH_TEXT_SECTION void gap_config_ccc_bits_count(uint8_t gatt_server_ccc_bits_count,
                                                      uint8_t gatt_storage_ccc_bits_count);

/**
 * @brief   Configure maximum service table number for GATT server.
 *          NOTE: Default value is 12. The number of service table can be used by app is (gatt_max_attribute_table_count - 1).
 *
 * @param[in]      gatt_max_attribute_table_count    maximum service table number.
 *
 * <b>Example usage</b>
 * \code{.c}
    APP_FLASH_TEXT_SECTION void bt_stack_config_init(void)
    {
        gap_config_max_attribute_table_count(14);
    }
 * \endcode
 */
APP_FLASH_TEXT_SECTION void gap_config_max_attribute_table_count(uint8_t
                                                                 gatt_max_attribute_table_count);

/**
 * @brief   Configure maximum size of Maximum Transmission Unit.
 *          NOTE: Default value is 247.
 *
 * @param[in]      att_max_mtu_size    maximum size of Maximum Transmission Unit.
 *
 * <b>Example usage</b>
 * \code{.c}
    APP_FLASH_TEXT_SECTION void bt_stack_config_init(void)
    {
        gap_config_max_mtu_size(200);
    }
 * \endcode
 */
APP_FLASH_TEXT_SECTION void gap_config_max_mtu_size(uint16_t att_max_mtu_size);

/**
 * @brief   Configure key storage flag to determine keys that need to be saved.
 *
 *          NOTE: Storage bits are defined by group GAP_LE_STORAGE_BITS in gap_storage_le.h.
                  storage bit: 0-not save, 1-save.
                  Default value is 0xFF, which indicates all keys need to be saved.
 *
 * @param[in]      le_key_storage_flag   key storage flag to determine keys that need to be saved
 *
 * <b>Example usage</b>
 * \code{.c}
    APP_FLASH_TEXT_SECTION void bt_stack_config_init(void)
    {
        gap_config_le_key_storage_flag(LE_KEY_STORE_CCCD_DATA_BIT | LE_KEY_STORE_REMOTE_LTK_BIT | LE_KEY_STORE_REMOTE_BD_BIT);
    }
 * \endcode
 */
APP_FLASH_TEXT_SECTION void gap_config_le_key_storage_flag(uint16_t le_key_storage_flag);

/**
 * @brief   Configure LE maximum bonded device number.
 *
 *          NOTE: Default value is 1.
 *
 * @param[in]      max_le_paired_device   LE maximum bonded device number
 *
 * <b>Example usage</b>
 * \code{.c}
    APP_FLASH_TEXT_SECTION void bt_stack_config_init(void)
    {
        gap_config_max_le_paired_device(2);
    }
 * \endcode
 */
APP_FLASH_TEXT_SECTION void gap_config_max_le_paired_device(uint8_t max_le_paired_device);

/**
 * @brief   Print parameters configured by app.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        gap_config_print_config_param();
    }
 * \endcode
 */
void gap_config_print_config_param(void);

/** End of GAP_LE_CONFIG_Exported_Functions
  * @}
  */

/** End of GAP_LE_CONFIG
  * @}
  */

/** End of GAP_LE
  * @}
  */

/** End of GAP
  * @}
  */


#endif
#ifdef __cplusplus
}

#endif /* GAP_CONFIG_H */

