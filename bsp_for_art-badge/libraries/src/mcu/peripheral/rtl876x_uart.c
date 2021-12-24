/**
*********************************************************************************************************
*               Copyright(c) 2019, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_uart.c
* @brief    This file provides all the UART firmware functions.
* @details
* @author   yuan
* @date     2019-11-14
* @version  v1.0
*********************************************************************************************************
*/
#include "rtl876x_rcc.h"
#include "rtl876x_uart.h"

#define LCR_DLAB_Set                    ((uint32_t)(1 << 7))
#define LCR_DLAB_Reset                  ((uint32_t)~(1 << 7))

/**
  * @brief  Deinitializes the UART peripheral registers to their default reset values(turn off UART clock).
  * @param  UARTx: selected UART peripheral.
  * @retval None
  */
void UART_DeInit(UART_TypeDef *UARTx)
{
    assert_param(IS_UART_PERIPH(UARTx));

    if (UARTx == UART0)
    {
        RCC_PeriphClockCmd(APBPeriph_UART0, APBPeriph_UART0_CLOCK, DISABLE);
    }
    else if (UARTx == UART1)
    {
        RCC_PeriphClockCmd(APBPeriph_UART1, APBPeriph_UART1_CLOCK, DISABLE);
    }
    else if (UARTx == UART2)
    {
        RCC_PeriphClockCmd(APBPeriph_UART2, APBPeriph_UART2_CLOCK, DISABLE);
    }
    return;
}

/**
  * @brief Initializes the UART peripheral according to the specified
  *   parameters in the UART_InitStruct
  * @param  UARTx: selected UART peripheral.
  * @param  UART_InitStruct: pointer to a UART_InitTypeDef structure that
  *   contains the configuration information for the specified UART peripheral
  * @retval None
  */
void UART_Init(UART_TypeDef *UARTx, UART_InitTypeDef *UART_InitStruct)
{
    assert_param(IS_UART_PERIPH(UARTx));
    assert_param(IS_UART_WORD_LENGTH(UART_InitStruct->UART_WordLen));
    assert_param(IS_UART_STOPBITS(UART_InitStruct->UART_StopBits));
    assert_param(IS_UART_PARITY(UART_InitStruct->UART_Parity));
    assert_param(IS_UART_RX_FIFO_TRIGGER_LEVEL(UART_InitStruct->UART_RxThdLevel));
    assert_param(IS_UART_IDLE_TIME(UART_InitStruct->UART_IdleTime));
    assert_param(IS_UART_DMA_CFG(UART_InitStruct->UART_DmaEn));
    assert_param(IS_UART_AUTO_FLOW_CTRL(UART_InitStruct->UART_HardwareFlowControl));

    //clear DLAB bit
    UARTx->LCR &= LCR_DLAB_Reset;
    //disable all interrupt
    UARTx->DLH_INTCR = 0x00;

    //read to clear Line Status Reg
    UARTx->LSR;
    //clear FIFO
    UARTx->INTID_FCR |= (FCR_CLEAR_RX_FIFO_Set | FCR_CLEAR_TX_FIFO_Set);

    //set baudrate, firstly set DLAB bit
    UARTx->LCR |= LCR_DLAB_Set;
    //set calibration parameters(OVSR)
    UARTx->STSR &= ~0xF0;
    UARTx->STSR |= (UART_InitStruct->UART_Ovsr << 4);
    //set calibration parameters(OVSR_adj)
    UARTx->SPR &= (~(0x7ff << 16));
    UARTx->SPR |= (UART_InitStruct->UART_OvsrAdj << 16);
    //set DLL and DLH
    UARTx->DLL = (UART_InitStruct->UART_Div & 0x00FF);
    UARTx->DLH_INTCR = ((UART_InitStruct->UART_Div & 0xFF00) >> 8);
    //after set baudrate, clear DLAB bit
    UARTx->LCR &= LCR_DLAB_Reset;

    //set LCR reg
    UARTx->LCR = (UART_InitStruct->UART_Parity | UART_InitStruct->UART_StopBits |
                  UART_InitStruct->UART_WordLen);
    //set FCR reg, FIFO must enable
    UARTx->INTID_FCR = ((1 << 0) | UART_InitStruct->UART_TxThdLevel << 16 \
                        | UART_InitStruct->UART_RxThdLevel << 8 | UART_InitStruct->UART_DmaEn);

    /* harware flow control */
    UARTx->MCR &= (~((1 << 5) | (1 << 1)));
    UARTx->MCR |= UART_InitStruct->UART_HardwareFlowControl;

    /* set rx idle time */
    UARTx->RX_IDLE_TOCR = (UART_InitStruct->UART_IdleTime);

    if (UART_InitStruct->UART_DmaEn == UART_DMA_ENABLE)
    {
        /* Config UART Tx dma parameter */
        if (UART_InitStruct->UART_TxDmaEn != DISABLE)
        {
            /* Mask uart TX threshold value */
            UARTx->MISCR &= ~(0x1f << 3);
            UARTx->MISCR |= ((UART_InitStruct->UART_TxThdLevel) << 3) | BIT(1);
        }
        /* Config UART Rx dma parameter */
        if (UART_InitStruct->UART_RxDmaEn != DISABLE)
        {
            /* Mask uart RX threshold value */
            UARTx->MISCR &= ~(0x3f << 8);
            UARTx->MISCR |= (UART_InitStruct->UART_RxWaterLevel << 8) | BIT(2);
        }
    }

    return;
}

