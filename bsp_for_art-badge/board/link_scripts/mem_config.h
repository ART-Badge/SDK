/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    mem_config.h
  * @brief   Memory Configuration
  * @date    2017.6.6
  * @version v1.0
  * *************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   * *************************************************************************************
  */

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef MEM_CONFIG_H
#define MEM_CONFIG_H

#include "flash_map.h"

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*
 *                        Code configuration
 *============================================================================*/
/** @brief set app bank to support OTA: 1 is ota bank1, 0 is ota bank0 */
#define APP_BANK                                  0

/** @brief ram code configuration: 1 is ram code, 0 is flash code */
#define FEATURE_RAM_CODE                          0

/** @brief encrypt app or not */
#define FEATURE_ENCRYPTION                        0


/*============================================================================*
 *                        data ram layout configuration
 *============================================================================*/
/* Data RAM layout:                   144K
example:
   1) reserved for rom:                13K (fixed)
   2) patch ram code:                  16K (fixed)
   3) upperstack global + ram code:    4K  (adjustable, config UPPERSTACK_GLOBAL_SIZE, must consponding to used upperstack image)
   4) app global + ram code:           95K (adjustable, config APP_GLOBAL_SIZE)
   5) Heap ON:                         16K (adjustable, defined by UPPERSTACK_GLOBAL_SIZE and APP_GLOBAL_SIZE)
*/

/** @brief data ram size for upperstack global variables and code */
#define UPPERSTACK_GLOBAL_SIZE         (4 * 1024)

/** @brief data ram size for app global variables and code, could be changed */
#define APP_GLOBAL_SIZE                (59 * 1024)

/** @brief data ram size for heap, could be changed, but (UPPERSTACK_GLOBAL_SIZE + APP_GLOBAL_SIZE + HEAP_DATA_ON_SIZE) must be 115k, and can't less than 10KB  */
#define HEAP_DATA_ON_SIZE              (115 * 1024 - APP_GLOBAL_SIZE - UPPERSTACK_GLOBAL_SIZE)

/** @brief shared cache ram size (adjustable, config SHARE_CACHE_RAM_SIZE: 0/8KB/16KB) */
#define SHARE_CACHE_RAM_SIZE           (0 * 1024)
/*****************************************************/
#define BOOT_INTEGRITY_CHECK_EN           0
/** @brief app global ram start addr, can't be modified */
#define APP_GLOBAL_ADDR                (0x200000 + 29 * 1024 + UPPERSTACK_GLOBAL_SIZE)
#if (APP_BANK == 0)
#define UPPERSTACK_FLASH_ADDR          (BANK0_APP_ADDR + 1024)
#else
#define UPPERSTACK_FLASH_ADDR          (BANK1_APP_ADDR + 1024)
#endif

#ifdef __cplusplus
}
#endif


/** @} */ /* End of group MEM_CONFIG */
#endif
