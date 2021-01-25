/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    simple_ble_config.h
  * @brief   This file includes common constants or types for Simple BLE service/client.
  *          And some optional feature may be defined in this file.
  * @details
  * @author  Ethan
  * @date    2016-02-18
  * @version v0.1
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion **/
#ifndef _AIS_BLE_CONFIG_H_
#define _AIS_BLE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif


/** @defgroup SIMP_Service Simple Ble Service
  * @brief Simple BLE service
  * @{
  */

/** @defgroup SIMP_Service_CONFIG SIMP Service Config
  * @brief Simple BLE service configuration file
  * @{
  */
/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup SIMP_Common_Exported_Macros SIMP Service Config Exported Constants
  * @brief
  * @{
  */

/** @defgroup SIMP_UUIDs SIMP UUIDs
  * @brief Simple BLE Profile UUID definitions
  * @{
  */
#define GATT_UUID_AIS_PROFILE                    0xFEB3
#define GATT_UUID_CHAR_AIS_RC               0xFED4
#define GATT_UUID_CHAR_AIS_WC               0xFED5
#define GATT_UUID_CHAR_AIS_IC               0xFED6
#define GATT_UUID_CHAR_AIS_WWNRC            0xFED7
#define GATT_UUID_CHAR_AIS_NC               0xFED8
/** @} End of SIMP_UUIDs */

/** @} End of SIMP_Common_Exported_Macros */

/** @} End of SIMP_Service_CONFIG */

/** @} End of SIMP_Service */


#ifdef __cplusplus
}
#endif

#endif
