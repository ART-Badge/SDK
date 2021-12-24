/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     atvv_service.h
  * @brief    Head file for using ATV voice service.
  * @details  ATVV data structs and external functions declaration.
  * @author   Chenjie Jin
  * @date     2017-12-8
  * @version  v0.1
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _ATVV_H_
#define _ATVV_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

/* Add Includes here */
#include "profile_server.h"

#if (VOICE_FLOW_SEL == ATV_GOOGLE_VOICE_FLOW)

/** @defgroup ATVV ATV Voice Service
  * @brief ATV voice service
  * @{
  */

/** @defgroup ATVV_Exported_Constants ATVV Exported Constants
  * @brief macros that other .c files may use all defined here
  * @{
  */

///@cond
#define ATVV_VERSION_1_0            0x0100
#define ATVV_VERSION_0_4            0x0004

#define ATTV_CODEC_MASK_8K_ADPCM    0x0001
#define ATTV_CODEC_MASK_16K_ADPCM   0x0002
#define ATTV_CODEC_MASK_OPUS        0x0004

#define ATVV_CHAR_RX_DATA_LEN 20
#define ATVV_CHAR_CTL_DATA_LEN 20
#define ATVV_CHAR_WRITE_DATA_LEN 20

/** @brief ATVV related services UUIDs */
#define GATT_UUID_ATV_CHAR_TX  0x64, 0xB6, 0x17, 0xF6, 0x01, 0xAF, 0x7D, 0xBC, 0x05, 0x4F, 0x21, 0x5A, 0x02, 0x00, 0x5E, 0xAB
#define GATT_UUID_ATV_CHAR_RX  0x64, 0xB6, 0x17, 0xF6, 0x01, 0xAF, 0x7D, 0xBC, 0x05, 0x4F, 0x21, 0x5A, 0x03, 0x00, 0x5E, 0xAB
#define GATT_UUID_ATV_CHAR_CTL 0x64, 0xB6, 0x17, 0xF6, 0x01, 0xAF, 0x7D, 0xBC, 0x05, 0x4F, 0x21, 0x5A, 0x04, 0x00, 0x5E, 0xAB

/** @brief Index defines for Characteristic's value */
#define GATT_SVC_ATVV_CHAR_TX_VALUE_INDEX   2
#define GATT_SVC_ATVV_CHAR_RX_VALUE_INDEX   4
#define GATT_SVC_ATVV_CHAR_RX_CCCD_INDEX    5
#define GATT_SVC_ATVV_CHAR_CTL_VALUE_INDEX  7
#define GATT_SVC_ATVV_CHAR_CTL_CCCD_INDEX   8
///@endcond

/** @} End of ATVV_Exported_Constants */

/** @defgroup ATVV_Exported_Types IAS Exported Types
  * @brief  types that other .c files may use all defined here
  * @{
  */
/* Add all public types here */
/** @defgroup ATVV_Callback_Data ATTB Callback Data
  * @brief ATVV data struct for notification data to application.
  * @{
  */

/** @defgroup ATVV_Read_Info ATVV Read Info
  * @brief  Parameter for read characteristic value.
  * @{
  */
#define ATVV_READ_CHAR_RX_INDEX   1
#define ATVV_READ_CHAR_CTL_INDEX  2
/** @} */

/** @defgroup ATVV_Write_Info ATVV Write Info
  * @brief  Parameter for write characteristic value.
  * @{
  */
#define ATVV_WRITE_CHAR_TX_INDEX   1
/** @} */

/** @defgroup ATVV_Notify_Indicate_Info ATVV Notify Indicate Info
  * @brief  Parameter for enable or disable notification or indication.
  * @{
  */
#define ATVV_CHAR_RX_NOTIFY_ENABLE   1
#define ATVV_CHAR_RX_NOTIFY_DISABLE  2
#define ATVV_CHAR_CTL_NOTIFY_ENABLE  3
#define ATVV_CHAR_CTL_NOTIFY_DISABLE 4

/** @defgroup ATT MTU zie and Date length extantion
  * @brief  GATT client negotiate ATT MTU and send DLE
  * @{
  */
#define ATVV_DLE_AND_MTU_UPDATE_DISABLE     0
#define ATVV_DLE_ENABLE_MTU_SIZE_UPDATE     1

/** Message content */
typedef union
{
    struct
    {
        uint8_t len;
        uint8_t *report;
    } report_data;
} T_ATVV_WRITE_PARAMETER;

/** @struct _TATVV_WRITE_MSG
  * @brief write message
  */
typedef struct
{
    uint8_t write_type; /**< ref: @ref ATVV_RmC_Write_Info */
    T_ATVV_WRITE_PARAMETER write_parameter;
} T_ATVV_WRITE_MSG;

