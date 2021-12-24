/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     dfu_client.h
  * @brief    Head file for using dfu service Client.
  * @details  Data structs and external functions declaration.
  * @author   bill
  * @date     2018-4-17
  * @version  v2.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _DFU_CLIENT_H_
#define _DFU_CLIENT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Add Includes here */
#include "patch_header_check.h"
#include "profile_client.h"
#include "dfu_api.h"

/** @defgroup DFU_Client
  * @{
  */
/** @brief  Define links number. range: 0-4 */
#define DFU_MAX_LINKS                           4


/** @brief Handle cache for intrested UUIDs */
typedef enum
{
    HDL_DFU_SRV_START,           // start handle of dfu service
    HDL_DFU_SRV_END,             // end handle of dfu service
    HDL_DFU_DATA,
    HDL_DFU_CP,
    HDL_DFU_CP_CCCD,
    HDL_DFU_CACHE_LEN
} T_DFU_HANDLE_TYPE;

/** @brief used to inform app the discovery procedure is done or pending */
typedef enum
{
    DISC_DFU_IDLE,
    DISC_DFU_START,
    DISC_DFU_DONE,
    DISC_DFU_FAIL,
} T_DFU_DISC_STATE;

/** @brief read request type, used by app to send read request */
typedef enum
{
    DFU_READ_CP_CCCD
} T_DFU_READ_TYPE;

/** @brief read content, used to inform app read response data content */
typedef union
{
    bool dfu_cp_cccd;
} T_DFU_READ_DATA;

/** @brief read data, used to inform app read response data */
typedef struct
{
    T_DFU_READ_TYPE type;
    T_DFU_READ_DATA data;
    uint16_t cause;
} T_DFU_READ_RESULT;

/** @brief write request result */
typedef enum
{
    DFU_WRITE_CP,
    DFU_WRITE_CP_CCCD,
    DFU_WRITE_DATA
} T_DFU_WRITE_TYPE;

typedef struct
{
    T_DFU_WRITE_TYPE type;
    uint16_t cause;
} T_DFU_WRITE_RESULT;

/** @brief notif/ind receive type */
typedef enum
{
    DFU_RECEIVE_CP_NOTIFY,
} T_DFU_NOTIFY_INDICATE_TYPE;

/** @brief notif/ind receive content */
typedef struct
{
    uint16_t len;
    uint8_t *pdata;
} T_DFU_NOTIFY_INDICATE_VAL;

/** @brief The notif/ind data received by the client from the server */
typedef struct
{
    T_DFU_NOTIFY_INDICATE_TYPE type;
    T_DFU_NOTIFY_INDICATE_VAL value;
} T_DFU_NOTIFY_INDICATE_DATA;

/** @brief Event type to inform app */
typedef enum
{
    DFU_CLIENT_CB_TYPE_DISC_STATE,          //!< Discovery procedure state, done or pending.
    DFU_CLIENT_CB_TYPE_READ_RESULT,         //!< Read request's result data, responsed from server.
    DFU_CLIENT_CB_TYPE_WRITE_RESULT,        //!< Write request result, success or fail.
    DFU_CLIENT_CB_TYPE_NOTIF_IND_RESULT,    //!< Notification or indication data received from server.
    DFU_CLIENT_CB_TYPE_INVALID              //!< Invalid callback type, no practical usage.
} T_DFU_CLIENT_CB_TYPE;

/** @brief Callback content sent to application from the client */
typedef union
{
    T_DFU_DISC_STATE disc_state;
    T_DFU_READ_RESULT read_result;
    T_DFU_WRITE_RESULT write_result;
    T_DFU_NOTIFY_INDICATE_DATA notif_ind_data;
} T_DFU_CLIENT_CB_CONTENT;

/** @brief Callback data sent to application from the client, include type and content */
typedef struct
{
    T_DFU_CLIENT_CB_TYPE cb_type;
    T_DFU_CLIENT_CB_CONTENT cb_content;
} T_DFU_CLIENT_CB_DATA;


typedef enum
{
    DFU_FSM_WRITE_IDLE,
    DFU_FSM_WRITE_CCCD_ENABLE,
    DFU_FSM_WRITE_DFU_DATA,
    DFU_FSM_WRITE_WAIT_WRITE_RESP
} T_DFU_CLIENT_FSM_WRITE;

typedef enum
{
    DFU_CB_START,
    DFU_CB_END,
    DFU_CB_FAIL,
    DFU_CB_MAX
} T_DFU_CB_TYPE;

typedef enum
{
    DFU_SUCCESS,
    DFU_FAIL_START_DFU,
    DFU_FAIL_BUF_CHECK,
    DFU_FAIL_VALIDATE_FW,
} T_DFU_FAIL_TYPE;

