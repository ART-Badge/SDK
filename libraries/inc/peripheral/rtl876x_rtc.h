/**
*********************************************************************************************************
*               Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file     rtl876x_rtc.h
* \brief    The header file of the peripheral RTC driver.
* \details  This file provides all RTC firmware functions.
* \author   yuan
* \date     2020-06-01
* \version  v2.1.0
* *********************************************************************************************************
*/

#ifndef _RTL876X_RTC_H_
#define _RTL876X_RTC_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup  IO          Peripheral Drivers
 * \defgroup    RTC         RTC
 *
 * \brief       Manage the RTC peripheral functions.
 *
 * \ingroup     IO
 */


/*============================================================================*
 *                         Includes
 *============================================================================*/
#include "rtl876x.h"
#include "rtl876x_alias.h"


/*============================================================================*
 *                         Registers Definitions
 *============================================================================*/

/* Peripheral: RTC */
/* Description: Real time counter register defines */

/* REG_RTC_SLEEP_MODE_SET[1] :RTC_NV_EN. Enable interrupt signal to MCU.  0x1: Enable. 0x0: Disable. */
#define RTC_NV_EN_Pos                   (1UL)
#define RTC_NV_EN_Msk                   (0x1UL << RTC_NV_EN_Pos)
#define RTC_NV_EN_CLR                   (~(RTC_NV_EN_Msk))

/* REG_RTC_SLEEP_MODE_SET[0] :RTC_WAKEUP_EN. Enable wakeup signal to AON register.  0x1: Enable. 0x0: Disable. */
#define RTC_WAKEUP_EN_Pos               (0UL)
#define RTC_WAKEUP_EN_Msk               (0x1UL << RTC_WAKEUP_EN_Pos)
#define RTC_WAKEUP_EN_CLR               (~(RTC_WAKEUP_EN_Msk))

/* Register: CR0 */
/* Description: RTC Control Register 0. Offset: 0x00. Address: 0x40000140. */

/* CR0[18] :RTC_INT_EN. Enable or disable prescale & comp3 interrupt.  0x1: Enable. 0x0: Disable. */
#define RTC_INT_PRE_COMP3_EN_Pos         (18UL)
#define RTC_INT_PRE_COMP3_EN_Msk         (0x1UL << RTC_INT_PRE_COMP3_EN_Pos)

/* CR0[17] :RTC_INT_EN. Enable or disable prescale comp interrupt.  0x1: Enable. 0x0: Disable. */
#define RTC_INT_PRE_COMP_EN_Pos          (17UL)
#define RTC_INT_PRE_COMP_EN_Msk          (0x1UL << RTC_INT_PRE_COMP_EN_Pos)

/* CR0[16] :RTC_INT_EN. Enable or disable tick interrupt.  0x1: Enable. 0x0: Disable. */
#define RTC_INT_TICK_EN_Pos             (16UL)
#define RTC_INT_TICK_EN_Msk             (0x1UL << RTC_INT_TICK_EN_Pos)

/* CR0[15] :RTC_INT_EN. Enable or disable compare3 interrupt.  0x1: Enable. 0x0: Disable. */
#define RTC_INT_COMP3_EN_Pos            (15UL)
#define RTC_INT_COMP3_EN_Msk            (0x1UL << RTC_INT_COMP3_EN_Pos)

/* CR0[14] :RTC_INT_EN. Enable or disable compare2 interrupt.  0x1: Enable. 0x0: Disable. */
#define RTC_INT_COMP2_EN_Pos            (14UL)
#define RTC_INT_COMP2_EN_Msk            (0x1UL << RTC_INT_COMP2_EN_Pos)

/* CR0[13] :RTC_INT_EN. Enable or disable compare1 interrupt.  0x1: Enable. 0x0: Disable. */
#define RTC_INT_COMP1_EN_Pos            (13UL)
#define RTC_INT_COMP1_EN_Msk            (0x1UL << RTC_INT_COMP1_EN_Pos)

/* CR0[12] :RTC_INT_EN. Enable or disable compare0 interrupt.  0x1: Enable. 0x0: Disable. */
#define RTC_INT_COMP0_EN_Pos            (12UL)
#define RTC_INT_COMP0_EN_Msk            (0x1UL << RTC_INT_COMP0_EN_Pos)

/* CR0[11] :RTC_MASK_TICK_INT. Mask RTC prescale & comp3 interrupt.  0x1: Unmask. 0x0: Mask. */
#define RTC_MASK_PRE_COMP3_INT_Pos       (11UL)
#define RTC_MASK_PRE_COMP3_INT_Msk       (0x1UL << RTC_MASK_PRE_COMP3_INT_Pos)

