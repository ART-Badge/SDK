/**
*********************************************************************************************************
*               Copyright(c) 2019, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_ir.c
* @brief    This file provides all the IR firmware functions.
* @details
* @author   yuan
* @date     2019-11-14
* @version  v1.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtl876x_rcc.h"
#include "rtl876x_ir.h"

/**
  * @brief  Deinitializes the IR peripheral registers to their default values.
  * @param  None
  * @retval None
  */
void IR_DeInit(void)
{
    RCC_PeriphClockCmd(APBPeriph_IR, APBPeriph_IR_CLOCK, DISABLE);
}

/**
  * @brief Initializes the IR peripheral according to the specified
  *   parameters in the IR_InitStruct
  * @param  IRx: selected IR peripheral.
  * @param  IR_InitStruct: pointer to a IR_InitTypeDef structure that
  *   contains the configuration information for the specified IR peripheral
  * @retval None
  */
void IR_Init(IR_InitTypeDef *IR_InitStruct)
{
    /* Check the parameters */
    assert_param(IS_IR_CLOCK(IR_InitStruct->IR_Clock));

    if (IR_InitStruct->IR_Clock == 40000000)
    {
        assert_param(IS_IR_FREQUENCY_40M(IR_InitStruct->IR_Freq));
    }
    else if (IR_InitStruct->IR_Clock == 90000000)
    {
        assert_param(IS_IR_FREQUENCY_90M(IR_InitStruct->IR_Freq));
    }
    else if (IR_InitStruct->IR_Clock == 100000000)
    {
        assert_param(IS_IR_FREQUENCY_100M(IR_InitStruct->IR_Freq));
    }
    assert_param(IS_IR_MODE(IR_InitStruct->IR_Mode));

    /* Configure IR clock divider. Formula: IR_CLK = IO_CLK/(1+IR_CLK_DIV) */
    uint32_t ir_clk_div_num = (IR_InitStruct->IR_Clock / IR_InitStruct->IR_Freq) - 1;
    uint32_t duty_cycle_num = ((double)(ir_clk_div_num + 1.0)) / (IR_InitStruct->IR_DutyCycle) - 1;

    IR->CLK_DIV = ir_clk_div_num;
    if (IR_InitStruct->IR_Mode == IR_MODE_TX)
    {
        /* Check the parameters in TX mode */
        assert_param(IS_IR_IDLE_STATUS(IR_InitStruct->IR_TxIdleLevel));
        assert_param(IS_IR_TX_DATA_TYPE(IR_InitStruct->IR_TxInverse));
        assert_param(IS_IR_TX_THRESHOLD(IR_InitStruct->IR_TxFIFOThrLevel));

        /* Configure TX mode parameters and disable all TX interrupt */
        IR->TX_CONFIG = (IR_InitStruct->IR_Mode) | \
                        (IR_InitStruct->IR_TxIdleLevel) | \
                        (IR_InitStruct->IR_TxInverse) | \
                        ((IR_InitStruct->IR_TxFIFOThrLevel) << IR_TX_FIFO_THRESHOLD_Pos) | \
                        ((duty_cycle_num & 0x3FFF) << IR_TX_DUTY_NUM_Pos);
        /* Clear all TX interrupt and TX FIFO */
        IR->TX_INT_CLR = IR_INT_ALL_CLR | IR_TX_FIFO_CLR_Msk;

        if (IR_InitStruct->IR_TxDmaEn != DISABLE)
        {
            IR->DMA_CONFIG = ((IR_InitStruct->IR_TxDmaEn << IR_TX_DMA_EN_Pos) & IR_TX_DMA_EN_Msk) | \
                             ((IR_InitStruct->IR_TxWaterLevel << IR_TX_WATER_LEVEL_Pos) & IR_TX_WATER_LEVEL_Msk);
        }
        else
        {
            IR->DMA_CONFIG = ((IR_InitStruct->IR_TxDmaEn << IR_TX_DMA_EN_Pos) & IR_TX_DMA_EN_CLR) | \
                             ((IR_InitStruct->IR_TxWaterLevel << IR_TX_WATER_LEVEL_Pos) & IR_TX_WATER_LEVEL_CLR);
        }
    }
    else
    {
        /* Check the parameters in RX mode */
        assert_param(IS_RX_START_MODE(IR_InitStruct->IR_RxStartMode));
        assert_param(IS_IR_RX_THRESHOLD(IR_InitStruct->IR_RxFIFOThrLevel));
        assert_param(IS_IR_RX_FIFO_FULL_CTRL(IR_InitStruct->IR_RxFIFOFullCtrl));
        assert_param(IS_RX_RX_TRIGGER_EDGE(IR_InitStruct->IR_RxTriggerMode));
        assert_param(IS_IR_RX_FILTER_TIME_CTRL(IR_InitStruct->IR_RxFilterTime));
        assert_param(IS_IR_RX_COUNT_LEVEL_CTRL(IR_InitStruct->IR_RxCntThrType));
        assert_param(IS_IR_RX_COUNTER_THRESHOLD(IR_InitStruct->IR_RxCntThr));

        /* Enable RX mode */
        IR->TX_CONFIG |= (IR_InitStruct->IR_Mode);
        /* Configure RX mode parameters and disable all RX interrupt */
        IR->RX_CONFIG = (IR_InitStruct->IR_RxStartMode) | \
                        (IR_InitStruct->IR_RxTriggerMode)  | \
                        (IR_InitStruct->IR_RxFilterTime)   | \
                        (IR_InitStruct->IR_RxFIFOFullCtrl) | \
                        (IR_InitStruct->IR_RxFIFOThrLevel << IR_RX_FIFO_LEVEL_Pos);
        /* Configure IR RX counter threshold parameters */
        IR->RX_CNT_INT_SEL = (IR_InitStruct->IR_RxCntThrType) | (IR_InitStruct->IR_RxCntThr);
        /* Clear all RX interrupt and RX FIFO */
        IR->RX_INT_CLR = IR_RX_INT_ALL_CLR | IR_RX_FIFO_CLR_Msk;

        if (IR_InitStruct->IR_RxDmaEn != DISABLE)
        {
            IR->DMA_CONFIG = ((IR_InitStruct->IR_RxDmaEn << IR_RX_DMA_EN_Pos) & IR_RX_DMA_EN_Msk) | \
                             ((IR_InitStruct->IR_RxWaterLevel << IR_RX_WATER_LEVEL_Pos) & IR_RX_WATER_LEVEL_Msk);
        }
        else
        {
            IR->DMA_CONFIG = ((IR_InitStruct->IR_RxDmaEn << IR_RX_DMA_EN_Pos) & IR_RX_DMA_EN_CLR) | \
                             ((IR_InitStruct->IR_RxWaterLevel << IR_RX_WATER_LEVEL_Pos) & IR_RX_WATER_LEVEL_CLR);
        }
    }
}

