/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     algorithm_align.c
* @brief
* @details
* @author   parker_xue
* @date     2018-4-13
* @version  v0.1
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "trace.h"
#include "app_task.h"
#include "rtl876x_rcc.h"
#include "rtl876x_tim.h"
#include "rtl876x_nvic.h"
#include "wristband_global_data.h"
#include "module_algorithm_align.h"
#include "hub_gsensor.h"
#include "hub_task.h"

#define ALIGN_TIMER                 TIM6
#define ALIGN_TIMER_IRQ             TIMER6_IRQn
#define ALIGN_TIMER_HANDLER         Timer6_Handler


void algorithm_align_timer_init(void)
{
    RCC_PeriphClockCmd(APBPeriph_TIMER, APBPeriph_TIMER_CLOCK, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_InitStruct;
    TIM_StructInit(&TIM_InitStruct);
    TIM_InitStruct.TIM_PWM_En = PWM_DISABLE;
    TIM_InitStruct.TIM_SOURCE_DIV = TIM_CLOCK_DIVIDER_4;
    TIM_InitStruct.TIM_Period = 10000000 - 1 ; //1s Timer Period
    TIM_InitStruct.TIM_Mode = TIM_Mode_UserDefine;
    TIM_TimeBaseInit(TIM6, &TIM_InitStruct);

    /*  Enable TIMER6 IRQ  */
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = ALIGN_TIMER_IRQ;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    TIM_ClearINT(ALIGN_TIMER);
    TIM_INTConfig(ALIGN_TIMER, ENABLE);
}

/**
* @brief  TIM6 interrupt handler function.
* @param   No parameter.
* @return  void
*/
void ALIGN_TIMER_HANDLER(void)
{
    TIM_ClearINT(ALIGN_TIMER);
    TIM_Cmd(ALIGN_TIMER, DISABLE);
    //Add User code here

    T_IO_MSG sensor_msg;
    sensor_msg.type = HUB_MSG_GSENSOR;
    sensor_msg.subtype = GSENSOR_MSG_WAKEUP;
    send_msg_to_ui_task(&sensor_msg, __LINE__);

    TIM_ChangePeriod(ALIGN_TIMER, 10000000 - 1);
    TIM_Cmd(ALIGN_TIMER, ENABLE);

}

void check_sensor_fifo_exit_dlps(void)
{
    uint32_t timer_period;
    uint16_t len = wristband_gsensor_get_fifo_length();
    if (len >= 22)
    {
        T_IO_MSG sensor_msg;
        sensor_msg.type = HUB_MSG_GSENSOR;
        sensor_msg.subtype = GSENSOR_MSG_WAKEUP;
        send_msg_to_ui_task(&sensor_msg, __LINE__);
        timer_period = 10000000;
    }
    else
    {
        timer_period = (25 - len) * 400000;
    }

    TIM_Cmd(ALIGN_TIMER, DISABLE);
    TIM_ChangePeriod(ALIGN_TIMER, timer_period - 1);
    TIM_Cmd(ALIGN_TIMER, ENABLE);
}

void algorithm_align_timer_start(void)
{
    TIM_ClearINT(ALIGN_TIMER);
    TIM_Cmd(ALIGN_TIMER, ENABLE);
}

void algorithm_align_timer_stop(void)
{
    TIM_ClearINT(ALIGN_TIMER);
    TIM_Cmd(ALIGN_TIMER, DISABLE);
}



