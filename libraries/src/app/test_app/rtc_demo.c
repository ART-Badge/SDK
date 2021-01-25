/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtc_demo.c
* @brief    This file provides demo code to realize RTC comparator function.
* @details
* @author   elliot chen
* @date     2016-12-15
* @version  v1.0
*********************************************************************************************************
*/

/* Defines ------------------------------------------------------------------*/
/** Prescaler value.
  * 12 bits prescaler for COUNTER frequency (32768/(PRESCALER+1)).
  * If use internal 32KHz, (32000/(PRESCALER+1)).
  * Must be written when RTC 24-bit counter is stopped.
  */
#define RTC_PRESCALER_VALUE     (320-1)//f = 100Hz
/* RTC has four comparators.0~3 */
#define RTC_COMP_INDEX          1
#define RTC_INT_CMP_1           RTC_INT_COMP1
#define RTC_INT_CMP_NUM         RTC_INT_COMP1
#define RTC_COMP_VALUE          (10)

/* Includes ------------------------------------------------------------------*/
#include "rtl876x_rcc.h"
#include "rtl876x_rtc.h"
#include "rtl876x_nvic.h"
#include "trace.h"
//#include "portmacro.h"

/**
  * @brief  Initialize rtc peripheral.
  * @param   No parameter.
  * @return  void
  */
void driver_rtc_init(void)
{
    RTC_DeInit();
    RTC_SetPrescaler(RTC_PRESCALER_VALUE);

    RTC_SetCompValue(RTC_COMP_INDEX, RTC_COMP_VALUE);
    RTC_MaskINTConfig(RTC_INT_CMP_NUM, DISABLE);
    RTC_INTConfig(RTC_INT_CMP_NUM, ENABLE);

    /* Config RTC interrupt */
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    /* Enable or disable system wake up of RTC or LPC. */
    RTC_SystemWakeupConfig(ENABLE);

    RTC_NvCmd(ENABLE);
    /* Start RTC */
    RTC_Cmd(ENABLE);
}

/**
  * @brief  demo code of operation about RTC.
  * @param   No parameter.
  * @return  void
  */
void RTC_SampleCode(void)
{
    /* Initialize RTC peripheral */
    driver_rtc_init();

    //Add application code here
}

/**
  * @brief  RTC interrupt handle function.
  * @param  None.
  * @return None.
  */
void RTC_Handler(void)
{
    if (RTC_GetINTStatus(RTC_INT_CMP_NUM) == SET)
    {
//        DBG_DIRECT("[main]RTC_Handler: RTC counter current value = %d", RTC_GetCounter());
        RTC_SetCompValue(RTC_COMP_INDEX, RTC_GetCounter() + RTC_COMP_VALUE);
        RTC_ClearCompINT(RTC_COMP_INDEX);
    }
}

/******************* (C) COPYRIGHT 2016 Realtek Semiconductor Corporation *****END OF FILE****/

