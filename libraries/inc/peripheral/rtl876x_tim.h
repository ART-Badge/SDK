/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file     rtl876x_tim.h
* \brief    The header file of the peripheral TIMER driver.
* \details  This file provides all TIMER firmware functions.
* \author   elliot chen
* \date     2015-05-15
* \version  v1.0
* *********************************************************************************************************
*/

#ifndef _RTL876X_TIM_H_
#define _RTL876X_TIM_H_

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \addtogroup  IO          Peripheral Drivers
 * \defgroup    TIM         TIM
 * \brief       Manage the TIM peripheral functions.
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
 * \defgroup    TIM_Exported_Types Init Params Struct
 *
 * \ingroup     TIM
 */

/**
 * \brief       TIM init structure definition
 *
 * \ingroup     TIM_Exported_Types
 */
typedef struct
{
    uint16_t TIM_ClockSrc;          /*!< <b>Deprecated</b> use RCC instead.*/
    uint16_t TIM_DIV;               /*!< Clock fix at 40Mhz,add this parameter div the clock*/
    uint16_t TIM_SOURCE_DIV;        /*!< Specifies the clock source div.
                                            This parameter can be a value of \ref TIM_Clock_Divider*/
    uint16_t TIM_SOURCE_DIV_En;     /*!< Timer source clock div enable. */
    uint16_t TIM_Mode;              /*!< Specifies the counter mode.
                                            This parameter can be a value of \ref TIM_Mode. */
    uint16_t TIM_PWM_En;            /*!< Specifies the PWM mode.
                                            This parameter can be a value of DISABLE or ENABLE */
    uint32_t TIM_Period;            /*!< Specifies the period value to be loaded into the active
                                            Auto-Reload Register at the next update event.
                                            This parameter must range from 0x0000 to 0xFFFFFFFF. */
    uint32_t TIM_PWM_High_Count;    /*!< Specifies the PWM High Count.
                                            This parameter must range from 0x0000 to 0xFFFFFFFF. */
    uint32_t TIM_PWM_Low_Count;     /*!< Specifies the PWM Low Count.
                                            This parameter must range from 0x0000 to 0xFFFFFFFF. */
    uint32_t TIM_EventMode;         /*!< Specifies the TIM event mode. */
    uint32_t TIM_EventIndex;        /*!< Specifies the TIM event index. */
    uint32_t TIM_EventDuration;     /*!< Specifies the TIM event duration.*/
    uint8_t  ClockDepend;           /*!< Specifies TIM Source depend.timer3 depend timer2 ,timer5 depend timer4, timer7 depend timer6.
                                            This parameter can be a value of ENABLE or DISABLE */
    uint32_t PWM_Deazone_Size;      /*!<Size of deadzone time, DeadzoneTime=deadzonesize/32000 or 32768.
                                            This parameter must range from 1 to 0xffffffff. */
    uint16_t PWMDeadZone_En;        /*!<PWM Deadzone enable, pwm0_pn: timer2, pwm1_pn:timer3
                                        This parameter can be a value of ENABLE or DISABLE. */
    uint16_t PWM_Stop_State_P;      /*!< Specifies the PWM P stop state.
                                            This parameter can be a value of \ref PWMDeadZone_Stop_state. */
    uint16_t PWM_Stop_State_N;      /*!< Specifies the PWM N stop state.
                                            This parameter can be a value of \ref PWMDeadZone_Stop_state. */
} TIM_TimeBaseInitTypeDef;


/*============================================================================*
 *                         Constants
 *============================================================================*/

/* TIM private defines */
#define TIMER0_LOAD_COUNT2      *((volatile uint32_t *)0x400020b0UL)
#define TIMER1_LOAD_COUNT2      *((volatile uint32_t *)0x400020b4UL)
#define TIMER2_LOAD_COUNT2      *((volatile uint32_t *)0x400020b8UL)
#define TIMER3_LOAD_COUNT2      *((volatile uint32_t *)0x400020bCUL)
#define TIMER4_LOAD_COUNT2      *((volatile uint32_t *)0x400020C0UL)
#define TIMER5_LOAD_COUNT2      *((volatile uint32_t *)0x400020c4UL)
#define TIMER6_LOAD_COUNT2      *((volatile uint32_t *)0x400020c8UL)
#define TIMER7_LOAD_COUNT2      *((volatile uint32_t *)0x400020ccUL)