/* CR0[10] :RTC_MASK_TICK_INT. Mask RTC prescale comp interrupt.  0x1: Unmask. 0x0: Mask. */
#define RTC_MASK_PRE_COMP_INT_Pos        (10UL)
#define RTC_MASK_PRE_COMP_INT_Msk        (0x1UL << RTC_MASK_PRE_COMP_INT_Pos)

/* CR0[4] :RTC_MASK_TICK_INT. Mask RTC tick interrupt.  0x1: Unmask. 0x0: Mask. */
#define RTC_MASK_TICK_INT_Pos           (4UL)
#define RTC_MASK_TICK_INT_Msk           (0x1UL << RTC_MASK_TICK_INT_Pos)

/* CR0[2] :RTC_DIV_COUNTER_RST. Reset Prescale Counter. 0x1: Reset Counter to 0. */
#define RTC_DIV_COUNTER_RST_Pos         (2UL)
#define RTC_DIV_COUNTER_RST_Msk         (0x1UL << RTC_DIV_COUNTER_RST_Pos)
#define RTC_DIV_COUNTER_RST_CLR         (~(RTC_DIV_COUNTER_RST_Msk))

/* CR0[1] :RTC_COUNTER_RST. Reset 24bit-RTC Counter. 0x1: Reset Counter to 0. */
#define RTC_COUNTER_RST_Pos             (1UL)
#define RTC_COUNTER_RST_Msk             (0x1UL << RTC_COUNTER_RST_Pos)
#define RTC_COUNTER_RST_CLR             (~(RTC_COUNTER_RST_Msk))

/* CR0[0] :RTC_START. Start or stop RTC 24bit-RTC Counter. 0x1: Start 24bit-RTC Counter.0x0: Stop 24bit-RTC Counter. */
#define RTC_START_Pos                   (0UL)
#define RTC_START_Msk                   (0x1UL << RTC_START_Pos)
#define RTC_START_CLR                   (~(RTC_START_Msk))

/* Register: INT_CLR */
/* Description: Interrupt mask register. Offset: 0x04. Address: 0x40000144. */

/* INT_CLR[7] :RTC_PRE_COMP3_CLR. Clear Interrupt Status of prescale and prescale_comp3. */
/* This interrupt is cleared by software.write 1 then write 0 after 2T to clear. */
#define RTC_PRE_COMP3_CLR_Pos           (7UL)
#define RTC_PRE_COMP3_CLR_SET           (0x1UL << RTC_PRE_COMP3_CLR_Pos)
#define RTC_PRE_COMP3_CLR_RESET         (~(RTC_PRE_COMP3_CLR_SET))

/* INT_CLR[6] :RTC_PRE_COMP_CLR. Clear Interrupt Status of Comparator1. */
/* This interrupt is cleared by software.write 1 then write 0 after 2T to clear. */
#define RTC_PRE_COMP_CLR_Pos            (6UL)
#define RTC_PRE_COMP_CLR_SET            (0x1UL << RTC_PRE_COMP_CLR_Pos)
#define RTC_PRE_COMP_CLR_RESET          (~(RTC_PRE_COMP_CLR_SET))

/* INT_CLR[5] :RTC_COMP3_CLR. Clear Interrupt Status of Comparator1. */
/* This interrupt is cleared by software.write 1 then write 0 after 2T to clear. */
#define RTC_COMP3_CLR_Pos               (5UL)
#define RTC_COMP3_CLR_SET               (0x1UL << RTC_COMP3_CLR_Pos)
#define RTC_COMP3_CLR_RESET             (~(RTC_COMP3_CLR_SET))

/* INT_CLR[4] :RTC_COMP2_CLR. Clear Interrupt Status of Comparator1. */
/* This interrupt is cleared by software.write 1 then write 0 after 2T to clear. */
#define RTC_COMP2_CLR_Pos               (4UL)
#define RTC_COMP2_CLR_SET               (0x1UL << RTC_COMP2_CLR_Pos)
#define RTC_COMP2_CLR_RESET             (~(RTC_COMP2_CLR_SET))

/* INT_CLR[3] :RTC_COMP1_CLR. Clear Interrupt Status of Comparator1. */
/* This interrupt is cleared by software.write 1 then write 0 after 2T to clear. */
#define RTC_COMP1_CLR_Pos               (3UL)
#define RTC_COMP1_CLR_SET               (0x1UL << RTC_COMP1_CLR_Pos)
#define RTC_COMP1_CLR_RESET             (~(RTC_COMP1_CLR_SET))

