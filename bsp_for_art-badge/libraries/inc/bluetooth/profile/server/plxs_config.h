/**
*****************************************************************************************
*     Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     plxs_config.h
  * @brief    Pulse Oximeter Service configuration file.
  * @details  Configure the optional characteristic and fields in the Pulse Oximeter Service.
  * @author   danni
  * @date     2018-12-27
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion **/
#ifndef _PLXS_CONFIG_H_
#define _PLXS_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup PLXS Pulse Oximeter Service
* @brief Pulse Oximeter Service
* @{
*/

/** @defgroup PLXS_Service_CONFIG PLXS Service Config
* @brief PLXS service configuration file
* @{
*/

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup PLXS_Common_Exported_Macros PLXS Common Exported Macros
  * @{
  */
#define PLXS_SPOT_CHECK_MEASUREMENT_SUPPORT                                                            1 /**< if this value set to 1 means PLXS_SPOT_CHECK_MEASUREMENT_SUPPORT*/
#define PLXS_STORE_SPOT_CHECK_MEASUREMENT_SUPPORT                                                      (PLXS_SPOT_CHECK_MEASUREMENT_SUPPORT && 1) /**< if PLXS_STORE_SPOT_CHECK_MEASUREMENT_SUPPORT is set to 1 then PLXS_SPOT_CHECK_MEASUREMENT_SUPPORT and PLXS_SPOT_CHECK_MEASUREMENT_TIMESTAMP_SUPPORT must be set to 1*/
#define PLXS_SPOT_CHECK_MEASUREMENT_TIMESTAMP_SUPPORT                                                  (PLXS_SPOT_CHECK_MEASUREMENT_SUPPORT && 1) /**< flags to show timestamp field support or not*/
#define PLXS_DEVICE_CLOCK_NOT_SET                                                                      (PLXS_SPOT_CHECK_MEASUREMENT_SUPPORT && 1) /**< flags to show clock set or not*/

#define PLXS_MEASUREMENT_STATE_SUPPORT                                                                 1 /**< flags to show measurement status field support or not*/
#define PLXS_DEVICE_AND_SENSOR_STATUS_SUPPORT                                                          1 /**< flags to show device and sensor status field support or not*/
#define PLXS_PULSE_AMPLITUDE_INDEX_SUPPORT                                                             1 /**< flags to show pulse amplitude index field support or not*/
#define PLXS_MULTIPLE_BONDS_SUPPORT                                                                    0 /**< flags to show multiple bonds field support or not*/

#define PLXS_CONTINUOUS_MEASUREMENT_SUPPORT                                                            1 /**< if this value set to 1 means PLXS_CONTINUOUS_MEASUREMENT_SUPPORT*/
#define PLXS_CONTINUOUS_MEASUREMENT_SPO2PR_FAST_SUPPORT                                                (PLXS_CONTINUOUS_MEASUREMENT_SUPPORT && 1)/**< flags to show spo2pr fast measurement field support or not*/
#define PLXS_CONTINUOUS_MEASUREMENT_SPO2PR_SLOW_SUPPORT                                                (PLXS_CONTINUOUS_MEASUREMENT_SUPPORT && 1)/**< flags to show spo2pr slow measurement field support or not*/

#define PLXS_RACP_MAX_NBR_OF_STORED_RECS                                                               30 /**< the maximun records to be stored*/

/** @brief The Measurement Status Support field*/
#define PLXS_FEATURES_MEASUREMENT_STATUS_ONGOING_SUPPORT                                               0x0020 /**< Measurement Ongoing bit supported*/
#define PLXS_FEATURES_MEASUREMENT_STATUS_EARLY_ESTIMATED_DATA_SUPPORT                                  0x0040 /**< Early Estimated Data bit supported*/
#define PLXS_FEATURES_MEASUREMENT_STATUS_VALIDATED_DATA_SUPPORT                                        0x0080 /**< Validated Data bit supported*/
#define PLXS_FEATURES_MEASUREMENT_STATUS_FULLY_QUALIFIED_DATA_SUPPORT                                  0x0100 /**< Fully Qualified Data bit supported*/
#define PLXS_FEATURES_MEASUREMENT_STATUS_DATA_FROM_MEASUREMENT_STORAGE_SUPPORT                         0x0200 /**< Data from Measurement Storage bit supported*/
#define PLXS_FEATURES_MEASUREMENT_STATUS_DATA_FOR_DEMONSTRATION_SUPPORT                                0x0400 /**< Data for Demonstration bit supported*/
#define PLXS_FEATURES_MEASUREMENT_STATUS_DATA_FOR_TEST_SUPPORT                                         0x0800 /**< Data for Testing bit supported*/
#define PLXS_FEATURES_MEASUREMENT_STATUS_CALIBRATION_ONGOING_SUPPORT                                   0x1000 /**< Calibration Ongoing bit supported*/
#define PLXS_FEATURES_MEASUREMENT_STATUS_MEASUREMENT_UNAVALIABLE_SUPPORT                               0x2000 /**< Measurement Unavailable bit supported*/
#define PLXS_FEATURES_MEASUREMENT_STATUS_QUESTIONABLE_MEASUREMENT_DETECTED_SUPPORT                     0x4000 /**< Questionable Measurement Detected bit supported*/
#define PLXS_FEATURES_MEASUREMENT_STATUS_INVALID_MEASUREMENT_DETECTED_SUPPORT                          0x8000 /**< Invalid Measurement Detected bit supported*/

/** @brief define plx service features which measurement status are supported. this macro according user needs to add,
  *        for example:
  *        #define PLXS_FEATURES_MEASUREMENT_STATUS_SUPPORTS  PLXS_FEATURES_MEASUREMENT_STATUS_ONGOING_SUPPORT|PLXS_FEATURES_MEASUREMENT_STATUS_FULLY_QUALIFIED_DATA_SUPPORT
  *        means both support PLXS_FEATURES_MEASUREMENT_STATUS_ONGOING_SUPPORT and PLXS_FEATURES_MEASUREMENT_STATUS_FULLY_QUALIFIED_DATA_SUPPORT,if you want add other supports,
  *        please @ref The Measurement Status Support field above
  */
#define PLXS_FEATURES_MEASUREMENT_STATUS_SUPPORTS                                                     (PLXS_FEATURES_MEASUREMENT_STATUS_ONGOING_SUPPORT | PLXS_FEATURES_MEASUREMENT_STATUS_FULLY_QUALIFIED_DATA_SUPPORT)

/** @brief Device and Sensor Status Support field*/
#define PLXS_FEATURES_DEVICE_SENSOR_STATUS_EXTENDED_DISPLAY_UPDATE_ONGOING_SUPPORT                    0x000001 /**< Extended Display Update Ongoing bit supported*/
#define PLXS_FEATURES_DEVICE_SENSOR_STATUS_EQUIPMENT_MALFUNCTION_DETECTED_SUPPORT                     0x000002 /**< Equipment Malfunction Detected bit supported*/
#define PLXS_FEATURES_DEVICE_SENSOR_STATUS_SIGNAL_PROCESSING_IRREGULARITY_DETECTED_SUPPORT            0x000004 /**< Signal Processing Irregularity Detected bit supported*/
#define PLXS_FEATURES_DEVICE_SENSOR_STATUS_INADEQUATE_SIGNAL_DETECTED_SUPPORT                         0x000008 /**< Inadequate Signal Detected bit supported*/
#define PLXS_FEATURES_DEVICE_SENSOR_STATUS_POOR_SIGNAL_DETECTED_SUPPORT                               0x000010 /**< Poor Signal Detected bit supported*/
#define PLXS_FEATURES_DEVICE_SENSOR_STATUS_LOW_PERFUSION_DETECTED_SUPPORT                             0x000020 /**< Low Perfusion Detected bit supported*/
#define PLXS_FEATURES_DEVICE_SENSOR_STATUS_ERRATIC_SIGNAL_DETECTED_SUPPORT                            0x000040 /**< Erratic Signal Detected bit supported*/
#define PLXS_FEATURES_DEVICE_SENSOR_STATUS_NONPULSATILE_SIGNAL_DETECTED_SUPPORT                       0x000080 /**< Nonpulsatile Signal Detected bit supported*/
#define PLXS_FEATURES_DEVICE_SENSOR_STATUS_QUESTIONABLE_PULSE_DETECTED_SUPPORT                        0x000100 /**< Questionable Pulse Detected bit supported*/
#define PLXS_FEATURES_DEVICE_SENSOR_STATUS_SIGNAL_ANALYSIS_ONGOING_SUPPORT                            0x000200 /**< Signal Analysis Ongoing bit supported*/
#define PLXS_FEATURES_DEVICE_SENSOR_STATUS_SENSOR_INTERFERENCE_DETECTED_SUPPORT                       0x000400 /**< Sensor Interference Detected bit supported*/
#define PLXS_FEATURES_DEVICE_SENSOR_STATUS_SENSOR_UNCONNECTED_USER_SUPPORT                            0x000800 /**< Sensor Unconnected to User bit supported*/
#define PLXS_FEATURES_DEVICE_SENSOR_STATUS_UNKNOWN_SENSOR_CONNECTED_SUPPORT                           0x001000 /**< Unknown Sensor Connected bit supported*/
#define PLXS_FEATURES_DEVICE_SENSOR_STATUS_SENSOR_DISPLACED_SUPPORT                                   0x002000 /**< Sensor Displaced bit supported*/
#define PLXS_FEATURES_DEVICE_SENSOR_STATUS_SENSOR_MALFUNCTIONING_SUPPORT                              0x004000 /**< Sensor Malfunctioning bit supported*/
#define PLXS_FEATURES_DEVICE_SENSOR_STATUS_SENSOR_DISCONNECTED_SUPPORT                                0x008000 /**< Sensor Disconnected bit supported*/

/** @brief define plx service features which device and sensor status are supported.according user needs to add,
  *        for example:
  *        #define PLXS_FEATURES_DEVICE_SENSOR_STATUS_SUPPORTS  PLXS_FEATURES_DEVICE_SENSOR_STATUS_EXTENDED_DISPLAY_UPDATE_ONGOING_SUPPORT|PLXS_FEATURES_DEVICE_SENSOR_STATUS_SENSOR_DISCONNECTED_SUPPORT
  *        means both support PLXS_FEATURES_DEVICE_SENSOR_STATUS_EXTENDED_DISPLAY_UPDATE_ONGOING_SUPPORT and PLXS_FEATURES_DEVICE_SENSOR_STATUS_SENSOR_DISCONNECTED_SUPPORT,if you want add other supports,
  *        please @ref Device and Sensor Status Support field above
  */
#define PLXS_FEATURES_DEVICE_SENSOR_STATUS_SUPPORTS                                                  (PLXS_FEATURES_DEVICE_SENSOR_STATUS_EXTENDED_DISPLAY_UPDATE_ONGOING_SUPPORT | PLXS_FEATURES_DEVICE_SENSOR_STATUS_SENSOR_DISCONNECTED_SUPPORT)

/** @defgroup PLXS_Service_ATTI_INDEX PLXS Service attribute index
* @brief PLXS Service attribute index
* @{
*/
#if PLXS_SPOT_CHECK_MEASUREMENT_SUPPORT
#define PLXS_CHAR_SPOT_CHECK_MENSUREMENT_INDEX                                                       0x02
#define PLXS_CHAR_SPOT_CHECK_MENSUREMENT_CCCD_INDEX                                                  0x03
#endif

#if PLXS_CONTINUOUS_MEASUREMENT_SUPPORT
#define PLXS_CHAR_CONTINUOUS_MEASUREMENT_INDEX                                                      ((PLXS_SPOT_CHECK_MEASUREMENT_SUPPORT*3) + 2 )//0x05
#define PLXS_CHAR_CONTINUOUS_MEASUREMENT_CCCD_INDEX                                                 ((PLXS_SPOT_CHECK_MEASUREMENT_SUPPORT*3) + 3 )//0x06
#endif

#define PLXS_CHAR_FEATURE_INDEX                                                                     ((PLXS_SPOT_CHECK_MEASUREMENT_SUPPORT*3) + (PLXS_CONTINUOUS_MEASUREMENT_SUPPORT*3)+ 2)//0x05//0x08

#if PLXS_STORE_SPOT_CHECK_MEASUREMENT_SUPPORT
#define PLXS_CHAR_RECORS_ACCESS_CONTROL_POINT_INDEX                                                 ((PLXS_SPOT_CHECK_MEASUREMENT_SUPPORT*3) + (PLXS_CONTINUOUS_MEASUREMENT_SUPPORT*3)+ 4)//0x07//0x0A
#define PLXS_CHAR_RECORS_ACCESS_CONTROL_POINT_CCCD_INDEX                                            ((PLXS_SPOT_CHECK_MEASUREMENT_SUPPORT*3) + (PLXS_CONTINUOUS_MEASUREMENT_SUPPORT*3)+ 5)//0x08//0x0B
#endif
/** @} End of PLXS_Service_ATTI_INDEX */
/** @} End of PLXS_Common_Exported_Macros */
/** @} End of PLXS_Service_CONFIG */
/** @} End of PLXS */
#ifdef __cplusplus
}
#endif
#endif