#define TIMER_PWM0_CR           *((volatile uint32_t *)0x40000364UL)
#define TIMER_PWM1_CR           *((volatile uint32_t *)0x40000368UL)



/**
 * \defgroup    TIM_Exported_constants  Macro Definitions
 *
 * \ingroup     TIM
 */

#define IS_TIM_ALL_PERIPH(PERIPH) (((PERIPH) == TIM0) || \
                                   ((PERIPH) == TIM1) || \
                                   ((PERIPH) == TIM2) || \
                                   ((PERIPH) == TIM3) || \
                                   ((PERIPH) == TIM4) || \
                                   ((PERIPH) == TIM5) || \
                                   ((PERIPH) == TIM6) || \
                                   ((PERIPH) == TIM7))

/**
 * \def     PWMDeadZone_Peripheral  PWMDeadZone
 */

#define IS_PWM_ALL_PERIPH(PERIPH) (((PERIPH) == PWM2) || \
                                   ((PERIPH) == PWM3))

/**
 * \defgroup    TIM_Clock_Divider TIM Clock Divider
 * \{
 * \ingroup     TIM_Exported_constants
 */
#define TIM_CLOCK_DIVIDER_1                           ((uint16_t)0x0)
#define TIM_CLOCK_DIVIDER_2                           ((uint16_t)0x4)
#define TIM_CLOCK_DIVIDER_4                           ((uint16_t)0x5)
#define TIM_CLOCK_DIVIDER_8                           ((uint16_t)0x6)
#define TIM_CLOCK_DIVIDER_40                          ((uint16_t)0x7)
/** \} */
#define IS_TIM_SOURCE_DIVIDER(DIV)              (((DIV) == TIM_CLOCK_DIVIDER_1) || \
                                                 ((DIV) == TIM_CLOCK_DIVIDER_2) || \
                                                 ((DIV) == TIM_CLOCK_DIVIDER_4) || \
                                                 ((DIV) == TIM_CLOCK_DIVIDER_8) ||\
                                                 ((DIV) == TIM_CLOCK_DIVIDER_40))

/**
 * \defgroup    TIM_Mode TIM Mode
 * \{
 * \ingroup     TIM_Exported_constants
 */
#define TIM_Mode_FreeRun                    ((uint16_t)0x0000)
#define TIM_Mode_UserDefine                 ((uint16_t)0x0001)
/** \} */
#define IS_TIM_MODE(mode) (((mode) == TIM_Mode_FreeRun) || \
                           ((mode) == TIM_Mode_UserDefine))

/**
 * \defgroup    TIM_PWM_En TIM PWM Mode Enable
 * \{
 * \ingroup     TIM_Exported_constants
 */
#define PWM_ENABLE                        ((uint16_t)0x1)
#define PWM_DISABLE                       ((uint16_t)0x0)
/** \} */
#define IS_TIM_PWM_En(mode) (((mode) == PWM_ENABLE) || \
                             ((mode) == PWM_DISABLE))

/**
 * \defgroup    TIM_Event_Duration TIM Event Duration
 * \{
 * \ingroup     TIM_Exported_constants
 */
#define TIM_EventDuration_32us                    ((uint16_t)0x0000)
#define TIM_EventDuration_64us                    ((uint16_t)0x0001)
#define TIM_EventDuration_128us                   ((uint16_t)0x0002)
#define TIM_EventDuration_256us                   ((uint16_t)0x0003)
/** \} */

#define IS_TIM_Event_DURATION(duration) (((duration) == TIM_EventDuration_32us) || \
                                         ((duration) == TIM_EventDuration_64us) || \
                                         ((duration) == TIM_EventDuration_128us) || \
                                         ((duration) == TIM_EventDuration_256us))

/**
 * \defgroup    PWMDeadZone_En PWM DeadZone enable
 * \{
 * \ingroup     TIM_Exported_constants
 */
#define DEADZONE_ENABLE                        ((uint16_t)0x1)
#define DEADZONE_DISABLE                       ((uint16_t)0x0)
/** \} */
#define IS_TIM_PWM_DeadZone_En(mode) (((mode) == DEADZONE_ENABLE) || \
                                      ((mode) == DEADZONE_DISABLE))

