#ifndef _BLS_DEFINE_H_
#define _BLS_DEFINE_H_


/** @defgroup Blood Pressure Service define file
  * @brief Blood pressure service define file
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup BLS_Common_Exported_Macros BLS Common Exported Macros
  * @{
  */

/** @details
   Set BLS_INTERMEDIATE_CUFF_PRESSURE_SUPPORT to 1 to support intermediate cuff pressure characteristic,
   otherwise set it to 0.
*/
#define BLS_INTERMEDIATE_CUFF_PRESSURE_SUPPORT    1

/** @brief service related UUIDs. */

#define GATT_UUID_BLOOD_PRESSURE                        0x1810
#define GATT_UUID_CHAR_BLP_MEASUREMENT                  0x2A35
#define GATT_UUID_CHAR_INTERMEDIATE_CUFF_PRESSURE       0x2A36
#define GATT_UUID_CHAR_BLP_FEATURE                      0x2A49


#define BLS_MEASUREMENT_VALUE_MAX_LEN  25

#define BLS_MEASUREMENT_COMPOUND_VALUE_NAN  0x07ff
#define BLS_MEASUREMENT_USER_ID_UNKNOW      0xff

#define BLS_MEASUREMENT_VALUE_UNITS_MMHG      0
#define BLS_MEASUREMENT_VALUE_UNITS_KPA       1

#define BLS_FLAG_MEASUREMENT_UINT_BIT                       0x01
#define BLS_FLAG_MEASUREMENT_TIMESTAMP_PRESENT_BIT          0x02
#define BLS_FLAG_MEASUREMENT_PULSE_RATE_BIT                 0x04
#define BLS_FLAG_MEASUREMENT_USER_ID_BIT                    0x08
#define BLS_FLAG_MEASUREMENT_STATUS_BIT                     0x10

#define BLS_FEATURE_BODY_MOVEMENT_DETECTION_SUPPORT_BIT           0x01
#define BLS_FEATURE_CUFF_FIT_DETECTION_SUPPORT_BIT                0x02
#define BLS_FEATURE_IRREGULAR_PULSE_DETECTION_SUPPORT_BIT         0x04
#define BLS_FEATURE_PULSE_RATE_RANGE_DETECTION_SUPPORT_BIT        0x08
#define BLS_FEATURE_MEASUREMENT_POSITION_DETECTION_SUPPORT_BIT    0x10
#define BLS_FEATURE_MULTIPLE_BOND_SUPPORT_BIT                     0x20

typedef uint8_t     SFLOAT[2];         /* 4 bit (MSB) exponent, 12 bit mantissa */

typedef struct
{
    SFLOAT bp_meas_systolic_value;
    SFLOAT bp_meas_diastolic_value;
    SFLOAT bp_meas_map_value;
} T_BLOOD_PRESSUREE_MEAS_COMPOUND_VALUE;

typedef struct
{
    uint16_t year;
    uint8_t  month;
    uint8_t  day;
    uint8_t  hours;
    uint8_t  minutes;
    uint8_t  seconds;
} T_BLS_TIME_STAMP;

typedef struct
{
    uint16_t bp_meas_body_movement_detection_flag: 1;
    uint16_t bp_meas_cuff_fit_detection_flag: 1;
    uint16_t bp_meas_irregular_pulse_detection_flag: 1;
    uint16_t bp_meas_pulse_rate_range_detection_flag: 2;
    uint16_t bp_meas_measurement_position_detection_flag: 1;
    uint16_t rfu: 10;
} T_BLOOD_PRESSUREE_MEAS_STATUS;

typedef struct
{
    uint8_t bp_meas_flag;
    T_BLOOD_PRESSUREE_MEAS_COMPOUND_VALUE bp_meas_compound_value;
    T_BLS_TIME_STAMP time_stamp;
    SFLOAT bp_meas_pulse_rate;
    uint8_t bp_meas_user_id;
    T_BLOOD_PRESSUREE_MEAS_STATUS bp_meas_status;
} T_BLOOD_PRESSURE_MEASURMENT;

/** @} End of BLS_Common_Exported_Macros */

/** @} End of BLS_DEFINE */

#endif
