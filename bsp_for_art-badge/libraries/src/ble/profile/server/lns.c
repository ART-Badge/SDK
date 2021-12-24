/*********************************************************************************************************
*               Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     lns.c
* @brief    Location and Navigation Service source file.
* @details  Interfaces to access Location and Navigation Service.
* @author
* @date     2017-09-22
* @version  v0.1
*********************************************************************************************************
*/

#include "stdint.h"
#include "gatt.h"
#include <string.h>
#include "trace.h"
#include "lns.h"


/********************************************************************************************************
* local static variables defined here, only used in this source file.
********************************************************************************************************/

#define LNS_POSITION_QUALITY_CHAR_SUPPORT
#define LNS_LN_CONTROL_POINT_CHAR_SUPPORT
#define LNS_LN_NAVIGATION_CHAR_SUPPORT

#define LNS_CP_PARA_NAME_OF_ROUTE_MAX_LEN                   10

#define LNS_LN_FEATURE_VALUE_INDEX                          2//read
#define LNS_LN_LOCATION_AND_SPEED_VALUE_INDEX               4//notify
#define LNS_LN_POSITION_QUALITY_VALUE_INDEX                 7//read
#define LNS_LN_CP_VALUE_INDEX                               9 //write, indicate
#define LNS_LN_NAVIGATION_VALUE_INDEX                       12//notify

#define LNS_LN_LOCATION_AND_SPEED_CCCD_INDEX                5
#define LNS_LN_CP_CCCD_INDEX                                10
#define LNS_LN_NAVIGATION_CCCD_INDEX                        13

#define GATT_UUID_SERVICE_LOCATION_AND_NAVIGATION           0x1819
#define GATT_UUID_CHAR_LNS_LOCATION_AND_SPEED               0x2A67
#define GATT_UUID_CHAR_LNS_NAVIGATION                       0x2A68
#define GATT_UUID_CHAR_LNS_POSITION_QUALITY                 0x2A69
#define GATT_UUID_CHAR_LNS_LN_FEATURE                       0x2A6A
#define GATT_UUID_CHAR_LNS_LN_CP                            0x2A6B



#define LN_CP_RSPCODE_RESERVED                              0x00
#define LN_CP_RSPCODE_SUCCESS                               0x01
#define LN_CP_RSPCODE_OPCODE_UNSUPPORT                      0x02
#define LN_CP_RSPCODE_INVALID_PARAMETER                     0x03
#define LN_CP_RSPCODE_OPERATION_FAILED                      0x04


#define LN_CTL_PNT_OPERATE_ACTIVE(x)                     \
    ( (x == LN_CP_OPCODE_SET_CUMULATIVE_VALUE)  || \
      (x == LN_CP_OPCODE_MASK_LOCATION_AND_SPEED_CHAR_CONTENT)  || \
      (x == LN_CP_OPCODE_NAVIGATION_CONTROL) || \
      (x == LN_CP_OPCODE_REQUEST_NUMBER_OF_ROUTES) || \
      (x == LN_CP_OPCODE_REQUEST_NAME_OF_ROUTE) || \
      (x == LN_CP_OPCODE_SELECT_ROUTE) || \
      (x == LN_CP_OPCODE_SET_FIX_RATE) || \
      (x == LN_CP_OPCODE_SET_ELEVATION) || \
      (x == LN_CP_OPCODE_RESPONSE_CODE) )

#define LNS_POSITION_AND_QUALITY_VALUE_MAX_LEN  16
#define LNS_LOCATION_AND_SPEED_VALUE_MAX_LEN    28
#define LNS_NAVIGATION_VALUE_MAX_LEN            19

uint32_t lns_ln_feature_support = 0;

T_LNS_NOTIFY_INDICATE_FLAG lns_notify_indicate_flag = {0};
T_LNS_CONTROL_POINT lns_ctl_pnt = {0};

T_POSITION_QUALITY_VALUE lns_position_quality_value = {0};
uint8_t lns_pq_value_for_read_cfm[LNS_POSITION_AND_QUALITY_VALUE_MAX_LEN] = {0};
uint8_t lns_pq_value_length_for_read_cfm = 0;

LOCATION_AND_SPEED_VALUE lns_location_and_speed_value = {0};
T_NAVIGATION_VALUE lns_navigation_value = {0};
uint16_t lns_cp_number_of_route = 0;
uint8_t lns_cp_name_of_route[LNS_CP_PARA_NAME_OF_ROUTE_MAX_LEN] = {0};
uint8_t lns_cp_name_of_route_len = 0;

/**<  Function pointer used to send event to application from location and navigation profile. */

static P_FUN_SERVER_GENERAL_CB pfn_lns_app_cb = NULL;


/**< @brief  profile/service definition.  */
static const T_ATTRIB_APPL lns_attr_tbl[] =
{
    /*----------------- Health Temperature Service -------------------*/
    /* <<Primary Service>>, .. 0,*/
    {
        (ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_LE),   /* wFlags     */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_PRIMARY_SERVICE),
            HI_WORD(GATT_UUID_PRIMARY_SERVICE),
            LO_WORD(GATT_UUID_SERVICE_LOCATION_AND_NAVIGATION),              /* service UUID */
            HI_WORD(GATT_UUID_SERVICE_LOCATION_AND_NAVIGATION)
        },
        UUID_16BIT_SIZE,                            /* bValueLen     */
        NULL,                                       /* pValueContext */
        GATT_PERM_READ                              /* wPermissions  */
    },

    /* <<Characteristic>>, .. 1, LN Feature*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ                     /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /* Temperature Measurement value 2,*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_LNS_LN_FEATURE),
            HI_WORD(GATT_UUID_CHAR_LNS_LN_FEATURE)
        },
        0,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },

    /* <<Characteristic>>, ..Location and Speed */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_NOTIFY                     /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_LNS_LOCATION_AND_SPEED),
            HI_WORD(GATT_UUID_CHAR_LNS_LOCATION_AND_SPEED)
        },
        0,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /* client characteristic configuration */
    {
        ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_CCCD_APPL,                   /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            /* NOTE: this value has an instantiation for each client, a write to */
            /* this attribute does not modify this default value:                */
            LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT), /* client char. config. bit field */
            HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)
        },
        2,                                          /* bValueLen */
        NULL,
        (GATT_PERM_READ | GATT_PERM_WRITE)          /* wPermissions */
    },

