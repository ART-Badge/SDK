/**
*********************************************************************************************************
*               Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rscs.c
* @brief    RSC service source file.
* @details  Interfaces to get and put rsc characteristics value and rsc control point procedure.
* @author   ethan_su
* @date     2017-10-10
* @version  v1.0
*********************************************************************************************************
*/
#include "trace.h"
#include <string.h>
#include "gatt.h"
#include "rscs.h"

#define GATT_UUID_RSC                       0x1814
#define GATT_UUID_CHAR_RSCS_MEASUREMENT      0x2A53
#define GATT_UUID_CHAR_RSCS_FEATURE          0x2A54
#define GATT_UUID_CHAR_SC_CONTROL_POINT     0x2A55
#define GATT_UUID_CHAR_SENSOR_LOCATION      0x2A5D


#define RSCS_MULTIPLE_SENSOR_LOCATIONS_SUPPORT
#define RSCS_SC_CONTROL_POINT_SUPPORT
/** @brief  Index of each characteristic in RSC service database. */
#define GATT_SVC_RSCS_MEASUREMENT_INDEX      2
#define GATT_SVC_RSCS_FEATURE_INDEX          5
#define GATT_SVC_RSCS_SENS_LOC_INDEX         7
#define GATT_SVC_RSCS_CTL_PNT_INDEX          9
#define GATT_SVC_RSCS_MEAS_CCCD_INDEX        (GATT_SVC_RSCS_MEASUREMENT_INDEX + 1)
#define GATT_SVC_RSCS_CTL_PNT_CCCD_INDEX     (GATT_SVC_RSCS_CTL_PNT_INDEX + 1)

/********************************************************************************************************
* local static variables defined here, only used in this source file.
********************************************************************************************************/

typedef struct
{
    uint8_t rsc_measurement_notify_enable: 1;
    uint8_t sc_cp_indicate_enable: 1;
    uint8_t rfu: 6;
} RSCS_NOTIFY_INDICATE_FLAG;

/** @brief  Judge RSC Control Point operation is available or not. */
#define RSCS_CTL_PNT_OPERATE_ACTIVE(x)                     \
    (((x >= RSCS_CP_OPCODE_SET_CUMULATIVE) &&  \
      (x <= RSCS_CP_OPCODE_REQ_SENS_LOC_LIST)) || \
     x == RSCS_CP_OPCODE_RSP_CODE )

/**<  RSC measurement data. */
static T_RSCS_MEASUREMENT rscs_measurement;
/**<  RSC feature that RSC service supported, can be configured by user. */
static uint16_t rscs_feature;
/**<  RSC current sensor location, where the RSC sensor located. */
static uint8_t rscs_cur_sens_location;
/**<  RSC supported sensor location list, can be configured by user. */
static uint16_t rscs_sens_loc_list;
/**<  RSC control point data. */
static T_RSCS_CONTROL_POINT rscs_control_point = {0};
/**<  RSC control point indication enable flag. */
static RSCS_NOTIFY_INDICATE_FLAG rscs_notify_indicate_flag = {0};
/**<  Function pointer used to send event to application from RSC profile. Initiated in rscs_add_service. */
static P_FUN_SERVER_GENERAL_CB pfn_rscs_cb = NULL;

