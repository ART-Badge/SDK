/**
*********************************************************************************************************
*               Copyright(c) 2019, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_gdma.c
* @brief    This file provides all the DMA firmware functions.
* @details
* @author   yuan
* @date     2019-11-14
* @version  v1.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtl876x_rcc.h"
#include "rtl876x_gdma.h"

/*fifo depth*/
#define UART_RX_FIFO_DEPTH      32
#define UART_TX_FIFO_DEPTH      16
#define SPI_RX_FIFO_DEPTH       36
#define SPI_TX_FIFO_DEPTH       36
#define I2C_RX_FIFO_DEPTH       16
#define I2C_TX_FIFO_DEPTH       24

#define REG_GDMA_TIMER          *((volatile uint32_t *)0x40006024UL)

/**
  * @brief  Initializes the GDMA Channelx according to the specified
  *         parameters in the GDMA_InitStruct.
  * @param  GDMA_Channelx: where x can be 0 to 7  to select the DMA Channel.
  * @param  GDMA_InitStruct: pointer to a GDMA_InitTypeDef structure that
  *         contains the configuration information for the specified DMA Channel.
  * @retval None
  */
void GDMA_Init(GDMA_ChannelTypeDef *GDMA_Channelx, GDMA_InitTypeDef *GDMA_InitStruct)
{
    uint32_t temp_bit = 0;
    uint32_t autoreload_temp;
    uint32_t llp_temp;

    /* Check the parameters */
    assert_param(IS_GDMA_ALL_PERIPH(GDMA_Channelx));
    assert_param(IS_GDMA_ChannelNum(GDMA_InitStruct->GDMA_ChannelNum));
    assert_param(IS_GDMA_DIR(GDMA_InitStruct->GDMA_DIR));
    assert_param(IS_GDMA_SourceInc(GDMA_InitStruct->GDMA_SourceInc));
    assert_param(IS_GDMA_DestinationInc(GDMA_InitStruct->GDMA_DestinationInc));
    assert_param(IS_GDMA_DATA_SIZE(GDMA_InitStruct->GDMA_SourceDataSize));
    assert_param(IS_GDMA_DATA_SIZE(GDMA_InitStruct->GDMA_DestinationDataSize));

    /*------------------configure source and destination address of GDMA---------*/
    /* program SARx register to set source address */
    GDMA_Channelx->SAR = GDMA_InitStruct->GDMA_SourceAddr;
    /* program DARx register to set destination address */
    GDMA_Channelx->DAR = GDMA_InitStruct->GDMA_DestinationAddr;

    /* Enable GDMA in DmaCfgReg*/
    GDMA_BASE->DmaCfgReg = 0x01;

    /* read ChEnReg to check channel is busy or not */
    if (GDMA_BASE->ChEnReg & BIT(GDMA_InitStruct->GDMA_ChannelNum))
    {
        //channel is be used
        //error handle code
        //while (1);
    }

    /*--------------------------- GDMA Configuration -----------------*/
    /* clear pending interrupts of corresponding GDMA channel */
    temp_bit = BIT(GDMA_InitStruct->GDMA_ChannelNum);
    GDMA_BASE->CLEAR_TFR |= temp_bit;
    GDMA_BASE->CLEAR_BLOCK |= temp_bit;
    GDMA_BASE->CLEAR_DST_TRAN |= temp_bit;
    GDMA_BASE->CLEAR_SRC_TRAN |= temp_bit;
    GDMA_BASE->CLEAR_ERR |= temp_bit;

    /* mask pending interrupts of corresponding GDMA channel */
    temp_bit = BIT(GDMA_InitStruct->GDMA_ChannelNum + 8);
    GDMA_BASE->MASK_TFR = temp_bit;
    GDMA_BASE->MASK_BLOCK = temp_bit;
    GDMA_BASE->MASK_DST_TRAN = temp_bit;
    GDMA_BASE->MASK_SRC_TRAN = temp_bit;
    GDMA_BASE->MASK_ERR = temp_bit;

    /*---------------------------- configure CTL register --------------------------------*/

    /* configure low 32 bit of CTL register */
    GDMA_Channelx->CTL_LOW = BIT(0)
                             | (GDMA_InitStruct->GDMA_DestinationDataSize << 1)
                             | (GDMA_InitStruct->GDMA_SourceDataSize << 4)
                             | (GDMA_InitStruct->GDMA_DestinationInc << 7)
                             | (GDMA_InitStruct->GDMA_SourceInc << 9)
                             | (GDMA_InitStruct->GDMA_DestinationMsize << 11)
                             | (GDMA_InitStruct->GDMA_SourceMsize << 14)
                             | (GDMA_InitStruct->GDMA_DIR << 20);
    /* configure high 32 bit of CTL register */
    GDMA_Channelx->CTL_HIGH = GDMA_InitStruct->GDMA_BufferSize;

    /*---------------------------- configure CFG register --------------------------------*/

    switch (GDMA_InitStruct->GDMA_DIR)
    {
    case GDMA_DIR_MemoryToMemory:
        GDMA_Channelx->CFG_LOW = (0x03 << 10);
        break;
    case GDMA_DIR_MemoryToPeripheral:
        GDMA_Channelx->CFG_LOW = BIT11;
        break;
    case GDMA_DIR_PeripheralToMemory:
        GDMA_Channelx->CFG_LOW = BIT10;
        break;
    case GDMA_DIR_PeripheralToPeripheral:
        GDMA_Channelx->CFG_LOW = 0;
        break;
    default:
        GDMA_Channelx->CFG_LOW = 0;
        break;
    }

    //set for scatter and gather
    if ((GDMA_InitStruct->GDMA_Gather_En == 1) && (GDMA_Channelx == GDMA_Channel4))  //GDMA_GatherCount
    {
        GDMA_Channelx->CTL_LOW |= BIT(17);
        GDMA_Channelx->SGR = GDMA_InitStruct->GDMA_GatherCount << 20 |
                             GDMA_InitStruct->GDMA_GatherInterval;
    }
    if ((GDMA_InitStruct->GDMA_Scatter_En == 1) && (GDMA_Channelx == GDMA_Channel4))
    {
        GDMA_Channelx->CTL_LOW |= BIT(18);
        GDMA_Channelx->DSR = GDMA_InitStruct-> GDMA_ScatterCount << 20 |
                             GDMA_InitStruct->GDMA_ScatterInterval;
    }
    if (GDMA_InitStruct->GDMA_Multi_Block_En == 1 &&
        ((GDMA_Channelx == GDMA_Channel0) | (GDMA_Channelx == GDMA_Channel1) |
         (GDMA_Channelx == GDMA_Channel2) | (GDMA_Channelx == GDMA_Channel3)))
    {
        /* Config multi-block mode */
        GDMA_Channelx->CFG_LOW &= ~AUTO_RELOAD_SELECTED_BIT;
        GDMA_Channelx->CTL_LOW &= ~LLP_SELECTED_BIT;
        autoreload_temp = (GDMA_InitStruct->GDMA_Multi_Block_Mode & AUTO_RELOAD_SELECTED_BIT);
        llp_temp = (GDMA_InitStruct->GDMA_Multi_Block_Mode & LLP_SELECTED_BIT);
        if (llp_temp)
        {
            GDMA_Channelx->LLP = GDMA_InitStruct->GDMA_Multi_Block_Struct;
            GDMA_Channelx->CTL_LOW |= llp_temp;
        }
        GDMA_Channelx->CFG_LOW |= autoreload_temp;
    }

    /* configure peripheral parameters and configure source or destination hardware handshaking interface */
    GDMA_Channelx->CFG_HIGH &=  ~(0x03ff << 7);

    GDMA_Channelx->CFG_HIGH |= (((((GDMA_InitStruct->GDMA_SourceHandshake) & 0x10) << 4) \
                                 | ((GDMA_InitStruct->GDMA_SourceHandshake) & 0x0f)) << 7);
    GDMA_Channelx->CFG_HIGH |= (((((GDMA_InitStruct->GDMA_DestHandshake) & 0x10) << 1) \
                                 | ((GDMA_InitStruct->GDMA_DestHandshake) & 0x0f)) << 11);

    /* Set Vendor register about timer handshake */
    if (GDMA_InitStruct->GDMA_DestHandshake >= 29)
    {
        REG_GDMA_TIMER |= BIT(GDMA_InitStruct->GDMA_DestHandshake - 8);
    }

    /* Enable FIFO mode and Flow control mode */
    GDMA_Channelx->CFG_HIGH &= ~0x03;
    GDMA_Channelx->CFG_HIGH |= 0x02;

    /* ---------------clear pending interrupts of corresponding GDMA channel------------------ */
    temp_bit = BIT(GDMA_InitStruct->GDMA_ChannelNum);
    GDMA_BASE->CLEAR_TFR |= temp_bit;
    GDMA_BASE->CLEAR_BLOCK |= temp_bit;
    GDMA_BASE->CLEAR_DST_TRAN |= temp_bit;
    GDMA_BASE->CLEAR_SRC_TRAN |= temp_bit;
    GDMA_BASE->CLEAR_ERR |= temp_bit;
}

