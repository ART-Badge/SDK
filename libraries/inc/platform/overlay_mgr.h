/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    overlay_mgr.h
  * @brief
  * @author
  * @date
  * @version
  ***************************************************************************************
    * @attention
  * <h2><center>&copy; COPYRIGHT 2015 Realtek Semiconductor Corporation</center></h2>
  ***************************************************************************************
  */

#ifndef _OVERLAY_MGR_H_
#define _OVERLAY_MGR_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup OVERLAY_MANAGER    Overlay Manager
  * @{
  */

/*============================================================================*
  *                                   Types
  *============================================================================*/
/** @defgroup OVERLAY_MANAGER_Exported_Types Overlay Manager Exported Types
  * @{
  */
typedef enum
{
    OVERLAY_SCENARIO_BOOT_ONCE = 0,
    OVERLAY_SCENARIO_B,
    OVERLAY_SCENARIO_C,
    OVERLAY_SCENARIO_NUM,
} T_OVERLAY_SCENARIO_IDX;

typedef struct
{
    char *signature;

    void *load_ro_base;
    void *load_rw_base;

    void *image_ro_base;
    void *image_rw_base;
    void *image_zi_base;

    uint32_t ro_length;
    uint32_t rw_length;
    uint32_t zi_length;
} T_OVERLAY_SECTION;
/** End of OVERLAY_MANAGER_Exported_Types
  * @}
  */

/*============================================================================*
  *                                Functions
  *============================================================================*/
/** @defgroup OVERLAY_MANAGER_Exported_Functions Overlay Manager Exported Functions
    * @brief
    * @{
    */
/**
 * @brief  Load Code(RO), RW Data and ZI Data of a given overlay section.
 * @param  scenario_idx Overlay section index, support only three sections now.
 * @return None
 */
bool load_overlay(T_OVERLAY_SCENARIO_IDX scenario_idx);

/**
 * @brief  return the current scenario index
 * @param  none.
 * @return  @ref T_OVERLAY_SCENARIO_IDX
 * @retval OVERLAY_SCENARIO_NUM, invalid scenario index
 * @retval other T_OVERLAY_SCENARIO_IDX value, valid secnario index
 */
T_OVERLAY_SCENARIO_IDX get_current_scenario_index(void);

/** @} */ /* End of group OVERLAY_MANAGER_Exported_Functions */


/** @} */ /* End of group OVERLAY_MANAGER */

#ifdef __cplusplus
}
#endif

#endif /* _OVERLAY_MGR_H_ */