#ifdef LNS_POSITION_QUALITY_CHAR_SUPPORT
    /* <<Characteristic>>, .. Position Quality*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ                     /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_LNS_POSITION_QUALITY),
            HI_WORD(GATT_UUID_CHAR_LNS_POSITION_QUALITY)
        },
        0,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
#endif

#ifdef LNS_LN_CONTROL_POINT_CHAR_SUPPORT
    /* <<Characteristic>>, .. LN Control Point*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            (GATT_CHAR_PROP_WRITE | GATT_CHAR_PROP_INDICATE) /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /* Measurement Interval value*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_LNS_LN_CP),
            HI_WORD(GATT_UUID_CHAR_LNS_LN_CP)
        },
        0,                                          /* bValueLen */
        NULL,
        GATT_PERM_WRITE/* wPermissions */
    },
    /* client characteristic configuration */
    {
        ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_CCCD_APPL,                   /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            /* NOTE: this value has an instantiation for each client, a write to */
            /* this attribute does not modify this default value:                */
            LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT), /* client char. config. bit field */
            HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)
        },
        2,                                          /* bValueLen */
        NULL,
        (GATT_PERM_READ | GATT_PERM_WRITE)          /* wPermissions */
    },
#endif

#ifdef LNS_LN_NAVIGATION_CHAR_SUPPORT
    /* <<Characteristic>>, .. Navigation*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_NOTIFY/* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_LNS_NAVIGATION),
            HI_WORD(GATT_UUID_CHAR_LNS_NAVIGATION)
        },
        0,                                          /* bValueLen */
        NULL,
        GATT_PERM_NOTIF_IND_AUTHEN_REQ/* wPermissions */
    },
    /* client characteristic configuration */
    {
        ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_CCCD_APPL,                   /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT), /* client char. config. bit field */
            HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)
        },
        2,                                          /* bValueLen */
        NULL,
        (GATT_PERM_READ | GATT_PERM_WRITE)          /* wPermissions */
    }
#endif

};

