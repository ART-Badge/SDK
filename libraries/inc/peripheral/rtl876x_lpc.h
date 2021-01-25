/**
*********************************************************************************************************
*               Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file     rtl876x_lpc.h
* \brief    The header file of the peripheral LPC driver.
* \details  This file provides all LPC firmware functions.
* \author   yuan
* \date     2020-06-01
* \version  v2.1.0
* *********************************************************************************************************
*/

#ifndef _RTL876X_LPC_H_
#define _RTL876X_LPC_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup  IO          Peripheral Drivers
 * \defgroup    LPC         LPC
 *
 * \brief       Manage the LPC peripheral functions.
 *
 * \ingroup     IO
 */

/*============================================================================*
 *                         Includes
 *============================================================================*/
#include "rtl876x.h"


/*============================================================================*
 *                         Types
 *============================================================================*/

/**
 * \defgroup    LPC_Exported_Types  Init Params Struct
 *
 * \ingroup     LPC
 */

/**
 * \brief       LPC init structure definition
 *
 * \ingroup     LPC_Exported_Types
 */
typedef struct
{
    uint16_t LPC_Channel;           /*!< Specifies the input pin.
                                            This parameter can be a value of ADC_0 to ADC_7. */
    uint32_t LPC_Edge;              /*!< Specifies the comparator output edge. */
    uint32_t LPC_Threshold;         /*!< Specifies the threshold value of comparator voltage. */

} LPC_InitTypeDef;


/*============================================================================*
 *                         Register Defines
 *============================================================================*/

/* Peripheral: LPC */
/* Description: Low Power Comparator register defines */

/* Register: LPC_AON_52 */
/* Description: LPC AON Register for threshold voltage configuration */

/* LPC_AON_52[5:0] :LPC_Threshold. Specified the threshold value of comparator voltage */
#define LPC_AON_52_THRESHOLD_Pos        (0UL)
#define LPC_AON_52_THRESHOLD_Msk        (0x3fUL << LPC_AON_52_THRESHOLD_Pos)

/* Register: LPC_AON_114 */
/* Description: LPC AON Register for parameters configuration */

/* LPC_AON_114[5] :LPC_Edge. Specified the cmp output edge */
#define LPC_AON_114_POSEDGE_Pos         (5UL)
#define LPC_AON_114_POSEDGE_Msk         (0x1UL << LPC_AON_114_POSEDGE_Pos)
/* LPC_AON_114[4:1] :LPC_Channel. Specified the input pin */
#define LPC_AON_114_CH_NUM_Pos          (1UL)
#define LPC_AON_114_CH_NUM_Msk          (0xFUL << LPC_AON_114_CH_NUM_Pos)
/* LPC_AON_114[0] : LPC_POWER. 1: Enable power of low power cmp */
#define LPC_AON_114_POWER_EN_Pos        (0UL)
#define LPC_AON_114_POWER_EN_Msk        (0x1UL << LPC_AON_114_POWER_EN_Pos)
#define LPC_AON_114_POWER_EN_CLR        (~LPC_AON_114_POWER_EN_Msk)

/* Register: LPC_CR0 */
/* Description: Control Register 0 */
/* LPC_CR0[9] :LPC_SRC_INT_EN. 1: Enable lpcomp out sync signal to AON interrupt */
#define LPC_INT_LPCOMP_AON_EN_Pos       (19UL)
#define LPC_INT_LPCOMP_AON_EN_Msk       (0x1UL << LPC_INT_LPCOMP_AON_EN_Pos)
#define LPC_INT_LPCOMP_AON_EN_CLR       (~(LPC_INT_LPCOMP_AON_EN_Msk))

/* LPC_CR0[9] :LPC_NV_EN. 1: Enable lpcomp out sync signal to CPU interrupt */
#define LPC_INT_LPCOMP_NV_EN_Pos        (9UL)
#define LPC_INT_LPCOMP_NV_EN_Msk        (0x1UL << LPC_INT_LPCOMP_NV_EN_Pos)
#define LPC_INT_LPCOMP_NV_EN_CLR        (~(LPC_INT_LPCOMP_NV_EN_Msk))

