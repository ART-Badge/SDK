/**
*********************************************************************************************************
*               Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     cscs.c
* @brief    CSC profile interfaces called in profile.c.
* @details  Interfaces to get and put csc characteristics value and csc control point procedure.
* @author   ethan_su
* @date     2017-10-11
* @version  v1.0
*********************************************************************************************************
*/
#include "trace.h"
#include <string.h>
#include "gatt.h"
#include "cscs.h"

/********************************************************************************************************
* local static variables defined here, only used in this source file.
********************************************************************************************************/
typedef struct
{
    uint8_t csc_measurement_notify_enable: 1;
    uint8_t sc_cp_indicate_enable: 1;
    uint8_t rfu: 6;
} CSCS_NOTIFY_INDICATE_FLAG;

/** @brief  CSC service related UUIDs. */
#define GATT_UUID_CSCS                       0x1816
#define GATT_UUID_CHAR_CSCS_MEASUREMENT      0x2A5B
#define GATT_UUID_CHAR_CSCS_FEATURE          0x2A5C
#define GATT_UUID_CHAR_SENSOR_LOCATION      0x2A5D
#define GATT_UUID_CHAR_SC_CONTROL_POINT     0x2A55


/**<  CSC measurement data. */
static T_CSCS_MEASUREMENT cscs_measurement;
/**<  CSC feature that CSC service supported, can be configured by user. */
static uint16_t  cscs_feature;
/**<  CSC current sensor location, where the CSC sensor located. */
static uint8_t cscs_cur_sens_location;
/**<  CSC supported sensor location list, can be configured by user. */
static uint16_t cscs_sens_loc_list;
/**<  CSC control point data. */
static T_CSCS_CONTROL_POINT  cscs_control_point = {0};
/**<  CSC control point indication enable flag. */
static CSCS_NOTIFY_INDICATE_FLAG cscs_notify_indicate_flag = {0};
/**<  Function pointer used to send event to application from CSC profile. Initiated in cscs_add_service. */
static P_FUN_SERVER_GENERAL_CB pfn_cscs_cb = NULL;

/**< @brief  profile/service definition.  */
const T_ATTRIB_APPL cscs_attr_tbl[] =
{
    /*-------------------------- CSC Service ---------------------------*/
    /* <<Primary Service>>, ..  Index 0 */
    {
        (ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_LE),  /* wFlags     */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_PRIMARY_SERVICE),
            HI_WORD(GATT_UUID_PRIMARY_SERVICE),
            LO_WORD(GATT_UUID_CSCS),               /* service UUID */
            HI_WORD(GATT_UUID_CSCS)
        },
        UUID_16BIT_SIZE,                            /* bValueLen     */
        NULL,                                       /* pValueContext */
        GATT_PERM_READ                              /* wPermissions  */
    },

    /* <<Characteristic>>, ..  Index 1 */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_NOTIFY,                    /* characteristic properties */
            //XXXXMJMJ GATT_CHAR_PROP_INDICATE,                  /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*--- CSC Measurement characteristic value --- Index 2 */
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_CSCS_MEASUREMENT),
            HI_WORD(GATT_UUID_CHAR_CSCS_MEASUREMENT)
        },
        0,                                          /* variable size */
        (void *)NULL,
        GATT_PERM_NONE                              /* wPermissions */
    },
    /* client characteristic configuration  Index 3 */
    {
        (ATTRIB_FLAG_VALUE_INCL |                   /* wFlags */
         ATTRIB_FLAG_CCCD_APPL),
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

    /* <<Characteristic>>, ..  Index 4 */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ                       /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*--- CSC Features characteristic value --- Index 5 */
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_CSCS_FEATURE),
            HI_WORD(GATT_UUID_CHAR_CSCS_FEATURE),
        },
        0,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    }

