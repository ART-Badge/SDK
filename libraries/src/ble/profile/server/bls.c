/****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rigbls reserved.
*****************************************************************************************

  * @file     bls.c
  * @brief    blood pressure service source file.
  * @details  Interface to access the blood pressure service.
  * @author
  * @date
  * @version  v1.0
  * *************************************************************************************
  */

#include "trace.h"
#include <string.h>
#include "profile_server.h"
#include "bls.h"

/********************************************************************************************************
* local static variables defined here, only used in this source file.
********************************************************************************************************/

#define BLS_BLOOD_PRESSURE_MEASUREMENT_INDEX   2

#if BLS_INTERMEDIATE_CUFF_PRESSURE_SUPPORT
#define BLS_INTERMEDIATE_CUFF_PRESSURE_INDEX   5
#define BLS_BLOOD_PRESSURE_FEATURE_INDEX       8
#else
#define BLS_BLOOD_PRESSURE_FEATURE_INDEX       5
#endif

#define BLS_BLOOD_PRESSURE_MEASUREMENT_CCCD_INDEX             3

#if BLS_INTERMEDIATE_CUFF_PRESSURE_SUPPORT
#define BLS_INTERMEDIATE_CUFF_PRESSURE_CCCD_INDEX             6
#endif

static uint16_t bls_blood_pressure_feature = 0;

static uint8_t bls_measurement_value_for_notify_indicate[BLS_MEASUREMENT_VALUE_MAX_LEN];
static uint8_t bls_measurement_value_actual_length = 0;

static P_FUN_SERVER_GENERAL_CB pfn_bls_cb = NULL;

/** @brief  profile/service definition.  */
const T_ATTRIB_APPL bls_attr_tbl[] =
{
    /* <<Primary Service>>, .. 0*/
    {
        (ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_LE),  /* wFlags     */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_PRIMARY_SERVICE),
            HI_WORD(GATT_UUID_PRIMARY_SERVICE),
            LO_WORD(GATT_UUID_BLOOD_PRESSURE),      /* service UUID */
            HI_WORD(GATT_UUID_BLOOD_PRESSURE)
        },
        UUID_16BIT_SIZE,                            /* bValueLen     */
        NULL,                                       /* pValueContext */
        GATT_PERM_READ                              /* wPermissions  */
    },
    /* <<Characteristic>>, .. 1*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_INDICATE  /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*--- Blood Pressure Measurement characteristic value ---2*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_BLP_MEASUREMENT),
            HI_WORD(GATT_UUID_CHAR_BLP_MEASUREMENT)
        },
        0,                                          /* bValueLen */
        NULL,
        GATT_PERM_NONE                              /* wPermissions */
    },
    /* client characteristic configuration 3*/
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

#if BLS_INTERMEDIATE_CUFF_PRESSURE_SUPPORT
    ,
    /* <<Characteristic>>, .. 4*/
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
    /*--- Intermediate Cuff Pressure characteristic value ---5*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_INTERMEDIATE_CUFF_PRESSURE),
            HI_WORD(GATT_UUID_CHAR_INTERMEDIATE_CUFF_PRESSURE)
        },
        0,                                          /* bValueLen */
        NULL,
        GATT_PERM_NONE                              /* wPermissions */
    },
    /* client characteristic configuration 6*/
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
#endif  /**< end of INTERMEDIATE_CUFF_PRESSURE_SUPPORT */

    ,
    /* <<Characteristic>>, .. 7*/
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
    /*---Blood Pressure Feature characteristic value ---8*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_BLP_FEATURE),
            HI_WORD(GATT_UUID_CHAR_BLP_FEATURE),
        },
        0,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    }
};

/**< @brief  blood pressure service size definition.  */
const static uint16_t bls_attr_tbl_size = sizeof(bls_attr_tbl);

bool bls_set_parameter(T_BLS_PARAM_TYPE param_type, uint8_t length, void *p_value)
{
    bool ret = true;

    switch (param_type)
    {
    default:
        {
            ret = false;
            PROFILE_PRINT_ERROR1("bls_set_parameter: unknown param_type 0x%02x", param_type);
        }
        break;


    case BLS_PARAM_BLOOD_PRESUREE_FEATURE:
        {
            if (length != sizeof(uint16_t))
            {
                ret = false;
            }
            else
            {
                memcpy(&bls_blood_pressure_feature, p_value, 2);
            }
        }
        break;
    }

    return ret;
}

void bls_format_measurement_value(T_BLOOD_PRESSURE_MEASURMENT *p_data)
{
    uint8_t cur_offset = 0;

    memcpy(&bls_measurement_value_for_notify_indicate[cur_offset], &p_data->bp_meas_flag, 1);
    cur_offset += 1;

    memcpy(&bls_measurement_value_for_notify_indicate[cur_offset],
           &p_data->bp_meas_compound_value, 6);
    cur_offset += 6;

    if (p_data->bp_meas_flag & BLS_FLAG_MEASUREMENT_TIMESTAMP_PRESENT_BIT)
    {
        memcpy(&bls_measurement_value_for_notify_indicate[cur_offset], &p_data->time_stamp, 7);
        cur_offset += 7;
    }

    if (p_data->bp_meas_flag & BLS_FLAG_MEASUREMENT_PULSE_RATE_BIT)
    {
        memcpy(&bls_measurement_value_for_notify_indicate[cur_offset], &p_data->bp_meas_pulse_rate,
               2);
        cur_offset += 2;
    }

    if (p_data->bp_meas_flag & BLS_FLAG_MEASUREMENT_USER_ID_BIT)
    {
        memcpy(&bls_measurement_value_for_notify_indicate[cur_offset], &p_data->bp_meas_user_id,
               1);
        cur_offset += 1;
    }

    if (p_data->bp_meas_flag & BLS_FLAG_MEASUREMENT_STATUS_BIT)
    {
        memcpy(&bls_measurement_value_for_notify_indicate[cur_offset], &p_data->bp_meas_status,
               2);
        cur_offset += 2;
    }

    bls_measurement_value_actual_length = cur_offset;

}

