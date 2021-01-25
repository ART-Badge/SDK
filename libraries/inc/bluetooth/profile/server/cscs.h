/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     csc_service.h
  * @brief    Variables and interfaces for Cycling Speed and Cadence Service.
  * @details  CSC service data structs and functions.
  * @author   ethan_su
  * @date     2017-10-13
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _CSCS_SERVICE_DEF_H
#define _CSCS_SERVICE_DEF_H

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

/* Add Includes here */
#include "profile_server.h"
#include "cscs_config.h"


/** @defgroup CSCS Cycling Speed and Cadence Service
  * @brief  Cycling Speed and Cadence Service
   * @details

    The Cycling Speed and Cadence (CSC) Service exposes speed-related data and/or
    cadence-related data while using the Cycling Speed and Cadence sensor (Server).
    CSC Measurement, CSC Measurement Client Characteristic Configuration descriptor and CSC Feature characteristic are mandatory
    exposed in the Cycling Speed and Cadence Service.
    if the Multiple Sensor Locations feature is supported, CSC Feature will be mandatory, otherwise optional.
    if at least one SC Control Point procedure is supported, SC Control Point and SC Control Point Client
    Characteristic Configuration Descriptor will be mandatory, otherwise excluded.

    The CSC Measurement characteristic is used to send speed-related data and/or cadence-related data.
    The CSC Feature characteristic shall be used to describe the supported features of the Server.
    The Sensor Location characteristic of the device may be used to describe the physical location of the Server when correctly fitted.

    If the SC Control Point is supported, profiles utilizing this service are required to ensure that the Client configures the
    SC Control Point characteristic for indications (i.e. via the Client Characteristic Configuration descriptor) at the first connection.

    Application shall register Cycling Speed and Cadence service when initialization through @ref cscs_add_service function.

    Application can set the Cycling Speed and Cadence parameters through @ref cscs_set_parameter function.

    Application can send the measurement value through @ref cscs_meas_value_notify function.

  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup CSCS_Exported_Constants CSC Exported Macros
  * @brief
  * @{
  */

/** @defgroup CSCS_Optional_Features CSC Optional Features
  * @brief CSC Service optional features bit mask
  * @{
  */

/** @defgroup CSCS_Measurement_Flags CSC Measurement Flags
  * @brief CSC measurement 'Flags' present bit mask
  * @{
  */
#define CSCS_INC_WHEEL_REVOL_MASK            (1)
#define CSCS_INC_CRANK_REVOL_MASK            (1<<1)
#define CSCS_INC_ALL_PRESENTS                (0x03)
/** @} */

/** @defgroup CSCS_Features CSC Features
  * @brief CSC features bit mask
  * @{
  */
#define CSCS_SUPPORT_WHEEL_REVOL_MASK        (1)
#define CSCS_SUPPORT_CRANK_REVOL_MASK        (1<<1)
#define CSCS_SUPPORT_MULTI_SENSOR_MASK       (1<<2)
#define CSCS_ALL_FEATURE_SUPPORTED           (0x07)
/** @} */

/** @defgroup CSCS_Sensor_Location CSC Sensor Location
  * @brief CSC supported sensor location  bit mask
  * @{
  */
