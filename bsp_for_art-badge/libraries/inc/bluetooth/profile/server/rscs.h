/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     rsc_service.h
  * @brief    Head file for using Running Speed and Cadence Service.
  * @details  RSC data structs and external functions declaration.
  * @author   ethan_su
  * @date     2017-10-10
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _RSCS_SERVICE_DEF_H
#define _RSCS_SERVICE_DEF_H

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

/* Add Includes here */
#include "profile_server.h"



/** @defgroup RSCS Running Speed and Cadence Service
  * @brief Running Speed and Cadence Service
  * @details

    The Running Speed and Cadence (RSC) Service exposes speed, cadence and other data related to fitness applications
    such as the stride length and the total distance the user has traveled while using the Running Speed and Cadence sensor (Server).

    Application shall register Running Speed and Cadence service when initialization through @ref rscs_add_service function.

    The RSC Measurement characteristic is used to send speed and cadence measurements. Included in the characteristic value are
    a Flags field (for showing the presence of optional fields and, if supported by the Server, whether the user is walking or running),
    an Instantaneous Speed field, an Instantaneous Cadence field, depending upon the contents of the Flags field, an Instantaneous
    Stride Length field and a Total Distance field.

    The Server measures the instantaneous speed at which the user is moving. The cadence represents the number of times per minute
    the foot with the sensor hits the ground. The stride length represents the distance between two successive contacts of the same foot
    to the ground and the total distance represents the distance the user has travelled with the sensor since the last reset of the total
    distance.

    Application can set a running speed and cadence service parameter through @ref rscs_set_parameter function.

    Application can get a running speed and cadence service parameter through @ref rscs_get_parameter function.

    After set service parameter, running speed and cadence service can send notification data to client through @ref rscs_meas_value_notify function.

    Running speed and cadence service can indicate result of calibration to client through @ref rscs_calib_resutl_indicate function.

  * @{
  */


