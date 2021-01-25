/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_3wire_spi.c
* @brief    This file provides all the 3 wire SPI firmware functions.
* @details
* @author   elliot chen
* @date     2016-12-13
* @version  v1.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtl876x_rcc.h"
#include "rtl876x_3wire_spi.h"

/**
  * @brief  Deinitializes the 3WIRE SPI peripheral registers to their default reset values.
  * @retval None
  */
void SPI3WIRE_DeInit(void)
{
    RCC_PeriphClockCmd(APBPeriph_SPI2W, APBPeriph_SPI2W_CLOCK, DISABLE);
}

/**
  * @brief Initializes the SPI_3WIRE peripheral according to the specified
  *   parameters in the SPI_3WIRE_InitStruct
  * @param  SPI3WIRE_InitStruct: pointer to a SPI3WIRE_InitTypeDef structure that
  *   contains the configuration information for the specified SPI_3WIRE peripheral
  * @retval None
  */
void SPI3WIRE_Init(SPI3WIRE_InitTypeDef *SPI3WIRE_InitStruct)
{
    /* Check the parameters */
    assert_param(IS_SPI3WIRE_MODE(SPI3WIRE_InitStruct->SPI3WIRE_Mode));
    assert_param(IS_SPI3WIRE_OE_DELAY_CFG(SPI3WIRE_InitStruct->SPI3WIRE_OutputDelay));
    assert_param(IS_SPI3WIRE_END_EXTEND_MODE(SPI3WIRE_InitStruct->SPI3WIRE_ExtMode));
    assert_param(IS_SPI3WIRE_READ_CYCLE_DELAY(SPI3WIRE_InitStruct->SPI3WIRE_ReadDelay));

    uint32_t divValue = 0;

    divValue = (((SPI3WIRE_InitStruct->SPI3WIRE_SysClock) / (SPI3WIRE_InitStruct->SPI3WIRE_Speed * 2) -
                 1)) & 0xFF;

    /* Initialize the parameters */
    SPI3WIRE->CFGR = (SPI3WIRE_InitStruct->SPI3WIRE_Mode) | \
                     (SPI3WIRE_InitStruct->SPI3WIRE_OutputDelay) | \
                     (SPI3WIRE_InitStruct->SPI3WIRE_ExtMode) | \
                     (divValue << SPI3WIRE_SSI_DIV_NUM_Pos) | \
                     (SPI3WIRE_InitStruct->SPI3WIRE_ReadDelay << SPI3WIRE_SSI_DLY_CYCLE_Pos);

    /* Clear all read data registers,  read number and interrupt */
    SPI3WIRE->INTCR = SPI3WIRE_FIFO_INT_ALL_CLR;
}

/**
  * @brief  Fills each SPI3WIRE_InitStruct member with its default value.
  * @param  SPI3WIRE_InitStruct: pointer to an SPI3WIRE_InitTypeDef structure which will be initialized.
  * @retval None
  */
void SPI3WIRE_StructInit(SPI3WIRE_InitTypeDef *SPI3WIRE_InitStruct)
{
    SPI3WIRE_InitStruct->SPI3WIRE_SysClock      = 20000000;
    SPI3WIRE_InitStruct->SPI3WIRE_Speed         = 1000000;
    SPI3WIRE_InitStruct->SPI3WIRE_Mode          = SPI3WIRE_2WIRE_MODE;
    SPI3WIRE_InitStruct->SPI3WIRE_ReadDelay     =
        0;/* Delay time = (SPI3WIRE_ReadDelay+1)/(2*SPI3WIRE_Speed) */
    SPI3WIRE_InitStruct->SPI3WIRE_OutputDelay   = SPI3WIRE_OE_DELAY_1T;
    SPI3WIRE_InitStruct->SPI3WIRE_ExtMode       = SPI3WIRE_NORMAL_MODE;
}

/**
  * @brief  Enables or disables the specified SPI3WIRE peripheral.
  * @param  NewState: new state of the SPI3WIRE peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI3WIRE_Cmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the selected SPI3WIRE peripheral */
        SPI3WIRE->CFGR |= SPI3WIRE_SSI_EN_Msk;
    }
    else
    {
        /* Disable the selected SPI3WIRE peripheral */
        SPI3WIRE->CFGR &= SPI3WIRE_SSI_EN_CLR;
    }
}

/**
  * @brief  Enables or disables the specified SPI3WIRE interrupts.
  * @param  SPI3WIRE_INT: specifies the SPI3WIRE interrupts sources to be enabled or disabled.
  *   This parameter can be only be the following value:
  *     @arg SPI3WIRE_INT_BIT: enable SPI3WIRE interrupt.
  * @param  NewState: new state of the specified SPI3WIRE interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI3WIRE_INTConfig(uint32_t SPI3WIRE_INT, FunctionalState newState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(newState));
    assert_param(IS_SPI3WIRE_INT(SPI3WIRE_INT));

    if (newState == ENABLE)
    {
        /* Enable the selected 3WIRE_SPI interrupts */
        SPI3WIRE->CFGR |= SPI3WIRE_INT;
    }
    else
    {
        /* Disable the selected 3WIRE_SPI interrupts */
        SPI3WIRE->CFGR &= (uint32_t)(~SPI3WIRE_INT);
    }
}

/**
  * @brief  Configure resync signal time value.
  * @param  value: Resync signal time value whose uint is 1/(2*SPI3WIRE_Speed).
  *   This parameter can be only be the following value: 0x0 to 0xf.
  * @retval None
  */
void SPI3WIRE_SetResyncTime(uint32_t value)
{
    SPI3WIRE->CFGR &= SPI3WIRE_SSI_RESYNC_TIME_CLR;
    SPI3WIRE->CFGR |= value << SPI3WIRE_SSI_RESYNC_TIME_Pos;
}