/**< @brief  Health Temperature service size definition.  */
const uint16_t lns_attr_tbl_size = sizeof(lns_attr_tbl);

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
bool lns_set_parameter(T_LNS_PARAM_TYPE param_type, uint8_t len, void *p_value)
{
    bool ret = true;

    switch (param_type)
    {
    default:
        {
            ret = false;
            PROFILE_PRINT_ERROR0("lns_set_parameter failed\n");
        }
        break;

    case LNS_PARAM_LN_FEATURE_SUPPORT:
        {
            if (len != sizeof(uint32_t))
            {
                ret = false;
            }
            else
            {
                memcpy(&lns_ln_feature_support, p_value, len);
            }
        }
        break;

    //Location And Speed Value
    case LNS_LAS_PARAM_INC_FLAG:
        {
            if (len != sizeof(uint16_t))
            {
                ret = false;
            }
            else
            {
                memcpy(&lns_location_and_speed_value.flag, p_value, len);
            }
        }
        break;

    case LNS_LAS_PARAM_INSTANTANEOUS_SPEED:
        {
            if (len != sizeof(uint16_t))
            {
                ret = false;
            }
            else
            {
                memcpy(&lns_location_and_speed_value.instantaneous_speed, p_value, len);

            }
        }
        break;

    case LNS_LAS_PARAM_TOTAL_DISTANCE:
        {
            if (len != 3)
            {
                ret = false;
            }
            else
            {
                memcpy(&lns_location_and_speed_value.total_distance, p_value, len);

            }
        }
        break;

    case LNS_LAS_PARAM_LOCATION_LATITUDE:
        {
            if (len != 3)
            {
                ret = false;
            }
            else
            {
                memcpy(&lns_location_and_speed_value.location_latitude, p_value, len);

            }
        }
        break;

    case LNS_LAS_PARAM_LOCATION_LONGITUDE:
        {
            if (len != 3)
            {
                ret = false;
            }
            else
            {
                memcpy(&lns_location_and_speed_value.location_latitude, p_value, len);

            }
        }
        break;

    case LNS_LAS_PARAM_ELEVATION:
        {
            if (len != 3)
            {
                ret = false;
            }
            else
            {
                memcpy(&lns_location_and_speed_value.elevation, p_value, len);

            }
        }
        break;

    case LNS_LAS_PARAM_HEADING:
        {
            if (len != sizeof(uint16_t))
            {
                ret = false;
            }
            else
            {
                memcpy(&lns_location_and_speed_value.heading, p_value, len);

            }
        }
        break;

    case LNS_LAS_PARAM_ROLLING_TIME:
        {
            if (len != sizeof(uint8_t))
            {
                ret = false;
            }
            else
            {
                memcpy(&lns_location_and_speed_value.rolling_time, p_value, len);

            }
        }
        break;

    case LNS_LAS_PARAM_UTC_TIME:
        {
            if (len != 7)
            {
                ret = false;
            }
            else
            {
                memcpy(&lns_location_and_speed_value.utc_time, p_value, len);

            }
        }
        break;

    case LNS_LAS_PARAM_POSITION_STATUS:
        {
            lns_location_and_speed_value.flag.position_status =  *(uint8_t *)p_value;
        }
        break;

    case LNS_LAS_PARAM_SPEED_AND_DISTANCE_FORMAT:
        {
            lns_location_and_speed_value.flag.speed_and_distance_format =  *(uint8_t *)p_value;
        }
        break;

    case LNS_LAS_PARAM_ELEVATION_SOURCE:
        {
            lns_location_and_speed_value.flag.elevation_source =  *(uint8_t *)p_value;
        }
        break;

    case LNS_LAS_PARAM_HEADING_SOURCE:
        {
            lns_location_and_speed_value.flag.heading_source =  *(uint8_t *)p_value;
        }
        break;

    //Navigation value
    case LNS_NAVIGATION_PARAM_INC_FLAG:
        {
            if (len != sizeof(uint16_t))
            {
                ret = false;
            }
            else
            {
                memcpy(&lns_navigation_value.flag, p_value, len);
            }
        }
        break;

    case LNS_NAVIGATION_PARAM_BEARING:
        {
            if (len != sizeof(uint16_t))
            {
                ret = false;
            }
            else
            {
                memcpy(&lns_navigation_value.bearing, p_value, len);

            }
        }
        break;

    case LNS_NAVIGATION_PARAM_HEADING:
        {
            if (len != sizeof(uint16_t))
            {
                ret = false;
            }
            else
            {
                memcpy(&lns_navigation_value.bearing, p_value, len);

            }
        }
        break;

    case LNS_NAVIGATION_PARAM_REMAINING_DISTANCE:
        {
            if (len != 3)
            {
                ret = false;
            }
            else
            {
                memcpy(&lns_navigation_value.reamining_distance, p_value, len);

            }
        }
        break;

    case LNS_NAVIGATION_PARAM_REMAINING_VERTICAL_DISTANCE:
        {
            if (len != 3)
            {
                ret = false;
            }
            else
            {
                memcpy(&lns_navigation_value.reamining_vertical_distance, p_value, len);

            }
        }
        break;

    case LNS_NAVIGATION_PARAM_ESTIMATED_TIME_OF_ARRIVAL:
        {
            if (len != 7)
            {
                ret = false;
            }
            else
            {
                memcpy(&lns_navigation_value.estimated_time_of_arrival, p_value, len);

            }
        }
        break;

    case LNS_NAVIGATION_PARAM_POSITION_STATUS:
        {
            lns_navigation_value.flag.position_status =  *(uint8_t *)p_value;
        }
        break;
    case LNS_NAVIGATION_PARAM_HEADING_SOURCE:
        {
            lns_navigation_value.flag.heading_source =  *(uint8_t *)p_value;
        }
        break;

    case LNS_NAVIGATION_PARAM_NAVIGATION_INDICATOR_TYPE:
        {
            lns_navigation_value.flag.navigation_indicator_type =  *(uint8_t *)p_value;
        }
        break;

    case LNS_NAVIGATION_PARAM_WAYPOINT_REACHED:
        {
            lns_navigation_value.flag.waypoint_reached =  *(uint8_t *)p_value;
        }
        break;

    case LNS_NAVIGATION_PARAM_DESTINATION_REACHED:
        {
            lns_navigation_value.flag.destination_reached =  *(uint8_t *)p_value;
        }
        break;

    case LNS_PQ_PARAM_INC_FLAG:
        {
            if (len != sizeof(uint16_t))
            {
                ret = false;
            }
            else
            {
                memcpy(&lns_position_quality_value.flag, p_value, len);
            }
        }
        break;

    case LNS_PQ_PARAM_NUMBER_OF_BEACONS_IN_SOLUTION:
        {
            if (len != sizeof(uint8_t))
            {
                ret = false;
            }
            else
            {
                memcpy(&lns_position_quality_value.number_of_beacons_in_solution, p_value, len);
            }
        }
        break;

    case LNS_PQ_PARAM_NUMBER_OF_BEACONS_IN_VIEW:
        {
            if (len != sizeof(uint8_t))
            {
                ret = false;
            }
            else
            {
                memcpy(&lns_position_quality_value.number_of_beacons_in_view, p_value, len);
            }
        }
        break;


    case LNS_PQ_PARAM_TIME_TO_FIRST_FIX:
        {
            if (len != sizeof(uint16_t))
            {
                ret = false;
            }
            else
            {
                memcpy(&lns_position_quality_value.time_to_first_fix, p_value, len);
            }
        }
        break;

    case LNS_PQ_PARAM_EHPE:
        {
            if (len != sizeof(uint32_t))
            {
                ret = false;
            }
            else
            {
                memcpy(&lns_position_quality_value.ehpe, p_value, len);
            }
        }
        break;

    case LNS_PQ_PARAM_EVPE:
        {
            if (len != sizeof(uint32_t))
            {
                ret = false;
            }
            else
            {
                memcpy(&lns_position_quality_value.evpe, p_value, len);
            }
        }
        break;
    case LNS_PQ_PARAM_HDOP:
        {
            if (len != sizeof(uint8_t))
            {
                ret = false;
            }
            else
            {
                memcpy(&lns_position_quality_value.hdop, p_value, len);
            }
        }
        break;

    case LNS_PQ_PARAM_VDOP:
        {
            if (len != sizeof(uint8_t))
            {
                ret = false;
            }
            else
            {
                memcpy(&lns_position_quality_value.vdop, p_value, len);
            }
        }
        break;

    case LNS_CP_PARA_NUMBER_OF_ROUTE:
        {
            if (len != sizeof(uint16_t))
            {
                ret = false;
            }
            else
            {
                memcpy(&lns_cp_number_of_route, p_value, len);
            }
        }
        break;

    case LNS_CP_PARA_NAME_OF_ROUTE:
        {
            if (len > LNS_CP_PARA_NAME_OF_ROUTE_MAX_LEN)
            {
                ret = false;
            }
            else
            {
                memcpy(&lns_cp_name_of_route, p_value, len);
                lns_cp_name_of_route_len = len;
            }
        }
        break;
    case LNS_PARAM_CTL_PNT_PROG_CLR:
        {
            if (len != 0)
            {
                ret = false;
            }
            else
            {
                lns_ctl_pnt.value[0] = LN_CP_OPCODE_RESERVED;
            }
        }
        break;

    }

    return ret;
}