/**
 * \defgroup    PWMDeadZone_Stop_State PWM DeadZone Stop State
 * \{
 * \ingroup     TIM_Exported_constants
 */
#define PWM_STOP_AT_HIGH                        ((uint16_t)0x1)
#define PWM_STOP_AT_LOW                         ((uint16_t)0x0)
/** \} */

/*============================================================================*
 *                         Functions
 *============================================================================*/

/**
 * \defgroup    TIM_Exported_Functions  Peripheral APIs
 * \{
 * \ingroup     TIM
 */

/**
 * \brief   Deinitialize the TIMx peripheral registers to their default reset values.
 * \return  None.
 */
void TIM_DeInit(void);

/**
 * \brief   Initialize the TIMx time base unit peripheral according to
 *          the specified parameters in TIM_TimeBaseInitStruct.
 * \param[in] TIMx: where x can be 0 to 7 to select the TIM peripheral.
 * \param[in] TIM_TimeBaseInitStruct: Pointer to a TIM_TimeBaseInitTypeDef
 *            structure that contains the configuration information for the selected TIM peripheral.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_timer_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_TIMER, APBPeriph_TIMER_CLOCK, ENABLE);
 *
 *     TIM_TimeBaseInitTypeDef TIM_InitStruct;
 *     TIM_StructInit(&TIM_InitStruct);
 *
 *     TIM_InitStruct.TIM_PWM_En = PWM_DISABLE;
 *     TIM_InitStruct.TIM_Period = 1000000 - 1 ;
 *     TIM_InitStruct.TIM_Mode = TIM_Mode_UserDefine;
 *     TIM_TimeBaseInit(TIMER_NUM, &TIM_InitStruct);
 * }
 * \endcode
 */
void TIM_TimeBaseInit(TIM_TypeDef *TIMx, TIM_TimeBaseInitTypeDef *TIM_TimeBaseInitStruct);

/**
 * \brief   Fills each TIM_InitStruct member with its default value.
 * \param[in] TIM_TimeBaseInitStruct: Pointer to a TIM_TimeBaseInitTypeDef structure which will be initialized.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_timer_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_TIMER, APBPeriph_TIMER_CLOCK, ENABLE);
 *
 *     TIM_TimeBaseInitTypeDef TIM_InitStruct;
 *     TIM_StructInit(&TIM_InitStruct);
 *
 *     TIM_InitStruct.TIM_PWM_En = PWM_DISABLE;
 *     TIM_InitStruct.TIM_Period = 1000000 - 1;
 *     TIM_InitStruct.TIM_Mode = TIM_Mode_UserDefine;
 *     TIM_TimeBaseInit(TIM6, &TIM_InitStruct);
 * }
 * \endcode
 */
void TIM_StructInit(TIM_TimeBaseInitTypeDef *TIM_TimeBaseInitStruct);

/**
 * \brief   Enables or disables the specified TIM peripheral.
 * \param[in] TIMx: Where x can be 0 to 7 to select the TIMx peripheral.
 * \param[in] NewState: New state of the TIMx peripheral.
 *      This parameter can be: ENABLE or DISABLE.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_timer_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_TIMER, APBPeriph_TIMER_CLOCK, ENABLE);
 *
 *     TIM_TimeBaseInitTypeDef TIM_InitStruct;
 *     TIM_StructInit(&TIM_InitStruct);
 *
 *     TIM_InitStruct.TIM_PWM_En = PWM_DISABLE;
 *     TIM_InitStruct.TIM_Period = 1000000 - 1;
 *     TIM_InitStruct.TIM_Mode = TIM_Mode_UserDefine;
 *     TIM_TimeBaseInit(TIM6, &TIM_InitStruct);
 *     TIM_Cmd(TIM6, ENABLE);
 * }
 * \endcode
 */
void TIM_Cmd(TIM_TypeDef *TIMx, FunctionalState NewState);

