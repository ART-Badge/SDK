/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     wss.h
  * @brief    Head file for using weight scale service.
  * @details  Weight scale data types and external functions declaration.
  * @author   Vera
  * @date
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _WSS_H_
#define _WSS_H_

#ifdef __cplusplus
extern "C"  {
#endif      /* __cplusplus */


/* Add Includes here */
#include "stdint.h"
#include "profile_server.h"



/** @defgroup WSS Weight Scale Service
  * @brief Weight Scale Service
   * @details

    The Weight Scale (WS) Service exposes weight and related data from a weight scale (Server) intended for
    consumer healthcare as well as sports/fitness applications.

    Weight Scale Feature and Weight Measurement are exposed in the Weight Scale Service.
    Only one instance of each characteristic is permitted within this service.
    The Weight Scale Feature characteristic shall be used to describe the supported features of the Server.
    When read, the Weight Scale Feature characteristic returns a value that is used by a Client to determine
    the supported features of the Server.
    The Weight Measurement characteristic is used to send weight-related data to the Client.
    Included in the characteristic value are a Flags field (for showing the presence of optional fields and
    measurement units), a Weight field, and depending upon the contents of the Flags field, may include one or more optional fields.

    Application shall register weight scale service when initialization through @ref wss_add_service function.

    Application can set the weight scale service parameters through @ref wss_set_parameter function.

    Application can send the weight-related data of WSS to the client through @ref wss_measurement_indicate function.

  * @{
  */


/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup WSS_Exported_Macros WSS Exported Macros
  * @brief
  * @{
  */

/**
*  @brief Weight scale service parameter type
*/
typedef enum
{
    WSS_PARAM_MEASUREMENT_WEIGHTPARAM_FLAG = 0x01,
    WSS_PARAM_MEASUREMENT_WEIGHT_VALUE,
    WSS_PARAM_MEASUREMENT_TIME_STAMP,
    WSS_PARAM_MEASUREMENT_USERID,
    WSS_PARAM_MEASUREMENT_BMI,
    WSS_PARAM_MEASUREMENT_HEIGHT_VALUE,
    WSS_PARAM_FEATURE_READ_CHAR_VAL,
} T_WSS_PARAM_TYPE;


/** @defgroup WSS_Measurement_Flag WSS Measurement Flag
  * @{
  */
#define WSS_FLAG_MEASUREMENT_UINT_BIT                       BIT0
#define WSS_FLAG_MEASUREMENT_TIMESTAMP_PRESENT_BIT          BIT1
#define WSS_FLAG_MEASUREMENT_USERID_PRESNET_BIT             BIT2
#define WSS_FLAG_MEASUREMENT_BMI_PRESNET_BIT                BIT3

/** @} */



/** @defgroup WSS_Service_Read_Info WSS Service Read Info
  * @brief  Parameter for reading characteristic value.
  * @{
  */
#define WEIGHT_SCALE_READ_FEATURE                   1
/** @} End of WSS_Service_Read_Info*/



/** @defgroup WSS_Service_Notify_Indicate_Info WSS Service Notify Indicate Info
  * @brief  Parameter for enable or disable notification or indication.
  * @{
  */
#define WSS_INDICATE_WEIGHT_MEASUREMENT_ENABLE     1
#define WSS_INDICATE_WEIGHT_MEASUREMENT_DISABLE    2
/** @} */

/** @} End of WSS_Exported_Macros */


/*============================================================================*
 *                         Types
 *============================================================================*/

/** @defgroup WSS_Service_Exported_Types WSS Service Exported Types
  * @brief
  * @{
  */

/** @defgroup T_WSS_UPSTREAM_MSG_DATA TWS UPSTREAM MSG DATA
  * @brief Weight scale service callback message content.
  * @{
  */
typedef union
{
    uint8_t notification_indication_index; //!< ref: @ref WSS_Service_Notify_Indicate_Info
    uint8_t read_value_index; //!< ref: @ref WSS_Service_Read_Info
} T_WSS_UPSTREAM_MSG_DATA;
/** @} End of T_WSS_UPSTREAM_MSG_DATA */

/** @defgroup T_WSS_CALLBACK_DATA TWS CALLBACK DATA
  * @brief Weight scale service data to inform application.
  * @{
  */
typedef struct
{
    T_SERVICE_CALLBACK_TYPE     msg_type;
    T_WSS_UPSTREAM_MSG_DATA    msg_data;
} T_WSS_CALLBACK_DATA;
/** @} End of T_WSS_CALLBACK_DATA */


/** @defgroup T_WEIGHT_SCALE_FEATURE_VALUE WEIGHT SCALE FEATURE VALUE
  * @brief Weight Scale Feature Value.
  * @{
  */
typedef struct
{
    uint32_t time_stamp_supported: 1;
    uint32_t multiple_users_supported: 1;
    uint32_t bmi_supported: 1;
    uint32_t weight_measurement_resolution: 4;
    uint32_t height_measurement_resolution: 3;
    uint32_t rfu: 22;
} T_WEIGHT_SCALE_FEATURE_VALUE;
/** @} End of T_WEIGHT_SCALE_FEATURE_VALUE */

/** @defgroup T_WEIGHT_MEASUREMENT_VALUE_FLAG WEIGHT MEASUREMENT VALUE FLAG
  * @brief Weight Measurement Value Flag.
  * @{
  */
typedef struct
{
    uint8_t measurement_units: 1;
    uint8_t time_stamp_present: 1;
    uint8_t user_id_present: 1;
    uint8_t bmi_and_height_present: 1;
    uint8_t rfu: 4;
} T_WEIGHT_MEASUREMENT_VALUE_FLAG;
/** @} End of T_WEIGHT_MEASUREMENT_VALUE_FLAG */

/** @defgroup T_WSS_TIME_STAMP WSS TIME STAMP
  * @brief Weight Scale Feature Time Stamp.
  * @{
  */
typedef struct
{
    uint16_t year;
    uint8_t  month;
    uint8_t  day;
    uint8_t  hours;
    uint8_t  minutes;
    uint8_t  seconds;
} T_WSS_TIME_STAMP;
/** @} End of T_WSS_TIME_STAMP */

/** @defgroup T_WSS_MEASUREMENT WSS MEASUREMENT
  * @brief Weight Measurement Value.
  * @{
  */
typedef struct
{
    T_WEIGHT_MEASUREMENT_VALUE_FLAG flag;
    uint16_t weight_si_value;
    uint16_t weight_imperial_value;
    T_WSS_TIME_STAMP time_stamp;
    uint8_t user_id;
    uint16_t bmi;
    uint16_t height_si_value;
    uint16_t height_imperial_value;
} T_WSS_MEASUREMENT;
/** @} End of T_WSS_MEASUREMENT */

/** @} End of WSS_Service_Exported_Types */


/*============================================================================*
 *                         Functions
 *============================================================================*/

/** @defgroup WSS_Service_Exported_Functions WSS Service Exported Functions
  * @brief
  * @{
  */

/**
  * @brief       Add Weight scale service to the BLE stack database.
  *
  *
  * @param[in]   p_func  Callback when service attribute was read, write or cccd update.
  * @return Service id generated by the BLE stack: @ref T_SERVER_ID.
  * @retval 0xFF Operation failure.
  * @retval Others Service id assigned by stack.
  *
  * <b>Example usage</b>
  * \code{.c}
     void profile_init()
     {
         server_init(1);
         wss_id = wss_add_service(app_handle_profile_message);
     }
  * \endcode
  */
T_SERVER_ID wss_add_service(void *p_func);

/**
 * @brief       Set a weight scale service parameter.
 *
 *              NOTE: You can call this function with a weight scale service parameter type and it will set the
 *                      weight scale service parameter.  Weight scale service parameters are defined in @ref T_WSS_PARAM_TYPE.
 *                      If the "len" field sets to the size of a "uint16_t" ,the
 *                      "p_value" field must point to a data with type of "uint16_t".
 *
 * @param[in]   param_type   Weight scale service parameter type: @ref T_WSS_PARAM_TYPE
 * @param[in]   len       Length of data to write
 * @param[in]   p_value Pointer to data to write.  This is dependent on
 *                      the parameter type and WILL be cast to the appropriate
 *                      data type (For example: if data type of param is uint16_t, p_value will be cast to
 *                      pointer of uint16_t).
 *
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint32_t weight_feature = 0x0000001f;
        wss_set_parameter(WSS_PARAM_FEATURE_READ_CHAR_VAL, 4, &weight_feature);
    }
 * \endcode
 */
bool wss_set_parameter(T_WSS_PARAM_TYPE param_type, uint8_t len, void *p_value);


/**
 * @brief       Send measurement indication data.
 *
 *
 * @param[in]   conn_id  Connection id.
 * @param[in]   service_id  Service id.
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint8_t wm_flag = WSS_FLAG_MEASUREMENT_UINT_BIT
                          | WSS_FLAG_MEASUREMENT_TIMESTAMP_PRESENT_BIT
                          | WSS_FLAG_MEASUREMENT_USERID_PRESNET_BIT
                          | WSS_FLAG_MEASUREMENT_BMI_PRESNET_BIT;

        wss_set_parameter(WSS_PARAM_MEASUREMENT_WEIGHTPARAM_FLAG, sizeof(wm_flag), &wm_flag);
        wss_set_parameter(WSS_PARAM_MEASUREMENT_TIME_STAMP, sizeof(wss_measure_time_stamp),
                          &wss_measure_time_stamp);
        wss_measurement_indicate(p_parse_value->dw_param[0], wss_id);
    }
 * \endcode
 */
bool wss_measurement_indicate(uint8_t conn_id, T_SERVER_ID service_id);
/** @} End of WSS_Service_Exported_Functions */

/** @} End of WSS */



#ifdef __cplusplus
}
#endif

#endif /* _WSS_H_ */
