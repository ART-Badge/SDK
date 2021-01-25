/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     hids_client.h
  * @brief    Head file for using hid service client.
  * @details  HID client data structs and external functions declaration.
  * @author   jane
  * @date     2016-02-18
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _HIDS_CLIENT_H_
#define _HIDS_CLIENT_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

/* Add Includes here */
#include <profile_client.h>
#include <stdint.h>
#include <stdbool.h>

/** @defgroup HIDS_CLIENT HID Service Client
  * @brief HIDS client
  * @details
     Application shall register hid host when initialization through @ref hids_add_client function.

     Application can start discovery battery service through @ref hids_start_discovery function.

     Application shall handle callback function registered by hids_add_client.
  * \code{.c}
    T_APP_RESULT app_client_callback(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data)
    {
        T_APP_RESULT  result = APP_RESULT_SUCCESS;
        if (client_id == hid_host_id)
        {
            T_HIDS_CLIENT_CB_DATA *p_hids_client_cb_data = (T_HIDS_CLIENT_CB_DATA *)p_data;
            switch (p_hids_client_cb_data->cb_type)
            {
            case HIDS_CLIENT_CB_TYPE_DISC_STATE:
                switch (p_hids_client_cb_data->cb_content.disc_state)
                {
                case DISC_HIDS_DONE:
                ......
        }
    }
  * \endcode
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @addtogroup HIDS_CLIENT_Exported_Macros HOGP Client Exported Macros
  * @brief
  * @{
  */
/** @brief  Define links number. range: 0-4 */
#define HIDS_CLIENT_MAX_LINKS  4

/** @brief  Define report number. */
#define HIDS_CLIENT_MAX_REPORT_NUM  10

/** @brief  Bit field of HIDS read flag. */
#define HIDS_READ_HID_INFO_BIT      0x01
#define HIDS_READ_PROTOCOL_MODE_BIT 0x02
#define HIDS_READ_REPORT_MAP_BIT    0x04
#define HIDS_READ_REPORT_REF_BIT    0x08

/** @defgroup HID UUIDs
  * @brief HID UUID definitions
  * @{
  */
#define GATT_UUID_HID                               0x1812
#define GATT_UUID_CHAR_HID_INFO                     0x2A4A
#define GATT_UUID_CHAR_REPORT_MAP                   0x2A4B
#define GATT_UUID_CHAR_HID_CONTROL_POINT            0x2A4C
#define GATT_UUID_CHAR_REPORT                       0x2A4D
#define GATT_UUID_CHAR_PROTOCOL_MODE                0x2A4E

#define GATT_UUID_CHAR_BOOT_KB_IN_REPORT            0x2A22    // boot keyboard input report
#define GATT_UUID_CHAR_BOOT_KB_OUT_REPORT           0x2A32    // boot keyboard output report
#define GATT_UUID_CHAR_BOOT_MS_IN_REPORT            0x2A33    // boot mouse input report

/** @} End of Hid UUIDs */

/** End of HIDS_CLIENT_Exported_Macros
* @}
*/