/**
  * @brief  Fills each IR_InitStruct member with its default value.
  * @param  IR_InitStruct: pointer to an IR_InitTypeDef structure which will be initialized.
  * @retval None
  */
void IR_StructInit(IR_InitTypeDef *IR_InitStruct)
{
    IR_InitStruct->IR_Clock             = IR_CLOCK_40M;
    IR_InitStruct->IR_Freq              = 38000;
    IR_InitStruct->IR_DutyCycle         = 3;
    IR_InitStruct->IR_Mode              = IR_MODE_TX;
    IR_InitStruct->IR_TxIdleLevel       = IR_IDLE_OUTPUT_LOW;
    IR_InitStruct->IR_TxInverse         = IR_TX_DATA_NORMAL;
    IR_InitStruct->IR_TxFIFOThrLevel    = 0;
    IR_InitStruct->IR_RxStartMode       = IR_RX_AUTO_MODE;
    IR_InitStruct->IR_RxFIFOThrLevel    = 0;
    IR_InitStruct->IR_RxFIFOFullCtrl    = IR_RX_FIFO_FULL_DISCARD_NEWEST;
    IR_InitStruct->IR_RxTriggerMode     = IR_RX_FALL_EDGE;
    IR_InitStruct->IR_RxFilterTime      = IR_RX_FILTER_TIME_50ns;
    IR_InitStruct->IR_RxCntThrType      = IR_RX_Count_Low_Level;
    IR_InitStruct->IR_RxCntThr          = 0x23a; /* This value can be 0 to 0x7fffffff */
    IR_InitStruct->IR_TxDmaEn           = DISABLE;  /*  */
    IR_InitStruct->IR_TxWaterLevel      = 15;       /*  */
    IR_InitStruct->IR_RxDmaEn           = DISABLE;  /*  */
    IR_InitStruct->IR_RxWaterLevel      = 0;        /*  */
}