/* INT_CLR[2] :RTC_COMP0_CLR. Clear Interrupt Status of Comparator0. */
/* This interrupt is cleared by software.write 1 then write 0 after 2T to clear. */
#define RTC_COMP0_CLR_Pos               (2UL)
#define RTC_COMP0_CLR_SET               (0x1UL << RTC_COMP0_CLR_Pos)
#define RTC_COMP0_CLR_RESET             (~(RTC_COMP0_CLR_SET))

/* INT_CLR[1] :RTC_OVERFLOW_CLR. Clear Interrupt Status of Overflow. */
/* This interrupt is cleared by software.write 1 then write 0 after 2T to clear. */
#define RTC_OVERFLOW_CLR_Pos            (1UL)
#define RTC_OVERFLOW_CLR_SET            (0x1UL << RTC_OVERFLOW_CLR_Pos)
#define RTC_OVERFLOW_CLR_RESET          (~(RTC_OVERFLOW_CLR_SET))

/* INT_MASK[0] :RTC_TICK_CLR. Clear Interrupt Status of Tick. */
/* This interrupt is cleared by software.write 1 then write 0 after 2T to clear. */
#define RTC_TICK_CLR_Pos                (0UL)
#define RTC_TICK_CLR_SET                (0x1UL << RTC_TICK_CLR_Pos)
#define RTC_TICK_CLR_RESET              (~(RTC_TICK_CLR_SET))

#define RTC_COMP_INT_EN_OFFSET          (RTC_INT_COMP0_EN_Pos - RTC_COMP0_CLR_Pos)
#define RTC_PRE_COMP_INT_EN_OFFSET      (RTC_INT_PRE_COMP_EN_Pos - RTC_PRE_COMP_CLR_Pos)


/* Clear all interrupt */
#define RTC_ALL_INT_CLR_SET             (RTC_PRE_COMP3_CLR_SET | RTC_PRE_COMP_CLR_SET | \
                                         RTC_COMP3_CLR_SET | RTC_COMP2_CLR_SET | \
                                         RTC_COMP1_CLR_SET | RTC_COMP0_CLR_SET | \
                                         RTC_OVERFLOW_CLR_SET | RTC_TICK_CLR_SET)

#define RTC_ALL_INT_CLR_RESET           (RTC_PRE_COMP3_CLR_RESET & RTC_PRE_COMP_CLR_RESET & \
                                         RTC_COMP3_CLR_RESET & RTC_COMP2_CLR_RESET & \
                                         RTC_COMP1_CLR_RESET & RTC_COMP0_CLR_RESET & \
                                         RTC_OVERFLOW_CLR_RESET & RTC_TICK_CLR_RESET)

/*============================================================================*
 *                         Constants
 *============================================================================*/

/**
 * \defgroup    RTC_Exported_Constants Macro Definitions
 *
 * \ingroup     RTC
 */

/**
 * \defgroup    RTC_Interrupts_Definition RTC Interrupts Definition
 * \{
 * \ingroup     RTC_Exported_Constants
 */
#define RTC_INT_TICK                    ((uint32_t)(1 << 0))
#define RTC_INT_OVF                     ((uint32_t)(1 << 1))
#define RTC_INT_COMP0                   ((uint32_t)(1 << 2))
#define RTC_INT_COMP1                   ((uint32_t)(1 << 3))
#define RTC_INT_COMP2                   ((uint32_t)(1 << 4))
#define RTC_INT_COMP3                   ((uint32_t)(1 << 5))
#define RTC_INT_PRE_COMP                ((uint32_t)(1 << 6))
#define RTC_INT_PRE_COMP3               ((uint32_t)(1 << 7))
/** \} */

#define IS_RTC_INT(INT) (((INT) == RTC_INT_TICK) || \
                         ((INT) == RTC_INT_OVF) || \
                         ((INT) == RTC_INT_COMP0) || \
                         ((INT) == RTC_INT_COMP1) || \
                         ((INT) == RTC_INT_COMP2) || \
                         ((INT) == RTC_INT_COMP3) || \
                         ((INT) == RTC_INT_PRE_COMP) || \
                         ((INT) == RTC_INT_PRE_COMP3))

#define IS_RTC_CONFIG_INT(INT) (((INT) == RTC_INT_TICK) || \
                                ((INT) == RTC_INT_COMP0) || \
                                ((INT) == RTC_INT_COMP1) || \
                                ((INT) == RTC_INT_COMP2) || \
                                ((INT) == RTC_INT_COMP3) || \
                                ((INT) == RTC_INT_PRE_COMP) || \
                                ((INT) == RTC_INT_PRE_COMP3))