typedef union
{
    T_DFU_FAIL_TYPE dfu_fail_reason;
} T_DFU_CB_DATA;

/** Dfu service data to inform application */
typedef struct
{
    uint8_t conn_id;
    T_DFU_CB_TYPE type;
    T_DFU_CB_DATA data;
} T_DFU_CLIENT_CB_MSG;

typedef enum
{
    DFU_FEATURE_USER_DEFINED_DFU,
    DFU_FEATURE_AUTO_COPY_DFU,

} T_DFU_CLIENT_FEATURE_TYPE;

typedef enum
{
    DFU_SET_FEATURE,
    DFU_SET_MODE_VALUE,
    DFU_SET_MAX_BUF_SIZE,
    DFU_SET_DFU_IMG_ADDR,
    DFU_SET_DFU_IMG_VER,
    DFU_SET_NORMAL_OTA_FLAG,
} T_DFU_SET_CLIENT_CTX_TYPE;

typedef struct
{
    T_IMG_CTRL_HEADER_FORMAT dfu_ctrl_header;

    T_OTA_MODE ota_mode;
    T_DFU_CLIENT_FEATURE_TYPE dfu_feature;
    T_DFU_CLIENT_FSM_WRITE dfu_fsm_write;
    bool is_normal_ota;

    uint16_t mtu_size;
    uint16_t max_buffer_size;

    uint32_t dfu_img_start_addr;
    uint32_t dfu_img_version;
    uint32_t dfu_img_total_length;

    uint32_t curr_offset;
    uint32_t target_img_version;

    uint16_t dfu_buf_check_crc_val;
    uint16_t dfu_buf_check_size;

    /*ctx used when enable or disable buffer check*/
    uint16_t dfu_pkt_num;
    uint16_t dfu_pkt_counter;
    uint16_t dfu_last_pkt_size;
} T_DFU_CTX;

typedef void (*P_FUN_DFU_CLIENT_INIT_APP_CB)(void);

/** @} */

/** @defgroup DFU_Client_Exported_Functions Exported Functions
  * @{
  */
bool dfu_encrypt(uint8_t image[16]);
uint16_t dfu_client_crc_cal(uint8_t *buf, uint32_t length, uint16_t checksum16);

void dfu_client_set_dfu_ctx_value(T_DFU_SET_CLIENT_CTX_TYPE type, void *p_value);

/**
  * @brief start the service discovery
  *
  * contains the service/characteristic/cccd declaration
  * @param[in] conn_id: the connection id
  * @return the operatino result
  */
bool dfu_client_start_discovery(uint8_t conn_id);

/**
  * @brief  Used by application, to set the handles in Dfu handle cache.
  * @param  handle_type: handle types of this specific profile.
  * @param  handle_value: handle value to set.
  * @retval true--set success.
  *         flase--set failed.
  */
bool dfu_client_set_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len);

/**
  * @brief  Used by application, to get handle cache.
  * @param[in]  conn_id connection ID.
  * @param[in]  p_hdl_cache pointer of the handle cache table
  * @param[in]  len the length of handle cache table
  * @retval true success.
  * @retval false failed.
  */
bool dfu_client_get_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len);

/**
  * @brief  Used by application, to enable or disable the notification of peer server's V3 Notify Characteristic.
  * @param  command: 0--disable the notification, 1--enable the notification.
  * @retval true--send request to upper stack success.
  *         flase--send request to upper stack failed.
  */
bool dfu_client_cp_cccd_set(uint8_t conn_id, bool command);

/**
  * @brief  Used internal, to send write request to peer server's V5 Control Point Characteristic.
  * @param  ctl_pnt_ptr: pointer of control point data to write.
  * @retval true--send request to upper stack success.
  *         flase--send request to upper stack failed.
  */
bool dfu_client_cp_write(uint8_t conn_id, uint8_t *pdata, uint16_t length);

/**
  * @brief  Used internal, to send write request to peer server's V5 Control Point Characteristic.
  * @param  ctl_pnt_ptr: pointer of control point data to write.
  * @retval true--send request to upper stack success.
  *         flase--send request to upper stack failed.
  */
bool dfu_client_data_write(uint8_t conn_id, uint8_t *pdata, uint16_t length);

/**
  * @brief  add Dfu client to application.
  * @param  appCB: pointer of app callback function to handle specific client module data.
  * @retval Client ID of the specific client module.
  */
T_CLIENT_ID dfu_add_client(P_FUN_GENERAL_APP_CB app_cb, uint8_t link_num);


/** @} */
/** @} */

#ifdef __cplusplus
}
#endif

#endif  /* _DFU_CLIENT_H_ */
