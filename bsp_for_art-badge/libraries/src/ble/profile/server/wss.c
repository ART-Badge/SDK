/**
*********************************************************************************************************
*               Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     wss.c
* @brief    weight scale service source file.
* @details  Interface to access the weight scale service.
* @author   Vera
* @date
* @version  v1.0
*********************************************************************************************************
*/
#include "trace.h"
#include <string.h>
#include "gatt.h"
#include "wss.h"


/* Weight scale service UUID */
#define GATT_UUID_WEIGHT_SCALE_SERVICE              0x181D
/* Weight scale feature characteristic UUID*/
#define GATT_UUID_CHAR_WEIGHT_SCALE_FEATURE_READ    0x2A9E
/* Weight measurement characteristic UUID*/
#define GATT_UUID_CHAR_WEIGHT_MEASUREMENT_INDICATE  0x2A9D

/** @brief The Maximum Length of Weight Measurement Value*/
#define WSS_MEASUREMENT_VALUE_MAX_LEN  15


/** @brief  Index of each characteristic in weight scale service database. */
#define WEIGHT_SCALE_SERVICE_CHAR_FEATURE_READ_INDEX          0x02
#define WEIGHT_SCALE_SERVICE_CHAR_MEASUREMENT_INDICATE_INDEX  0x04
#define WEIGHT_SCALE_SERVICE_CHAR_INDICATE_CCCD_INDEX         (WEIGHT_SCALE_SERVICE_CHAR_MEASUREMENT_INDICATE_INDEX + 1)

#define WSS_FEATURE_TIMESTAMP_SUPPORTED            1
#define WSS_FEATURE_MULTIUSERS_SUPPORTED           1
#define WSS_FEATURE_BMI_SUPPORTED                  1
#define WSS_FEATURE_WEIGHTMEASUREMENT_RESOLUTION   1
#define WSS_FEATURE_HEIGHTMEASUREMENT_RESOLUTION   1

/********************************************************************************************************
* local static variables defined here, only used in this source file.
********************************************************************************************************/
/**<  Value of weight scale feature read characteristic. */
static T_WEIGHT_SCALE_FEATURE_VALUE weight_scale_char_read_value =
{
    WSS_FEATURE_TIMESTAMP_SUPPORTED,
    WSS_FEATURE_MULTIUSERS_SUPPORTED,
    WSS_FEATURE_BMI_SUPPORTED,
    WSS_FEATURE_WEIGHTMEASUREMENT_RESOLUTION,
    WSS_FEATURE_HEIGHTMEASUREMENT_RESOLUTION,
    0
};

/**<  Function pointer used to send event to application from Weight Scale Service. Initiated in wss_add_service. */
static P_FUN_SERVER_GENERAL_CB pfn_wss_cb = NULL;
/**<  Measurement Related Value. */
static T_WSS_MEASUREMENT wss_measurement = {0};
static uint8_t wss_measurement_value_for_indicate[WSS_MEASUREMENT_VALUE_MAX_LEN];
static uint8_t wss_measurement_value_actual_length = 0;

