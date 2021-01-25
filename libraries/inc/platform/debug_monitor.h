/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    debug_monitor.h
  * @brief   Debug Monitor
  * @author  lory_xu
  * @date    2017-03-17
  * @version v1.0
  * *************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   * *************************************************************************************
  */

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef __DEBUG_MONITOR_H
#define __DEBUG_MONITOR_H

/*============================================================================*
 *                               Header Files
*============================================================================*/

#include <stdint.h>
#include <stdbool.h>
#include "core_cm4.h"

#ifdef __cplusplus
extern "C" {
#endif


/** @defgroup DEBUG_MONITOR Debug Monitor
  * @brief Wapper functions for ARM DWT feature
  * @{
  */

/*============================================================================*
 *                               Constants
*============================================================================*/
/** @defgroup DEBUG_MONITOR_Exported_Constants Debug Monitor Exported Constants
  * @{
  */

/************************ DWT *********************/
#define DWT_FUNCTION_READ          (0x5 << DWT_FUNCTION_FUNCTION_Pos)
#define DWT_FUNCTION_WRITE         (0x6 << DWT_FUNCTION_FUNCTION_Pos)
#define DWT_FUNCTION_READ_OR_WRITE (0x7 << DWT_FUNCTION_FUNCTION_Pos)
#define DWT_DATAVSIZE_BYTE         (0x0 << DWT_FUNCTION_DATAVSIZE_Pos)
#define DWT_DATAVSIZE_HALF_WORD    (0x1 << DWT_FUNCTION_DATAVSIZE_Pos)
#define DWT_DATAVSIZE_WORD         (0x2 << DWT_FUNCTION_DATAVSIZE_Pos)

#define DWT_DATASIZE_ALIGN_1_BYTE    0
#define DWT_DATASIZE_ALIGN_2_BYTE    1
#define DWT_DATASIZE_ALIGN_4_BYTE    2
#define DWT_DATASIZE_ALIGN_8_BYTE    3
#define DWT_DATASIZE_ALIGN_16_BYTE   4
#define DWT_DATASIZE_ALIGN_32_BYTE   5
#define DWT_DATASIZE_ALIGN_64_BYTE   6

/** @} */ /* End of group DEBUG_MONITOR_Exported_Constants */


/*============================================================================*
 *                               Functions
*============================================================================*/
/** @defgroup DEBUG_MONITOR_Exported_Functions Debug Monitor Exported Functions
  * @{
  */

/**
 * @brief     Configure Watchpoint 0
 * @param[in] watch_address: the address of variable which is being watched
 * @param[in] watch_size: the size of variable which is being watched.
 *            the valid value are listed below:
 *      @arg @ref DWT_DATAVSIZE_BYTE      Detect 1 byte access
 *      @arg @ref DWT_DATAVSIZE_HALF_WORD  Detect 2 bytes access
 *      @arg @ref DWT_DATAVSIZE_WORD     Detect 4 bytes access
 * @param[in] read_write_func: the detecting type of Watchpoint.
 *            the valid value are listed below:
 *      @arg @ref DWT_FUNCTION_READ          Detect Read access
 *      @arg @ref DWT_FUNCTION_WRITE         Detect Write access
 *      @arg @ref DWT_FUNCTION_READ_OR_WRITE Detect Read or Write access
 * @return    none
 */
static inline void watch_point_0_setting(uint32_t watch_address, uint32_t watch_size,
                                         uint32_t read_write_func)
{
    DWT->COMP0 = watch_address;
    DWT->MASK0 = 0; // all bits are compared
    DWT->FUNCTION0 = read_write_func | watch_size;
}

/**
 * @brief  Configure Watchpoint 1
 * @param[in] watch_address: the address of variable which is being watched
 * @param[in] watch_size: the size of variable which is being watched.
 *            the valid value are listed below:
 *      @arg @ref DWT_DATAVSIZE_BYTE      Detect 1 byte access
 *      @arg @ref DWT_DATAVSIZE_HALF_WORD Detect 2 bytes access
 *      @arg @ref DWT_DATAVSIZE_WORD     Detect 4 bytes access
 * @param[in] read_write_func: the detecting type of Watchpoint.
 *            the valid value are listed below:
 *      @arg @ref DWT_FUNCTION_READ          Detect Read access
 *      @arg @ref DWT_FUNCTION_WRITE         Detect Write access
 *      @arg @ref DWT_FUNCTION_READ_OR_WRITE Detect Read or Write access
 * @return    none
 */
static inline void watch_point_1_setting(uint32_t watch_address, uint32_t watch_size,
                                         uint32_t read_write_func)
{
    DWT->COMP1 = watch_address;
    DWT->MASK1 = 0; // all bits are compared
    DWT->FUNCTION1 = read_write_func | watch_size;
}

/**
 * @brief  Configure Watchpoint 2
 * @param[in] watch_address: the address of variable which is being watched
 * @param[in] watch_size: the size of variable which is being watched.
 *            the valid value are listed below:
 *      @arg @ref DWT_DATAVSIZE_BYTE      Detect 1 byte access
 *      @arg @ref DWT_DATAVSIZE_HALF_WORD Detect 2 bytes access
 *      @arg @ref DWT_DATAVSIZE_WORD     Detect 4 bytes access
 * @param[in] read_write_func: the detecting type of Watchpoint.
 *            the valid value are listed below:
 *      @arg @ref DWT_FUNCTION_READ          Detect Read access
 *      @arg @ref DWT_FUNCTION_WRITE         Detect Write access
 *      @arg @ref DWT_FUNCTION_READ_OR_WRITE Detect Read or Write access
 * @return    none
 */
static inline void watch_point_2_setting(uint32_t watch_address, uint32_t watch_size,
                                         uint32_t read_write_func)
{
    DWT->COMP2 = watch_address;
    DWT->MASK2 = 0; // all bits are compared
    DWT->FUNCTION2 = read_write_func | watch_size;
}

/**
 * @brief  Configure Watchpoint 3
 * @param[in] watch_address: the address of variable which is being watched
 * @param[in] watch_size: the size of variable which is being watched.
 *            the valid value are listed below:
 *      @arg @ref DWT_DATAVSIZE_BYTE      Detect 1 byte access
 *      @arg @ref DWT_DATAVSIZE_HALF_WORD Detect 2 bytes access
 *      @arg @ref DWT_DATAVSIZE_WORD     Detect 4 bytes access
 * @param[in] read_write_func: the detecting type of Watchpoint.
 *            the valid value are listed below:
 *      @arg @ref DWT_FUNCTION_READ          Detect Read access
 *      @arg @ref DWT_FUNCTION_WRITE         Detect Write access
 *      @arg @ref DWT_FUNCTION_READ_OR_WRITE Detect Read or Write access
 * @return    none
 */
static inline void watch_point_3_setting(uint32_t watch_address, uint32_t watch_size,
                                         uint32_t read_write_func)
{
    DWT->COMP3 = watch_address;
    DWT->MASK3 = 0; // all bits are compared
    DWT->FUNCTION3 = read_write_func | watch_size;
}

/**
 * @brief     Configure Watchpoint 0
 * @param[in] watch_address_align: the aligned start address which is being watched
              If the watch_address_align is not aligned to the watch_size, the lower bits of the watch_address_align will be ignored.
              For example, watch_address_align is 0x200004, watch_size is DWT_DATASIZE_ALIGN_8_BYTE,
              the lower 3 bits of 0x200004 will be ignored, so the compare address is 0x200000, size is 8 bytes.
 * @param[in] watch_size: the aligned size of memory which is being watched.
 *            the valid value are listed below:
 *      @arg @ref DWT_DATASIZE_ALIGN_1_BYTE   Detect 1 byte access
 *      @arg @ref DWT_DATASIZE_ALIGN_2_BYTE   Detect 2 bytes access
 *      @arg @ref DWT_DATASIZE_ALIGN_4_BYTE   Detect 4 bytes access
 *      @arg @ref DWT_DATASIZE_ALIGN_8_BYTE   Detect 8 bytes access
 *      @arg @ref DWT_DATASIZE_ALIGN_16_BYTE  Detect 16 bytes access
 *      @arg @ref DWT_DATASIZE_ALIGN_32_BYTE  Detect 32 bytes access
 *      @arg @ref DWT_DATASIZE_ALIGN_64_BYTE  Detect 64 bytes access
 * @param[in] read_write_func: the detecting type of Watchpoint.
 *            the valid value are listed below:
 *      @arg @ref DWT_FUNCTION_READ          Detect Read access
 *      @arg @ref DWT_FUNCTION_WRITE         Detect Write access
 *      @arg @ref DWT_FUNCTION_READ_OR_WRITE Detect Read or Write access
 * @return    none
 */
static inline void watch_point_0_setting_align(uint32_t watch_address_align, uint16_t watch_size,
                                               uint32_t read_write_func)
{
    DWT->COMP0 = watch_address_align;
    DWT->MASK0 = watch_size;
    DWT->FUNCTION0 = read_write_func;
}

/**
 * @brief     Configure Watchpoint 1
 * @param[in] watch_address_align: the aligned start address which is being watched
              If the watch_address_align is not aligned to the watch_size, the lower bits of the watch_address_align will be ignored.
              For example, watch_address_align is 0x200004, watch_size is DWT_DATASIZE_ALIGN_8_BYTE,
              the lower 3 bits of 0x200004 will be ignored, so the compare address is 0x200000, size is 8 bytes.
 * @param[in] watch_size: the aligned size of memory which is being watched.
 *            the valid value are listed below:
 *      @arg @ref DWT_DATASIZE_ALIGN_1_BYTE   Detect 1 byte access
 *      @arg @ref DWT_DATASIZE_ALIGN_2_BYTE   Detect 2 bytes access
 *      @arg @ref DWT_DATASIZE_ALIGN_4_BYTE   Detect 4 bytes access
 *      @arg @ref DWT_DATASIZE_ALIGN_8_BYTE   Detect 8 bytes access
 *      @arg @ref DWT_DATASIZE_ALIGN_16_BYTE  Detect 16 bytes access
 *      @arg @ref DWT_DATASIZE_ALIGN_32_BYTE  Detect 32 bytes access
 *      @arg @ref DWT_DATASIZE_ALIGN_64_BYTE  Detect 64 bytes access
 * @param[in] read_write_func: the detecting type of Watchpoint.
 *            the valid value are listed below:
 *      @arg @ref DWT_FUNCTION_READ          Detect Read access
 *      @arg @ref DWT_FUNCTION_WRITE         Detect Write access
 *      @arg @ref DWT_FUNCTION_READ_OR_WRITE Detect Read or Write access
 * @return    none
 */
static inline void watch_point_1_setting_align(uint32_t watch_address_align, uint16_t watch_size,
                                               uint32_t read_write_func)
{
    DWT->COMP1 = watch_address_align;
    DWT->MASK1 = watch_size;
    DWT->FUNCTION1 = read_write_func;
}

/**
 * @brief     Configure Watchpoint 2
 * @param[in] watch_address_align: the aligned start address which is being watched
              If the watch_address_align is not aligned to the watch_size, the lower bits of the watch_address_align will be ignored.
              For example, watch_address_align is 0x200004, watch_size is DWT_DATASIZE_ALIGN_8_BYTE,
              the lower 3 bits of 0x200004 will be ignored, so the compare address is 0x200000, size is 8 bytes.
 * @param[in] watch_size: the aligned size of memory which is being watched.
 *            the valid value are listed below:
 *      @arg @ref DWT_DATASIZE_ALIGN_1_BYTE   Detect 1 byte access
 *      @arg @ref DWT_DATASIZE_ALIGN_2_BYTE   Detect 2 bytes access
 *      @arg @ref DWT_DATASIZE_ALIGN_4_BYTE   Detect 4 bytes access
 *      @arg @ref DWT_DATASIZE_ALIGN_8_BYTE   Detect 8 bytes access
 *      @arg @ref DWT_DATASIZE_ALIGN_16_BYTE  Detect 16 bytes access
 *      @arg @ref DWT_DATASIZE_ALIGN_32_BYTE  Detect 32 bytes access
 *      @arg @ref DWT_DATASIZE_ALIGN_64_BYTE  Detect 64 bytes access
 * @param[in] read_write_func: the detecting type of Watchpoint.
 *            the valid value are listed below:
 *      @arg @ref DWT_FUNCTION_READ          Detect Read access
 *      @arg @ref DWT_FUNCTION_WRITE         Detect Write access
 *      @arg @ref DWT_FUNCTION_READ_OR_WRITE Detect Read or Write access
 * @return    none
 */
static inline void watch_point_2_setting_align(uint32_t watch_address_align, uint16_t watch_size,
                                               uint32_t read_write_func)
{
    DWT->COMP2 = watch_address_align;
    DWT->MASK2 = watch_size;
    DWT->FUNCTION2 = read_write_func;
}

/**
 * @brief     Configure Watchpoint 3
 * @param[in] watch_address_align: the aligned start address which is being watched
              If the watch_address_align is not aligned to the watch_size, the lower bits of the watch_address_align will be ignored.
              For example, watch_address_align is 0x200004, watch_size is DWT_DATASIZE_ALIGN_8_BYTE,
              the lower 3 bits of 0x200004 will be ignored, so the compare address is 0x200000, size is 8 bytes.
 * @param[in] watch_size: the aligned size of memory which is being watched.
 *            the valid value are listed below:
 *      @arg @ref DWT_DATASIZE_ALIGN_1_BYTE   Detect 1 byte access
 *      @arg @ref DWT_DATASIZE_ALIGN_2_BYTE   Detect 2 bytes access
 *      @arg @ref DWT_DATASIZE_ALIGN_4_BYTE   Detect 4 bytes access
 *      @arg @ref DWT_DATASIZE_ALIGN_8_BYTE   Detect 8 bytes access
 *      @arg @ref DWT_DATASIZE_ALIGN_16_BYTE  Detect 16 bytes access
 *      @arg @ref DWT_DATASIZE_ALIGN_32_BYTE  Detect 32 bytes access
 *      @arg @ref DWT_DATASIZE_ALIGN_64_BYTE  Detect 64 bytes access
 * @param[in] read_write_func: the detecting type of Watchpoint.
 *            the valid value are listed below:
 *      @arg @ref DWT_FUNCTION_READ          Detect Read access
 *      @arg @ref DWT_FUNCTION_WRITE         Detect Write access
 *      @arg @ref DWT_FUNCTION_READ_OR_WRITE Detect Read or Write access
 * @return    none
 */
static inline void watch_point_3_setting_align(uint32_t watch_address_align, uint16_t watch_size,
                                               uint32_t read_write_func)
{
    DWT->COMP3 = watch_address_align;
    DWT->MASK3 = watch_size;
    DWT->FUNCTION3 = read_write_func;
}

/** @} */ /* End of group DEBUG_MONITOR_Exported_Functions */


/** @} */ /* End of group DEBUG_MONITOR */

#ifdef __cplusplus
}
#endif

#endif  /* __DEBUG_MONITOR_H */