#define CSCS_SENS_LOC_OTHER_MASK             (1)
#define CSCS_SENS_LOC_TOP_OF_SHOE_MASK       (1<<1)
#define CSCS_SENS_LOC_IN_SHOE_MASK           (1<<2)
#define CSCS_SENS_LOC_HIP_MASK               (1<<3)
#define CSCS_SENS_LOC_FRONT_WHEEL_MASK       (1<<4)
#define CSCS_SENS_LOC_LEFT_CRANK_MASK        (1<<5)
#define CSCS_SENS_LOC_RIGHT_CRANK_MASK       (1<<6)
#define CSCS_SENS_LOC_LEFT_PEDAL_MASK        (1<<7)
#define CSCS_SENS_LOC_RIGHT_PEDAL_MASK       (1<<8)
#define CSCS_SENS_LOC_FRONT_HUB_MASK         (1<<9)
#define CSCS_SENS_LOC_REAR_DROPOUT_MASK      (1<<10)
#define CSCS_SENS_LOC_CHAINSTAY_MASK         (1<<11)
#define CSCS_SENS_LOC_REAR_WHEEL_MASK        (1<<12)
#define CSCS_SENS_LOC_REAR_HUB_MASK          (1<<13)
#define CSCS_SENS_LOC_CHEST_MASK             (1<<14)
#define CSCS_ALL_SENS_LOC_SUPPORTED          (0x7FFF)
/** @} */

/** @} End of CSCS_Optional_Features */

/**
*  @brief CSC service parameter type
*/
typedef enum
{
    CSCS_PARAM_CSCS_FEATURE = 0x01,
    CSCS_PARAM_INC_FLAG = 0x11,
    CSCS_PARAM_WHEEL_REVOL,
    CSCS_PARAM_WHEEL_EVT_TIME,
    CSCS_PARAM_CRANK_REVOL,
    CSCS_PARAM_CRANK_EVT_TIME,
    CSCS_PARAM_CTL_PNT_PROG_CLR = 0x17,
    CSCS_PARAM_SENSOR_LOC = 0x21,
} T_CSCS_PARAM_TYPE;


/** @defgroup CSCS_Upstream_Message CSC Upstream Message
  * @brief  Upstream message used to inform application.
  * @{
  */

/** @defgroup CSCS_Read_Info CSC Read Info
  * @brief  Parameter for reading characteristic value.
  * @{
  */
#define CSCS_READ_CSCS_FEATURE                                1
#define CSCS_READ_SENSOR_LOCATION                            2
/** @} */

/** @defgroup CSCS_Notify_Indicate_Info CSC Notify Indicate Info
  * @brief  Parameter for enable or disable notification or indication.
  * @{
  */
#define CSCS_NOTIFY_INDICATE_MEASUREMENT_ENABLE              1
#define CSCS_NOTIFY_INDICATE_MEASUREMENT_DISABLE             2
#define CSCS_NOTIFY_INDICATE_SC_CP_ENABLE                    3
#define CSCS_NOTIFY_INDICATE_SC_CP_DISABLE                   4
/** @} */

/** @} End of CSCS_Upstream_Message */

/** @defgroup CSCS_Control_Point CSC Control Point
  * @brief  Control Point
  * @{
  */

/** @defgroup CSCS_Control_Point_OpCodes CSC Control Point OpCodes
  * @brief  Control Point OpCodes
  * @{
  */
#define CSCS_CP_OPCODE_RESERVED              0x00
#define CSCS_CP_OPCODE_SET_CUMULATIVE        0x01
#define CSCS_CP_OPCODE_UPDATE_SENS_LOC       0x03
#define CSCS_CP_OPCODE_REQ_SENS_LOC_LIST     0x04
#define CSCS_CP_OPCODE_RSP_CODE              0x10
/** @} */

/** @defgroup CSCS_Control_Point_Response_Codes CSC Control Point Response Codes
  * @brief  Control Point Response Codes
  * @{
  */
#define CSCS_CP_RSPCODE_RESERVED             0x00
#define CSCS_CP_RSPCODE_SUCCESS              0x01
#define CSCS_CP_RSPCODE_OPCODE_UNSUPPORT     0x02
#define CSCS_CP_RSPCODE_INVALID_PARAMETER    0x03
#define CSCS_CP_RSPCODE_OPERATION_FAILED     0x04
/** @} */