void lns_format_pq_value_for_read()
{
    uint8_t cur_offset = 0;

    memcpy(&lns_pq_value_for_read_cfm[cur_offset], &lns_position_quality_value.flag, 2);
    cur_offset += 2;

    if (lns_position_quality_value.flag.number_of_beacons_in_solution_present)
    {
        memcpy(&lns_pq_value_for_read_cfm[cur_offset],
               &lns_position_quality_value.number_of_beacons_in_solution, 1);
        cur_offset += 1;
    }

    if (lns_position_quality_value.flag.number_of_beacons_in_view_present)
    {
        memcpy(&lns_pq_value_for_read_cfm[cur_offset],
               &lns_position_quality_value.number_of_beacons_in_view, 1);
        cur_offset += 1;
    }

    if (lns_position_quality_value.flag.time_to_first_fix_present)
    {
        memcpy(&lns_pq_value_for_read_cfm[cur_offset], &lns_position_quality_value.time_to_first_fix, 2);
        cur_offset += 2;
    }

    if (lns_position_quality_value.flag.ehpe_present)
    {
        memcpy(&lns_pq_value_for_read_cfm[cur_offset], &lns_position_quality_value.ehpe, 4);
        cur_offset += 4;
    }

    if (lns_position_quality_value.flag.evpe_present)
    {
        memcpy(&lns_pq_value_for_read_cfm[cur_offset], &lns_position_quality_value.evpe, 4);
        cur_offset += 4;
    }

    if (lns_position_quality_value.flag.hdop_present)
    {
        memcpy(&lns_pq_value_for_read_cfm[cur_offset], &lns_position_quality_value.hdop, 1);
        cur_offset += 1;
    }

    if (lns_position_quality_value.flag.vdop_present)
    {
        memcpy(&lns_pq_value_for_read_cfm[cur_offset],
               &lns_position_quality_value.vdop, 1);
        cur_offset += 1;
    }

    lns_pq_value_length_for_read_cfm = cur_offset;

}

uint8_t lns_get_fisrt_value_offset_to_notify()
{

    uint8_t cur_offset = 2;

    if (lns_location_and_speed_value.flag.instantaneous_speed_present)
    {
        cur_offset += 2;
    }

    if (lns_location_and_speed_value.flag.total_distance_present)
    {
        cur_offset += 3;
    }

    if (lns_location_and_speed_value.flag.location_present)
    {
        cur_offset += 4;
        cur_offset += 4;
    }

    if (lns_location_and_speed_value.flag.elevation_present)
    {
        cur_offset += 3;
    }

    if (lns_location_and_speed_value.flag.heading_present)
    {
        cur_offset += 2;
    }


    if (lns_location_and_speed_value.flag.rolling_time_present)
    {
        cur_offset += 1;
    }

    if (cur_offset > 20)
    {
        return cur_offset - 1;
    }


    if (lns_location_and_speed_value.flag.utc_time_present)
    {
        cur_offset += 7;
    }

    if (cur_offset > 20)
    {
        return cur_offset - 7;
    }

    return cur_offset;

}

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
bool lns_location_and_speed_value_notify(uint8_t conn_id, T_SERVER_ID service_id)
{
    bool ret = true;

    T_LOCATION_AND_SPEED_VALUE_FLAG flag = {0};
    uint8_t location_and_speed_value[20];


    uint8_t cur_offset = 2;
    uint8_t first_notify_value_offset = lns_get_fisrt_value_offset_to_notify();

    if (lns_location_and_speed_value.flag.instantaneous_speed_present)
    {
        flag.instantaneous_speed_present = 1;
        memcpy(&location_and_speed_value[cur_offset], &lns_location_and_speed_value.instantaneous_speed, 2);
        cur_offset += 2;
    }

    if (lns_location_and_speed_value.flag.total_distance_present)
    {
        flag.total_distance_present = 1;
        memcpy(&location_and_speed_value[cur_offset], &lns_location_and_speed_value.total_distance, 3);
        cur_offset += 3;
    }

    if (lns_location_and_speed_value.flag.location_present)
    {
        flag.location_present = 1;
        memcpy(&location_and_speed_value[cur_offset], &lns_location_and_speed_value.location_latitude, 4);
        cur_offset += 4;
        memcpy(&location_and_speed_value[cur_offset], &lns_location_and_speed_value.location_longitute, 4);
        cur_offset += 4;
    }

    if (lns_location_and_speed_value.flag.elevation_present)
    {
        flag.elevation_present = 1;
        memcpy(&location_and_speed_value[cur_offset], &lns_location_and_speed_value.elevation, 3);
        cur_offset += 3;
    }

    if (lns_location_and_speed_value.flag.heading_present)
    {
        flag.heading_present = 1;
        memcpy(&location_and_speed_value[cur_offset],
               &lns_location_and_speed_value.heading, 2);
        cur_offset += 2;
    }

    if (cur_offset == first_notify_value_offset)
    {
        //copy flag
        memcpy(&location_and_speed_value[0], &flag, 2);
        //send first notification here
        server_send_data(conn_id, service_id, LNS_LN_LOCATION_AND_SPEED_VALUE_INDEX,
                         location_and_speed_value, cur_offset, GATT_PDU_TYPE_NOTIFICATION);

        //send remain data
        memset(&flag, 0, 2);
        cur_offset = 2;
        if (lns_location_and_speed_value.flag.rolling_time_present)
        {
            memcpy(&location_and_speed_value[cur_offset],
                   &lns_location_and_speed_value.rolling_time, 1);
            flag.rolling_time_present = 1;
            cur_offset += 1;
        }

        if (lns_location_and_speed_value.flag.utc_time_present)
        {

            memcpy(&location_and_speed_value[cur_offset],
                   &lns_location_and_speed_value.utc_time, 7);
            flag.utc_time_present = 1;
            cur_offset += 7;
        }
        memcpy(&location_and_speed_value[0], &flag, 2);
        //send second notification
        server_send_data(conn_id, service_id, LNS_LN_LOCATION_AND_SPEED_VALUE_INDEX,
                         location_and_speed_value, cur_offset, GATT_PDU_TYPE_NOTIFICATION);

        return ret;
    }

    if (lns_location_and_speed_value.flag.rolling_time_present)
    {
        flag.rolling_time_present = 1;
        memcpy(&location_and_speed_value[cur_offset],
               &lns_location_and_speed_value.rolling_time, 1);
        cur_offset += 1;
    }

    if (cur_offset == first_notify_value_offset)
    {
        //copy flag
        memcpy(&location_and_speed_value[0], &flag, 2);
        //send first notification here
        server_send_data(conn_id, service_id, LNS_LN_LOCATION_AND_SPEED_VALUE_INDEX,
                         location_and_speed_value, cur_offset, GATT_PDU_TYPE_NOTIFICATION);


        //send remain data
        memset(&flag, 0, 2);
        cur_offset = 2;
        if (lns_location_and_speed_value.flag.utc_time_present)
        {
            memcpy(&location_and_speed_value[cur_offset],
                   &lns_location_and_speed_value.utc_time, 7);
            flag.utc_time_present = 1;
            cur_offset += 7;
        }
        memcpy(&location_and_speed_value[0], &lns_location_and_speed_value.flag, 2);
        //send second notification
        server_send_data(conn_id, service_id, LNS_LN_LOCATION_AND_SPEED_VALUE_INDEX,
                         location_and_speed_value, cur_offset, GATT_PDU_TYPE_NOTIFICATION);

        return ret;
    }

    if (lns_location_and_speed_value.flag.utc_time_present)
    {
        if (cur_offset <= 13)
        {
            memcpy(&location_and_speed_value[cur_offset],
                   &lns_location_and_speed_value.utc_time, 7);
            flag.utc_time_present = 1;
            cur_offset += 7;
        }
        else if ((cur_offset > 13) && (cur_offset <= 19))
        {
            memcpy(&location_and_speed_value[cur_offset],
                   &lns_location_and_speed_value.utc_time, (20 - cur_offset));
            flag.utc_time_present = 1;
            cur_offset = 20;
        }
    }
    memcpy(&location_and_speed_value[0], &lns_location_and_speed_value.flag, 2);
    //send nortification
    server_send_data(conn_id, service_id, LNS_LN_LOCATION_AND_SPEED_VALUE_INDEX,
                     location_and_speed_value, cur_offset, GATT_PDU_TYPE_NOTIFICATION);

    return ret;
}

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
bool lns_navigation_value_notify(uint8_t conn_id, T_SERVER_ID service_id)
{
    uint8_t navigation_value[LNS_NAVIGATION_VALUE_MAX_LEN];
    uint8_t iCurOffset = 0;

    memcpy(&navigation_value[iCurOffset], &lns_navigation_value.flag, 2);
    iCurOffset += 2;
    memcpy(&navigation_value[iCurOffset], &lns_navigation_value.bearing, 2);
    iCurOffset += 2;
    memcpy(&navigation_value[iCurOffset], &lns_navigation_value.heading, 2);
    iCurOffset += 2;

    if (lns_navigation_value.flag.remaining_distance_present)
    {
        memcpy(&navigation_value[iCurOffset], &lns_navigation_value.reamining_distance, 3);
        iCurOffset += 3;
    }

    if (lns_navigation_value.flag.remaining_vertical_distance_present)
    {
        memcpy(&navigation_value[iCurOffset], &lns_navigation_value.reamining_vertical_distance, 3);
        iCurOffset += 3;
    }

    if (lns_navigation_value.flag.estimated_time_of_arrival_present)
    {
        memcpy(&navigation_value[iCurOffset], &lns_navigation_value.estimated_time_of_arrival, 7);
        iCurOffset += 7;
    }

    PROFILE_PRINT_INFO0("lns_navigation_value_notify");
    return server_send_data(conn_id, service_id, LNS_LN_NAVIGATION_VALUE_INDEX, navigation_value,
                            iCurOffset, GATT_PDU_TYPE_NOTIFICATION);
}

