/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     lns.h
  * @brief    Variables and interfaces for using Location And Navigation Service.
  * @details  Location And Navigation Service data structs and functions.
  * @author   ranhui
  * @date     2017-09-20
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _LNS_SERVICE_DEF_H
#define _LNS_SERVICE_DEF_H

#ifdef __cplusplus
extern "C"  {
#endif      /* __cplusplus */

/* Add Includes here */
#include "profile_server.h"

/** @defgroup LNS Location And Navigation Service
  * @brief  Location And Navigation service
  * @details

    The Location and Navigation Service (LN Service) exposes location and navigation-related data from a
    Location and Navigation sensor (Server) intended for outdoor activity applications.

    Application shall register LN Service when initialization through @ref lns_add_service function.

    The LN Feature characteristic shall be used to describe the supported features of the Server.
    Reserved for Future Use (RFU) bits in the LN Feature characteristic value shall be set to 0.

    The Location and Speed characteristic is used to send location and speed related data.

    Application can send location and speed related data through @ref lns_location_and_speed_value_notify function.

    The Position Quality characteristic is used to send position quality-related data.

    If the LN Control Point is supported, profiles utilizing this service are required to ensure that the Client configures the LN Control Point characteristic for indications (i.e., via the Client
    Characteristic Configuration descriptor) at the first connection.
    Support for this characteristic is mandatory if the Server supports any of the features requiring control point procedures.

    The Navigation characteristic is used to send navigation-related data.
    Application can send navigation-related data through @ref lns_navigation_value_notify function.

    Application can set the parameters of LNS through @ref lns_set_parameter function.


  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup LNS_Exported_Macros LNS Exported Macros
  * @brief
  * @{
  */


/** @defgroup LNS_LN_Feature LNS LN Feature
  * @{
  */
#define LN_FEATURE_INSTANTANEOUS_SPEED_SUPPORTED            BIT0
#define LN_FEATURE_TOTAL_DISTANCE_SUPPORTED                 BIT1
#define LN_FEATURE_LOCATION_SUPPORTED                       BIT2
#define LN_FEATURE_ELEVATION_SUPPORTED                      BIT3
#define LN_FEATURE_HEADING_SUPPORTED                        BIT4
#define LN_FEATURE_ROLLING_TIME_SUPPORTED                   BIT5
#define LN_FEATURE_UTC_TIME_SUPPORTED                       BIT6
#define LN_FEATURE_REMAINING_DISTANCE_SUPPORTED             BIT7
#define LN_FEATURE_REMAINING_VERTICAL_DISTANCE_SUPPORTED    BIT8
#define LN_FEATURE_ESTIMATED_TIME_OF_ARRIVAL_SUPPORTED      BIT9
#define LN_FEATURE_NUMBER_OF_BEACONS_IN_SOLUTION_SUPPORTED  BIT10
#define LN_FEATURE_NUMBER_OF_BEACONS_IN_VIEW_SUPPORTED      BIT11
#define LN_FEATURE_TIME_TO_FIRST_FIX_SUPPORTED              BIT12
#define LN_FEATURE_ESTIMATED_HORIZONTAL_POSITION_ERROR_SUPPORTED    BIT13
#define LN_FEATURE_ESTIMATED_VERTICAL_POSITION_ERROR_SUPPORTED      BIT14
#define LN_FEATURE_HORIZONTAL_DILUTION_OF_PRECISION_SUPPORTED       BIT15
#define LN_FEATURE_VERTICAL_DILUTION_OF_PRECISION_SUPPORTED         BIT16
#define LN_FEATURE_LOCATION_AND_SPEED_CHARACTERISTIC_CONTENT_MASKING_SUPPORTED  BIT17
#define LN_FEATURE_FIX_RATE_SETTING_SUPPORTED               BIT18
#define LN_FEATURE_ELEVATION_SETTING_SUPPORTED              BIT19
#define LN_FEATURE_POSITION_STATUS_SUPPORTED                BIT20
#define LN_FEATURE_RFU                                      BIT21
/** @} */

#define LNS_READ_POSITION_QUALITY_VALUE                     1

#define LNS_NOTIFY_INDICATE_LOCATION_AND_SPEED_ENABLE       1
#define LNS_NOTIFY_INDICATE_LOCATION_AND_SPEED_DISABLE      2
#define LNS_NOTIFY_INDICATE_CP_ENABLE                       3
#define LNS_NOTIFY_INDICATE_CP_DISABLE                      4
#define LNS_NOTIFY_INDICATE_NAVIGATION_ENABLE               5
#define LNS_NOTIFY_INDICATE_NAVIGATION_DISABLE              6

#define LNS_MAX_CTL_PNT_VALUE                               17

/** End of LNS_Exported_Macros
* @}
*/



/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup LNS_Exported_Types LNS Exported Types
  * @brief
  * @{
  */

typedef enum
{
    LNS_LAS_PARAM_INC_FLAG = 0x01,
    LNS_LAS_PARAM_INSTANTANEOUS_SPEED,
    LNS_LAS_PARAM_TOTAL_DISTANCE,
    LNS_LAS_PARAM_LOCATION_LATITUDE,
    LNS_LAS_PARAM_LOCATION_LONGITUDE,
    LNS_LAS_PARAM_ELEVATION,
    LNS_LAS_PARAM_HEADING,
    LNS_LAS_PARAM_ROLLING_TIME,
    LNS_LAS_PARAM_UTC_TIME,
    LNS_LAS_PARAM_POSITION_STATUS,
    LNS_LAS_PARAM_SPEED_AND_DISTANCE_FORMAT,
    LNS_LAS_PARAM_ELEVATION_SOURCE,
    LNS_LAS_PARAM_HEADING_SOURCE,

    LNS_NAVIGATION_PARAM_INC_FLAG = 0x11,
    LNS_NAVIGATION_PARAM_BEARING,
    LNS_NAVIGATION_PARAM_HEADING,
    LNS_NAVIGATION_PARAM_REMAINING_DISTANCE,
    LNS_NAVIGATION_PARAM_REMAINING_VERTICAL_DISTANCE,
    LNS_NAVIGATION_PARAM_ESTIMATED_TIME_OF_ARRIVAL,
    LNS_NAVIGATION_PARAM_POSITION_STATUS,
    LNS_NAVIGATION_PARAM_HEADING_SOURCE,
    LNS_NAVIGATION_PARAM_NAVIGATION_INDICATOR_TYPE,
    LNS_NAVIGATION_PARAM_WAYPOINT_REACHED,
    LNS_NAVIGATION_PARAM_DESTINATION_REACHED,

    LNS_PQ_PARAM_INC_FLAG = 0x20,
    LNS_PQ_PARAM_NUMBER_OF_BEACONS_IN_SOLUTION,
    LNS_PQ_PARAM_NUMBER_OF_BEACONS_IN_VIEW,
    LNS_PQ_PARAM_TIME_TO_FIRST_FIX,
    LNS_PQ_PARAM_EHPE,
    LNS_PQ_PARAM_EVPE,
    LNS_PQ_PARAM_HDOP,
    LNS_PQ_PARAM_VDOP,

    LNS_PARAM_LN_FEATURE_SUPPORT = 0x30,

    LNS_CP_PARA_NUMBER_OF_ROUTE = 0x41,
    LNS_CP_PARA_NAME_OF_ROUTE,

    LNS_PARAM_CTL_PNT_PROG_CLR = 0x51,
} T_LNS_PARAM_TYPE;

/* Add all public types here */
typedef struct
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} T_DATE_TIME;

/** Position Quality Value Flag */
typedef struct
{
    uint16_t number_of_beacons_in_solution_present: 1;
    uint16_t number_of_beacons_in_view_present: 1;
    uint16_t time_to_first_fix_present: 1;
    uint16_t ehpe_present: 1;
    uint16_t evpe_present: 1;
    uint16_t hdop_present: 1;
    uint16_t vdop_present: 1;
    uint16_t position_status: 2;
    uint16_t rfu: 7;
} T_POSITION_QUALITY_VALUE_FLAG;

/** Position Quality Value */
typedef struct
{
    T_POSITION_QUALITY_VALUE_FLAG flag;
    uint8_t number_of_beacons_in_solution;
    uint8_t number_of_beacons_in_view;
    uint16_t time_to_first_fix;
    uint32_t ehpe;
    uint32_t evpe;
    uint8_t hdop;
    uint8_t vdop;
} T_POSITION_QUALITY_VALUE;

typedef struct
{
    uint8_t location_and_speed_notify_enable: 1;
    uint8_t ln_cp_indicate_enable: 1;
    uint8_t navigation_enable: 1;
    uint8_t rfu: 5;
} T_LNS_NOTIFY_INDICATE_FLAG;

typedef struct
{
    uint16_t instantaneous_speed_present: 1;
    uint16_t total_distance_present: 1;
    uint16_t location_present: 1;
    uint16_t elevation_present: 1;
    uint16_t heading_present: 1;
    uint16_t rolling_time_present: 1;
    uint16_t utc_time_present: 1;
    uint16_t position_status: 2;
    uint16_t speed_and_distance_format: 1;
    uint16_t elevation_source: 2;
    uint16_t heading_source: 1;
    uint16_t rfu: 3;
} T_LOCATION_AND_SPEED_VALUE_FLAG;

/** Location and Speed Value */
typedef struct
{
    T_LOCATION_AND_SPEED_VALUE_FLAG flag;
    uint16_t instantaneous_speed;
    uint32_t total_distance; //uint24
    int32_t location_latitude; //sint32
    int32_t location_longitute;
    int32_t elevation;//sint24
    uint16_t heading;
    uint8_t rolling_time;
    T_DATE_TIME utc_time;
} LOCATION_AND_SPEED_VALUE;

/** Navigation Value */
typedef struct
{
    uint16_t remaining_distance_present: 1;
    uint16_t remaining_vertical_distance_present: 1;
    uint16_t estimated_time_of_arrival_present: 1;
    uint16_t position_status: 2;
    uint16_t heading_source: 1;
    uint16_t navigation_indicator_type: 1;
    uint16_t waypoint_reached: 1;
    uint16_t destination_reached: 1;
    uint16_t rfus: 7;
} T_NAVIGATION_VALUE_FLAG;

typedef struct
{
    T_NAVIGATION_VALUE_FLAG flag;
    uint16_t bearing;
    uint16_t heading;
    uint32_t reamining_distance;
    int32_t reamining_vertical_distance;
    T_DATE_TIME estimated_time_of_arrival;
} T_NAVIGATION_VALUE;

typedef enum
{
    LN_CP_OPCODE_RESERVED = 0,
    LN_CP_OPCODE_SET_CUMULATIVE_VALUE = 1,
    LN_CP_OPCODE_MASK_LOCATION_AND_SPEED_CHAR_CONTENT = 2,
    LN_CP_OPCODE_NAVIGATION_CONTROL = 3,
    LN_CP_OPCODE_REQUEST_NUMBER_OF_ROUTES = 4,
    LN_CP_OPCODE_REQUEST_NAME_OF_ROUTE = 5,
    LN_CP_OPCODE_SELECT_ROUTE = 6,
    LN_CP_OPCODE_SET_FIX_RATE = 7,
    LN_CP_OPCODE_SET_ELEVATION = 8,
    LN_CP_OPCODE_RESPONSE_CODE = 32
} T_LN_CP_OPCODE;


/**
 * @brief CSC Control Point data, variable length during connection, max can reach 17 bytes.
 *
 * CSC Control Point data used to store the Control Point Command recieved from the client.
*/
typedef struct
{
    uint8_t    cur_length;            /**<  length of current LNS Control Point data, . */
    uint8_t
    value[LNS_MAX_CTL_PNT_VALUE];            /**<  value of current LNS Control Point data, . */
} T_LNS_CONTROL_POINT;

/** @defgroup LNS_Callback_Data LNS Callback Data
  * @brief LNS data struct for notification data to application.
  * @{
  */
typedef union
{
    uint32_t cumulative_total_distance; //uint24
    uint16_t mask_location_and_speed;
    uint8_t  navigation_control;
    uint16_t  number_of_desired_route; //request_name_of_route
    uint16_t  select_route_desired_route_number;
    uint8_t  set_fix_rate;
    int32_t  set_elevation; //sint24
} T_LNS_CP_PARAMETER;

typedef struct
{
    T_LN_CP_OPCODE opcode;
    T_LNS_CP_PARAMETER cp_parameter;
} T_LNS_WRITE_MSG;

typedef union
{
    uint8_t notification_indification_index;
    uint8_t read_value_index;
    T_LNS_WRITE_MSG write;
} T_LNS_UPSTREAM_MSG_DATA;

/** LNS service data to inform application */
typedef struct
{
    T_SERVICE_CALLBACK_TYPE     msg_type;
    T_LNS_UPSTREAM_MSG_DATA    msg_data;
} T_LNS_CALLBACK_DATA;
/** @} */

/** End of LNS_Exported_Types
* @}
*/


/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup LNS_Exported_Functions LNS Exported Functions
  * @brief
  * @{
  */

/**
  * @brief       Add location and navigation service to the BLE stack database.
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
     lns_id = lns_add_service(app_handle_profile_message);
    }
  * \endcode
  */
uint8_t lns_add_service(void *p_func);

/**
 * @brief       Set a Location And Navigation service parameter.
 *
 *              NOTE: You can call this function with a Location And Navigation service parameter type and it will set the
 *                      Location And Navigation service parameter.  Location And Navigation service parameters are defined
 *                      in @ref T_LNS_PARAM_TYPE.
 *                      If the "len" field sets to the size of a "uint16_t" ,the
 *                      "p_value" field must point to a data with type of "uint16_t".
 *
 * @param[in]   param_type   Location And Navigation service parameter type: @ref T_LNS_PARAM_TYPE
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
        T_LOCATION_AND_SPEED_VALUE_FLAG flag;
        flag.instantaneous_speed_present = 1;
        flag.total_distance_present = 1;
        flag.location_present = 1;
        flag.elevation_present = 1;
        flag.heading_present = 1;
        flag.rolling_time_present = 1;
        flag.utc_time_present = 1;
        flag.position_status = 1;
        flag.speed_and_distance_format = 1;
        flag.elevation_source = 1;
        flag.heading_source = 1;
        flag.rfu = 0;

        lns_set_parameter(LNS_LAS_PARAM_INC_FLAG, 2, &flag);
    }
 * \endcode
 */
bool lns_set_parameter(T_LNS_PARAM_TYPE param_type, uint8_t len, void *p_value);

/**
 * @brief       Send location and speed value notification data.
 *              Application shall call @ref lns_set_parameter to set location and speed value first,
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
        bool op_result;
        T_LOCATION_AND_SPEED_VALUE_FLAG flag;
        flag.instantaneous_speed_present = 1;
        flag.total_distance_present = 1;
        flag.location_present = 1;
        flag.elevation_present = 1;
        flag.heading_present = 1;
        flag.rolling_time_present = 1;
        flag.utc_time_present = 1;
        flag.position_status = 1;
        flag.speed_and_distance_format = 1;
        flag.elevation_source = 1;
        flag.heading_source = 1;
        flag.rfu = 0;

        lns_set_parameter(LNS_LAS_PARAM_INC_FLAG, 2, &flag);

        op_result = lns_location_and_speed_value_notify(p_parse_value->dw_param[0], lns_id);
    }
 * \endcode
 */
bool lns_location_and_speed_value_notify(uint8_t conn_id, T_SERVER_ID service_id);

/**
 * @brief       Send navigation value notification data.
 *              Application shall call @ref lns_set_parameter to set navigation value first,
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
        bool op_result;

        T_NAVIGATION_VALUE_FLAG flag;
        flag.remaining_distance_present = 1;
        flag.remaining_vertical_distance_present = 1;
        flag.estimated_time_of_arrival_present = 1;
        flag.position_status = 1;
        flag.heading_source = 1;
        flag.navigation_indicator_type = 1;
        flag.waypoint_reached = 1;
        flag.destination_reached = 1;
        flag.rfus = 0;

        lns_set_parameter(LNS_NAVIGATION_PARAM_INC_FLAG, 2, &flag);

        op_result = lns_navigation_value_notify(p_parse_value->dw_param[0], lns_id);

    }
 * \endcode
 */
bool lns_navigation_value_notify(uint8_t conn_id, T_SERVER_ID service_id);

/** @} End of LNS_Exported_Functions */

/** @} End of LNS */

#ifdef __cplusplus
}
#endif

#endif // _LNS_SERVICE_DEF_H