/**
  * @brief  Send resync signal or not .Must send when SPI3WIRE is disable.
  * @param  NewState: new state of the SPI3WIRE peripheral.
  *   This parameter can be only be the following value:
  * @param ENABLE: trigger resync signal.
  * @param ENABLE: disable resync signal.
  * @retval None
  */
void SPI3WIRE_ResyncSignalCmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        SPI3WIRE->INTCR |= SPI3WIRE_RESYNV_EN_Msk;
    }
    else
    {
        SPI3WIRE->INTCR &= SPI3WIRE_RESYNV_EN_CLR;
    }
}

/**
  * @brief  Get total number of data byte in each SPI reading.
  * @param  None.
  * @retval The total number of data byte in each SPI reading.
  */
uint8_t SPI3WIRE_GetRxDataLen(void)
{
    return (uint8_t)(SPI3WIRE->SR & SPI3WIRE_RDATA_NUM_Msk);
}

/**
  * @brief  Clear read data number status.
  * @param  None.
  * @retval None
  */
void SPI3WIRE_ClearRxDataLen(void)
{
    SPI3WIRE->INTCR |= SPI3WIRE_RD_NUM_CLEAR_Msk;
}

/**
  * @brief  Clear all read data registers.
  * @param  None.
  * @retval None.
  */
void SPI3WIRE_ClearRxFIFO(void)
{
    SPI3WIRE->INTCR |= SPI3WIRE_RD_DATA_CLEAR_Msk;
}

/**
  * @brief  start to write data.
  * @param  address: write address.
  * @param  data: write data.
  * @retval None
  */
void SPI3WIRE_StartWrite(uint8_t address, uint8_t data)
{
    SPI3WIRE->CR = (uint32_t)(SPI3WIRE_RW_MODE_Msk) | ((uint32_t)address << SPI3WIRE_ADDRESS_Pos) |
                   (uint32_t)data;
}

/**
  * @brief  start read.
  * @param  address: read address.
  * @param  readNum: number of data to read.This value can be 0x1 to 0xf.
  * @retval None.
  */
void SPI3WIRE_StartRead(uint8_t address, uint32_t len)
{
    if (len == 1)
    {
        /* Disable burst read */
        SPI3WIRE->CFGR &= SPI3WIRE_SSI_BURST_READ_EN_CLR;
        /* Read address */
        SPI3WIRE->CR = ((uint32_t)address << SPI3WIRE_ADDRESS_Pos);
    }
    else
    {
        /* Enable burst read */
        SPI3WIRE->CFGR |= SPI3WIRE_SSI_BURST_READ_EN_Msk;
        /* Read num */
        SPI3WIRE->CFGR &= SPI3WIRE_SSI_BURST_READ_NUM_CLR;
        SPI3WIRE->CFGR |= ((len - 1) << SPI3WIRE_SSI_BURST_READ_NUM_Pos);
        /* Set read command and read address */
        SPI3WIRE->CR = ((uint32_t)address << SPI3WIRE_ADDRESS_Pos);
    }
}

/**
  * @brief  write data.
  * @param  outBuf: buffer to store read datas.
  * @param  readNum: read number.
  * @retval None
  */
void SPI3WIRE_ReadBuf(uint8_t *pBuf, uint8_t readNum)
{
    uint8_t i = 0;
    uint8_t j = 0;
    uint32_t regVal = 0;
    uint8_t regIndex =  readNum / 4;

    for (i = 0; i < regIndex; i++)
    {
        regVal = *((volatile uint32_t *)(&(SPI3WIRE->RD0) + i));

        for (j = 0; j < 4; j++)
        {
            *pBuf++ = (uint8_t)(regVal >> (8 * j));
        }
    }

    if (readNum > regIndex * 4)
    {
        regVal = *((volatile uint32_t *)(&(SPI3WIRE->RD0) + regIndex));

        for (j = 0; j < (readNum - regIndex * 4); j++)
        {
            *pBuf++ = (uint8_t)(regVal >> (8 * j));
        }
    }
}

/**
  * @brief  Checks whether the specified 3WIRE_SPI flag is set or not.
  * @param  SPI3WIRE_FLAG: specifies the flag to check.
  *   This parameter can be one of the following values:
  *     @arg SPI3WIRE_FLAG_BUSY: 3wire spi is busy.
  *     @arg SPI3WIRE_FLAG_INT_IND: there is 3wire spi interrupt.
  *     @arg SPI3WIRE_FLAG_RESYNC_BUSY: resync busy or not.
  * @retval The new state of UART_FLAG (SET or RESET).
  */
FlagStatus SPI3WIRE_GetFlagStatus(uint32_t SPI3WIRE_FLAG)
{
    FlagStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_SPI3WIRE_FLAG(SPI3WIRE_FLAG));

    if (SPI3WIRE->SR & SPI3WIRE_FLAG)
    {
        bitstatus = SET;
    }
    return bitstatus;
}

/**
  * @brief  Clears the 3WIRE_SPI interrupt pending bits.
  * @param  SPI3WIRE_INT: specifies the SPI3WIRE interrupts sources to be enabled or disabled.
  *   This parameter can be only be the following value:
  *     @arg SPI3WIRE_INT_BIT: enable SPI3WIRE interrupt.
  * @retval None
  */
void SPI3WIRE_ClearINTPendingBit(uint32_t SPI3WIRE_INT)
{
    /* Check the parameters */
    assert_param(IS_SPI3WIRE_INT(SPI3WIRE_INT));

    SPI3WIRE->INTCR |= SPI3WIRE_INT;
}

/******************* (C) COPYRIGHT 2016 Realtek Semiconductor Corporation *****END OF FILE****/

