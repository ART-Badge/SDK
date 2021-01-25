/**
**********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_tim.c
* @brief    This file provides all the Timer firmware functions.
* @details
* @author
* @date
* @version  v0.1
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtl876x_tim.h"
#include "rtl876x_rcc.h"

/* TIM Private Defines */
#define TIMER_CLK_SOURCE_REG_348 *((volatile uint32_t *)0x40000348UL)
#define TIMER_CLK_SOURCE_REG_360 (*((volatile uint32_t *)0x40000360UL))
#define TIMER_CLK_SOURCE_REG_35C *((volatile uint32_t *)0x4000035CUL)

/**
  * @brief  Deinitializes the TIMx peripheral registers to their default reset values.
  * @param  TIMx: where x can be 0 to 7 to select the TIM peripheral.
  * @retval None
  */
void TIM_DeInit()
{
    /*enable timer IP clock and function */
    RCC_PeriphClockCmd(APBPeriph_TIMER, APBPeriph_TIMER_CLOCK, DISABLE);
}

/**
  * @brief  Initializes the TIMx Time Base Unit peripheral according to
  *         the specified parameters in the TIM_TimeBaseInitStruct.
  * @param  TIMx: where x can be 0 to 7 to select the TIM peripheral.
  * @param  TIM_TimeBaseInitStruct: pointer to a TIM_TimeBaseInitTypeDef
  *         structure that contains the configuration information for the
  *         specified TIM peripheral.
  * @retval None
  */
void TIM_TimeBaseInit(TIM_TypeDef *TIMx, TIM_TimeBaseInitTypeDef *TIM_TimeBaseInitStruct)
{
    uint32_t timerid = 0;
    uint32_t tempreg = 0;
    volatile uint32_t *count2_address;

    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_MODE(TIM_TimeBaseInitStruct->TIM_Mode));
    assert_param(IS_TIM_PWM_DeadZone_En(TIM_TimeBaseInitStruct->PWMDeadZone_En));

    /* Select clock source which can be system clock or 40 MHz  or pll*/
    tempreg = (uint32_t)TIMx;
    timerid = (tempreg - TIM0_REG_BASE) / 20;

    /*div the clock source,actually it need enable TIM_SOURCE_CLOCK_DIV_EN*/
    TIMER_CLK_SOURCE_REG_360 |= BIT9;

    if (TIMER_CLK_SOURCE_REG_360 & BIT(10))
    {
        TIMER_CLK_SOURCE_REG_360 &= ~(BIT(10));
    }

    if (timerid < 2)
    {
        TIMER_CLK_SOURCE_REG_360 &= ~(0x7 << (timerid * 3));
        TIMER_CLK_SOURCE_REG_360 |= ((TIM_TimeBaseInitStruct->TIM_SOURCE_DIV) << (timerid * 3));
    }
    else if ((timerid >= 2) && (timerid < 8))
    {
        TIMER_CLK_SOURCE_REG_360 &= ~(0x7 << (((timerid - 2) * 3) + 13));
        TIMER_CLK_SOURCE_REG_360 |= ((TIM_TimeBaseInitStruct->TIM_SOURCE_DIV) << (((
                                                                                       timerid - 2) * 3) + 13));
    }

    if (timerid == 3 || timerid == 5 || timerid == 7)
    {
        if (TIM_TimeBaseInitStruct->ClockDepend == true)
        {
            TIMER_CLK_SOURCE_REG_35C |= BIT(((timerid - 1) >> 1) - 1);
        }
        else
        {
            TIMER_CLK_SOURCE_REG_35C &= ~BIT(((timerid - 1) >> 1) - 1);
        }
    }

    /* set timer mode and mask interrupt */
    if (TIM_TimeBaseInitStruct->TIM_PWM_En == PWM_DISABLE)
    {
        TIMx->ControlReg = (TIM_TimeBaseInitStruct->TIM_Mode << 1) | BIT(2);
        /* set timer period */
        TIMx->LoadCount = TIM_TimeBaseInitStruct->TIM_Period;
    }
    else
    {
        TIMx->ControlReg = (TIM_TimeBaseInitStruct->TIM_Mode << 1) | BIT(2) | BIT(3);
        count2_address = &TIMER0_LOAD_COUNT2;
        count2_address = count2_address + timerid;
        *count2_address = TIM_TimeBaseInitStruct->TIM_PWM_High_Count ;
        /* set timer period */
        TIMx->LoadCount = TIM_TimeBaseInitStruct->TIM_PWM_Low_Count;
    }

    /* set pwm deadzone mode, pwm0_pn based on timer2, and pwm1_pn based on timer3 */
    if (TIM_TimeBaseInitStruct->PWMDeadZone_En == ENABLE)
    {
        if (timerid  == 2)
        {
            /* set pwm deadzone time */
            TIMER_PWM0_CR = ((TIM_TimeBaseInitStruct->PWM_Deazone_Size) \
                             | (TIM_TimeBaseInitStruct->PWM_Stop_State_N << 9) \
                             | (TIM_TimeBaseInitStruct->PWM_Stop_State_P << 10) \
                             | BIT12 | BIT17 | BIT18);
        }
        else if (timerid == 3)
        {
            /* set pwm deadzone time */
            TIMER_PWM1_CR = ((TIM_TimeBaseInitStruct->PWM_Deazone_Size) \
                             | (TIM_TimeBaseInitStruct->PWM_Stop_State_N << 9) \
                             | (TIM_TimeBaseInitStruct->PWM_Stop_State_P << 10) \
                             | BIT12 | BIT17 | BIT18);
        }
    }
    else
    {
        if (timerid == 2)
        {
            /*disable pwm0 deadzone mode*/
            TIMER_PWM0_CR &= ~(BIT12 | BIT17 | BIT18);
        }
        else if (timerid == 3)
        {
            /*disable pwm1 deadzone mode*/
            TIMER_PWM1_CR &= ~(BIT12 | BIT17 | BIT18);
        }
    }

    /* Clear the IT status */
    TIMx->EOI;
}