/* LPC_CR0[8] :LPC_EVENT_EN. 1: Enable or disable LPCOMP event */
#define LPC_INT_LPCOMP_CNT_EN_Pos       (8UL)
#define LPC_INT_LPCOMP_CNT_EN_Msk       (0x1UL << LPC_INT_LPCOMP_CNT_EN_Pos)
#define LPC_INT_LPCOMP_CNT_EN_CLR       (~(LPC_INT_LPCOMP_CNT_EN_Msk))

/* LPC_CR0[2] :LPC_FLAG_CLEAR. 1: Clear Event Status of LPCOMP */
#define LPC_LPCOMP_CNT_CLEAR_Pos        (2UL)
#define LPC_LPCOMP_CNT_CLEAR_Msk        (0x1UL << LPC_LPCOMP_CNT_CLEAR_Pos)

/* LPC_CR0[1] :LPC_COUNTER_START. 1: Start LPCOMP counter. */
#define LPC_COUNTER_START_Pos           (1UL)
#define LPC_COUNTER_START_Msk           (0x1UL << LPC_COUNTER_START_Pos)
#define LPC_COUNTER_START_CLR           (~(LPC_COUNTER_START_Msk))

/* LPC_CR0[0] :LPC_COUNTER_RESET. 1: Reset LPCOMP Counter */
#define LPC_COUNTER_RESET_Pos           (0UL)
#define LPC_COUNTER_RESET_Msk           (0x1UL << LPC_COUNTER_RESET_Pos)
#define LPC_COUNTER_RESET_CLR           (~(LPC_COUNTER_RESET_Msk))

/* Register: LPC_SR */
/* Description: Status Register */

/* LPC_SR[1] :LPC_COMP_OUTPUT. Event Status of LPCOMP output signal */
#define LPC_LPCOMP_OUTPUT_AON_Pos       (1UL) /*!< Position of */
#define LPC_LPCOMP_OUTPUT_AON_Msk       (0x1UL << LPC_LPCOMP_OUTPUT_AON_Pos)

/* LPC_SR[0] :LPC_COMP. Event Status of LPCOMP */
#define LPC_LPCOMP_CNT_Pos              (0UL) /*!< Position of */
#define LPC_LPCOMP_CNT_Msk              (0x1UL << LPC_LPCOMP_CNT_Pos)

/*============================================================================*
 *                         Constants
 *============================================================================*/

/**
 * \defgroup    LPC_Exported_constants  Marco Definitions
 *
 * \ingroup     LPC
 */

/**
 * \defgroup    LPC_Channel LPC Channel
 * \{
 * \ingroup     LPC_Exported_constants
 */
#define LPC_CHANNEL_P2_0                ((uint32_t)0)
#define LPC_CHANNEL_P2_1                ((uint32_t)1)
#define LPC_CHANNEL_P2_2                ((uint32_t)2)
#define LPC_CHANNEL_P2_3                ((uint32_t)3)
#define LPC_CHANNEL_P2_4                ((uint32_t)4)
#define LPC_CHANNEL_P2_5                ((uint32_t)5)
#define LPC_CHANNEL_P2_6                ((uint32_t)6)
#define LPC_CHANNEL_P2_7                ((uint32_t)7)
#define LPC_CHANNEL_VBAT                ((uint32_t)8)
/** \} */

#define IS_LPC_CHANNEL(CHANNEL) (((CHANNEL) == LPC_CHANNEL_P2_0) || \
                                 ((CHANNEL) == LPC_CHANNEL_P2_1) || \
                                 ((CHANNEL) == LPC_CHANNEL_P2_2) || \
                                 ((CHANNEL) == LPC_CHANNEL_P2_3) || \
                                 ((CHANNEL) == LPC_CHANNEL_P2_4) || \
                                 ((CHANNEL) == LPC_CHANNEL_P2_5) || \
                                 ((CHANNEL) == LPC_CHANNEL_P2_6) || \
                                 ((CHANNEL) == LPC_CHANNEL_P2_7) || \
                                 ((CHANNEL) == LPC_CHANNEL_VBAT))

