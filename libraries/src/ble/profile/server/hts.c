/**
*********************************************************************************************************
*               Copyright(c) 2014, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     hts.c
* @brief    Health Thermometer service source file.
* @details  Interfaces to access Health Thermometer service.
* @author
* @date     2017-9-20
* @version  v1.0
*********************************************************************************************************
*/
#include "gatt.h"
#include "string.h"
#include "trace.h"
#include "hts.h"


/********************************************************************************************************
* local static variables defined here, only used in this source file.
********************************************************************************************************/
/**<  Temperature Type value. */
#define HTS_TEMPERATURE_TYPE_CHAR_SUPPORT
#define HTS_INTERMEDIATE_TEMPERATURE_CHAR_SUPPORT
#define HTS_MEASUREMENT_INTERVAL_CHAR_SUPPORT
#define HTS_MEASUREMENT_INTERVAL_CHAR_INDICATE_SUPPORT
#define HTS_MEASUREMENT_INTERVAL_CHAR_WRITE_SUPPORT


#define HTS_TEMPERATURE_MEASUREMENT_INDEX                   2
#define HTS_TEMPERATURE_TYPE_INDEX                          5
#define HTS_INTERMEDIATE_TEMPERATURE_INDEX                  7
#define HTS_MEASUREMENT_INTERVAL_INDEX                      10
#define HTS_MEASUREMENT_INTERVAL_VALID_RANGE_INDEX          12

#define HTS_TEMPERATURE_MEASUREMENT_CCCD_INDEX              3
#define HTS_INTERMEDIATE_TEMPERATURE_CCCD_INDEX             8
#define HTS_MEASUREMENT_INTERVAL_CCCD_INDEX                 11

#define GATT_UUID_HELTH_TEMPERATURE_SERVICE                 0x1809
#define GATT_UUID_CHAR_HTS_MEASUREMENT_INTERVAL             0x2A21
#define GATT_UUID_CHAR_HTS_INTERMEDIATE_TMPERATURE          0x2A1E
#define GATT_UUID_CHAR_HTS_TEMPERATURE_MEASUREMENT          0x2a1c
#define GATT_UUID_CHAR_HTS_TEMPERATURE_TYPE                 0x2a1d

#define HTS_MEASUREMENT_VALUE_MAX_LEN  13

typedef struct
{
    T_HEALTH_THERMOMETER_MEASUREMENT_VALUE_FLAG flag;
    uint32_t temp_celsius_value;
    uint32_t temp_fahrenheit_value;
    T_TIME_STAMP  time_stamp;
    uint8_t temper_next_desc;
} T_HTS_MESAURMENT;

static T_HTS_TEMPERATURE_TYPE temperature_type = HTS_TEMPERATURE_TYPE_ARMPIT;
static uint16_t hts_measurement_interval = 50;
static T_HTS_MEASUREMENT_INTERVAL_VALID_RANGE hts_measurement_interval_valid_range = {0};

static P_FUN_SERVER_GENERAL_CB pfn_hts_cb = NULL;

static T_HTS_MESAURMENT hts_measurement = {0};
static uint8_t hts_measurement_value_for_notify_indicate[HTS_MEASUREMENT_VALUE_MAX_LEN];
static uint8_t hts_measurement_value_actual_length = 0;

/**< @brief  profile/service definition.  */
static const T_ATTRIB_APPL hts_attr_tbl[] =
{
    /*----------------- Health Temperature Service -------------------*/
    /* <<Primary Service>>, .. 0,*/
    {
        (ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_LE),   /* wFlags     */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_PRIMARY_SERVICE),
            HI_WORD(GATT_UUID_PRIMARY_SERVICE),
            LO_WORD(GATT_UUID_HELTH_TEMPERATURE_SERVICE),              /* service UUID */
            HI_WORD(GATT_UUID_HELTH_TEMPERATURE_SERVICE)
        },
        UUID_16BIT_SIZE,                            /* bValueLen     */
        NULL,                                       /* pValueContext */
        GATT_PERM_READ                              /* wPermissions  */
    },
    /* <<Characteristic>>, .. 1,*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            (GATT_CHAR_PROP_INDICATE                     /* characteristic properties */
            )
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
            LO_WORD(GATT_UUID_CHAR_HTS_TEMPERATURE_MEASUREMENT),
            HI_WORD(GATT_UUID_CHAR_HTS_TEMPERATURE_MEASUREMENT)
        },
        0,                                          /* bValueLen */
        NULL,
        GATT_PERM_NONE                              /* wPermissions */
    },
    /* client characteristic configuration 3*/
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
    }