bool lns_ln_ctl_pnt_value_indicate(uint8_t conn_id, T_SERVER_ID service_id, uint8_t op_code,
                                   uint8_t rsp_code,
                                   uint8_t *p_parameter, uint8_t para_len)
{
    uint16_t attrib_index = LNS_LN_CP_VALUE_INDEX;

    uint8_t *p_data;
    uint16_t data_len;

    lns_ctl_pnt.value[0] = LN_CP_OPCODE_RESPONSE_CODE;
    lns_ctl_pnt.value[1] = op_code;    /* Control Point request opcode. */
    lns_ctl_pnt.value[2] = rsp_code;

    if (p_parameter != NULL && para_len != 0)
    {
        memcpy(&lns_ctl_pnt.value[3], p_parameter, para_len);
    }

    lns_ctl_pnt.cur_length = 3 * sizeof(uint8_t) + para_len;


    p_data = lns_ctl_pnt.value;
    data_len = lns_ctl_pnt.cur_length;

    // send indication to client
    return server_send_data(conn_id, service_id, attrib_index, p_data, data_len,
                            GATT_PDU_TYPE_INDICATION);
}




static void lns_ctl_pnt_set_cumulative_value(uint8_t conn_id, T_SERVER_ID service_id,
                                             uint8_t rsp_code)
{
    uint8_t              op_code = LN_CP_OPCODE_SET_CUMULATIVE_VALUE;
    lns_ln_ctl_pnt_value_indicate(conn_id, service_id, op_code, rsp_code, NULL, 0);
}

static void lns_ctl_pnt_mask_location_speed_char_content(uint8_t conn_id, T_SERVER_ID service_id,
                                                         uint8_t rsp_code)
{
    uint8_t              op_code = LN_CP_OPCODE_MASK_LOCATION_AND_SPEED_CHAR_CONTENT;
    lns_ln_ctl_pnt_value_indicate(conn_id, service_id, op_code, rsp_code, NULL, 0);
}

static void lns_ctl_pnt_navigation_control(uint8_t conn_id, T_SERVER_ID service_id,
                                           uint8_t rsp_code)
{
    uint8_t              op_code = LN_CP_OPCODE_NAVIGATION_CONTROL;
    lns_ln_ctl_pnt_value_indicate(conn_id, service_id, op_code, rsp_code, NULL, 0);
}

