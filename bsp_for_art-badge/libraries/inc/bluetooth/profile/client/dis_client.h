/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     dis_client.h
  * @brief    Head file for using DIS Client.
  * @details  dis data structs and external functions declaration.
  * @author   ken
  * @date     2017-12-05
  * @version  v0.1
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _DIS_CLIENT_H_
#define _DIS_CLIENT_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

/* Add Includes here */
#include <profile_client.h>


/** @defgroup DIS_Client dis service client
  * @brief dis service client
  * @details
     dis Profile is a customized BLE-based Profile. Dis ble service please refer to @ref DIS_Service .
  * @{
  */
/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup DIS_Client_Exported_Macros DIS Client Exported Macros
  * @brief
  * @{
  */
/** @defgroup DIS UUIDs
  * @brief Dis BLE Profile UUID definitions
  * @{
  */
#define GATT_UUID_DEVICE_INFORMATION_SERVICE   0x180A

#define GATT_UUID_CHAR_SYSTEM_ID               0x2A23
#define GATT_UUID_CHAR_MODEL_NUMBER            0x2A24
#define GATT_UUID_CHAR_SERIAL_NUMBER           0x2A25
#define GATT_UUID_CHAR_FIRMWARE_REVISION       0x2A26
#define GATT_UUID_CHAR_HARDWARE_REVISION       0x2A27
#define GATT_UUID_CHAR_SOFTWARE_REVISION       0x2A28
#define GATT_UUID_CHAR_MANUFACTURER_NAME       0x2A29
#define GATT_UUID_CHAR_IEEE_CERTIF_DATA_LIST   0x2A2A
#define GATT_UUID_CHAR_PNP_ID                  0x2A50
/** @} End of DIS_UUIDs */

/** @brief  Define links number. range: 0-4 */
#define DIS_MAX_LINKS  2
/** End of DIS_Client_Exported_Macros
  * @}
  */


/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup DIS_Exported_Types DIS Client Exported Types
  * @brief
  * @{
  */

/** @brief DIS client handle type*/
typedef enum
{
    HDL_DIS_SRV_START,           //!< start handle of lls service
    HDL_DIS_SRV_END,             //!< end handle of lls service
    HDL_DIS_SYSTEM_ID,           //!< DIS system id read characteristic value handle
    HDL_DIS_MODEL_NUMBER,        //!< DIS model number read characteristic value handle
    HDL_DIS_SERIAL_NUMBER,       //!< DIS serial number read characteristic value handle
    HDL_DIS_FIRMWARE_REVISION,   //!< DIS firmware revision read characteristic value handle
    HDL_DIS_HARDWARE_REVISION,   //!< DIS hardware revision read characteristic value handle
    HDL_DIS_SOFTWARE_REVISION,   //!< DIS software revision read characteristic value handle
    HDL_DIS_MANUFACTURER_NAME,   //!< DIS manufacturer name read characteristic value handle
    HDL_DIS_IEEE_CERTIF_DATA_LIST,//!< DIS ieee certif data list read characteristic value handle
    HDL_DIS_PNP_ID,              //!< DIS pnp id read characteristic value handle
    HDL_DIS_CACHE_LEN            //!< handle cache length
} T_DIS_HANDLE_TYPE;

/** @brief DIS client discovery state*/
typedef enum
{
    DISC_DIS_IDLE,
    DISC_DIS_START,
    DISC_DIS_DONE,
    DISC_DIS_FAILED
} T_DIS_DISC_STATE;

/** @brief dis client read type*/
typedef enum
{
    DIS_READ_SYSTEM_ID,
    DIS_READ_MODEL_NUMBER,
    DIS_READ_SERIAL_NUMBER,
    DIS_READ_FIRMWARE_REVISION,
    DIS_READ_HARDWARE_REVISION,
    DIS_READ_SOFTWARE_REVISION,
    DIS_READ_MANUFACTURER_NAME,
    DIS_READ_IEEE_CERTIF_DATA_LIST,
    DIS_READ_PNP_ID
} T_DIS_READ_TYPE;

/** @brief DIS client read value*/
typedef struct
{
    uint16_t value_size;
    uint8_t *p_value;
} T_DIS_READ_VALUE;

/** @brief DIS client read data*/
typedef union
{
    T_DIS_READ_VALUE v1_read;
//    bool v3_notify_cccd;
//    bool v4_indicate_cccd;
} T_DIS_READ_DATA;

/** @brief DIS client read result*/
typedef struct
{
    T_DIS_READ_TYPE type;
    T_DIS_READ_DATA data;
    uint16_t cause;
} T_DIS_READ_RESULT;


///** @brief LLS client write type*/
//typedef enum
//{
//    LLS_WRITE_PARA,
//} T_LLS_WRTIE_TYPE;

///** @brief LLS client write result*/
//typedef struct
//{
//    T_LLS_WRTIE_TYPE type;
//    uint16_t cause;
//} T_LLS_WRITE_RESULT;

/** @brief LLS client notif/ind receive type*/
//typedef enum
//{
//    LLS_KEY_NOTIFY,
//} T_LLS_NOTIF_IND_TYPE;

///** @brief LLS client notif/ind receive data*/
//typedef struct
//{
//    uint16_t value_size;
//    uint8_t *p_value;
//} T_LLS_NOTIF_IND_VALUE;

/** @brief LLS client notif/ind receive content*/
//typedef struct
//{
//    T_LLS_NOTIF_IND_TYPE type;
//    T_LLS_NOTIF_IND_VALUE data;
//} T_LLS_NOTIF_IND_DATA;

/** @brief DIS client callback type*/
typedef enum
{
    DIS_CLIENT_CB_TYPE_DISC_STATE,          //!< Discovery procedure state, done or pending.
    DIS_CLIENT_CB_TYPE_READ_RESULT,         //!< Read request's result data, responsed from server.
    //LLS_CLIENT_CB_TYPE_WRITE_RESULT,        //!< Write request result, success or fail.
    //LLS_CLIENT_CB_TYPE_NOTIF_IND_RESULT,    //!< Notification or indication data received from server.
    DIS_CLIENT_CB_TYPE_INVALID              //!< Invalid callback type, no practical usage.
} T_DIS_CLIENT_CB_TYPE;

/** @brief DIS client callback content*/
typedef union
{
    T_DIS_DISC_STATE      disc_state;
    T_DIS_READ_RESULT     read_result;
    //T_DIS_WRITE_RESULT    write_result;
    //T_LLS_NOTIF_IND_DATA  notif_ind_data;
} T_DIS_CLIENT_CB_CONTENT;

/** @brief DIS client callback data*/
typedef struct
{
    T_DIS_CLIENT_CB_TYPE     cb_type;
    T_DIS_CLIENT_CB_CONTENT  cb_content;
} T_DIS_CLIENT_CB_DATA;

/** End of DIS_Client_Exported_Types * @} */

/** @defgroup DIS_Client_Exported_Functions DIS Client Exported Functions
  * @{
  */

/**
  * @brief      Add dis service client to application.
  * @param[in]  app_cb pointer of app callback function to handle specific client module data.
  * @param[in]  link_num initialize link num.
  * @return Client ID of the specific client module.
  * @retval 0xff failed.
  * @retval other success.
  *
  * <b>Example usage</b>
  * \code{.c}
    void app_le_profile_init(void)
    {
        client_init(1);
        dis_client_id = dis_add_client(app_client_callback, APP_MAX_LINKS);
    }
  * \endcode
  */
T_CLIENT_ID dis_add_client(P_FUN_GENERAL_APP_CB app_cb, uint8_t link_num);

/**
  * @brief  Used by application, to start the discovery procedure of dis server.
  * @param[in]  conn_id connection ID.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
bool dis_client_start_discovery(uint8_t conn_id);

/**
  * @brief  Used by application, to read data from server by using handles.
  * @param[in]  conn_id connection ID.
  * @param[in]  read_type one of characteristic that has the readable property.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
bool dis_client_read_by_handle(uint8_t conn_id, T_DIS_READ_TYPE read_type);

/**
  * @brief  Used by application, to read data from server by using UUIDs.
  * @param[in]  conn_id connection ID.
  * @param[in]  read_type one of characteristic that has the readable property.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
bool dis_client_read_by_uuid(uint8_t conn_id, T_DIS_READ_TYPE read_type);

/**
  * @brief  Used by application, to write data of write Characteristic.
  * @param[in]  conn_id connection ID.
  * @param[in]  length  write data length
  * @param[in]  p_value point the value to write
  * @param[in]  type    write type.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
//bool dis_client_write_char(uint8_t conn_id, uint16_t length, uint8_t *p_value,
//                                   T_GATT_WRITE_TYPE type);

/**
  * @brief  Used by application, to enable or disable the notification of peer server's V3 Notify Characteristic.
  * @param[in]  conn_id connection ID.
  * @param[in]  notify 0--disable the notification, 1--enable the notification.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
//bool lls_client_set_v3_notify(uint8_t conn_id, bool notify);

/**
  * @brief  Used by application, to enable or disable the indication of peer server's V4 Indicate Characteristic.
  * @param[in]  conn_id connection ID.
  * @param[in]  ind 0--disable the indication, 1--enable the indication.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
//bool lls_client_set_v4_ind(uint8_t conn_id, bool ind);

/**
  * @brief  Used by application, to get handle cache.
  * @param[in]  conn_id connection ID.
  * @param[in]  p_hdl_cache pointer of the handle cache table
  * @param[in]  len the length of handle cache table
  * @retval true success.
  * @retval false failed.
  */
bool dis_client_get_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len);

/**
  * @brief  Used by application, to set handle cache.
  * @param[in]  conn_id connection ID.
  * @param[in]  p_hdl_cache pointer of the handle cache table
  * @param[in]  len the length of handle cache table
  * @retval true success.
  * @retval false failed.
  */
bool dis_client_set_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len);

/** @} End of DIS_Client_Exported_Functions */

/** @} End of DIS_Client */


#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif  /* _DIS_CLIENT_H_ */