/**
  * @brief  Deinitializes the GDMA registers to their default reset
  *         values.
  * @param  None
  * @retval None
  */
void GDMA_DeInit(void)
{
    /* Disable GDMA block in REG_SOC_FUNC_EN */
    RCC_PeriphClockCmd(APBPeriph_GDMA, APBPeriph_GDMA_CLOCK, DISABLE);
}

/**
  * @brief  Fills each GDMA_InitStruct member with its default value.
  * @param  GDMA_InitStruct : pointer to a GDMA_InitTypeDef structure which will
  *         be initialized.
  * @retval None
  */
void GDMA_StructInit(GDMA_InitTypeDef *GDMA_InitStruct)
{
    /*-------------- Reset DMA init structure parameters values ------------------*/
    GDMA_InitStruct->GDMA_ChannelNum      = 0;
    GDMA_InitStruct->GDMA_DIR             = GDMA_DIR_PeripheralToMemory;
    GDMA_InitStruct->GDMA_BufferSize      = 200;
    GDMA_InitStruct->GDMA_SourceInc       = DMA_SourceInc_Fix;
    GDMA_InitStruct->GDMA_DestinationInc  = DMA_DestinationInc_Inc;

    GDMA_InitStruct->GDMA_SourceDataSize  = GDMA_DataSize_Byte;
    GDMA_InitStruct->GDMA_DestinationDataSize = GDMA_DataSize_Byte;
    GDMA_InitStruct->GDMA_SourceMsize     = GDMA_Msize_1;
    GDMA_InitStruct->GDMA_DestinationMsize  = GDMA_Msize_1;

    GDMA_InitStruct->GDMA_SourceAddr      = 0;
    GDMA_InitStruct->GDMA_DestinationAddr = 0;
    GDMA_InitStruct->GDMA_ChannelPriority = 0;

    GDMA_InitStruct->GDMA_Multi_Block_Mode = LLI_TRANSFER;

    GDMA_InitStruct->GDMA_SourceHandshake = 0;
    GDMA_InitStruct->GDMA_DestHandshake   = 0;

    GDMA_InitStruct->GDMA_Multi_Block_En  = DISABLE;
    GDMA_InitStruct->GDMA_Scatter_En      = DISABLE;
    GDMA_InitStruct->GDMA_Gather_En       = DISABLE;
    GDMA_InitStruct->GDMA_Multi_Block_Struct = 0;
    GDMA_InitStruct->GDMA_ScatterCount = 0;
    GDMA_InitStruct->GDMA_ScatterInterval = 0;
    GDMA_InitStruct->GDMA_GatherCount = 0;
    GDMA_InitStruct->GDMA_GatherInterval = 0;
}