/** \defgroup   LPC_Edge    LPC Edge
 * \{
 * \ingroup     LPC_Exported_constants
 */
#define LPC_Vin_Below_Vth               ((uint32_t)0)
#define LPC_Vin_Over_Vth                ((uint32_t)(0x0001UL << LPC_AON_114_POSEDGE_Pos))
/** \} */

#define IS_LPC_EDGE(EDGE) (((EDGE) == LPC_Vin_Below_Vth) || \
                           ((EDGE) == LPC_Vin_Over_Vth))

/** \defgroup   LPC_Threshold   LPC Threshold
 * \{
 * \ingroup     LPC_Exported_constants
 */
#define LPC_80_mV                  ((uint32_t)(0x0000 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_160_mV                 ((uint32_t)(0x0001 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_240_mV                 ((uint32_t)(0x0002 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_320_mV                 ((uint32_t)(0x0003 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_400_mV                 ((uint32_t)(0x0004 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_480_mV                 ((uint32_t)(0x0005 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_560_mV                 ((uint32_t)(0x0006 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_640_mV                 ((uint32_t)(0x0007 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_680_mV                 ((uint32_t)(0x0008 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_720_mV                 ((uint32_t)(0x0009 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_760_mV                 ((uint32_t)(0x000a << LPC_AON_52_THRESHOLD_Pos))
#define LPC_800_mV                 ((uint32_t)(0x000b << LPC_AON_52_THRESHOLD_Pos))
#define LPC_840_mV                 ((uint32_t)(0x000c << LPC_AON_52_THRESHOLD_Pos))
#define LPC_880_mV                 ((uint32_t)(0x000d << LPC_AON_52_THRESHOLD_Pos))
#define LPC_920_mV                 ((uint32_t)(0x000e << LPC_AON_52_THRESHOLD_Pos))
#define LPC_960_mV                 ((uint32_t)(0x000f << LPC_AON_52_THRESHOLD_Pos))
#define LPC_1000_mV                ((uint32_t)(0x0010 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_1040_mV                ((uint32_t)(0x0011 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_1080_mV                ((uint32_t)(0x0012 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_1120_mV                ((uint32_t)(0x0013 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_1160_mV                ((uint32_t)(0x0014 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_1200_mV                ((uint32_t)(0x0015 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_1240_mV                ((uint32_t)(0x0016 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_1280_mV                ((uint32_t)(0x0017 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_1320_mV                ((uint32_t)(0x0018 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_1360_mV                ((uint32_t)(0x0019 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_1400_mV                ((uint32_t)(0x001a << LPC_AON_52_THRESHOLD_Pos))
#define LPC_1440_mV                ((uint32_t)(0x001b << LPC_AON_52_THRESHOLD_Pos))
#define LPC_1480_mV                ((uint32_t)(0x001c << LPC_AON_52_THRESHOLD_Pos))
#define LPC_1520_mV                ((uint32_t)(0x001d << LPC_AON_52_THRESHOLD_Pos))
#define LPC_1560_mV                ((uint32_t)(0x001e << LPC_AON_52_THRESHOLD_Pos))
#define LPC_1600_mV                ((uint32_t)(0x001f << LPC_AON_52_THRESHOLD_Pos))
#define LPC_1640_mV                ((uint32_t)(0x0020 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_1680_mV                ((uint32_t)(0x0021 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_1720_mV                ((uint32_t)(0x0022 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_1760_mV                ((uint32_t)(0x0023 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_1800_mV                ((uint32_t)(0x0024 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_1840_mV                ((uint32_t)(0x0025 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_1880_mV                ((uint32_t)(0x0026 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_1920_mV                ((uint32_t)(0x0027 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_1960_mV                ((uint32_t)(0x0028 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_2000_mV                ((uint32_t)(0x0029 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_2040_mV                ((uint32_t)(0x002a << LPC_AON_52_THRESHOLD_Pos))
#define LPC_2080_mV                ((uint32_t)(0x002b << LPC_AON_52_THRESHOLD_Pos))
#define LPC_2120_mV                ((uint32_t)(0x002c << LPC_AON_52_THRESHOLD_Pos))
#define LPC_2160_mV                ((uint32_t)(0x002d << LPC_AON_52_THRESHOLD_Pos))
#define LPC_2200_mV                ((uint32_t)(0x002e << LPC_AON_52_THRESHOLD_Pos))
#define LPC_2240_mV                ((uint32_t)(0x002f << LPC_AON_52_THRESHOLD_Pos))
#define LPC_2280_mV                ((uint32_t)(0x0030 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_2320_mV                ((uint32_t)(0x0031 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_2360_mV                ((uint32_t)(0x0032 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_2400_mV                ((uint32_t)(0x0033 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_2440_mV                ((uint32_t)(0x0034 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_2480_mV                ((uint32_t)(0x0035 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_2520_mV                ((uint32_t)(0x0036 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_2560_mV                ((uint32_t)(0x0037 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_2640_mV                ((uint32_t)(0x0038 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_2720_mV                ((uint32_t)(0x0039 << LPC_AON_52_THRESHOLD_Pos))
#define LPC_2800_mV                ((uint32_t)(0x003a << LPC_AON_52_THRESHOLD_Pos))
#define LPC_2880_mV                ((uint32_t)(0x003b << LPC_AON_52_THRESHOLD_Pos))
#define LPC_2960_mV                ((uint32_t)(0x003c << LPC_AON_52_THRESHOLD_Pos))
#define LPC_3040_mV                ((uint32_t)(0x003d << LPC_AON_52_THRESHOLD_Pos))
#define LPC_3120_mV                ((uint32_t)(0x003e << LPC_AON_52_THRESHOLD_Pos))
#define LPC_3200_mV                ((uint32_t)(0x003f << LPC_AON_52_THRESHOLD_Pos))
/** \} */