/**
 * \brief   Enables or disables the specified TIMx interrupt.
 * \param[in]   TIMx: Where x can be 0 to 7 to select the TIMx peripheral.
 * \param[in]   NewState: New state of the TIMx peripheral.
 *      This parameter can be: ENABLE or DISABLE.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_timer_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_TIMER, APBPeriph_TIMER_CLOCK, ENABLE);
 *
 *     TIM_TimeBaseInitTypeDef TIM_InitStruct;
 *     TIM_StructInit(&TIM_InitStruct);
 *
 *     TIM_InitStruct.TIM_PWM_En = PWM_DISABLE;
 *     TIM_InitStruct.TIM_Period = 1000000 - 1;
 *     TIM_InitStruct.TIM_Mode = TIM_Mode_UserDefine;
 *     TIM_TimeBaseInit(TIM6, &TIM_InitStruct);
 *     TIM_ClearINT(TIM6);
 *     TIM_INTConfig(TIM6, ENABLE);
 */
void TIM_INTConfig(TIM_TypeDef *TIMx, FunctionalState NewState);

/**
 * \brief   Change TIM period value.
 * \param[in]   TIMx: Where x can be 0 to 7 to select the TIMx peripheral.
 * \param[in]   period: Period value to be changed.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void timer_demo(void)
 * {
 *     uint32_t new_period = 1000000 - 1;
 *     TIM_Cmd(TIM6, DISABLE);
 *     TIM_ChangePeriod(TIM6, new_period);
 *
 * }
 * \endcode
 */
void TIM_ChangePeriod(TIM_TypeDef *TIMx, uint32_t period);

/**
 * \brief   Change PWM freq and duty according high_cnt and low_cnt
 * \param[in]   TIMx: Where x can be 2 to 3 to select the TIMx peripheral.
 * \param[in]   high_count: This parameter can be 0x00~0xFFFFFFFF.
 * \param[in]  low_count: This parameter can be 0x00~0xFFFFFFFF.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void timer_demo(void)
 * {
 *     uint32_t high_count = 1000000 - 1;
 *     uint32_t low_count = 1000000 - 1;
 *     TIM_Cmd(TIM6, DISABLE);
 *     TIM_ChangePeriod(TIM6, high_count, low_count);
 * }
 * \endcode
 */
void TIM_PWMChangeFreqAndDuty(TIM_TypeDef *TIMx, uint32_t high_count, uint32_t low_count);

/**
 * \brief   Get TIMx current value when timer is running.
 * \param[in]   TIMx: where x can be 0 to 7 to select the TIMx peripheral.
 * \return  The counter value.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void timer_demo(void)
 * {
 *     uint32_t cur_value = TIM_GetCurrentValue(TIM6);
 * }
 * \endcode
 */
__STATIC_INLINE uint32_t TIM_GetCurrentValue(TIM_TypeDef *TIMx)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    return TIMx->CurrentValue;
}

/**
 * \brief   Check whether the TIM interrupt has occurred or not.
 * \param[in]   TIMx: Where x can be 0 to 7 to select the TIMx peripheral.
 * \return  The new state of the TIM_IT(SET or RESET).
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void timer_demo(void)
 * {
 *     ITStatus int_status = TIM_GetINTStatus(TIM6);
 * }
 * \endcode
 */
__STATIC_INLINE ITStatus TIM_GetINTStatus(TIM_TypeDef *TIMx)
{
    ITStatus bitstatus = RESET;
    uint16_t itstatus = (uint16_t)TIMx->IntStatus;

    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    if (itstatus != (uint16_t)RESET)
    {
        bitstatus = SET;
    }

    return bitstatus;
}

/**
 * \brief   Clear TIM interrupt.
 * \param[in]   TIMx: Where x can be 0 to 7 to select the TIMx peripheral.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void timer_demo(void)
 * {
 *     TIM_ClearINT(TIM6);
 * }
 * \endcode
 */
__STATIC_INLINE void TIM_ClearINT(TIM_TypeDef *TIMx)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    /* Clear the IT */
    TIMx->EOI;
}

/**
 * \brief   Check whether the TIM is in operation or not.
 * \param[in]   TIMx: Where x can be 0 to 7 to select the TIMx peripheral.
 * \return  The new state of the timer operation status (SET or RESET).
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void timer_demo(void)
 * {
 *     ITStatus intstatus = TIM_GetOperationStatus(TIM6);
 * }
 * \endcode
 */
