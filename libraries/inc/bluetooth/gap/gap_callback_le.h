/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    gap_callback_le.h
  * @brief   This file contains function prototype for all GAP roles.
  * @details
  * @author  ranhui
  * @date    2016-02-18
  * @version v0.1
  * *************************************************************************************
  */

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef GAP_CALLBACK_LE_H
#define GAP_CALLBACK_LE_H

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include "upperstack_config.h"
#include "gap_storage_le.h"
#include "gap_le_types.h"

/** @defgroup GAP_CB_MSG_MODULE GAP Callback Message
  * @brief GAP Callback Message
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup Gap_CB_Msg_Exported_Macros GAP Callback Msg Exported Macros
  * @{
  */

/** @defgroup GAP_LE_MSG_Types GAP LE Msg Types
  * @{
  */

//gap_le.h
#define GAP_MSG_LE_MODIFY_WHITE_LIST                0x01 //!<Response msg type for le_modify_white_list
#define GAP_MSG_LE_SET_RAND_ADDR                    0x02 //!<Response msg type for le_set_rand_addr
#define GAP_MSG_LE_SET_HOST_CHANN_CLASSIF           0x03 //!<Response msg type for le_set_host_chann_classif
#if F_BT_LE_4_2_DATA_LEN_EXT_SUPPORT
#define GAP_MSG_LE_WRITE_DEFAULT_DATA_LEN           0x04 //!<Response msg type for le_write_default_data_len
#endif

//gap_conn_le.h
#define GAP_MSG_LE_READ_RSSI                        0x10 //!<Response msg type for le_read_rssi
#if F_BT_LE_READ_CHANN_MAP
#define GAP_MSG_LE_READ_CHANN_MAP                   0x11 //!<Response msg type for le_read_chann_map
#endif
#define GAP_MSG_LE_DISABLE_SLAVE_LATENCY            0x12 //!<Response msg type for le_disable_slave_latency
#if F_BT_LE_4_2_DATA_LEN_EXT_SUPPORT
#define GAP_MSG_LE_SET_DATA_LEN                     0x13 //!<Response msg type for le_set_data_len
#define GAP_MSG_LE_DATA_LEN_CHANGE_INFO             0x14 //!<Notification msg type for data length changed
#endif
#if F_BT_LE_GAP_CENTRAL_SUPPORT
#define GAP_MSG_LE_CONN_UPDATE_IND                  0x15 //!<Indication for le connection parameter update
#endif
#define GAP_MSG_LE_CREATE_CONN_IND                  0x16 //!<Indication for create le connection
#if F_BT_LE_5_0_SET_PHYS_SUPPORT
#define GAP_MSG_LE_PHY_UPDATE_INFO                  0x17 //!<Indication for le phyical update information
#endif
#define GAP_MSG_LE_UPDATE_PASSED_CHANN_MAP          0x18 //!<Response msg type for le_update_passed_chann_map
#if F_BT_LE_READ_REMOTE_FEATS
#define GAP_MSG_LE_REMOTE_FEATS_INFO                0x19 //!<Information for remote device supported features
#endif
#define GAP_MSG_LE_SET_CONN_TX_PWR                  0x1A //!<Response msg type for le_set_conn_tx_power
#if F_BT_LE_READ_REMOTE_VERSION_INFO_SUPPORT
#define GAP_MSG_LE_READ_REMOTE_VERSION              0x1B //!<Response msg type for le_read_remote_version
#endif

//gap_bond_le.h
#define GAP_MSG_LE_BOND_MODIFY_INFO                 0x20 //!<Notification msg type for bond modify
#if F_BT_LE_4_2_KEY_PRESS_SUPPORT
#define GAP_MSG_LE_KEYPRESS_NOTIFY                  0x21 //!<Response msg type for le_bond_keypress_notify
#define GAP_MSG_LE_KEYPRESS_NOTIFY_INFO             0x22 //!<Notification msg type for le_bond_keypress_notify
#endif
#if F_BT_LE_ATT_SIGNED_WRITE_SUPPORT
#define GAP_MSG_LE_GATT_SIGNED_STATUS_INFO          0x23 //!<Notification msg type for le signed status information
#endif

//gap_scan.h
#if F_BT_LE_GAP_SCAN_SUPPORT
#define GAP_MSG_LE_SCAN_INFO                        0x30 //!<Notification msg type for le scan
#if F_BT_LE_PRIVACY_SUPPORT
#define GAP_MSG_LE_DIRECT_ADV_INFO                  0x31 //!<Notification msg type for le direct adv info
#endif
#endif