/**< @brief  profile/service definition.  */
const T_ATTRIB_APPL rscs_attr_tbl[] =
{
    /*-------------------------- RSC Service ---------------------------*/
    /* <<Primary Service>>, ..  Index 0 */
    {
        (ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_LE),  /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_PRIMARY_SERVICE),
            HI_WORD(GATT_UUID_PRIMARY_SERVICE),
            LO_WORD(GATT_UUID_RSC),               /* service UUID */
            HI_WORD(GATT_UUID_RSC)
        },
        UUID_16BIT_SIZE,                            /* bValueLen     */
        NULL,                                       /* pValueContext */
        GATT_PERM_READ                              /* wPermissions  */
    },

    /* <<Characteristic>>, ..  Index 1 */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
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
    /*--- RSC Measurement characteristic value --- Index 2 */
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_RSCS_MEASUREMENT),
            HI_WORD(GATT_UUID_CHAR_RSCS_MEASUREMENT)
        },
        0,                                          /* variable size */
        (void *)NULL,
        GATT_PERM_NONE                              /* wPermissions */
    },
    /* client characteristic configuration  Index 3 */
    {
        (ATTRIB_FLAG_VALUE_INCL |                   /* wFlags */
         ATTRIB_FLAG_CCCD_APPL),
        {   /* bTypeValue */
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
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ                       /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*--- RSC Features characteristic value --- Index 5 */
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_RSCS_FEATURE),
            HI_WORD(GATT_UUID_CHAR_RSCS_FEATURE),
        },
        0,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    }

#ifdef RSCS_MULTIPLE_SENSOR_LOCATIONS_SUPPORT
    ,
    /* <<Characteristic>>, ..  Index 6 */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
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
    /*--- RSC sensor location characteristic value --- Index 7 */
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_SENSOR_LOCATION),
            HI_WORD(GATT_UUID_CHAR_SENSOR_LOCATION)
        },
        0,                                          /* variable size */
        (void *)NULL,
        GATT_PERM_READ                              /* wPermissions */
    }
#endif

