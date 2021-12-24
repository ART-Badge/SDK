/**
***************************************************************************************
*     Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
***************************************************************************************
* @file     plxs.c
* @brief    Pulse Oximeter Service source file.
* @details  Interface to access the Pulse Oximeter Service.
* @author   danni
* @date     2018-12-27
* @version  v1.0
* *************************************************************************************
*/
#include "trace.h"
#include <string.h>
#include <gatt.h>
#include "plxs.h"

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup PLXS_SERVICE_UUID service related UUIDs.
 * @{
 */
#define GATT_UUID_PULSE_OXIMETER                                                  0x1822
#define GATT_UUID_CHAR_PLX_SPOT_CHECK_MEASUREMENT                                 0x2A5E
#define GATT_UUID_CHAR_PLX_CONTINUOUS_MEASUREMENT                                 0x2A5F
#define GATT_UUID_CHAR_PLX_FEATURE                                                0x2A60
#define GATT_UUID_CHAR_RECORD_ACCESS_CONTROL_POINT                                0x2A52
/** @} End of PLXS_SERVICE_UUID */

/** @brief  Check PLXS Record Access Control Point operation is available or not. */
#define PLXS_RACP_OPERATION_ACTIVE(x)                                             (((x) >= PLXS_RACP_OPCODE_REPORT_RECS) && ((x) < PLXS_RACP_OPCODE_NBR_OF_RECS_RESP))

/** @defgroup PLXS_APP_ERROR_CODE profile specific attribute protocol application error codes
  * @{
  */
#define PLXS_ERR_PROC_ALREADY_IN_PROGRESS                                         0xFE
#define PLXS_ERR_CCCD_IMPROPERLY_CONFIGURED                                       0xFD
/** @} End of PLXS_APP_ERROR_CODE */

/** @defgroup PLXS_SPOT_CHECK_FLAGS PLX Spot-check Measurement Characteristic Flags,The Flags field is an 8-bit bit field which indicates what fields are present in the PLX Spot-check
  *           Measurement Characteristic value
  * @{
  */
#define PLXS_SPOT_CHECK_MEASUREMENT_FLAG_TIMESTAMP                                0x01
#define PLXS_SPOT_CHECK_MEASUREMENT_FLAG_MEASUREMENT_STATUS                       0x02
#define PLXS_SPOT_CHECK_MEASUREMENT_FLAG_DEVICE_SENSOR_STATUS                     0x04
#define PLXS_SPOT_CHECK_MEASUREMENT_FLAG_PULSE_AMPLITUDE_INDEX                    0x08
#define PLXS_SPOT_CHECK_MEASUREMENT_FLAG_DEVICE_CLOCK_NOT_SET                     0x10
/** @} End of PLXS_SPOT_CHECK_FLAGS */

/** @defgroup PLXS_CONTINUOUS_MEAS_FLAGS PLX Continuous Measurement Characteristic Flags,The Flags field is an 8-bit bit field which indicates what fields are present in the PLX Continuous
  *           Measurement Characteristic value.
  * @{
  */
#define PLXS_CONTINUOUS_MEASUREMENT_FLAG_SPO2PR_FAST                              0x01
#define PLXS_CONTINUOUS_MEASUREMENT_FLAG_SPO2PR_SLOW                              0x02
#define PLXS_CONTINUOUS_MEASUREMENT_FLAG_MEASUREMENT_STATUS                       0x04
#define PLXS_CONTINUOUS_MEASUREMENT_FLAG_DEVICE_SENSOR_STATUS                     0x08
#define PLXS_CONTINUOUS_MEASUREMENT_FLAG_PULSE_AMPLITUDE_INDEX                    0x10
/** @} End of PLXS_CONTINUOUS_MEAS_FLAGS */

/** @defgroup PLXS_FEASURES_FLAGS PLX Supported Features field. The Supported Features field is a 16-bit bit field which indicates feature support as well as what
  *           fields are present in the PLX Features characteristic.
  * @{
  */
#define PLXS_FEATURES_MEASUREMENT_STATUS                                          0x0001
#define PLXS_FEATURES_DEVICE_SENSOR_STATUS                                        0x0002
#define PLXS_FEATURES_MEASUREMENT_STORAGE                                         0x0004
#define PLXS_FEATURES_TIMESTAMP                                                   0x0008
#define PLXS_FEATURES_SPO2PR_FAST_METRIC                                          0x0010
#define PLXS_FEATURES_SPO2PR_SLOW_METRIC                                          0x0020
#define PLXS_FEATURES_PULSE_AMPLITUDE_INDEX                                       0x0040
#define PLXS_FEATURES_MULTIPLE_BONDS                                              0x0080
/** @} End of PLXS_FEASURES_FLAGS */

/** @brief  The Server should be able to store 30 or more measurements*/
#define PLXS_RACP_DATABASE_SIZE                                                   (PLXS_RACP_MAX_NBR_OF_STORED_RECS + 1)
/*============================================================================*
 *                         Types
 *============================================================================*/
#if PLXS_STORE_SPOT_CHECK_MEASUREMENT_SUPPORT
typedef struct
{
    T_PLXS_SPOT_CHECK_MEASUREMENT_VALUE plxs_spot_check_measurement_value;

} T_PLXS_PATIENT_RECORD;

/** @brief data base for store spot check measurement value*/
typedef struct
{
    T_PLXS_PATIENT_RECORD records[PLXS_RACP_DATABASE_SIZE];
    uint8_t record_num;
    uint8_t head;
    uint8_t tail;
} T_PLXS_RECORD_DATA_BASE;

/** @brief  PLXS Record Access Control Point OpCodes. */
typedef enum
{
    PLXS_RACP_OPCODE_RESERVED = 0x00,/**< reserved for future use operator:N/A */
    PLXS_RACP_OPCODE_REPORT_RECS = 0x01,/**< report stored records(operator:All records)*/
    PLXS_RACP_OPCODE_DELETE_RECS = 0x02,/**< delete stored  records(operator:All records)*/
    PLXS_RACP_OPCODE_ABORT_OPERATION  = 0x03,/**< Abort operation(operator:Null 0x00)*/
    PLXS_RACP_OPCODE_REPORT_NBR_OF_RECS = 0x04,/**< report number of stored records(operator:All records)*/
    PLXS_RACP_OPCODE_NBR_OF_RECS_RESP = 0x05,/**< number of stored records response(operator:Null 0x00)*/
    PLXS_RACP_OPCODE_RESP_CODE  = 0x06/**< response code (operator:Null 0x00)*/
} T_PLXS_CTRL_POINT_OPCODE;

/**@brief  PLXS Record Access Control Point Operator. */
typedef enum
{
    PLXS_RACP_OPERATOR_NULL = 0x00,
    PLXS_RACP_OPERATOR_ALL_RECS = 0x01,/**< All records*/
    PLXS_RACP_OPERATOR_LT_EQ = 0x02,/**< less than or equal to*/
    PLXS_RACP_OPERATOR_GT_EQ = 0x03,/**< greater than or equal to*/
    PLXS_RACP_OPERATOR_RANGE = 0x04,/**< within range of */
    PLXS_RACP_OPERATOR_FIRST = 0x05,/**< first records*/
    PLXS_RACP_OPERATOR_LAST = 0x06/**< last records*/
} T_PLXS_CTRL_POINT_OPERATOR;

/** @brief  Glucose Record Access Control Point Response Codes. */
typedef enum
{
    PLXS_RACP_RESP_RESERVED = 0x00,
    PLXS_RACP_RESP_SUCCESS = 0x01,
    PLXS_RACP_RESP_OPCODE_NOT_SUPPORTED = 0x02,
    PLXS_RACP_RESP_INVALID_OPERATOR = 0x03,
    PLXS_RACP_RESP_OPERATOR_NOT_SUPPORTED = 0x04,
    PLXS_RACP_RESP_INVALID_OPERAND = 0x05,
    PLXS_RACP_RESP_NO_RECS_FOUND = 0x06,
    PLXS_RACP_RESP_ABORT_UNSUCCESSFUL = 0x07,
    PLXS_RACP_RESP_PROC_NOT_COMPLETED  = 0x08,
    PLXS_RACP_RESP_OPERAND_NOT_SUPPORTED = 0x09
} T_PLXS_CTRL_POINT_RESP_CODES;

typedef struct
{
    T_PLXS_CTRL_POINT_OPCODE  op_code;
    T_PLXS_CTRL_POINT_OPERATOR op;/**< operator*/
    uint8_t operand[18];
} T_PLXS_CONTROL_POINT;

typedef struct
{
    T_PLXS_CONTROL_POINT ctrl_point;
    uint8_t cp_length;/**< length of current operand of control point*/
    T_PLXS_RECORD_DATA_BASE record_db;
} T_PLXS_RACP;
/** @brief plxs report records procedure flags*/
typedef struct
{
    uint16_t plxs_num_records_to_report;
    uint16_t plxs_current_record_to_report;
    uint16_t plxs_record_to_report_start_index;
    uint16_t plxs_records_to_report_offset;
    bool     plxs_send_data_end_flag;
} T_PLXS_REPORT_RECORDS_FLAGS;
#endif
/** @brief notification or indiaction enabled or not.1:enabled,0: disabled*/
typedef struct
{
    uint8_t  spot_check_measurement_indication_enable: 1;
    uint8_t  continuous_measurement_notify_enable: 1;
    uint8_t  racp_indication_enable: 1;
    uint8_t  rfu: 5;
} T_PLXS_NOTIFY_INDICATE_FLAG;
/** @brief add flow control for notify and indicate*/
typedef struct
{
    uint8_t spot_check_indication_status_flag;
    uint8_t continuous_measurement_notify_status_flag;
    uint8_t report_records_indication_status_flag;
} T_PLXS_DATA_SEND_STATUS_FLAGS;

/********************************************************************************************************
* local static and global variables defined here, only used in this source file.
********************************************************************************************************/
#if PLXS_STORE_SPOT_CHECK_MEASUREMENT_SUPPORT
/**< @brief plxs record access control point struct*/
static T_PLXS_RACP                     plxs_racp;
/**< @brief report stored repords procedure needs flags*/
static T_PLXS_REPORT_RECORDS_FLAGS     plxs_report_records_flags = {0, 0, 0, 0, 1};
/**< @brief plxs abort procedure flags*/
static bool                            plxs_abort_flag = false;
static bool                            plxs_abort_by_app_flag = false;
#endif
/**< @brief plxs feature characteristic value struct*/
static T_PLXS_FEATURES_VALUE           plxs_features;
#if PLXS_SPOT_CHECK_MEASUREMENT_SUPPORT
/**< @brief plxs spot check measurement flags*/
static uint8_t                         plxs_spot_check_measurement_flags;
#endif
#if PLXS_CONTINUOUS_MEASUREMENT_SUPPORT
/**< @brief plxs continuous measurement flags*/
static uint8_t                         plxs_continuous_measurement_flags;
#endif
/**< @brief flags of notification and indication for characteristic */
static T_PLXS_NOTIFY_INDICATE_FLAG     plxs_notify_indicate_flag = {0};

static T_PLXS_DATA_SEND_STATUS_FLAGS   plxs_data_send_status_flags = {PLXS_STATUS_SPOT_CHECK_MEASUREMENT_INDICATION_IDLE,
                                                                      PLXS_STATUS_CONTINUOUS_MEASUREMENT_NOTIFY_IDLE,
                                                                      PLXS_STATUS_REPORT_RECORDS_INDICATION_IDLE
                                                                     };
/**< @brief Function pointer used to send event to application from plxs. Initiated in plxs_add_service. */
static P_FUN_SERVER_GENERAL_CB         pfn_plxs_cb = NULL;

/** @brief  profile/service definition.  */
const T_ATTRIB_APPL  plxs_att_tbl[] =
{
    /* <<Primary Service>>, .. */
    {
        (ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_LE), /*wFlags*/
        {                                          /*bTypeValue*/
            LO_WORD(GATT_UUID_PRIMARY_SERVICE),
            HI_WORD(GATT_UUID_PRIMARY_SERVICE),
            LO_WORD(GATT_UUID_PULSE_OXIMETER),     /*service uuid*/
            HI_WORD(GATT_UUID_PULSE_OXIMETER)
        },
        UUID_16BIT_SIZE,                           /*bValueLen*/
        NULL,                                      /*pValueContext*/
        GATT_PERM_READ                             /*wPermissions*/

    },
#if PLXS_SPOT_CHECK_MEASUREMENT_SUPPORT
    /* <<Characteristic>>, .. */
    {
        ATTRIB_FLAG_VALUE_INCL,                    /*wFlags*/
        {                                          /*bTypeValue*/
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_INDICATE                /*characteristic properties*/
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                         /*bValueLen*/
        NULL,                                      /*pValueContext*/
        GATT_PERM_READ                             /*wPermissions*/

    },
    /*--- PLX Spot-check Measurement characteristic value ---*/
    {
        ATTRIB_FLAG_VALUE_APPL,                    /*wFlags*/
        {
            LO_WORD(GATT_UUID_CHAR_PLX_SPOT_CHECK_MEASUREMENT),
            HI_WORD(GATT_UUID_CHAR_PLX_SPOT_CHECK_MEASUREMENT)
        },
        0,                                         /*bValueLen*/
        NULL,                                      /*pValueContext*/
        GATT_PERM_NONE                             /*wPermissions*/

    },
    /*client characteristic configuration */
    {
        (ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_CCCD_APPL),/*wFlags*/
        {
            LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            /* NOTE: this value has an instantiation for each client, a write to */
            /* this attribute does not modify this default value:*/
            LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT),
            HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)
        },
        2,                                           /*bValueLen*/
        NULL,
        (GATT_PERM_READ_AUTHEN_REQ | GATT_PERM_WRITE_AUTHEN_REQ)           /*wPermissions*/
    },
#endif


#if PLXS_CONTINUOUS_MEASUREMENT_SUPPORT
    /* <<Characteristic>>, .. */
    {
        ATTRIB_FLAG_VALUE_INCL,                         /* wFlags */
        {                                               /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_NOTIFY                      /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                              /* bValueLen */
        NULL,                                           /*pValueContext*/
        GATT_PERM_READ                                  /* wPermissions */
    },
    /*--- PLX Continuous Measurement characteristic value ---*/
    {
        ATTRIB_FLAG_VALUE_APPL,                         /* wFlags */
        {                                               /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_PLX_CONTINUOUS_MEASUREMENT),
            HI_WORD(GATT_UUID_CHAR_PLX_CONTINUOUS_MEASUREMENT)
        },
        0,                                              /* bValueLen */
        NULL,                                           /*pValueContext*/
        GATT_PERM_NONE                                  /* wPermissions */
    },
    /* client characteristic configuration */
    {
        (ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_CCCD_APPL),/* wFlags */
        {                                                 /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            /* NOTE: this value has an instantiation for each client, a write to */
            /* this attribute does not modify this default value:*/
            LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT),
            HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)
        },
        2,                                                /* bValueLen */
        NULL,
        (GATT_PERM_READ_AUTHEN_REQ | GATT_PERM_WRITE_AUTHEN_REQ)                /* wPermissions */

    },

