/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     vibrate_motor.c
* @brief
* @details
* @author
* @date
* @version  v0.1
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "os_timer.h"
#include "string.h"
#include "trace.h"
#include "rtl876x_rcc.h"
#include "rtl876x_pinmux.h"
#include "rtl876x_tim.h"
#include "rtl876x_nvic.h"
#include "module_vibrate_motor.h"

#include "wristband_global_data.h"

#define CYCLE_HW_TIMER              TIM5
#define CYCLE_HW_TIMER_IRQ          TIMER5_IRQn
#define CYCLE_HW_TIMER_CALLBACK     Timer5_Handler

bool allowed_motor_enter_dlps = true;

bool motor_started = false;
uint16_t motor_count = 0;
uint16_t cycle;
uint16_t pwm_out_time;
uint16_t pwm_idle_time;

void motor_pad_init(void)
{
    Pinmux_Config(PWM_OUT, timer_pwm2);
    Pad_Config(PWM_OUT, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE, PAD_OUT_LOW);
}

void motor_exit_dlps(void)
{
    Pad_Config(PWM_OUT, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE, PAD_OUT_LOW);
}

void motor_enter_dlps(void)
{
    Pad_Config(PWM_OUT, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_DOWN, PAD_OUT_DISABLE, PAD_OUT_LOW);
}

void motor_start(uint16_t total_cycle, uint16_t pwm_out, uint16_t pwm_idle)
{
    if (motor_started)
    {
        return;
    }
    else
    {
        motor_started = true;
    }
    APP_PRINT_INFO3(" ***motor start cycle = %d, pwm_out = %d, pwm_idle = %d", total_cycle, pwm_out,
                    pwm_idle);
    cycle = total_cycle;
    pwm_out_time = pwm_out;
    pwm_idle_time = pwm_idle;
    motor_count = 0;

    TIM_INTConfig(CYCLE_HW_TIMER, ENABLE);
    TIM_Cmd(CYCLE_HW_TIMER, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
    allowed_motor_enter_dlps = false;
}

void motor_stop(void)
{
    APP_PRINT_INFO0(" ***motor stop\n\n");
    motor_started = false;
    cycle = 0;
    pwm_out_time = 0;
    pwm_idle_time = 0;
    motor_count = 0;

    TIM_INTConfig(CYCLE_HW_TIMER, DISABLE);
    TIM_Cmd(CYCLE_HW_TIMER, DISABLE);
    TIM_Cmd(TIM2, DISABLE);

    allowed_motor_enter_dlps = true;
}

void motor_init(void)
{
    motor_pad_init();
    /* Open timer clock */
    RCC_PeriphClockCmd(APBPeriph_TIMER, APBPeriph_TIMER_CLOCK, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_InitStruct;
    TIM_StructInit(&TIM_InitStruct);
    TIM_InitStruct.TIM_PWM_En = PWM_ENABLE;
    /* Set period */
    TIM_InitStruct.TIM_Period = 10 - 1 ;
    /* Set PWM high count and low count */
    TIM_InitStruct.TIM_PWM_High_Count = 9000 - 1 ;
    TIM_InitStruct.TIM_PWM_Low_Count = 1000 - 1 ;
    /* Set timer mode User_define */
    TIM_InitStruct.TIM_Mode = TIM_Mode_UserDefine;
    /* Set timer clock source divider 40, timer pclk = 40M/4 */
    TIM_InitStruct.TIM_SOURCE_DIV = TIM_CLOCK_DIVIDER_4;
    TIM_TimeBaseInit(TIM2, &TIM_InitStruct);


    TIM_InitStruct.TIM_PWM_En = PWM_DISABLE;
    /* Set period */
    TIM_InitStruct.TIM_Period = 100000 - 1 ;//Period = 100ms
    /* Set timer mode User_define */
    TIM_InitStruct.TIM_Mode = TIM_Mode_UserDefine;
    /* Set timer clock source divider 40, timer pclk = 40M/40 */
    TIM_InitStruct.TIM_SOURCE_DIV = TIM_CLOCK_DIVIDER_40;
    TIM_TimeBaseInit(CYCLE_HW_TIMER, &TIM_InitStruct);

    /*  Enable TIMER3 IRQ  */
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = CYCLE_HW_TIMER_IRQ;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    TIM_ClearINT(CYCLE_HW_TIMER);
    TIM_INTConfig(CYCLE_HW_TIMER, DISABLE);
    TIM_Cmd(CYCLE_HW_TIMER, DISABLE);
    TIM_Cmd(TIM2, DISABLE);

}
bool motor_getstatus(void)
{
    if (allowed_motor_enter_dlps == true)
    {
        return false;
    }
    else
    {
        return true;
    }
}
/**
* @brief TIM interrupt handler function.
* @param   No parameter.
* @return  void
*/
void CYCLE_HW_TIMER_CALLBACK(void)
{
    if (TIM_GetINTStatus(CYCLE_HW_TIMER))
    {
        motor_count++;

        if ((motor_count % (pwm_out_time + pwm_idle_time)) == pwm_out_time)
        {
            TIM_Cmd(TIM2, DISABLE);
            if (motor_count == (cycle * (pwm_out_time + pwm_idle_time) - pwm_idle_time))
            {
                TIM_Cmd(CYCLE_HW_TIMER, DISABLE);
                allowed_motor_enter_dlps = true;
                motor_started = false;
            }
        }
        else if ((motor_count % (pwm_out_time + pwm_idle_time)) == 0)
        {
            TIM_Cmd(TIM2, ENABLE);
        }
        TIM_ClearINT(CYCLE_HW_TIMER);
    }
}