#define IS_LPC_THRESHOLD(THRESHOLD) (((THRESHOLD) == LPC_80_mV) || ((THRESHOLD) == LPC_160_mV) || ((THRESHOLD) == LPC_240_mV) || \
                                     ((THRESHOLD) == LPC_320_mV) || ((THRESHOLD) == LPC_400_mV) || ((THRESHOLD) == LPC_480_mV) || \
                                     ((THRESHOLD) == LPC_560_mV) || ((THRESHOLD) == LPC_640_mV) || ((THRESHOLD) == LPC_680_mV) || \
                                     ((THRESHOLD) == LPC_720_mV) || ((THRESHOLD) == LPC_760_mV) || ((THRESHOLD) == LPC_800_mV) || \
                                     ((THRESHOLD) == LPC_840_mV) || ((THRESHOLD) == LPC_880_mV) || ((THRESHOLD) == LPC_920_mV) || \
                                     ((THRESHOLD) == LPC_960_mV) || ((THRESHOLD) == LPC_1000_mV) || ((THRESHOLD) == LPC_1040_mV) || \
                                     ((THRESHOLD) == LPC_1080_mV) || ((THRESHOLD) == LPC_1120_mV) || ((THRESHOLD) == LPC_1160_mV) || \
                                     ((THRESHOLD) == LPC_1200_mV) || ((THRESHOLD) == LPC_1240_mV) || ((THRESHOLD) == LPC_1280_mV) || \
                                     ((THRESHOLD) == LPC_1320_mV) || ((THRESHOLD) == LPC_1360_mV) || ((THRESHOLD) == LPC_1400_mV) || \
                                     ((THRESHOLD) == LPC_1440_mV) || ((THRESHOLD) == LPC_1480_mV) || ((THRESHOLD) == LPC_1520_mV) || \
                                     ((THRESHOLD) == LPC_1560_mV) || ((THRESHOLD) == LPC_1600_mV) || ((THRESHOLD) == LPC_1640_mV) || \
                                     ((THRESHOLD) == LPC_1680_mV) || ((THRESHOLD) == LPC_1720_mV) || ((THRESHOLD) == LPC_1760_mV) || \
                                     ((THRESHOLD) == LPC_1800_mV) || ((THRESHOLD) == LPC_1840_mV) || ((THRESHOLD) == LPC_1880_mV) || \
                                     ((THRESHOLD) == LPC_1920_mV) || ((THRESHOLD) == LPC_1960_mV) || ((THRESHOLD) == LPC_2000_mV) || \
                                     ((THRESHOLD) == LPC_2040_mV) || ((THRESHOLD) == LPC_2080_mV) || ((THRESHOLD) == LPC_2120_mV) || \
                                     ((THRESHOLD) == LPC_2160_mV) || ((THRESHOLD) == LPC_2200_mV) || ((THRESHOLD) == LPC_2240_mV) || \
                                     ((THRESHOLD) == LPC_2280_mV) || ((THRESHOLD) == LPC_2320_mV) || ((THRESHOLD) == LPC_2360_mV) || \
                                     ((THRESHOLD) == LPC_2400_mV) || ((THRESHOLD) == LPC_2440_mV) || ((THRESHOLD) == LPC_2480_mV) || \
                                     ((THRESHOLD) == LPC_2520_mV) || ((THRESHOLD) == LPC_2560_mV) || ((THRESHOLD) == LPC_2640_mV) || \
                                     ((THRESHOLD) == LPC_2720_mV) || ((THRESHOLD) == LPC_2800_mV) || ((THRESHOLD) == LPC_2880_mV) || \
                                     ((THRESHOLD) == LPC_2960_mV) || ((THRESHOLD) == LPC_3040_mV) || ((THRESHOLD) == LPC_3120_mV) || \
                                     ((THRESHOLD) == LPC_3200_mV))

