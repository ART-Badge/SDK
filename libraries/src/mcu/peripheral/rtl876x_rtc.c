/**
*********************************************************************************************************
*               Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_rtc.c
* @brief    This file provides all the RTC firmware functions.
* @details
* @author   yuan
* @date     2020-06-01
* @version  v1.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtl876x_rtc.h"

/* Internal defines ------------------------------------------------------------*/
#define REG_FAST_WRITE_BASE_ADDR        (0x40000100UL)
#define REG_RTC_SLEEP_MODE_SET          (0x40000100UL)
#define REG_RTC_BACKUP                  (0x4000013CUL)
#define REG_RTC_FAST_WDATA              (0x400001F0UL)
#define REG_RTC_FAST_ADDR               (0x400001F4UL)
#define REG_RTC_WR_STROBE               (0x400001F8UL)

/**
  * @brief  Fast write RTC register.
  * @param  offset: the register address.
  * @param  data: data which write to register.
  * @retval None
  */
void RTC_WriteReg(uint32_t regAddress, uint32_t data)
{
    static bool is_called = false;

    if (is_called == false)
    {
        *((volatile uint32_t *)0x40000014) |= BIT(9);//no need run this every time
        is_called = true;
    }

    /* Write data */
    *((volatile uint32_t *)REG_RTC_FAST_WDATA) = data;
    /* Write RTC register address. Only offset */
    *((volatile uint32_t *)REG_RTC_FAST_ADDR) = regAddress - REG_FAST_WRITE_BASE_ADDR;
    *((volatile uint32_t *)REG_RTC_WR_STROBE) = 1;
}

/**
  * @brief  Reset RTC.
  * @param  None
  * @retval None
  */
void RTC_DeInit(void)
{
    /* Stop RTC counter */
    RTC_WriteReg((uint32_t)(&(RTC->CR0)), (RTC->CR0 & RTC_START_CLR));

    /* Disable wakeup signal */
    RTC_WriteReg(REG_RTC_SLEEP_MODE_SET, 0);

    /* Clear all RTC interrupt */
    RTC_WriteReg((uint32_t)(&(RTC->INT_CLR)), RTC_ALL_INT_CLR_SET);
    __NOP();
    __NOP();
    RTC_WriteReg((uint32_t)(&(RTC->INT_CLR)), RTC_ALL_INT_CLR_RESET);

    /* Clear prescale register */
    RTC_WriteReg((uint32_t)(&(RTC->PRESCALER)), 0);
    /* Clear all comparator register */
    RTC_WriteReg((uint32_t)(&(RTC->COMP0)), 0);
    RTC_WriteReg((uint32_t)(&(RTC->COMP1)), 0);
    RTC_WriteReg((uint32_t)(&(RTC->COMP2)), 0);
    RTC_WriteReg((uint32_t)(&(RTC->COMP3)), 0);
    RTC_WriteReg((uint32_t)(&(RTC->PRE_COMP)), 0);

    /* Reset prescale counter and counter */
    RTC_WriteReg((uint32_t)(&(RTC->CR0)), RTC_DIV_COUNTER_RST_Msk | RTC_COUNTER_RST_Msk);
    __NOP();
    __NOP();
    RTC_WriteReg((uint32_t)(&(RTC->CR0)), 0x0);
}

/**
  * @brief  Set RTC prescaler value.
  * @param  value: the prescaler value to be set.Should be no more than 12 bits!
  * @retval None
  */
void RTC_SetPrescaler(uint32_t value)
{
    RTC_WriteReg((uint32_t)(&(RTC->PRESCALER)), value & 0xFFF);
}

/**
  * @brief  Start or stop RTC peripheral.
  * @param  NewState: new state of RTC peripheral.
  *     This parameter can be the following values:
  *     @arg ENABLE: start RTC.
  *     @arg DISABLE: stop RTC.
  * @retval None
  */
void RTC_Cmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState == ENABLE)
    {
        /* Start RTC */
        RTC_WriteReg((uint32_t)(&(RTC->CR0)), RTC->CR0 | RTC_START_Msk);
    }
    else
    {
        /* Stop RTC */
        RTC_WriteReg((uint32_t)(&(RTC->CR0)), RTC->CR0 & RTC_START_CLR);
    }
}