__STATIC_INLINE ITStatus TIM_GetOperationStatus(TIM_TypeDef *TIMx)
{
    ITStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    if (TIMx->ControlReg & BIT(0))
    {
        bitstatus = SET;
    }

    return bitstatus;
}

/**
 * \brief   PWM complementary output emergency stop and resume.
 *          PWM_P emergency stop level state is configured by PWM_Stop_State_P,
 *          PWM_N emergency stop level state is configured by PWM_Stop_State_N.
 * \param[in] PWMx: Where x can be 2 to 3 to select the PWMx peripheral.
 * \param[in] NewState: New state of complementary output.
 *      \ref DISABLE: Resume PWM complementary output.
 *      \ref ENABLE: PWM complementary output emergency stop.
 * \note    To use this function, need to configure the corresponding timer.
 *          PWM2 ->> TIM2, PWM3 ->> TIM3.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void board_pwm_init(void)
 * {
 *     Pad_Config(P0_1, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_HIGH);
 *     Pad_Config(P0_2, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE,
 *                PAD_OUT_HIGH);
 *     Pad_Config(P2_2, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE,
 *                PAD_OUT_HIGH);
 *
 *     Pinmux_Config(P0_1, PWM_OUT_PIN_PINMUX);
 *     Pinmux_Config(P0_2, PWM_OUT_P_PIN_PINMUX);
 *     Pinmux_Config(P2_2, PWM_OUT_N_PIN_PINMUX);
 * }
 *
 * void driver_pwm_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_TIMER, APBPeriph_TIMER_CLOCK, ENABLE);
 *
 *     TIM_TimeBaseInitTypeDef TIM_InitStruct;
 *     TIM_StructInit(&TIM_InitStruct);
 *     TIM_InitStruct.TIM_Mode             = TIM_Mode_UserDefine;
 *     TIM_InitStruct.TIM_PWM_En           = PWM_ENABLE;
 *     TIM_InitStruct.TIM_PWM_High_Count   = PWM_HIGH_COUNT;
 *     TIM_InitStruct.TIM_PWM_Low_Count    = PWM_LOW_COUNT;
 *     TIM_InitStruct.PWM_Stop_State_P     = PWM_STOP_AT_HIGH;
 *     TIM_InitStruct.PWM_Stop_State_N     = PWM_STOP_AT_LOW;
 *     TIM_InitStruct.PWMDeadZone_En       = DEADZONE_ENABLE;  //enable to use pwn p/n output
 *     TIM_InitStruct.PWM_Deazone_Size     = 255;
 *     TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
 *
 *     TIM_Cmd(TIM2, ENABLE);
 * }
 *
 * void pwm_demo(void)
 * {
 *    board_pwm_init();
 *    driver_pwm_init();
 *    //Add delay.
 *    TIM_PWMComplOutputEMCmd(PWM2,ENABLE);
 * }
 * \endcode
 */
void TIM_PWMComplOutputEMCmd(PWM_TypeDef *PWMx, FunctionalState NewState);

/**
 * \brief   Enable or disable bypass dead zone function of PWM complementary output.
 *          After enabling, PWM_P = ~PWM_N.
 * \param[in] PWMx: Where x can be 2 to 3 to select the PWMx peripheral.
 * \param[in] NewState: New state of the PWMx peripheral.
 *      \ref DISABLE: Disable bypass dead zone function.
 *      \ref ENABLE: Enable bypass dead zone function.
 * \return  None.
 * \note    To use this function, need to configure the corresponding timer.
 *          PWM2 ->> TIM2, PWM3 ->> TIM3.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_pwm_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_TIMER, APBPeriph_TIMER_CLOCK, ENABLE);
 *
 *     TIM_TimeBaseInitTypeDef TIM_InitStruct;
 *     TIM_StructInit(&TIM_InitStruct);
 *     TIM_InitStruct.TIM_Mode             = TIM_Mode_UserDefine;
 *     TIM_InitStruct.TIM_PWM_En           = PWM_ENABLE;
 *     TIM_InitStruct.TIM_PWM_High_Count   = PWM_HIGH_COUNT;
 *     TIM_InitStruct.TIM_PWM_Low_Count    = PWM_LOW_COUNT;
 *     TIM_InitStruct.PWM_Stop_State_P     = PWM_STOP_AT_HIGH;
 *     TIM_InitStruct.PWM_Stop_State_N     = PWM_STOP_AT_LOW;
 *     TIM_InitStruct.PWMDeadZone_En       = DEADZONE_ENABLE;  //enable to use pwn p/n output
 *     TIM_InitStruct.PWM_Deazone_Size     = 255;
 *     TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
 *
 *     TIM_Cmd(TIM2, ENABLE);
 *     TIM_PWMDZBypassCmd(PWM2, ENABLE);
 * }
 * \endcode
 */