/**
  * @brief  Fills each TIM_InitStruct member with its default value.
  * @param  TIM_InitStruct : pointer to a TIM_InitTypeDef structure which will be initialized.
  * @retval None
  */
void TIM_StructInit(TIM_TimeBaseInitTypeDef *TIM_TimeBaseInitStruct)
{
    TIM_TimeBaseInitStruct->TIM_Mode = TIM_Mode_UserDefine;
    TIM_TimeBaseInitStruct->TIM_Period = 0xfff;
    TIM_TimeBaseInitStruct->TIM_SOURCE_DIV = TIM_CLOCK_DIVIDER_1;
    TIM_TimeBaseInitStruct->ClockDepend = false;
    TIM_TimeBaseInitStruct->TIM_PWM_En = PWM_DISABLE;
    TIM_TimeBaseInitStruct->PWM_Stop_State_P = PWM_STOP_AT_LOW;
    TIM_TimeBaseInitStruct->PWM_Stop_State_N = PWM_STOP_AT_HIGH;
    TIM_TimeBaseInitStruct->PWMDeadZone_En = DEADZONE_DISABLE;
}

/**
  * @brief  Enables or disables the specified TIM peripheral.
  * @param  TIMx: where x can be 0 to 7 to select the TIMx peripheral.
  * @param  NewState: new state of the TIMx peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void TIM_Cmd(TIM_TypeDef *TIMx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the TIM Counter */
        TIMx->ControlReg |= BIT(0);
    }
    else
    {
        /* Disable the TIM Counter */
        TIMx->ControlReg &= ~(BIT(0));
    }
}

/**
  * @brief  change TIM period value.
  * @param  TIMx: where x can be 0 to 7 to select the TIMx peripheral.
  * @retval The new state of success or not  (SET or RESET).
  */
void TIM_ChangePeriod(TIM_TypeDef *TIMx, uint32_t period)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    TIMx->LoadCount = period;

    return;
}

/**
  * @brief  Enables or disables the specified TIMx interrupt.
  * @param  TIMx: where x can be 0 to 7 to select the TIMx peripheral.
  * @param  NewState: new state of the TIMx peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void TIM_INTConfig(TIM_TypeDef *TIMx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    if (NewState != DISABLE)
    {
        /* Enable the Interrupt sources */
        TIMx->ControlReg &= ~(BIT(2));
    }
    else
    {
        /* Disable the Interrupt sources */
        TIMx->ControlReg |= BIT(2);
    }
}