#endif
    /* <<Characteristic>>, .. */
    {
        ATTRIB_FLAG_VALUE_INCL,                           /* wFlags */
        {                                                 /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ                           /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                                /* bValueLen */
        NULL,                                             /*pValueContext*/
        GATT_PERM_READ                                    /* wPermissions */

    },
    /*--- PLXS features characteristic value read ---*/
    {
        ATTRIB_FLAG_VALUE_APPL,
        {
            LO_WORD(GATT_UUID_CHAR_PLX_FEATURE),
            HI_WORD(GATT_UUID_CHAR_PLX_FEATURE)

        },
        0,
        NULL,
        GATT_PERM_READ_AUTHEN_REQ
    },
#if PLXS_STORE_SPOT_CHECK_MEASUREMENT_SUPPORT
    /* <<Characteristic>>, .. */
    {
        ATTRIB_FLAG_VALUE_INCL,
        {
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            (GATT_CHAR_PROP_WRITE | GATT_CHAR_PROP_INDICATE)
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,
        NULL,
        GATT_PERM_READ

    },
    /*--- PLXS Record Access Control Point value ---*/
    {
        ATTRIB_FLAG_VALUE_APPL,
        {
            LO_WORD(GATT_UUID_CHAR_RECORD_ACCESS_CONTROL_POINT),
            HI_WORD(GATT_UUID_CHAR_RECORD_ACCESS_CONTROL_POINT)
        },
        0,
        NULL,
        GATT_PERM_WRITE_AUTHEN_REQ
    },
    /* client characteristic configuration */
    {
        (ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_CCCD_APPL),/* wFlags */
        {                                                /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            /* NOTE: this value has an instantiation for each client, a write to */
            /* this attribute does not modify this default value:*/
            LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT),    /* client char. config. bit field */
            HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)
        },
        2,                                          /* bValueLen */
        NULL,                                       /*pValueContext*/
        (GATT_PERM_READ_AUTHEN_REQ | GATT_PERM_WRITE_AUTHEN_REQ)          /* wPermissions */
    }
#endif
};
/********************************************************************************************************
*                                 functions
********************************************************************************************************/
/**
  * @brief  init spot-check measurement characteristic flags, continuous measurement
  *         characteristic flags and feature characteristic flags according Macros defined in plxs_config.h
  *
  * @param[in] None
  * @return  None
  * <b>Example usage</b>
  * \code{.c}
     T_SERVER_ID plxs_add_service(void * p_func)
    {
        plxs_flags_init();
        ......
    }
  * \endcode
  */
void plxs_flags_init()
{
#if PLXS_SPOT_CHECK_MEASUREMENT_SUPPORT //spot check start
#if PLXS_SPOT_CHECK_MEASUREMENT_TIMESTAMP_SUPPORT //TIMESTAMP_SUPPORT start
    plxs_spot_check_measurement_flags |= PLXS_SPOT_CHECK_MEASUREMENT_FLAG_TIMESTAMP;
#endif //TIMESTAMP_SUPPORT end

#if PLXS_MEASUREMENT_STATE_SUPPORT //MEASUREMENT_STATE_SUPPORT start
    plxs_spot_check_measurement_flags |= PLXS_SPOT_CHECK_MEASUREMENT_FLAG_MEASUREMENT_STATUS;
#endif //MEASUREMENT_STATE_SUPPORT end

#if PLXS_DEVICE_AND_SENSOR_STATUS_SUPPORT //DEVICE_AND_SENSOR_STATUS_SUPPORT start
    plxs_spot_check_measurement_flags |= PLXS_SPOT_CHECK_MEASUREMENT_FLAG_DEVICE_SENSOR_STATUS;
#endif //DEVICE_AND_SENSOR_STATUS_SUPPORT end

#if PLXS_PULSE_AMPLITUDE_INDEX_SUPPORT
    plxs_spot_check_measurement_flags |= PLXS_SPOT_CHECK_MEASUREMENT_FLAG_PULSE_AMPLITUDE_INDEX;
#endif

#if PLXS_DEVICE_CLOCK_NOT_SET
    plxs_spot_check_measurement_flags |= PLXS_SPOT_CHECK_MEASUREMENT_FLAG_DEVICE_CLOCK_NOT_SET;
#endif
#endif //spot check end

#if PLXS_CONTINUOUS_MEASUREMENT_SUPPORT // continueous measurement start
#if PLXS_CONTINUOUS_MEASUREMENT_SPO2PR_FAST_SUPPORT
    plxs_continuous_measurement_flags |= PLXS_CONTINUOUS_MEASUREMENT_FLAG_SPO2PR_FAST;
#endif

#if PLXS_CONTINUOUS_MEASUREMENT_SPO2PR_SLOW_SUPPORT
    plxs_continuous_measurement_flags |= PLXS_CONTINUOUS_MEASUREMENT_FLAG_SPO2PR_SLOW;
#endif

#if PLXS_MEASUREMENT_STATE_SUPPORT
    plxs_continuous_measurement_flags |= PLXS_CONTINUOUS_MEASUREMENT_FLAG_MEASUREMENT_STATUS;
#endif

#if PLXS_DEVICE_AND_SENSOR_STATUS_SUPPORT
    plxs_continuous_measurement_flags |= PLXS_CONTINUOUS_MEASUREMENT_FLAG_DEVICE_SENSOR_STATUS;
#endif

#if PLXS_PULSE_AMPLITUDE_INDEX_SUPPORT
    plxs_continuous_measurement_flags |= PLXS_CONTINUOUS_MEASUREMENT_FLAG_PULSE_AMPLITUDE_INDEX;
#endif
#endif //continuous measurement end

//plxs feature support bit init
#if PLXS_MEASUREMENT_STATE_SUPPORT
    plxs_features.supported_features  |= PLXS_FEATURES_MEASUREMENT_STATUS;
    plxs_features.measurement_status_support_bits = PLXS_FEATURES_MEASUREMENT_STATUS_SUPPORTS;
#endif

#if PLXS_DEVICE_AND_SENSOR_STATUS_SUPPORT
    plxs_features.supported_features  |= PLXS_FEATURES_DEVICE_SENSOR_STATUS;
    plxs_features.device_and_sensor_status_support_bits = PLXS_FEATURES_DEVICE_SENSOR_STATUS_SUPPORTS;
#endif

#if PLXS_STORE_SPOT_CHECK_MEASUREMENT_SUPPORT
    plxs_features.supported_features  |= PLXS_FEATURES_MEASUREMENT_STORAGE;
#endif

#if PLXS_SPOT_CHECK_MEASUREMENT_TIMESTAMP_SUPPORT
    plxs_features.supported_features  |= PLXS_FEATURES_TIMESTAMP;
#endif

#if PLXS_CONTINUOUS_MEASUREMENT_SPO2PR_FAST_SUPPORT
    plxs_features.supported_features  |= PLXS_FEATURES_SPO2PR_FAST_METRIC;
#endif

#if PLXS_CONTINUOUS_MEASUREMENT_SPO2PR_SLOW_SUPPORT
    plxs_features.supported_features  |= PLXS_FEATURES_SPO2PR_SLOW_METRIC;
#endif

#if PLXS_PULSE_AMPLITUDE_INDEX_SUPPORT
    plxs_features.supported_features  |= PLXS_FEATURES_PULSE_AMPLITUDE_INDEX;
#endif

#if PLXS_MULTIPLE_BONDS_SUPPORT
    plxs_features.supported_features  |= PLXS_FEATURES_MULTIPLE_BONDS;
#endif
}
/**
  * @brief  Set PLX service parameter.
  *         set spot_check characteristic vaule ,continuous measurement characteristic value,
  *         feature characteristic value and racp characteristic value used parameters
  * @param[in] param_type  parameter type set by T_PLXS_PARAM_TYPE @refT_PLXS_PARAM_TYPE.
  * @param[in] len  type value length to be set
  * @param[in] p_value  type value to be set
  * @return parameter set result.
  * @retval PLXS_APP_RESULT_SUCCESS  Operation success.
  * @retval others Operation failure.
  *
  * <b>Example usage</b>
  * \code{.c}
    static T_USER_CMD_PARSE_RESULT cmd_plxs_feature_flags_set(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        T_PLXS_FEATURES_VALUE plxs_features;
               ......
        plxs_set_parameter(PLXS_PARAM_FEATURE_FLAGS, 2, &plxs_features);
        return RESULT_SUCESS;
    }
  * \endcode
  */