/**
 * \defgroup    RTC_Flags_Definition RTC Flags Definition
 * \{
 * \ingroup     RTC_Exported_Constants
 */

#define RTC_FLAG_TICK                   ((uint32_t)(1 << 0))
#define RTC_FLAG_OVF                    ((uint32_t)(1 << 1))
#define RTC_FLAG_COMP0                  ((uint32_t)(1 << 2))
#define RTC_FLAG_COMP1                  ((uint32_t)(1 << 3))
#define RTC_FLAG_COMP2                  ((uint32_t)(1 << 4))
#define RTC_FLAG_COMP3                  ((uint32_t)(1 << 5))
#define RTC_FLAG_PRE_COMP               ((uint32_t)(1 << 6))
#define RTC_FLAG_PRE_COMP3              ((uint32_t)(1 << 7))
/** \} */

#define IS_RTC_FLAG(FLAG) (((FLAG) == RTC_FLAG_TICK) || \
                           ((FLAG) == RTC_FLAG_OVF) || \
                           ((FLAG) == RTC_FLAG_COMP0) || \
                           ((FLAG) == RTC_FLAG_COMP1) || \
                           ((FLAG) == RTC_FLAG_COMP2) || \
                           ((FLAG) == RTC_FLAG_COMP3) || \
                           ((FLAG) == RTC_FLAG_PRE_COMP) || \
                           ((FLAG) == RTC_FLAG_PRE_COMP3))

/**
 * \defgroup    RTC_Comp_Definition RTC Comparator Definition
 * \{
 * \ingroup     RTC_Exported_Constants
 */
#define RTC_COMP0                       ((uint32_t)(0))
#define RTC_COMP1                       ((uint32_t)(1))
#define RTC_COMP2                       ((uint32_t)(2))
#define RTC_COMP3                       ((uint32_t)(3))
#define RTC_PRE_COMP                    ((uint32_t)(4))
/** \} */

#define IS_RTC_COMP(COMP) (((COMP) == RTC_COMP0) || \
                           ((COMP) == RTC_COMP1) || \
                           ((COMP) == RTC_COMP2) || \
                           ((COMP) == RTC_COMP3))


/*============================================================================*
 *                         Functions
 *============================================================================*/

/**
 * \defgroup    RTC_Exported_Functions Peripheral APIs
 * \{
 * \ingroup     RTC
 */

/**
 * \brief   Deinitializes the RTC peripheral registers to their
 *          default reset values(turn off clock).
 * \param[in] None.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_rtc_init(void)
 * {
 *     RTC_DeInit();
 * }
 * \endcode
 */
void RTC_DeInit(void);

/**
 * \brief  Set RTC prescaler value.
 * \param[in]  value: The prescaler value to be set.Should be no more than 12 bits!
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * #define RTC_PRESCALER_VALUE     49
 * #define RTC_COMP_INDEX          RTC_COMP3
 * #define RTC_COMP_INDEX_INT      RTC_INT_COMP3
 * #define RTC_COMP_VALUE          (1000)
 *
 * void driver_rtc_init(void)
 * {
 *     RTC_DeInit();
 *
 *     RTC_SetPrescaler(RTC_PRESCALER_VALUE);
 *     RTC_SetCompValue(RTC_COMP_INDEX, RTC_COMP_VALUE);
 *
 *     RTC_MaskINTConfig(RTC_COMP_INDEX_INT, DISABLE);
 *     RTC_INTConfig(RTC_COMP_INDEX_INT, ENABLE);
 *
 *     RTC_NvCmd(ENABLE);
 *     RTC_Cmd(ENABLE);
 * }
 * \endcode
 */
void RTC_SetPrescaler(uint32_t value);

/**
 * \brief  Start or stop RTC peripheral.
 * \param[in]  NewState: New state of RTC peripheral.
 *     This parameter can be one of the following values:
 *     \arg ENABLE: Start RTC.
 *     \arg DISABLE: Stop RTC.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * #define RTC_PRESCALER_VALUE     49
 * #define RTC_COMP_INDEX          RTC_COMP3
 * #define RTC_COMP_INDEX_INT      RTC_INT_COMP3
 * #define RTC_COMP_VALUE          (1000)
 *
 * void driver_rtc_init(void)
 * {
 *     RTC_DeInit();
 *
 *     RTC_SetPrescaler(RTC_PRESCALER_VALUE);
 *     RTC_SetCompValue(RTC_COMP_INDEX, RTC_COMP_VALUE);
 *
 *     RTC_MaskINTConfig(RTC_COMP_INDEX_INT, DISABLE);
 *     RTC_INTConfig(RTC_COMP_INDEX_INT, ENABLE);
 *
 *     RTC_NvCmd(ENABLE);
 *     RTC_Cmd(ENABLE);
 * }
 * \endcode
 */