#ifdef HTS_TEMPERATURE_TYPE_CHAR_SUPPORT
    ,
    /* <<Characteristic>>, .. 4*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            (GATT_CHAR_PROP_READ                     /* characteristic properties */
            )
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /* Temperature Type value 5*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_HTS_TEMPERATURE_TYPE),
            HI_WORD(GATT_UUID_CHAR_HTS_TEMPERATURE_TYPE)
        },
        0,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    }
#endif

#ifdef HTS_INTERMEDIATE_TEMPERATURE_CHAR_SUPPORT
    ,
    /* <<Characteristic>>, .. 6*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            (GATT_CHAR_PROP_NOTIFY                   /* characteristic properties */
            )
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /* Intermediate Temperature value,7*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_HTS_INTERMEDIATE_TMPERATURE),
            HI_WORD(GATT_UUID_CHAR_HTS_INTERMEDIATE_TMPERATURE)
        },
        0,                                          /* bValueLen */
        NULL,
        GATT_PERM_NONE                              /* wPermissions */
    },
    /* client characteristic configuration ,8*/
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
    }
#endif

#ifdef HTS_MEASUREMENT_INTERVAL_CHAR_SUPPORT
    ,
    /* <<Characteristic>>, .. ,9*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            (GATT_CHAR_PROP_READ                     /* characteristic properties */
#ifdef HTS_MEASUREMENT_INTERVAL_CHAR_INDICATE_SUPPORT
             | GATT_CHAR_PROP_INDICATE
#endif
#ifdef HTS_MEASUREMENT_INTERVAL_CHAR_WRITE_SUPPORT
             | GATT_CHAR_PROP_WRITE
#endif
            )
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /* Measurement Interval value, 10*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_HTS_MEASUREMENT_INTERVAL),
            HI_WORD(GATT_UUID_CHAR_HTS_MEASUREMENT_INTERVAL)
        },
        0,                                          /* bValueLen */
        NULL,
        (GATT_PERM_READ | GATT_PERM_WRITE) /* wPermissions */
    }

#ifdef HTS_MEASUREMENT_INTERVAL_CHAR_INDICATE_SUPPORT
    ,
    /* client characteristic configuration, 11*/
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
    }
#endif

#ifdef HTS_MEASUREMENT_INTERVAL_CHAR_WRITE_SUPPORT
    ,
    /* valid range descriptor ,12*/
    {
        ATTRIB_FLAG_VALUE_APPL,
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_VALID_RANGE),
            HI_WORD(GATT_UUID_CHAR_VALID_RANGE),

        },
        0,                                          /* bValueLen */
        NULL,
        (GATT_PERM_READ)          /* wPermissions */
    }
#endif
#endif
};
/**< @brief  Health Temperature service size definition.  */
const uint16_t hts_attr_tbl_size = sizeof(hts_attr_tbl);

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
bool hts_set_parameter(T_HTS_PARAM_TYPE param_type, uint8_t len, void *p_value)
{
    bool ret = true;

    switch (param_type)
    {
    default:
        {
            ret = false;
            PROFILE_PRINT_ERROR0("hts_set_parameter failed\n");
        }
        break;
    case HTS_PARAM_MEASUREMENT_TEMPPARAM_FLAG:
        {
            memcpy(&hts_measurement.flag, p_value, 1);
        }
        break;
    case HTS_PARAM_MEASUREMENT_TEMPERATUER_VALUE:
        {
            if (len != 4)
            {
                PROFILE_PRINT_ERROR0("hts_set_parameter Temperature value \n");
                break;
            }
            if (hts_measurement.flag.temp_value_units_bit == 0)
            {
                memcpy(&hts_measurement.temp_celsius_value, p_value, len);
            }
            else
            {
                memcpy(&hts_measurement.temp_fahrenheit_value, p_value, len);
            }
        }
        break;
    case HTS_PARAM_MEASUREMENT_TIME_STAMP:
        {
            if (len != sizeof(T_TIME_STAMP))
            {
                PROFILE_PRINT_ERROR0("hts_set_parameter Temperature value \n");
                break;
            }
            memcpy(&hts_measurement.time_stamp, p_value, len);
        }
        break;
    case HTS_PARAM_MEASUREMENT_TEMPERATURE_TYPE:
        {
            hts_measurement.temper_next_desc = *(uint8_t *)p_value;
        }
        break;

    case HTS_PARAM_MEASUREMENT_INTERVAL:
        memcpy(&hts_measurement_interval, p_value, len);
        break;

    case HTS_PARAM_VALID_RANGE:
        memcpy(&hts_measurement_interval_valid_range, p_value, len);
        break;
    }

    return ret;
}


