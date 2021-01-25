/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     tps_client.h
  * @brief    Head file for using TPS Client.
  * @details  tps data structs and external functions declaration.
  * @author   ken
  * @date     2017-12-05
  * @version  v0.1
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _TPS_CLIENT_H_
#define _TPS_CLIENT_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

/* Add Includes here */
#include <profile_client.h>


/** @defgroup TPS_Client tps service client
  * @brief tps service client
  * @details
     tps Profile is a customized BLE-based Profile. Tps ble service please refer to @ref TPS_Service .
  * @{
  */
/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup TPS_Client_Exported_Macros TPS Client Exported Macros
  * @brief
  * @{
  */
/** @defgroup TPS UUIDs
  * @brief Tps BLE Profile UUID definitions
  * @{
  */
#define GATT_UUID_TX_POWER_SERVICE              0x1804
#define GATT_UUID_CHAR_TX_LEVEL                 0x2A07
/** @} End of TPS_UUIDs */

/** @brief  Define links number. range: 0-4 */
#define TPS_MAX_LINKS  2
/** End of TPS_Client_Exported_Macros
  * @}
  */


/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup TPS_Exported_Types TPS Client Exported Types
  * @brief
  * @{
  */

/** @brief TPS client handle type*/
typedef enum
{
    HDL_TPS_SRV_START,           //!< start handle of tps service
    HDL_TPS_SRV_END,             //!< end handle of tps service
    HDL_TPS_PARA,                //!< TPS parameter read/write characteristic value handle
    HDL_TPS_CACHE_LEN            //!< handle cache length
} T_TPS_HANDLE_TYPE;

/** @brief TPS client discovery state*/
typedef enum
{
    DISC_TPS_IDLE,
    DISC_TPS_START,
    DISC_TPS_DONE,
    DISC_TPS_FAILED
} T_TPS_DISC_STATE;

/** @brief tps client read type*/
typedef enum
{
    TPS_READ_PARA,
} T_TPS_READ_TYPE;


/** @brief TPS client read data */
typedef union
{
    uint8_t txpower_level;
} T_TPS_READ_DATA;


/** @brief TPS client read result*/
typedef struct
{
    T_TPS_READ_TYPE type;
    T_TPS_READ_DATA data;
    uint16_t cause;
} T_TPS_READ_RESULT;

/** @brief LLS client write type*/
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

/** @brief TPS client callback type*/
typedef enum
{
    TPS_CLIENT_CB_TYPE_DISC_STATE,          //!< Discovery procedure state, done or pending.
    TPS_CLIENT_CB_TYPE_READ_RESULT,         //!< Read request's result data, responsed from server.
    //LLS_CLIENT_CB_TYPE_WRITE_RESULT,        //!< Write request result, success or fail.
    //LLS_CLIENT_CB_TYPE_NOTIF_IND_RESULT,    //!< Notification or indication data received from server.
    TPS_CLIENT_CB_TYPE_INVALID              //!< Invalid callback type, no practical usage.
} T_TPS_CLIENT_CB_TYPE;

/** @brief TPS client callback content*/
typedef union
{
    T_TPS_DISC_STATE      disc_state;
    T_TPS_READ_RESULT     read_result;
    //T_TPS_WRITE_RESULT    write_result;
    //T_LLS_NOTIF_IND_DATA  notif_ind_data;
} T_TPS_CLIENT_CB_CONTENT;

/** @brief TPS client callback data*/
typedef struct
{
    T_TPS_CLIENT_CB_TYPE     cb_type;
    T_TPS_CLIENT_CB_CONTENT  cb_content;
} T_TPS_CLIENT_CB_DATA;

/** End of TPS_Client_Exported_Types * @} */

/** @defgroup TPS_Client_Exported_Functions TPS Client Exported Functions
  * @{
  */

/**
  * @brief      Add tps service client to application.
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
        tps_client_id = tps_add_client(app_client_callback, APP_MAX_LINKS);
    }
  * \endcode
  */
T_CLIENT_ID tps_add_client(P_FUN_GENERAL_APP_CB app_cb, uint8_t link_num);

/**
  * @brief  Used by application, to start the discovery procedure of ias server.
  * @param[in]  conn_id connection ID.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
bool tps_start_discovery(uint8_t conn_id);

/**
  * @brief  Used by application, to read power level from server.
  * @param[in]  conn_id connection ID.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
bool tps_read_power_level(uint8_t conn_id);


//bool tps_client_read_by_handle(uint8_t conn_id, T_TPS_READ_TYPE read_type);

///**
//  * @brief  Used by application, to read data from server by using UUIDs.
//  * @param[in]  conn_id connection ID.
//  * @param[in]  read_type one of characteristic that has the readable property.
//  * @retval true send request to upper stack success.
//  * @retval false send request to upper stack failed.
//  */
//bool tps_client_read_by_uuid(uint8_t conn_id, T_TPS_READ_TYPE read_type);

/**
  * @brief  Used by application, to write data of write Characteristic.
  * @param[in]  conn_id connection ID.
  * @param[in]  length  write data length
  * @param[in]  p_value point the value to write
  * @param[in]  type    write type.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
//bool tps_client_write_char(uint8_t conn_id, uint16_t length, uint8_t *p_value,
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
bool tps_client_get_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len);

/**
  * @brief  Used by application, to set handle cache.
  * @param[in]  conn_id connection ID.
  * @param[in]  p_hdl_cache pointer of the handle cache table
  * @param[in]  len the length of handle cache table
  * @retval true success.
  * @retval false failed.
  */
bool tps_client_set_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len);

/** @} End of LLS_Client_Exported_Functions */

/** @} End of LLS_Client */


#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif  /* _KNS_CLIENT_H_ */