/**< @brief  service definition.  */
/**<  Attribute Table */
static const T_ATTRIB_APPL wss_tbl[] =
{
    /* <<Primary Service>>, .. 0*/
    {
        (ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_LE),  /* wFlags     */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_PRIMARY_SERVICE),
            HI_WORD(GATT_UUID_PRIMARY_SERVICE),
            LO_WORD(GATT_UUID_WEIGHT_SCALE_SERVICE),      /* service UUID */
            HI_WORD(GATT_UUID_WEIGHT_SCALE_SERVICE)
        },
        UUID_16BIT_SIZE,                            /* bValueLen     */
        NULL,                                       /* pValueContext */
        GATT_PERM_READ                              /* wPermissions  */
    },
    /* <<Characteristic>> for read , .. 1*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ  /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /* Weight Scale Feature value , .. 2 */
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_WEIGHT_SCALE_FEATURE_READ),
            HI_WORD(GATT_UUID_CHAR_WEIGHT_SCALE_FEATURE_READ)
        },
        0,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },

    /* <<Characteristic>> for indicate , .. 3 */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                          /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            (GATT_CHAR_PROP_INDICATE)               /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /* Weight Measurement value , .. 4 */
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                         /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_WEIGHT_MEASUREMENT_INDICATE),
            HI_WORD(GATT_UUID_CHAR_WEIGHT_MEASUREMENT_INDICATE)
        },
        0,                                          /* bValueLen */
        NULL,
        GATT_PERM_NONE                              /* wPermissions */
    },
    /* client characteristic configuration , .. 5 */
    {
        ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_CCCD_APPL,                 /* wFlags */
        {                                         /* bTypeValue */
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

};

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
bool wss_set_parameter(T_WSS_PARAM_TYPE param_type, uint8_t len, void *p_value)
{
    bool ret = true;

    switch (param_type)
    {
    default:
        {
            ret = false;
            PROFILE_PRINT_ERROR0("wss_set_parameter failed\n");
        }
        break;
    case WSS_PARAM_MEASUREMENT_WEIGHTPARAM_FLAG:
        {
            if (len != 1)
            {
                PROFILE_PRINT_ERROR0("wss_set_parameter measurement weightParam Flag \n");
            }
            memcpy(&wss_measurement.flag, p_value, len);
        }
        break;
    case WSS_PARAM_MEASUREMENT_WEIGHT_VALUE:
        {
            if (len != 2)
            {
                PROFILE_PRINT_ERROR0("wss_set_parameter measurement weight value \n");
                break;
            }
            if (wss_measurement.flag.measurement_units == 0)
            {
                memcpy(&wss_measurement.weight_si_value, p_value, len);
            }
            else
            {
                memcpy(&wss_measurement.weight_imperial_value, p_value, len);
            }
        }
        break;
    case WSS_PARAM_MEASUREMENT_TIME_STAMP:
        {
            if (len != sizeof(T_WSS_TIME_STAMP))
            {
                PROFILE_PRINT_ERROR0("wss_set_parameter weight measurement time stamp \n");
                break;
            }

            memcpy(&wss_measurement.time_stamp, p_value, len);

        }
        break;
    case WSS_PARAM_MEASUREMENT_USERID:
        {
            if (len != 1)
            {
                PROFILE_PRINT_ERROR0("wss_set_parameter weight measurement user_id \n");
                break;
            }

            memcpy(&wss_measurement.user_id, p_value, len);
        }
        break;
    case WSS_PARAM_MEASUREMENT_BMI:
        {
            if (len != 2)
            {
                PROFILE_PRINT_ERROR0("wss_set_parameter weight measurement bmi \n");
                break;
            }

            memcpy(&wss_measurement.bmi, p_value, len);

        }
        break;
    case WSS_PARAM_MEASUREMENT_HEIGHT_VALUE:
        {
            if (len != 2)
            {
                PROFILE_PRINT_ERROR0("wss_set_parameter height value \n");
                break;
            }
            if (wss_measurement.flag.measurement_units == 0)
            {
                memcpy(&wss_measurement.height_si_value, p_value, len);
            }
            else
            {
                memcpy(&wss_measurement.height_imperial_value, p_value, len);
            }

        }
        break;
    case WSS_PARAM_FEATURE_READ_CHAR_VAL:
        {
            if (len != 4)
            {
                PROFILE_PRINT_ERROR0("wss_set_parameter feature value \n");
                break;
            }
            memcpy(&weight_scale_char_read_value, p_value, len);

        }
    }

    if (!ret)
    {
        PROFILE_PRINT_ERROR0("wss_set_parameter failed\n");
    }
    if (ret)
    {
        PROFILE_PRINT_INFO0("wss_set_parameter success\n");
    }

    return ret;
}


void wss_format_measurement_value()
{
    uint8_t cur_offset = 0;

    memcpy(&wss_measurement_value_for_indicate[cur_offset], &wss_measurement.flag, 1);
    cur_offset += 1;

    if (wss_measurement.flag.measurement_units == 0)
    {
        memcpy(&wss_measurement_value_for_indicate[cur_offset], &wss_measurement.weight_si_value, 2);
    }
    else
    {
        memcpy(&wss_measurement_value_for_indicate[cur_offset], &wss_measurement.weight_imperial_value, 2);
    }
    cur_offset += 2;

    if (wss_measurement.flag.time_stamp_present)
    {
        memcpy(&wss_measurement_value_for_indicate[cur_offset], &wss_measurement.time_stamp, 7);
        cur_offset += 7;
    }

    if (wss_measurement.flag.user_id_present)
    {
        memcpy(&wss_measurement_value_for_indicate[cur_offset], &wss_measurement.user_id, 1);
        cur_offset += 1;
    }
    if (wss_measurement.flag.bmi_and_height_present)
    {
        memcpy(&wss_measurement_value_for_indicate[cur_offset], &wss_measurement.bmi, 2);
        cur_offset += 2;
        if (wss_measurement.flag.measurement_units == 0)
        {
            memcpy(&wss_measurement_value_for_indicate[cur_offset], &wss_measurement.height_si_value, 2);
        }
        else
        {
            memcpy(&wss_measurement_value_for_indicate[cur_offset], &wss_measurement.height_imperial_value, 2);
        }
        cur_offset += 2;
    }

    wss_measurement_value_actual_length = cur_offset;

}


/**
 * @brief read characteristic data from service.
 *
 * @param[in] conn_id  Connection id.
 * @param[in] service_id          ServiceID of characteristic data.
 * @param[in] attrib_index          Attribute index of getting characteristic data.
 * @param[in] offset                Used for Blob Read.
 * @param[in,out] p_length          Length of getting characteristic data.
 * @param[in,out] pp_value          Data got from service.
 * @return Profile procedure result
*/
T_APP_RESULT wss_attr_read_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                              uint16_t offset, uint16_t *p_length, uint8_t **pp_value)
{
    T_APP_RESULT  wCause  = APP_RESULT_SUCCESS;

    switch (attrib_index)
    {
    default:
        PROFILE_PRINT_ERROR1("wss_attr_read_cb, Attr not found, index=%d", attrib_index);
        wCause  = APP_RESULT_ATTR_NOT_FOUND;
        break;
    case WEIGHT_SCALE_SERVICE_CHAR_FEATURE_READ_INDEX:
        {
            PROFILE_PRINT_INFO1("wss_attr_read_cb, index = %d", attrib_index);

            T_WSS_CALLBACK_DATA callback_data;
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE;
            callback_data.msg_data.read_value_index = WEIGHT_SCALE_READ_FEATURE;
            pfn_wss_cb(service_id, (void *)&callback_data);
            *pp_value  = (uint8_t *)&weight_scale_char_read_value;
            *p_length = sizeof(weight_scale_char_read_value);
            PROFILE_PRINT_INFO2("wss_attr_read_cb weight_scale_char_read_value = %x,Length = %d",
                                weight_scale_char_read_value, *p_length);
        }
        break;
    }

    return (wCause);
}

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
bool wss_measurement_indicate(uint8_t conn_id, T_SERVER_ID service_id)
{
    wss_format_measurement_value();

    PROFILE_PRINT_INFO0("WeightScaleService_Measurement_Indicate");
    // send indication to client
    return server_send_data(conn_id, service_id, WEIGHT_SCALE_SERVICE_CHAR_MEASUREMENT_INDICATE_INDEX,
                            (uint8_t *)&wss_measurement_value_for_indicate, wss_measurement_value_actual_length,
                            GATT_PDU_TYPE_INDICATION);
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
void wss_cccd_update_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t index, uint16_t ccc_bits)
{
    T_WSS_CALLBACK_DATA callback_data;
    bool handle = false;
    PROFILE_PRINT_INFO2("wss_cccd_update_cb Index = %d wCCCDBits %x", index, ccc_bits);
    switch (index)
    {
    case WEIGHT_SCALE_SERVICE_CHAR_INDICATE_CCCD_INDEX:
        {
            if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_INDICATE)
            {
                // Enable Indication
                callback_data.msg_type = SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION;
                callback_data.msg_data.notification_indication_index = WSS_INDICATE_WEIGHT_MEASUREMENT_ENABLE;
            }
            else
            {
                // Disable Indication
                callback_data.msg_type = SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION;
                callback_data.msg_data.notification_indication_index = WSS_INDICATE_WEIGHT_MEASUREMENT_DISABLE;
            }
            handle =  true;
        }
        break;
    default:
        break;
    }
    if (pfn_wss_cb && (handle == true))
    {
        pfn_wss_cb(service_id, (void *)&callback_data);
    }
}

/**
 * @brief Weight Scale Service Callbacks.
*/
const T_FUN_GATT_SERVICE_CBS wss_cbs =
{
    wss_attr_read_cb,  // Read callback function pointer
    NULL, // Write callback function pointer
    wss_cccd_update_cb  // CCCD update callback function pointer
};

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
T_SERVER_ID wss_add_service(void *p_func)
{
    T_SERVER_ID service_id;
    if (false == server_add_service(&service_id,
                                    (uint8_t *)wss_tbl,
                                    sizeof(wss_tbl),
                                    wss_cbs))
    {
        PROFILE_PRINT_ERROR1("wss_add_service: ServiceId %d", service_id);
        service_id = 0xff;
        return service_id;
    }
    PROFILE_PRINT_INFO1("wss_add_service: ServiceId %d", service_id);
    pfn_wss_cb = (P_FUN_SERVER_GENERAL_CB)p_func;
    return service_id;
}