#ifdef RSCS_SC_CONTROL_POINT_SUPPORT
    ,
    /* <<Characteristic>>, ..  Index 8 */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
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
    /*--- RSC SC Control Point value --- Index 9 */
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
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
        {   /* bTypeValue */
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
/**< @brief  RSC service size definition.  */
const int32_t rscs_attr_tbl_size = sizeof(rscs_attr_tbl);

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
bool rscs_set_parameter(T_RSCS_PARAM_TYPE param_type, uint8_t len, void *p_value)
{
    bool ret = true;
    uint8_t inc_flag = rscs_measurement.value[0];

    switch (param_type)
    {
    default:
        ret = false;
        break;
    case RSCS_PARAM_CSCS_FEATURE:
        memcpy(&rscs_feature, p_value, 2);
        break;
    case RSCS_PARAM_INC_FLAG:
        rscs_measurement.value[0] = *(uint8_t *)p_value;
        inc_flag = rscs_measurement.value[0];
        rscs_measurement.cur_length = 4;
        if (inc_flag & RSCS_INC_STRIDE_LENGTH_MASK)
        {
            rscs_measurement.cur_length += 2;
        }
        if (inc_flag & RSCS_INC_TOTAL_DISTANCE_MASK)
        {
            rscs_measurement.cur_length += 4;
        }
        break;
    case RSCS_PARAM_SPEED:
        LE_UINT16_TO_ARRAY(rscs_measurement.value + 1, *(uint16_t *)p_value);
        break;
    case RSCS_PARAM_CADENCE:
        rscs_measurement.value[3] = *(uint8_t *)p_value;
        break;
    case RSCS_PARAM_STRIDE_LENGTH:
        if (inc_flag & RSCS_INC_STRIDE_LENGTH_MASK)
        {
            LE_UINT16_TO_ARRAY(rscs_measurement.value + 4, *(uint16_t *)p_value) ;
        }
        else
        {
            ret = false;
        }
        break;
    case RSCS_PARAM_TOTALDISTANCE:
        if (inc_flag & RSCS_INC_TOTAL_DISTANCE_MASK)
        {
            if (inc_flag & RSCS_INC_STRIDE_LENGTH_MASK)
            {
                LE_UINT32_TO_ARRAY(rscs_measurement.value + 6, *(uint32_t *)p_value);
            }
            else
            {
                LE_UINT32_TO_ARRAY(rscs_measurement.value + 4, *(uint32_t *)p_value);
            }
        }
        else
        {
            ret = false;
        }
        break;
    case RSCS_PARAM_SENSOR_LOC:
        rscs_cur_sens_location = *(uint8_t *)p_value;
        break;
    case RSCS_PARAM_CTL_PNT_PROG_CLR:
        rscs_control_point.value[0] = RSCS_CP_OPCODE_RESERVED;
        break;
    }

    if (!ret)
    {
        PROFILE_PRINT_INFO0("rscs parameter set failed\n");
    }

    return ret;
}

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
bool rscs_get_parameter(T_RSCS_PARAM_TYPE param_type, void *p_value)
{
    bool ret = true;
    uint8_t inc_flag = rscs_measurement.value[0];

    switch (param_type)
    {
    default:
        ret = false;
        break;
    case RSCS_PARAM_INC_FLAG:
        *(uint8_t *)p_value = rscs_measurement.value[0];
        break;
    case RSCS_PARAM_SPEED:
        LE_ARRAY_TO_UINT16(*(uint16_t *)p_value, rscs_measurement.value + 1);
        break;
    case RSCS_PARAM_CADENCE:
        *(uint8_t *)p_value = rscs_measurement.value[3];
        break;
    case RSCS_PARAM_STRIDE_LENGTH:
        if (inc_flag & RSCS_INC_STRIDE_LENGTH_MASK)
        {
            LE_ARRAY_TO_UINT16(*(uint16_t *)p_value, rscs_measurement.value + 4);
        }
        else
        {
            ret = false;
        }
        break;
    case RSCS_PARAM_TOTALDISTANCE:
        if (inc_flag & RSCS_INC_TOTAL_DISTANCE_MASK)
        {
            if (inc_flag & RSCS_INC_STRIDE_LENGTH_MASK)
            {
                LE_ARRAY_TO_UINT32(*(uint32_t *)p_value, rscs_measurement.value + 6);
            }
            else
            {
                LE_ARRAY_TO_UINT32(*(uint32_t *)p_value, rscs_measurement.value + 4);
            }
        }
        else
        {
            ret = false;
        }
        break;
    case RSCS_PARAM_SENSOR_LOC:
        *(uint8_t *)p_value = rscs_cur_sens_location;
        break;
    }

    if (!ret)
    {
        PROFILE_PRINT_INFO0("rscs parameter get failed\n");
    }

    return ret;
}

/**
 * @brief  display control point response.
 *
 * @param RSCS_ctl_pnt_ptr       pointer to RSC control point data.
 * @return none
 * @retval  void
*/
static void rscs_ctl_pnt_display_rsp(T_RSCS_CONTROL_POINT *rscs_ctl_pnt_ptr)
{
    PROFILE_PRINT_INFO1("RSCCP response: ReqOpCode=0x%x", rscs_ctl_pnt_ptr->value[1]);
    PROFILE_PRINT_INFO1("rsp_code=0x%x", rscs_ctl_pnt_ptr->value[2]);
}

/**
 * @brief  whether the sensor location supported.
 *
 * @param sens_location     sensor location to be judged(RSC sensor support or not).
 * @return support result.
 * @retval  1   support
 * @retval  0   not support
*/
static bool rscs_sens_loc_supported(uint8_t sens_location)
{
    uint8_t result = false;
    if ((rscs_sens_loc_list >> sens_location) & 0x0001)
    {
        result = true;
    }
    return (result);
}

/**
 * @brief  send response to client, control point indication procedure.
 *
 * @param service_id         service ID of service.
 * @param op_code            control point request opcode type.
 * @param rsp_code           control point response code to indicate to client.
 * @return indication action result
 * @retval  1 TRUE
 * @retval  0 FALSE
*/
bool rscs_ctl_pnt_indicate(uint8_t conn_id, T_SERVER_ID service_id, uint8_t op_code,
                           uint8_t rsp_code)
{
    uint16_t attrib_index = GATT_SVC_RSCS_CTL_PNT_INDEX;
    uint8_t sens_location;
    uint8_t param_offset;
    uint8_t *p_data;
    uint16_t dataLen;

    rscs_control_point.value[1] = rscs_control_point.value[0];  /* Control Point request opcode. */
    rscs_control_point.value[2] = rsp_code;
    rscs_control_point.value[0] = RSCS_CP_OPCODE_RSP_CODE;
    rscs_control_point.cur_length = 3 * sizeof(uint8_t);

    /* Diff RspType, different indication contents. */
    if (op_code == RSCS_CP_OPCODE_REQ_SENS_LOC_LIST)
    {
        /* get sensor location list */
        for (sens_location = RSCS_SENSOR_LOC_OTHER, param_offset = 3; sens_location < RSCS_SENSOR_LOC_MAX;
             sens_location++)
        {
            if (rscs_sens_loc_supported(sens_location))
            {
                rscs_control_point.value[param_offset] = sens_location;
                param_offset++;
            }
        }
        rscs_control_point.cur_length = param_offset;
    }

    rscs_ctl_pnt_display_rsp(&rscs_control_point);
    p_data = rscs_control_point.value;
    dataLen = rscs_control_point.cur_length;

    // send indication to client
    return server_send_data(conn_id, service_id, attrib_index, p_data, dataLen,
                            GATT_PDU_TYPE_INDICATION);
}


/**
 * @brief  set cumulative response.
 *
 * @param service_id
 * @return none
 * @retval  void
*/
static void rscs_ctl_pnt_set_cumulative(uint8_t conn_id, T_SERVER_ID service_id, uint8_t rsp_code)
{
    uint8_t op_code = RSCS_CP_OPCODE_SET_CUMULATIVE;
    rscs_ctl_pnt_indicate(conn_id, service_id, op_code, rsp_code);
}


static void rscs_ctl_pnt_start_calib(uint8_t conn_id, T_SERVER_ID service_id, bool result)
{
    uint8_t op_code = RSCS_CP_OPCODE_SET_CUMULATIVE;
    uint8_t rsp_code;
    if (result == true)
    {
        rsp_code = RSCS_CP_RSPCODE_SUCCESS;
    }
    else
    {
        rsp_code = RSCS_CP_RSPCODE_OPERATION_FAILED;
    }
    rscs_ctl_pnt_indicate(conn_id, service_id, op_code, rsp_code);
}

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
void rscs_calib_resutl_indicate(uint8_t conn_id, T_SERVER_ID service_id, bool result)
{
    rscs_ctl_pnt_start_calib(conn_id, service_id, result);
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
static void rscs_ctl_pnt_update_sens_loc(uint8_t conn_id, T_SERVER_ID service_id, uint8_t rsp_code)
{

    uint8_t op_code = RSCS_CP_OPCODE_UPDATE_SENS_LOC;
    rscs_ctl_pnt_indicate(conn_id, service_id, op_code, rsp_code);
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
static void rscs_ctl_pnt_req_sens_loc_list(uint8_t conn_id, T_SERVER_ID service_id,
                                           uint8_t rsp_code)
{
    uint8_t op_code = RSCS_CP_OPCODE_REQ_SENS_LOC_LIST;
    rscs_ctl_pnt_indicate(conn_id, service_id, op_code, rsp_code);
}


static uint8_t rscs_hanlde_ctl_pnt_proc_2(uint8_t service_id, uint16_t write_length,
                                          uint8_t *value_ptr)
{
    T_RSCS_CALLBACK_DATA callback_data;
    uint8_t resp_code = RSCS_CP_RSPCODE_SUCCESS;
    uint16_t parameter_length = 0;
    memcpy(rscs_control_point.value, value_ptr, write_length);
    if (write_length >= 1)
    {
        parameter_length = write_length - 1;
    }

    PROFILE_PRINT_INFO1("rscs_hanlde_ctl_pnt_proc_2 request: OpCode=0x%x", rscs_control_point.value[0]);

    callback_data.msg_type = SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE;
    callback_data.msg_data.write.opcode = rscs_control_point.value[0];

    switch (rscs_control_point.value[0])
    {

    case RSCS_CP_OPCODE_SET_CUMULATIVE:
        {
            if (parameter_length == 4)
            {
                memcpy(&callback_data.msg_data.write.cp_parameter.cumulative_value,
                       &rscs_control_point.value[1], 4);
            }
            else
            {
                resp_code = RSCS_CP_RSPCODE_INVALID_PARAMETER;
            }
        }

        break;

    case RSCS_CP_OPCODE_START_CALIBRATION:
        {
            if (parameter_length == 0)
            {
            }
            else
            {
                resp_code = RSCS_CP_RSPCODE_INVALID_PARAMETER;
            }
        }
        break;
    case RSCS_CP_OPCODE_UPDATE_SENS_LOC:
        {
            if (parameter_length == 1)
            {
                memcpy(&callback_data.msg_data.write.cp_parameter.sensor_location_value,
                       &rscs_control_point.value[1], 1);
                if (callback_data.msg_data.write.cp_parameter.sensor_location_value >= RSCS_SENSOR_LOC_MAX)
                {
                    resp_code = RSCS_CP_RSPCODE_INVALID_PARAMETER;
                }
            }
            else
            {
                resp_code = RSCS_CP_RSPCODE_INVALID_PARAMETER;
            }
        }
        break;

    case RSCS_CP_OPCODE_REQ_SENS_LOC_LIST:
        {
            if (parameter_length == 0)
            {
            }
            else
            {
                resp_code = RSCS_CP_RSPCODE_INVALID_PARAMETER;
            }
        }
        break;

    default:
        {
            resp_code = RSCS_CP_RSPCODE_OPCODE_UNSUPPORT;
        }
        break;
    }

    if (resp_code == RSCS_CP_RSPCODE_SUCCESS)
    {
        pfn_rscs_cb(service_id, (void *)&callback_data);
    }
    return resp_code;
}



/**
 * @brief  handle control point write (request).
 *
 * @param[in] conn_id  Connection id.
 * @param[in] service_id Service ID.
 * @param[in] write_length      write request data length.
 * @param[in] value_ptr         pointer to write request data.
 * @return none
 * @retval  void
*/
static void rscs_ctl_pnt_handle_req(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                                    uint16_t write_length, uint8_t *value_ptr)
{
    uint8_t resp_code = RSCS_CP_RSPCODE_SUCCESS;
    memcpy(rscs_control_point.value, value_ptr, write_length);
    rscs_control_point.cur_length = write_length;

    PROFILE_PRINT_INFO1("rscscp request: op_code=0x%x", rscs_control_point.value[0]);

    resp_code = rscs_hanlde_ctl_pnt_proc_2(service_id, write_length, value_ptr);

    if (resp_code == RSCS_CP_RSPCODE_SUCCESS)
    {

        switch (rscs_control_point.value[0])
        {
        default:
            break;
        case RSCS_CP_OPCODE_SET_CUMULATIVE:
            if ((rscs_feature & RSCS_SUPPORT_TOTAL_DISTANCE_MASK) &&
                (rscs_measurement.value[0] & RSCS_INC_TOTAL_DISTANCE_MASK))
            {
                rscs_ctl_pnt_set_cumulative(conn_id, service_id, resp_code);
                return;
            }
            break;
        case RSCS_CP_OPCODE_START_CALIBRATION:
            if (rscs_feature & RSCS_SUPPORT_CALIBRATE_MASK)
            {

                return;
            }
            break;
        case RSCS_CP_OPCODE_UPDATE_SENS_LOC:
            if (rscs_feature & RSCS_SUPPORT_MULTI_SENSOR_MASK)
            {
                rscs_ctl_pnt_update_sens_loc(conn_id, service_id, resp_code);
                return;
            }
            break;
        case RSCS_CP_OPCODE_REQ_SENS_LOC_LIST:
            if (rscs_feature & RSCS_SUPPORT_MULTI_SENSOR_MASK)
            {
                rscs_ctl_pnt_req_sens_loc_list(conn_id, service_id, resp_code);
                return;
            }
            break;
        }
    }

    /* Send indication to client when request opcode not supported. */
    rscs_ctl_pnt_indicate(conn_id, service_id, rscs_control_point.value[0], resp_code);
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
T_APP_RESULT rscs_attr_read_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                               uint16_t offset, uint16_t *p_length, uint8_t **pp_value)
{
    T_APP_RESULT cause  = APP_RESULT_SUCCESS;

    switch (attrib_index)
    {
    default:
        PROFILE_PRINT_ERROR1("rscs_attr_read_cb, Attr not found, index = %d", attrib_index);
        cause  = APP_RESULT_ATTR_NOT_FOUND;
        break;
    case GATT_SVC_RSCS_FEATURE_INDEX:
        {
            T_RSCS_CALLBACK_DATA callback_data;
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE;
            callback_data.msg_data.read_value_index = RSCS_READ_RSCS_FEATURE;
            pfn_rscs_cb(service_id, &callback_data);

            *pp_value = (uint8_t *)&rscs_feature;
            *p_length = sizeof(rscs_feature);

        }
        break;
    case GATT_SVC_RSCS_SENS_LOC_INDEX:
        if (rscs_feature & RSCS_SUPPORT_MULTI_SENSOR_MASK)
        {
            T_RSCS_CALLBACK_DATA callback_data;
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE;
            callback_data.msg_data.read_value_index = RSCS_READ_SENSOR_LOCATION;
            pfn_rscs_cb(service_id, &callback_data);
            *pp_value = (uint8_t *)&rscs_cur_sens_location;
            *p_length = sizeof(rscs_cur_sens_location);
        }
        else
        {
            cause  = APP_RESULT_ATTR_NOT_FOUND;
        }
        break;
    }

    return cause;
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
T_APP_RESULT rscs_attr_write_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                                T_WRITE_TYPE write_type, uint16_t length, uint8_t *p_value,
                                P_FUN_WRITE_IND_POST_PROC *p_write_ind_post_proc)
{
    T_APP_RESULT cause = APP_RESULT_SUCCESS;
    if (GATT_SVC_RSCS_CTL_PNT_INDEX == attrib_index)
    {
        /* Attribute value has variable size, make sure written value size is valid. */
        if ((length > RSCS_MAX_CTL_PNT_VALUE) || (p_value == NULL))
        {
            cause  = APP_RESULT_INVALID_VALUE_SIZE;
        }
        /* Make sure Control Point is not "Process already in progress". */
        else if (RSCS_CTL_PNT_OPERATE_ACTIVE(rscs_control_point.value[0]))
        {
            cause = (T_APP_RESULT)0x80;//ProfileResult_AppErr_ProcAlreadyInProgress
        }
        /* Make sure Control Point is configured indication enable. */
        else if (!rscs_notify_indicate_flag.sc_cp_indicate_enable)
        {
            cause = (T_APP_RESULT)0x81;//ProfileResult_AppErr_CccdImproperlyConfigured
        }
        else
        {
            /* handle SCCP request after sending write response */
            *p_write_ind_post_proc = rscs_ctl_pnt_handle_req;
        }
    }
    else
    {
        PROFILE_PRINT_ERROR2("rscs_attr_write_cb Error attrib_index = 0x%x length=%d", attrib_index,
                             length);
        cause = APP_RESULT_ATTR_NOT_FOUND;
    }

    return cause;
}

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
bool rscs_meas_value_notify(uint8_t conn_id, T_SERVER_ID service_id)
{
    uint16_t attrib_index = GATT_SVC_RSCS_MEASUREMENT_INDEX;
    uint8_t *p_data = rscs_measurement.value;
    uint16_t dataLen = rscs_measurement.cur_length;

    PROFILE_PRINT_INFO0("rscs measurement notification");
    return server_send_data(conn_id, service_id, attrib_index, p_data, dataLen,
                            GATT_PDU_TYPE_NOTIFICATION);
}


/**
 * @brief update CCCD bits from stack.
 *
 * @param[in] conn_id  Connection id.
 * @param[in] service_id          Service ID.
 * @param[in] index          Attribute index of characteristic data.
 * @param[in] ccc_bits         CCCD bits from stack.
 * @return None
*/
void rscs_cccd_update_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t index, uint16_t ccc_bits)
{
    T_RSCS_CALLBACK_DATA callback_data;
    bool handle = false;
    PROFILE_PRINT_INFO2("rscs_cccd_update_cb index = %d ccc_bits %x", index, ccc_bits);
    switch (index)
    {
    case GATT_SVC_RSCS_MEAS_CCCD_INDEX:
        if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_NOTIFY)
        {
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION;
            callback_data.msg_data.notification_indification_index = RSCS_NOTIFY_INDICATE_MEASUREMENT_ENABLE;
            rscs_notify_indicate_flag.rsc_measurement_notify_enable = true;
        }
        else
        {
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION;
            callback_data.msg_data.notification_indification_index = RSCS_NOTIFY_INDICATE_MEASUREMENT_ENABLE;
            rscs_notify_indicate_flag.rsc_measurement_notify_enable = false;
        }
        handle = true;
        break;

    case GATT_SVC_RSCS_CTL_PNT_CCCD_INDEX:
        if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_INDICATE)
        {
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION;
            callback_data.msg_data.notification_indification_index = RSCS_NOTIFY_INDICATE_SC_CP_ENABLE;
            rscs_notify_indicate_flag.sc_cp_indicate_enable = true;
        }
        else
        {
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION;
            callback_data.msg_data.notification_indification_index = RSCS_NOTIFY_INDICATE_SC_CP_DISABLE;
            rscs_notify_indicate_flag.sc_cp_indicate_enable = false;
        }
        handle = true;
        break;
    default:
        break;
    }
    /* Notify Application. */
    if (pfn_rscs_cb && (handle == true))
    {
        pfn_rscs_cb(service_id, (void *)&callback_data);
    }
}