/**
  * @brief  Fills each UART_InitStruct member with its default value.
  * @param  UART_InitStruct: pointer to an UART_InitTypeDef structure which will be initialized.
  * @retval None
  */
void UART_StructInit(UART_InitTypeDef *UART_InitStruct)
{
    //115200 default
    UART_InitStruct->UART_Div               = 20;
    UART_InitStruct->UART_Ovsr              = 12;
    UART_InitStruct->UART_OvsrAdj           = 0x252;

    UART_InitStruct->UART_WordLen           = UART_WORD_LENGTH_8BIT;
    UART_InitStruct->UART_StopBits          = UART_STOP_BITS_1;
    UART_InitStruct->UART_Parity            = UART_PARITY_NO_PARTY;
    UART_InitStruct->UART_TxThdLevel        = 16; //1~29
    UART_InitStruct->UART_RxThdLevel        = 16; //1~29
    UART_InitStruct->UART_IdleTime          = UART_RX_IDLE_2BYTE; //idle interrupt wait time
    UART_InitStruct->UART_HardwareFlowControl   = UART_HW_FLOW_CTRL_DISABLE;
    UART_InitStruct->UART_DmaEn                 = UART_DMA_DISABLE;
    UART_InitStruct->UART_TxDmaEn               = DISABLE;
    UART_InitStruct->UART_RxDmaEn               = DISABLE;
    UART_InitStruct->UART_TxWaterLevel          = 15; //Better to equal TX_FIFO_SIZE(16)- GDMA_MSize
    UART_InitStruct->UART_RxWaterLevel          = 1; //Better to equal GDMA_MSize
    return;
}

