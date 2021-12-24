/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     kns_client.h
  * @brief    Head file for using KNS Client.
  * @details  kns data structs and external functions declaration.
  * @author   ken
  * @date     2017-12-04
  * @version  v0.1
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _KNS_CLIENT_H_
#define _KNS_CLIENT_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

/* Add Includes here */
#include <profile_client.h>


/** @defgroup KNS_Client Kns service client
  * @brief kns service client
  * @details
     kns Profile is a customized BLE-based Profile. Kns ble service please refer to @ref KNS_Service .
  * @{
  */
/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup KNS_Client_Exported_Macros KNS Client Exported Macros
  * @brief
  * @{
  */
/** @defgroup KNS UUIDs
  * @brief Kns BLE Profile UUID definitions
  * @{
  */
//#define GATT_UUID_EXS  0xFFD0
//#define GATT_UUID_CHAR_PARAM 0xFFD1
//#define GATT_UUID_CHAR_KEY  0xFFD2
extern uint8_t   GATT_UUID128_KNS_SERVICE[16];
#define GATT_UUID128_KNS    0xA6, 0xF6, 0xF6, 0x07, 0x4D, 0xC4, 0x9D, 0x98, 0x6D, 0x45, 0x29, 0xBB, 0xD0, 0xFF, 0x00, 0x00,
#define GATT_UUID128_CHAR_PARAM   0xA6, 0xF6, 0xF6, 0x07, 0x4D, 0xC4, 0x9D, 0x98, 0x6D, 0x45, 0x29, 0xBB, 0xD1, 0xFF, 0x00, 0x00,
#define GATT_UUID128_CHAR_KEY     0xA6, 0xF6, 0xF6, 0x07, 0x4D, 0xC4, 0x9D, 0x98, 0x6D, 0x45, 0x29, 0xBB, 0xD2, 0xFF, 0x00, 0x00,

/** @} End of KNS_UUIDs */

/** @brief  Define links number. range: 0-4 */
#define KNS_MAX_LINKS  2
/** End of KNS_Client_Exported_Macros
  * @}
  */


/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup KNS_Exported_Types KNS Client Exported Types
  * @brief
  * @{
  */

/** @brief KNS client handle type*/
typedef enum
{
    HDL_KNS_SRV_START,           //!< start handle of kns service
    HDL_KNS_SRV_END,             //!< end handle of kns service
    HDL_KNS_PARA,                //!< KNS parameter read/write characteristic value handle
    HDL_KNS_NOTIFY_KEY,           //!< key notify characteristic value handle
    HDL_KNS_NOTIFY_KEY_CCCD,      //!< key notify characteristic CCCD handle
    HDL_KNS_CACHE_LEN            //!< handle cache length
} T_KNS_HANDLE_TYPE;

/** @brief KNS client discovery state*/
typedef enum
{
    DISC_KNS_IDLE,
    DISC_KNS_START,
    DISC_KNS_DONE,
    DISC_KNS_FAILED
} T_KNS_DISC_STATE;

/** @brief kns client read type*/
typedef enum
{
    KNS_READ_PARAM,
    KNS_READ_KEY_NOTIFY_CCCD,
} T_KNS_READ_TYPE;

/** @brief KNS client read value*/
typedef struct
{
    uint16_t value_size;
    uint8_t *p_value;
} T_KNS_READ_VALUE;

/** @brief KNS client read data*/
typedef union
{
    T_KNS_READ_VALUE v1_read;
    bool v3_notify_cccd;
    bool v4_indicate_cccd;
} T_KNS_READ_DATA;

/** @brief KNS client read result*/
typedef struct
{
    T_KNS_READ_TYPE type;
    T_KNS_READ_DATA data;
    uint16_t cause;
} T_KNS_READ_RESULT;


/** @brief KNS client write type*/
typedef enum
{
    KNS_WRITE_PARAM,
    KNS_WRITE_KEY_NOTIFY_CCCD,
} T_KNS_WRTIE_TYPE;

/** @brief KNS client write result*/
typedef struct
{
    T_KNS_WRTIE_TYPE type;
    uint16_t cause;
} T_KNS_WRITE_RESULT;

/** @brief KNS client notif/ind receive type*/
typedef enum
{
    KNS_KEY_NOTIFY,
} T_KNS_NOTIF_IND_TYPE;

/** @brief KNS client notif/ind receive data*/
typedef struct
{
    uint16_t value_size;
    uint8_t *p_value;
} T_KNS_NOTIF_IND_VALUE;

/** @brief KNS client notif/ind receive content*/
typedef struct
{
    T_KNS_NOTIF_IND_TYPE type;
    T_KNS_NOTIF_IND_VALUE data;
} T_KNS_NOTIF_IND_DATA;

/** @brief KNS client callback type*/
typedef enum
{
    KNS_CLIENT_CB_TYPE_DISC_STATE,          //!< Discovery procedure state, done or pending.
    KNS_CLIENT_CB_TYPE_READ_RESULT,         //!< Read request's result data, responsed from server.
    KNS_CLIENT_CB_TYPE_WRITE_RESULT,        //!< Write request result, success or fail.
    KNS_CLIENT_CB_TYPE_NOTIF_IND_RESULT,    //!< Notification or indication data received from server.
    KNS_CLIENT_CB_TYPE_INVALID              //!< Invalid callback type, no practical usage.
} T_KNS_CLIENT_CB_TYPE;

/** @brief KNS client callback content*/
typedef union
{
    T_KNS_DISC_STATE      disc_state;
    T_KNS_READ_RESULT     read_result;
    T_KNS_WRITE_RESULT    write_result;
    T_KNS_NOTIF_IND_DATA  notif_ind_data;
} T_KNS_CLIENT_CB_CONTENT;

/** @brief KNS client callback data*/
typedef struct
{
    T_KNS_CLIENT_CB_TYPE     cb_type;
    T_KNS_CLIENT_CB_CONTENT  cb_content;
} T_KNS_CLIENT_CB_DATA;

/** End of KNS_Client_Exported_Types * @} */

/** @defgroup KNS_Client_Exported_Functions KNS Client Exported Functions
  * @{
  */

/**
  * @brief      Add kns service client to application.
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
        kns_client_id = kns_add_client(app_client_callback, APP_MAX_LINKS);
    }
  * \endcode
  */
T_CLIENT_ID kns_add_client(P_FUN_GENERAL_APP_CB app_cb, uint8_t link_num);

/**
  * @brief  Used by application, to start the discovery procedure of ias server.
  * @param[in]  conn_id connection ID.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
bool kns_client_start_discovery(uint8_t conn_id);

/**
  * @brief  Used by application, to read data from server by using handles.
  * @param[in]  conn_id connection ID.
  * @param[in]  read_type one of characteristic that has the readable property.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
bool kns_client_read_by_handle(uint8_t conn_id, T_KNS_READ_TYPE read_type);

/**
  * @brief  Used by application, to read data from server by using UUIDs.
  * @param[in]  conn_id connection ID.
  * @param[in]  read_type one of characteristic that has the readable property.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
bool kns_client_read_by_uuid(uint8_t conn_id, T_KNS_READ_TYPE read_type);

/**
  * @brief  Used by application, to write data of V2 write Characteristic.
  * @param[in]  conn_id connection ID.
  * @param[in]  length  write data length
  * @param[in]  p_value point the value to write
  * @param[in]  type    write type.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
bool kns_client_write_v2_char(uint8_t conn_id, uint16_t length, uint8_t *p_value,
                              T_GATT_WRITE_TYPE type);

/**
  * @brief  Used by application, to enable or disable the notification of peer server's V3 Notify Characteristic.
  * @param[in]  conn_id connection ID.
  * @param[in]  notify 0--disable the notification, 1--enable the notification.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
bool kns_client_set_v3_notify(uint8_t conn_id, bool notify);

/**
  * @brief  Used by application, to enable or disable the indication of peer server's V4 Indicate Characteristic.
  * @param[in]  conn_id connection ID.
  * @param[in]  ind 0--disable the indication, 1--enable the indication.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  */
bool kns_client_set_v4_ind(uint8_t conn_id, bool ind);

/**
  * @brief  Used by application, to get handle cache.
  * @param[in]  conn_id connection ID.
  * @param[in]  p_hdl_cache pointer of the handle cache table
  * @param[in]  len the length of handle cache table
  * @retval true success.
  * @retval false failed.
  */
bool kns_client_get_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len);

/**
  * @brief  Used by application, to set handle cache.
  * @param[in]  conn_id connection ID.
  * @param[in]  p_hdl_cache pointer of the handle cache table
  * @param[in]  len the length of handle cache table
  * @retval true success.
  * @retval false failed.
  */
bool kns_client_set_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len);

/** @} End of KNS_Client_Exported_Functions */

/** @} End of KNS_Client */


#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif  /* _KNS_CLIENT_H_ */