/**
 * @brief RSC Service Callbacks.
*/
const T_FUN_GATT_SERVICE_CBS rscs_cbs =
{
    rscs_attr_read_cb,  // Read callback function pointer
    rscs_attr_write_cb, // Write callback function pointer
    rscs_cccd_update_cb  // CCCD update callback function pointer
};

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
T_SERVER_ID rscs_add_service(void *p_func)
{
    T_SERVER_ID service_id;

    /* Initiate RSC service related data, modify according to user's demand. */
    rscs_feature = RSCS_ALL_FEATURE_SUPPORTED;
    rscs_measurement.value[0] = RSCS_INC_ALL_PRESENTS;
    rscs_measurement.cur_length = RSCS_MAX_MEASUREMENT_VALUE;
    rscs_sens_loc_list = RSCS_ALL_SENS_LOC_SUPPORTED;
    rscs_cur_sens_location = RSCS_SENSOR_LOC_TOP_OF_SHOE;

    /* register RSC service to profile layer. */
    if (false == server_add_service(&service_id,
                                    (uint8_t *)rscs_attr_tbl,
                                    rscs_attr_tbl_size,
                                    rscs_cbs))
    {
        PROFILE_PRINT_ERROR1("rscs_add_service: service_id %d", service_id);
        service_id = 0xff;
        return service_id;
    }

    /* register callback for profile to inform application that some events happened. */
    pfn_rscs_cb = (P_FUN_SERVER_GENERAL_CB)p_func;
    return service_id;
}