/**
  * @brief  Mask or unmask the specified RTC interrupts.
  * @param  RTC_INT: specifies the RTC interrupt source to be enabled or disabled.
  *     This parameter can be any combination of the following values:
  *     @arg RTC_INT_TICK: RTC tick interrupt
  *     @arg RTC_INT_OVF: RTC counter overflow interrupt
  *     @arg RTC_INT_CMP0: compare 0 interrupt
  *     @arg RTC_INT_CMP1: compare 1 interrupt
  *     @arg RTC_INT_CMP2: compare 2 interrupt
  *     @arg RTC_INT_CMP3: compare 3 interrupt
  *     @arg RTC_INT_PRE_CMP: prescale compare interrupt
  *     @arg RTC_INT_PRE_CMP3: prescale & compare 3 interrupt
  * @param  NewState: new state of the specified RTC interrupt.
  *     This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RTC_MaskINTConfig(uint32_t RTC_INT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RTC_INT(RTC_INT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != ENABLE)
    {
        /* Unmask the selected RTC interrupt */
        RTC_WriteReg((uint32_t)(&(RTC->CR0)), (RTC->CR0) | (RTC_INT << RTC_MASK_TICK_INT_Pos));
    }
    else
    {
        /* Mask the selected RTC interrupt */
        RTC_WriteReg((uint32_t)(&(RTC->CR0)), (RTC->CR0) & (~(RTC_INT << RTC_MASK_TICK_INT_Pos)));
    }
}

/**
  * @brief  Enable or disable the specified RTC interrupts.
  * @param  RTC_INT: specifies the RTC interrupt source to be enabled or disabled.
  *     This parameter can be any combination of the following values:
  *     @arg RTC_INT_TICK: tick interrupt
  *     @arg RTC_INT_CMP0: compare 0 interrupt
  *     @arg RTC_INT_CMP1: compare 1 interrupt
  *     @arg RTC_INT_CMP2: compare 2 interrupt
  *     @arg RTC_INT_CMP3: compare 3 interrupt
  *     @arg RTC_INT_PRE_CMP: prescale compare interrupt
  *     @arg RTC_INT_PRE_CMP3: prescale & compare 3 interrupt
  * @param  NewState: new state of the specified RTC interrupt.
  *     This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RTC_INTConfig(uint32_t RTC_INT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RTC_CONFIG_INT(RTC_INT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState == ENABLE)
    {
        /* Enable the selected RTC comparator interrupt */
        if (RTC_INT == RTC_INT_TICK)
        {
            RTC_WriteReg((uint32_t)(&(RTC->CR0)), (RTC->CR0) | (RTC->CR0 | RTC_INT_TICK_EN_Msk));
        }
        else if ((RTC_INT <= RTC_INT_COMP3) && (RTC_INT >= RTC_INT_COMP0))
        {
            RTC_WriteReg((uint32_t)(&(RTC->CR0)), (RTC->CR0) | (RTC_INT << RTC_COMP_INT_EN_OFFSET));
        }
        else if ((RTC_INT == RTC_INT_PRE_COMP) || (RTC_INT == RTC_INT_PRE_COMP3))
        {
            RTC_WriteReg((uint32_t)(&(RTC->CR0)), (RTC->CR0) | (RTC_INT << RTC_PRE_COMP_INT_EN_OFFSET));
        }
    }
    else
    {
        /* Disable the selected RTC comparator interrupt */
        if (RTC_INT == RTC_INT_TICK)
        {
            RTC_WriteReg((uint32_t)(&(RTC->CR0)), (RTC->CR0) | (RTC->CR0 & (~RTC_INT_TICK_EN_Msk)));
        }
        else if ((RTC_INT <= RTC_INT_COMP3) && (RTC_INT >= RTC_INT_COMP0))
        {
            RTC_WriteReg((uint32_t)(&(RTC->CR0)), (RTC->CR0) & (~(RTC_INT << RTC_COMP_INT_EN_OFFSET)));
        }
        else if ((RTC_INT == RTC_INT_PRE_COMP) || (RTC_INT == RTC_INT_PRE_COMP3))
        {
            RTC_WriteReg((uint32_t)(&(RTC->CR0)), (RTC->CR0) & (~(RTC_INT << RTC_PRE_COMP_INT_EN_OFFSET)));
        }
    }
}

/**
  * @brief  Enable interrupt signal to MCU.
  * @param  This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RTC_NvCmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        RTC_WriteReg(REG_RTC_SLEEP_MODE_SET,
                     (*(volatile uint32_t *)(REG_RTC_SLEEP_MODE_SET)) | RTC_NV_EN_Msk);
    }
    else
    {
        RTC_WriteReg(REG_RTC_SLEEP_MODE_SET,
                     (*(volatile uint32_t *)(REG_RTC_SLEEP_MODE_SET)) & RTC_NV_EN_CLR);
    }
}

/**
  * @brief  Set RTC comparator value.
  * @param  index: the comparator number 0 ~ 3.
  * @param  value: the comparator value to be set.Should be no more than 24 bits!
  * @retval none
  */