void TIM_PWMDZBypassCmd(PWM_TypeDef *PWMx, FunctionalState NewState);

/**
 * \brief   Change the PWM dead time clock source.
 * \param[in] PWMx: Where x can be 2 to 3 to select the PWMx peripheral.
 * \param[in] NewState: New state of the PWMx peripheral.
 *      \ref DISABLE: Use 32k clock source.
 *      \ref ENABLE: Use 5M clock source.
 * \return  None.
 * \note    To use this function, need to configure the corresponding timer.
 *          PWM2 ->> TIM2, PWM3 ->> TIM3.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_pwm_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_TIMER, APBPeriph_TIMER_CLOCK, ENABLE);
 *     //Open 5M clock source.
 *     RCC_ClockSrc5MCmd();
 *
 *     TIM_TimeBaseInitTypeDef TIM_InitStruct;
 *
 *     TIM_StructInit(&TIM_InitStruct);
 *     TIM_InitStruct.TIM_Mode             = TIM_Mode_UserDefine;
 *     TIM_InitStruct.TIM_PWM_En           = PWM_ENABLE;
 *     TIM_InitStruct.TIM_PWM_High_Count   = PWM_HIGH_COUNT;
 *     TIM_InitStruct.TIM_PWM_Low_Count    = PWM_LOW_COUNT;
 *     TIM_InitStruct.PWM_Stop_State_P     = PWM_STOP_AT_HIGH;
 *     TIM_InitStruct.PWM_Stop_State_N     = PWM_STOP_AT_LOW;
 *     TIM_InitStruct.PWMDeadZone_En       = DEADZONE_ENABLE;  //enable to use pwn p/n output
 *     TIM_InitStruct.PWM_Deazone_Size     = PWM_DEAD_ZONE_SIZE;
 *     TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
 *
 *     //Use 5M clock source.
 *     TIM_PWMChangeDZClockSrc(PWM2,ENABLE);
 *
 *     TIM_Cmd(TIM2, ENABLE);
 * }
 * \endcode
 */
void TIM_PWMChangeDZClockSrc(PWM_TypeDef *PWMx, FunctionalState NewState);

/**
 * \brief   PWM complementary output emergency stop.
 * \param[in] PWMx: PWM2 or PWM3.
 * \return  None.
 */
__STATIC_INLINE void PWM_Deadzone_EMStop(PWM_TypeDef *PWMx)
{
    /* Check the parameters. */
    assert_param(IS_PWM_ALL_PERIPH(PWMx));

    PWMx->CR |= BIT(8);
}

/**
  * @brief  Get PWM current output status.
  * @param  TIMx: where x can be 0 to 7 to select the TIMx peripheral.
  * @retval The output state of the timer(SET: High or RESET: Low).
  */
__STATIC_INLINE FlagStatus TIM_GetPWMOutputStatus(TIM_TypeDef *TIMx)
{
    uint32_t timer_index = 0;
    uint32_t reg_value = 0;
    ITStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    timer_index = ((uint32_t)TIMx - TIM0_REG_BASE) / (TIM1_REG_BASE - TIM0_REG_BASE);
    reg_value = *((volatile uint32_t *)0x40006024UL);

    if ((reg_value >> timer_index) & BIT(0))
    {
        bitstatus = SET;
    }

    return bitstatus;
}
/** \} */ /* End of group TIM_Exported_Functions */

#ifdef __cplusplus
}
#endif

#endif /*_RTL876X_TIM_H_*/


/******************* (C) COPYRIGHT 2015 Realtek Semiconductor Corporation *****END OF FILE****/