#ifdef CSCS_MULTIPLE_SENSOR_LOCATIONS_SUPPORT
    ,
    /* <<Characteristic>>, ..  Index 6 */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ,                    /* characteristic properties */
            //XXXXMJMJ GATT_CHAR_PROP_INDICATE,                  /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*--- CSC sensor location characteristic value --- Index 7 */
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_SENSOR_LOCATION),
            HI_WORD(GATT_UUID_CHAR_SENSOR_LOCATION)
        },
        0,                                          /* variable size */
        (void *)NULL,
        GATT_PERM_READ                              /* wPermissions */
    }
#endif

#ifdef CSCS_SC_CONTROL_POINT_SUPPORT
    ,
    /* <<Characteristic>>, ..  Index 8 */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            (GATT_CHAR_PROP_WRITE |                   /* characteristic properties */
             GATT_CHAR_PROP_INDICATE)
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*--- CSC SC Control Point value --- Index 9 */
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_SC_CONTROL_POINT),
            HI_WORD(GATT_UUID_CHAR_SC_CONTROL_POINT)
        },
        0,                                          /* bValueLen, 0 : variable length */
        NULL,
        GATT_PERM_WRITE                             /* wPermissions */
    },
    /* client characteristic configuration  Index 10 */
    {
        (ATTRIB_FLAG_VALUE_INCL |                   /* wFlags */
         ATTRIB_FLAG_CCCD_APPL),
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
    }
