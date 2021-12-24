/**
*****************************************************************************************
*     Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     plxs.h
  * @brief    Head file for using Pulse Oximeter Service .
  * @details  Pulse Oximeter Service data types and external functions declaration.
  * @author   danni
  * @date     2018-12-27
  * @version  v1.0
  * *************************************************************************************
  */
/* Define to prevent recursive inclusion */
#ifndef _PLXS_H
#define _PLXS_H
#ifdef __cplusplus
extern "C"  {
#endif      /* __cplusplus */

/* Add Includes here */
#include "profile_server.h"
#include "plxs_config.h"

/** @defgroup PLXS Pulse Oximeter Service
  * @brief Pulse Oximeter Service
  * @{
  */
/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup PLXS_Exported_Macros PLXS Exported Macros
  * @brief
  * @{
  */
#define PLXS_SPOT_CHECK_MEASUREMENT_INDICATION_ENABLE                                          1    /**< spot check measurement indication cccd enable  */
#define PLXS_SPOT_CHECK_MEASUREMENT_INDICATION_DISABLE                                         2    /**< spot check measurement indication cccd disable */
#define PLXS_CONTINUOUS_MEASUREMENT_NOTIFY_ENABLE                                              3    /**< continuous measurement notify cccd enable      */
#define PLXS_CONTINUOUS_MEASUREMENT_NOTIFY_DISABLE                                             4    /**< continuous measurement notify cccd disable     */
#define PLXS_RACP_INDICATION_ENABLE                                                            5    /**< racp indication cccd enable*/
#define PLXS_RACP_INDICATION_DISABLE                                                           6    /**< racp indication cccd disable*/

#define PLXS_SFLOAT_VALUE_NaN                                                                  0x07ff    /**< not a number             */
#define PLXS_SFLOAT_VALUE_NRes                                                                 0x0800    /**< not at this resolution   */
#define PLXS_SFLOAT_VALUE_PlusINFINITY                                                         0x07fe    /**< + INFINITY               */
#define PLXS_SFLOAT_VALUE_MinusINFINITY                                                        0x0802    /**< - INFINITY               */
#define PLXS_SFLOAT_VALUE_RFU                                                                  0x0801    /**< reserved for future use  */
/** @} End of PLXS_Exported_Macros */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup PLXS_Exported_Types PLXS Exported Types
  * @brief
  * @{
  */
/** @brief define PLXS_TIMESTAMP type to store date time data,PLXS_TIMESTAMP[0] = LSO,PLXS_TIMESTAMP[6]=MSO*/
typedef  uint8_t PLXS_TIMESTAMP[7];

/** @brief define PLXS_SFLOAT type to store spo2,pr,Pulse Amplitude Index value,PLXS_SFLOAT[0] = LSO,PLXS_SFLOAT[1] = MSO*/
typedef   uint8_t PLXS_SFLOAT[2];

/** @brief plxs service parameter type*/
typedef enum
{
    PLXS_PARAM_SPOT_CHECK_MEASUREMENT_FLAGS = 0x01,
    PLXS_PARAM_CONTINUOUS_MEASUREMENT_FLAGS,
    PLXS_PARAM_FEATURE_FLAGS
} T_PLXS_PARAM_TYPE;

/** @brief  spot check measurement vaule indicate, continuous measurement vaule notify or racp procedure status  */
typedef enum
{
    PLXS_STATUS_SPOT_CHECK_MEASUREMENT_INDICATION_IDLE = 0x00,/**< function plxs_spot_check_measurement_value_indicate() never be called and in initialization state*/
    PLXS_STATUS_SPOT_CHECK_MEASUREMENT_INDICATION_DOING,/**< function plxs_spot_check_measurement_value_indicate()  be called success, but PROFILE_EVT_SEND_DATA_COMPLETE was not received*/
    PLXS_STATUS_SPOT_CHECK_MEASUREMENT_INDICATION_DONE,/**< function plxs_spot_check_measurement_value_indicate()  be called success, and PROFILE_EVT_SEND_DATA_COMPLETE was  received*/
    PLXS_STATUS_CONTINUOUS_MEASUREMENT_NOTIFY_IDLE,/**< function plxs_continuous_measurement_value_notify() never be called and in initialization state*/
    PLXS_STATUS_CONTINUOUS_MEASUREMENT_NOTIFY_DOING,/**< function plxs_continuous_measurement_value_notify()  be called success, but PROFILE_EVT_SEND_DATA_COMPLETE was not received*/
    PLXS_STATUS_CONTINUOUS_MEASUREMENT_NOTIFY_DONE,/**< function plxs_continuous_measurement_value_notify()  be called success, and PROFILE_EVT_SEND_DATA_COMPLETE was  received*/
    PLXS_STATUS_REPORT_RECORDS_INDICATION_IDLE,/**< function plxs_spot_check_measurement_value_store_indicate() never be called and in initialization state*/
    PLXS_STATUS_REPORT_RECORDS_INDICATION_DOING,/**< function plxs_spot_check_measurement_value_store_indicate() be called success, but PROFILE_EVT_SEND_DATA_COMPLETE was not received*/
    PLXS_STATUS_REPORT_RECORDS_INDICATION_DONE,/**< function plxs_spot_check_measurement_value_store_indicate() be called success and PROFILE_EVT_SEND_DATA_COMPLETE was  received*/
    PLXS_STATUS_NOT_SUPPORT,/**< status not supported*/
} T_PLXS_DATA_SEND_STATUS;

/** @brief  APP Return Result List */
typedef enum
{
    PLXS_APP_RESULT_SUCCESS = 0x00,/**< plx service return result success*/
    PLXS_APP_RESULT_PENDING,/**< if plx service return pending, means that report stored records procedure is in progress,please send data later*/
    PLXS_APP_RESULT_QUEUE_NULL,/**< plx service return result empty*/
    PLXS_APP_RESULT_NOT_SUPPORT,/**< plx service return result procedure not support*/
    PLXS_APP_RESULT_INVALID_VALUE_SIZE,/**< plx service return result invalid value size*/
    PLXS_APP_RESULT_INVALID_TYPE,/**< plx service return result invalid type*/
    PLXS_APP_RESULT_INVALID_OFFSET,/**< plx service return result invalid offset*/
    PLXS_APP_RESULT_POINTER_NULL,/**< plx service return result pointer is null*/
    PLXS_APP_RESULT_FAIL,/**< plx service return result fail for other reasons*/
    PLXS_APP_RESULT_CCCD_NOT_ENABLED/**< plx service return result cccd not enable*/
} T_PLXS_APP_RESULT;

/** @brief measurement status bits*/
typedef struct
{
    uint16_t    rfu: 5;
    uint16_t    measurement_ongoing: 1;
    uint16_t    early_estimated_data: 1;
    uint16_t    validated_data: 1;
    uint16_t    fully_qualified_data: 1;
    uint16_t    data_from_measurement_storage: 1;
    uint16_t    data_for_demonstration: 1;
    uint16_t    data_for_testing: 1;
    uint16_t    calibration_ongoing: 1;
    uint16_t    measurement_unavailable: 1;
    uint16_t    questionable_measurement_detected: 1;
    uint16_t    invalid_measurement_detected: 1;
} T_PLXS_MEASUREMENT_STATE;
/** @brief device and sensor status bits*/
typedef  struct
{
    uint8_t  extended_display_update_ongoing: 1;
    uint8_t  equipment_malfunction_detected: 1;
    uint8_t  signal_processing_irregularity_detected: 1;
    uint8_t  inadequate_signal_detected: 1;
    uint8_t  poor_signal_detected: 1;
    uint8_t  low_perfusion_detected: 1;
    uint8_t  erratic_signal_detected: 1;
    uint8_t  nonPulsatile_signal_detected: 1;
    uint8_t  questionable_pulse_detected: 1;
    uint8_t  signal_analysis_ongoing: 1;
    uint8_t  sensor_interference_detected: 1;
    uint8_t  sensor_unconnected_to_user: 1;
    uint8_t  unknown_sensor_connected: 1;
    uint8_t  sensor_displaced: 1;
    uint8_t  sensor_malfunctioning: 1;
    uint8_t  sensor_disconnected: 1;
    uint8_t  rfu: 8;
} T_PLXS_DEVICE_AND_SENSOR_STATE;

/** @brief typedef spot check measurement value*/
typedef struct
{
    PLXS_SFLOAT spo2;
    PLXS_SFLOAT pr;
#if PLXS_SPOT_CHECK_MEASUREMENT_TIMESTAMP_SUPPORT
    PLXS_TIMESTAMP time;
#endif

#if PLXS_MEASUREMENT_STATE_SUPPORT
    T_PLXS_MEASUREMENT_STATE   measurement_status;
#endif

#if PLXS_DEVICE_AND_SENSOR_STATUS_SUPPORT
    T_PLXS_DEVICE_AND_SENSOR_STATE  device_and_sensor_status;
#endif

#if PLXS_PULSE_AMPLITUDE_INDEX_SUPPORT
    PLXS_SFLOAT   pulse_amplitude_index;
#endif
} T_PLXS_SPOT_CHECK_MEASUREMENT_VALUE;

/** @brief typedef spo2PR*/
typedef  struct
{
    PLXS_SFLOAT spo2;
    PLXS_SFLOAT pr;
} T_SPO2PR;

/**@brief typedef continuous measurement value*/
typedef struct
{
    T_SPO2PR    spo2pr_normal;

#if PLXS_CONTINUOUS_MEASUREMENT_SPO2PR_FAST_SUPPORT
    T_SPO2PR    spo2pr_fast;
#endif

#if PLXS_CONTINUOUS_MEASUREMENT_SPO2PR_SLOW_SUPPORT
    T_SPO2PR    spo2pr_slow;
#endif

#if PLXS_MEASUREMENT_STATE_SUPPORT
    T_PLXS_MEASUREMENT_STATE   measurement_status;
#endif

#if PLXS_DEVICE_AND_SENSOR_STATUS_SUPPORT
    T_PLXS_DEVICE_AND_SENSOR_STATE  device_and_sensor_status;
#endif

#if PLXS_PULSE_AMPLITUDE_INDEX_SUPPORT
    PLXS_SFLOAT   pulse_amplitude_index;
#endif
} T_PLXS_CONTINUOUS_MEASUREMENT_VALUE;

/** @brief typedef features value */
typedef struct
{
    uint16_t     supported_features;

#if PLXS_MEASUREMENT_STATE_SUPPORT
    uint16_t   measurement_status_support_bits;
#endif

#if PLXS_DEVICE_AND_SENSOR_STATUS_SUPPORT
    uint32_t  device_and_sensor_status_support_bits;
#endif
} T_PLXS_FEATURES_VALUE;

/** @} End of PLXS_Exported_Types */

/** @defgroup PLXS_Callback_Data PLXS Callback Data
  * @brief PLXS data struct for notification data to application.
  * @{
  */
typedef union
{
    uint8_t notify_indicate_index;
} T_PLXS_UPSTREAM_MSG_DATA;

typedef struct
{
    uint8_t                        conn_id;
    T_SERVICE_CALLBACK_TYPE        msg_type;
    T_PLXS_UPSTREAM_MSG_DATA       msg_data;
} T_PLXS_CALLBACK_DATA;
/** @} End of PLXS_Callback_Data */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup PLXS_Exported_Functions PLXS Exported Functions
  * @brief
  * @{
  */
/**
  * @brief  Add PLX service to the BLE stack database.
  *
  * @param[in] p_func  Callback when service attribute was read, write or cccd update.
  * @return Service id generated by the BLE stack: @ref T_SERVER_ID.
  * @retval 0xFF Operation failure.
  * @retval Others Service id assigned by stack.
  *
  * <b>Example usage</b>
  * \code{.c}
     void app_le_profile_init()
     {
         server_init(1);
         plxs_srv_id = plxs_add_service(app_handle_profile_message);
     }
  * \endcode
  */
T_SERVER_ID plxs_add_service(void *p_func);
/**
  * @brief  get plxs parameters such as spot check measurement flags,continuous measurement flags or features supports @ref T_PLXS_PARAM_TYPE
  *
  * @param[in] param_type  @ref T_PLXS_PARAM_TYPE
  * @param[in,out] p_value  get spot check measurement flags,continuous measurement flags or features supports
  * @return plxs_get_parameter result
  * @retval PLXS_APP_RESULT_SUCCESS Operation success.
  * @retval others Operation fail.
  *
  * <b>Example usage</b>
  * \code{.c}
     static T_USER_CMD_PARSE_RESULT cmd_plxs_feature_flags_set(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        T_PLXS_FEATURES_VALUE plxs_features;
        plxs_get_parameter(PLXS_PARAM_FEATURE_FLAGS, &plxs_features);
        if (p_parse_value->dw_param[0] == 1)
        {
            //both measurement status and device sensor status not support
            plxs_features.supported_features = plxs_features.supported_features & 0xFFFC;
        }
        ......
    }
  * \endcode
  */
T_PLXS_APP_RESULT  plxs_get_parameter(T_PLXS_PARAM_TYPE param_type, void *p_value);
/**
  * @brief  send plxs spot check measurement characteristic indication
  *
  * @param[in] conn_id  connection index
  * @param[in] Service_id  generated by the BLE stack: @ref T_SERVER_ID.
  * @param[in] p_spot_check_measurement_value @ref T_PLXS_SPOT_CHECK_MEASUREMENT_VALUE
  * @return plxs_spot_check_measurement_value_indicate result
  * @retval PLXS_APP_RESULT_SUCCESS Operation success.
  * @retval others Operation fail.
  *
  * <b>Example usage</b>
  * \code{.c}
     void test_plxs(void)
     {
        T_PLXS_SPOT_CHECK_MEASUREMENT_VALUE  plxs_spot_check_measure_value;
        ......//plxs_spot_check_measure_value initialization
        bool result = plxs_spot_check_measurement_value_indicate(conn_id,plxs_srv_id,&plxs_spot_check_measure_value);
        if(result == PLXS_APP_RESULT_SUCCESS)
        {
        }
     }
  * \endcode
  */
#if PLXS_SPOT_CHECK_MEASUREMENT_SUPPORT
T_PLXS_APP_RESULT plxs_spot_check_measurement_value_indicate(uint8_t conn_id, uint8_t service_id,
                                                             T_PLXS_SPOT_CHECK_MEASUREMENT_VALUE *p_spot_check_measurement_value);
#endif
/**
  * @brief  send plxs continuous measurement characteristic notification
  *
  * @param[in] conn_id  connection index
  * @param[in] Service_id  generated by the BLE stack: @ref T_SERVER_ID.
  * @param[in] plxs_continuous_measurement_value @ref T_PLXS_CONTINUOUS_MEASUREMENT_VALUE
  * @return plxs_continuous_measurement_value_notify result
  * @retval PLXS_APP_RESULT_SUCCESS Operation success.
  * @retval others Operation fail.
  *
  * <b>Example usage</b>
  * \code{.c}
     void test_plxs(void)
     {
        ......//plxs_continuous_measurement_value initialization
        bool result = plxs_continuous_measurement_value_notify( conn_id, plxs_srv_id,&plxs_continuous_measurement_value);
        if(result == PLXS_APP_RESULT_SUCCESS)
        {
            PROFILE_PRINT_INFO0("plxs_continuous_measurement_value_notify notify data send success!");
        }
     }
  * \endcode
  */
#if PLXS_CONTINUOUS_MEASUREMENT_SUPPORT
T_PLXS_APP_RESULT plxs_continuous_measurement_value_notify(uint8_t conn_id, T_SERVER_ID service_id,
                                                           T_PLXS_CONTINUOUS_MEASUREMENT_VALUE *p_plxs_continuous_measurement_value);
#endif

/**
  * @brief  clear flags if procedure fail or disconnect
  *
  * <b>Example usage</b>
  * \code{.c}
    void app_handle_conn_state_evt(uint8_t conn_id, T_GAP_CONN_STATE new_state, uint16_t disc_cause)
    {
        APP_PRINT_INFO4("app_handle_conn_state_evt: conn_id %d old_state %d new_state %d, disc_cause 0x%x",
                        conn_id, gap_conn_state, new_state, disc_cause);
        switch (new_state)
        {
        case GAP_CONN_STATE_DISCONNECTED:
            {
                if ((disc_cause != (HCI_ERR | HCI_ERR_REMOTE_USER_TERMINATE))&& (disc_cause != (HCI_ERR | HCI_ERR_LOCAL_HOST_TERMINATE)))
                {
                    APP_PRINT_ERROR1("app_handle_conn_state_evt: connection lost cause 0x%x", disc_cause);
                }
                plxs_flags_clear();//when disconnect clear plxs flags
                le_adv_start();
            }
            break;
        }
    }
  * \endcode
  */
void plxs_flags_clear(void);
/**
  * @brief  plxs check report data send procedure to enable flow control of notification or indication
  * @param[in] conn_id  connection index
  * @param[in] Service_id  generated by the BLE stack: @ref T_SERVER_ID.
  * @param[in] attribute_index  attribute_index
  *
  * <b>Example usage</b>
  * \code{.c}
    T_APP_RESULT app_handle_profile_message(T_SERVER_ID service_id, void *p_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        if (service_id == SERVICE_PROFILE_GENERAL_ID)
        {
            T_SERVER_APP_CB_DATA *p_para = (T_SERVER_APP_CB_DATA *)p_data;
            switch (p_para->eventId)
            {
                         ......
            case PROFILE_EVT_SEND_DATA_COMPLETE:
                         ......
                else if (p_para->event_data.send_data_result.service_id == plxs_srv_id)
                {
                    uint8_t conn_id  = p_para->event_data.send_data_result.conn_id;
                    if (p_para->event_data.send_data_result.cause == GAP_SUCCESS)
                    {
                        plxs_check_report_data_send_procedure(conn_id, plxs_srv_id,p_para->event_data.send_data_result.attrib_idx);
                    }
                    else
                    {
                        APP_PRINT_ERROR1("PROFILE_EVT_SEND_DATA_COMPLETE failed,cause %x", p_para->event_data.send_data_result.cause);
                    }
                }
                break;
            }
        }
    }
  * \endcode
  */
void plxs_check_report_data_send_procedure(uint8_t conn_id, T_SERVER_ID service_id,
                                           uint16_t attribute_index);
/** @} End of PLXS_Exported_Functions */
/** @} End of PLXS */
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* PULSE_OXIMETER_SERVICE_H */