/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup RSCS_Exported_Macros RSCS Exported Macros
  * @brief
  * @{
  */

/** @defgroup RSCS_Measurement_Flag_Bit_Mask RSC Measurement Flag Bit Mask
  * @brief  RSC measurement 'Flags' present bit mask.
  * @{
  */
#define RSCS_INC_STRIDE_LENGTH_MASK          (1)             /**<  Instantaneous Stride Length Present. */
#define RSCS_INC_TOTAL_DISTANCE_MASK         (1<<1)          /**< Total Distance Present. */
#define RSCS_INC_RUNNING_STATUS_MASK         (1<<2)          /**< Walking or Running Status bits. */
#define RSCS_INC_ALL_PRESENTS                (0x07)
/** @} */

/** @defgroup RSCS_Feature_Bit_Mask RSC Feature Bit Mask
  * @brief  RSC features bit mask.
  * @{
  */
#define RSCS_SUPPORT_STRIDE_LENGTH_MASK      (1)
#define RSCS_SUPPORT_TOTAL_DISTANCE_MASK     (1<<1)
#define RSCS_SUPPORT_RUNNING_STATUS_MASK     (1<<2)
#define RSCS_SUPPORT_CALIBRATE_MASK          (1<<3)
#define RSCS_SUPPORT_MULTI_SENSOR_MASK       (1<<4)
#define RSCS_ALL_FEATURE_SUPPORTED           (0x1F)
/** @} */

/** @defgroup RSCS_Sensor_Location_Bit_Mask RSC Sensor Location Bit Mask
  * @brief  RSC supported sensor location bit mask.
  * @{
  */
#define RSCS_SENS_LOC_OTHER_MASK             (1)
#define RSCS_SENS_LOC_TOP_OF_SHOE_MASK       (1<<1)
#define RSCS_SENS_LOC_IN_SHOE_MASK           (1<<2)
#define RSCS_SENS_LOC_HIP_MASK               (1<<3)
#define RSCS_SENS_LOC_FRONT_WHEEL_MASK       (1<<4)
#define RSCS_SENS_LOC_LEFT_CRANK_MASK        (1<<5)
#define RSCS_SENS_LOC_RIGHT_CRANK_MASK       (1<<6)
#define RSCS_SENS_LOC_LEFT_PEDAL_MASK        (1<<7)
#define RSCS_SENS_LOC_RIGHT_PEDAL_MASK       (1<<8)
#define RSCS_SENS_LOC_FRONT_HUB_MASK         (1<<9)
#define RSCS_SENS_LOC_REAR_DROPOUT_MASK      (1<<10)
#define RSCS_SENS_LOC_CHAINSTAY_MASK         (1<<11)
#define RSCS_SENS_LOC_REAR_WHEEL_MASK        (1<<12)
#define RSCS_SENS_LOC_REAR_HUB_MASK          (1<<13)
#define RSCS_SENS_LOC_CHEST_MASK             (1<<14)
#define RSCS_ALL_SENS_LOC_SUPPORTED          (0x7FFF)
/** @} */

/** @defgroup RSCS_Control_Point_OpCodes RSC Control Point OpCodes
  * @brief  Control Point OpCodes
  * @{
  */
#define RSCS_CP_OPCODE_RESERVED              0x00
#define RSCS_CP_OPCODE_SET_CUMULATIVE        0x01
#define RSCS_CP_OPCODE_START_CALIBRATION     0x02
#define RSCS_CP_OPCODE_UPDATE_SENS_LOC       0x03
#define RSCS_CP_OPCODE_REQ_SENS_LOC_LIST     0x04
#define RSCS_CP_OPCODE_RSP_CODE              0x10
/** @} */

/** @defgroup RSCS_Control_Point_Response_Codes RSC Control Point Response Codes
  * @brief  Control Point Response Codes
  * @{
  */
#define RSCS_CP_RSPCODE_RESERVED             0x00
#define RSCS_CP_RSPCODE_SUCCESS              0x01
#define RSCS_CP_RSPCODE_OPCODE_UNSUPPORT     0x02
#define RSCS_CP_RSPCODE_INVALID_PARAMETER    0x03
#define RSCS_CP_RSPCODE_OPERATION_FAILED     0x04
/** @} */

/** @defgroup RSCS_Error_Codes RSC Error Codes
  * @brief  Error codes defined in RSC spec
  * @{
  */
#define RSCS_ERR_PROC_ALREADY_IN_PROG        0x80
#define RSCS_ERR_CCCD_IMPROPERLY_CFG         0x81
/** @} */

/** @defgroup RSCS_Data_Length RSC Data Length
  * @brief  Max bytes of RSC Measurement data and RSC Control Point data
  * @{
  */
#define RSCS_MAX_MEASUREMENT_VALUE           10
#define RSCS_MAX_CTL_PNT_VALUE               18
/** @} */


/** @defgroup RSCS_Read_Info RSC Read Info
  * @brief  Parameter for reading characteristic value.
  * @{
  */
#define RSCS_READ_RSCS_FEATURE                           1
#define RSCS_READ_SENSOR_LOCATION                        2
/** @} */

/** @defgroup RSCS_Notify_Indicate_Info RSCS Notify Indicate Info
  * @brief  Parameter for enable or disable notification or indication.
  * @{
  */
#define RSCS_NOTIFY_INDICATE_MEASUREMENT_ENABLE          1
#define RSCS_NOTIFY_INDICATE_MEASUREMENT_DISABLE         2
#define RSCS_NOTIFY_INDICATE_SC_CP_ENABLE                3
#define RSCS_NOTIFY_INDICATE_SC_CP_DISABLE               4
/** @} */

/** End of RSCS_Exported_Macros
* @}
*/


/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup RSCS_Exported_Types RSCS Exported Types
  * @brief
  * @{
  */

/**
*  @brief RSC service parameter type
*/
typedef enum
{
    RSCS_PARAM_CSCS_FEATURE = 0x01,
    RSCS_PARAM_INC_FLAG,
    RSCS_PARAM_SPEED,
    RSCS_PARAM_CADENCE,
    RSCS_PARAM_STRIDE_LENGTH,
    RSCS_PARAM_TOTALDISTANCE,
    RSCS_PARAM_CTL_PNT_PROG_CLR,
    RSCS_PARAM_SENSOR_LOC,
} T_RSCS_PARAM_TYPE;


/** @defgroup RSCS_Sensor_Locations RSC Sensor Locations
  * @brief  All RSC sensor locations defined in spec
  * @{
  */
typedef enum
{
    RSCS_SENSOR_LOC_OTHER            = 0,
    RSCS_SENSOR_LOC_TOP_OF_SHOE      = 1,
    RSCS_SENSOR_LOC_IN_SHOE          = 2,
    RSCS_SENSOR_LOC_HIP              = 3,
    RSCS_SENSOR_LOC_FRONT_WHEEL      = 4,
    RSCS_SENSOR_LOC_LEFT_CRANK       = 5,
    RSCS_SENSOR_LOC_RIGHT_CRANK      = 6,
    RSCS_SENSOR_LOC_LEFT_PEDAL       = 7,
    RSCS_SENSOR_LOC_RIGHT_PEDAL      = 8,
    RSCS_SENSOR_LOC_FRONT_HUB        = 9,
    RSCS_SENSOR_LOC_REAR_DROPOUT     = 10,
    RSCS_SENSOR_LOC_CHAINSTAY        = 11,
    RSCS_SENSOR_LOC_REAL_WHEEL       = 12,
    RSCS_SENSOR_LOC_REAL_HUB         = 13,
    RSCS_SENSOR_LOC_CHEST            = 14,
    RSCS_SENSOR_LOC_MAX
} TRSCSensorLocation;
/** @} */

/** @defgroup RSCS_Measurement_Data RSC Measurement Data
  * @brief  RSC measurement data, variable length during connection, max can reach 10 bytes
  * @{
  */
typedef struct
{
    uint8_t    cur_length; /**<  length of current RSC measurement data. */
    uint8_t    value[RSCS_MAX_MEASUREMENT_VALUE]; /**<  value of current RSC measurement data. */
} T_RSCS_MEASUREMENT;
/** @} */

/** @defgroup RSCS_Control_Point_Data RSC Control Point Data
  * @brief  RSC Control Point data, variable length during connection, max can reach 18 bytes
  * @{
  */
typedef struct
{
    uint8_t    cur_length; /**<  length of current RSC Control Point data, . */
    uint8_t    value[RSCS_MAX_CTL_PNT_VALUE]; /**<  value of current RSC Control Point data, . */
} T_RSCS_CONTROL_POINT;
/** @} */


/** @defgroup RSCS_Callback_Data RSCS Callback Data
  * @brief RSC data struct for notification data to application.
  * @{
  */
typedef union
{
    uint8_t  sensor_location_value;
    uint32_t cumulative_value;
} T_RSCS_CP_PARAMETER;

typedef struct
{
    uint8_t opcode; //!< ref: @ref RSCS_Control_Point_OpCodes
    T_RSCS_CP_PARAMETER cp_parameter;
} T_RSCS_WRITE_MSG;

typedef union
{
    uint8_t notification_indification_index; //!< ref: @ref RSCS_Notify_Indicate_Info
    uint8_t read_value_index; //!< ref: @ref RSCS_Read_Info
    T_RSCS_WRITE_MSG write;
} T_RSCS_UPSTREAM_MSG_DATA;

/** RSC service data to inform application */
typedef struct
{
    T_SERVICE_CALLBACK_TYPE     msg_type;
    T_RSCS_UPSTREAM_MSG_DATA    msg_data;
} T_RSCS_CALLBACK_DATA;
/** @} */

/** End of RSCS_Exported_Types
* @}
*/

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup RSCS_Exported_Functions RSCS Exported Functions
 * @brief
 * @{
 */


/**
 * @brief       Add Running Speed and Cadence Service.
 *
 * @param[in]   p_func  Callback when service attribute was read/write.
 *
 * @return service id @ref T_SERVER_ID.
 * @retval 0xFF Operation failure.
 * @retval others Service id assigned by stack.
 *
 * <b>Example usage</b>
 * \code{.c}
   void profile_init()
   {
        server_init(1);
        rscs_id = rscs_add_service(app_handle_profile_message);
  }
 * \endcode
 */
T_SERVER_ID rscs_add_service(void *p_func);


/**
 * @brief       Set a Running Speed and Cadence Service parameter.
 *
 *              NOTE: You can call this function with a Running Speed and Cadence Service parameter type and it will set the
 *                      Running Speed and Cadence Service parameter.  Running Speed and Cadence Service parameters are defined
 *                      in @ref T_RSCS_PARAM_TYPE.
 *                      If the "len" field sets to the size of a "uint16_t" ,the
 *                      "p_value" field must point to a data with type of "uint16_t".
 *
 * @param[in]   param_type   Running Speed and Cadence Service parameter type: @ref T_RSCS_PARAM_TYPE
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
        uint32_t total_distance = p_parse_value->dw_param[0];
        bool op_result;

        op_result = rscs_set_parameter(RSCS_PARAM_TOTALDISTANCE, 4, (uint8_t*)&total_distance);

        if ( op_result )
        {
            rscs_get_parameter( RSCS_PARAM_TOTALDISTANCE, &total_distance );
            data_uart_print("set total_distance = %d\r\n", total_distance);
        }
        else
        {
            data_uart_print("rscs_set_parameter total_distance failed\r\n");
        }
    }
 * \endcode
 */
bool rscs_set_parameter(T_RSCS_PARAM_TYPE param_type, uint8_t len, void *p_value);

/**
 * @brief       Get a Running Speed and Cadence Service parameter.
 *
 *              NOTE: You can call this function with a Running Speed and Cadence Service parameter type and it will get the
 *                      parameter.  Running Speed and Cadence Service parameters are defined in @ref T_RSCS_PARAM_TYPE.
 *
 * @param[in]   param_type   Running Speed and Cadence Service parameter type: @ref T_RSCS_PARAM_TYPE
 * @param[in]   p_value Pointer to data to read.
 *
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint8_t bFlag;

        rscs_get_parameter(RSCS_PARAM_INC_FLAG, &bFlag);
    }
 * \endcode
 */
bool rscs_get_parameter(T_RSCS_PARAM_TYPE param_type, void *p_value);

/**
 * @brief       Send Running Speed and Cadence Service value notification data.
 *              Application shall call @ref rscs_set_parameter to set value first,
 *              and then call this api to send the notication value.
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
        bool op_result;
        uint8_t flag = RSCS_INC_ALL_PRESENTS;

        rscs_set_parameter(RSCS_PARAM_INC_FLAG, 1, &flag);

        op_result = rscs_meas_value_notify(p_parse_value->dw_param[0], rscs_id);

        if ( op_result )
        {
            data_uart_print("Notify RSC measurement value succeed\r\n");
        }
        else
        {
            data_uart_print("Notify RSC measurement value failed\r\n");
        }

    }
 * \endcode
 */
bool rscs_meas_value_notify(uint8_t conn_id, T_SERVER_ID service_id);

/**
 * @brief       Send Running Speed and Cadence Service value indication data,
 *              send indication of result of calibration to client.
 *
 * @param[in]   conn_id  Connection id.
 * @param[in]   service_id  Service id.
 * @param[in]   result calibration result.
 *
 * @return none;
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint8_t conn_id = p_parse_value->dw_param[0];
        uint8_t result = p_parse_value->dw_param[1];
        rscs_calib_resutl_indicate(conn_id, rscs_id, result);
        data_uart_print("cmd_rscs_calib_result_indicate result= %d\r\n", result);
    }
 * \endcode
 */
void rscs_calib_resutl_indicate(uint8_t conn_id, T_SERVER_ID service_id, bool result);


/** @} End of RSCS_Exported_Functions */


/** @} End of RSCS */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif  /* _RSCS_DEF_H */