/** \defgroup   LPC_Interrupts_Definition   LPC Interrupts Definition
 * \{
 * \ingroup     LPC_Exported_constants
 */
#define LPC_INT_LPCOMP_AON                  (LPC_INT_LPCOMP_AON_EN_Msk)
#define LPC_INT_LPCOMP_NV                   (LPC_INT_LPCOMP_NV_EN_Msk)
#define LPC_INT_LPCOMP_CNT                  (LPC_INT_LPCOMP_CNT_EN_Msk)
/** \} */

#define IS_LPC_CONFIG_INT(INT) (((INT) == LPC_INT_LPCOMP_AON) || \
                                ((INT) == LPC_INT_LPCOMP_NV) || \
                                ((INT) == LPC_INT_LPCOMP_CNT))
#define IS_LPC_STATUS_INT(INT) (((INT) == LPC_INT_LPCOMP_AON) || \
                                ((INT) == LPC_INT_LPCOMP_CNT))
#define IS_LPC_CLEAR_INT(INT) ((INT) == LPC_INT_LPCOMP_CNT)

/** \defgroup   LPC_Flags_Definition    LPC Interrupts Definition
 * \{
 * \ingroup     LPC_Exported_constants
 */
#define LPC_FLAG_LPCOMP_AON                 (LPC_INT_LPCOMP_AON_EN_Msk)
#define LPC_FLAG_LPCOMP_CNT                 (LPC_INT_LPCOMP_CNT_EN_Msk)
/** \} */

#define IS_LPC_FLAG(FLAG) (((FLAG) == LPC_FLAG_LPCOMP_AON) || \
                           ((FLAG) == LPC_FLAG_LPCOMP_CNT))
#define IS_LPC_CLEAR_FLAG(FLAG) ((FLAG) == LPC_INT_LPCOMP_CNT)

/*============================================================================*
 *                         Functions
 *============================================================================*/

/**
 * \defgroup    LPC_Exported_Functions  Peripheral APIs
 * \{
 * \ingroup     LPC
 */