void RTC_Cmd(FunctionalState NewState);

/**
 * \brief  Mask or unmask the specified RTC interrupt source.
 * \param[in] RTC_INT: Specifies the RTC interrupt source which to be enabled or disabled.
 *     This parameter can be any combination of the following values:
 *     \arg RTC_INT_TICK: RTC tick interrupt source.
 *     \arg RTC_INT_OVF: RTC counter overflow interrupt source.
 *     \arg RTC_INT_COMP0: Compare 0 interrupt source.
 *     \arg RTC_INT_COMP1: Compare 1 interrupt source.
 *     \arg RTC_INT_COMP2: Compare 2 interrupt source.
 *     \arg RTC_INT_COMP3: Compare 3 interrupt source.
 *     \arg RTC_INT_PRE_COMP: Prescale compare interrupt source.
 *     \arg RTC_INT_PRE_COMP3: Prescale & compare 3 interrupt source.
 * \param[in] NewState: New state of the specified RTC interrupt source.
 *     This parameter can be: ENABLE or DISABLE.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * #define RTC_PRESCALER_VALUE     49
 * #define RTC_COMP_INDEX          RTC_COMP3
 * #define RTC_COMP_INDEX_INT      RTC_INT_COMP3
 * #define RTC_COMP_VALUE          (1000)
 *
 * void driver_rtc_init(void)
 * {
 *     RTC_DeInit();
 *
 *     RTC_SetPrescaler(RTC_PRESCALER_VALUE);
 *     RTC_SetCompValue(RTC_COMP_INDEX, RTC_COMP_VALUE);
 *
 *     RTC_MaskINTConfig(RTC_COMP_INDEX_INT, DISABLE);
 *     RTC_INTConfig(RTC_COMP_INDEX_INT, ENABLE);
 *
 *     RTC_NvCmd(ENABLE);
 *     RTC_Cmd(ENABLE);
 * }
 * \endcode
 */
void RTC_MaskINTConfig(uint32_t RTC_INT, FunctionalState NewState);

/**
 * \brief  Enable or disable the specified RTC interrupt source.
 * \param[in] RTC_INT: Specifies the RTC interrupt source which to be enabled or disabled.
 *     This parameter can be any combination of the following values:
 *     \arg RTC_INT_TICK: Tick interrupt source.
 *     \arg RTC_INT_COMP0: Compare 0 interrupt source.
 *     \arg RTC_INT_COMP1: Compare 1 interrupt source.
 *     \arg RTC_INT_COMP2: Compare 2 interrupt source.
 *     \arg RTC_INT_COMP3: Compare 3 interrupt source.
 *     \arg RTC_INT_PRE_COMP: Prescale compare interrupt source.
 *     \arg RTC_INT_PRE_COMP3: Prescale & compare 3 interrupt source.
 * \param[in] NewState: New state of the specified RTC interrupt.
 *     This parameter can be: ENABLE or DISABLE.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * #define RTC_PRESCALER_VALUE     49
 * #define RTC_COMP_INDEX          RTC_COMP3
 * #define RTC_COMP_INDEX_INT      RTC_INT_COMP3
 * #define RTC_COMP_VALUE          (1000)
 *
 * void driver_rtc_init(void)
 * {
 *     RTC_DeInit();
 *
 *     RTC_SetPrescaler(RTC_PRESCALER_VALUE);
 *     RTC_SetCompValue(RTC_COMP_INDEX, RTC_COMP_VALUE);
 *
 *     RTC_MaskINTConfig(RTC_COMP_INDEX_INT, DISABLE);
 *     RTC_INTConfig(RTC_COMP_INDEX_INT, ENABLE);
 *
 *     RTC_NvCmd(ENABLE);
 *     RTC_Cmd(ENABLE);
 * }
 * \endcode
 */
void RTC_INTConfig(uint32_t RTC_INT, FunctionalState NewState);