T_PLXS_APP_RESULT plxs_set_parameter(T_PLXS_PARAM_TYPE param_type, uint8_t len,
                                     void *p_value)////todo: just for bqb,maybe not used by user?????
{
    T_PLXS_APP_RESULT ret = PLXS_APP_RESULT_SUCCESS;
    if (p_value == NULL)
    {
        ret = PLXS_APP_RESULT_POINTER_NULL;
        PROFILE_PRINT_ERROR0("plxs_set_parameter:PLXS_APP_RESULT_POINTER_NULL");
        return ret;
    }
    switch (param_type)
    {
    // set plxs feature value according to PLXS_PARAM_FEATURE_FLAGS
    case PLXS_PARAM_FEATURE_FLAGS:
        {
            plxs_features = *((T_PLXS_FEATURES_VALUE *)p_value);
        }
        break;
#if PLXS_SPOT_CHECK_MEASUREMENT_SUPPORT //spot-check start
    case PLXS_PARAM_SPOT_CHECK_MEASUREMENT_FLAGS:
        {
            //set plxs spot check measurement flags
            if (len == 1)
            {
                plxs_spot_check_measurement_flags = *((uint8_t *)p_value);
            }
            else
            {
                ret = PLXS_APP_RESULT_INVALID_VALUE_SIZE;
            }
        }
        break;
#endif //spot-check end

# if PLXS_CONTINUOUS_MEASUREMENT_SUPPORT//continuous-measurement start
    case PLXS_PARAM_CONTINUOUS_MEASUREMENT_FLAGS:
        {
            //set continuous measurement flags
            if (len == 1)
            {
                plxs_continuous_measurement_flags = *((uint8_t *)p_value);
            }
            else
            {
                ret = PLXS_APP_RESULT_INVALID_VALUE_SIZE;
            }
        }
        break;
#endif //continuous-measurement end
    default:
        {
            ret = PLXS_APP_RESULT_INVALID_TYPE;
        }
        break;
    }
    if (ret == PLXS_APP_RESULT_SUCCESS)
    {
        PROFILE_PRINT_INFO2("plxs_set_parameter success: param_type = %d,value = %x\n", param_type,
                            *(uint8_t *)p_value);
    }
    else
    {
        PROFILE_PRINT_ERROR2("plxs_set_parameter failed: param_type = %d,value = %x\n", param_type,
                             *(uint8_t *)p_value);
    }
    return ret;
}

/**
  * @brief  get plxs parameters such as spot check measurement flags,continuous measurement flags or features supports @ref T_PLXS_PARAM_TYPE
  *
  * @param[in] param_type  @ref T_PLXS_PARAM_TYPE
  * @param[in,out] p_value  get spot check measurement flags,continuous measurement flags or features supports
  * @return plxs_get_parameter result
  * @retval PLXS_APP_RESULT_SUCCESS Operation success.
  * @retval others Operation fail.
  *
  * <b>Example usage</b>
  * \code{.c}
     static T_USER_CMD_PARSE_RESULT cmd_plxs_feature_flags_set(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        T_PLXS_FEATURES_VALUE plxs_features;
        plxs_get_parameter(PLXS_PARAM_FEATURE_FLAGS, &plxs_features);
        if (p_parse_value->dw_param[0] == 1)
        {
            //both measurement status and device sensor status not support
            plxs_features.supported_features = plxs_features.supported_features & 0xFFFC;
        }
        ......
    }
  * \endcode
  */
T_PLXS_APP_RESULT  plxs_get_parameter(T_PLXS_PARAM_TYPE param_type, void *p_value)
{
    T_PLXS_APP_RESULT ret = PLXS_APP_RESULT_SUCCESS;
    if (p_value == NULL)
    {
        ret = PLXS_APP_RESULT_POINTER_NULL;
        PROFILE_PRINT_ERROR0("plxs_get_parameter:PLXS_APP_RESULT_POINTER_NULL");
        return ret;
    }
    switch (param_type)
    {
    case PLXS_PARAM_FEATURE_FLAGS:
        {
            //get feature value
            memcpy((T_PLXS_FEATURES_VALUE *)p_value, &plxs_features, sizeof(T_PLXS_FEATURES_VALUE));
        }
        break;
#if PLXS_CONTINUOUS_MEASUREMENT_SUPPORT//continuous-measurement start
    case PLXS_PARAM_CONTINUOUS_MEASUREMENT_FLAGS:
        {
            //get continuous measurement flags
            memcpy((uint8_t *)p_value, &plxs_continuous_measurement_flags, 1);
        }
        break;
#endif //continuous-measurement end

#if PLXS_SPOT_CHECK_MEASUREMENT_SUPPORT //spot-check start
    case PLXS_PARAM_SPOT_CHECK_MEASUREMENT_FLAGS:
        {
            //get spot check measurement flags
            memcpy((uint8_t *)p_value, &plxs_spot_check_measurement_flags, 1);
        }
        break;
#endif //spot-check end
    default:
        {
            ret = PLXS_APP_RESULT_INVALID_TYPE;
        }
    }
    return ret;
}


/**
  * @brief pop record from database
  *
  * @param[in] p_spot_check_value  pointer to p_spot_check_value  @ref T_PLXS_SPOT_CHECK_MEASUREMENT_VALUE
  * @return result
  * @retval  PLXS_APP_RESULT_SUCCESS operator success
  * @retval  others  operator fail
  */
#if PLXS_STORE_SPOT_CHECK_MEASUREMENT_SUPPORT
T_PLXS_APP_RESULT pop_record_from_database(T_PLXS_SPOT_CHECK_MEASUREMENT_VALUE *p_spot_check_value)
{
    T_PLXS_APP_RESULT ret = PLXS_APP_RESULT_SUCCESS;
    if (p_spot_check_value == NULL)
    {
        ret = PLXS_APP_RESULT_POINTER_NULL;
        PROFILE_PRINT_ERROR0("pop_record_from_database:PLXS_APP_RESULT_POINTER_NULL");
        return ret;
    }
#if PLXS_STORE_SPOT_CHECK_MEASUREMENT_SUPPORT
    if (plxs_racp.record_db.tail == plxs_racp.record_db.head)
    {
        PROFILE_PRINT_ERROR0("pop_record_from_database: PLXS_APP_RESULT_QUEUE_NULL");
        ret = PLXS_APP_RESULT_QUEUE_NULL;
        return ret;
    }
    *p_spot_check_value =
        plxs_racp.record_db.records[plxs_racp.record_db.head].plxs_spot_check_measurement_value;
    plxs_racp.record_db.head = (plxs_racp.record_db.head + 1) % PLXS_RACP_DATABASE_SIZE;
    plxs_racp.record_db.record_num--;
#else
    PROFILE_PRINT_ERROR0("pop_record_from_database: not support");
    ret = PLXS_APP_RESULT_NOT_SUPPORT;
#endif
    return ret;
}

/**
  * @brief  push spot check measurement records to database
  * @param[in]  spot_check_value
  * @return result
  * @retval  PLXS_APP_RESULT_SUCCESS operator success
  * @retval  others  operator fail
  * <b>Example usage</b>
  * \code{.c}
    void test_plxs(void)
    {
         T_PLXS_SPOT_CHECK_MEASUREMENT_VALUE  plxs_spot_check_measure_value;
        //spot_check_indication_test
        bool result = push_record_to_database(plxs_spot_check_measure_value);
        if(result == PLXS_APP_RESULT_SUCCESS)
        {
          ......
        }
    }
  * \endcode
  */
T_PLXS_APP_RESULT  push_record_to_database(T_PLXS_SPOT_CHECK_MEASUREMENT_VALUE spot_check_value)
{
    T_PLXS_APP_RESULT ret = PLXS_APP_RESULT_SUCCESS;
#if PLXS_STORE_SPOT_CHECK_MEASUREMENT_SUPPORT
    PROFILE_PRINT_INFO0("plxs_ push_record_to_database  enter");
    if (((plxs_racp.record_db.tail + 1) % PLXS_RACP_DATABASE_SIZE) == plxs_racp.record_db.head)
    {
        PROFILE_PRINT_ERROR0("push_record_to_database database full delete the oldest record");
        T_PLXS_SPOT_CHECK_MEASUREMENT_VALUE spot_check_value;
        pop_record_from_database(&spot_check_value);
    }
    plxs_racp.record_db.records[plxs_racp.record_db.tail].plxs_spot_check_measurement_value =
        spot_check_value;
    plxs_racp.record_db.tail = (plxs_racp.record_db.tail + 1) % PLXS_RACP_DATABASE_SIZE;
    plxs_racp.record_db.record_num++;
#else
    PROFILE_PRINT_ERROR0("plxs_ push_record_to_database not support");
    ret = PLXS_APP_RESULT_NOT_SUPPORT;
#endif
    return ret;
}

/**
  * @brief  send plxs spot check measurement characteristic indication from records database
  *
  * @param[in] conn_id  connection index
  * @param[in] Service_id  generated by the BLE stack: @ref T_SERVER_ID.
  * @param[in] index  record index in database
  * @return plxs_spot_check_measurement_value_store_indicate result
  * @retval PLXS_APP_RESULT_SUCCESS  Operation success.
  * @retval others Operation fail.
  *
  * <b>Example usage</b>
  * \code{.c}
     bool plxs_report_records_task(uint8_t conn_id, T_SERVER_ID service_id)
     {
         if (plxs_spot_check_measurement_value_store_indicate(conn_id, service_id,plxs_report_records_flags.plxs_current_record_to_report++) == PLXS_APP_RESULT_SUCCESS)
        {
            PROFILE_PRINT_INFO1("plxs_report_records_task:record:%d,success",plxs_report_records_flags.plxs_current_record_to_report);
        }
        else
        {
            ret = false;
            PROFILE_PRINT_ERROR1("plxs_report_records_task:record:%d,fail",plxs_report_records_flags.plxs_current_record_to_report);
        }
     }
  * \endcode
  */