/**
  * @brief  Enables or disables the specified GDMA Channelx.
  * @param  GDMA_Channelx: x can be 0 to 6 to select the DMA Channel.
  * @param  NewState: new state of the DMA Channelx.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void GDMA_Cmd(uint8_t GDMA_ChannelNum, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_GDMA_ChannelNum(GDMA_ChannelNum));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the selected DMAy Channelx */
        GDMA_BASE->ChEnReg |= BIT(GDMA_ChannelNum) | BIT(GDMA_ChannelNum + 8);
    }
    else
    {
        /* Disable the selected DMAy Channelx */
        GDMA_BASE->ChEnReg = BIT(GDMA_ChannelNum + 8);
    }
}

/**
  * @brief  Enables or disables the specified DMAy Channelx interrupts.
  * @param  GDMA_Channelx: where x can be 0 to 6 to select the GDMA Channel.
  * @param  GDMA_IT: specifies the GDMA interrupts sources to be enabled
  *   or disabled.
  *   This parameter can be any combination of the following values:
  *     @arg GDMA_INT_Transfer:  Transfer complete interrupt unmask
  *     @arg GDMA_INT_Block:  Block transfer interrupt unmask
  *     @arg GDMA_INT_SrcTransfer:  SourceTransfer interrupt unmask
  *     @arg GDMA_INT_DstTransfer:  Destination Transfer interrupt unmask
  *     @arg GDMA_INT_Error:  Transfer error interrupt unmask
  * @param  NewState: new state of the specified DMA interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void GDMA_INTConfig(uint8_t GDMA_ChannelNum, uint32_t GDMA_IT, FunctionalState NewState)
{
    uint32_t temp_bit = 0;

    /* Check the parameters */
    assert_param(IS_GDMA_ChannelNum(GDMA_ChannelNum));
    assert_param(IS_GDMA_CONFIG_IT(GDMA_IT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the selected DMA interrupts */
        temp_bit = BIT(GDMA_ChannelNum) | BIT(GDMA_ChannelNum + 8);

        if (GDMA_IT & GDMA_INT_Transfer)
        {
            GDMA_BASE->MASK_TFR |= temp_bit;
        }
        if (GDMA_IT & GDMA_INT_Block)
        {
            GDMA_BASE->MASK_BLOCK |= temp_bit;
        }
        if (GDMA_IT & GDMA_INT_SrcTransfer)
        {
            GDMA_BASE->MASK_SRC_TRAN |= temp_bit;
        }
        if (GDMA_IT & GDMA_INT_DstTransfer)
        {
            GDMA_BASE->MASK_DST_TRAN |= temp_bit;
        }
        if (GDMA_IT & GDMA_INT_Error)
        {
            GDMA_BASE->MASK_ERR |= temp_bit;
        }
    }
    else
    {
        /* Disable the selected DMA interrupts */
        temp_bit = BIT(GDMA_ChannelNum + 8);
        if (GDMA_IT & GDMA_INT_Transfer)
        {
            GDMA_BASE->MASK_TFR = temp_bit;
        }
        if (GDMA_IT & GDMA_INT_Block)
        {
            GDMA_BASE->MASK_BLOCK = temp_bit;
        }
        if (GDMA_IT & GDMA_INT_SrcTransfer)
        {
            GDMA_BASE->MASK_SRC_TRAN = temp_bit;
        }
        if (GDMA_IT & GDMA_INT_DstTransfer)
        {
            GDMA_BASE->MASK_DST_TRAN = temp_bit;
        }
        if (GDMA_IT & GDMA_INT_Error)
        {
            GDMA_BASE->MASK_ERR = temp_bit;
        }
    }
}

