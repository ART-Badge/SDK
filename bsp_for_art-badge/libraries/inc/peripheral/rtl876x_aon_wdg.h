/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      rtl876x_aon_wdg.h
* @brief     header file of aon watch dog driver.
* @details
* @author    Serval Li
* @date      2017-06-27
* @version   v0.1
* *********************************************************************************************************
*/

#ifndef _RTL876X_AON_WDG_H_
#define _RTL876X_AON_WDG_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup AON_WATCH_DOG AON_WATCH_DOG
  * @brief AON Watch Dog driver module
  * @{
  */

/** @defgroup AON_WATCH_DOG_Exported_Functions AON Watch Dog Exported Functions
  * @{
  */

/**
   * @brief  AON Watchdog Config.
   * @param  reset_level: 0, Reset whole chip; 1, Reset whole chip except partial AON and RTC.
   * @param  comp: Set period in ms.
   * @param  cnt_ctl: 0, Stop count in low power mode; 1, Continue count in low power mode.
   * @param  cnt_reload: 0, Not reload counter when exit low power mode
                         1, Reload counter when exit low power mode.
   * @retval none.
   */
void AON_WDG_Config(uint8_t reset_level, uint32_t comp, uint8_t cnt_ctl, uint8_t cnt_reload);

/**
   * @brief  Config Reset Level.
   * @param  reset_level: 0, Reset whole chip; 1, Reset whole chip except partial AON and RTC.
   * @retval none.
   */
void AON_WDG_ConfigResetLevel(uint8_t reset_level);

/**
   * @brief  Config Period.
   * @param  comp: Set period in ms.
   * @retval none.
   */
void AON_WDG_ConfigComp(uint32_t comp);

/**
   * @brief  Config wheather continue count in low power mode or not.
   * @param  cnt_ctl: 0, Stop count in low power mode; 1, Continue count in low power mode.
   * @retval none.
   */
void AON_WDG_ConfigCntCtl(uint8_t cnt_ctl);

/**
   * @brief  Config wheather reload counter when exit low power mode or not.
   * @param  cnt_reload: 0, Not reload counter when exit low power mode
                         1, Reload counter when exit low power mode.
   * @retval none.
   */
void AON_WDG_ConfigCntReload(uint8_t cnt_reload);

/**
   * @brief  AON Watch Dog Timer Enable.
   */
void AON_WDG_Enable(void);

/**
   * @brief  AON Watch Dog Timer Disable.
   */
void AON_WDG_Disable(void);

/**
   * @brief  AON Watch Dog Timer Restart.
   */
void AON_WDG_Restart(void);

/**
   * @brief  AON Watch Dog System Reset.
   */
void AON_WDG_SystemReset(void);

/**
   * @brief  AON Watch Dog init.
   * reset_level: 0, Reset whole chip; 1, Reset whole chip except partial AON and RTC.
   * timeout_second: timeout period in seconds, max value is 65s
   */
static inline void aon_wdg_init(uint8_t reset_level, uint8_t timeout_second)
{
    uint32_t comp = (timeout_second * 1000 * 4) & 0x3FFFF;  //only 18 bit
    AON_WDG_Config(reset_level, comp, 1, 1);
}

/**
   * @brief  AON Watch Dog enable.
   */
static inline void aon_wdg_enable(void)
{
    AON_WDG_Enable();
}

/**
   * @brief  AON Watch Dog counter is clear and disable.
   */
static inline void aon_wdg_disable(void)
{
    AON_WDG_Restart();
    AON_WDG_Disable();
}


/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* _RTL876X_AON_WDG_H_ */