/**
  * @brief   Enable or disable the selected IR mode.
  * @param  mode: selected IR operation mode.
  *   This parameter can be the following values:
  *     @arg IR_MODE_TX: Transmission mode.
  *     @arg IR_MODE_RX: Receiving mode.
  * @param  NewState: new state of the operation mode.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void IR_Cmd(uint32_t mode, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_IR_MODE(mode));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        if (mode == IR_MODE_TX)
        {
            /* Start Transmission mode */
            IR->TX_CONFIG |= IR_TX_START_Msk;
        }
        else
        {
            /* Start Receiving mode */
            IR->RX_CONFIG |= IR_RX_START_Msk;
        }
    }
    else
    {
        if (mode == IR_MODE_TX)
        {
            /* Stop Transmission mode */
            IR->TX_CONFIG &= IR_TX_START_CLR;
        }
        else
        {
            /* Stop Receiving mode */
            IR->RX_CONFIG &= IR_RX_START_CLR;
        }
    }
}

/**
  * @brief  Mask or unmask the specified IR interrupts.
  * @param  IR_INT: specifies the IR interrupts sources to be mask or unmask.
  *   This parameter can be the following values:
  *     @arg IR_INT_TF_EMPTY: TX FIFO empty interrupt.
  *     @arg IR_INT_TF_LEVEL: TX FIFO threshold interrupt.
  *     @arg IR_INT_TF_OF: TX FIFO overflow interrupt.
  *     @arg IR_INT_RF_FULL: RX FIFO  full interrupt.
  *     @arg IR_INT_RF_LEVEL: RX FIFO threshold interrupt.
  *     @arg IR_INT_RX_CNT_OF: RX counter overflow interrupt.
  *     @arg IR_INT_RF_OF: RX FIFO overflow interrupt.
  *     @arg IR_INT_RX_CNT_THR: RX counter threshold interrupt.
  *     @arg IR_INT_RF_ERROR: RX FIFO error read interrupt. Trigger when RX FIFO empty and read RX FIFO.
  * @param  NewState: new state of the specified IR interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void IR_MaskINTConfig(uint32_t IR_INT, FunctionalState newState)
{
    /* Check the parameters */
    assert_param(IS_IR_INT_CONFIG(IR_INT));
    assert_param(IS_FUNCTIONAL_STATE(newState));

    if (newState == ENABLE)
    {
        if ((IR->TX_CONFIG) & IR_MODE_SEL_Msk)
        {
            /* Enable the selected IR interrupts in RX mode */
            IR->RX_CONFIG |= (IR_INT << IR_RX_MSK_TO_EN_Pos);
        }
        else
        {
            /* Enable the selected IR interrupts in TX mode */
            if (IR_INT == IR_INT_TF_OF)
            {
                IR->TX_CONFIG |= (IR_INT << IR_TX_FIFO_OVER_MSK_TO_EN_Pos);
            }
            else
            {
                IR->TX_CONFIG |= (IR_INT << IR_TX_MSK_TO_EN_Pos);
            }
        }
    }
    else
    {
        if ((IR->TX_CONFIG) & IR_MODE_SEL_Msk)
        {
            /* Disable the selected IR interrupts in RX mode */
            IR->RX_CONFIG &= (~(IR_INT << IR_RX_MSK_TO_EN_Pos));
        }
        else
        {
            /* Disable the selected IR interrupts in TX mode */
            if (IR_INT == IR_INT_TF_OF)
            {
                IR->TX_CONFIG &= (~(IR_INT << IR_TX_FIFO_OVER_MSK_TO_EN_Pos));
            }
            else
            {
                IR->TX_CONFIG &= (~(IR_INT << IR_TX_MSK_TO_EN_Pos));
            }
        }
    }
}