T_PLXS_APP_RESULT plxs_spot_check_measurement_value_store_indicate(uint8_t conn_id,
                                                                   T_SERVER_ID service_id, uint8_t index)
{
    T_PLXS_APP_RESULT ret = PLXS_APP_RESULT_SUCCESS;
    PROFILE_PRINT_INFO1("plxs_spot_check_measurement_value_store_indicate enter  index = %d", index);
    if (plxs_data_send_status_flags.report_records_indication_status_flag ==
        PLXS_STATUS_REPORT_RECORDS_INDICATION_DOING
        || plxs_data_send_status_flags.spot_check_indication_status_flag ==
        PLXS_STATUS_SPOT_CHECK_MEASUREMENT_INDICATION_DOING)
    {
        ret = PLXS_APP_RESULT_PENDING;
        PROFILE_PRINT_ERROR0("plxs_spot_check_measurement_value_store_indicate:indication procedure is doing please send data later!!");
        return ret;
    }
    if (plxs_notify_indicate_flag.spot_check_measurement_indication_enable == 0)
    {
        ret = PLXS_APP_RESULT_CCCD_NOT_ENABLED;
        PROFILE_PRINT_ERROR0("plxs_spot_check_measurement_value_store_indicate: PLXS_APP_RESULT_CCCD_NOT_ENABLED");
        return ret;
    }
    int current = (plxs_report_records_flags.plxs_record_to_report_start_index + index) %
                  PLXS_RACP_DATABASE_SIZE;
    T_PLXS_SPOT_CHECK_MEASUREMENT_VALUE *p_spot_check_measurement_value =
        &plxs_racp.record_db.records[current].plxs_spot_check_measurement_value;
    uint8_t temp_spot_check_measurement_value[sizeof(T_PLXS_SPOT_CHECK_MEASUREMENT_VALUE)];
    uint8_t offset = 0;
    if (index >= plxs_racp.record_db.record_num)
    {
        PROFILE_PRINT_ERROR2("plxs_spot_check_measurement_value_store_indicate access overflow, index = %d,total_record_num = %d",
                             index, plxs_racp.record_db.record_num);
        ret = PLXS_APP_RESULT_INVALID_OFFSET;
        return ret;
    }
    if (current == plxs_racp.record_db.tail)
    {
        PROFILE_PRINT_ERROR0("plxs_spot_check_measurement_value_store_indicate database empty");
        ret = PLXS_APP_RESULT_QUEUE_NULL;
        return ret;
    }
    memcpy(&temp_spot_check_measurement_value[offset], &plxs_spot_check_measurement_flags, 1);
    offset += 1;
    memcpy(&temp_spot_check_measurement_value[offset], &p_spot_check_measurement_value->spo2, 2);
    offset += 2;
    memcpy(&temp_spot_check_measurement_value[offset], &p_spot_check_measurement_value->pr, 2);
    offset += 2;
#if PLXS_SPOT_CHECK_MEASUREMENT_TIMESTAMP_SUPPORT
    if (plxs_spot_check_measurement_flags & PLXS_SPOT_CHECK_MEASUREMENT_FLAG_TIMESTAMP)
    {
        memcpy(&temp_spot_check_measurement_value[offset], &p_spot_check_measurement_value->time, 7);
        offset += 7;
    }
#endif

#if PLXS_MEASUREMENT_STATE_SUPPORT
    if (plxs_spot_check_measurement_flags & PLXS_SPOT_CHECK_MEASUREMENT_FLAG_MEASUREMENT_STATUS)
    {
        memcpy(&temp_spot_check_measurement_value[offset],
               &p_spot_check_measurement_value->measurement_status, 2);
        offset += 2;
    }
#endif

# if PLXS_DEVICE_AND_SENSOR_STATUS_SUPPORT
    if (plxs_spot_check_measurement_flags & PLXS_SPOT_CHECK_MEASUREMENT_FLAG_DEVICE_SENSOR_STATUS)
    {
        memcpy(&temp_spot_check_measurement_value[offset],
               &p_spot_check_measurement_value->device_and_sensor_status, 3);
        offset += 3;
    }
#endif

#if PLXS_PULSE_AMPLITUDE_INDEX_SUPPORT
    if (plxs_spot_check_measurement_flags & PLXS_SPOT_CHECK_MEASUREMENT_FLAG_PULSE_AMPLITUDE_INDEX)
    {
        memcpy(&temp_spot_check_measurement_value[offset],
               &p_spot_check_measurement_value->pulse_amplitude_index, 2);
        offset += 2;
    }
#endif

    if (server_send_data(conn_id, service_id, PLXS_CHAR_SPOT_CHECK_MENSUREMENT_INDEX,
                         temp_spot_check_measurement_value, offset, GATT_PDU_TYPE_INDICATION))
    {
        PROFILE_PRINT_INFO0("plxs_spot_check_measurement_value_store_indicate:server_send_data and set status doing success");
        plxs_data_send_status_flags.report_records_indication_status_flag =
            PLXS_STATUS_REPORT_RECORDS_INDICATION_DOING;
    }
    else
    {
        ret = PLXS_APP_RESULT_FAIL;
        PROFILE_PRINT_ERROR0("plxs_spot_check_measurement_value_store_indicate:server_send_data fail");
        plxs_data_send_status_flags.report_records_indication_status_flag =
            PLXS_STATUS_REPORT_RECORDS_INDICATION_IDLE;
    }
    return ret;
}
#endif
/**
  * @brief  send plxs spot check measurement characteristic indication
  *
  * @param[in] conn_id  connection index
  * @param[in] Service_id  generated by the BLE stack: @ref T_SERVER_ID.
  * @param[in] p_spot_check_measurement_value @ref T_PLXS_SPOT_CHECK_MEASUREMENT_VALUE
  * @return plxs_spot_check_measurement_value_indicate result
  * @retval PLXS_APP_RESULT_SUCCESS Operation success.
  * @retval others Operation fail.
  *
  * <b>Example usage</b>
  * \code{.c}
     void test_plxs(void)
     {
        T_PLXS_SPOT_CHECK_MEASUREMENT_VALUE  plxs_spot_check_measure_value;
        ......//plxs_spot_check_measure_value initialization
        bool result = plxs_spot_check_measurement_value_indicate(conn_id,plxs_srv_id,&plxs_spot_check_measure_value);
        if(result == PLXS_APP_RESULT_SUCCESS)
        {
        }
     }
  * \endcode
  */
#if PLXS_SPOT_CHECK_MEASUREMENT_SUPPORT
T_PLXS_APP_RESULT plxs_spot_check_measurement_value_indicate(uint8_t conn_id, uint8_t service_id,
                                                             T_PLXS_SPOT_CHECK_MEASUREMENT_VALUE *p_spot_check_measurement_value)
{
    T_PLXS_APP_RESULT ret = PLXS_APP_RESULT_SUCCESS;
    if (p_spot_check_measurement_value == NULL)
    {
        ret = PLXS_APP_RESULT_POINTER_NULL;
        PROFILE_PRINT_ERROR0("plxs_spot_check_measurement_value_indicate: PLXS_APP_RESULT_POINTER_NULL");
        return ret;
    }
    if (plxs_notify_indicate_flag.spot_check_measurement_indication_enable == 0)
    {
        ret = PLXS_APP_RESULT_CCCD_NOT_ENABLED;
        PROFILE_PRINT_ERROR0("plxs_spot_check_measurement_value_indicate: PLXS_APP_RESULT_CCCD_NOT_ENABLED");
        return ret;
    }
    PROFILE_PRINT_INFO0("plxs_spot_check_measurement_value_indicate enter");
    if (plxs_data_send_status_flags.spot_check_indication_status_flag ==
        PLXS_STATUS_SPOT_CHECK_MEASUREMENT_INDICATION_DOING
#if PLXS_STORE_SPOT_CHECK_MEASUREMENT_SUPPORT
        || plxs_racp.ctrl_point.op_code == PLXS_RACP_OPCODE_REPORT_RECS
#endif
       )
    {
        ret = PLXS_APP_RESULT_PENDING;
        PROFILE_PRINT_ERROR0("plxs_spot_check_measurement_value_indicate:procedure is doing please send data later!!");
        return ret;
    }
    uint8_t temp_plxs_spot_check_measurement_value[sizeof(T_PLXS_SPOT_CHECK_MEASUREMENT_VALUE)];
    uint8_t offset = 0;
    memcpy(&temp_plxs_spot_check_measurement_value[offset], &plxs_spot_check_measurement_flags, 1);
    offset += 1;
    memcpy(&temp_plxs_spot_check_measurement_value[offset], &p_spot_check_measurement_value->spo2, 2);
    offset += 2;
    memcpy(&temp_plxs_spot_check_measurement_value[offset], &p_spot_check_measurement_value->pr, 2);
    offset += 2;

#if PLXS_SPOT_CHECK_MEASUREMENT_TIMESTAMP_SUPPORT
    if (plxs_spot_check_measurement_flags & PLXS_SPOT_CHECK_MEASUREMENT_FLAG_TIMESTAMP)
    {
        memcpy(&temp_plxs_spot_check_measurement_value[offset], &p_spot_check_measurement_value->time, 7);
        offset += 7;
    }
#endif

#if PLXS_MEASUREMENT_STATE_SUPPORT
    if (plxs_spot_check_measurement_flags & PLXS_SPOT_CHECK_MEASUREMENT_FLAG_MEASUREMENT_STATUS)
    {
        memcpy(&temp_plxs_spot_check_measurement_value[offset],
               &p_spot_check_measurement_value->measurement_status, 2);
        offset += 2;
    }
#endif

# if PLXS_DEVICE_AND_SENSOR_STATUS_SUPPORT
    if (plxs_spot_check_measurement_flags & PLXS_SPOT_CHECK_MEASUREMENT_FLAG_DEVICE_SENSOR_STATUS)
    {
        memcpy(&temp_plxs_spot_check_measurement_value[offset],
               &p_spot_check_measurement_value->device_and_sensor_status, 3);
        offset += 3;
    }
#endif

#if PLXS_PULSE_AMPLITUDE_INDEX_SUPPORT
    if (plxs_spot_check_measurement_flags & PLXS_SPOT_CHECK_MEASUREMENT_FLAG_PULSE_AMPLITUDE_INDEX)
    {
        memcpy(&temp_plxs_spot_check_measurement_value[offset],
               &p_spot_check_measurement_value->pulse_amplitude_index, 2);
        offset += 2;
    }
#endif

    if (server_send_data(conn_id, service_id, PLXS_CHAR_SPOT_CHECK_MENSUREMENT_INDEX,
                         temp_plxs_spot_check_measurement_value, offset,
                         GATT_PDU_TYPE_INDICATION))
    {
        PROFILE_PRINT_INFO0("plxs_spot_check_measurement_value_indicate:server_send_data and set status doing success");
        plxs_data_send_status_flags.spot_check_indication_status_flag =
            PLXS_STATUS_SPOT_CHECK_MEASUREMENT_INDICATION_DOING;
    }
    else
    {
        ret = PLXS_APP_RESULT_FAIL;
        PROFILE_PRINT_ERROR0("plxs_spot_check_measurement_value_indicate:server_send_data fail");
        //if send fail data may be need to be stored
#if PLXS_STORE_SPOT_CHECK_MEASUREMENT_SUPPORT
        push_record_to_database(*p_spot_check_measurement_value);
#endif
        plxs_data_send_status_flags.spot_check_indication_status_flag =
            PLXS_STATUS_SPOT_CHECK_MEASUREMENT_INDICATION_IDLE;
    }
    return ret;
}
#endif
/**
  * @brief  send plxs continuous measurement characteristic notification
  *
  * @param[in] conn_id  connection index
  * @param[in] Service_id  generated by the BLE stack: @ref T_SERVER_ID.
  * @param[in] plxs_continuous_measurement_value @ref T_PLXS_CONTINUOUS_MEASUREMENT_VALUE
  * @return plxs_continuous_measurement_value_notify result
  * @retval PLXS_APP_RESULT_SUCCESS Operation success.
  * @retval others Operation fail.
  *
  * <b>Example usage</b>
  * \code{.c}
     void test_plxs(void)
     {
        ......//plxs_continuous_measurement_value initialization
        bool result = plxs_continuous_measurement_value_notify( conn_id, plxs_srv_id,&plxs_continuous_measurement_value);
        if(result == PLXS_APP_RESULT_SUCCESS)
        {
            PROFILE_PRINT_INFO0("plxs_continuous_measurement_value_notify notify data send success!");
        }
     }
  * \endcode
  */