static void lns_ctl_pnt_request_number_of_routes(uint8_t conn_id, T_SERVER_ID service_id,
                                                 uint8_t rsp_code, uint16_t number_of_route)
{
    uint8_t              op_code = LN_CP_OPCODE_REQUEST_NUMBER_OF_ROUTES;
    lns_ln_ctl_pnt_value_indicate(conn_id, service_id, op_code, rsp_code, (uint8_t *)&number_of_route,
                                  2);
}

static void lns_ctl_pnt_request_name_of_route(uint8_t conn_id, T_SERVER_ID service_id,
                                              uint8_t rsp_code, uint8_t *pNameOfRoute, uint8_t len)
{
    uint8_t              op_code = LN_CP_OPCODE_REQUEST_NAME_OF_ROUTE;
    lns_ln_ctl_pnt_value_indicate(conn_id, service_id, op_code, rsp_code, pNameOfRoute, len);
}

static void lns_ctl_pnt_select_route(uint8_t conn_id, T_SERVER_ID service_id, uint8_t rsp_code)
{
    uint8_t              op_code = LN_CP_OPCODE_SELECT_ROUTE;
    lns_ln_ctl_pnt_value_indicate(conn_id, service_id, op_code, rsp_code, NULL, 0);
}

static void lns_ctl_pnt_set_fix_rate(uint8_t conn_id, T_SERVER_ID service_id, uint8_t rsp_code)
{
    uint8_t              op_code = LN_CP_OPCODE_SET_FIX_RATE;
    lns_ln_ctl_pnt_value_indicate(conn_id, service_id, op_code, rsp_code, NULL, 0);
}

static void lns_ctl_pnt_set_elevation(uint8_t conn_id, T_SERVER_ID service_id, uint8_t rsp_code)
{
    uint8_t              op_code = LN_CP_OPCODE_SET_ELEVATION;
    lns_ln_ctl_pnt_value_indicate(conn_id, service_id, op_code, rsp_code, NULL, 0);
}

static uint8_t lns_hanlde_ctl_pnt_proc_2(uint8_t service_id, uint16_t write_length,
                                         uint8_t *value_ptr)
{
    T_LNS_CALLBACK_DATA callback_data;
    uint8_t resp_code  = LN_CP_RSPCODE_SUCCESS;
    uint16_t parameter_length = 0;
    memcpy(lns_ctl_pnt.value, value_ptr, write_length);
    if (write_length >= 1)
    {
        parameter_length = write_length - 1;
    }

    PROFILE_PRINT_INFO1("lns_hanlde_ctl_pnt_proc request: OpCode=0x%x", lns_ctl_pnt.value[0]);

    callback_data.msg_type = SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE;
    callback_data.msg_data.write.opcode = (T_LN_CP_OPCODE)lns_ctl_pnt.value[0];

    switch (lns_ctl_pnt.value[0])
    {
    case LN_CP_OPCODE_NAVIGATION_CONTROL:
        {
            if (parameter_length == 1)
            {
                memcpy(&callback_data.msg_data.write.cp_parameter.navigation_control,
                       &lns_ctl_pnt.value[1], 1);
            }
            else
            {
                resp_code = LN_CP_RSPCODE_INVALID_PARAMETER;
            }
        }

        break;

    case LN_CP_OPCODE_SET_CUMULATIVE_VALUE:
        {
            if (parameter_length == 3)
            {
                memcpy(&callback_data.msg_data.write.cp_parameter.cumulative_total_distance,
                       &lns_ctl_pnt.value[1], 3);
            }
            else
            {
                resp_code = LN_CP_RSPCODE_INVALID_PARAMETER;
            }
        }
        break;

    case LN_CP_OPCODE_MASK_LOCATION_AND_SPEED_CHAR_CONTENT:
        {
            if (parameter_length == 2)
            {
                memcpy(&callback_data.msg_data.write.cp_parameter.mask_location_and_speed,
                       &lns_ctl_pnt.value[1], 2);
            }
            else
            {
                resp_code = LN_CP_RSPCODE_INVALID_PARAMETER;
            }
        }
        break;

    case LN_CP_OPCODE_REQUEST_NUMBER_OF_ROUTES:
        {
            //no parameter
        }
        break;
    case LN_CP_OPCODE_REQUEST_NAME_OF_ROUTE:
        {
            if (parameter_length == 2)
            {
                memcpy(&callback_data.msg_data.write.cp_parameter.number_of_desired_route,
                       &lns_ctl_pnt.value[1], 1);
            }
            else
            {
                resp_code = LN_CP_RSPCODE_INVALID_PARAMETER;
            }
        }
        break;

    case LN_CP_OPCODE_SELECT_ROUTE:
        {
            if (parameter_length == 2)
            {
                memcpy(&callback_data.msg_data.write.cp_parameter.select_route_desired_route_number,
                       &lns_ctl_pnt.value[1], 2);
            }
            else
            {
                resp_code = LN_CP_RSPCODE_INVALID_PARAMETER;
            }
        }
        break;

    case LN_CP_OPCODE_SET_FIX_RATE:
        {
            if (parameter_length == 1)
            {
                memcpy(&callback_data.msg_data.write.cp_parameter.set_fix_rate,
                       &lns_ctl_pnt.value[1], 1);
            }
            else
            {
                resp_code = LN_CP_RSPCODE_INVALID_PARAMETER;
            }
        }
        break;

    case LN_CP_OPCODE_SET_ELEVATION:
        {
            if (parameter_length == 3)
            {
                memcpy(&callback_data.msg_data.write.cp_parameter.set_elevation,
                       &lns_ctl_pnt.value[1], 3);
            }
            else
            {
                resp_code = LN_CP_RSPCODE_INVALID_PARAMETER;
            }
        }
        break;

    default:
        {
            resp_code = LN_CP_RSPCODE_OPCODE_UNSUPPORT;
        }
        break;
    }

    if (resp_code == LN_CP_RSPCODE_SUCCESS)
    {
        pfn_lns_app_cb(service_id, (void *)&callback_data);
    }
    return resp_code;
}