#endif

};
/**< @brief  CSC service size definition.  */
const int32_t cscs_attr_tbl_size = sizeof(cscs_attr_tbl);

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
bool cscs_set_parameter(T_CSCS_PARAM_TYPE param_type, uint8_t len, void *p_value)
{
    bool ret = true;
    uint8_t inc_flag = cscs_measurement.value[0];

    switch (param_type)
    {
    default:
        ret = false;
        break;
    case CSCS_PARAM_CSCS_FEATURE:
        memcpy(&cscs_feature, p_value, 2);
        break;

    case CSCS_PARAM_INC_FLAG:
        cscs_measurement.value[0] = *(uint8_t *)p_value;
        inc_flag = cscs_measurement.value[0];
        cscs_measurement.cur_length = 1;
        if (inc_flag & CSCS_INC_WHEEL_REVOL_MASK)
        {
            cscs_measurement.cur_length += 6;
        }
        if (inc_flag & CSCS_INC_CRANK_REVOL_MASK)
        {
            cscs_measurement.cur_length += 4;
        }
        break;
    case CSCS_PARAM_WHEEL_REVOL:
        if (inc_flag & CSCS_INC_WHEEL_REVOL_MASK)
        {
            LE_UINT32_TO_ARRAY(cscs_measurement.value + 1, *(uint32_t *)p_value);
        }
        else
        {
            ret = false;
        }
        break;
    case CSCS_PARAM_WHEEL_EVT_TIME:
        if (inc_flag & CSCS_INC_WHEEL_REVOL_MASK)
        {
            LE_UINT16_TO_ARRAY(cscs_measurement.value + 5, *(uint16_t *)p_value);
        }
        else
        {
            ret = false;
        }
        break;
    case CSCS_PARAM_CRANK_REVOL:
        if (inc_flag & CSCS_INC_CRANK_REVOL_MASK)
        {
            if (inc_flag & CSCS_INC_WHEEL_REVOL_MASK)
            {
                LE_UINT16_TO_ARRAY(cscs_measurement.value + 7, *(uint16_t *)p_value);
            }
            else
            {
                LE_UINT16_TO_ARRAY(cscs_measurement.value + 1, *(uint16_t *)p_value);
            }
        }
        else
        {
            ret = false;
        }
        break;
    case CSCS_PARAM_CRANK_EVT_TIME:
        if (inc_flag & CSCS_INC_CRANK_REVOL_MASK)
        {
            if (inc_flag & CSCS_INC_WHEEL_REVOL_MASK)
            {
                LE_UINT16_TO_ARRAY(cscs_measurement.value + 9, *(uint16_t *)p_value);
            }
            else
            {
                LE_UINT16_TO_ARRAY(cscs_measurement.value + 3, *(uint16_t *)p_value);
            }
        }
        else
        {
            ret = false;
        }
        break;
    case CSCS_PARAM_SENSOR_LOC:
        cscs_cur_sens_location = *(uint8_t *)p_value;
        break;
    case CSCS_PARAM_CTL_PNT_PROG_CLR:
        cscs_control_point.value[0] = CSCS_CP_OPCODE_RESERVED;
        break;
    }

    if (!ret)
    {
        PROFILE_PRINT_INFO0("CSC parameter set failed\n");
    }

    return ret;
}

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
bool cscs_get_parameter(T_CSCS_PARAM_TYPE param_type, void *p_value)
{
    bool ret = true;
    uint8_t inc_flag = cscs_measurement.value[0];

    switch (param_type)
    {
    default:
        ret = false;
        break;
    case CSCS_PARAM_INC_FLAG:
        *(uint8_t *)p_value = cscs_measurement.value[0];
        break;
    case CSCS_PARAM_WHEEL_REVOL:
        if (inc_flag & CSCS_INC_WHEEL_REVOL_MASK)
        {
            LE_ARRAY_TO_UINT16(*(uint16_t *)p_value, cscs_measurement.value + 1);
        }
        else
        {
            ret = false;
        }
        break;
    case CSCS_PARAM_WHEEL_EVT_TIME:
        if (inc_flag & CSCS_INC_WHEEL_REVOL_MASK)
        {
            LE_ARRAY_TO_UINT16(*(uint16_t *)p_value, cscs_measurement.value + 5);
        }
        else
        {
            ret = false;
        }
        break;
    case CSCS_PARAM_CRANK_REVOL:
        if (inc_flag & CSCS_INC_CRANK_REVOL_MASK)
        {
            if (inc_flag & CSCS_INC_WHEEL_REVOL_MASK)
            {
                LE_ARRAY_TO_UINT16(*(uint16_t *)p_value, cscs_measurement.value + 7);
            }
            else
            {
                LE_ARRAY_TO_UINT16(*(uint16_t *)p_value, cscs_measurement.value + 1);
            }
        }
        else
        {
            ret = false;
        }
        break;
    case CSCS_PARAM_CRANK_EVT_TIME:
        if (inc_flag & CSCS_INC_CRANK_REVOL_MASK)
        {
            if (inc_flag & CSCS_INC_WHEEL_REVOL_MASK)
            {
                LE_ARRAY_TO_UINT16(*(uint16_t *)p_value, cscs_measurement.value + 9);
            }
            else
            {
                LE_ARRAY_TO_UINT16(*(uint16_t *)p_value, cscs_measurement.value + 3);
            }
        }
        else
        {
            ret = false;
        }
        break;
    case CSCS_PARAM_SENSOR_LOC:
        *(uint8_t *)p_value = cscs_cur_sens_location;
        break;
    }

    if (!ret)
    {
        PROFILE_PRINT_INFO0("cscs parameter get failed\n");
    }

    return ret;
}

/**
 * @brief  display control point response.
 *
 * @param cscs_ctl_pnt_ptr       pointer to CSC control point data.
 * @return none
 * @retval  void
*/
static void  cscs_ctl_pnt_display_rsp(T_CSCS_CONTROL_POINT *cscs_ctl_pnt_ptr)
{
    PROFILE_PRINT_INFO1("cscs cp response: req_op_code=0x%x", cscs_ctl_pnt_ptr->value[1]);
    PROFILE_PRINT_INFO1("rsp_code=0x%x", cscs_ctl_pnt_ptr->value[2]);
}

/**
 * @brief  whether the sensor location supported.
 *
 * @param[in] sens_location     sensor location to be judged(CSC sensor support or not).
 * @return support result.
 * @retval  1   support
 * @retval  0   not support
*/
static bool cscs_sens_loc_supported(uint8_t sens_location)
{
    uint8_t result = false;
    if ((cscs_sens_loc_list >> sens_location) & 0x0001)
    {
        result = true;
    }

    return (result);
}