/**
 * \brief   Initializes LPC peripheral according to
 *          the specified parameters in LPC_InitStruct.
 * \param[in] LPC_InitStruct: Pointer to a LPC_InitTypeDef structure that contains
 *                            the configuration information for the specified LPC peripheral.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_lpc_init(void)
 * {
 *     LPC_InitTypeDef LPC_InitStruct;
 *
 *     LPC_StructInit(&LPC_InitStruct);
 *     LPC_InitStruct.LPC_Channel   = LPC_CAPTURE_CHANNEL;
 *     LPC_InitStruct.LPC_Edge      = LPC_VOLTAGE_DETECT_EDGE;
 *     LPC_InitStruct.LPC_Threshold = LPC_VOLTAGE_DETECT_THRESHOLD;
 *     LPC_Init(&LPC_InitStruct);
 *     LPC_Cmd(ENABLE);
 *
 *     LPC_ResetCounter();
 *     LPC_SetCompValue(LPC_COMP_VALUE);
 *     LPC_CounterCmd(ENABLE);
 *     LPC_INTConfig(LPC_INT_LPCOMP_CNT, ENABLE);
 *
 *     NVIC_InitTypeDef NVIC_InitStruct;
 *     NVIC_InitStruct.NVIC_IRQChannel = RTC_IRQn;
 *     NVIC_InitStruct.NVIC_IRQChannelPriority = 2;
 *     NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
 *     NVIC_Init(&NVIC_InitStruct);

 *     RTC_NvCmd(ENABLE);
 * }
 * \endcode
 */
void LPC_Init(LPC_InitTypeDef *LPC_InitStruct);

/**
 * \brief  Fills each LPC_InitStruct member with its default value.
 * \param[in]  LPC_InitStruct : Pointer to a LPC_InitTypeDef structure which will be initialized.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_lpc_init(void)
 * {
 *     LPC_InitTypeDef LPC_InitStruct;
 *
 *     LPC_StructInit(&LPC_InitStruct);
 *     LPC_InitStruct.LPC_Channel   = LPC_CAPTURE_CHANNEL;
 *     LPC_InitStruct.LPC_Edge      = LPC_VOLTAGE_DETECT_EDGE;
 *     LPC_InitStruct.LPC_Threshold = LPC_VOLTAGE_DETECT_THRESHOLD;
 *     LPC_Init(&LPC_InitStruct);
 * }
 * \endcode
 */
void LPC_StructInit(LPC_InitTypeDef *LPC_InitStruct);

/**
 * \brief  Enables or disables LPC peripheral.
 * \param[in]  NewState: New state of LPC peripheral.
 *      This parameter can be: ENABLE or DISABLE.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_lpc_init(void)
 * {
 *     LPC_InitTypeDef LPC_InitStruct;
 *
 *     LPC_StructInit(&LPC_InitStruct);
 *     LPC_InitStruct.LPC_Channel   = LPC_CAPTURE_CHANNEL;
 *     LPC_InitStruct.LPC_Edge      = LPC_VOLTAGE_DETECT_EDGE;
 *     LPC_InitStruct.LPC_Threshold = LPC_VOLTAGE_DETECT_THRESHOLD;
 *     LPC_Init(&LPC_InitStruct);
 *     LPC_Cmd(ENABLE);
 * }
 * \endcode
 */
void LPC_Cmd(FunctionalState NewState);

/**
 * \brief   Start or stop the LPC counter.
 * \param[in] NewState: New state of the LPC counter.
 *     This parameter can be one of the following values:
 *     \arg ENABLE: Start LPCOMP counter.
 *     \arg DISABLE: Stop LPCOMP counter.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_lpc_init(void)
 * {
 *     LPC_InitTypeDef LPC_InitStruct;
 *
 *     LPC_StructInit(&LPC_InitStruct);
 *     LPC_InitStruct.LPC_Channel   = LPC_CAPTURE_CHANNEL;
 *     LPC_InitStruct.LPC_Edge      = LPC_VOLTAGE_DETECT_EDGE;
 *     LPC_InitStruct.LPC_Threshold = LPC_VOLTAGE_DETECT_THRESHOLD;
 *     LPC_Init(&LPC_InitStruct);
 *     LPC_Cmd(ENABLE);
 *
 *     LPC_ResetCounter();
 *     LPC_SetCompValue(LPC_COMP_VALUE);
 *     LPC_CounterCmd(ENABLE);
 * }
 * \endcode
 */
void LPC_CounterCmd(FunctionalState NewState);