#if PLXS_CONTINUOUS_MEASUREMENT_SUPPORT
T_PLXS_APP_RESULT plxs_continuous_measurement_value_notify(uint8_t conn_id, T_SERVER_ID service_id,
                                                           T_PLXS_CONTINUOUS_MEASUREMENT_VALUE *p_plxs_continuous_measurement_value)
{
    T_PLXS_APP_RESULT ret = PLXS_APP_RESULT_SUCCESS;
    if (p_plxs_continuous_measurement_value == NULL)
    {
        ret = PLXS_APP_RESULT_POINTER_NULL;
        PROFILE_PRINT_ERROR0("plxs_continuous_measurement_value_notify:PLXS_APP_RESULT_POINTER_NULL");
        return ret;
    }
    if (plxs_notify_indicate_flag.continuous_measurement_notify_enable == 0)
    {
        ret = PLXS_APP_RESULT_CCCD_NOT_ENABLED;
        PROFILE_PRINT_ERROR0("plxs_continuous_measurement_value_notify: PLXS_APP_RESULT_CCCD_NOT_ENABLED");
        return ret;
    }
    if (plxs_data_send_status_flags.continuous_measurement_notify_status_flag ==
        PLXS_STATUS_CONTINUOUS_MEASUREMENT_NOTIFY_DOING)
    {
        ret = PLXS_APP_RESULT_PENDING;
        PROFILE_PRINT_ERROR0("plxs_continuous_measurement_value_notify:procedure is doing please send data later!!");
        return ret;
    }
    uint8_t temp_plxs_continuous_measurement_value[sizeof(T_PLXS_CONTINUOUS_MEASUREMENT_VALUE)];
    uint8_t offset = 0;

    memcpy(&temp_plxs_continuous_measurement_value[offset], &plxs_continuous_measurement_flags, 1);
    offset += 1;
    memcpy(&temp_plxs_continuous_measurement_value[offset],
           &p_plxs_continuous_measurement_value->spo2pr_normal.spo2, 2);
    offset += 2;
    memcpy(&temp_plxs_continuous_measurement_value[offset],
           &p_plxs_continuous_measurement_value->spo2pr_normal.pr, 2);
    offset += 2;
#if PLXS_CONTINUOUS_MEASUREMENT_SPO2PR_FAST_SUPPORT
    if (plxs_continuous_measurement_flags & PLXS_CONTINUOUS_MEASUREMENT_FLAG_SPO2PR_FAST)
    {
        memcpy(&temp_plxs_continuous_measurement_value[offset],
               &p_plxs_continuous_measurement_value->spo2pr_fast.spo2, 2);
        offset += 2;
        memcpy(&temp_plxs_continuous_measurement_value[offset],
               &p_plxs_continuous_measurement_value->spo2pr_fast.pr, 2);
        offset += 2;
    }
#endif

#if PLXS_CONTINUOUS_MEASUREMENT_SPO2PR_SLOW_SUPPORT
    if (plxs_continuous_measurement_flags & PLXS_CONTINUOUS_MEASUREMENT_FLAG_SPO2PR_SLOW)
    {
        memcpy(&temp_plxs_continuous_measurement_value[offset],
               &p_plxs_continuous_measurement_value->spo2pr_slow.spo2, 2);
        offset += 2;
        memcpy(&temp_plxs_continuous_measurement_value[offset],
               &p_plxs_continuous_measurement_value->spo2pr_slow.pr, 2);
        offset += 2;
    }
#endif

#if PLXS_MEASUREMENT_STATE_SUPPORT
    if (plxs_continuous_measurement_flags & PLXS_CONTINUOUS_MEASUREMENT_FLAG_MEASUREMENT_STATUS)
    {
        memcpy(&temp_plxs_continuous_measurement_value[offset],
               &p_plxs_continuous_measurement_value->measurement_status, 2);
        offset += 2;
    }
#endif

# if PLXS_DEVICE_AND_SENSOR_STATUS_SUPPORT
    if (plxs_continuous_measurement_flags & PLXS_CONTINUOUS_MEASUREMENT_FLAG_DEVICE_SENSOR_STATUS)
    {
        memcpy(&temp_plxs_continuous_measurement_value[offset],
               &p_plxs_continuous_measurement_value->device_and_sensor_status, 3);
        offset += 3;
    }
#endif

#if PLXS_PULSE_AMPLITUDE_INDEX_SUPPORT
    if (plxs_continuous_measurement_flags & PLXS_CONTINUOUS_MEASUREMENT_FLAG_PULSE_AMPLITUDE_INDEX)
    {
        memcpy(&temp_plxs_continuous_measurement_value[offset],
               &p_plxs_continuous_measurement_value->pulse_amplitude_index, 2);
        offset += 2;
    }
#endif
    if (server_send_data(conn_id, service_id, PLXS_CHAR_CONTINUOUS_MEASUREMENT_INDEX,
                         temp_plxs_continuous_measurement_value, offset,
                         GATT_PDU_TYPE_NOTIFICATION))
    {
        PROFILE_PRINT_INFO0("plxs_continuous_measurement_value_notify:server_send_data and set status doing success");
        plxs_data_send_status_flags.continuous_measurement_notify_status_flag =
            PLXS_STATUS_CONTINUOUS_MEASUREMENT_NOTIFY_DOING;

    }
    else
    {
        ret = PLXS_APP_RESULT_FAIL;
        PROFILE_PRINT_ERROR0("plxs_continuous_measurement_value_notify:server_send_data fail");
        plxs_data_send_status_flags.continuous_measurement_notify_status_flag =
            PLXS_STATUS_CONTINUOUS_MEASUREMENT_NOTIFY_IDLE;
    }
    return ret;
}
#endif
/**
  * @brief  check cccd enabled or disabled
  *
  * @param[in] None
  * @return cccd check result
  * @retval false Operation failure.
  * @retval true Operation success.
  */
bool plxs_check_cccd()
{
    if ((0 == plxs_notify_indicate_flag.racp_indication_enable) ||
        (0 == plxs_notify_indicate_flag.spot_check_measurement_indication_enable))
    {
        return false;//cccd not enabled return false
    }
    else
    {
        return true;
    }
}
/**
  * @brief read characteristic data from service.
  *
  * @param[in] conn_id  Connection id.
  * @param[in] service_id          ServiceID of characteristic data.
  * @param[in] attrib_index       Attribute index of getting characteristic data.
  * @param[in] offset            Used for Blob Read.
  * @param[in,out] p_length           length of getting characteristic data.
  * @param[in,out] pp_value            data got from service.
  * @return Profile procedure result
  */
T_APP_RESULT plxs_attr_read_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                               uint16_t offset, uint16_t *p_length, uint8_t **pp_value)
{
    T_APP_RESULT cause = APP_RESULT_SUCCESS;
    PROFILE_PRINT_INFO2("plxs_attr_read_cb attribIndex = %d offset %x", attrib_index, offset);
    *p_length = 0;
    static uint8_t   temp_plxs_features[sizeof(T_PLXS_FEATURES_VALUE)] = {0};
    switch (attrib_index)
    {
    case PLXS_CHAR_FEATURE_INDEX:
        {
            memcpy(&temp_plxs_features[*p_length], &plxs_features.supported_features,
                   sizeof(plxs_features.supported_features));
            *p_length = sizeof(plxs_features.supported_features);
#if PLXS_MEASUREMENT_STATE_SUPPORT
            if (plxs_features.supported_features & PLXS_FEATURES_MEASUREMENT_STATUS)
            {
                PROFILE_PRINT_INFO1("plxs_features.measurement_status 0x%x",
                                    plxs_features.measurement_status_support_bits);
                memcpy(&temp_plxs_features[*p_length], &plxs_features.measurement_status_support_bits, 2);
                *p_length += 2;
            }
#endif

#if PLXS_DEVICE_AND_SENSOR_STATUS_SUPPORT
            if (plxs_features.supported_features & PLXS_FEATURES_DEVICE_SENSOR_STATUS)
            {
                PROFILE_PRINT_INFO1("plxs_features.device_and_sensor_status 0x%x",
                                    plxs_features.device_and_sensor_status_support_bits);
                memcpy(&temp_plxs_features[*p_length], &plxs_features.device_and_sensor_status_support_bits, 3);
                *p_length += 3;
            }
#endif
            *pp_value = temp_plxs_features;
        }
        break;
    default:
        {
            PROFILE_PRINT_ERROR1("plxs_attr_read_cb, attr not found, index=%d", attrib_index);
            cause  = APP_RESULT_ATTR_NOT_FOUND;
        }
        break;
    }
    return cause;
}


/**
  * @brief abort procedure.
  * @param[in] None
  * @return None
  *
  */
#if PLXS_STORE_SPOT_CHECK_MEASUREMENT_SUPPORT
void plxs_abort_racp_procedure(void)
{
    if (plxs_racp.ctrl_point.op_code == PLXS_RACP_OPCODE_REPORT_RECS)
    {
        plxs_abort_by_app_flag = true;
    }
    else
    {
        PROFILE_PRINT_INFO0("plxs_abort_racp_procedure no procedure to abort by application");
    }
}

/**
  * @brief response report records number control point procedure result to client
  *
  * @param[in] conn_id      Connection id.
  * @param[in] service_id          ServiceID of characteristic data.
  * @param[in] num               number of records
  * @return report records number procedure result
  */
bool plxs_racp_num_response(uint8_t conn_id, T_SERVER_ID service_id, uint16_t num)
{
    bool ret;
    T_PLXS_RACP temp_racp;
    uint16_t attrib_index = PLXS_CHAR_RECORS_ACCESS_CONTROL_POINT_INDEX;
    LE_UINT16_TO_ARRAY(temp_racp.ctrl_point.operand, num);
    temp_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_NBR_OF_RECS_RESP;
    temp_racp.ctrl_point.op = PLXS_RACP_OPERATOR_NULL;
    temp_racp.cp_length = sizeof(T_PLXS_CTRL_POINT_OPCODE) + sizeof(T_PLXS_CTRL_POINT_OPERATOR)
                          + sizeof(uint16_t);
    ret = server_send_data(conn_id, service_id, attrib_index, (uint8_t *)&temp_racp.ctrl_point,
                           temp_racp.cp_length, GATT_PDU_TYPE_INDICATION);
    PROFILE_PRINT_INFO1("plxs_racp_num_response num of records: %d \n", num);
    return ret;
}

/**
  * @brief response control point procedure result to client
  *
  * @param[in] conn_id      Connection id.
  * @param[in] service_id          ServiceID of characteristic data.
  * @param[in] opcode               opcode
  * @param[in] rsp_code
  * @return response control point procedure result
  */
bool plxs_racp_response(uint8_t conn_id, T_SERVER_ID service_id, uint8_t opcode, uint8_t rsp_code)
{
    bool ret;
    T_PLXS_RACP temp_racp;
    uint16_t attrib_index = PLXS_CHAR_RECORS_ACCESS_CONTROL_POINT_INDEX;

    temp_racp.ctrl_point.operand[0] = opcode;
    temp_racp.ctrl_point.operand[1] = rsp_code;
    temp_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESP_CODE;
    temp_racp.ctrl_point.op = PLXS_RACP_OPERATOR_NULL;
    temp_racp.cp_length = sizeof(T_PLXS_CTRL_POINT_OPCODE) + sizeof(T_PLXS_CTRL_POINT_OPERATOR)
                          + sizeof(T_PLXS_CTRL_POINT_OPCODE) + sizeof(T_PLXS_CTRL_POINT_RESP_CODES);

    // send indication to client
    ret = server_send_data(conn_id, service_id, attrib_index, (uint8_t *)&temp_racp.ctrl_point,
                           temp_racp.cp_length, GATT_PDU_TYPE_INDICATION);
    PROFILE_PRINT_INFO1("plxs_racp_response code value: %d \n", rsp_code);
    return ret;
}