/**
 * @brief  set cumulative response.
 *
 * @param[in] conn_id  Connection id.
 * @param[in] serviceID
 * @return none
 * @retval  void
*/
static void cscs_ctl_pnt_set_cumulative(uint8_t conn_id, T_SERVER_ID service_id, uint8_t rsp_code)
{
    uint8_t op_code = CSCS_CP_OPCODE_SET_CUMULATIVE;
    cscs_ctl_pnt_indicate(conn_id, service_id, op_code, rsp_code);
}

/**
 * @brief  update sensor location response.
 *
 * @param[in] conn_id  Connection id.
 * @param[in] service_id Service ID.
 * @param[in] rsp_code Response Code.
 * @return none
 * @retval  void
*/
static void cscs_ctl_pnt_update_sens_loc(uint8_t conn_id, T_SERVER_ID service_id, uint8_t rsp_code)
{
    uint8_t op_code = CSCS_CP_OPCODE_UPDATE_SENS_LOC;
    cscs_ctl_pnt_indicate(conn_id, service_id, op_code, rsp_code);
}

/**
 * @brief  supported sensor location response.
 *
 * @param[in] conn_id  Connection id.
 * @param[in] service_id Service ID.
 * @param[in] rsp_code Response Code.
 * @return none
 * @retval  void
*/
static void cscs_ctl_pnt_req_sens_loc_list(uint8_t conn_id, T_SERVER_ID  service_id,
                                           uint8_t rsp_code)
{
    uint8_t op_code = CSCS_CP_OPCODE_REQ_SENS_LOC_LIST;
    cscs_ctl_pnt_indicate(conn_id, service_id, op_code, rsp_code);
}

static uint8_t  cscs_hanlde_ctl_pnt_proc2(uint8_t service_id, uint16_t write_length,
                                          uint8_t *p_value)
{
    T_CSCS_CALLBACK_DATA callback_data;
    uint8_t resp_code  = CSCS_CP_RSPCODE_SUCCESS;
    uint16_t parameter_length = 0;
    memcpy(cscs_control_point.value, p_value, write_length);
    if (write_length >= 1)
    {
        parameter_length = write_length - 1;
    }

    PROFILE_PRINT_INFO1("cscs_hanlde_ctl_pnt_proc2 request: op_code=0x%x", cscs_control_point.value[0]);

    callback_data.msg_type = SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE;
    callback_data.msg_data.write.opcode = cscs_control_point.value[0];

    switch (cscs_control_point.value[0])
    {

    case CSCS_CP_OPCODE_SET_CUMULATIVE:
        {
            if (parameter_length == 4)
            {
                memcpy(&callback_data.msg_data.write.cp_parameter.cumulative_value,
                       &cscs_control_point.value[1], 4);
            }
            else
            {
                resp_code = CSCS_CP_RSPCODE_INVALID_PARAMETER;
            }
        }

        break;

    case CSCS_CP_OPCODE_UPDATE_SENS_LOC:
        {
            if (parameter_length == 1)
            {
                memcpy(&callback_data.msg_data.write.cp_parameter.sensor_location_value,
                       &cscs_control_point.value[1], 1);
                if (callback_data.msg_data.write.cp_parameter.sensor_location_value >= CSCS_SENSOR_LOC_MAX)
                {
                    resp_code = CSCS_CP_RSPCODE_INVALID_PARAMETER;
                }
            }
            else
            {
                resp_code = CSCS_CP_RSPCODE_INVALID_PARAMETER;
            }
        }
        break;

    case CSCS_CP_OPCODE_REQ_SENS_LOC_LIST:
        {
            if (parameter_length == 0)
            {
            }
            else
            {
                resp_code = CSCS_CP_RSPCODE_INVALID_PARAMETER;
            }
        }
        break;

    default:
        {
            resp_code = CSCS_CP_RSPCODE_OPCODE_UNSUPPORT;
        }
        break;
    }

    if (resp_code == CSCS_CP_RSPCODE_SUCCESS)
    {
        pfn_cscs_cb(service_id, (void *)&callback_data);
    }
    return resp_code;
}


