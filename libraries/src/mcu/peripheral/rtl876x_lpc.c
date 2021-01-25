/**
**********************************************************************************************************
*               Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_lpc.c
* @brief    This file provides all the lpcomp firmware functions.
* @details
* @author   yuan
* @date     2020-06-01
* @version  v1.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtl876x_lpc.h"

/* Internal defines ------------------------------------------------------------*/
/* Fast operation register address defines */
#define REG_FAST_WRITE_BASE_ADDR    (0x40000100UL)
#define REG_LPC_FAST_WDATA          (0x400001f0UL)
#define REG_LPC_FAST_ADDR           (0x400001f4UL)
#define REG_LPC_WR_STROBE           (0x400001f8UL)
/* AON register address defines */
#define LPC_AON_52                  (0x52)
#define LPC_AON_114                 (0x114)

/**
  * @brief  Fast write LPC register
  * @param  address: the address of LPC register .
  * @param  data: dta which write to LPC register.
  * @retval None
  */
void LPC_WriteReg(uint32_t offset, uint32_t data)
{
    static bool is_called = false;

    if (is_called == false)
    {
        *((volatile uint32_t *)0x40000014) |= BIT(9);//no need run this every time
        is_called = true;
    }

    /* Write data */
    *((volatile uint32_t *)REG_LPC_FAST_WDATA) = data;
    /* Write RTC register address. Only offset */
    *((volatile uint32_t *)REG_LPC_FAST_ADDR) = offset - REG_FAST_WRITE_BASE_ADDR;
    *((volatile uint32_t *)REG_LPC_WR_STROBE) = 1;
}

/**
  * @brief  Initializes LPC peripheral according to
  *    the specified parameters in the LPC_InitStruct.
  * @param  LPC_InitStruct: pointer to a LPC_InitTypeDef
  *    structure that contains the configuration information for the
  *    specified LPC peripheral.
  * @retval None
  */
void LPC_Init(LPC_InitTypeDef *LPC_InitStruct)
{
    uint32_t reg_value = 0;

    /* Check the parameters */
    assert_param(IS_LPC_CHANNEL(LPC_InitStruct->LPC_Channel));
    assert_param(IS_LPC_EDGE(LPC_InitStruct->LPC_Edge));
    assert_param(IS_LPC_THRESHOLD(LPC_InitStruct->LPC_Threshold));

    /* Configure parameters */
    reg_value = btaon_fast_read_safe(LPC_AON_52);

    reg_value |= LPC_InitStruct->LPC_Threshold & LPC_AON_52_THRESHOLD_Msk;
    btaon_fast_write_safe(LPC_AON_52, reg_value);

    reg_value = ((LPC_InitStruct->LPC_Channel) << LPC_AON_114_CH_NUM_Pos) | \
                (LPC_InitStruct->LPC_Edge);
    btaon_fast_write_safe(LPC_AON_114, reg_value);

    LPC_WriteReg((uint32_t)(&(LPC->LPC_CR0)), LPC_LPCOMP_CNT_CLEAR_Msk);
    __NOP();
    __NOP();
    LPC_WriteReg((uint32_t)(&(LPC->LPC_CR0)), 0x0);
}

/**
  * @brief  Fills each LPC_InitStruct member with its default value.
  * @param  LPC_InitStruct : pointer to a LPC_InitTypeDef structure which will be initialized.
  * @retval None
  */
void LPC_StructInit(LPC_InitTypeDef *LPC_InitStruct)
{
    LPC_InitStruct->LPC_Channel   = LPC_CHANNEL_P2_0;
    LPC_InitStruct->LPC_Edge      = LPC_Vin_Below_Vth;
    LPC_InitStruct->LPC_Threshold = LPC_2000_mV;
}

/**
  * @brief  Enables or disables LPC peripheral.
  * @param  NewState: new state of LPC peripheral.
  *     This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void LPC_Cmd(FunctionalState NewState)
{
    uint32_t reg_value = 0;

    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the LPC power */
        reg_value = btaon_fast_read_safe(LPC_AON_114);
        reg_value |= LPC_AON_114_POWER_EN_Msk;
        btaon_fast_write_safe(LPC_AON_114, reg_value);
    }
    else
    {
        /* Disable the LPC power */
        reg_value = btaon_fast_read_safe(LPC_AON_114);
        reg_value &= LPC_AON_114_POWER_EN_CLR;
        btaon_fast_write_safe(LPC_AON_114, reg_value);
    }
}

/**
  * @brief Start or stop the LPC counter.
  * @param  NewState: new state of the LPC counter.
  *   This parameter can be one of the following values:
  *     @arg ENABLE: Start LPCOMP counter.
  *     @arg DISABLE: Stop LPCOMP counter.
  * @retval None
  */
void LPC_CounterCmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Start the LPCOMP counter */
        LPC_WriteReg((uint32_t)(&(LPC->LPC_CR0)), ((LPC->LPC_CR0) | LPC_COUNTER_START_Msk));
    }
    else
    {
        /* Stop the LPCOMP counter */
        LPC_WriteReg((uint32_t)(&(LPC->LPC_CR0)), ((LPC->LPC_CR0) & (LPC_COUNTER_START_CLR)));
    }
}