void hts_format_measurement_value()
{
    uint8_t cur_offset = 0;

    memcpy(&hts_measurement_value_for_notify_indicate[cur_offset], &hts_measurement.flag, 1);
    cur_offset += 1;

    if (hts_measurement.flag.temp_value_units_bit == 0)
    {
        memcpy(&hts_measurement_value_for_notify_indicate[cur_offset], &hts_measurement.temp_celsius_value,
               4);
    }
    else
    {
        memcpy(&hts_measurement_value_for_notify_indicate[cur_offset],
               &hts_measurement.temp_fahrenheit_value, 4);
    }
    cur_offset += 4;

    if (hts_measurement.flag.temp_time_stamp_present_bit)
    {
        memcpy(&hts_measurement_value_for_notify_indicate[cur_offset], &hts_measurement.time_stamp, 7);
        cur_offset += 7;
    }

    if (hts_measurement.flag.temp_type_present_bit)
    {
        memcpy(&hts_measurement_value_for_notify_indicate[cur_offset], &hts_measurement.temper_next_desc,
               1);
        cur_offset += 1;
    }

    hts_measurement_value_actual_length = cur_offset;

}

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
bool hts_measurement_value_indicate(uint8_t conn_id, T_SERVER_ID service_id)
{
    hts_format_measurement_value();
    return server_send_data(conn_id, service_id, HTS_TEMPERATURE_MEASUREMENT_INDEX,
                            (uint8_t *)&hts_measurement_value_for_notify_indicate, hts_measurement_value_actual_length,
                            GATT_PDU_TYPE_INDICATION);
}

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

bool hts_intermediate_temperature_value_notify(uint8_t conn_id, T_SERVER_ID service_id)
{
    hts_format_measurement_value();
    return server_send_data(conn_id, service_id, HTS_INTERMEDIATE_TEMPERATURE_INDEX,
                            (uint8_t *)&hts_measurement_value_for_notify_indicate, hts_measurement_value_actual_length,
                            GATT_PDU_TYPE_NOTIFICATION);
}

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

bool hts_measurement_interval_notify(uint8_t conn_id, T_SERVER_ID service_id, uint16_t seconds)
{
    return server_send_data(conn_id, service_id, HTS_MEASUREMENT_INTERVAL_INDEX, (uint8_t *)&seconds,
                            sizeof(seconds), GATT_PDU_TYPE_NOTIFICATION);
}

/**
 * @brief read characteristic data from service.
 *
 * @param[in] conn_id     Connection id.
 * @param[in] service_id          ServiceID of characteristic data.
 * @param[in] attrib_index          Attribute index of getting characteristic data.
 * @param[in] offset                Used for Blob Read.
 * @param[in,out] p_length            length of getting characteristic data.
 * @param[in,out] pp_value            data got from service.
 * @return Profile procedure result
*/
T_APP_RESULT hts_attr_read_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                              uint16_t offset, uint16_t *p_length, uint8_t **pp_value)
{
    T_APP_RESULT cause = APP_RESULT_SUCCESS;
    *p_length = 0;
    T_HTS_CALLBACK_DATA callback_data;
    callback_data.msg_type = SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE;

    if (HTS_MEASUREMENT_INTERVAL_INDEX == attrib_index)
    {
        callback_data.msg_data.read_value_index = HTS_READ_MEASUREMENT_INTERVAL_VALUE;
        pfn_hts_cb(service_id, (void *)&callback_data);
        *pp_value = (uint8_t *)&hts_measurement_interval;
        *p_length = sizeof(hts_measurement_interval);
    }
    else if (HTS_TEMPERATURE_TYPE_INDEX == attrib_index)
    {
        callback_data.msg_data.read_value_index = HTS_READ_TEMPERATURE_TYPE_VALUE;
        pfn_hts_cb(service_id, (void *)&callback_data);
        *pp_value = (uint8_t *)&temperature_type;
        *p_length = 1;
    }
    else if (HTS_MEASUREMENT_INTERVAL_VALID_RANGE_INDEX == attrib_index)
    {
        callback_data.msg_data.read_value_index = HTS_READ_VALID_RANGE_VALUE;
        pfn_hts_cb(service_id, (void *)&callback_data);
        *pp_value = (uint8_t *)&hts_measurement_interval_valid_range;
        *p_length = sizeof(hts_measurement_interval_valid_range);
    }

    else
    {
        cause  = APP_RESULT_ATTR_NOT_FOUND;
    }

    return cause;
}