void RTC_SetCompValue(uint8_t index, uint32_t value)
{
    /* Check the parameters */
    assert_param(IS_RTC_COMP(index));

    RTC_WriteReg((uint32_t)(&(RTC->COMP0) + index), (value & 0xFFFFFF));
}

/**
  * @brief  Set RTC prescaler comparator value.
  * @param  value: the comparator value to be set.Should be no more than 12 bits!
  * @retval none
  */
void RTC_SetPreCompValue(uint32_t value)
{
    RTC_WriteReg((uint32_t)(&(RTC->PRE_COMP)), (value & 0xFFF));
}

/**
  * @brief  Enable or disable system wake up of RTC or LPC.
  * @param  NewState: new state of the wake up function.
  *     This parameter can be: ENABLE or DISABLE.
  * @retval none
  */
void RTC_SystemWakeupConfig(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState == ENABLE)
    {
        /* Enable system wake up */
        RTC_WriteReg(REG_RTC_SLEEP_MODE_SET,
                     (*(volatile uint32_t *)(REG_RTC_SLEEP_MODE_SET)) | RTC_WAKEUP_EN_Msk);
    }
    else
    {
        /* Disable system wake up */
        RTC_WriteReg(REG_RTC_SLEEP_MODE_SET,
                     (*(volatile uint32_t *)(REG_RTC_SLEEP_MODE_SET)) & (~RTC_WAKEUP_EN_Msk));
    }
}

/**
  * @brief  Reset counter value of RTC.
  * @param  None
  * @retval None
  */
void RTC_ResetCounter(void)
{
    RTC_WriteReg((uint32_t)(&(RTC->CR0)), (RTC->CR0) | RTC_COUNTER_RST_Msk);
    __NOP();
    __NOP();
    RTC_WriteReg((uint32_t)(&(RTC->CR0)), (RTC->CR0) & RTC_COUNTER_RST_CLR);
}

/**
  * @brief  Reset prescaler counter value of RTC.
  * @param  None
  * @retval None
  */
void RTC_ResetPrescalerCounter(void)
{
    RTC_WriteReg((uint32_t)(&(RTC->CR0)), (RTC->CR0) | RTC_DIV_COUNTER_RST_Msk);
    __NOP();
    __NOP();
    RTC_WriteReg((uint32_t)(&(RTC->CR0)), (RTC->CR0) & RTC_DIV_COUNTER_RST_CLR);
}

/**
  * @brief  Write backup reg for store time information.
  * @param  value
  * @retval none
  */
void RTC_WriteBackupReg(uint32_t value)
{
    RTC_WriteReg(REG_RTC_BACKUP, value);
}

/**
  * @brief  Read backup reg.
  * @param  None
  * @retval reg value
  */
uint32_t RTC_ReadBackupReg(void)
{
    return (*((volatile uint32_t *)REG_RTC_BACKUP));
}

/**
  * @brief  Checks whether the specified RTC flag is set or not.
  * @param  RTC_FLAG: specifies the flag to check.
  *     This parameter can be any combination of the following values:
  *     @arg RTC_FLAG_TICK: RTC tick flag
  *     @arg RTC_FLAG_OVF: RTC counter overflow flag
  *     @arg RTC_FLAG_CMP0: RTC compare 0 flag
  *     @arg RTC_FLAG_CMP1: RTC compare 1 flag
  *     @arg RTC_FLAG_CMP2: RTC compare 2 flag
  *     @arg RTC_FLAG_CMP3: RTC compare 3 flag
  *     @arg RTC_FLAG_PRE_COMP: RTC prescale compare flag
  *     @arg RTC_FLAG_PRE_COMP3: RTC prescale & compare 3 flag
  * @retval The new state of RTC_FLAG (SET or RESET).
  */
FlagStatus RTC_GetFlagStatus(uint32_t RTC_FLAG)
{
    /* Check the parameters */
    assert_param(IS_RTC_FLAG(RTC_FLAG));

    FlagStatus flag_status = RESET;

    if ((RTC->INT_SR & RTC_FLAG) != (uint32_t)RESET)
    {
        flag_status = SET;
    }

    /* Return the RTC_INT status */
    return  flag_status;
}

/**
  * @brief  Clear the flags of RTC.
  * @param  RTC_FLAG: specifies the flag to clear.
  *     This parameter can be any combination of the following values:
  *     @arg RTC_FLAG_TICK: RTC tick flag
  *     @arg RTC_FLAG_OVF: RTC counter overflow flag
  *     @arg RTC_FLAG_CMP0: RTC compare 0 flag
  *     @arg RTC_FLAG_CMP1: RTC compare 1 flag
  *     @arg RTC_FLAG_CMP2: RTC compare 2 flag
  *     @arg RTC_FLAG_CMP3: RTC compare 3 flag
  *     @arg RTC_FLAG_PRE_COMP: RTC prescale compare flag
  *     @arg RTC_FLAG_PRE_COMP3: RTC prescale & compare 3 flag
  * @retval None
  */