/**
  * @brief  delete spot check measurement records from database
  *
  * @param[in] None
  * @return delete result
  * @retval true success
  * <b>Example usage</b>
  * \code{.c}
    void test_plxs(void)
    {
        delete_all_records_from_database();
    }
  * \endcode
  */
bool delete_all_records_from_database()
{
    bool ret = true;
    plxs_racp.record_db.tail = plxs_racp.record_db.head;
    plxs_racp.record_db.record_num = 0;
    return ret;
}

/**
  * @brief  check if report records procedure has complete,if not complete continuous report records
  *
  * @param[in] conn_id  connection index
  * @param[in] Service_id  generated by the BLE stack: @ref T_SERVER_ID.
  * @return check result
  * @retval false Operation failure.
  * @retval true Operation success.
  */
bool plxs_check_records_report_complete(uint8_t conn_id, T_SERVER_ID service_id)
{

    if (plxs_racp.ctrl_point.op_code == PLXS_RACP_OPCODE_REPORT_RECS)
    {
        PROFILE_PRINT_INFO1("plxs_records_to_report_offset:%d",
                            plxs_report_records_flags.plxs_records_to_report_offset);
        plxs_report_records_flags.plxs_records_to_report_offset =
            plxs_report_records_flags.plxs_current_record_to_report;
        return false;
    }
    else
    {
        return true;
    }
}

/**
  * @brief report spot check measurement value stored in database to client
  *
  * @param[in] conn_id  connection index
  * @param[in] Service_id  generated by the BLE stack: @ref T_SERVER_ID.
  * @return plxs report records task result
  * @retval false Operation failure.
  * @retval true Operation success.
  */
bool plxs_report_records_task(uint8_t conn_id, T_SERVER_ID service_id)
{
    bool ret = true;
    if (plxs_abort_flag  == true)
    {
        uint8_t opcode = PLXS_RACP_OPCODE_ABORT_OPERATION;
        PROFILE_PRINT_INFO2("plxs_report_records_task request opcode = %x,response code value = %x", opcode,
                            PLXS_RACP_RESP_SUCCESS);
        /*in this case when send response to client means that the procedure has end,should set plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED*/
        plxs_racp_response(conn_id, service_id, opcode, PLXS_RACP_RESP_SUCCESS);
        plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED;
        plxs_abort_flag = false;
        plxs_report_records_flags.plxs_send_data_end_flag = true;

        return ret;
    }
    if (plxs_abort_by_app_flag == true)
    {
        PROFILE_PRINT_INFO2("plxs_report_records_task request opcode = %x,response code value = %x",
                            plxs_racp.ctrl_point.op_code, PLXS_RACP_RESP_PROC_NOT_COMPLETED);
        /*in this case when send response to client means that the procedure has end,should set plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED*/
        plxs_racp_response(conn_id, service_id, plxs_racp.ctrl_point.op_code,
                           PLXS_RACP_RESP_PROC_NOT_COMPLETED);
        plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED;
        plxs_abort_by_app_flag = false;
        plxs_report_records_flags.plxs_send_data_end_flag = true;
        return ret;
    }
    if (plxs_report_records_flags.plxs_send_data_end_flag == true)
    {
        PROFILE_PRINT_INFO0("plxs_report_records_task plxs_send_data_end_flag== true");
        //plxs_send_data_end_flag = false;
        return ret;
    }
    if ((plxs_report_records_flags.plxs_current_record_to_report >=
         plxs_report_records_flags.plxs_num_records_to_report) &&
        plxs_racp.ctrl_point.op_code == PLXS_RACP_OPCODE_REPORT_RECS)
    {
        PROFILE_PRINT_INFO0("plxs_report_records_task all records send success");
        plxs_report_records_flags.plxs_send_data_end_flag = true;
        plxs_report_records_flags.plxs_current_record_to_report = 0;
        //plxs_records_to_report_offset = 0;
        //plxs_record_to_report_start_index = 0;
        plxs_racp_response(conn_id, service_id, plxs_racp.ctrl_point.op_code, PLXS_RACP_RESP_SUCCESS);
        /*in this case when send response to client means that the procedure has end,should set plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED*/
        plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED;
        return true;
    }
    PROFILE_PRINT_INFO3("plxs_report_records_task  current = %d, total = %d,start_index = %d ",
                        plxs_report_records_flags.plxs_current_record_to_report,
                        plxs_report_records_flags.plxs_num_records_to_report,
                        plxs_report_records_flags.plxs_record_to_report_start_index);

    if (plxs_spot_check_measurement_value_store_indicate(conn_id, service_id,
                                                         plxs_report_records_flags.plxs_current_record_to_report++) == PLXS_APP_RESULT_SUCCESS)
    {
        PROFILE_PRINT_INFO1("plxs_report_records_task:record:%d,success",
                            plxs_report_records_flags.plxs_current_record_to_report);
    }
    else
    {
        ret = false;
        PROFILE_PRINT_ERROR1("plxs_report_records_task:record:%d,fail",
                             plxs_report_records_flags.plxs_current_record_to_report);
    }
    return ret;
}

/**
  * @brief     plxs report records
  *
  * @param[in] conn_id    conn_id
  * @param[in] Service id   generated by the BLE stack: @ref T_SERVER_ID.
  * @return None
  *
  */
void plxs_report_records(uint8_t conn_id, T_SERVER_ID service_id)
{
    PROFILE_PRINT_INFO2(" plxs_report_records opcode = %x,operator = %x",
                        plxs_racp.ctrl_point.op_code, plxs_racp.ctrl_point.op);
    if (plxs_racp.ctrl_point.op >= 7)
    {
        PROFILE_PRINT_ERROR0("plxs_report_records  PLXS_RACP_RESP_INVALID_OPERATOR");
        plxs_racp_response(conn_id, service_id, plxs_racp.ctrl_point.op_code,
                           PLXS_RACP_RESP_OPERATOR_NOT_SUPPORTED);
        /*in this case when send response to client means that the procedure has end,should set plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED*/
        plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED;
        return;
    }
    if (plxs_racp.ctrl_point.op != PLXS_RACP_OPERATOR_ALL_RECS)
    {
        PROFILE_PRINT_ERROR0("plxs_report_records  PLXS_RACP_RESP_OPERATOR_NOT_SUPPORTED");
        plxs_racp_response(conn_id, service_id, plxs_racp.ctrl_point.op_code,
                           PLXS_RACP_RESP_INVALID_OPERATOR);
        /*in this case when send response to client means that the procedure has end,should set plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED*/
        plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED;
        return;
    }
    if (plxs_racp.record_db.record_num == 0)
    {
        PROFILE_PRINT_ERROR0("plxs_report_records PLXS_RACP_RESP_NO_RECS_FOUND");
        plxs_racp_response(conn_id, service_id, plxs_racp.ctrl_point.op_code, PLXS_RACP_RESP_NO_RECS_FOUND);
        /*in this case when send response to client means that the procedure has end,should set plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED*/
        plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED;
        return ;
    }
    else
    {

        plxs_report_records_flags.plxs_record_to_report_start_index = plxs_racp.record_db.head;
        plxs_report_records_flags.plxs_current_record_to_report =
            plxs_report_records_flags.plxs_records_to_report_offset;
        plxs_report_records_flags.plxs_send_data_end_flag = false;
        plxs_report_records_flags.plxs_num_records_to_report = plxs_racp.record_db.record_num;
        if (plxs_report_records_flags.plxs_records_to_report_offset >=
            plxs_report_records_flags.plxs_num_records_to_report)
        {
            PROFILE_PRINT_ERROR0("plxs_report_records after first report success no records to be report");
            plxs_racp_response(conn_id, service_id, plxs_racp.ctrl_point.op_code, PLXS_RACP_RESP_NO_RECS_FOUND);
            /*in this case when send response to client means that the procedure has end,should set plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED*/
            plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED;
            return ;
        }
        if (false ==  plxs_report_records_task(conn_id, service_id))
        {
            PROFILE_PRINT_ERROR0("plxs_report_records_task false");
            plxs_racp_response(conn_id, service_id, PLXS_RACP_OPCODE_REPORT_RECS,
                               PLXS_RACP_RESP_PROC_NOT_COMPLETED);
            /*in this case when send response to client means that the procedure has end,should set plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED*/
            plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED;
            plxs_report_records_flags.plxs_send_data_end_flag = true;
            plxs_report_records_flags.plxs_current_record_to_report =
                plxs_report_records_flags.plxs_records_to_report_offset;
        };
    }
}

/**
  * @brief     plxs_report_num_of_records
  *
  * @param[in] conn_id    conn_id
  * @param[in] Service id   generated by the BLE stack: @ref T_SERVER_ID.
  * @return None
  *
  */
void plxs_report_num_of_records(uint8_t conn_id, T_SERVER_ID service_id)
{
    PROFILE_PRINT_INFO2(" plxs_report_num_of_records opcode = %x,operator = %x",
                        plxs_racp.ctrl_point.op_code, plxs_racp.ctrl_point.op);
    if (plxs_racp.ctrl_point.op >= 7)
    {
        plxs_racp_response(conn_id, service_id, plxs_racp.ctrl_point.op_code,
                           PLXS_RACP_RESP_OPERATOR_NOT_SUPPORTED);
        /*in this case when send response to client means that the procedure has end,should set plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED*/
        plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED;
        return;
    }
    if (plxs_racp.ctrl_point.op != PLXS_RACP_OPERATOR_ALL_RECS)
    {
        plxs_racp_response(conn_id, service_id, plxs_racp.ctrl_point.op_code,
                           PLXS_RACP_RESP_INVALID_OPERATOR);
        /*in this case when send response to client means that the procedure has end,should set plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED*/
        plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED;
        return;
    }
    uint16_t num_of_records;
    num_of_records = plxs_racp.record_db.record_num;
    plxs_racp_num_response(conn_id, service_id, num_of_records);
    /*in this case when send response to client means that the procedure has end,should set plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED*/
    plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED;
}

/**
  * @brief       plxs delete records
  *
  * @param[in] conn_id    conn_id
  * @param[in] Service id   generated by the BLE stack: @ref T_SERVER_ID.
  * @return None
  *
  */
