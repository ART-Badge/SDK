/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    dlps.h
  * @brief   DLPS implementation head file.
  * @author  lory_xu
  * @date    2014-08-05
  * @version v1.0
  * *************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   * *************************************************************************************
  */

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef __DLPS_H
#define __DLPS_H


/*============================================================================*
 *                               Header Files
*============================================================================*/
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif




/** @defgroup DLPS_PLATFORM DLPS Platform
  * @brief Deep low power state support
  * @{
  */

/*============================================================================*
 *                              Types
*============================================================================*/
/** @defgroup DLPS_PLATFORM_Exported_Types DLPS Platform Exported Types
  * @{
  */

/** @brief This CB is used for any module which needs to be checked before entering DLPS */
typedef bool (*DLPSEnterCheckFunc)();

/** @brief This CB is used for any module which needs to control the hw before entering or after exiting from DLPS */
typedef void (*DLPSHWControlFunc)();

/** @brief DLPSState struct */
typedef enum
{
    DLPS_ENTER           = 0, /**< note for all module */
    DLPS_EXIT1_CPU_READY = 1, /**< note for some special module, like wdg and swd */
    DLPS_EXIT2_OS_READY  = 2, /**< note for lower stack only */
    DLPS_EXIT3_BT_ISR    = 3, /**< note for all module at BT ISR */
    DLPS_EXIT4_BT_READY  = 4  /**< note for all module after BT is ready */
} DLPSState;

/** @brief LPSMode struct */
typedef enum
{
    LPM_POWERDOWN_MODE   = 0,   /**< Power down */
    LPM_HIBERNATE_MODE   = 1,   /**< Hibernate  */
    LPM_DLPS_MODE        = 2,   /**< DLPS       */
    LPM_LPS_MODE         = 3,   /**< LPS        */
    LPM_ACTIVE_MODE      = 4,   /**< Active     */
    LPM_DEF_MAX          = 5
} LPSMode;

/** @brief DLPS error code*/
typedef enum
{
    DLPS_ERROR_UNKNOWN                      = 0x0,
    DLPS_ERROR_NOT_EMPTY_QUEUE              = 0x1,
    DLPS_ERROR_NOT_ENOUGH_MEMORY            = 0x2,
    DLPS_ERROR_CONTROLLER_TO_HOST_BUSY      = 0x3,
    DLPS_ERROR_DISABLE_DLPS                 = 0x4,
    DLPS_ERROR_LPM_ACTIVE_MODE              = 0x5,
    DLPS_ERROR_GREAT_THEN_LPM_ACTIVE_MODE   = 0x6,
    DLPS_ERROR_NOT_LPM_ACTIVE_MODE          = 0x7,
    DLPS_ERROR_NOT_EMPTY_QUEUE_OF_LOWER     = 0x8,
    DLPS_ERROR_NOT_EMPTY_H2C_COUNT          = 0x9,
    DLPS_ERROR_TX_BUSY                      = 0xA,
    DLPS_ERROR_CHANNEL_MAP_UPDATE           = 0xB,
    DLPS_ERROR_CONNECTION_UPDATE            = 0xC,
    DLPS_ERROE_BQB                          = 0xD,
    DLPS_ERROR_NOT_EMPTY_TIMER_COUNT        = 0xE,
    DLPS_ERROR_LE_LPS_DISABLE               = 0xF,
    DLPS_ERROR_INITIATOR_UNIT_ENABLE        = 0x10,
    DLPS_ERROR_ADV_UNIT_ENABLE              = 0x11,
    DLPS_ERROR_SCAN_UNIT_ENABLE             = 0x12,
    DLPS_ERROR_MUTI_LINK_COUNTS             = 0x13,
    DLPS_ERROR_PEND_TERMINATE_IND           = 0x14,
    DLPS_ERROR_CE_INTERVAL_TOO_SMALL        = 0x15,
    DLPS_ERROR_CONN_COUNTER_LESS_TEHN_7     = 0x16,
    DLPS_ERROR_LPS_MODE_STATUS_NOT_IDLE     = 0x17,
    DLPS_ERROR_PERIOD_ADV_DISABLE           = 0x18,
    DLPS_ERROR_PERIOD_SCAN_DISABLE          = 0x19,
    DLPS_ERROR_ADV_STATE_NOT_IDLE           = 0x1A,
    DLPS_ERROR_ADV_NEXT_INSTANCE_TOO_SMALL  = 0x1B,
    DLPS_ERROR_SCAN_STATE_NOT_IDLE          = 0x1C,
    DLPS_ERROR_SCAN_NEXT_INSTANCE_TOO_SMALL = 0x1D,
    DLPS_ERROR_CONN_STATE_NOT_IDLE          = 0x1E,
    DLPS_ERROR_PERIOD_CONN_DISABLE          = 0x1F,
    DLPS_ERROR_SCAN_WK_INSTANCE_TOO_SMALL   = 0x20,
    DLPS_ERROR_LE_REG_S_INST                = 0x21,
    DLPS_ERROR_SCHEDULE_LOWER_STACK         = 0x22,
    DLPS_ERROR_POWER_SAVE_DISABLE           = 0x23,
    DLPS_ERROR_BB_STATE_UNNORMAL            = 0x24,
    DLPS_ERROR_WK_INSTANCE_TOO_SMALL        = 0x25,
    DLPS_ERROR_ROLE_SWITCH                  = 0x26,
    DLPS_ERROR_LEGACY_NOT_IDLE              = 0x27,
    DLPS_ERROR_LE_SCHE_NOT_READY            = 0x28,
    DLPS_ERROR_CSB_ENABLE                   = 0x29,
    DLPS_ERROR_CAL_NOT_READY                = 0x2A,
    DLPS_ERROR_PSD                          = 0x2B,
    DLPS_ERROR_DISABLE_DLPS_TIME            = 0x2C,
    DLPS_ERROR_PHY_UPDATE                   = 0x2D,
    DLPS_ERROR_APP                          = 0xF0,
} DLPSErrorCode;