/**
  * @brief  Enables or disables the specified IR interrupts.
  * @param  IR_INT: specifies the IR interrupts sources to be enabled or disabled.
  *   This parameter can be the following values:
  *     @arg IR_INT_TF_EMPTY: TX FIFO empty interrupt.
  *     @arg IR_INT_TF_LEVEL: TX FIFO threshold interrupt.
  *     @arg IR_INT_TF_OF: TX FIFO overflow interrupt.
  *     @arg IR_INT_RF_FULL: RX FIFO  full interrupt.
  *     @arg IR_INT_RF_LEVEL: RX FIFO threshold interrupt.
  *     @arg IR_INT_RX_CNT_OF: RX counter overflow interrupt.
  *     @arg IR_INT_RF_OF: RX FIFO overflow interrupt.
  *     @arg IR_INT_RX_CNT_THR: RX counter threshold interrupt.
  *     @arg IR_INT_RF_ERROR: RX FIFO error read interrupt. Trigger when RX FIFO empty and read RX FIFO.
  * @param  NewState: new state of the specified IR interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void IR_INTConfig(uint32_t IR_INT, FunctionalState newState)
{
    /* Check the parameters */
    assert_param(IS_IR_INT_CONFIG(IR_INT));
    assert_param(IS_FUNCTIONAL_STATE(newState));

    if (newState == ENABLE)
    {
        if (IR->TX_CONFIG & IR_MODE_SEL_Msk)
        {
            /* Enable the selected IR interrupts in RX mode */
            IR->RX_CONFIG |= IR_INT;
        }
        else
        {
            /* Enable the selected IR interrupts in TX mode */
            IR->TX_CONFIG |= IR_INT;
        }
    }
    else
    {
        if (IR->TX_CONFIG & IR_MODE_SEL_Msk)
        {
            /* Disable the selected IR interrupts in RX mode */
            IR->RX_CONFIG &= (~IR_INT);
        }
        else
        {
            /* Disable the selected IR interrupts in TX mode */
            IR->TX_CONFIG &= (~IR_INT);
        }
    }
}

/**
  * @brief   Start trigger only in manual receive mode.
  * @param  None.
  * @retval None
  */
void IR_StartManualRxTrigger(void)
{
    /* Start Rx manual mode */
    IR->RX_CONFIG |= IR_RX_MAN_START_Msk;
}

/**
  * @brief   Configure counter threshold value in receiving mode.You can use it to stop receiving IR data.
  * @param  IR_RxCntThrType:
  *   This parameter can be the following values:
  *     @arg IR_RX_Count_Low_Level: Low level counter value >= IR_RxCntThr, trigger IR_INT_RX_CNT_THR interrupt.
  *     @arg IR_RX_Count_High_Level: High level counter value >= IR_RxCntThr, trigger IR_INT_RX_CNT_THR interrupt.
  * @param  IR_RxCntThr: Configure IR Rx counter threshold value which can be 0 to 0x7fffffffUL.
  * @retval None
  */