void plxs_delete_records(uint8_t conn_id, T_SERVER_ID service_id)
{
    PROFILE_PRINT_INFO2(" plxs_delete_records opcode = %x,operator = %x",
                        plxs_racp.ctrl_point.op_code, plxs_racp.ctrl_point.op);
    if (plxs_racp.ctrl_point.op >= 7)
    {
        plxs_racp_response(conn_id, service_id, plxs_racp.ctrl_point.op_code,
                           PLXS_RACP_RESP_OPERATOR_NOT_SUPPORTED);
        /*in this case when send response to client means that the procedure has end,should set plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED*/
        plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED;
        return;
    }
    if (plxs_racp.ctrl_point.op != PLXS_RACP_OPERATOR_ALL_RECS)
    {

        plxs_racp_response(conn_id, service_id, plxs_racp.ctrl_point.op_code,
                           PLXS_RACP_RESP_INVALID_OPERATOR);
        /*in this case when send response to client means that the procedure has end,should set plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED*/
        plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED;
        return;
    }
    delete_all_records_from_database();
    plxs_racp_response(conn_id, service_id, plxs_racp.ctrl_point.op_code, PLXS_RACP_RESP_SUCCESS);
    /*in this case when send response to client means that the procedure has end,should set plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED*/
    plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED;
}

/**
  * @brief       plxs_ctr_pnt_write_ind_post_proc
  *
  * @param[in] conn_id    conn_id
  * @param[in] Service id   generated by the BLE stack: @ref T_SERVER_ID.
  * @param[in] attrib_index   attribute index
  * @param[in] write_length   write value length
  * @param[in] p_value   write value
  * @return None
  *
  */
static void plxs_ctr_pnt_write_ind_post_proc(uint8_t conn_id, T_SERVER_ID service_id,
                                             uint16_t attrib_index, uint16_t write_length, uint8_t *p_value)
{
    if (write_length > 2)
    {
        PROFILE_PRINT_ERROR1("plxs_ctr_pnt_write_ind_post_proc invalid write length, error: operand = 0x%x",
                             p_value[2]);
        plxs_racp_response(conn_id, service_id, p_value[0], PLXS_RACP_RESP_OPERAND_NOT_SUPPORTED);
        /*in this case when send response to client means that the procedure has end,should set plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED*/
        plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED;
        return;
    }
    PROFILE_PRINT_INFO4("plxs_ctr_pnt_write_ind_post_proc,opcode = 0x%x,operator = 0x%x,operand = 0x%x,write_length = 0x%x",
                        p_value[0], p_value[1], p_value[2], write_length);
    if (p_value[0] != PLXS_RACP_OPCODE_ABORT_OPERATION)
    {
        memset(&plxs_racp.ctrl_point, 0, sizeof(T_PLXS_CONTROL_POINT));
        memcpy(&plxs_racp.ctrl_point, p_value, write_length);
        plxs_racp.cp_length = write_length;
        switch (plxs_racp.ctrl_point.op_code)
        {
        case PLXS_RACP_OPCODE_REPORT_NBR_OF_RECS:
            {
                PROFILE_PRINT_INFO1("plxs_report_num_of_records operator %d", plxs_racp.ctrl_point.op);
                plxs_report_num_of_records(conn_id, service_id);
            }
            break;
        case PLXS_RACP_OPCODE_REPORT_RECS:
            {
                PROFILE_PRINT_INFO1("plxs_report_records operator %d", plxs_racp.ctrl_point.op);
                plxs_report_records(conn_id, service_id);
            }
            break;
        case PLXS_RACP_OPCODE_DELETE_RECS:
            {
                plxs_delete_records(conn_id, service_id);
            }
            break;
        default:
            {
                PROFILE_PRINT_INFO0("plxs_ctr_pnt_write_ind_post_proc PLXS_RACP_RESP_OPCODE_NOT_SUPPORTED");
                plxs_racp_response(conn_id, service_id, plxs_racp.ctrl_point.op_code,
                                   PLXS_RACP_RESP_OPCODE_NOT_SUPPORTED);
                /*in this case when send response to client means that the procedure has end,should set plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED*/
                plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED;
            }
            break;
        }
    }
    else
    {
        if (p_value[1] >= 7)
        {
            plxs_racp_response(conn_id, service_id, p_value[0], PLXS_RACP_RESP_OPERATOR_NOT_SUPPORTED);
            /*in this case if abort fail,means that procedure did't end ,and should't set plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED*/
            //plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED;
            return;
        }
        if (p_value[1] != PLXS_RACP_OPERATOR_NULL)
        {
            plxs_racp_response(conn_id, service_id, p_value[0], PLXS_RACP_RESP_INVALID_OPERATOR);
            /*in this case if abort fail,means that procedure did't end ,and should't set plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED*/
            //plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED;
            return;
        }
        if (plxs_racp.ctrl_point.op_code == PLXS_RACP_OPCODE_REPORT_RECS)
        {
            //if procedure is doing set plxs_abort_flag is true
            plxs_abort_flag = true;
        }
        else
        {
            PROFILE_PRINT_INFO0("plxs_ctr_pnt_write_ind_post_proc PLXS_RACP_OPCODE_ABORT_OPERATION SUCCESS");
            plxs_racp_response(conn_id, service_id, PLXS_RACP_OPCODE_ABORT_OPERATION, PLXS_RACP_RESP_SUCCESS);
            /*in this case when send response to client means that the procedure has end,should set plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED*/
            plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED;
        }
    }
}
#endif

/**
  * @brief  clear flags if procedure fail or disconnect
  *
  * <b>Example usage</b>
  * \code{.c}
    void app_handle_conn_state_evt(uint8_t conn_id, T_GAP_CONN_STATE new_state, uint16_t disc_cause)
    {
        APP_PRINT_INFO4("app_handle_conn_state_evt: conn_id %d old_state %d new_state %d, disc_cause 0x%x",
                        conn_id, gap_conn_state, new_state, disc_cause);
        switch (new_state)
        {
        case GAP_CONN_STATE_DISCONNECTED:
            {
                if ((disc_cause != (HCI_ERR | HCI_ERR_REMOTE_USER_TERMINATE))&& (disc_cause != (HCI_ERR | HCI_ERR_LOCAL_HOST_TERMINATE)))
                {
                    APP_PRINT_ERROR1("app_handle_conn_state_evt: connection lost cause 0x%x", disc_cause);
                }
                plxs_flags_clear();//when disconnect clear plxs flags
                le_adv_start();
            }
            break;
        }
    }
  * \endcode
  */
void plxs_flags_clear()
{
    PROFILE_PRINT_ERROR0("plxs_flags_clear");
#if PLXS_STORE_SPOT_CHECK_MEASUREMENT_SUPPORT
    /*in this case when send response to client means that the procedure has end,should set plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED*/
    plxs_racp.ctrl_point.op_code = PLXS_RACP_OPCODE_RESERVED;
    plxs_report_records_flags.plxs_send_data_end_flag = true;
    plxs_report_records_flags.plxs_current_record_to_report = 0;
    plxs_abort_flag = false;
    plxs_abort_by_app_flag = false;
    if (plxs_report_records_flags.plxs_records_to_report_offset >=
        plxs_report_records_flags.plxs_num_records_to_report)
    {
        plxs_report_records_flags.plxs_records_to_report_offset = 0;
    }
#endif
    plxs_data_send_status_flags.continuous_measurement_notify_status_flag =
        PLXS_STATUS_CONTINUOUS_MEASUREMENT_NOTIFY_IDLE;
    plxs_data_send_status_flags.report_records_indication_status_flag =
        PLXS_STATUS_REPORT_RECORDS_INDICATION_IDLE;
    plxs_data_send_status_flags.spot_check_indication_status_flag =
        PLXS_STATUS_SPOT_CHECK_MEASUREMENT_INDICATION_IDLE;
}
/**
  * @brief write characteristic data from service.
  *
  * @param[in] conn_id          Connection id.
  * @param[in] service_id       ServiceID to be written.
  * @param[in] attrib_index     Attribute index of characteristic.
  * @param[in] write_type       @ref T_WRITE_TYPE
  * @param[in] length           Length of value to be written.
  * @param[in] p_value              Value to be written.
  * @param[in] p_write_ind_post_proc @ref P_FUN_WRITE_IND_POST_PROC
  * @return Profile procedure result
  */
T_APP_RESULT plxs_attr_write_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                                T_WRITE_TYPE write_type, uint16_t length, uint8_t *p_value,
                                P_FUN_WRITE_IND_POST_PROC *p_write_ind_post_proc)
{
    T_APP_RESULT cause = APP_RESULT_SUCCESS;
    PROFILE_PRINT_INFO2("plxs_attr_write_cb: attrib_index %d, length 0x%x ", attrib_index, length);
    switch (attrib_index)
    {
#if PLXS_STORE_SPOT_CHECK_MEASUREMENT_SUPPORT
    case PLXS_CHAR_RECORS_ACCESS_CONTROL_POINT_INDEX :
        {
            /* Attribute value has variable size, make sure written value size is valid. */
            if ((length > sizeof(T_PLXS_CONTROL_POINT)) || (p_value == NULL))
            {
                PROFILE_PRINT_INFO0("plxs_attr_write_cb APP_RESULT_INVALID_VALUE_SIZE");
                cause = APP_RESULT_INVALID_VALUE_SIZE;
            }/* Make sure Control Point is not "Process already in progress". */
            else if (PLXS_RACP_OPERATION_ACTIVE(plxs_racp.ctrl_point.op_code) &&
                     (p_value[0] != PLXS_RACP_OPCODE_ABORT_OPERATION))
            {
                PROFILE_PRINT_INFO0("plxs_attr_write_cb PLXS_ERR_PROC_ALREADY_IN_PROGRESS");
                cause = (T_APP_RESULT)(ATT_ERR | PLXS_ERR_PROC_ALREADY_IN_PROGRESS);
            } /* Make sure Control Point is configured indication enable. */
            else if (!plxs_check_cccd())
            {
                PROFILE_PRINT_INFO0("plxs_attr_write_cb PLXS_ERR_CCCD_IMPROPERLY_CONFIGURED");
                cause = (T_APP_RESULT)(ATT_ERR | PLXS_ERR_CCCD_IMPROPERLY_CONFIGURED);
            }/*Process already in progress when abort, but abort operator not right*/
            else if (PLXS_RACP_OPERATION_ACTIVE(plxs_racp.ctrl_point.op_code) &&
                     (p_value[0] == PLXS_RACP_OPCODE_ABORT_OPERATION) &&
                     (p_value[1] != PLXS_RACP_OPERATOR_NULL))
            {
                PROFILE_PRINT_INFO0("plxs_attr_write_cb abort operator invalid");
                cause = (T_APP_RESULT)(ATT_ERR | ATT_ERR_INVALID_VALUE);
            }
            else/** handle RACP request after sending write response */
            {
                PROFILE_PRINT_INFO2("plxs_attr_write_cb opcode: old = %d, new = %d\n", plxs_racp.ctrl_point.op_code,
                                    p_value[0]);
                *p_write_ind_post_proc = plxs_ctr_pnt_write_ind_post_proc;
            }
        }
        break;
#endif
    default:
        {
            PROFILE_PRINT_ERROR1("plxs_attr_write_cb attribIndex %d not found", attrib_index);
            cause = APP_RESULT_ATTR_NOT_FOUND;
        }
        break;
    }
    return cause;
}
/**
  * @brief  plxs check report data send procedure to enable flow control of notification or indication
  * @param[in] conn_id  connection index
  * @param[in] Service_id  generated by the BLE stack: @ref T_SERVER_ID.
  * @param[in] attribute_index  attribute_index
  * <b>Example usage</b>
  * \code{.c}
    T_APP_RESULT app_handle_profile_message(T_SERVER_ID service_id, void *p_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        if (service_id == SERVICE_PROFILE_GENERAL_ID)
        {
            T_SERVER_APP_CB_DATA *p_para = (T_SERVER_APP_CB_DATA *)p_data;
            switch (p_para->eventId)
            {
                         ......
            case PROFILE_EVT_SEND_DATA_COMPLETE:
                         ......
                else if (p_para->event_data.send_data_result.service_id == plxs_srv_id)
                {
                    uint8_t conn_id  = p_para->event_data.send_data_result.conn_id;
                    if (p_para->event_data.send_data_result.cause == GAP_SUCCESS)
                    {
                        plxs_check_report_data_send_procedure(conn_id, plxs_srv_id,p_para->event_data.send_data_result.attrib_idx);
                    }
                    else
                    {
                        APP_PRINT_ERROR1("PROFILE_EVT_SEND_DATA_COMPLETE failed,cause %x", p_para->event_data.send_data_result.cause);
                    }
                }
                break;
            }
        }
    }
  * \endcode
  */
