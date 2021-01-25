/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     simple_ble_service.h
  * @brief    Demonstration of how to implement a self-definition service.
  * @details  Demonstration of different kinds of service interfaces.
  * @author
  * @date
  * @version
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _AIS_SERVICE_H_
#define _AIS_SERVICE_H_

#ifdef __cplusplus
extern "C"  {
#endif      /* __cplusplus */

/* Add Includes here */
#include <profile_server.h>
#include <ais_config.h>


#define AIS_SERVICE_CHAR_RC_INDEX           0x02
#define AIS_SERVICE_CHAR_WC_INDEX           0x04
#define AIS_SERVICE_CHAR_IC_INDEX           0x06
#define AIS_SERVICE_CHAR_WWNRC_INDEX        0x09
#define AIS_SERVICE_CHAR_NC_INDEX           0x0B
#define AIS_SERVICE_CHAR_IC_CCCD_INDEX      (AIS_SERVICE_CHAR_IC_INDEX + 1)
#define AIS_SERVICE_CHAR_NC_CCCD_INDEX      (AIS_SERVICE_CHAR_NC_INDEX + 1)


#define BREEZE_AIS_MESSAGE_ENABLE_NC           0x00
#define BREEZE_AIS_MESSAGE_DISABLE_NC           0x01
#define BREEZE_AIS_MESSAGE_ENABLE_IC           0x02
#define BREEZE_AIS_MESSAGE_DISABLE_IC           0x03

/** @defgroup SIMP_Service Simple Ble Service
  * @brief Simple BLE service
  * @{
  */
/*============================================================================*
 *                              Macros
 *============================================================================*/
/** @defgroup SIMP_Service_Exported_Macros SIMP Service Exported Macros
  * @brief
  * @{
  */

/** @defgroup SIMP_Service_Application_Parameters SIMP Service Application Parameters
  * @brief  Type of parameters set/got from application.
  * @{
  */
typedef enum
{
    AIS_SERVICE_PARAM_V1_READ_CHAR_VAL = 0x01,
} T_SIMP_PARAM_TYPE;

/** @} */



/** @defgroup SIMP_Service_Upstream_Message SIMP Service Upstream Message
  * @brief  Upstream message used to inform application.
  * @{
  */

/** @defgroup SIMP_Service_Read_Info SIMP Service Read Info
  * @brief  Parameter for reading characteristic value.
  * @{
  */
#define SIMP_READ_V1                                        1
/** @} */

/** @defgroup SIMP_Service_Write_Info SIMP Service Write Info
  * @brief  Parameter for writing characteristic value.
  * @{
  */
#define SIMP_WRITE_V2                                       1

/** @} */

/** @defgroup SIMP_Service_Notify_Indicate_Info SIMP Service Notify Indicate Info
  * @brief  Parameter for enable or disable notification or indication.
  * @{
  */
#define SIMP_NOTIFY_INDICATE_V3_ENABLE     1
#define SIMP_NOTIFY_INDICATE_V3_DISABLE    2
#define SIMP_NOTIFY_INDICATE_V4_ENABLE     3
#define SIMP_NOTIFY_INDICATE_V4_DISABLE    4


/** @} */

#define SIMP_READ_V1_MAX_LEN               300

/** @} End of SIMP_Service_Upstream_Message */



/** @} End of SIMP_Service_Exported_Macros */
/*============================================================================*
 *                              Types
 *============================================================================*/
/** @defgroup SIMP_Service_Exported_Types SIMP Service Exported Types
  * @brief
  * @{
  */

/** @defgroup TSIMP_WRITE_MSG TSIMP_WRITE_MSG
  * @brief Simple BLE service written msg to application.
  * @{
  */
typedef struct
{
    uint8_t opcode; //!< ref:  @ref SIMP_Service_Write_Info
    T_WRITE_TYPE write_type;
    uint16_t len;
    uint8_t *p_value;
} TSIMP_WRITE_MSG;
/** @} End of TSIMP_WRITE_MSG */


/** @defgroup TSIMP_UPSTREAM_MSG_DATA TSIMP_UPSTREAM_MSG_DATA
  * @brief Simple BLE service callback message content.
  * @{
  */
typedef union
{
    uint8_t notification_indification_index; //!< ref: @ref SIMP_Service_Notify_Indicate_Info
    uint8_t read_value_index; //!< ref: @ref SIMP_Service_Read_Info
    TSIMP_WRITE_MSG write;
} TSIMP_UPSTREAM_MSG_DATA;
/** @} End of TSIMP_UPSTREAM_MSG_DATA */

/** @defgroup TSIMP_CALLBACK_DATA TSIMP_CALLBACK_DATA
  * @brief Simple BLE service data to inform application.
  * @{
  */
typedef struct
{
    uint8_t                 conn_id;
    T_SERVICE_CALLBACK_TYPE msg_type;
    TSIMP_UPSTREAM_MSG_DATA msg_data;
} TSIMP_CALLBACK_DATA;
/** @} End of TSIMP_CALLBACK_DATA */

/** @} End of SIMP_Service_Exported_Types */

typedef struct _breeze_hal_ble_cb_def
{
    void (*disconnect_func)(uint8_t reason);
    void (*connect_func)(uint8_t err);
    void (*nc_cfg_changed_func)(uint16_t value);
    void (*ic_cfg_changed_func)(uint16_t value);
    size_t (*read_ais_rc_func)(void *buf, uint16_t len, uint16_t offset);
    size_t (*read_ais_wc_func)(void *buf, uint16_t len, uint16_t offset);
    size_t (*write_ais_wc_func)(const void *buf, uint16_t len, uint16_t offset, uint8_t flag);
    size_t (*read_ais_ic_func)(void *buf, uint16_t len, uint16_t offset);
    size_t (*read_ais_wwnrc_func)(void *buf, uint16_t len, uint16_t offset);
    size_t (*write_ais_wwnrc_func)(const void *buf, uint16_t len, uint16_t offset, uint8_t flag);
    size_t (*read_ais_nc_func)(void *buf, uint16_t len, uint16_t offset);
    void (*indicate_cb_func)(uint8_t err);
} breeze_hal_ble_cb_def;

extern breeze_hal_ble_cb_def *breeze_cb;
extern T_SERVER_ID ais_id;

/*============================================================================*
 *                              Functions
 *============================================================================*/
/** @defgroup SIMP_Service_Exported_Functions SIMP Service Exported Functions
  * @brief
  * @{
  */

/**
  * @brief Add simple BLE service to the BLE stack database.
  *
  * @param[in]   p_func  Callback when service attribute was read, write or cccd update.
  * @return Service id generated by the BLE stack: @ref T_SERVER_ID.
  * @retval 0xFF Operation failure.
  * @retval others Service id assigned by stack.
  *
  */
T_SERVER_ID ais_add_service(void *p_func);

/**
  * @brief  Set service related data from application.
  *
  * @param[in] param_type            parameter type to set.
  * @param[in] len                   value length to be set.
  * @param[in] p_value             value to set.
  * @return parameter set result.
  * @retval 0 false
  * @retval 1 true
  */
bool simp_ble_service_set_parameter(T_SIMP_PARAM_TYPE param_type, uint16_t len, void *p_value);

/**
  * @brief send notification of simple notify characteristic value.
  *
  * @param[in] conn_id           connection id
  * @param[in] service_id        service ID of service.
  * @param[in] p_value           characteristic value to notify
  * @param[in] length            characteristic value length to notify
  * @return notification action result
  * @retval 1 true
  * @retval 0 false
  */
bool ais_send_notify(uint8_t conn_id, T_SERVER_ID service_id,
                     void *p_value, uint16_t length);
/**
  * @brief send indication of simple indicate characteristic value.
  *
  * @param[in] conn_id           connection id
  * @param[in] service_id        service ID of service.
  * @param[in] p_value           characteristic value to notify
  * @param[in] length            characteristic value length to notify
  * @return notification action result
  * @retval 1 true
  * @retval 0 false
  */
bool ais_send_indicate(uint8_t conn_id, T_SERVER_ID service_id,
                       void *p_value, uint16_t length);

void breeze_hal_ble_cb_register(breeze_hal_ble_cb_def *cb);
void breeze_message_handle(uint32_t param);
/** @} End of SIMP_Service_Exported_Functions */


/** @} End of SIMP_Service */


#ifdef __cplusplus
}
#endif

#endif /* _AIS_SERVICE_H_ */