/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup HIDS_CLIENT__Exported_Types HOGP Client Exported Types
  * @brief
  * @{
  */

/** @brief HID host handle type*/
typedef enum
{
    HDL_HIDS_SRV_START,           //!< start handle of hid service
    HDL_HIDS_SRV_END,             //!< end handle of hid service
    HDL_HIDS_CHAR_HID_INFO,       //!< hid information characteristic value handle
    HDL_HIDS_CHAR_REPORT_MAP,     //!< report map characteristic value handle
    HDL_HIDS_CHAR_HID_CONTROL_POINT,   //!< hid control point characteristic value handle
    HDL_HIDS_CHAR_PROTOCOL_MODE,     //!< protocol mode characteristic value handle
    HDL_HIDS_CACHE_LEN       //!< handle cache length
} T_HIDS_CLIENT_HANDLE_TYPE;

/** @brief HID host discovery state*/
typedef enum
{
    DISC_HIDS_IDLE,
    DISC_HIDS_START,
    DISC_HIDS_DONE,
    DISC_HIDS_FAILED
} T_HIDS_CLIENT_DISC_STATE;

/** @brief HID host report type*/
typedef enum
{
    HIDS_RESERVED_REPORT,
    HIDS_INPUT_REPORT,
    HIDS_OUTPUT_REPORT,
    HIDS_FEATURE_REPORT,
} T_HIDS_REPORT_TYPE;

/** @brief HID host report reference data*/
typedef struct
{
    uint8_t             report_id;
    T_HIDS_REPORT_TYPE  report_type;
} T_HIDS_REPORT_REF_DATA;

/** @brief HID host report*/
typedef struct
{
    uint16_t hdl_report_value;
    uint16_t hdl_report_cccd;
    uint16_t hdl_report_reference;
    T_HIDS_REPORT_REF_DATA report_ref;
} T_HIDS_CLIENT_REPORT;


typedef struct
{
    uint8_t  CountryCode;
    uint8_t  bFlags;
    uint16_t BcdHID;
} HID_INFO_ATTRB;

/** @brief HID host read type*/
typedef enum
{
    HIDS_READ_PROTOCOL_MODE,
    HIDS_READ_REPORT_MAP,
    HIDS_READ_HID_INFO,
} T_HIDS_READ_TYPE;

/** @brief HID host read value */
typedef struct
{
    uint16_t value_size;
    uint8_t *p_value;
} T_HIDS_READ_VALUE;

#if 0
/** @brief HID host read data */
typedef union
{
    T_HIDS_READ_VALUE report_map;
    T_HIDS_READ_VALUE report_ref;
} T_HIDS_READ_DATA;
#endif

/** @brief HID host read result type */
typedef enum
{
    HIDS_READ_RESULT_RESERVED,
    HIDS_READ_RESULT_REPORT_MAP,
} T_HIDS_READ_RESULT_TYPE;

/** @brief HID host read result */
typedef struct
{
    T_HIDS_READ_RESULT_TYPE type;
    uint16_t cause;
} T_HIDS_CLIENT_READ_RESULT;

/** @brief HID host write type*/
typedef enum
{
    HIDS_WRITE_REPORT,
    HIDS_WRITE_ALL_REPORT_CCCD,
    //HIDS_WRITE_PROTOCOL_MODE,//write command
    //HIDS_WRITE_HID_CONTROL_POINT,//write command
} T_HIDS_WRTIE_TYPE;

typedef struct
{
    uint8_t report_id;
    uint16_t value_size;
    uint8_t *p_value;
} T_HIDS_WRITE_REPORT_DATA;

/** @brief HID host write result*/
typedef struct
{
    T_HIDS_WRTIE_TYPE type;
    uint16_t cause;
    T_HIDS_REPORT_REF_DATA report_ref; //just used for HIDS_WRITE_REPORT
} T_HIDS_CLIENT_WRITE_RESULT;

/** @brief HID host notification receive type*/
typedef enum
{
    HIDS_REPORT_NOTIFY,
} T_HIDS_NOTIF_TYPE;

/** @brief HID host notification receive data*/
typedef struct
{
    uint8_t report_id;
    uint16_t value_size;
    uint8_t *p_value;
} T_HIDS_NOTIF_VALUE;

/** @brief HID host notification receive content*/
typedef struct
{
    T_HIDS_NOTIF_TYPE type;
    T_HIDS_NOTIF_VALUE data;
} T_HIDS_CLIENT_NOTIF_DATA;

/** @brief HOGP client callback type*/
typedef enum
{
    HIDS_CLIENT_CB_TYPE_DISC_STATE,          //!< Discovery procedure state, done or pending.
    HIDS_CLIENT_CB_TYPE_READ_RESULT,         //!< Read request's result data, responsed from server.
    HIDS_CLIENT_CB_TYPE_WRITE_RESULT,        //!< Write request result, success or fail.
    HIDS_CLIENT_CB_TYPE_NOTIF_RESULT,    //!< Notification or indication data received from server.
    HIDS_CLIENT_CB_TYPE_INVALID              //!< Invalid callback type, no practical usage.
} T_HIDS_CLIENT_CB_TYPE;

/** @brief HOGP client callback content*/
typedef union
{
    T_HIDS_CLIENT_DISC_STATE disc_state;
    T_HIDS_CLIENT_READ_RESULT read_result;
    T_HIDS_CLIENT_NOTIF_DATA notify_data;
    T_HIDS_CLIENT_WRITE_RESULT write_result;
} T_HIDS_CLIENT_CB_CONTENT;

/** @brief HOGP client callback data*/
typedef struct
{
    T_HIDS_CLIENT_CB_TYPE     cb_type;
    T_HIDS_CLIENT_CB_CONTENT  cb_content;
} T_HIDS_CLIENT_CB_DATA;

/** @brief  HID Host storage information block definition. */
typedef struct
{
    uint8_t          read_flags;
    uint8_t          protocol_mode;
    uint8_t          report_num;
    HID_INFO_ATTRB   hids_info;
    uint16_t         hdl_cache[HDL_HIDS_CACHE_LEN];
    T_HIDS_CLIENT_REPORT report[HIDS_CLIENT_MAX_REPORT_NUM];
} T_HIDS_CLIENT_STORAGE_INFO;
/** End of HIDS_CLIENT_Exported_Types
* @}
*/


/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup HIDS_CLIENT_Exported_Functions HOGP Client Exported Functions
  * @brief
  * @{
  */

/**
  * @brief      Add hid host to application.
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
        hids_client_client_id = hids_add_client(app_client_callback, APP_MAX_LINKS);
    }
  * \endcode
  */
T_CLIENT_ID hids_add_client(P_FUN_GENERAL_APP_CB app_cb, uint8_t link_num);

/**
  * @brief  Used by application, to start the discovery procedure of battery service.
  * @param[in]  conn_id connection ID.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    static T_USER_CMD_PARSE_RESULT cmd_hidhostdis(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        uint8_t conn_id = p_parse_value->dw_param[0];
        bool ret = hids_start_discovery(conn_id);
        ......
    }
  * \endcode
  */
bool hids_start_discovery(uint8_t conn_id);

bool hids_write_report(uint8_t conn_id, uint8_t report_id, T_HIDS_REPORT_TYPE report_type,
                       uint16_t length, uint8_t *p_value, T_GATT_WRITE_TYPE type);

bool hids_enable_all_cccd(uint8_t conn_id);

bool hids_read_saved_data(uint8_t conn_id, T_HIDS_READ_TYPE read_type, T_HIDS_READ_VALUE *p_value);

bool hids_get_hdl_cache(uint8_t conn_id, T_HIDS_CLIENT_STORAGE_INFO *p_storage_info);

bool hids_set_hdl_cache(uint8_t conn_id, T_HIDS_CLIENT_STORAGE_INFO *p_storage_info);

bool hids_read_report_map(uint8_t conn_id);

bool hids_set_report_map(uint8_t conn_id, uint16_t value_size, uint8_t *p_value);

void hids_print_infos(uint8_t conn_id);

/** @} End of HIDS_CLEINT_Exported_Functions */

/** @} End of HIDS_CLEINT */




#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif  /* _HIDS_CLIENT_H_ */