bool bls_blood_pressure_measurement_value_indicate(uint8_t conn_id, T_SERVER_ID service_id,
                                                   T_BLOOD_PRESSURE_MEASURMENT *p_data)
{
    APP_PRINT_INFO0("bls_blood_pressure_measurement_value_indicate");
    bls_format_measurement_value(p_data);

    return server_send_data(conn_id, service_id, BLS_BLOOD_PRESSURE_MEASUREMENT_INDEX,
                            (uint8_t *)&bls_measurement_value_for_notify_indicate,
                            bls_measurement_value_actual_length, GATT_PDU_TYPE_INDICATION);
}

#if BLS_INTERMEDIATE_CUFF_PRESSURE_SUPPORT
bool bls_intermediate_cuff_pressure_value_notify(uint8_t conn_id, T_SERVER_ID service_id,
                                                 T_BLOOD_PRESSURE_MEASURMENT *p_data)
{
    APP_PRINT_INFO0("bls_intermediate_cuff_pressure_value_notify");
    bls_format_measurement_value(p_data);

    return server_send_data(conn_id, service_id, BLS_INTERMEDIATE_CUFF_PRESSURE_INDEX,
                            (uint8_t *)&bls_measurement_value_for_notify_indicate, bls_measurement_value_actual_length,
                            GATT_PDU_TYPE_NOTIFICATION);
}
#endif

T_APP_RESULT bls_attr_read_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                              uint16_t offset, uint16_t *p_length, uint8_t **pp_value)
{
    T_APP_RESULT cause = APP_RESULT_SUCCESS;
    *p_length = 0;
    T_BLS_CALLBACK_DATA callback_data;
    callback_data.msg_type = SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE;

    PROFILE_PRINT_INFO2("bls_attr_read_cb: attrib_index %d offset %d", attrib_index, offset);

    switch (attrib_index)
    {
    default:
        {
            PROFILE_PRINT_ERROR0("bls_attr_read_cb: unknown attrib_index");
            cause  = APP_RESULT_ATTR_NOT_FOUND;
        }
        break;

    case BLS_BLOOD_PRESSURE_FEATURE_INDEX:
        {
            callback_data.conn_id = conn_id;
            cause = pfn_bls_cb(service_id, (void *)&callback_data);

            *pp_value = (uint8_t *)&bls_blood_pressure_feature;
            *p_length = sizeof(bls_blood_pressure_feature);
        }
        break;
    }
    return (cause);
}

void bls_cccd_update_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t index, uint16_t ccc_bits)
{
    T_BLS_CALLBACK_DATA callback_data;
    callback_data.msg_type = SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION;
    callback_data.conn_id = conn_id;
    bool handle = true;
    PROFILE_PRINT_INFO2("bls_cccd_update_cb: index %d ccc_bits 0x%04x", index, ccc_bits);

    switch (index)
    {
    case BLS_BLOOD_PRESSURE_MEASUREMENT_CCCD_INDEX:
        {
            if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_INDICATE)
            {
                callback_data.msg_data.notification_indification_index =
                    BLS_INDICATE_BLOOD_PRESSURE_MEASUREMENT_ENABLE;
            }
            else
            {
                callback_data.msg_data.notification_indification_index =
                    BLS_INDICATE_BLOOD_PRESSURE_MEASUREMENT_DISABLE;
            }
            break;
        }
#if BLS_INTERMEDIATE_CUFF_PRESSURE_SUPPORT
    case BLS_INTERMEDIATE_CUFF_PRESSURE_CCCD_INDEX:
        {
            if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_NOTIFY)
            {
                callback_data.msg_data.notification_indification_index =
                    BLS_NOTIFY_INTERMEDIATE_CUFF_PRESSURE_ENABLE;
            }
            else
            {
                callback_data.msg_data.notification_indification_index =
                    BLS_NOTIFY_INTERMEDIATE_CUFF_PRESSURE_ENABLE;
            }
            break;
        }
#endif
    default:
        {
            handle = false;
            break;
        }

    }

    if (pfn_bls_cb && (handle == true))
    {
        pfn_bls_cb(service_id, (void *)&callback_data);
    }

    return;
}

/**
 * @brief BLS Service Callbacks.
*/
const T_FUN_GATT_SERVICE_CBS bls_cbs =
{
    bls_attr_read_cb,   // Read callback function pointer
    NULL,               // Write callback function pointer
    bls_cccd_update_cb  // CCCD update callback function pointer
};

T_SERVER_ID bls_add_service(void *p_func)
{
    T_SERVER_ID service_id;
    if (false == server_add_service(&service_id,
                                    (uint8_t *)bls_attr_tbl,
                                    bls_attr_tbl_size,
                                    bls_cbs))
    {
        PROFILE_PRINT_ERROR1("bls_add_service: service_id %d", service_id);
        service_id = 0xff;
    }
    pfn_bls_cb = (P_FUN_SERVER_GENERAL_CB)p_func;
    return service_id;
}