/**
 * @brief  handle control point write (request).
 *
 * @param[in] conn_id  Connection id.
 * @param[in] service_id Service ID.
 * @param[in] write_length      Write request data length.
 * @param[in] value_ptr         Pointer to write request data.
 * @return none
 * @retval  void
*/
static void cscs_ctl_pnt_handle_req(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                                    uint16_t write_length, uint8_t *p_value)
{
    uint8_t resp_code = CSCS_CP_RSPCODE_SUCCESS;

    memcpy(cscs_control_point.value, p_value, write_length);
    cscs_control_point.cur_length = write_length;

    PROFILE_PRINT_INFO1("csccp request: op_code=0x%x", cscs_control_point.value[0]);

    resp_code = cscs_hanlde_ctl_pnt_proc2(service_id, write_length, p_value);

    if (resp_code == CSCS_CP_RSPCODE_SUCCESS)
    {
        switch (cscs_control_point.value[0])
        {
        default:
            resp_code = CSCS_CP_RSPCODE_OPCODE_UNSUPPORT;
            break;
        case CSCS_CP_OPCODE_SET_CUMULATIVE:
            if ((cscs_feature & CSCS_SUPPORT_WHEEL_REVOL_MASK) &&
                (cscs_measurement.value[0] & CSCS_INC_WHEEL_REVOL_MASK))
            {
                cscs_ctl_pnt_set_cumulative(conn_id, service_id, resp_code);
                return;
            }
            break;
        case CSCS_CP_OPCODE_UPDATE_SENS_LOC:
            if (cscs_feature & CSCS_SUPPORT_MULTI_SENSOR_MASK)
            {
                cscs_ctl_pnt_update_sens_loc(conn_id, service_id,  resp_code);
                return;
            }
            break;
        case CSCS_CP_OPCODE_REQ_SENS_LOC_LIST:
            if (cscs_feature & CSCS_SUPPORT_MULTI_SENSOR_MASK)
            {
                cscs_ctl_pnt_req_sens_loc_list(conn_id, service_id,  resp_code);
                return;
            }
            break;
        }
    }

    /* Send indication to client when request opcode not supported. */
    cscs_ctl_pnt_indicate(conn_id, service_id, cscs_control_point.value[0], resp_code);
}

/**
 * @brief read characteristic data from service.
 *
 * @param[in] conn_id  Connection id.
 * @param[in] service_id          ServiceID of characteristic data.
 * @param[in] attrib_index          Attribute index of getting characteristic data.
 * @param[in] offset                Used for Blob Read.
 * @param[in,out] p_length            length of getting characteristic data.
 * @param[in,out] pp_value            data got from service.
 * @return Profile procedure result
*/
T_APP_RESULT cscs_attr_read(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                            uint16_t offset, uint16_t *p_length, uint8_t **pp_value)
{
    T_APP_RESULT cause  = APP_RESULT_SUCCESS;

    switch (attrib_index)
    {
    default:
        PROFILE_PRINT_ERROR1("cscs_attr_read, attr not found, index=%d", attrib_index);
        cause  = APP_RESULT_ATTR_NOT_FOUND;
        break;
    case GATT_SVC_CSCS_FEATURE_INDEX:
        {
            T_CSCS_CALLBACK_DATA callback_data;
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE;
            callback_data.msg_data.read_value_index = CSCS_READ_CSCS_FEATURE;
            pfn_cscs_cb(service_id, &callback_data);

            *pp_value = (uint8_t *)&cscs_feature;
            *p_length = sizeof(cscs_feature);
        }
        break;
    case GATT_SVC_CSCS_SENS_LOC_INDEX:
        if (cscs_feature & CSCS_SUPPORT_MULTI_SENSOR_MASK)
        {
            T_CSCS_CALLBACK_DATA callback_data;
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE;
            callback_data.msg_data.read_value_index = CSCS_READ_SENSOR_LOCATION;
            pfn_cscs_cb(service_id, &callback_data);
            *pp_value = (uint8_t *)&cscs_cur_sens_location;
            *p_length = sizeof(cscs_cur_sens_location);
        }
        else
        {
            cause  = APP_RESULT_ATTR_NOT_FOUND;
        }
        break;
    }

    return (cause);
}