/**
  * @brief  Mask or unmask the specified UART interrupts.
  * @param  UART_INT: specifies the IR interrupts sources to be mask or unmask.
  *     This parameter can be the following values:
  *     @arg UART_INT_MASK_RD_AVA: mask INTCR(erbi).
  *     @arg UART_INT_MASK_TX_FIFO_EMPTY: mask INTCR(etbei).
  *     @arg UART_INT_MASK_RX_LINE_STS: mask INTCR(elsi).
  *     @arg UART_INT_MASK_MODEM_STS: mask INTCR(edssi).
  *     @arg UART_INT_MASK_RX_BREAK: mask rx break interrupt.
  *     @arg UART_INT_MASK_RX_IDLE: mask rx idle interrupt.
  *     @arg UART_INT_MASK_TX_DONE: mask the interrupt tx done interrupt.
  *     @arg UART_INT_MASK_TX_THD: mask tx fifo threshold interrupt.
  * @param  NewState: new state of the specified UART interrupts.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_MaskINTConfig(UART_TypeDef *UARTx, uint32_t UART_INT_MASK, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));
    assert_param(IS_UART_INT_MASK(UART_INT_MASK));
    assert_param(IS_FUNCTIONAL_STATE(newState));

    if (NewState != DISABLE)
    {
        UARTx->INTMASK |= UART_INT_MASK;
    }
    else
    {
        UARTx->INTMASK &= ~(UART_INT_MASK);
    }
}

/**
  * @brief  Enables or disables the specified UART interrupts.
  * @param  UARTx: selected UARTx peripheral.
  * @param  UART_INT: specifies the UART interrupts sources to be enabled or disabled.
  *     This parameter can be any combination of the following values:
  *     @arg UART_INT_RD_AVA: Rx data avaliable interrupt source.
  *     @arg UART_INT_TX_FIFO_EMPTY: Tx FIFO empty interrupt source.
  *     @arg UART_INT_RX_LINE_STS: Rx line status interrupt source.
  *     @arg UART_INT_MODEM_STS: modem status interrupt source.
  *     @arg UART_INT_TX_DONE: Tx data done interrupt source.
  *     @arg UART_INT_TX_THD: Tx FIFO data <= thredhold interrupt source.
  *     @arg UART_INT_RX_BREAK: Rx break signal interrupt source.
  *     @arg UART_INT_RX_IDLE: Rx idle interrupt source.
  * @param  NewState: new state of the specified UART interrupts.
  *     This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_INTConfig(UART_TypeDef *UARTx, uint32_t UART_INT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));
    assert_param(IS_UART_INT(UART_INT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (UART_INT & UART_INT_RX_IDLE)
    {
        if (NewState == ENABLE)
        {
            UARTx->RX_IDLE_INTSR |= BIT0;
            UARTx->STSR  |= BIT3;
            UARTx->STSR  &= (~BIT3);
            UARTx->RX_IDLE_INTCR |= BIT0;
            UARTx->RX_IDLE_TOCR |= BIT31;
        }
        else
        {
            UARTx->RX_IDLE_TOCR &= (~BIT31);
            UARTx->RX_IDLE_INTCR &= (~BIT0);
        }
    }

    if (UART_INT & UART_INT_RX_BREAK)
    {
        if (NewState == ENABLE)
        {
            UARTx->SPR |= BIT7;
        }
        else
        {
            UARTx->SPR &= (~BIT7);
        }
    }

    if (UART_INT & 0x3F)
    {
        if (NewState == ENABLE)
        {
            /* Enable the selected UARTx interrupts */
            UARTx->DLH_INTCR |= UART_INT;
        }
        else
        {
            /* Disable the selected UARTx interrupts */
            UARTx->DLH_INTCR &= (uint32_t)~UART_INT;
        }
    }
    return;
}

/**
  * @brief  Send data to tx FIFO.
  * @param  UARTx: selected UART peripheral.
  * @param  inBuf: buffer to be written to Tx FIFO.
  * @param  num: number of data to be written.
  * @retval None
  */
void UART_SendData(UART_TypeDef *UARTx, const uint8_t *inBuf, uint16_t num)
{
    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));

    while (num--)
    {
        UARTx->RB_THR = *inBuf++;
    }

    return;
}

/**
  * @brief  Receive data from rx FIFO.
  * @param  UARTx: selected UART peripheral.
  * @param[out]  outBuf: buffer to save data read from UART FIFO.
  * @param  count: number of data to be read.
  * @retval None
  */
void UART_ReceiveData(UART_TypeDef *UARTx, uint8_t *outBuf, uint16_t count)
{
    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));

    while (count--)
    {
        *outBuf++ = (uint8_t)UARTx->RB_THR;
    }

    return;
}

/**
  *@brief  Change UART baudrate.
  *@param  UARTx: selected UART peripheral.
  *@param  div: parameter of the selected UART baudrate.
  *@param  ovsr: parameter of the selected UART baudrate.
  *@param  ovsr_adj: parameter of the selected UART baudrate.
  *@retval None.
  */
void UART_SetBaudRate(UART_TypeDef *UARTx, uint16_t div, uint16_t ovsr, uint16_t ovsr_adj)
{
    //set baudrate, firstly set DLAB bit
    UARTx->LCR |= LCR_DLAB_Set;

    //set calibration parameters(OVSR)
    UARTx->STSR &= ~0xF0;
    UARTx->STSR |= (ovsr << 4);
    //set calibration parameters(OVSR_adj)
    UARTx->SPR &= (~(0x7ff << 16));
    UARTx->SPR |= (ovsr_adj << 16);
    //set DLL and DLH
    UARTx->DLL = (div & 0x00FF);
    UARTx->DLH_INTCR = ((div & 0xFF00) >> 8);

    //after set baudrate, clear DLAB bit
    UARTx->LCR &= LCR_DLAB_Reset;
}