//gap_adv.h
#if F_BT_LE_GAP_PERIPHERAL_SUPPORT
#define GAP_MSG_LE_ADV_UPDATE_PARAM                 0x40 //!<Response msg type for le_adv_update_param
#if F_BT_LE_READ_ADV_TX_POWRE_SUPPORT
#define GAP_MSG_LE_ADV_READ_TX_POWER                0x41 //!<Response msg type for le_adv_read_tx_power
#endif
#define GAP_MSG_LE_ADV_SET_TX_POWER                 0x42 //!<Response msg type for le_adv_set_tx_power
#endif

//gap_dtm.h
#if F_BT_LE_4_0_DTM_SUPPORT
#define GAP_MSG_LE_DTM_RECEIVER_TEST                0x70 //!<Response msg type for le_dtm_receiver_test
#define GAP_MSG_LE_DTM_TRANSMITTER_TEST             0x71 //!<Response msg type for le_dtm_transmitter_test
#define GAP_MSG_LE_DTM_TEST_END                     0x72 //!<Response msg type for le_dtm_test_end
#if F_BT_LE_5_0_DTM_SUPPORT
#define GAP_MSG_LE_DTM_ENHANCED_RECEIVER_TEST       0x73 //!<Response msg type for le_dtm_enhanced_receiver_test
#define GAP_MSG_LE_DTM_ENHANCED_TRANSMITTER_TEST    0x74 //!<Response msg type for le_dtm_enhanced_transmitter_test
#endif
#if F_BT_LE_5_1_DTM_SUPPORT
#define GAP_MSG_LE_DTM_RECEIVER_TEST_V3             0x75 //!<Response msg type for le_dtm_receiver_test_v3
#define GAP_MSG_LE_DTM_TRANSMITTER_TEST_V3          0x76 //!<Response msg type for le_dtm_transmitter_test_v3
#endif
#endif

//gap_vendor.h
#define GAP_MSG_LE_VENDOR_SET_MIN_REM_SCA           0xA2 //!<Response msg type for le_set_min_rem_sca
#define GAP_MSG_LE_VENDOR_ONE_SHOT_ADV              0xA3 //!<Response msg type for le_vendor_one_shot_adv

#if F_BT_LE_5_0_AE_SCAN_SUPPORT
//gap_ext_scan.h
#define GAP_MSG_LE_EXT_ADV_REPORT_INFO              0x50 //!<Notification msg type for le extended adv report
#endif

#if F_BT_LE_5_0_AE_ADV_SUPPORT
//gap_ext_adv.h
#define GAP_MSG_LE_EXT_ADV_START_SETTING            0x60 //!<Response msg type for le_ext_adv_start_setting
#define GAP_MSG_LE_EXT_ADV_REMOVE_SET               0x61 //!<Response msg type for le_ext_adv_remove_set
#define GAP_MSG_LE_EXT_ADV_CLEAR_SET                0x62 //!<Response msg type for le_ext_adv_clear_set
#define GAP_MSG_LE_EXT_ADV_ENABLE                   0x63 //!<Response msg type for le_ext_adv_enable
#define GAP_MSG_LE_EXT_ADV_DISABLE                  0x64 //!<Response msg type for le_ext_adv_disable
#define GAP_MSG_LE_SCAN_REQ_RECEIVED_INFO           0x65 //!<Notification msg type for le scan received info
#endif

#define GAP_MSG_LE_GAP_STATE_MSG                    0xB0

/** End of GAP_LE_MSG_Types
  * @}
  */

/** End of Gap_CB_Msg_Exported_Macros
* @}
*/