/** @} */ /* End of group DLPS_PLATFORM_Exported_Types */

/*============================================================================*
 *                              Variables
*============================================================================*/
/** @defgroup DLPS_PLATFORM_Exported_Variables DLPS Platform Exported Variables
  * @{
  */

/** @brief dlps refuse reason(function) */
extern uint32_t *DlpsRefuseReason;
/** @brief dlps error code */
extern DLPSErrorCode DlpsErrorCode;

/** @} */ /* End of group DLPS_PLATFORM_Exported_Variables */

/*============================================================================*
 *                              Functions
*============================================================================*/

/** @defgroup DLPS_PLATFORM_Exported_Functions DLPS Platform Exported Functions
  * @{
  */

/**
 * @brief Register Check CB to DlpsPlatform which will call it before entering Dlps.
 * @param  func DLPSEnterCheckFunc
 * @return  Status of Operation.
 * @retval true success
 * @retval false fail
*/
extern bool dlps_check_cb_reg(DLPSEnterCheckFunc func);

/**
 * @brief Register HW Control CB to DlpsPlatform which will call it before entering Dlps or after exiting from Dlps (according to dlpsState) .
 * @param  func DLPSHWControlFunc
 * @param  dlpsState tell the DlpsPlatform the CB should be called when DLPS_ENTER or DLPS_EXITx_XXX.
 * @return  Status of Operation.
 * @retval true success
 * @retval false fail
*/
extern bool dlps_hw_control_cb_reg(DLPSHWControlFunc func, DLPSState dlpsState);

/**
 * @brief Unregister HW Control CB to DlpsPlatform.
 * @param  func DLPSHWControlFunc
 * @return  none
*/
extern void dlps_hw_control_cb_unreg(DLPSHWControlFunc func);

/**
 * @brief Keep platform in @ref LPM_ACTIVE_MODE which means will stop platform enter any lower power mode.
 *        stack size is 63
 * @param  none
 * @return  none
*/
extern void lps_mode_pause(void);

/**
 * @brief Restore to original LPSMode.
 * @param  none
 * @return  none
*/
extern void lps_mode_resume(void);

/**
 * @brief  LPSMode Set .
 * @param  mode LPSMode
 * @return  none
*/
extern void lps_mode_set(LPSMode mode);

/**
 * @brief  LPSMode Get .
 * @param  none
 * @return  @ref LPSMode
*/
extern LPSMode lps_mode_get(void);

/**
 * @brief  Return Pause LPSMode stack, only 0 can enter any low power mode.
 * @param  none
 * @return  stack num
*/
extern int8_t lps_mode_stack_get(void);

/**
 * @brief  Return dlps wakeup counts .
 * @param  none
 * @return  count value
*/
extern uint32_t lps_wakeup_count_get(void);

/**
 * @brief  Return dlps remain time.
 * @param  none
 * @return  dlps remain time with unit of us
*/
extern uint32_t last_lps_remain_us_get(void);


/** @} */ /* End of group DLPS_PLATFORM_Exported_Functions */

/** @} */ /* End of group DLPS_PLATFORM */


#ifdef __cplusplus
}
#endif

#endif  /* __DLPS_PLATFORM_H */
