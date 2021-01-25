/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     bls_client.h
  * @brief    Head file for using BLS Client.
  * @details  blood pressure service client data structs and external functions declaration.
  * @author   Herry
  * @date     2017-12-05
  * @version  v1.0
  * *************************************************************************************
  */


#ifndef _BLS_CLIENT_H_
#define _BLS_CLIENT_H_

#ifdef  __cplusplus
extern "C" {
#endif

/* Add Includes here */
#include <stdint.h>
#include <stdbool.h>
#include <bls_define.h>
#include <profile_client.h>



/** @defgroup BLS_CLIENT Blood pressure Service Client
  * @brief BLS client
  * @details
     Application shall register bls client when initialization through @ref bls_add_client function.

     Application can start discovery blood pressure service through @ref bls_start_discovery function.

     Application can read blood pressure feature characteristic value through @ref bls_read_blp_feature function.

     Application can config the notification flag and the indication flag through @ref bls_write_notify_indicate.

     Application shall handle callback function registered by bls_add_client.
  * \code{.c}
    T_APP_RESULT app_client_callback(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data)
    {
        T_APP_RESULT  result = APP_RESULT_SUCCESS;
        if (client_id == bls_cl_id)
        {
            T_BLS_CLIENT_CB_DATA *p_bls_cb_data = (T_BLS_CLIENT_CB_DATA *)p_data;
            switch (p_bls_cb_data->cb_type)
            {
            case BLS_CLIENT_CB_TYPE_DISC_STATE:
                switch (p_bls_cb_data->cb_content.disc_state)
                {
                case DISC_BLS_DONE:
                ......
        }
    }
  * \endcode
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @addtogroup BLS_CLIENT_Exported_Macros BLS Client Exported Macros
  * @brief
  * @{
  */

/** @brief  Define links number. range: 0-4 */
#define BLS_MAX_LINKS  4

/** End of BLS_CLIENT_Exported_Macros
* @}
*/


/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup BLS_CLIENT__Exported_Types BLS Client Exported Types
  * @brief
  * @{
  */

/** @brief BLS client handle type*/
typedef enum
{
    HDL_BLS_SRV_START,                        //!< start handle of blood pressure service
    HDL_BLS_SRV_END,                          //!< end handle of blood pressure service
    HDL_BLS_BLP_MEASUREMENT,                  //!< blood pressure measurement characteristic value handle
    HDL_BLS_BLP_MEASUREMENT_CCCD,             //!< blood pressure measurement characteristic CCCD handle
    HDL_BLS_INTERMEDIATE_CUFF_PRESSURE,       //!< intermediate cuff pressure characteristic value handle
    HDL_BLS_INTERMEDIATE_CUFF_PRESSURE_CCCD,  //!< intermediate cuff pressure characteristic CCCD handle
    HDL_BLS_BLP_FEATURE,                      //!< blood pressure feature characteristic value handle
    HDL_BLS_CACHE_LEN                         //!< handle cache length
} T_BLS_HANDLE_TYPE;

/** @brief BLS client discovery state*/
typedef enum
{
    DISC_BLS_IDLE,
    DISC_BLS_START,
    DISC_BLS_DONE,
    DISC_BLS_FAILED
} T_BLS_DISC_STATE;

typedef struct
{
    T_BLOOD_PRESSURE_MEASURMENT blood_presure_mesaurment;
} T_BLS_NOTIFY_INDICATE_DATA;

/** @brief BLS client write type*/
typedef enum
{
    BLS_WRITE_NOTIFY_INTER_CUFF_PRESS_ENABLE,
    BLS_WRITE_NOTIFY_INTER_CUFF_PRESS_DISABLE,
    BLS_WRITE_INDICATE_BLOOD_PRESS_MEAS_ENABLE,
    BLS_WRITE_INDICATE_BLOOD_PRESS_MEAS_DISABLE
} T_BLS_WRTIE_TYPE;

/** @brief BLS client write result*/
typedef struct
{
    T_BLS_WRTIE_TYPE type;
    uint16_t cause;
} T_BLS_WRITE_RESULT;

/** @brief BLS client read data */
typedef union
{
    uint16_t blood_pressure_feature;
} T_BLS_READ_DATA;

/** @brief BLS client read type*/
typedef enum
{
    BLS_READ_BLP_FEATURE,
} T_BLS_READ_TYPE;

/** @brief BLS client read result*/
typedef struct
{
    T_BLS_READ_TYPE type;
    T_BLS_READ_DATA data;
    uint16_t cause;
} T_BLS_READ_RESULT;

/** @brief BLS client callback type*/
typedef enum
{
    BLS_CLIENT_CB_TYPE_DISC_STATE,          //!< Discovery procedure state, done or pending.
    BLS_CLIENT_CB_TYPE_READ_RESULT,         //!< Read request's result data, responsed from server.
    BLS_CLIENT_CB_TYPE_WRITE_RESULT,        //!< Write request result, success or fail.
    BLS_CLIENT_CB_TYPE_NIFY_INTER_CUFF_PRESS,
    //!< Intermediate cuff pressure notification data received from server.
    BLS_CLIENT_CB_TYPE_IND_BLOOD_PRESS_MEAS,
    //!< Blood pressure measurement indication data received from server.
    BLS_CLIENT_CB_TYPE_INVALID              //!< Invalid callback type, no practical usage.
} T_BLS_CLIENT_CB_TYPE;

/** @brief BLS client callback content*/
typedef union
{
    T_BLS_DISC_STATE disc_state;
    T_BLS_READ_RESULT read_result;
    T_BLS_NOTIFY_INDICATE_DATA blp_meas_notify_indicate_data;
    T_BLS_WRITE_RESULT write_result;
} T_BLS_CLIENT_CB_CONTENT;

/** @brief BLS client callback data*/
typedef struct
{
    T_BLS_CLIENT_CB_TYPE     cb_type;
    T_BLS_CLIENT_CB_CONTENT  cb_content;
} T_BLS_CLIENT_CB_DATA;
/** End of BLS_CLIENT_Exported_Types*/

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup BLS_CLIENT_Exported_Functions BLS Client Exported Functions
  * @brief
  * @{
  */

/**
  * @brief      Add bls client to application.
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
        bls_client_id = bls_add_client(app_client_callback, APP_MAX_LINKS);
    }
  * \endcode
  */
T_CLIENT_ID bls_add_client(P_FUN_GENERAL_APP_CB app_cb, uint8_t link_num);

/**
  * @brief  Used by application, to start the discovery procedure of blood pressure service.
  * @param[in]  conn_id connection ID.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    static T_USER_CMD_PARSE_RESULT cmd_blsdis(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        uint8_t conn_id = p_parse_value->dw_param[0];
        bool ret = bls_start_discovery(conn_id);
        ......
    }

    T_APP_RESULT app_handle_client_message(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data)
    {
        T_APP_RESULT  result = APP_RESULT_SUCCESS;
        ......
        else if (client_id == bls_cl_id)
        {
            APP_PRINT_INFO0("app_handle_client_message: bls_cl_id");

            T_BLS_CLIENT_CB_DATA *p_bls_cb_data = (T_BLS_CLIENT_CB_DATA *)p_data;
            switch (p_bls_cb_data->cb_type)
            {
            case BLS_CLIENT_CB_TYPE_DISC_STATE:
                if (p_bls_cb_data->cb_content.disc_state == DISC_STATE_SRV_DONE)
                {
                    APP_PRINT_INFO0("app_handle_client_message: discovery service procedure done.");
                }
                else
                {
                    APP_PRINT_INFO0("app_handle_client_message: discovery state send to application directly.");
                }
            break;
            ......
            }
        ......
        }
  * \endcode
  */
bool bls_start_discovery(uint8_t conn_id);

/**
  * @brief  Used by application, to read blood pressure feature.
  * @param[in]  conn_id connection ID.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    static T_USER_CMD_PARSE_RESULT cmd_blsread(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        uint8_t conn_id = p_parse_value->dw_param[0];
        bool ret = false;
        ret = bls_read_blp_feature(conn_id);
        ......
    }

    T_APP_RESULT app_handle_client_message(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data)
    {
        T_APP_RESULT  result = APP_RESULT_SUCCESS;
        ......
        else if (client_id == bls_cl_id)
        {
            APP_PRINT_INFO0("app_handle_client_message: bls_cl_id");

            T_BLS_CLIENT_CB_DATA *p_bls_cb_data = (T_BLS_CLIENT_CB_DATA *)p_data;
            switch (p_bls_cb_data->cb_type)
            {
            case BLS_CLIENT_CB_TYPE_READ_RESULT:
                if (p_bls_cb_data->cb_content.read_result.cause == GAP_SUCCESS)
                {
                    APP_PRINT_INFO1("BLS_CLIENT_CB_TYPE_READ_RESULT blood pressure feature 0x%x",
                                p_bls_cb_data->cb_content.read_result.data.blood_pressure_feature);
                }
                break;
            ......
            }
        ......
        }
  * \endcode
  */
bool bls_read_blp_feature(uint8_t conn_id);

/**
  * @brief  Used by application, to set the notification flag and indication flag.
  * @param[in]  conn_id connection ID.
  * @param[in]  write_type Type of T_BLS_WRTIE_TYPE.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    static T_USER_CMD_PARSE_RESULT cmd_blswrite(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        uint8_t conn_id = p_parse_value->dw_param[0];
        uint8_t write_type = p_parse_value->dw_param[1];
        bool ret;
        ret = bls_write_notify_indicate(conn_id, (T_BLS_WRTIE_TYPE)write_type);
        ......
    }

    T_APP_RESULT app_handle_client_message(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data)
    {
        T_APP_RESULT  result = APP_RESULT_SUCCESS;
        ......
        else if (client_id == bls_cl_id)
        {
            APP_PRINT_INFO0("app_handle_client_message: bls_cl_id");

            T_BLS_CLIENT_CB_DATA *p_bls_cb_data = (T_BLS_CLIENT_CB_DATA *)p_data;
            switch (p_bls_cb_data->cb_type)
            {
          case BLS_CLIENT_CB_TYPE_WRITE_RESULT:
            switch (p_bls_cb_data->cb_content.write_result.type)
            {
            ......
            case BLS_WRITE_INDICATE_BLOOD_PRESS_MEAS_ENABLE:
                APP_PRINT_INFO1("BLS_WRITE_INDICATE_BLOOD_PRESS_MEAS_ENABLE: write result 0x%x",
                                p_bls_cb_data->cb_content.write_result.cause);
                break;
            case BLS_WRITE_INDICATE_BLOOD_PRESS_MEAS_DISABLE:
                APP_PRINT_INFO1("BLS_WRITE_INDICATE_BLOOD_PRESS_MEAS_DISABLE: write result 0x%x",
                                p_bls_cb_data->cb_content.write_result.cause);
                break;
            default:
                break;
            }
            break;

            case BLS_CLIENT_CB_TYPE_NOTIF_RESULT:
      #if BLS_INTERMEDIATE_CUFF_PRESSURE_SUPPORT
            case BLS_CLIENT_CB_TYPE_IND_RESULT:
      #endif
                {
                    T_BLOOD_PRESSURE_MEASURMENT blood_presure_mesaurment =
                        p_bls_cb_data->cb_content.blp_meas_notify_indicate_data.blood_presure_mesaurment;

                    if (p_bls_cb_data->cb_type == BLS_CLIENT_CB_TYPE_IND_RESULT)
                    {
                        APP_PRINT_INFO0("BLS_CLIENT_CB_TYPE_IND_RESULT");
                    }
                    else
                    {
                        APP_PRINT_INFO0("BLS_CLIENT_CB_TYPE_NOTIF_RESULT");
                    }

                    if (blood_presure_mesaurment.bp_meas_flag & BLS_FLAG_MEASUREMENT_UINT_BIT)
                    {
                        APP_PRINT_INFO5("blood pressure measurement bp_meas_flag 0x%x, bp_meas_value_unit %d, bp_meas_systolic_value %b, bp_meas_diastolic_value %b, bp_meas_diastolic_value %b",
                                        blood_presure_mesaurment.bp_meas_flag, BLS_MEASUREMENT_VALUE_UNITS_KPA,
                                        TRACE_BINARY(2, blood_presure_mesaurment.bp_meas_compound_value.bp_meas_systolic_value),
                                        TRACE_BINARY(2, blood_presure_mesaurment.bp_meas_compound_value.bp_meas_diastolic_value),
                                        TRACE_BINARY(2, blood_presure_mesaurment.bp_meas_compound_value.bp_meas_map_value));
                    }
                    else
                    {
                        APP_PRINT_INFO5("blood pressure measurement bp_meas_flag 0x%x, bp_meas_value_unit %d, bp_meas_systolic_value %b, bp_meas_diastolic_value %b, bp_meas_diastolic_value %b",
                                        blood_presure_mesaurment.bp_meas_flag, BLS_MEASUREMENT_VALUE_UNITS_MMHG,
                                        TRACE_BINARY(2, blood_presure_mesaurment.bp_meas_compound_value.bp_meas_systolic_value),
                                        TRACE_BINARY(2, blood_presure_mesaurment.bp_meas_compound_value.bp_meas_diastolic_value),
                                        TRACE_BINARY(2, blood_presure_mesaurment.bp_meas_compound_value.bp_meas_map_value));
                    }

                    if (blood_presure_mesaurment.bp_meas_flag & BLS_FLAG_MEASUREMENT_TIMESTAMP_PRESENT_BIT)
                    {
                        APP_PRINT_INFO6("blood pressure measurement year %d, month %d, day %d, hours %d, minutes %d, seconds %d",
                                        blood_presure_mesaurment.time_stamp.year, blood_presure_mesaurment.time_stamp.month,
                                        blood_presure_mesaurment.time_stamp.day, blood_presure_mesaurment.time_stamp.hours,
                                        blood_presure_mesaurment.time_stamp.minutes, blood_presure_mesaurment.time_stamp.seconds);
                    }

                    if (blood_presure_mesaurment.bp_meas_flag & BLS_FLAG_MEASUREMENT_PULSE_RATE_BIT)
                    {
                        APP_PRINT_INFO1("blood pressure measurement bp_meas_pulse_rate %b",
                                        TRACE_BINARY(2, blood_presure_mesaurment.bp_meas_pulse_rate));
                    }

                    if (blood_presure_mesaurment.bp_meas_flag & BLS_FLAG_MEASUREMENT_USER_ID_BIT)
                    {
                        APP_PRINT_INFO1("blood pressure measurement bp_meas_user_id 0x%x",
                                        blood_presure_mesaurment.bp_meas_user_id);
                    }

                    if (blood_presure_mesaurment.bp_meas_flag & BLS_FLAG_MEASUREMENT_STATUS_BIT)
                    {
                        APP_PRINT_INFO1("blood pressure measurement bp_meas_status 0x%x",
                                        blood_presure_mesaurment.bp_meas_status);
                    }
                    break;
                }

            default:
                break;
            }
        }
        ......
  * \endcode
  */
bool bls_write_notify_indicate(uint8_t conn_id, T_BLS_WRTIE_TYPE write_type);

/** @} End of BLS_CLIENT_Exported_Functions */

/** @} End of BLS_CLIENT */

#ifdef  __cplusplus
}
#endif

#endif