/**
  * @brief  Enables or disables the specified LPC interrupts.
  * @param  LPC_INT: specifies the LPC interrupt source to be enabled or disabled.
  *   This parameter can be one of the following values:
  *     @arg LPC_INT_VOLTAGE_COMP: voltage detection interrupt.If Vin<Vth, cause this interrupt.
  *     @arg LPC_INT_COUNT_COMP: couter comparator interrupt.
  * @param  NewState: new state of the specified LPC interrupt.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void LPC_INTConfig(uint32_t LPC_INT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_LPC_CONFIG_INT(LPC_INT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the selected LPC interrupt */
        LPC_WriteReg((uint32_t)(&(LPC->LPC_CR0)), ((LPC->LPC_CR0) | LPC_INT));
    }
    else
    {
        /* Disable the selected LPC interrupt */
        LPC_WriteReg((uint32_t)(&(LPC->LPC_CR0)), ((LPC->LPC_CR0) & (~LPC_INT)));
    }
}

/**
  * @brief  Configure LPCOMP counter's comparator value.
  * @param  value: LPCOMP counter's comparator value which can be 0 to 0xfff.
  * @retval None
  */
void LPC_SetCompValue(uint32_t value)
{
    LPC_WriteReg((uint32_t) & (LPC->LPC_CMP), (value & 0xFFF));
}

/**
  * @brief  Reset the LPC counter.
  * @retval none
  */
void LPC_ResetCounter(void)
{
    /* Reset the LPCOMP counter */
    LPC_WriteReg((uint32_t)(&(LPC->LPC_CR0)), (LPC->LPC_CR0 | LPC_COUNTER_RESET_Msk));
    __NOP();
    __NOP();
    LPC_WriteReg((uint32_t)(&(LPC->LPC_CR0)), 0);
}

/**
  * @brief  Checks whether the specified LPC interrupt is set or not.
  * @param  LPC_INT: specifies the LPC interrupt to check.
  *     This parameter can be one of the following values:
  *     @arg LPC_INT_COUNT_COMP: couter comparator interrupt.
  * @retval The new state of SPI_IT (SET or RESET).
  */
FlagStatus LPC_GetFlagStatus(uint32_t LPC_FLAG)
{
    ITStatus int_status = RESET;

    /* Check the parameters */
    assert_param(IS_LPC_STATUS_INT(LPC_FLAG));

    if (LPC_FLAG == LPC_INT_LPCOMP_AON)
    {
        if (((LPC->LPC_SR) & LPC_LPCOMP_OUTPUT_AON_Msk) != (uint32_t)RESET)
        {
            int_status = SET;
        }
    }
    else if (LPC_FLAG == LPC_INT_LPCOMP_CNT)
    {
        if (((LPC->LPC_SR) & LPC_LPCOMP_CNT_Msk) != (uint32_t)RESET)
        {
            int_status = SET;
        }
    }

    /* Return the LPC_INT status */
    return  int_status;
}

/**
  * @brief  Clear the specified LPC interrupt.
  * @param  LPC_INT: specifies the LPC interrupt to clear.
  *   This parameter can be one of the following values:
  *     @arg LPC_INT_COUNT_COMP: couter comparator interrupt.
  * @retval None
  */
void LPC_ClearFlag(uint32_t LPC_FLAG)
{
    /* Check the parameters */
    assert_param(IS_LPC_CLEAR_INT(LPC_FLAG));

    /* Clear counter comparator interrupt */
    if (LPC_FLAG == LPC_INT_LPCOMP_CNT)
    {
        LPC_WriteReg((uint32_t)(&(LPC->LPC_CR0)), ((LPC->LPC_CR0) | LPC_LPCOMP_CNT_CLEAR_Msk));
        LPC_WriteReg((uint32_t)(&(LPC->LPC_CR0)), (LPC->LPC_CR0 & (~(LPC_LPCOMP_CNT_CLEAR_Msk))));
    }
}

/**
  * @brief  Checks whether the specified LPC interrupt is set or not.
  * @param  LPC_INT: specifies the LPC interrupt to check.
  *     This parameter can be one of the following values:
  *     @arg LPC_INT_COUNT_COMP: couter comparator interrupt.
  * @retval The new state of SPI_IT (SET or RESET).
  */
ITStatus LPC_GetINTStatus(uint32_t LPC_INT)
{
    ITStatus int_status = RESET;

    /* Check the parameters */
    assert_param(IS_LPC_STATUS_INT(LPC_INT));

    if (LPC_INT == LPC_INT_LPCOMP_AON)
    {
        if (((LPC->LPC_SR) & LPC_LPCOMP_OUTPUT_AON_Msk) != (uint32_t)RESET)
        {
            int_status = SET;
        }
    }
    else if (LPC_INT == LPC_INT_LPCOMP_CNT)
    {
        if (((LPC->LPC_SR) & LPC_LPCOMP_CNT_Msk) != (uint32_t)RESET)
        {
            int_status = SET;
        }
    }

    /* Return the LPC_INT status */
    return  int_status;
}

/**
  * @brief  Clear the specified LPC interrupt.
  * @param  LPC_INT: specifies the LPC interrupt to clear.
  *   This parameter can be one of the following values:
  *     @arg LPC_INT_COUNT_COMP: couter comparator interrupt.
  * @retval None.
  */
void LPC_ClearINTPendingBit(uint32_t LPC_INT)
{
    /* Check the parameters */
    assert_param(IS_LPC_CLEAR_INT(LPC_INT));

    /* Clear counter comparator interrupt */
    if (LPC_INT == LPC_INT_LPCOMP_CNT)
    {
        LPC_WriteReg((uint32_t)(&(LPC->LPC_CR0)), ((LPC->LPC_CR0) | LPC_LPCOMP_CNT_CLEAR_Msk));
        LPC_WriteReg((uint32_t)(&(LPC->LPC_CR0)), (LPC->LPC_CR0 & (~(LPC_LPCOMP_CNT_CLEAR_Msk))));
    }
}
/******************* (C) COPYRIGHT 2020 Realtek Semiconductor Corporation *****END OF FILE****/