/**
  * @brief  Change PWM freq and duty according high_cnt and low_cnt
  * @param  TIMx: where x can be 0 to 7 to select the TIMx peripheral.
  * @param  high_count:
    *   This parameter can be:
    * @param  low_count:
  *   This parameter can be:
  * @retval None
  */
void TIM_PWMChangeFreqAndDuty(TIM_TypeDef *TIMx, uint32_t high_count, uint32_t low_count)
{
    volatile uint32_t *count2_address;
    uint32_t timerid = 0;
    uint32_t tempreg = 0;
    tempreg = (uint32_t)TIMx;
    timerid = (tempreg - TIM0_REG_BASE) / 20;
    TIMx->LoadCount = low_count;
    count2_address = &TIMER0_LOAD_COUNT2;
    count2_address = count2_address + timerid;
    *count2_address = high_count;

}

/**
 * \brief   PWM complementary output emergency stop.
 *          PWM_P emergency stop level state is configured by PWM_Stop_State_P,
 *          PWM_N emergency stop level state is configured by PWM_Stop_State_N.
 * \param[in] PWMx: Where x can be 2 to 3 to select the PWMx peripheral.
 * \param[in] NewState: New state of complementary output.
 *      \ref DISABLE: Resume PWM complementary output.
 *      \ref ENABLE: PWM complementary output emergency stop.
 * \return  None.
 * \note    To use this function, need to configure the corresponding timer.
 *          PWM2 ->> TIM2, PWM3 ->> TIM3.
 */
void TIM_PWMComplOutputEMCmd(PWM_TypeDef *PWMx, FunctionalState NewState)
{
    /* Check the parameters. */
    assert_param(IS_PWM_ALL_PERIPH(PWMx));

    if (NewState != DISABLE)
    {
        /* PWM complementary output emergency stop. */
        PWMx->CR |= BIT8;
    }
    else
    {
        /* Resume PWM complementary output. */
        PWMx->CR &= (~BIT8);
    }
}

/**
 * \brief   Enable or disable bypass dead zone function of PWM complementary output.
 *          After enabling, PWM_P = ~PWM_N.
 * \param[in] PWMx: Where x can be 2 to 3 to select the PWMx peripheral.
 * \param[in] NewState: New state of the PWMx peripheral.
 *      \ref DISABLE: Disable bypass dead zone function.
 *      \ref ENABLE: Enable bypass dead zone function.
 * \note    To use this function, need to configure the corresponding timer.
 *          PWM2 ->> TIM2, PWM3 ->> TIM3.
 * \return  None.
 */
void TIM_PWMDZBypassCmd(PWM_TypeDef *PWMx, FunctionalState NewState)
{
    /* Check the parameters. */
    assert_param(IS_PWM_ALL_PERIPH(PWMx));

    if (NewState != DISABLE)
    {
        /* Enable bypass dead zone function. */
        PWMx->CR |= BIT13;
    }
    else
    {
        /* Disable bypass dead zone function. */
        PWMx->CR &= (~BIT13);
    }
}

/**
 * \brief   Change the PWM dead zone clock source.
 * \param[in] PWMx: Where x can be 2 to 3 to select the PWMx peripheral.
 * \param[in] NewState: New state of the PWMx peripheral.
 *      \ref DISABLE: Use 32k clock source.
 *      \ref ENABLE: Use 5M clock source.
 * \return  None.
 * \note    To use this function, need to configure the corresponding timer.
 *          PWM2 ->> TIM2, PWM3 ->> TIM3.
 */
void TIM_PWMChangeDZClockSrc(PWM_TypeDef *PWMx, FunctionalState NewState)
{
    /* Check the parameters. */
    assert_param(IS_PWM_ALL_PERIPH(PWMx));

    if (NewState != DISABLE)
    {
        /* Use 5M clock source. */
        PWMx->CR |= BIT16;
    }
    else
    {
        /* Use 32k clock source. */
        PWMx->CR &= (~BIT16);
    }
}

/******************* (C) COPYRIGHT 2015 Realtek Semiconductor Corporation *****END OF FILE****/