void IR_SetRxCounterThreshold(uint32_t IR_RxCntThrType, uint32_t IR_RxCntThr)
{
    /* Check the parameters */
    assert_param(IS_IR_RX_COUNT_LEVEL_CTRL(IR_RxCntThrType));
    assert_param(IS_IR_RX_COUNTER_THRESHOLD(IR_RxCntThr));

    /* Configure IR RX counter threshold parameters */
    IR->RX_CNT_INT_SEL = (IR_RxCntThrType) | (IR_RxCntThr);
}

/**
  * @brief  Send data.
  * @param  buf: data buffer to send.
  * @param  length: buffer length.
  * @param IsLastPacket:
  *   This parameter can be the following values:
  *     @arg ENABLE: The last data in IR packet and there is no continous data.In other words, An infrared data transmission is completed.
  *     @arg DISABLE: There is data to be transmitted continuously.
  * @retval None
  */
void IR_SendBuf(uint32_t *pBuf, uint32_t len, FunctionalState IsLastPacket)
{
    uint32_t i = 0;

    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(IsLastPacket));

    if (len > 0)
    {
        i = len - 1;
        while (i--)
        {
            IR->TX_FIFO = *pBuf++;
        }

        /* If send the last IR packet, SET the following bit */
        if (IsLastPacket == ENABLE)
        {
            IR->TX_FIFO = (IR_TX_LAST_PACKEET_Msk | *pBuf);
        }
        else
        {
            IR->TX_FIFO = *pBuf;
        }
    }
}

/**
  * @brief  Send compensation data.
  * @param  comp_type:
  *     @arg IR_COMPEN_FLAG_1_2_CARRIER: 1/2 carrier freqency.
  *     @arg IR_COMPEN_FLAG_1_4_CARRIER: 1/4 carrier freqency.
  *     @arg IR_COMPEN_FLAG_1_N_SYSTEM_CLK: MOD((0x48[27:16]+0x00[11:0]), 4095)/40MHz.
         User can call function of IR_ConfigCompParam to configure 0x48[27:16].
  * @param  buf: data buffer to send.
  * @param  length: buffer length.
  * @param IsLastPacket:
  *   This parameter can be the following values:
  *     @arg ENABLE: The last data in IR packet and there is no continous data.In other words, An infrared data transmission is completed.
  *     @arg DISABLE: There is data to be transmitted continuously.
  * @retval None
  */
void IR_SendCompenBuf(IR_TX_COMPEN_TYPE comp_type, uint32_t *pBuf, uint32_t len,
                      FunctionalState IsLastPacket)
{
    uint32_t i = 0;

    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(IsLastPacket));

    if (len > 0)
    {
        i = len - 1;
        while (i--)
        {
            if (*pBuf & IR_DATA_TYPE_Msk)
            {
                IR->TX_FIFO = *pBuf++;
            }
            else
            {
                IR->TX_FIFO = *pBuf++ | comp_type;
            }
        }

        /* If send the last IR packet, SET the following bit */
        if (IsLastPacket == ENABLE)
        {
            if (*pBuf & IR_DATA_TYPE_Msk)
            {
                IR->TX_FIFO = (IR_TX_LAST_PACKEET_Msk | *pBuf);
            }
            else
            {
                IR->TX_FIFO = (IR_TX_LAST_PACKEET_Msk | *pBuf | comp_type);
            }
        }
        else
        {
            if (*pBuf & IR_DATA_TYPE_Msk)
            {
                IR->TX_FIFO = *pBuf;
            }
            else
            {
                IR->TX_FIFO = *pBuf | comp_type;
            }
        }
    }
}

/**
  * @brief  Read data From RX FIO.
  * @param  buf: buffer address to receive data.
  * @param  length: read data length.
  * @retval None
  */
void IR_ReceiveBuf(uint32_t *pBuf, uint32_t len)
{
    while (len--)
    {
        *pBuf++ = IR->RX_FIFO;
    }
}