/**
 * @brief  handle control point write (request).
 *
 * @param[in] conn_id  Connection id.
 * @param[in] service_id  Service id.
 * @param[in] write_length      write request data length.
 * @param[in] value_ptr         pointer to write request data.
 * @return none
 * @retval  void
*/
static T_APP_RESULT lns_hanlde_ctl_pnt_proc(uint8_t conn_id, T_SERVER_ID service_id,
                                            uint16_t write_length, uint8_t *p_value)
{
    T_APP_RESULT  cause  = APP_RESULT_SUCCESS;
    memcpy(lns_ctl_pnt.value, p_value, write_length);

    PROFILE_PRINT_INFO1("lns_hanlde_ctl_pnt_proc request: op_code=0x%x", lns_ctl_pnt.value[0]);

    switch (lns_ctl_pnt.value[0])
    {
    case LN_CP_OPCODE_NAVIGATION_CONTROL:
        {
            if (!lns_notify_indicate_flag.navigation_enable)
            {
                cause = APP_RESULT_CCCD_IMPROPERLY_CONFIGURED;
            }
        }
        break;

    default:
        {
        }
        break;
    }
    return cause;
}

static void  lns_ctl_pnt_write_ind_post_proc(uint8_t conn_id, T_SERVER_ID service_id,
                                             uint16_t attrib_index, uint16_t write_length, uint8_t *p_value)
{
    uint8_t resp_code = LN_CP_RSPCODE_SUCCESS;

    bool error = false;
    memcpy(lns_ctl_pnt.value, p_value, write_length);
    lns_ctl_pnt.cur_length = write_length;

    PROFILE_PRINT_INFO1("lns_ctl_pnt_write_ind_post_proc: OpCode=0x%x", lns_ctl_pnt.value[0]);

    resp_code = lns_hanlde_ctl_pnt_proc_2(service_id, write_length, p_value);

    switch (lns_ctl_pnt.value[0])
    {
    default:
        {
            error = true;
        }
        break;

    case LN_CP_OPCODE_SET_CUMULATIVE_VALUE:
        {
            lns_ctl_pnt_set_cumulative_value(conn_id, service_id, resp_code);
        }
        break;

    case LN_CP_OPCODE_MASK_LOCATION_AND_SPEED_CHAR_CONTENT:
        {
            lns_ctl_pnt_mask_location_speed_char_content(conn_id, service_id, resp_code);
        }
        break;

    case LN_CP_OPCODE_NAVIGATION_CONTROL:
        {
            lns_ctl_pnt_navigation_control(conn_id, service_id, resp_code);
        }
        break;

    case LN_CP_OPCODE_REQUEST_NUMBER_OF_ROUTES:
        {
            lns_ctl_pnt_request_number_of_routes(conn_id, service_id, resp_code, lns_cp_number_of_route);
        }
        break;
    case LN_CP_OPCODE_REQUEST_NAME_OF_ROUTE:
        {
            lns_ctl_pnt_request_name_of_route(conn_id, service_id, resp_code, lns_cp_name_of_route,
                                              lns_cp_name_of_route_len);
        }
        break;

    case LN_CP_OPCODE_SELECT_ROUTE:
        {
            lns_ctl_pnt_select_route(conn_id, service_id, resp_code);
        }
        break;

    case LN_CP_OPCODE_SET_FIX_RATE:
        {
            lns_ctl_pnt_set_fix_rate(conn_id, service_id, resp_code);
        }
        break;

    case LN_CP_OPCODE_SET_ELEVATION:
        {
            lns_ctl_pnt_set_elevation(conn_id, service_id, resp_code);
        }
        break;
    }

    if (error)
    {
        lns_ln_ctl_pnt_value_indicate(conn_id, service_id, lns_ctl_pnt.value[0],
                                      LN_CP_RSPCODE_OPCODE_UNSUPPORT, NULL, 0);
    }

}

/**
 * @brief read characteristic data from service.
 *
 * @param[in] conn_id           Connection id.
 * @param[in] service_id        Service id.
 * @param[in] attrib_index          Attribute index of getting characteristic data.
 * @param[in] offset                Used for Blob Read.
 * @param[in] p_length            length of getting characteristic data.
 * @param[in] pp_value            data got from service.
 * @return Profile procedure result
*/
T_APP_RESULT lns_attr_read_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                              uint16_t offset, uint16_t *p_length, uint8_t **pp_value)
{
    T_APP_RESULT cause = APP_RESULT_SUCCESS;

    PROFILE_PRINT_INFO2("lns_attr_read_cb iAttribIndex = %d iOffset %x", attrib_index, offset);

    *p_length = 0;
    lns_ctl_pnt.value[0] = LN_CP_OPCODE_RESERVED;//////

    switch (attrib_index)
    {
    case LNS_LN_FEATURE_VALUE_INDEX:
        {
            *p_length = sizeof(lns_ln_feature_support);
            *pp_value = (uint8_t *)&lns_ln_feature_support;
        }
        break;

    case LNS_LN_POSITION_QUALITY_VALUE_INDEX:
        {
            T_LNS_CALLBACK_DATA callback_data;
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE;
            callback_data.msg_data.read_value_index = LNS_READ_POSITION_QUALITY_VALUE;
            pfn_lns_app_cb(service_id, (void *)&callback_data);
            //after app call lns_set_parameter to set Position Quality Value, profile will response this value to remote device
            lns_format_pq_value_for_read();
            *p_length = lns_pq_value_length_for_read_cfm;
            *pp_value = lns_pq_value_for_read_cfm;
        }
        break;

    default:
        {
            PROFILE_PRINT_ERROR1("lns_attr_read_cb  iAttribIndex = %d not found", attrib_index);
            cause  = APP_RESULT_ATTR_NOT_FOUND;
        }
        break;
    }

    return cause;
}