///@cond
/** @brief  Judge CSC Control Point operation is available or not. */
#define CSCS_CTL_PNT_OPERATE_ACTIVE(x)                     \
    ( (x == CSCS_CP_OPCODE_SET_CUMULATIVE)  || \
      (x == CSCS_CP_OPCODE_UPDATE_SENS_LOC)  || \
      (x == CSCS_CP_OPCODE_REQ_SENS_LOC_LIST) || \
      (x == CSCS_CP_OPCODE_RSP_CODE) )
///@endcond

/** @} End of CSCS_Control_Point */

/**
 * @brief CSC sensor locations.
 *
 * All CSC sensor locations defined in spec.
*/
typedef enum
{
    CSCS_SENSOR_LOC_OTHER            = 0,
    CSCS_SENSOR_LOC_TOP_OF_SHOE      = 1,
    CSCS_SENSOR_LOC_IN_SHOE          = 2,
    CSCS_SENSOR_LOC_HIP              = 3,
    CSCS_SENSOR_LOC_FRONT_WHEEL      = 4,
    CSCS_SENSOR_LOC_LEFT_CRANK       = 5,
    CSCS_SENSOR_LOC_RIGHT_CRANK      = 6,
    CSCS_SENSOR_LOC_LEFT_PEDAL       = 7,
    CSCS_SENSOR_LOC_RIGHT_PEDAL      = 8,
    CSCS_SENSOR_LOC_FRONT_HUB        = 9,
    CSCS_SENSOR_LOC_REAR_DROPOUT     = 10,
    CSCS_SENSOR_LOC_CHAINSTAY        = 11,
    CSCS_SENSOR_LOC_REAL_WHEEL       = 12,
    CSCS_SENSOR_LOC_REAL_HUB         = 13,
    CSCS_SENSOR_LOC_CHEST            = 14,
    CSCS_SENSOR_LOC_MAX
} T_CSCS_SENSOR_LOCATION;

///@cond
/** @brief  Error codes defined in CSC service. */
#define CSCS_ERR_PROC_ALREADY_IN_PROG        0x80
#define CSCS_ERR_CCCD_IMPROPERLY_CFG         0x81

/** @brief  Max bytes of CSC Measurement data. */
#define CSCS_MAX_MEASUREMENT_VALUE           11
/** @brief  Max bytes of CSC Control Point data. */
#define CSCS_MAX_CTL_PNT_VALUE               18
///@endcond

/** @} End of CSCS_Exported_Constants */