typedef union _TATVV_UPSTREAM_MSG_DATA
{
    uint8_t notification_indification_index; /**< ref: @ref ATVV_RmC_Notify_Indicate_Info */
    uint8_t read_value_index; /**< ref: @ref ATVV_RmC_Read_Info */
    T_ATVV_WRITE_MSG write;
} T_ATVV_UPSTREAM_MSG_DATA;

/** ATVV service data to inform application */
typedef struct
{
    uint8_t conn_id;
    T_SERVICE_CALLBACK_TYPE msg_type;
    T_ATVV_UPSTREAM_MSG_DATA msg_data;
} T_ATVV_CALLBACK_DATA;
/** @} */

typedef enum
{
    ATV_TX_OPCODE_GET_CAPS = 0x0A,
    ATV_TX_OPCODE_MIC_OPEN = 0x0C,
    ATV_TX_OPCODE_MIC_CLOSE = 0x0D,
    ATV_TX_OPCODE_EXTEND = 0x0E,
} ATV_CHAR_TX_OPCODE;

#define ATV_TX_OPCODE_GET_CAPS_LEN 5
#define ATV_TX_OPCODE_MIC_OPEN_LEN 5

typedef enum
{
    ATV_CTL_OPCODE_AUDIO_STOP = 0x00,
    ATV_CTL_OPCODE_AUDIO_START = 0x04,
    ATV_CTL_OPCODE_DPAD_SELECT = 0x07,
    ATV_CTL_OPCODE_START_SEARCH = 0x08,
    ATV_CTL_OPCODE_AUDIO_SYNC = 0x0A,
    ATV_CTL_OPCODE_GET_CAPS_RESP = 0x0B,
    ATV_CTL_OPCODE_MIC_OPEN_ERROR = 0x0C,
} ATV_CHAR_CTL_CMD_OPCODE;

typedef enum
{
    ATV_START_REASON_MIC_OPEN_REQUEST = 0x00,
    ATV_START_REASON_PTT = 0x01,
    ATV_START_REASON_HTT = 0x03,
} ATV_AUDIO_START_REASON;

typedef enum
{
    ATV_STOP_REASON_MIC_CLOSE_MESSAGE = 0x00,
    ATV_STOP_REASON_HTT = 0x02,
    ATV_STOP_REASON_AUDIO_START_COMMAND = 0x04,
    ATV_STOP_REASON_AUDIO_TRANSFER_TIMEOUT = 0x08,
    ATV_STOP_REASON_ATVV_CHAR_AUDIO_DISABLE = 0x10,
    ATV_STOP_REASON_OTHERS = 0x80,
} ATV_AUDIO_STOP_REASON;

typedef enum
{
    ATV_MIC_OPEN_ERROR_RESERVED = 0x0f01,
    ATV_MIC_OPEN_ERROR_REMOTE_IS_NOT_ACTIVE = 0x0f02,
    ATV_MIC_OPEN_ERROR_ATVV_CHAR_AUDIO_IS_DISABLE = 0x0f03,
    ATV_MIC_OPEN_ERROR_PTT_HTT_IS_IN_PROGRESS = 0x0f80,
    ATV_MIC_OPEN_ERROR_INTERNAL_ERROR = 0x0fff,
} ATV_MIC_OPEN_ERROR;

typedef enum
{
    ATV_ASSISTANT_INTERACTION_MODEL_ON_REQUEST = 0x00,
    ATV_ASSISTANT_INTERACTION_MODEL_PRESS_TO_TALK = 0x01,
    ATV_ASSISTANT_INTERACTION_MODEL_HOLD_TO_TALK = 0x03,
} ATV_ASSISTANT_INTERACTION_MODEL;

/** @} End of IAS_Exported_Types */

typedef struct
{
    uint16_t app_support_version;
    uint16_t codec_used;
    uint8_t assistant_interaction_model;
    uint8_t audio_consumption_mode;
    uint8_t stream_id;
    uint8_t char_rx_data_buff[ATVV_CHAR_RX_DATA_LEN];
    uint8_t char_ctl_data_buff[ATVV_CHAR_RX_DATA_LEN];
    uint8_t char_write_dat_buff[ATVV_CHAR_WRITE_DATA_LEN];
} T_ATVV_GLOBAL_DATA;

extern T_ATVV_GLOBAL_DATA atvv_global_data;

/** @defgroup ATVV_Exported_Functions IAS Exported Functions
  * @brief functions that other .c files may use all defined here.
  * @{
  */
uint8_t atvv_add_service(void *p_func);
/** @} End of ATVV_Exported_Functions */

/** @} End of ATVV */

#endif  /* (VOICE_FLOW_SEL == ATV_GOOGLE_VOICE_FLOW) */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif //_IAS_H