/**
 * \brief  Enable RTC interrupt signal to MCU.
 * \param[in] NewState: Enable or disable RTC interrupt signal to MCU.
 *     This parameter can be: ENABLE or DISABLE..
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * #define RTC_PRESCALER_VALUE     49
 * #define RTC_COMP_INDEX          RTC_COMP3
 * #define RTC_COMP_INDEX_INT      RTC_INT_COMP3
 * #define RTC_COMP_VALUE          (1000)
 *
 * void driver_rtc_init(void)
 * {
 *     RTC_DeInit();
 *
 *     RTC_SetPrescaler(RTC_PRESCALER_VALUE);
 *     RTC_SetCompValue(RTC_COMP_INDEX, RTC_COMP_VALUE);
 *
 *     RTC_MaskINTConfig(RTC_COMP_INDEX_INT, DISABLE);
 *     RTC_INTConfig(RTC_COMP_INDEX_INT, ENABLE);
 *
 *     RTC_NvCmd(ENABLE);
 *     RTC_Cmd(ENABLE);
 * }
 * \endcode
 */
void RTC_NvCmd(FunctionalState NewState);

/**
 * \brief  Set RTC comparator value.
 * \param[in]  index: The comparator number,can be 0 ~ 3.
 * \param[in]  value: The comparator value to be set.Should be no more than 24 bits!
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * #define RTC_PRESCALER_VALUE     49
 * #define RTC_COMP_INDEX          RTC_COMP3
 * #define RTC_COMP_INDEX_INT      RTC_INT_COMP3
 * #define RTC_COMP_VALUE          (1000)
 *
 * void driver_rtc_init(void)
 * {
 *     RTC_DeInit();
 *
 *     RTC_SetPrescaler(RTC_PRESCALER_VALUE);
 *     RTC_SetCompValue(RTC_COMP_INDEX, RTC_COMP_VALUE);
 *
 *     RTC_MaskINTConfig(RTC_COMP_INDEX_INT, DISABLE);
 *     RTC_INTConfig(RTC_COMP_INDEX_INT, ENABLE);
 *
 *     RTC_NvCmd(ENABLE);
 *     RTC_Cmd(ENABLE);
 * }
 * \endcode
 */
void RTC_SetCompValue(uint8_t index, uint32_t value);

/**
 * \brief  Set RTC prescaler comparator value.
 * \param[in]  value: The comparator value to be set.Should be no more than 12 bits!
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * #define RTC_PRESCALER_VALUE     (3200 - 1)//max 4095
 * #define RTC_PRECOMP_VALUE       (320)//max 4095
 * #define RTC_COMP3_VALUE         (10)
 *
 * void driver_rtc_init(void)
 * {
 *     RTC_DeInit();
 *
 *     RTC_SetPrescaler(RTC_PRESCALER_VALUE);
 *     RTC_SetPreCompValue(RTC_PRECOMP_VALUE);
 *     RTC_SetCompValue(RTC_COMP3, RTC_COMP3_VALUE);
 *
 *     RTC_MaskINTConfig(RTC_INT_PRE_COMP3, DISABLE);
 *     RTC_INTConfig(RTC_INT_PRE_COMP3, ENABLE);
 *
 *     RTC_NvCmd(ENABLE);
 *     RTC_Cmd(ENABLE);
 * }
 * \endcode
 */
void RTC_SetPreCompValue(uint32_t value);

/**
 * \brief  Enable or disable system wake up of RTC or LPC.
 * \param[in]  NewState: new state of the wake up function.
 *     This parameter can be: ENABLE or DISABLE.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void rtc_demo(void)
 * {
 *     RTC_SystemWakeupConfig(ENABLE);
 * }
 * \endcode
 */
void RTC_SystemWakeupConfig(FunctionalState NewState);

/**
 * \brief  Reset counter value of RTC.
 * \param[in]  none
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void rtc_demo(void)
 * {
 *     RTC_ResetCounter();
 *     RTC_Cmd(ENABLE);
 * }
 * \endcode
 */
void RTC_ResetCounter(void);

/**
 * \brief  Reset prescaler counter value of RTC.
 * \param[in]  none
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void rtc_demo(void)
 * {
 *     RTC_ResetPrescalerCounter();
 *     RTC_Cmd(ENABLE);
 * }
 * \endcode
 */
void RTC_ResetPrescalerCounter(void);

/**
 * \brief  Write backup register for store time information.
 * \param[in]  value
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void rtc_demo(void)
 * {
 *     RTC_WriteBackupReg(0x01020304);
 * }
 * \endcode
 */
void RTC_WriteBackupReg(uint32_t value);

/**
 * \brief  Read backup register.
 * \param[in]  None.
 * \return Register value.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void rtc_demo(void)
 * {
 *     uint32_t reg_data = RTC_ReadBackupReg(0x01020304);
 * }
 * \endcode
 */