/**
  * @brief  Enables or disables the specified DMAy Channelx interrupts.
  * @param  GDMA_Channelx: where x can be 0 to 6 to select the GDMA Channel.
  * @param  GDMA_IT: specifies the GDMA interrupts sources to be enabled
  *   or disabled.
  *   This parameter can be any combination of the following values:
  *     @arg GDMA_INT_Transfer:  clear transfer complete interrupt
  *     @arg GDMA_INT_Block:  clear Block transfer interrupt
  *     @arg GDMA_INT_SrcTransfer:  clear SourceTransfer interrupt
  *     @arg GDMA_INT_DstTransfer:  clear Destination Transfer interrupt
  *     @arg GDMA_INT_Error:  clear Transfer error interrupt
  * @param  NewState: new state of the specified DMA interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void GDMA_ClearINTPendingBit(uint8_t GDMA_ChannelNum, uint32_t GDMA_IT)
{
    uint32_t temp_bit = 0;

    /* Check the parameters */
    assert_param(IS_GDMA_ChannelNum(GDMA_ChannelNum));
    assert_param(IS_GDMA_CONFIG_IT(GDMA_IT));

    /* clear the selected DMA interrupts */
    //temp_bit = BIT(GDMA_ChannelNum) | BIT(GDMA_ChannelNum + 8);
    temp_bit = BIT(GDMA_ChannelNum);

    if (GDMA_IT & GDMA_INT_Transfer)
    {
        GDMA_BASE->CLEAR_TFR = temp_bit;
    }
    if (GDMA_IT & GDMA_INT_Block)
    {
        GDMA_BASE->CLEAR_BLOCK = temp_bit;
    }
    if (GDMA_IT & GDMA_INT_SrcTransfer)
    {
        GDMA_BASE->CLEAR_SRC_TRAN = temp_bit;
    }
    if (GDMA_IT & GDMA_INT_DstTransfer)
    {
        GDMA_BASE->CLEAR_DST_TRAN = temp_bit;
    }
    if (GDMA_IT & GDMA_INT_Error)
    {
        GDMA_BASE->CLEAR_ERR = temp_bit;
    }
}



/******************* (C) COPYRIGHT 2019 Realtek Semiconductor Corporation *****END OF FILE****/

