/**
*********************************************************************************************************
*               Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file       rtl876x_i2s.c
* @brief     This file provides all the I2S interface firmware functions.
* @details
* @author   elliot chen
* @date      2017-12-06
* @version  v1.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtl876x_rcc.h"
#include "rtl876x_i2s.h"

/**
  * @brief Initializes the I2S peripheral according to the specified
  *   parameters in the I2S_InitStruct
  * @param  I2S: selected I2S peripheral.
  * @param  I2S_InitStruct: pointer to a I2S_InitTypeDef structure that
  *   contains the configuration information for the specified I2S peripheral
  * @retval None
  */
void I2S_Init(I2S_TypeDef *I2Sx, I2S_InitTypeDef *I2S_InitStruct)
{
    /* Check the parameters */
    assert_param(IS_I2S_ALL_PERIPH(I2Sx));
    assert_param(IS_I2S_CLK_SOURCE(I2S_InitStruct->I2S_ClockSource));
    assert_param(IS_I2S_DEVICE_MODE(I2S_InitStruct->I2S_DeviceMode));
    assert_param(IS_I2S_CHANNEL_TYPE(I2S_InitStruct->I2S_ChannelType));
    assert_param(IS_I2S_DATA_FORMAT(I2S_InitStruct->I2S_DataFormat));
    assert_param(IS_I2S_DATA_WIDTH(I2S_InitStruct->I2S_DataWidth));
    assert_param(IS_I2S_MCLK_OUTPUT_TYPE(I2S_InitStruct->I2S_MCLKOutput));
    assert_param(IS_I2S_DMA_CMD(I2S_InitStruct->I2S_DMACmd));

    /* Reset I2S module */
    I2Sx->CTRL0 |= I2S_RESET_MSK;
    I2Sx->CTRL0 &= I2S_RESET_CLR;

    /* Configure BCLK parameters */
    I2Sx->BCLK_DIV = I2S_InitStruct->I2S_BClockMi | (I2S_InitStruct->I2S_BClockNi << I2S_BCLK_NI_POS) |
                     I2S_MI_NI_UPDATE_MSK;

    /* Configure I2S initialization parameters */
    I2Sx->CTRL0 = I2S_InitStruct->I2S_MCLKOutput | I2S_InitStruct->I2S_TxChSequence | I2S_RX_DISABLE_MSK
                  | \
                  I2S_InitStruct->I2S_RxBitSequence | I2S_InitStruct->I2S_TxBitSequence | \
                  I2S_InitStruct->I2S_RxChSequence | I2S_TX_DISABLE_MSK | \
                  I2S_InitStruct->I2S_DataWidth | I2S_InitStruct->I2S_ChannelType | \
                  I2S_InitStruct->I2S_DataFormat | I2S_InitStruct->I2S_DeviceMode | I2S_InitStruct->I2S_DMACmd;

    /* Configure I2S Clock Source parameters */
    I2Sx->CTRL1 = I2S_InitStruct->I2S_ClockSource | I2S_CLR_RX_ERR_CNT_MSK | \
                  I2S_CLR_TX_ERR_CNT_MSK | I2S_FRAME_SYNC_OFFSET_DEFAULT;
    /* Clear error counter */
    I2Sx->CTRL1 &= I2S_CLR_RX_ERR_CNT_CLR & I2S_CLR_TX_ERR_CNT_CLR;

    /* Configure GDMA burst size */
    I2Sx->DMA_TRDLR = (I2S_InitStruct->I2S_RxWaterlevel << I2S_RX_DMA_BURST_SIZE_POS) | \
                      (I2S_InitStruct->I2S_TxWaterlevel << I2S_TX_DMA_BURST_SIZE_POS);
}

/**
  * @brief  Deinitializes the I2S peripheral registers to their default values.
  * @param  None.
  * @retval None
  */
void I2S_DeInit(I2S_TypeDef *I2Sx)
{
    if (I2Sx == I2S0)
    {
        RCC_PeriphClockCmd(APBPeriph_I2S0, APBPeriph_I2S0_CLOCK, DISABLE);
    }
    else
    {
        if (I2Sx == I2S1)
        {
            RCC_PeriphClockCmd(APBPeriph_I2S1, APBPeriph_I2S1_CLOCK, DISABLE);
        }
    }
}

/**
  * @brief  Fills each I2S_InitStruct member with its default value.
  * @param  I2S_InitStruct: pointer to an I2S_InitTypeDef structure which will be initialized.
  * @retval None
  */
void I2S_StructInit(I2S_InitTypeDef *I2S_InitStruct)
{
    I2S_InitStruct->I2S_ClockSource     = I2S_CLK_40M;
    I2S_InitStruct->I2S_BClockMi        = 0x271;/* <!BCLK = 16K */
    I2S_InitStruct->I2S_BClockNi        = 0x10;
    I2S_InitStruct->I2S_DeviceMode      = I2S_DeviceMode_Master;
    I2S_InitStruct->I2S_ChannelType     = I2S_Channel_Mono;
    I2S_InitStruct->I2S_TxChSequence    = I2S_TX_CH_L_R;
    I2S_InitStruct->I2S_RxChSequence    = I2S_RX_CH_L_R;
    I2S_InitStruct->I2S_DataFormat      = I2S_Mode;
    I2S_InitStruct->I2S_TxBitSequence   = I2S_TX_MSB_First;
    I2S_InitStruct->I2S_RxBitSequence   = I2S_RX_MSB_First;
    I2S_InitStruct->I2S_DataWidth       = I2S_Width_16Bits;
    I2S_InitStruct->I2S_MCLKOutput      = I2S_MCLK_128fs;
    I2S_InitStruct->I2S_DMACmd          = I2S_DMA_ENABLE;
    I2S_InitStruct->I2S_TxWaterlevel    = 1;
    I2S_InitStruct->I2S_RxWaterlevel    = 1;
}