uint32_t RTC_ReadBackupReg(void);

/**
 * \brief  Checks whether the specified RTC flag is set or not.
 * \param[in]  RTC_FLAG: specifies the flag to check.
 *     This parameter can be any combination of the following values:
 *     \arg RTC_FLAG_TICK: RTC tick flag
 *     \arg RTC_FLAG_OVF: RTC counter overflow flag
 *     \arg RTC_FLAG_CMP0: RTC compare 0 flag
 *     \arg RTC_FLAG_CMP1: RTC compare 1 flag
 *     \arg RTC_FLAG_CMP2: RTC compare 2 flag
 *     \arg RTC_FLAG_CMP3: RTC compare 3 flag
 *     \arg RTC_FLAG_PRE_COMP: RTC prescale compare flag
 *     \arg RTC_FLAG_PRE_COMP3: RTC prescale & compare 3 flag
 * \return The new state of RTC_FLAG (SET or RESET).
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * #define RTC_PRESCALER_VALUE     (3200 - 1)//max 4095
 * #define RTC_PRECOMP_VALUE       (320)//max 4095
 * #define RTC_COMP3_VALUE         (10)
 *
 * void driver_rtc_init(void)
 * {
 *     RTC_DeInit();
 *
 *     RTC_SetPrescaler(RTC_PRESCALER_VALUE);
 *     RTC_SetPreCompValue(RTC_PRECOMP_VALUE);
 *     RTC_SetCompValue(RTC_COMP3, RTC_COMP3_VALUE);
 *
 *     RTC_MaskINTConfig(RTC_INT_PRE_COMP3, DISABLE);
 *     RTC_INTConfig(RTC_INT_PRE_COMP3, ENABLE);
 *
 *     NVIC_InitTypeDef NVIC_InitStruct;
 *     NVIC_InitStruct.NVIC_IRQChannel = RTC_IRQn;
 *     NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
 *     NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
 *     NVIC_Init(&NVIC_InitStruct);
 *
 *     RTC_NvCmd(ENABLE);
 *     RTC_ResetCounter();
 *     RTC_Cmd(ENABLE);
 * }
 *
 * void RTC_Handler(void)
 * {
 *     if (RTC_GetINTStatus(RTC_INT_PRE_COMP3) == SET)
 *     {
 *         DBG_DIRECT("RTC_INT_PRE_COMP3");
 *         uint32_t counter  = RTC_GetCounter();
 *         RTC_SetCompValue(RTC_COMP3, counter + RTC_COMP3_VALUE);
 *         RTC_ClearINTPendingBit(RTC_INT_PRE_COMP3);
 *     }
 * }
 * \endcode
 */
FlagStatus RTC_GetFlagStatus(uint32_t RTC_FLAG);

/**
 * \brief  Clear the flags of RTC.
 * \param[in]  RTC_FLAG: specifies the flag to clear.
 *     This parameter can be any combination of the following values:
 *     \arg RTC_FLAG_TICK: RTC tick flag.
 *     \arg RTC_FLAG_OVF: RTC counter overflow flag.
 *     \arg RTC_FLAG_CMP0: RTC compare 0 flag.
 *     \arg RTC_FLAG_CMP1: RTC compare 1 flag.
 *     \arg RTC_FLAG_CMP2: RTC compare 2 flag.
 *     \arg RTC_FLAG_CMP3: RTC compare 3 flag.
 *     \arg RTC_FLAG_PRE_COMP: RTC prescale compare flag.
 *     \arg RTC_FLAG_PRE_COMP3: RTC prescale & compare 3 flag.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void rtc_demo(void)
 * {
 *     RTC_ClearFlag(RTC_FLAG_OVF);
 * }
 * \endcode
 */
void RTC_ClearFlag(uint32_t RTC_FLAG);

/**
 * \brief  Check whether the specified RTC interrupt is set.
  * \param[in]  RTC_INT: Specifies the RTC interrupt source to be enabled or disabled.
 *   This parameter can be any combination of the following values:
 *     \arg RTC_INT_TICK: RTC tick interrupt source.
 *     \arg RTC_INT_COMP0: Compare 0 interrupt source.
 *     \arg RTC_INT_COMP1: Compare 1 interrupt source.
 *     \arg RTC_INT_COMP2: Compare 2 interrupt source.
 *     \arg RTC_INT_COMP3: Compare 3 interrupt source.
 *     \arg RTC_INT_PRE_COMP: Prescale compare interrupt source.
 *     \arg RTC_INT_PRE_COMP3: Prescale & compare 3 interrupt source.
 * \return The new state of RTC_INT (SET or RESET).
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void rtc_demo(void)
 * {
 *     ITStatus int_status = RTC_GetINTStatus(RTC_INT_COMP0);
 * }
 * \endcode
 */
