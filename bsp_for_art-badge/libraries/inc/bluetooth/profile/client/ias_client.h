/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     ias_client.h
  * @brief    Head file for using Ias BLE Client.
  * @details  Ias data structs and external functions declaration.
  * @author   ken
  * @date     2017-12-04
  * @version  v0.1
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _IAS_CLIENT_H_
#define _IAS_CLIENT_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

/* Add Includes here */
#include <profile_client.h>


/** @defgroup IAS_Client Ias service client
  * @brief ias service client
  * @details
     Ias Profile is a customized BLE-based Profile. Ias ble service please refer to @ref IAS_Service .
  * @{
  */
/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup IAS_Client_Exported_Macros IAS Client Exported Macros
  * @brief
  * @{
  */
/** @defgroup IAS UUIDs
  * @brief Ias BLE Profile UUID definitions
  * @{
  */

#define GATT_UUID_IMMEDIATE_ALERT_SERVICE       0x1802
#define GATT_UUID_CHAR_ALERT_LEVEL              0x2A06

/** @} End of SIMP_UUIDs */

/** @brief  Define links number. range: 0-4 */
#define IAS_MAX_LINKS  2
/** End of IAS_Client_Exported_Macros
  * @}
  */


/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup IAS_Client_Exported_Types IAS Client Exported Types
  * @brief
  * @{
  */

/** @brief IAS client handle type*/
typedef enum
{
    HDL_IAS_SRV_START,           //!< start handle of simple ble service
    HDL_IAS_SRV_END,             //!< end handle of simple ble service
    HDL_IAS_WRITE,            //!< V2 write characteristic value handle
    HDL_IAS_CACHE_LEN            //!< handle cache length
} T_IAS_HANDLE_TYPE;

/** @brief IAS client discovery state*/
typedef enum
{
    DISC_IAS_IDLE,
    DISC_IAS_START,
    DISC_IAS_DONE,
    DISC_IAS_FAILED
} T_IAS_DISC_STATE;

///** @brief ias client read type*/
//typedef enum
//{
//    IAS_READ_V1_READ,
//    IAS_READ_V3_NOTIFY_CCCD,
//    IAS_READ_V4_INDICATE_CCCD,
//} T_IAS_READ_TYPE;

///** @brief IAS client read value*/
//typedef struct
//{
//    uint16_t value_size;
//    uint8_t *p_value;
//} T_IAS_READ_VALUE;

///** @brief IAS client read data*/
//typedef union
//{
//    T_IAS_READ_VALUE v1_read;
//    bool v3_notify_cccd;
//    bool v4_indicate_cccd;
//} T_IAS_READ_DATA;

///** @brief IAS client read result*/
//typedef struct
//{
//    T_IAS_READ_TYPE type;
//    T_IAS_READ_DATA data;
//    uint16_t cause;
//} T_IAS_READ_RESULT;


/** @brief IAS client write type*/
typedef enum
{
    IAS_WRITE_ALERT,
} T_IAS_WRTIE_TYPE;

/** @brief IAS client write result*/
typedef struct
{
    T_IAS_WRTIE_TYPE type;
    uint16_t cause;
} T_IAS_WRITE_RESULT;

///** @brief IAS client notif/ind receive type*/
//typedef enum
//{
//    IAS_V3_NOTIFY,
//    IAS_V4_INDICATE,
//} T_IAS_NOTIF_IND_TYPE;

///** @brief IAS client notif/ind receive data*/
//typedef struct
//{
//    uint16_t value_size;
//    uint8_t *p_value;
//} T_IAS_NOTIF_IND_VALUE;

///** @brief IAS client notif/ind receive content*/
//typedef struct
//{
//    T_IAS_NOTIF_IND_TYPE type;
//    T_IAS_NOTIF_IND_VALUE data;
//} T_IAS_NOTIF_IND_DATA;

/** @brief IAS client callback type*/
typedef enum
{
    IAS_CLIENT_CB_TYPE_DISC_STATE,          //!< Discovery procedure state, done or pending.
    //IAS_CLIENT_CB_TYPE_READ_RESULT,         //!< Read request's result data, responsed from server.
    IAS_CLIENT_CB_TYPE_WRITE_RESULT,        //!< Write request result, success or fail.
    // IAS_CLIENT_CB_TYPE_NOTIF_IND_RESULT,    //!< Notification or indication data received from server.
    IAS_CLIENT_CB_TYPE_INVALID              //!< Invalid callback type, no practical usage.
} T_IAS_CLIENT_CB_TYPE;

/** @brief IAS client callback content*/
typedef union
{
    T_IAS_DISC_STATE      disc_state;
    //T_IAS_READ_RESULT     read_result;
    T_IAS_WRITE_RESULT    write_result;
    //T_IAS_NOTIF_IND_DATA  notif_ind_data;
} T_IAS_CLIENT_CB_CONTENT;

/** @brief IAS client callback data*/
typedef struct
{
    T_IAS_CLIENT_CB_TYPE     cb_type;
    T_IAS_CLIENT_CB_CONTENT  cb_content;
} T_IAS_CLIENT_CB_DATA;

/** End of IAS_Client_Exported_Types * @} */

/** @defgroup IAS_Client_Exported_Functions IAS Client Exported Functions
  * @{
  */

/**
  * @brief      Add ias service client to application.
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
        ias_client_id = ias_add_client(app_client_callback, APP_MAX_LINKS);
    }
  * \endcode
  */
T_CLIENT_ID ias_add_client(P_FUN_GENERAL_APP_CB app_cb, uint8_t link_num);

/**
  * @brief  Used by application, to start the discovery procedure of ias server.
  * @param[in]  conn_id connection ID.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
bool ias_client_start_discovery(uint8_t conn_id);

/**
  * @brief  Used by application, to read data from server by using handles.
  * @param[in]  conn_id connection ID.
  * @param[in]  read_type one of characteristic that has the readable property.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
//bool ias_client_read_by_handle(uint8_t conn_id, T_IAS_READ_TYPE read_type);

/**
  * @brief  Used by application, to read data from server by using UUIDs.
  * @param[in]  conn_id connection ID.
  * @param[in]  read_type one of characteristic that has the readable property.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
//bool ias_client_read_by_uuid(uint8_t conn_id, T_IAS_READ_TYPE read_type);

/**
  * @brief  Used by application, to write data of Characteristic.
  * @param[in]  conn_id connection ID.
  * @param[in]  length  write data length
  * @param[in]  p_value point the value to write
  * @param[in]  type    write type.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
bool ias_client_write_char(uint8_t conn_id, uint16_t length, uint8_t *p_value,
                           T_GATT_WRITE_TYPE type);

/**
  * @brief  Used by application, to enable or disable the notification of peer server's V3 Notify Characteristic.
  * @param[in]  conn_id connection ID.
  * @param[in]  notify 0--disable the notification, 1--enable the notification.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
//bool simp_ble_client_set_v3_notify(uint8_t conn_id, bool notify);

/**
  * @brief  Used by application, to enable or disable the indication of peer server's V4 Indicate Characteristic.
  * @param[in]  conn_id connection ID.
  * @param[in]  ind 0--disable the indication, 1--enable the indication.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
//bool simp_ble_client_set_v4_ind(uint8_t conn_id, bool ind);

/**
  * @brief  Used by application, to get handle cache.
  * @param[in]  conn_id connection ID.
  * @param[in]  p_hdl_cache pointer of the handle cache table
  * @param[in]  len the length of handle cache table
  * @retval true success.
  * @retval false failed.
  */
bool ias_client_get_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len);

/**
  * @brief  Used by application, to set handle cache.
  * @param[in]  conn_id connection ID.
  * @param[in]  p_hdl_cache pointer of the handle cache table
  * @param[in]  len the length of handle cache table
  * @retval true success.
  * @retval false failed.
  */
bool ias_client_set_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len);

/** @} End of IAS_Client_Exported_Functions */

/** @} End of IAS_Client */


#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif  /* _IAS_CLIENT_H_ */
