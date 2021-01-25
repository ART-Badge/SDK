/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     hts.h
  * @brief    Variables and interfaces for using Health Thermometer Service.
  * @details  Health Thermometer Service data structs and functions.
  * @author
  * @date     2017-09-20
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _HTS_SERVICE_DEF_H
#define _HTS_SERVICE_DEF_H

#ifdef __cplusplus
extern "C"  {
#endif      /* __cplusplus */

/* Add Includes here */
#include "stdint.h"
#include "profile_server.h"
#include "rtl876x.h"


/** @defgroup HTS Health Thermometer Service
  * @brief  Health Thermometer Service
   * @details

     The Health Thermometer Service exposes temperature and other data related to a thermometer used for healthcare applications.
     Application shall register Health Thermometer Service when initialization through @ref hts_add_service function.

     The Temperature Measurement characteristic is used to send a temperature measurement.
     Application can send a temperature measurement through @ref hts_measurement_value_indicate function.

     The Intermediate Temperature characteristic is used to send intermediate temperature values to a device
     for display purposes while the measurement is in progress.
     Application can send intermediate temperature values through @ref hts_intermediate_temperature_value_notify function.

     The Measurement Interval characteristic is used to enable and control the interval
     between consecutive temperature measurements.
     Application can send interval through @ref hts_measurement_interval_notify function.



  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup HTS_Exported_Macros HTS Exported Macros
  * @brief
  * @{
  */

#define HTS_READ_TEMPERATURE_TYPE_VALUE                     1
#define HTS_READ_MEASUREMENT_INTERVAL_VALUE                 2
#define HTS_READ_VALID_RANGE_VALUE                          3


#define HTS_WRITE_MEASUREMENT_INTERVAL_VALUE                1

/** @defgroup HTS_Notify_Indicate_Info HTS Notify Indicate Info
  * @brief  Parameter for enable or disable notification or indication.
  * @{
  */
#define HTS_NOTIFY_INDICATE_TEMPERATURE_MEASUREMENT_VALUE_ENABLE     1
#define HTS_NOTIFY_INDICATE_TEMPERATURE_MEASUREMENT_VALUE_DISABLE    2
#define HTS_NOTIFY_INDICATE_INTERMEDIATE_TEMPERATURE_VALUE_ENABLE    3
#define HTS_NOTIFY_INDICATE_INTERMEDIATE_TEMPERATURE_VALUE_DISABLE   4
#define HTS_NOTIFY_INDICATE_MEASUREMENT_INTERVAL_VALUE_ENABLE        5
#define HTS_NOTIFY_INDICATE_MEASUREMENT_INTERVAL_VALUE_DISABLE       6
/** @} */

/** @defgroup HTS_Measurement_Flag HTS Measurement Flag
  * @{
  */
#define HTS_FLAG_MEASUREMENT_UINT_BIT                       BIT0
#define HTS_FLAG_MEASUREMENT_TIMESTAMP_PRESENT_BIT          BIT1
#define HTS_FLAG_MEASUREMENT_TYPE_PRESNET_BIT               BIT2
/** @} */

/** End of HTS_Exported_Macros
* @}
*/

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup HTS_Exported_Types HTS Exported Types
  * @brief
  * @{
  */

/**
*  @brief  Health Thermometer parameter type
*/
typedef enum
{
    HTS_PARAM_MEASUREMENT_TEMPPARAM_FLAG = 1,
    HTS_PARAM_MEASUREMENT_TEMPERATUER_VALUE,
    HTS_PARAM_MEASUREMENT_TIME_STAMP,
    HTS_PARAM_MEASUREMENT_TEMPERATURE_TYPE,
    HTS_PARAM_MEASUREMENT_INTERVAL,
    HTS_PARAM_VALID_RANGE
} T_HTS_PARAM_TYPE;

/**
*  @brief  Health Thermometer temperature type
*/
typedef enum
{
    HTS_TEMPERATURE_TYPE_ARMPIT = 1,
    HTS_TEMPERATURE_TYPE_BODY,
    HTS_TEMPERATURE_TYPE_EAR,
    HTS_TEMPERATURE_TYPE_FINGER,
    HTS_TEMPERATURE_TYPE_GASTRO_INTESTINAL_TRACT,
    HTS_TEMPERATURE_TYPE_MOUTH,
    HTS_TEMPERATURE_TYPE_RECTUM,
    HTS_TEMPERATURE_TYPE_TOE,
    HTS_TEMPERATURE_TYPE_TYMPANUM,
    HTS_TEMPERATURE_TYPE_RESERVED,
} T_HTS_TEMPERATURE_TYPE;

/**
*  @brief  Health Thermometer Measurement Value Flag
*/
typedef struct
{
    uint8_t temp_value_units_bit: 1;
    uint8_t temp_time_stamp_present_bit: 1;
    uint8_t temp_type_present_bit: 1;
    uint8_t rfu: 5;
} T_HEALTH_THERMOMETER_MEASUREMENT_VALUE_FLAG;

typedef struct
{
    uint16_t year;
    uint8_t  month;
    uint8_t  day;
    uint8_t  hours;
    uint8_t  minutes;
    uint8_t  seconds;
} T_TIME_STAMP;

typedef struct
{
    uint16_t lower_value;
    uint16_t upper_value;
} T_HTS_MEASUREMENT_INTERVAL_VALID_RANGE;

typedef struct
{
    uint8_t write_index;
    uint16_t measurement_interval;
} T_HTS_WRITE_MSG;

/**
*  @brief  Health Thermometer Service upper stream message data
*/
typedef union
{
    uint8_t notification_indification_index;
    uint8_t read_value_index;
    T_HTS_WRITE_MSG write;
} T_HTS_UPSTREAM_MSG_DATA;

/**
*  @brief  Health Thermometer Service callback data
*/
typedef struct
{
    T_SERVICE_CALLBACK_TYPE     msg_type;
    T_HTS_UPSTREAM_MSG_DATA     msg_data;
} T_HTS_CALLBACK_DATA;

/** End of HTS_Exported_Types
* @}
*/

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup HTS_Exported_Functions HTS Exported Functions
  * @brief
  * @{
  */

/**
  * @brief       Add health thermometer service to the BLE stack database.
  *
  *
  * @param[in]   p_func  Callback when service attribute was read, write or cccd update.
  * @return Service id generated by the BLE stack: @ref T_SERVER_ID.
  * @retval 0xFF Operation failure.
  * @retval others Service id assigned by stack.
  *
  * <b>Example usage</b>
  * \code{.c}
     void profile_init()
     {
         server_init(1);
         hts_id = hts_add_service(app_handle_profile_message);
     }
  * \endcode
  */
uint8_t hts_add_service(void *p_func);


/**
 * @brief       Set a health thermometer service parameter.
 *
 *              NOTE: You can call this function with a health thermometer service parameter type and it will set the
 *                      health thermometer service parameter.  Health thermometer service parameters are defined
 *                      in @ref T_HTS_PARAM_TYPE.
 *                      If the "len" field sets to the size of a "uint16_t" ,the
 *                      "p_value" field must point to a data with type of "uint16_t".
 *
 * @param[in]   param_type   Health thermometer service parameter type: @ref T_HTS_PARAM_TYPE
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
        uint8_t hts_flag = HTS_FLAG_MEASUREMENT_UINT_BIT | HTS_FLAG_MEASUREMENT_TIMESTAMP_PRESENT_BIT |
                           HTS_FLAG_MEASUREMENT_TYPE_PRESNET_BIT;
        T_HTS_TEMPERATURE_TYPE temperature_type = HTS_TEMPERATURE_TYPE_ARMPIT;

        hts_measure_time_stamp.seconds += hts_measurement_interval;

        hts_set_parameter(HTS_PARAM_MEASUREMENT_TEMPPARAM_FLAG, sizeof(hts_flag), &hts_flag);
        hts_set_parameter(HTS_PARAM_MEASUREMENT_TIME_STAMP, sizeof(hts_measure_time_stamp),
                          &hts_measure_time_stamp);
        hts_set_parameter(HTS_PARAM_MEASUREMENT_TEMPERATURE_TYPE, 1, &temperature_type);
    }
 * \endcode
 */

bool hts_set_parameter(T_HTS_PARAM_TYPE param_type, uint8_t len, void *p_value);

/**
 * @brief       Send measurement value indication data .
 *              Application shall call @ref hts_set_parameter to set intermediate temperature value first,
 *              and the call this api to send the notication value.
 *
 * @param[in]   conn_id  Connection id.
 * @param[in]   service_id  Service id.
 *
 * @return service id @ref T_SERVER_ID.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint8_t hts_flag = HTS_FLAG_MEASUREMENT_UINT_BIT | HTS_FLAG_MEASUREMENT_TIMESTAMP_PRESENT_BIT |
                           HTS_FLAG_MEASUREMENT_TYPE_PRESNET_BIT;
        T_HTS_TEMPERATURE_TYPE temperature_type = HTS_TEMPERATURE_TYPE_ARMPIT;

        hts_measure_time_stamp.seconds += hts_measurement_interval;

        hts_set_parameter(HTS_PARAM_MEASUREMENT_TEMPPARAM_FLAG, sizeof(hts_flag), &hts_flag);
        hts_set_parameter(HTS_PARAM_MEASUREMENT_TIME_STAMP, sizeof(hts_measure_time_stamp),
                          &hts_measure_time_stamp);
        hts_set_parameter(HTS_PARAM_MEASUREMENT_TEMPERATURE_TYPE, 1, &temperature_type);

        hts_measurement_value_indicate(p_parse_value->dw_param[0], hts_id);
    }
 * \endcode
 */
bool hts_measurement_value_indicate(uint8_t conn_id, uint8_t service_id);

/**
 * @brief       Send intermediate temperature notification data.
 *              Application shall call @ref hts_set_parameter to set intermediate temperature value first,
 *              and the call this api to send the notication value.
 *
 * @param[in]   conn_id  Connection id.
 * @param[in]   service_id  Service id.
 *
 * @return service id @ref T_SERVER_ID.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint8_t hts_flag = HTS_FLAG_MEASUREMENT_UINT_BIT
                           | HTS_FLAG_MEASUREMENT_TIMESTAMP_PRESENT_BIT
                           | HTS_FLAG_MEASUREMENT_TYPE_PRESNET_BIT;
        T_HTS_TEMPERATURE_TYPE temperature_type = HTS_TEMPERATURE_TYPE_ARMPIT;

        hts_set_parameter(HTS_PARAM_MEASUREMENT_TEMPPARAM_FLAG, sizeof(hts_flag), &hts_flag);
        hts_set_parameter(HTS_PARAM_MEASUREMENT_TIME_STAMP, sizeof(hts_measure_time_stamp),
                          &hts_measure_time_stamp);
        hts_set_parameter(HTS_PARAM_MEASUREMENT_TEMPERATURE_TYPE, 1, &temperature_type);

        hts_intermediate_temperature_value_notify(p_parse_value->dw_param[0], hts_id);

        return RESULT_SUCESS;

    }
 * \endcode
 */
bool hts_intermediate_temperature_value_notify(uint8_t conn_id, uint8_t service_id);


/**
 * @brief       Send measurement interval notification data.
 *
 *
 * @param[in]   conn_id     Connection id.
 * @param[in]   service_id  Service id.
 * @param[in]   seconds     Measurement interval.
 *
 * @return service id @ref T_SERVER_ID.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint16_t interval = 90;
        hts_measurement_interval_notify(p_parse_value->dw_param[0], hts_id, interval);
    }
 * \endcode
 */
bool hts_measurement_interval_notify(uint8_t conn_id, uint8_t service_id, uint16_t seconds);

/** @} End of HTS_Exported_Functions */

/** @} End of HTS */


#ifdef __cplusplus
}
#endif

#endif // _HTS_SERVICE_DEF_H