ITStatus RTC_GetINTStatus(uint32_t RTC_INT);

/**
 * \brief  Clear the interrupt pending bits of RTC.
 * \param[in]  RTC_INT: specifies the RTC interrupt flag to clear.
 *   This parameter can be any combination of the following values:
 *     \arg RTC_INT_TICK: RTC tick interrupt source.
 *     \arg RTC_INT_OVF: RTC counter overflow interrupt source.
 *     \arg RTC_INT_COMP0: Compare 0 interrupt source.
 *     \arg RTC_INT_COMP1: Compare 1 interrupt source.
 *     \arg RTC_INT_COMP2: Compare 2 interrupt source.
 *     \arg RTC_INT_COMP3: Compare 3 interrupt source.
 *     \arg RTC_INT_PRE_COMP: Prescale compare interrupt source.
 *     \arg RTC_INT_PRE_COMP3: Prescale & compare 3 interrupt source.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void rtc_demo(void)
 * {
 *     RTC_ClearINTPendingBit(RTC_INT_COMP0);
 * }
 * \endcode
 */
void RTC_ClearINTPendingBit(uint32_t RTC_INT);

/**
 * \brief  Clear the interrupt pending bit of the select comparator of RTC.
 * \param[in]  index: the comparator number.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void rtc_demo(void)
 * {
 *     RTC_ClearCompINT(0);
 * }
 * \endcode
 */
void RTC_ClearCompINT(uint8_t index);

/**
 * \brief  Clear the overflow interrupt pending bit of RTC.
 * \param[in]  None.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void rtc_demo(void)
 * {
 *     RTC_ClearOverFlowINT();
 * }
 * \endcode
 */
void RTC_ClearOverFlowINT(void);

/**
 * \brief  Clear the tick interrupt pending bit of RTC.
 * \param[in]  None.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void rtc_demo(void)
 * {
 *     RTC_ClearTickINT();
 * }
 * \endcode
 */
void RTC_ClearTickINT(void);

/**
 * \brief  Fast write RTC register.
 * \param[in]  regAddress: Rhe register address.
 * \param[in]  data: Data which write to register.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void rtc_demo(void)
 * {
 *     uitn32_t address = 0;//The adderss to be writen.
 *     uitn32_t data = 0x01020304;
 *     RTC_WriteReg(address, data);
 * }
 * \endcode
 */
void RTC_WriteReg(uint32_t regAddress, uint32_t data);

/**
 * \brief  Get counter value of RTC.
 * \param[in] None.
 * \return The counter value.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void rtc_demo(void)
 * {
 *     uitn32_t data = RTC_GetCounter();
 * }
 * \endcode
 */
__STATIC_INLINE uint32_t RTC_GetCounter(void)
{
    return RTC->CNT;
}

/**
 * \brief  Get prescaler counter value of RTC.
 * \param[in] None.
 * \return The prescaler counter value.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void rtc_demo(void)
 * {
 *     uitn32_t data = RTC_GetPreCounter();
 * }
 * \endcode
 */
__STATIC_INLINE uint32_t RTC_GetPreCounter(void)
{
    return RTC->PRE_CNT;
}

/**
 * \brief  Get RTC comparator value.
 * \param[in]  index: The comparator number.
 * \return  The comparator value.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void rtc_demo(void)
 * {
 *     uitn32_t data = RTC_GetCompValue();
 * }
 * \endcode
 */
__STATIC_INLINE uint32_t RTC_GetCompValue(uint8_t index)
{
    return *((volatile uint32_t *)(&(RTC->COMP0) + index));
}

/**
 * \brief  Get RTC prescaler comparator value.
 * \param[in] None.
 * \return The prescaler comparator value.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void rtc_demo(void)
 * {
 *     uitn32_t data = RTC_GetPreCompValue();
 * }
 * \endcode
 */
__STATIC_INLINE uint32_t RTC_GetPreCompValue(void)
{
    return RTC->PRE_COMP;
}

/** \} */ /* End of group RTC_Exported_Functions */

#ifdef __cplusplus
}
#endif

#endif /* _RTL876X_RTC_H_ */


/******************* (C) COPYRIGHT 2020 Realtek Semiconductor *****END OF FILE****/