/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup Gap_CB_Msg_Exported_Types GAP Callback Msg Exported Types
  * @{
  */
typedef struct
{
    uint16_t            cause;
} T_LE_CAUSE;

/** @brief  Response of le modify white list request.*/
typedef struct
{
    T_GAP_WHITE_LIST_OP operation;
    uint16_t            cause;
} T_LE_MODIFY_WHITE_LIST_RSP;

/** @brief  Response of le set random address request. */
typedef struct
{
    uint16_t        cause;
} T_LE_SET_RAND_ADDR_RSP;

/** @brief  Response of le set channel classification request. */
typedef struct
{
    uint16_t        cause;
} T_LE_SET_HOST_CHANN_CLASSIF_RSP;

/** @brief  Response for read rssi.*/
typedef struct
{
    uint8_t         conn_id;
    int8_t          rssi;
    uint16_t        cause;
} T_LE_READ_RSSI_RSP;

#if F_BT_LE_READ_CHANN_MAP
/** @brief  Response for read chanel map.*/
typedef struct
{
    uint8_t         conn_id;
    uint16_t        cause;
    uint8_t         channel_map[5];
} T_LE_READ_CHANN_MAP_RSP;
#endif
/** @brief  Response for disable slave latency.*/
typedef struct
{
    uint16_t        cause;
} T_LE_DISABLE_SLAVE_LATENCY_RSP;

#if F_BT_LE_4_2_DATA_LEN_EXT_SUPPORT
/** @brief  Response for set data length, which is used for BT4.2 data length extension.*/
typedef struct
{
    uint8_t         conn_id;
    uint16_t        cause;
} T_LE_SET_DATA_LEN_RSP;

/** @brief  Notification for data length change info, which is used for BT4.2 data length extension.*/
typedef struct
{
    uint8_t         conn_id;
    uint16_t        max_tx_octets;
    uint16_t        max_tx_time;
    uint16_t        max_rx_octets;
    uint16_t        max_rx_time;
} T_LE_DATA_LEN_CHANGE_INFO;
#endif

#if F_BT_LE_GAP_CENTRAL_SUPPORT
/** @brief  Indication for connection paramete update.*/
typedef struct
{
    uint8_t         conn_id;
    uint16_t        conn_interval_min;
    uint16_t        conn_interval_max;
    uint16_t        conn_latency;
    uint16_t        supervision_timeout;
} T_LE_CONN_UPDATE_IND;
#endif

/** @brief  Indication of le connectiona. */
typedef struct
{
    uint8_t                bd_addr[6];/**< Bluetooth address of remote device */
    T_GAP_REMOTE_ADDR_TYPE remote_addr_type; /**< Address type of remote device */
}  T_LE_CREATE_CONN_IND;

#if F_BT_LE_5_0_SET_PHYS_SUPPORT
/** @brief  Notification information when phy changed.*/
typedef struct
{
    uint8_t         conn_id;
    uint16_t        cause;
    T_GAP_PHYS_TYPE tx_phy;
    T_GAP_PHYS_TYPE rx_phy;
} T_LE_PHY_UPDATE_INFO;
#endif

/** @brief  Response for  update passed channel map.*/
typedef struct
{
    uint16_t        cause;
} T_LE_UPDATE_PASSED_CHANN_MAP_RSP;

#if F_BT_LE_READ_REMOTE_FEATS
/** @brief  Information for remote device features.*/
typedef struct
{
    uint8_t         conn_id;
    uint16_t        cause;
    uint8_t         remote_feats[8];
} T_LE_REMOTE_FEATS_INFO;
#endif

#if F_BT_LE_READ_REMOTE_VERSION_INFO_SUPPORT
/** @brief  Response for reading remote version information.*/
typedef struct
{
    uint8_t         conn_id;
    uint16_t        cause;
    uint8_t         version;
    uint16_t        manufacturer_name;
    uint16_t        subversion;
} T_LE_READ_REMOTE_VERSION_RSP;
#endif

//gap_bond_le.h
/** @brief  Bond information modify type*/
typedef enum
{
    LE_BOND_DELETE,
    LE_BOND_ADD,
    LE_BOND_CLEAR,
    LE_BOND_FULL,
    LE_BOND_KEY_MISSING,
} T_LE_BOND_MODIFY_TYPE;

/** @brief  Structure for modify bonding information.*/
typedef struct
{
    T_LE_BOND_MODIFY_TYPE type;
    T_LE_KEY_ENTRY        *p_entry;
} T_LE_BOND_MODIFY_INFO;

#if F_BT_LE_4_2_KEY_PRESS_SUPPORT
/** @brief  Structure for notify keypress result.*/
typedef struct
{
    uint8_t         conn_id;
    uint16_t        cause;
} T_LE_KEYPRESS_NOTIFY_RSP;

/** @brief  Structure for notify keypress information.*/
typedef struct
{
    uint8_t                   conn_id;
    T_GAP_KEYPRESS_NOTIF_TYPE event_type;
} T_LE_KEYPRESS_NOTIFY_INFO;
#endif

#if F_BT_LE_ATT_SIGNED_WRITE_SUPPORT
/** @brief  Structure for LE signed information.*/
typedef struct
{
    uint8_t         conn_id;
    uint16_t        cause;
    bool            update_local;
    uint32_t        local_sign_count;
    uint32_t        remote_sign_count;
} T_LE_GATT_SIGNED_STATUS_INFO;
#endif

#if F_BT_LE_4_0_DTM_SUPPORT
/** @brief  Response of le receiver test request. */
typedef struct
{
    uint16_t        cause;
    uint16_t        num_pkts;
} T_LE_DTM_TEST_END_RSP;
#endif

//gap_scan.h
#if F_BT_LE_GAP_SCAN_SUPPORT
/** @brief  Information of le scan information.  */
typedef struct
{
    uint8_t                bd_addr[6];/**< Bluetooth address of remote device. */
    T_GAP_REMOTE_ADDR_TYPE remote_addr_type;/**< Address type of remote device. */
    T_GAP_ADV_EVT_TYPE     adv_type;/**< Advertising event type. */
    int8_t                 rssi;   /**< RSSI. */
    uint8_t                data_len;
    uint8_t                data[31];
} T_LE_SCAN_INFO;

#if F_BT_LE_PRIVACY_SUPPORT
/** @brief  Information of le direct advertising.   */
typedef struct
{
    uint8_t                bd_addr[6];
    T_GAP_REMOTE_ADDR_TYPE remote_addr_type;
    uint8_t                direct_bd_addr[6];
    T_GAP_DIRECT_ADDR_TYPE direct_addr_type;
    T_GAP_ADV_EVT_TYPE     direct_adv_type;
    int8_t                 rssi;
} T_LE_DIRECT_ADV_INFO;
#endif
#endif

//gap_adv.h
#if F_BT_LE_GAP_PERIPHERAL_SUPPORT
/** @brief  LE advertising paramter update result.*/
typedef struct
{
    uint16_t        cause;
} T_LE_ADV_UPDATE_PARAM_RSP;

#if F_BT_LE_READ_ADV_TX_POWRE_SUPPORT
/** @brief  Response of le read advertising transmitter power request. */
typedef struct
{
    uint16_t        cause;
    uint8_t         tx_power_level;
} T_LE_ADV_READ_TX_POWER_RSP;
#endif
#endif

/** @brief  LE vendor advertising data type.*/
typedef enum
{
    GAP_ADV_VENDOR_ADV_38_DATA,
    GAP_ADV_VENDOR_ADV_39_DATA,
    GAP_ADV_VENDOR_SCANRSP_38_DATA,
    GAP_ADV_VENDOR_SCANRSP_39_DATA,
} T_GAP_ADV_VENDOR_DATA_TYPE;

typedef struct
{
    T_GAP_ADV_VENDOR_DATA_TYPE type;
    uint16_t                   cause;
} T_LE_VENDOR_ADV_3_DATA_SET_RSP;

#if F_BT_LE_5_0_AE_SCAN_SUPPORT
/** @brief  Information of le extended advertising report. */
typedef struct
{
    uint16_t                      event_type;
    T_GAP_EXT_ADV_EVT_DATA_STATUS data_status;
    T_GAP_REMOTE_ADDR_TYPE        addr_type;
    uint8_t                       bd_addr[6];
    T_GAP_PHYS_PRIM_ADV_TYPE      primary_phy;
    uint8_t                       secondary_phy;
    uint8_t                       adv_sid;
    int8_t                        tx_power;
    int8_t                        rssi;
    uint16_t                      peri_adv_interval;
    T_GAP_DIRECT_ADDR_TYPE        direct_addr_type;
    uint8_t                       direct_addr[6];
    uint8_t                       data_len;
    uint8_t                      *p_data; //max 229 bytes
} T_LE_EXT_ADV_REPORT_INFO;
#endif

#if F_BT_LE_5_0_AE_ADV_SUPPORT
//gap_ext_adv.h
typedef struct
{
    uint16_t        cause;
    uint8_t         adv_handle;
    uint8_t         flag;
} T_LE_EXT_ADV_START_SETTING_RSP;

typedef struct
{
    uint16_t        cause;
    uint8_t         adv_handle;
} T_LE_EXT_ADV_REMOVE_SET_RSP;

typedef struct
{
    uint16_t        cause;
} T_LE_EXT_ADV_CLEAR_SET_RSP;

/** @brief  Information of le scan request received. */
typedef struct
{
    uint8_t         adv_handle;
    uint8_t         scanner_addr_type;
    uint8_t         scanner_addr[6];
} T_LE_SCAN_REQ_RECEIVED_INFO;
#endif

/** @brief  GAP LE Callback Data*/
typedef union
{
    T_LE_CAUSE                                le_cause;

//gap_le.h
    T_LE_MODIFY_WHITE_LIST_RSP               *p_le_modify_white_list_rsp;
    T_LE_SET_RAND_ADDR_RSP                   *p_le_set_rand_addr_rsp;
    T_LE_SET_HOST_CHANN_CLASSIF_RSP          *p_le_set_host_chann_classif_rsp;

//gap_conn_le.h
    T_LE_READ_RSSI_RSP                       *p_le_read_rssi_rsp;
#if F_BT_LE_READ_CHANN_MAP
    T_LE_READ_CHANN_MAP_RSP                  *p_le_read_chann_map_rsp;
#endif
    T_LE_DISABLE_SLAVE_LATENCY_RSP           *p_le_disable_slave_latency_rsp;
#if F_BT_LE_4_2_DATA_LEN_EXT_SUPPORT
    T_LE_SET_DATA_LEN_RSP                    *p_le_set_data_len_rsp;
    T_LE_DATA_LEN_CHANGE_INFO                *p_le_data_len_change_info;
#endif
#if F_BT_LE_GAP_CENTRAL_SUPPORT
    T_LE_CONN_UPDATE_IND                     *p_le_conn_update_ind;
#endif
    T_LE_CREATE_CONN_IND                     *p_le_create_conn_ind;
#if F_BT_LE_5_0_SET_PHYS_SUPPORT
    T_LE_PHY_UPDATE_INFO                     *p_le_phy_update_info;
#endif
    T_LE_UPDATE_PASSED_CHANN_MAP_RSP         *p_le_update_passed_chann_map_rsp;
#if F_BT_LE_READ_REMOTE_FEATS
    T_LE_REMOTE_FEATS_INFO                   *p_le_remote_feats_info;
#endif
#if F_BT_LE_READ_REMOTE_VERSION_INFO_SUPPORT
    T_LE_READ_REMOTE_VERSION_RSP             *p_le_read_remote_version_rsp;
#endif

//gap_bond_le.h
    T_LE_BOND_MODIFY_INFO                    *p_le_bond_modify_info;
#if F_BT_LE_4_2_KEY_PRESS_SUPPORT
    T_LE_KEYPRESS_NOTIFY_RSP                 *p_le_keypress_notify_rsp;
    T_LE_KEYPRESS_NOTIFY_INFO                *p_le_keypress_notify_info;
#endif
#if F_BT_LE_ATT_SIGNED_WRITE_SUPPORT
    T_LE_GATT_SIGNED_STATUS_INFO             *p_le_gatt_signed_status_info;
#endif

//gap_scan.h
#if F_BT_LE_GAP_SCAN_SUPPORT
    T_LE_SCAN_INFO                           *p_le_scan_info;
#if F_BT_LE_PRIVACY_SUPPORT
    T_LE_DIRECT_ADV_INFO                     *p_le_direct_adv_info;
#endif
#endif

//gap_adv.h
#if F_BT_LE_GAP_PERIPHERAL_SUPPORT
    T_LE_ADV_UPDATE_PARAM_RSP                *p_le_adv_update_param_rsp;
#if F_BT_LE_READ_ADV_TX_POWRE_SUPPORT
    T_LE_ADV_READ_TX_POWER_RSP               *p_le_adv_read_tx_power_rsp;
#endif
#endif

//gap_dtm.h
#if F_BT_LE_4_0_DTM_SUPPORT
    T_LE_DTM_TEST_END_RSP                    *p_le_dtm_test_end_rsp;
#endif

//gap_vendor.h
    T_LE_VENDOR_ADV_3_DATA_SET_RSP           *p_le_vendor_adv_3_data_set_rsp;

//gap_ext_scan.h
#if F_BT_LE_5_0_AE_SCAN_SUPPORT
    T_LE_EXT_ADV_REPORT_INFO                 *p_le_ext_adv_report_info;
#endif

//gap_ext_adv.h
#if F_BT_LE_5_0_AE_ADV_SUPPORT
    T_LE_EXT_ADV_START_SETTING_RSP           *p_le_ext_adv_start_setting_rsp;
    T_LE_EXT_ADV_REMOVE_SET_RSP              *p_le_ext_adv_remove_set_rsp;
    T_LE_EXT_ADV_CLEAR_SET_RSP               *p_le_ext_adv_clear_set_rsp;
    T_LE_SCAN_REQ_RECEIVED_INFO              *p_le_scan_req_received_info;
#endif

    void                                     *p_gap_state_msg;
} T_LE_CB_DATA;

/** End of Gap_CB_Msg_Exported_Types
* @}
*/

/** End of GAP_CB_MSG_MODULE
* @}
*/

#ifdef __cplusplus
}
#endif

#endif /* GAP_MSG_H */