/**
  *@brief  Set UART parameters.
  *@param  UARTx: selected UART peripheral.
  *@param  wordLen: data width of selected UART peripheral.
  *@param  parity: parity of selected UART peripheral.
  *@param  stopBits: stop bit of selected UART peripheral.
  *@retval None
  */
void UART_SetParams(UART_TypeDef *UARTx, uint16_t wordLen, uint16_t parity, uint16_t stopBits)
{
    //set LCR reg
    UARTx->LCR = (wordLen | stopBits | parity);
}

/**
  *@brief  UART loop back mode config.
  *@param  UARTx: selected UART peripheral.
  *@param  NewState: new state of the DMA Channelx.
  *        This parameter can be: ENABLE or DISABLE.
  *@retval None.
  */
void UART_LoopBackCmd(UART_TypeDef *UARTx, FunctionalState NewState)
{
    assert_param(IS_UART_PERIPH(UARTx));

    if (NewState == ENABLE)
    {
        UARTx->MCR |= BIT4;
    }
    else
    {
        UARTx->MCR &= ~BIT4;
    }
}

/**
  * @brief  Checks whether the specified UART flag is set or not.
  * @param  UARTx: selected UART peripheral.
  * @param  UART_FLAG: specifies the flag to check.
  *     This parameter can be one of the following values:
  *     @arg UART_FLAG_RX_DATA_RDY: rx data is avaliable.
  *     @arg UART_FLAG_RX_OVERRUN: rx overrun.
  *     @arg UART_FLAG_PARTY_ERR: parity error.
  *     @arg UART_FLAG_FRAME_ERR: UARTx frame error.
  *     @arg UART_FLAG_BREAK_ERR: UARTx break error.
  *     @arg UART_FLAG_THR_EMPTY: tx FIFO is empty.
  *     @arg UART_FLAG_THR_TSR_EMPTY: tx FIFO and tx shift reg are both empty.
  *     @arg UART_FLAG_RX_FIFO_ERR: rx FIFO error.
  *     @arg UART_FLAG_RX_IDLE.
  * @retval The new state of UART_FLAG (SET or RESET).
  */
FlagStatus UART_GetFlagStatus(UART_TypeDef *UARTx, uint32_t UART_FLAG)
{
    FlagStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));
    assert_param(IS_UART_GET_FLAG(UART_FLAG));

    if (UART_FLAG == UART_FLAG_TX_THD)
    {
        if (UARTx->TX_THR_INTSR & BIT(0))
        {
            bitstatus = SET;
        }
    }
    else if (UART_FLAG == UART_FLAG_TX_DONE)
    {
        if (UARTx->TX_DONE_INTSR & BIT(0))
        {
            bitstatus = SET;
        }
    }
    else if (UART_FLAG == UART_FLAG_RX_IDLE)
    {
        if (UARTx->RX_IDLE_INTSR & BIT(0))
        {
            bitstatus = SET;
        }
    }
    else
    {
        if (UARTx->LSR & UART_FLAG)
        {
            bitstatus = SET;
        }
    }

    return bitstatus;
}

///**
//  * @brief  Get interrupt identifier.
//  * @param  UARTx: selected UART peripheral.
//  * @retval The interrupt identifier value.
//  *   This return value can be one of the following values:
//  *     @arg UART_INT_ID_LINE_STATUS: interrupt identifier--line status interrupt.
//  *     @arg UART_INT_ID_RX_LEVEL_REACH: interrupt identifier--rx trigger level reached interrupt.
//  *     @arg UART_INT_ID_RX_TMEOUT: interrupt identifier--line status interrupt.
//  *     @arg UART_INT_ID_TX_EMPTY: interrupt identifier--line status interrupt.
//  *     @arg UART_INT_ID_MODEM_STATUS: interrupt identifier--line status interrupt.
//  */
//__STATIC_INLINE uint16_t UART_GetINTStatus(UART_TypeDef *UARTx)
//{
//    /* Check the parameters */
//    assert_param(IS_UART_PERIPH(UARTx));

//    return (uint16_t)(UARTx->INTID_FCR & (0x0000000E));
//}

/******************* (C) COPYRIGHT 2019 Realtek Semiconductor *****END OF FILE****/