/**
 * @brief write characteristic data from service.
 *
 * @param[in] conn_id  Connection id.
 * @param[in] service_id          ServiceID to be written.
 * @param[in] attrib_index          Attribute index of characteristic.
 * @param[in] length            length of value to be written.
 * @param[in] p_value            value to be written.
 * @param[in] p_write_ind_post_proc pointer of a function to handle control point write.
 * @return Profile procedure result
*/
T_APP_RESULT cscs_attr_write(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                             T_WRITE_TYPE write_type, uint16_t length, uint8_t *p_value,
                             P_FUN_WRITE_IND_POST_PROC *p_write_ind_post_proc)
{
    T_APP_RESULT cause  = APP_RESULT_SUCCESS;
    if (GATT_SVC_CSCS_CTL_PNT_INDEX == attrib_index)
    {
        /* Attribute value has variable size, make sure written value size is valid. */
        if ((length > CSCS_MAX_CTL_PNT_VALUE) || (p_value == NULL))
        {
            cause  = APP_RESULT_INVALID_VALUE_SIZE;
        }
        /* Make sure Control Point is not "Process already in progress". */
        else if (CSCS_CTL_PNT_OPERATE_ACTIVE(cscs_control_point.value[0]))
        {
            cause  = (T_APP_RESULT)0x80;//ProfileResult_AppErr_ProcAlreadyInProgress
        }
        /* Make sure Control Point is configured indication enable. */
        else if (false == cscs_notify_indicate_flag.sc_cp_indicate_enable)
        {
            cause = (T_APP_RESULT)0x81;//ProfileResult_AppErr_CccdImproperlyConfigured
        }
        else
        {
            /* handle SCCP request after sending write response */
            *p_write_ind_post_proc = cscs_ctl_pnt_handle_req;
        }
    }
    else
    {
        PROFILE_PRINT_ERROR2("cscs_attr_write Error attrib_index = 0x%x length=%d", attrib_index, length);
        cause = APP_RESULT_ATTR_NOT_FOUND;
    }

    return cause;
}

/**
 * @brief       Send measurement value notification data .
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
bool cscs_meas_value_notify(uint8_t conn_id, T_SERVER_ID service_id)
{
    uint16_t attrib_index = GATT_SVC_CSCS_MEASUREMENT_INDEX;
    uint8_t *p_data = cscs_measurement.value;
    uint16_t dataLen = cscs_measurement.cur_length;

    PROFILE_PRINT_INFO0("cscs measurement notification");
    return server_send_data(conn_id, service_id, attrib_index, p_data, dataLen,
                            GATT_PDU_TYPE_NOTIFICATION);
}

/**
 * @brief       Send control point indication.
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
                           uint8_t rsp_code)
{
    uint16_t attrib_index = GATT_SVC_CSCS_CTL_PNT_INDEX;
    uint8_t sens_location;
    uint8_t param_offset;
    uint8_t *p_data;
    uint16_t data_len;

    cscs_control_point.value[1] = cscs_control_point.value[0];  /* Control Point request opcode. */
    cscs_control_point.value[2] = rsp_code;
    cscs_control_point.value[0] = CSCS_CP_OPCODE_RSP_CODE;
    cscs_control_point.cur_length = 3 * sizeof(uint8_t);

    /* Diff RspType, different indication contents. */
    if (op_code == CSCS_CP_OPCODE_REQ_SENS_LOC_LIST)
    {
        /* get sensor location list */
        for (sens_location = CSCS_SENSOR_LOC_OTHER, param_offset = 3; sens_location < CSCS_SENSOR_LOC_MAX;
             sens_location++)
        {
            if (cscs_sens_loc_supported(sens_location))
            {
                cscs_control_point.value[param_offset] = sens_location;
                param_offset++;
            }
        }
        cscs_control_point.cur_length = param_offset;
    }

    cscs_ctl_pnt_display_rsp(&cscs_control_point);
    p_data = cscs_control_point.value;
    data_len = cscs_control_point.cur_length;

    // send indication to client
    return server_send_data(conn_id, service_id, attrib_index, p_data, data_len,
                            GATT_PDU_TYPE_INDICATION);
}