/**
  * @brief Get the specified IR interrupt status.
  * @param  IR_INT: the specified IR interrupts.
  * This parameter can be one of the following values:
  *     @arg IR_INT_TF_EMPTY: TX FIFO empty interrupt.
  *     @arg IR_INT_TF_LEVEL: TX FIFO threshold interrupt.
  *     @arg IR_INT_TF_OF: TX FIFO overflow interrupt.
  *     @arg IR_INT_RF_FULL: RX FIFO  full interrupt.
  *     @arg IR_INT_RF_LEVEL: RX FIFO threshold interrupt.
  *     @arg IR_INT_RX_CNT_OF: RX counter overflow interrupt.
  *     @arg IR_INT_RF_OF: RX FIFO overflow interrupt.
  *     @arg IR_INT_RX_CNT_THR: RX counter threshold interrupt.
  *     @arg IR_INT_RF_ERROR: RX FIFO error read interrupt. Trigger when RX FIFO empty and read RX FIFO.
  * @retval The new state of IR_INT (SET or RESET).
  */
ITStatus IR_GetINTStatus(uint32_t IR_INT)
{
    ITStatus bit_status = RESET;

    /* Check the parameters */
    assert_param(IS_IR_INT_CONFIG(IR_INT));

    if (IR->TX_CONFIG & IR_MODE_SEL_Msk)
    {
        /* Get the selected IR interrupts in RX mode */
        if ((IR->RX_SR & IR_INT) != (uint32_t)RESET)
        {
            bit_status = SET;
        }
    }
    else
    {
        /* Get the selected IR interrupts in TX mode */
        if (IR_INT != IR_INT_TF_OF)
        {
            if ((IR->TX_SR & IR_INT) != (uint32_t)RESET)
            {
                bit_status = SET;
            }
        }
        else
        {
            if ((IR->TX_SR & (IR_INT >> IR_TX_STATUS_TO_EN_Pos)) != (uint32_t)RESET)
            {
                bit_status = SET;
            }
        }
    }

    /* Return the IR_INT status */
    return  bit_status;
}

/**
  * @brief  Clears the IR interrupt pending bits.
  * @param  IR_IT: specifies the interrupt pending bit to clear.
  *   This parameter can be any combination of the following values:
  *     @arg IR_INT_TF_EMPTY_CLR: Clear TX FIFO empty interrupt.
  *     @arg IR_INT_TF_LEVEL_CLR: Clear TX FIFO threshold interrupt.
  *     @arg IR_INT_TF_OF_CLR: Clear TX FIFO overflow interrupt.
  *     @arg IR_INT_RF_FULL_CLR: Clear RX FIFO  full interrupt.
  *     @arg IR_INT_RF_LEVEL_CLR: Clear RX FIFO threshold interrupt.
  *     @arg IR_INT_RX_CNT_OF_CLR: Clear RX counter overflow interrupt.
  *     @arg IR_INT_RF_OF_CLR: Clear RX FIFO overflow interrupt.
  *     @arg IR_INT_RX_CNT_THR_CLR: Clear RX counter threshold interrupt.
  *     @arg IR_INT_RF_ERROR_CLR: Clear RX FIFO error read interrupt. Trigger when RX FIFO empty and read RX FIFO.
  * @retval None
  */
void IR_ClearINTPendingBit(uint32_t IR_CLEAR_INT)
{
    /* Check the parameters */
    assert_param(IS_IR_INT_CLEAR(IR_CLEAR_INT));

    if (IR->TX_CONFIG & IR_MODE_SEL_Msk)
    {
        /* Clear the selected IR interrupts in RX mode */
        IR->RX_INT_CLR |= IR_CLEAR_INT;
    }
    else
    {
        /* Clear the selected IR interrupts in TX mode */
        IR->TX_INT_CLR |= IR_CLEAR_INT;
    }
}

/******************* (C) COPYRIGHT 2019 Realtek Semiconductor Corporation *****END OF FILE****/

