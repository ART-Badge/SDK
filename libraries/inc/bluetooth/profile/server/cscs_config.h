#ifndef _CSCS_CONFIG_H_
#define _CSCS_CONFIG_H_



/** @defgroup CSCS Cycling Speed and Cadence Service
  * @brief Cycling Speed and Cadence Service
  * @{
  */

/** @defgroup CSCS_CONFIG Cycling Speed and Cadence Service Config
  * @brief Cycling Speed and Cadence service configuration file
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup CSCS_Common_Exported_Macros CSCS Common Exported Macros
  * @{
  */

#define CSCS_MULTIPLE_SENSOR_LOCATIONS_SUPPORT
#define CSCS_SC_CONTROL_POINT_SUPPORT

/** @brief  Index of each characteristic in CSC service database. */
#define GATT_SVC_CSCS_MEASUREMENT_INDEX      2
#define GATT_SVC_CSCS_FEATURE_INDEX          5
#define GATT_SVC_CSCS_SENS_LOC_INDEX         7
#define GATT_SVC_CSCS_CTL_PNT_INDEX          9
#define GATT_SVC_CSCS_MEAS_CCCD_INDEX        (GATT_SVC_CSCS_MEASUREMENT_INDEX + 1)
#define GATT_SVC_CSCS_CTL_PNT_CCCD_INDEX     (GATT_SVC_CSCS_CTL_PNT_INDEX + 1)

/** @} End of CSCS_Common_Exported_Macros */

/** @} End of CSCS_CONFIG */

/** @} End of CSCS */

#endif