void plxs_check_report_data_send_procedure(uint8_t conn_id, T_SERVER_ID service_id,
                                           uint16_t attribute_index)
{
#if PLXS_CONTINUOUS_MEASUREMENT_SUPPORT
    if (attribute_index == PLXS_CHAR_CONTINUOUS_MEASUREMENT_INDEX &&
        plxs_data_send_status_flags.continuous_measurement_notify_status_flag ==
        PLXS_STATUS_CONTINUOUS_MEASUREMENT_NOTIFY_DOING)
    {
        PROFILE_PRINT_INFO0("PLXS_STATUS_CONTINUOUS_MEASUREMENT_NOTIFY_DONE");
        plxs_data_send_status_flags.continuous_measurement_notify_status_flag =
            PLXS_STATUS_CONTINUOUS_MEASUREMENT_NOTIFY_DONE;
    }
#endif
#if  PLXS_SPOT_CHECK_MEASUREMENT_SUPPORT
    if (attribute_index == PLXS_CHAR_SPOT_CHECK_MENSUREMENT_INDEX &&
        plxs_data_send_status_flags.spot_check_indication_status_flag ==
        PLXS_STATUS_SPOT_CHECK_MEASUREMENT_INDICATION_DOING)
    {
        PROFILE_PRINT_INFO0("PLXS_STATUS_SPOT_CHECK_MEASUREMENT_INDICATION_DONE");
        plxs_data_send_status_flags.spot_check_indication_status_flag =
            PLXS_STATUS_SPOT_CHECK_MEASUREMENT_INDICATION_DONE;
    }
#endif
#if PLXS_STORE_SPOT_CHECK_MEASUREMENT_SUPPORT
    if (attribute_index == PLXS_CHAR_SPOT_CHECK_MENSUREMENT_INDEX &&
        plxs_data_send_status_flags.report_records_indication_status_flag ==
        PLXS_STATUS_REPORT_RECORDS_INDICATION_DOING)
    {
        PROFILE_PRINT_INFO0("PLXS_STATUS_REPORT_RECORDS_INDICATION_DONE");
        plxs_data_send_status_flags.report_records_indication_status_flag =
            PLXS_STATUS_REPORT_RECORDS_INDICATION_DONE;
        if (plxs_check_records_report_complete(conn_id, service_id))
        {
            APP_PRINT_INFO0("records report complete or condition not allow");
        }
        else
        {
            if (false == plxs_report_records_task(conn_id, service_id))
            {
                PROFILE_PRINT_ERROR0("plxs_report_records_task send fail");
                plxs_racp_response(conn_id, service_id, PLXS_RACP_OPCODE_REPORT_RECS,
                                   PLXS_RACP_RESP_PROC_NOT_COMPLETED);
                plxs_flags_clear();
            };
        }
    }
#endif
}
/**
  * @brief       plxs_get_data_send_status
  *
  * @param[in] conn_id    conn_id
  * @param[in] Service id   generated by the BLE stack: @ref T_SERVER_ID.
  * @return plxs data send status
  * @retval data send status @ref T_PLXS_DATA_SEND_STATUS_FLAGS
  */
T_PLXS_DATA_SEND_STATUS_FLAGS plxs_get_data_send_status(uint8_t conn_id, T_SERVER_ID service_id)
{
    return plxs_data_send_status_flags;
}
/**
  * @brief       plxs_set_data_send_status
  *
  * @param[in] conn_id    conn_id
  * @param[in] Service id   generated by the BLE stack: @ref T_SERVER_ID.
  * @param[in] data_send_status @ref T_PLXS_DATA_SEND_STATUS_FLAGS
  * @return None
  */
void plxs_set_data_send_status(uint8_t conn_id, T_SERVER_ID service_id,
                               T_PLXS_DATA_SEND_STATUS_FLAGS data_send_status)
{
    plxs_data_send_status_flags = data_send_status;
}

/**
  * @brief update CCCD bits from stack.
  *
  * @param[in] conn_id          Connection id.
  * @param[in] service_id          Service ID.
  * @param[in] index          Attribute index of characteristic data.
  * @param[in] ccc_bits         CCCD bits from stack.
  * @return None
  */
void plxs_cccd_update_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t index, uint16_t ccc_bits)
{
    PROFILE_PRINT_INFO1("plxs_cccd_update_cb index = %d ", index);
    T_PLXS_CALLBACK_DATA plxs_upstream_msg;
    bool is_handled = false;
    plxs_upstream_msg.conn_id = conn_id;
    plxs_upstream_msg.msg_type = SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION;
    switch (index)
    {
#if PLXS_CONTINUOUS_MEASUREMENT_SUPPORT
    case PLXS_CHAR_CONTINUOUS_MEASUREMENT_CCCD_INDEX:
        {
            if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_NOTIFY)
            {
                plxs_notify_indicate_flag.continuous_measurement_notify_enable = 1;
                plxs_upstream_msg.msg_data.notify_indicate_index  = PLXS_CONTINUOUS_MEASUREMENT_NOTIFY_ENABLE;
                PROFILE_PRINT_INFO1("PLXS_CHAR_CONTINUOUS_MEASUREMENT_CCCD notify enable index = %d",
                                    PLXS_CHAR_CONTINUOUS_MEASUREMENT_CCCD_INDEX);
            }
            else
            {
                plxs_notify_indicate_flag.continuous_measurement_notify_enable = 0;
                plxs_upstream_msg.msg_data.notify_indicate_index  = PLXS_CONTINUOUS_MEASUREMENT_NOTIFY_DISABLE;
                PROFILE_PRINT_INFO1("PLXS_CHAR_CONTINUOUS_MEASUREMENT_CCCD notify disable index = %d",
                                    PLXS_CHAR_CONTINUOUS_MEASUREMENT_CCCD_INDEX);
            }
            is_handled =  true;
        }
        break;
#endif

#if PLXS_SPOT_CHECK_MEASUREMENT_SUPPORT
    case PLXS_CHAR_SPOT_CHECK_MENSUREMENT_CCCD_INDEX:
        {
            if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_INDICATE)
            {
                plxs_notify_indicate_flag.spot_check_measurement_indication_enable = 1;
                plxs_upstream_msg.msg_data.notify_indicate_index  = PLXS_SPOT_CHECK_MEASUREMENT_INDICATION_ENABLE;
                PROFILE_PRINT_INFO1("PLXS_CHAR_SPOT_CHECK_MENSUREMENT_CCCD indication enable index = %d",
                                    PLXS_CHAR_SPOT_CHECK_MENSUREMENT_CCCD_INDEX);
            }
            else
            {
                plxs_notify_indicate_flag.spot_check_measurement_indication_enable = 0;
                plxs_upstream_msg.msg_data.notify_indicate_index  = PLXS_SPOT_CHECK_MEASUREMENT_INDICATION_DISABLE;
                PROFILE_PRINT_INFO1("PLXS_CHAR_SPOT_CHECK_MENSUREMENT_CCCD indication disable index = %d",
                                    PLXS_CHAR_SPOT_CHECK_MENSUREMENT_CCCD_INDEX);
            }
            is_handled =  true;
        }
        break;
#endif

#if PLXS_STORE_SPOT_CHECK_MEASUREMENT_SUPPORT
    case PLXS_CHAR_RECORS_ACCESS_CONTROL_POINT_CCCD_INDEX:
        {
            if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_INDICATE)
            {
                plxs_notify_indicate_flag.racp_indication_enable = 1;
                plxs_upstream_msg.msg_data.notify_indicate_index  = PLXS_RACP_INDICATION_ENABLE;
                PROFILE_PRINT_INFO1("PLXS_CHAR_RECORS_ACCESS_CONTROL_POINT_CCCD indication enable index  = %d",
                                    PLXS_CHAR_RECORS_ACCESS_CONTROL_POINT_CCCD_INDEX);
            }
            else
            {
                plxs_notify_indicate_flag.racp_indication_enable = 0;
                plxs_upstream_msg.msg_data.notify_indicate_index  = PLXS_RACP_INDICATION_DISABLE;
                PROFILE_PRINT_INFO1("PLXS_CHAR_RECORS_ACCESS_CONTROL_POINT_CCCD indication disable index = %d",
                                    PLXS_CHAR_RECORS_ACCESS_CONTROL_POINT_CCCD_INDEX);
            }
            is_handled =  true;
        }
        break;
#endif
    default:
        {
            PROFILE_PRINT_ERROR1("plxs_cccd_update_cb index = %d not found", index);
        }
        break;
    }
    /* Notify Application. */
    if (pfn_plxs_cb && (is_handled == true))
    {
        pfn_plxs_cb(service_id, (void *)&plxs_upstream_msg);
    }
}
/**
  * @brief plx Profile Service Callbacks.
  */
const  T_FUN_GATT_SERVICE_CBS plxs_cbs =
{
    plxs_attr_read_cb,
    plxs_attr_write_cb,
    plxs_cccd_update_cb
};

/**
  * @brief  Add PLX service to the BLE stack database.
  *
  * @param[in] p_func  Callback when service attribute was read, write or cccd update.
  * @return Service id generated by the BLE stack: @ref T_SERVER_ID.
  * @retval 0xFF Operation failure.
  * @retval Others Service id assigned by stack.
  *
  * <b>Example usage</b>
  * \code{.c}
     void app_le_profile_init()
     {
         server_init(1);
         plxs_srv_id = plxs_add_service(app_handle_profile_message);
     }
  * \endcode
  */
T_SERVER_ID plxs_add_service(void *p_func)
{
    plxs_flags_init();
    T_SERVER_ID service_id = 0xff;
#if (PLXS_SPOT_CHECK_MEASUREMENT_SUPPORT || PLXS_CONTINUOUS_MEASUREMENT_SUPPORT)
    {
        if (false == server_add_service(&service_id,
                                        (uint8_t *)plxs_att_tbl,
                                        sizeof(plxs_att_tbl),
                                        plxs_cbs))
        {
            PROFILE_PRINT_ERROR1("plxs_add_service:service_id %d", service_id);
            service_id = 0xff;
            return service_id;
        }
#if PLXS_STORE_SPOT_CHECK_MEASUREMENT_SUPPORT
        plxs_racp.record_db.record_num = 0;
        plxs_racp.record_db.head = 0;
        plxs_racp.record_db.tail = 0;
        plxs_racp.cp_length = 0;
#endif
        pfn_plxs_cb = (P_FUN_SERVER_GENERAL_CB)p_func;
        return service_id;
    }
#else
    {
        PROFILE_PRINT_ERROR0("SPOT_CHECK_MEASUREMENT and CONTINUOUS_MEASUREMENT Mandatory to support at least one ");
        service_id = 0xff;
        return service_id;
    }
#endif
}