/**
 * \brief  Enables or disables the specified LPC interrupts.
 * \param[in] LPC_INT: Specifies the LPC interrupt source to be enabled or disabled.
 *      This parameter can be one of the following values:
 *      \arg LPC_INT_VOLTAGE_COMP: Voltage detection interrupt source.If Vin<Vth, cause this interrupt.
 *      \arg LPC_INT_COUNT_COMP: Couter comparator interrupt source.
 * \param[in]  NewState: New state of the specified LPC interrupt.
 *      This parameter can be: ENABLE or DISABLE.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_lpc_init(void)
 * {
 *     LPC_InitTypeDef LPC_InitStruct;
 *
 *     LPC_StructInit(&LPC_InitStruct);
 *     LPC_InitStruct.LPC_Channel   = LPC_CAPTURE_CHANNEL;
 *     LPC_InitStruct.LPC_Edge      = LPC_VOLTAGE_DETECT_EDGE;
 *     LPC_InitStruct.LPC_Threshold = LPC_VOLTAGE_DETECT_THRESHOLD;
 *     LPC_Init(&LPC_InitStruct);
 *     LPC_Cmd(ENABLE);
 *
 *     LPC_ResetCounter();
 *     LPC_SetCompValue(LPC_COMP_VALUE);
 *     LPC_CounterCmd(ENABLE);
 *     LPC_INTConfig(LPC_INT_LPCOMP_CNT, ENABLE);
 *
 *     NVIC_InitTypeDef NVIC_InitStruct;
 *     NVIC_InitStruct.NVIC_IRQChannel = RTC_IRQn;
 *     NVIC_InitStruct.NVIC_IRQChannelPriority = 2;
 *     NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
 *     NVIC_Init(&NVIC_InitStruct);

 *     RTC_NvCmd(ENABLE);
 * }
 * \endcode
 */
void LPC_INTConfig(uint32_t LPC_INT, FunctionalState NewState);

/**
 * \brief  Configure LPCOMP counter's comparator value.
 * \param[in]  value: LPCOMP counter's comparator value which can be 0 to 0xfff.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_lpc_init(void)
 * {
 *     LPC_InitTypeDef LPC_InitStruct;
 *
 *     LPC_StructInit(&LPC_InitStruct);
 *     LPC_InitStruct.LPC_Channel   = LPC_CAPTURE_CHANNEL;
 *     LPC_InitStruct.LPC_Edge      = LPC_VOLTAGE_DETECT_EDGE;
 *     LPC_InitStruct.LPC_Threshold = LPC_VOLTAGE_DETECT_THRESHOLD;
 *     LPC_Init(&LPC_InitStruct);
 *     LPC_Cmd(ENABLE);
 *
 *     LPC_ResetCounter();
 *     LPC_SetCompValue(LPC_COMP_VALUE);
 *     LPC_CounterCmd(ENABLE);
 * }
 * \endcode
 */
void LPC_SetCompValue(uint32_t value);

/**
 * \brief   Reset the LPC counter.
 * \param   None.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_lpc_init(void)
 * {
 *     LPC_InitTypeDef LPC_InitStruct;
 *
 *     LPC_StructInit(&LPC_InitStruct);
 *     LPC_InitStruct.LPC_Channel   = LPC_CAPTURE_CHANNEL;
 *     LPC_InitStruct.LPC_Edge      = LPC_VOLTAGE_DETECT_EDGE;
 *     LPC_InitStruct.LPC_Threshold = LPC_VOLTAGE_DETECT_THRESHOLD;
 *     LPC_Init(&LPC_InitStruct);
 *     LPC_Cmd(ENABLE);
 *
 *     LPC_ResetCounter();
 *     LPC_SetCompValue(LPC_COMP_VALUE);
 *     LPC_CounterCmd(ENABLE);
 * }
 * \endcode
 */
void LPC_ResetCounter(void);