/**
 * @brief update CCCD bits from stack.
 *
 * @param[in] conn_id       Connection id.
 * @param[in] service_id    Service ID.
 * @param[in] index         Attribute index of characteristic data.
 * @param[in] ccc_bits      CCCD bits from stack.
 * @return None
*/
void cscs_cccd_update(uint8_t conn_id, T_SERVER_ID service_id, uint16_t index, uint16_t ccc_bits)
{
    T_CSCS_CALLBACK_DATA callback_data;
    bool handle = false;
    PROFILE_PRINT_INFO2("cscs_cccd_update index = %d ccc_bits %x", index, ccc_bits);
    switch (index)
    {
    case GATT_SVC_CSCS_MEAS_CCCD_INDEX:
        if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_NOTIFY)
        {
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION;
            callback_data.msg_data.notification_indification_index = CSCS_NOTIFY_INDICATE_MEASUREMENT_ENABLE;
            cscs_notify_indicate_flag.csc_measurement_notify_enable = true;
        }
        else
        {
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION;
            callback_data.msg_data.notification_indification_index = CSCS_NOTIFY_INDICATE_MEASUREMENT_ENABLE;
            cscs_notify_indicate_flag.csc_measurement_notify_enable = false;
        }
        handle =  true;
        break;
    case GATT_SVC_CSCS_CTL_PNT_CCCD_INDEX:
        if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_INDICATE)
        {
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION;
            callback_data.msg_data.notification_indification_index = CSCS_NOTIFY_INDICATE_SC_CP_ENABLE;
            cscs_notify_indicate_flag.sc_cp_indicate_enable = true;

        }
        else
        {
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION;
            callback_data.msg_data.notification_indification_index = CSCS_NOTIFY_INDICATE_SC_CP_DISABLE;
            cscs_notify_indicate_flag.sc_cp_indicate_enable = false;
        }
        handle = true;
        break;
    default:
        break;
    }
    /* Notify Application. */
    if (pfn_cscs_cb && (handle == true))
    {
        pfn_cscs_cb(service_id, (void *)&callback_data);
    }
}

/**
 * @brief CSC Service Callbacks.
*/
const T_FUN_GATT_SERVICE_CBS cscs_cbs =
{
    cscs_attr_read,  // Read callback function pointer
    cscs_attr_write, // Write callback function pointer
    cscs_cccd_update  // CCCD update callback function pointer
};

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
T_SERVER_ID cscs_add_service(void *p_func)
{
    T_SERVER_ID service_id;

    /* Initiate CSC service related data, modify according to user's demand. */
    cscs_feature = CSCS_ALL_FEATURE_SUPPORTED;
    cscs_measurement.value[0] = CSCS_INC_ALL_PRESENTS;
    cscs_measurement.cur_length = CSCS_MAX_MEASUREMENT_VALUE;
    cscs_sens_loc_list = CSCS_ALL_SENS_LOC_SUPPORTED;
    cscs_cur_sens_location = CSCS_SENSOR_LOC_REAL_WHEEL;

    /* register CSC service to profile layer. */
    if (false == server_add_service(&service_id,
                                    (uint8_t *)cscs_attr_tbl,
                                    cscs_attr_tbl_size,
                                    cscs_cbs))
    {
        PROFILE_PRINT_ERROR1("cscs_add_service: serviceId %d", service_id);
        service_id = 0xff;
        return service_id;
    }

    /* register callback for profile to inform application that some events happened. */
    pfn_cscs_cb = (P_FUN_SERVER_GENERAL_CB)p_func;
    return service_id;
}