void RTC_ClearFlag(uint32_t RTC_FLAG)
{
    /* Check the parameters */
    assert_param(IS_RTC_FLAG(RTC_FLAG));

    RTC_WriteReg((uint32_t)(&(RTC->INT_CLR)), RTC_FLAG);
    __NOP();
    __NOP();
    RTC_WriteReg((uint32_t)(&(RTC->INT_CLR)), 0);
}

/**
  * @brief  Checks whether the specified RTC interrupt is set or not.
   * @param  RTC_INT: specifies the RTC interrupt source to be enabled or disabled.
  *   This parameter can be any combination of the following values:
  *     @arg RTC_INT_TICK: RTC tick interrupt source
  *     @arg RTC_INT_COMP0: compare 0 interrupt source
  *     @arg RTC_INT_COMP1: compare 1 interrupt source
  *     @arg RTC_INT_COMP2: compare 2 interrupt source
  *     @arg RTC_INT_COMP3: compare 3 interrupt source
  *     @arg RTC_INT_PRE_COMP: prescale compare interrupt source
  *     @arg RTC_INT_PRE_COMP3: prescale & compare 3 interrupt source
  * @retval The new state of RTC_INT (SET or RESET).
  */
ITStatus RTC_GetINTStatus(uint32_t RTC_INT)
{
    /* Check the parameters */
    assert_param(IS_RTC_CONFIG_INT(RTC_INT));

    ITStatus int_status = RESET;

    if ((RTC->INT_SR & RTC_INT) != (uint32_t)RESET)
    {
        int_status = SET;
    }

    /* Return the RTC_INT status */
    return  int_status;
}

/**
  * @brief  Clear the interrupt pending bits of RTC.
  * @param  RTC_INT: specifies the RTC interrupt flag to clear.
  *   This parameter can be any combination of the following values:
  *     @arg RTC_INT_TICK: RTC tick interrupt source
  *     @arg RTC_INT_OVF: RTC counter overflow interrupt source
  *     @arg RTC_INT_COMP0: compare 0 interrupt source
  *     @arg RTC_INT_COMP1: compare 1 interrupt source
  *     @arg RTC_INT_COMP2: compare 2 interrupt source
  *     @arg RTC_INT_COMP3: compare 3 interrupt source
  *     @arg RTC_INT_PRE_COMP: prescale compare interrupt source
  *     @arg RTC_INT_PRE_COMP3: prescale & compare 3 interrupt source
  * @retval None
  */
void RTC_ClearINTPendingBit(uint32_t RTC_INT)
{
    /* Check the parameters */
    assert_param(IS_RTC_INT(RTC_INT));

    RTC_WriteReg((uint32_t)(&(RTC->INT_CLR)), RTC_INT);
    __NOP();
    __NOP();
    RTC_WriteReg((uint32_t)(&(RTC->INT_CLR)), 0);
}

/**
  * @brief  Clear the interrupt pending bit of the select comparator of RTC.
  * @param  index: the comparator number.
  * @retval None
  */
void RTC_ClearCompINT(uint8_t index)
{
    RTC_WriteReg((uint32_t)(&(RTC->INT_CLR)), BIT(RTC_COMP0_CLR_Pos + index));
    __NOP();
    __NOP();
    RTC_WriteReg((uint32_t)(&(RTC->INT_CLR)), 0);
}

/**
  * @brief  Clear the overflow interrupt pending bit of RTC.
  * @param  None
  * @retval None
  */
void RTC_ClearOverFlowINT(void)
{
    RTC_WriteReg((uint32_t)(&(RTC->INT_CLR)), RTC_OVERFLOW_CLR_SET);
    __NOP();
    __NOP();
    RTC_WriteReg((uint32_t)(&(RTC->INT_CLR)), 0);
}

/**
  * @brief  Clear the tick interrupt pending bit of RTC.
  * @param  None
  * @retval None
  */
void RTC_ClearTickINT(void)
{
    RTC_WriteReg((uint32_t)(&(RTC->INT_CLR)), RTC_TICK_CLR_SET);
    __NOP();
    __NOP();
    RTC_WriteReg((uint32_t)(&(RTC->INT_CLR)), 0);
}

/******************* (C) COPYRIGHT 2020 Realtek Semiconductor Corporation *****END OF FILE****/