/**
  * @brief   Enable or disable the selected I2S mode.
  * @param  I2S: selected I2S peripheral.
  * @param  mode: selected I2S operation mode.
  *   This parameter can be the following values:
  *     @arg I2S_MODE_TX: transmission mode.
  *     @arg I2S_MODE_RX: receiving mode.
  * @param  NewState: new state of the operation mode.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void I2S_Cmd(I2S_TypeDef *I2Sx, uint32_t mode, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_I2S_ALL_PERIPH(I2Sx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState == ENABLE)
    {
        if (mode & I2S_MODE_TX)
        {
            I2Sx->CTRL0 &= I2S_TX_DISABLE_CLR;
        }

        if (mode & I2S_MODE_RX)
        {
            I2Sx->CTRL0 &= I2S_RX_DISABLE_CLR;
        }
        I2Sx->CTRL0 |= mode;
    }
    else
    {
        if (mode & I2S_MODE_TX)
        {
            I2Sx->CTRL0 |= I2S_TX_DISABLE_MSK;
        }

        if (mode & I2S_MODE_RX)
        {
            I2Sx->CTRL0 |= I2S_RX_DISABLE_MSK;
        }
        I2Sx->CTRL0 &= ~(mode);
    }
}

/**
  * @brief  Enable or disable the specified I2S interrupts.
  * @param  I2S_INT: specifies the I2S interrupts sources to be enable or disable.
  *   This parameter can be the following values:
  *     @arg I2S_INT_TX_IDLE: Transmit idle interrupt.
  *     @arg I2S_INT_RF_EMPTY: Receive FIFO empty interrupt.
  *     @arg I2S_INT_TF_EMPTY: Transmit FIFO empty interrupt.
  *     @arg I2S_INT_RF_FULL: Receive FIFO full interrupt.
  *     @arg I2S_INT_TF_FULL: Transmit FIFO full interrupt.
  *     @arg I2S_INT_RX_READY: Ready to receive interrupt.
  *     @arg I2S_INT_TX_READY: Ready to transmit interrupt.
  * @param  NewState: new state of the specified I2S interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void I2S_INTConfig(I2S_TypeDef *I2Sx, uint32_t I2S_INT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_I2S_INT_CONFIG(I2S_INT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState == ENABLE)
    {
        I2Sx->CTRL1 |= I2S_INT;
    }
    else
    {
        I2Sx->CTRL1 &= ~(I2S_INT);
    }
}

/**
  * @brief Get the specified I2S flag status.
  * @param  I2S_INT: the specified I2S interrupt.
  * This parameter can be one of the following values:
  *   This parameter can be the following values:
  *     @arg I2S_INT_TX_IDLE: Transmit idle interrupt.
  *     @arg I2S_INT_RF_EMPTY: Receive FIFO empty interrupt.
  *     @arg I2S_INT_TF_EMPTY: Transmit FIFO empty interrupt.
  *     @arg I2S_INT_RF_FULL: Receive FIFO full interrupt.
  *     @arg I2S_INT_TF_FULL: Transmit FIFO full interrupt.
  *     @arg I2S_INT_RX_READY: Ready to receive interrupt.
  *     @arg I2S_INT_TX_READY: Ready to transmit interrupt.
  * @retval The new state of LCD_FLAG (SET or RESET).
  */
ITStatus I2S_GetINTStatus(I2S_TypeDef *I2Sx, uint32_t I2S_INT)
{
    ITStatus bit_status = RESET;

    /* Check the parameters */
    assert_param(IS_I2S_INT_CONFIG(I2S_INT));

    if (I2Sx->SR & (I2S_INT >> I2S_READY_TO_TX_POS))
    {
        bit_status = SET;
    }

    /* Return the I2S_INT status */
    return  bit_status;
}

/**
  * @brief  Clears the I2S interrupt pending bits.
  * @param  I2S_CLEAR_INT: specifies the interrupt pending bit to clear.
  *   This parameter can be any combination of the following values:
  *     @arg I2S_CLEAR_INT_RX_READY: Clear ready to receive interrupt.
  *     @arg I2S_CLEAR_INT_TX_READY: Clear ready to transmit interrupt.
  * @retval None
  */
void I2S_ClearINTPendingBit(I2S_TypeDef *I2Sx, uint32_t I2S_CLEAR_INT)
{
    /* Check the parameters */
    assert_param(IS_I2S_INT_CONFIG(I2S_CLEAR_INT));

    I2Sx->DSP_INT_CR |= I2S_CLEAR_INT;
    I2Sx->DSP_INT_CR &= ~I2S_CLEAR_INT;
}

/******************* (C) COPYRIGHT 2017 Realtek Semiconductor Corporation *****END OF FILE****/