/**
 * @brief write characteristic data from service.
 *
 * @param[in] conn_id     Connection id.
 * @param[in] service_id          ServiceID to be written.
 * @param[in] attrib_index          Attribute index of characteristic.
 * @param[in] write_type          Write type.
 * @param[in] length            length of value to be written.
 * @param[in] p_value            value to be written.
 * @param[in] p_write_ind_post_proc value to be written.
 * @return Profile procedure result
*/
T_APP_RESULT hts_attr_write_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                               T_WRITE_TYPE write_type,
                               uint16_t length, uint8_t *p_value, P_FUN_WRITE_IND_POST_PROC *p_write_ind_post_proc)
{
    T_APP_RESULT cause = APP_RESULT_SUCCESS;
    PROFILE_PRINT_ERROR2("hts_attr_write_cb  iAttribIndex = %d wLength %x", attrib_index, length);

    switch (attrib_index)
    {
    case HTS_MEASUREMENT_INTERVAL_INDEX:
        {
            T_HTS_CALLBACK_DATA callback_data;
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE;
            callback_data.msg_data.write.write_index = HTS_WRITE_MEASUREMENT_INTERVAL_VALUE;
            memcpy(&callback_data.msg_data.write.measurement_interval, p_value, 2);
            pfn_hts_cb(service_id, (void *)&callback_data);
        }
        break;

    default:
        {
            PROFILE_PRINT_ERROR1("hts_attr_write_cb  iAttribIndex = %d not found", attrib_index);
            cause = APP_RESULT_ATTR_NOT_FOUND;
        }
        break;
    }

    return cause;
}

/**
 * @brief update CCCD bits from stack.
 *
 * @param[in] conn_id       Connection id.
 * @param[in] service_id          Service ID.
 * @param[in] index          Attribute index of characteristic data.
 * @param[in] ccc_bits         CCCD bits from stack.
 * @return None
*/
void hts_cccd_update_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t index, uint16_t ccc_bits)
{
    T_HTS_CALLBACK_DATA callback_data;
    callback_data.msg_type = SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION;
    bool handle = true;

    PROFILE_PRINT_INFO2("HtsCccdUpdate  Index = %d wCCCDBits %x", index, ccc_bits);

    switch (index)
    {
    case HTS_TEMPERATURE_MEASUREMENT_CCCD_INDEX:
        {
            if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_NOTIFY_INDICATE)
            {
                // Enable Notification
                callback_data.msg_data.notification_indification_index =
                    HTS_NOTIFY_INDICATE_TEMPERATURE_MEASUREMENT_VALUE_ENABLE;

            }
            else
            {
                callback_data.msg_data.notification_indification_index =
                    HTS_NOTIFY_INDICATE_TEMPERATURE_MEASUREMENT_VALUE_DISABLE;
            }
        }
        break;
    case HTS_INTERMEDIATE_TEMPERATURE_CCCD_INDEX:
        if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_NOTIFY_INDICATE)
        {
            // Enable Notification
            callback_data.msg_data.notification_indification_index =
                HTS_NOTIFY_INDICATE_INTERMEDIATE_TEMPERATURE_VALUE_ENABLE;

        }
        else
        {
            callback_data.msg_data.notification_indification_index =
                HTS_NOTIFY_INDICATE_INTERMEDIATE_TEMPERATURE_VALUE_DISABLE;
        }
        break;
    case HTS_MEASUREMENT_INTERVAL_CCCD_INDEX:
        if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_NOTIFY_INDICATE)
        {
            // Enable Notification
            callback_data.msg_data.notification_indification_index =
                HTS_NOTIFY_INDICATE_MEASUREMENT_INTERVAL_VALUE_ENABLE;

        }
        else
        {
            callback_data.msg_data.notification_indification_index =
                HTS_NOTIFY_INDICATE_MEASUREMENT_INTERVAL_VALUE_DISABLE;
        }
        break;

    default:
        handle = false;
        break;

    }
    /* Notify Application. */
    if (pfn_hts_cb && (handle == true))
    {
        pfn_hts_cb(service_id, (void *)&callback_data);
    }

    return;
}

/**
 * @brief HTS Service Callbacks.
*/
const T_FUN_GATT_SERVICE_CBS hts_cbs =
{
    hts_attr_read_cb,  // Read callback function pointer
    hts_attr_write_cb, // Write callback function pointer
    hts_cccd_update_cb  // CCCD update callback function pointer
};

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

T_SERVER_ID hts_add_service(void *p_func)
{
    T_SERVER_ID service_id;
    if (false == server_add_service(&service_id,
                                    (uint8_t *)hts_attr_tbl,
                                    hts_attr_tbl_size,
                                    hts_cbs))
    {
        PROFILE_PRINT_ERROR1("hts_add_service: service_id %d", service_id);
        service_id = 0xff;
    }
    pfn_hts_cb = (P_FUN_SERVER_GENERAL_CB)p_func;
    return service_id;

}