/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup CSCS_Exported_Types CSC Exported Types
  * @brief
  * @{
  */

/* Add all public types here */
/** @defgroup CSCS_Callback_Data CSC Callback Data
  * @brief CSC data struct for notification data to application.
  * @{
  */
typedef union
{
    uint8_t  sensor_location_value;
    uint32_t cumulative_value;

} T_CSCS_CP_PARAMETER;

typedef struct
{
    uint8_t opcode; //!< ref: @ref CSCS_Control_Point_OpCodes
    T_CSCS_CP_PARAMETER cp_parameter;
} T_CSCS_WRITE_MSG;

typedef union
{
    uint8_t notification_indification_index; //!< ref: @ref CSCS_Notify_Indicate_Info
    uint8_t read_value_index; //!< ref: @ref CSCS_Read_Info
    T_CSCS_WRITE_MSG write;
} T_CSCS_UPSTREAM_MSG_DATA;

/** CSC service data to inform application */
typedef struct
{
    T_SERVICE_CALLBACK_TYPE     msg_type;
    T_CSCS_UPSTREAM_MSG_DATA    msg_data;
} T_CSCS_CALLBACK_DATA;
/** @} */

/** @brief CSC measurement data, variable length during connection, max can reach 11 bytes */
typedef struct
{
    uint8_t    cur_length;          /**<  length of current CSC measurement data. */
    uint8_t    value[CSCS_MAX_MEASUREMENT_VALUE];/**<  value of current CSC measurement data. */
} T_CSCS_MEASUREMENT;

/**
 * @brief CSC Control Point data, variable length during connection, max can reach 17 bytes.
 *
 * CSC Control Point data used to store the Control Point Command recieved from the client.
*/
typedef struct
{
    uint8_t    cur_length;            /**<  length of current CSC Control Point data, . */
    uint8_t
    value[CSCS_MAX_CTL_PNT_VALUE];            /**<  value of current CSC Control Point data, . */
} T_CSCS_CONTROL_POINT;
/** @} End of CSCS_Exported_Types */


/*============================================================================*
 *                         Functions
 *============================================================================*/

/** @defgroup CSCS_Exported_Functions CSC Exported Functions
  * @brief
  * @{
  */


/**
  * @brief       Add cycling speed and cadence service to the BLE stack database.
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
         cscs_id = cscs_add_service(app_handle_profile_message);
     }
  * \endcode
  */
T_SERVER_ID cscs_add_service(void *p_func);

/**
 * @brief       Set a cycling speed and cadence service parameter.
 *
 *              NOTE: You can call this function with a cycling speed and cadence service parameter type and it will set the
 *                      cycling speed and cadence service parameter. Cycling speed and cadence service parameters are defined in @ref T_CSCS_PARAM_TYPE.
 *                      If the "len" field sets to the size of a "uint16_t" ,the
 *                      "p_value" field must point to a data with type of "uint16_t".
 *
 * @param[in]   param_type   Cycling speed and cadence service parameter type: @ref T_CSCS_PARAM_TYPE
 * @param[in]   len       Length of data to write
 * @param[in]   p_value Pointer to data to write.  This is dependent on
 *                      the parameter type and will be cast to the appropriate
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
        cscs_set_parameter(CSCS_PARAM_CTL_PNT_PROG_CLR, 0, NULL);
    }
 * \endcode
 */
bool cscs_set_parameter(T_CSCS_PARAM_TYPE param_type,  uint8_t len, void *p_value);

/**
 * @brief   Get a cycling speed and cadence parameter.
 *
 *          NOTE: You can call this function with a cycling speed and cadence parameter type and it will get a
 *          gulcose parameter. Cycling speed and cadence service parameters are defined in @ref T_CSCS_PARAM_TYPE.
 *
 * @param[in]   param_type Cycling speed and cadence parameter type: @ref T_CSCS_PARAM_TYPE
 * @param[in,out]  p_value Pointer to the location to get the parameter value.  This is dependent on
 *                      the parameter type and will be cast to the appropriate
 *                      data type (For example: if data type of param is uint16_t, p_value will be cast to
 *                      pointer of uint16_t).
 *
 * @return Operation result.
 * @retval GAP_CAUSE_SUCCESS Operation success.
 * @retval GAP_CAUSE_INVALID_PARAM Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        int record_num;
        gls_get_parameter(GLS_PARAM_RECORD_NUM, &len, &record_num);
    }
 * \endcode
 */
bool cscs_get_parameter(T_CSCS_PARAM_TYPE param_type, void *p_value);


/**
 * @brief       Send measurement value notification data .
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
        bool op_result;
        uint8_t conn_id = p_parse_value->dw_param[0];
        cscs_set_parameter(CSCS_PARAM_WHEEL_REVOL, sizeof(cscs_cul_value), &cscs_cul_value);
        op_result = cscs_meas_value_notify(conn_id, cscs_id);
    }
 * \endcode
 */
bool cscs_meas_value_notify(uint8_t conn_id, T_SERVER_ID service_id);



/**
 * @brief       Send control point indication.
 *
 *
 * @param[in]   conn_id     Connection id.
 * @param[in]   service_id  Service id.
 * @param[in]   op_code     Op code.
 * @param[in]   rsp_code    Response code.
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 */
bool cscs_ctl_pnt_indicate(uint8_t conn_id, T_SERVER_ID service_id, uint8_t op_code,
                           uint8_t rsp_code);




/** @} End of CSCS_Exported_Functions */

/** @} End of CSC */


#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif  /* _CSCS_DEF_H */