/**
 * \brief  Checks whether the specified LPC interrupt is set or not.
 * \param[in]  LPC_INT: specifies the LPC interrupt to check.
 *   This parameter can be one of the following values:
 *     \arg LPC_INT_COUNT_COMP: couter comparator interrupt.
 * \return  The new state of SPI_IT (SET or RESET).
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * #define LPC_CAPTURE_PIN                 P2_4
 * #define LPC_CAPTURE_CHANNEL             LPC_CAPTURE_PIN - P2_0

 * #define LPC_VOLTAGE_DETECT_EDGE         LPC_Vin_Over_Vth;
 * #define LPC_VOLTAGE_DETECT_THRESHOLD    LPC_2400_mV;
 * #define LPC_COMP_VALUE                  0x0A//A LPC comparator interrupt occurs every ten times
 *
 * void board_lpc_init(void)
 * {
 *     Pad_Config(LPC_CAPTURE_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE,
 *                PAD_OUT_HIGH);
 *     Pinmux_Config(LPC_CAPTURE_PIN, IDLE_MODE);
 * }
 *
 * void driver_lpc_init(void)
 * {
 *     LPC_InitTypeDef LPC_InitStruct;
 *
 *     LPC_StructInit(&LPC_InitStruct);
 *     LPC_InitStruct.LPC_Channel   = LPC_CAPTURE_CHANNEL;
 *     LPC_InitStruct.LPC_Edge      = LPC_VOLTAGE_DETECT_EDGE;
 *     LPC_InitStruct.LPC_Threshold = LPC_VOLTAGE_DETECT_THRESHOLD;
 *     LPC_Init(&LPC_InitStruct);
 *     LPC_Cmd(ENABLE);
 *
 *     LPC_ResetCounter();
 *     LPC_SetCompValue(LPC_COMP_VALUE);
 *     LPC_CounterCmd(ENABLE);
 *     LPC_INTConfig(LPC_INT_LPCOMP_CNT, ENABLE);
 *
 *     NVIC_InitTypeDef NVIC_InitStruct;
 *     NVIC_InitStruct.NVIC_IRQChannel = RTC_IRQn;
 *     NVIC_InitStruct.NVIC_IRQChannelPriority = 2;
 *     NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
 *     NVIC_Init(&NVIC_InitStruct);

 *     RTC_NvCmd(ENABLE);
 * }
 *
 * void lpc_demo(void)
 * {
 *     board_lpc_init();
 *     driver_lpc_init();
 * }
 *
 * void RTC_Handler(void)
 * {
 *     if (LPC_GetINTStatus(LPC_INT_LPCOMP_CNT) == SET)
 *     {
 *         LPC_SetCompValue(LPC_GetCounter() + LPC_COMP_VALUE);
 *         LPC_ClearINTPendingBit(LPC_INT_LPCOMP_CNT);
 *     }
 * }
 * \endcode
 */
ITStatus LPC_GetINTStatus(uint32_t LPC_INT);

/**
 * \brief   Clear the specified LPC interrupt pending bit.
 * \param[in] LPC_INT: specifies the LPC interrupt to clear.
 *      This parameter can be one of the following values:
 *      \arg LPC_INT_COUNT_COMP: Couter comparator interrupt.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void lpc_demo(void)
 * {
 *     LPC_ClearINTPendingBit(LPC_INT_COUNT_COMP);
 * }
 * \endcode
 */
void LPC_ClearINTPendingBit(uint32_t LPC_INT);

/**
 * \brief   Read LPCOMP comparator value.
 * \param   None.
 * \return  LPCOMP comparator value.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void lpc_demo(void)
 * {
 *     uint16_t value = LPC_GetCompValue();
 * }
 * \endcode
 */
__STATIC_INLINE uint16_t LPC_GetCompValue(void)
{
    return ((LPC->LPC_CMP) & 0xFFF);
}

/**
 * \brief   Read LPC counter value.
 * \return  LPCOMP counter value.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void lpc_demo(void)
 * {
 *     uint16_t counter = LPC_GetCounter();
 * }
 * \endcode
 */
__STATIC_INLINE uint16_t LPC_GetCounter(void)
{
    return ((LPC->LPC_CNT) & 0xFFF);
}

/** \} */ /* End of group LPC_Exported_Functions */

#ifdef __cplusplus
}
#endif

#endif /* _RTL876X_LPC_H_ */



/******************* (C) COPYRIGHT 2020 Realtek Semiconductor Corporation *****END OF FILE****/

