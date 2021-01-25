/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     voice_service.h
  * @brief    Head file for using Voice Service.
  * @details  Voice service data structs and external functions declaration.
  * @author   Chenjie Jin
  * @date     2019-6-27
  * @version  v1.1
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _VOICE_SERVICE_H_
#define _VOICE_SERVICE_H_

#ifdef __cplusplus
extern "C"  {
#endif      /* __cplusplus */


/* Add Includes here */
#include "profile_server.h"
#include "board.h"
#include "app_msg.h"

/** @defgroup Voice_Service Voice Service
  * @brief Voice Service based on GATT
  * @{
  */

/** @defgroup Voice_Service_Exported_Constants Voice Service Exported Constants
  * @brief macros that other .c files may use all defined here
  * @{
  */

///@cond
/** @brief  Voice_Service related UUIDs. */
#define GATT_UUID_VOICE_CHAR_CTL                     0x3A40
#define GATT_UUID_VOICE_CHAR_DATA                    0x3A41
#define GATT_UUID_VOICE_CHAR_CMD                     0x3A42

///@endcond

/** @defgroup Voice_Service_Attribute_Index Voice Service Attribute Index
  * @brief  Index defines for Characteristic's value in Voice_Service
  * @{
  */
#define GATT_SRV_VOICE_CHAR_CTL_VALUE_INDEX             2
#define GATT_SRV_VOICE_CHAR_DATA_VALUE_INDEX            4
#define GATT_SRV_VOICE_CHAR_DATA_CCCD_INDEX             5
#define GATT_SRV_VOICE_CHAR_CMD_VALUE_INDEX             7
#define GATT_SRV_VOICE_CHAR_CMD_CCCD_INDEX              8

/** @} */

/** @defgroup Voice_Service_Upstream_Message Voice Service Upstream Message
  * @brief  Upstream message used to inform application.
  * @{
  */

/** @defgroup Voice_Service_Read_Info Voice Service Read Info
  * @brief  Parameter for read characteristic value.
  * @{
  */
#define VOICE_SVC_READ_PARAM_VOICE_DATA         1
#define VOICE_SVC_READ_PARAM_VOICE_CMD          2
/** @} */

/** @defgroup Voice_Service_Write_Info Voice Service Write Info
  * @brief  Parameter for write characteristic value.
  * @{
  */
#define VOICE_SVC_WRITE_CHAR_CTL_INDEX   1
/** @} */

#define VOICE_WRITE_VOIC_CTL            1

#define VOICE_WRITE_VOICE_STOP          0x00
#define VOICE_WRITE_VOICE_START         0x01
#define VOICE_WRITE_EQ_CONFIG           0x02

#define VOICE_WRITE_VOICE_STOP_LEN      1
#define VOICE_WRITE_VOICE_START_LEN     1
#define VOICE_WRITE_EQ_CONFIG_LEN       3

/** @defgroup Voice_Service_Notify_Indicate_Info Voice Service Notify Indicate Info
  * @brief  Parameter for enable or disable notification or indication.
  * @{
  */
#define  VOICE_NOTIFY_DATA_ENABLE             1
#define  VOICE_NOTIFY_DATA_DISABLE            2
#define  VOICE_NOTIFY_CMD_ENABLE              3
#define  VOICE_NOTIFY_CMD_DISABLE             4
/** @} */

/** @} End of Voice_Service_Upstream_Message */

/** @} End of Voice_Service_Exported_Constants */

/** @defgroup Voice_Service_Exported_Types Voice Service Exported Types
  * @brief  types that other.c files may use all defined here
  * @{
  */

/** Message content */
typedef union
{
    struct
    {
        uint8_t len;
        uint8_t *report;
    } report_data;
} T_VOICE_WRITE_PARAMETER;

/** @struct T_VOICE_WRITE_MSG
  * @brief write message
  */
typedef struct
{
    uint8_t write_type;
    T_VOICE_WRITE_PARAMETER write_parameter;
} T_VOICE_WRITE_MSG;

/** @struct T_VOICE_UPSTREAM_MSG_DATA
  * @brief upstream message
  */
typedef struct
{
    uint8_t notification_indification_index;
    uint8_t read_value_index;
    T_VOICE_WRITE_MSG write;
} T_VOICE_UPSTREAM_MSG_DATA;

/** @struct T_VOICE_CALLBACK_DATA
  * @brief callback data
  */
typedef struct
{
    uint8_t                 conn_id;
    T_SERVICE_CALLBACK_TYPE     msg_type;
    T_VOICE_UPSTREAM_MSG_DATA    msg_data;
} T_VOICE_CALLBACK_DATA;


/** @} End of Voice_Service_Exported_Types */



/** @defgroup Voice_Service_Exported_Functions Voice Service Exported Functions
  * @brief functions that other .c files may use all defined here.
  * @{
  */
uint8_t voice_service_add_service(void *pfn);
/** @} End of Voice_Service_Exported_Functions */

/** @} End of Voice_Service */


#ifdef __cplusplus
}
#endif      /*  __cplusplus */

#endif /* _VOICE_SERVICE_H_ */