/**
 * @brief write characteristic data from service.
 *
 * @param[in] conn_id           Connection id.
 * @param[in] service_id        Service id.
 * @param[in] attrib_index      Attribute index of characteristic.
 * @param[in] write_type        Write type.
 * @param length            length of value to be written.
 * @param p_value            value to be written.
 * @param[in] p_write_ind_post_proc pointer of a function to handle attribute write.
 * @return Profile procedure result
*/
T_APP_RESULT lns_attr_write_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                               T_WRITE_TYPE write_type,
                               uint16_t length, uint8_t *p_value, P_FUN_WRITE_IND_POST_PROC *p_write_ind_post_proc)
{
    T_APP_RESULT cause  = APP_RESULT_SUCCESS;
    PROFILE_PRINT_INFO2("lns_attr_write_cb attrib_index = %d length %x", attrib_index, length);

    switch (attrib_index)
    {
    case LNS_LN_CP_VALUE_INDEX:
        {
            /* Attribute value has variable size, make sure written value size is valid. */
            if ((length > LNS_MAX_CTL_PNT_VALUE) || (p_value == NULL))
            {
                cause  = APP_RESULT_INVALID_VALUE_SIZE;
            }
            /* Make sure Control Point is not "Process already in progress". */
            else if (lns_ctl_pnt.value[0] == LN_CP_OPCODE_RESPONSE_CODE)
            {
                PROFILE_PRINT_ERROR1("lns_attr_write_cb: lns_ctl_pnt.value[0] %d ", lns_ctl_pnt.value[0]);
                cause  = APP_RESULT_PROC_ALREADY_IN_PROGRESS;
            }
            /* Make sure Control Point is configured indication enable. */
            else if (!lns_notify_indicate_flag.ln_cp_indicate_enable)
            {
                cause = APP_RESULT_CCCD_IMPROPERLY_CONFIGURED;
            }
            else
            {
                cause = lns_hanlde_ctl_pnt_proc(conn_id, service_id, length, p_value);
                if (cause == APP_RESULT_SUCCESS)
                {
                    *p_write_ind_post_proc = lns_ctl_pnt_write_ind_post_proc;
                }
                else
                {
                    *p_write_ind_post_proc = NULL;
                }
            }

        }
        break;

    default:
        {
            PROFILE_PRINT_ERROR1("lns_attr_write_cb  attrib_index = %d not found", attrib_index);
            cause = APP_RESULT_ATTR_NOT_FOUND;
        }
        break;
    }

    return cause;
}

/**
 * @brief update CCCD bits from stack.
 *
 * @param[in] conn_id           Connection id.
 * @param[in] service_id        Service id.
 * @param[in] index          Attribute index of characteristic data.
 * @param[in] ccc_bits         CCCD bits from stack.
 * @return None
*/
void lns_cccd_update_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t index, uint16_t ccc_bits)
{
    T_LNS_CALLBACK_DATA callback_data;
    bool bHandle = true;

    PROFILE_PRINT_INFO2("lns_cccd_update_cb index = %d ccc_bits %x", index, ccc_bits);

    switch (index)
    {
    case LNS_LN_LOCATION_AND_SPEED_CCCD_INDEX:
        {
            if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_NOTIFY)
            {
                // Enable Notification
                lns_notify_indicate_flag.location_and_speed_notify_enable = 1;
                callback_data.msg_data.notification_indification_index =
                    LNS_NOTIFY_INDICATE_LOCATION_AND_SPEED_ENABLE;
            }
            else
            {
                lns_notify_indicate_flag.location_and_speed_notify_enable = 0;
                callback_data.msg_data.notification_indification_index =
                    LNS_NOTIFY_INDICATE_LOCATION_AND_SPEED_DISABLE;
            }
        }
        break;

    case LNS_LN_CP_CCCD_INDEX:
        {
            if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_INDICATE)
            {
                // Enable Indofication
                lns_notify_indicate_flag.ln_cp_indicate_enable = 1;
                callback_data.msg_data.notification_indification_index = LNS_NOTIFY_INDICATE_CP_ENABLE;
            }
            else
            {
                lns_notify_indicate_flag.ln_cp_indicate_enable = 0;
                callback_data.msg_data.notification_indification_index = LNS_NOTIFY_INDICATE_CP_DISABLE;
            }

        }
        break;

    case LNS_LN_NAVIGATION_CCCD_INDEX:
        {
            if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_NOTIFY)
            {
                // Enable Notification
                lns_notify_indicate_flag.navigation_enable = 1;
                callback_data.msg_data.notification_indification_index = LNS_NOTIFY_INDICATE_NAVIGATION_ENABLE;
            }
            else
            {
                lns_notify_indicate_flag.navigation_enable = 0;
                callback_data.msg_data.notification_indification_index = LNS_NOTIFY_INDICATE_NAVIGATION_DISABLE;
            }
        }
        break;

    default:
        {
            bHandle = false;
            PROFILE_PRINT_ERROR1("lns_cccd_update_cb Index = %d not found", index);
        }
        break;

    }
    /* Notify Application. */
    if (pfn_lns_app_cb && (bHandle == true))
    {
        callback_data.msg_type = SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION;
        pfn_lns_app_cb(service_id, (void *)&callback_data);
    }
    return;
}

/**
 * @brief LOCATION AND NAVIGATION Service Callbacks.
*/
const T_FUN_GATT_SERVICE_CBS lns_cbs =
{
    lns_attr_read_cb,  // Read callback function pointer
    lns_attr_write_cb, // Write callback function pointer
    lns_cccd_update_cb  // CCCD update callback function pointer
};

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
uint8_t lns_add_service(void *p_func)
{
    uint8_t service_id;
    if (false == server_add_service(&service_id,
                                    (uint8_t *)lns_attr_tbl,
                                    lns_attr_tbl_size,
                                    lns_cbs))
    {
        PROFILE_PRINT_ERROR1("lns_add_service: ServiceId %d", service_id);
        service_id = 0xff;
    }
    pfn_lns_app_cb = (P_FUN_SERVER_GENERAL_CB)p_func;
    return service_id;
}


