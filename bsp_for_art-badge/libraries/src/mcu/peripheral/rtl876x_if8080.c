/**
*********************************************************************************************************
*            Copyright(c) 2019, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file      rtl876x_if8080.c
* @brief     This file provides all the 8080 parallel interface firmware functions.
* @details
* @author    elliot chen
* @date      2019-10-10
* @version   v1.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtl876x_rcc.h"
#include "rtl876x_if8080.h"

/* Internal defines -----------------------------------------------------------*/
#define IF8080_REG_TEST_MODE           *((volatile uint32_t *)0x400002A8UL)

/**
  * @brief  Deinitializes the IF8080 peripheral registers to their default values.
  * @param  None.
  * @retval None
  */
void IF8080_DeInit(void)
{
    RCC_PeriphClockCmd(APBPeriph_IF8080, APBPeriph_IF8080_CLOCK, DISABLE);
}

/**
  * @brief Select the IF8080 output pin group.
  * @param IF8080_PinGroupType:
    This parameter can be one of the following values:
    --IF8080_PinGroup_DISABLE : disable IF8080 interface
    --IF8080_PinGroup_1 : CS(P3_3), DCX(P3_4), WR(P3_2), RD(P2_0), Vsync(P1_2)
      D0(P3_5), D1(P0_1), D2(P0_2), D3(P0_4), D4(P4_0), D5(P4_1), D6(P4_2), D7(P4_3)
    --IF8080_PinGroup_2 : CS(P0_0), DCX(P1_5), WR(P0_2), RD(P1_6), Vsync(P0_1)
      D0(P0_4), D1(P0_5), D2(P0_6), D3(P0_7), D4(P4_0), D5(P4_1), D6(P4_2), D7(P4_3)
  * @retval None
  */
void IF8080_PinGroupConfig(uint32_t IF8080_PinGroupType)
{
    /* Check the parameters */
    assert_param(IS_IF8080_PINGROUP_IDX(IF8080_PinGroupType));

    /* Dedicated IF8080 pin option */
    IF8080_REG_TEST_MODE &= ~(0x03 << 28);
    IF8080_REG_TEST_MODE |= IF8080_PinGroupType;
}

/**
  * @brief  Initializes the IF8080 peripheral according to the specified
  *   parameters in the IF8080_InitStruct
  * @param  IF8080: selected IF8080 peripheral.
  * @param  IF8080_InitStruct: pointer to a IF8080_InitTypeDef structure that
  *   contains the configuration information for the specified IF8080 peripheral
  * @retval None
  */
void IF8080_Init(IF8080_InitTypeDef *IF8080_InitStruct)
{
    /* Check the parameters */
    assert_param(IS_IF8080_CLOCK_DIV(IF8080_InitStruct->IF8080_ClockDiv));
    assert_param(IS_IF8080_MODE(IF8080_InitStruct->IF8080_Mode));
    assert_param(IS_IF8080_Auto_Mode_DIR(IF8080_InitStruct->IF8080_AutoModeDirection));
    assert_param(IS_IF8080_GUARD_TIME_CMD(IF8080_InitStruct->IF8080_InitGuardTimeCmd));
    assert_param(IS_IF8080_GUARD_TIME(IF8080_InitStruct->IF8080_InitGuardTime));
    assert_param(IS_IF8080_GUARD_TIME_CMD(IF8080_InitStruct->IF8080_CmdGuardTimeCmd));
    assert_param(IS_IF8080_GUARD_TIME(IF8080_InitStruct->IF8080_CmdGuardTime));
    assert_param(IS_IF8080_GUARD_TIME_CMD(IF8080_InitStruct->IF8080_GuardTimeCmd));
    assert_param(IS_IF8080_GUARD_TIME(IF8080_InitStruct->IF8080_GuardTime));
    assert_param(IS_IF8080_WR_DELAY_TIME(IF8080_InitStruct->IF8080_WRDelay));
    assert_param(IS_IF8080_8BitSwap_CMD(IF8080_InitStruct->IF8080_8BitSwap));
    assert_param(IS_IF8080_16BitSwap_CMD(IF8080_InitStruct->IF8080_16BitSwap));
    assert_param(IS_IF8080_TX_DMA_CMD(IF8080_InitStruct->IF8080_TxDMACmd));
    assert_param(IS_IF8080_VSYNC_CMD(IF8080_InitStruct->IF8080_VsyncCmd));
    assert_param(IS_IF8080_VSYNC_POLARITY(IF8080_InitStruct->IF8080_VsyncPolarity));

    /* Configure Tx FIFO threshold and mask interrupt */
    IF8080->IMR = IF8080_InitStruct->IF8080_TxThr |
                  IF8080_MASK_TX_FIFO_THR_INT_MSK | IF8080_MASK_TX_FIFO_OVERFLOW_INT_MSK | \
                  IF8080_MASK_TX_FIFO_EMPTY_INT_MSK | IF8080_MASK_TX_AUTO_DONE_INT_MSK | \
                  IF8080_MASK_RX_FIFO_OVERFLOW_INT_MSK | IF8080_MASK_RX_AUTO_DONE_INT_MSK | \
                  IF8080_MASK_VSYNC_INT_MSK;

    /* Configure clock divider, bypass guard time, frame format */
    IF8080->CTRL0 = IF8080_InitStruct->IF8080_ClockDiv | \
                    IF8080_InitStruct->IF8080_InitGuardTime | IF8080_InitStruct->IF8080_InitGuardTimeCmd | \
                    IF8080_InitStruct->IF8080_CmdGuardTime | IF8080_InitStruct->IF8080_CmdGuardTimeCmd | \
                    IF8080_InitStruct->IF8080_GuardTime | IF8080_InitStruct->IF8080_GuardTimeCmd | \
                    IF8080_InitStruct->IF8080_8BitSwap | IF8080_InitStruct->IF8080_16BitSwap | \
                    IF8080_InitStruct->IF8080_AutoModeDirection | \
                    IF8080_InitStruct->IF8080_VsyncCmd | IF8080_InitStruct->IF8080_VsyncPolarity | \
                    IF8080_InitStruct->IF8080_Mode;

    /* Configure DMA parameter */
    IF8080->CTRL1 = IF8080_TX_DMA_FIFO_THR | IF8080_InitStruct->IF8080_TxDMACmd;

    /* Configure IF8080 mode */
    IF8080->CFG = IF8080_InitStruct->IF8080_WRDelay | IF8080_MANUAL_CS_CTRL_MSK;

    /* Clear Tx data counter */
    IF8080->ICR = IF8080_CLR_TX_OUTPUT_CNT_MSK | IF8080_CLR_RX_OUTPUT_CNT_MSK;
}

/**
  * @brief  Fills each IF8080_InitStruct member with its default value.
  * @param  IF8080_InitStruct: pointer to an IF8080_InitTypeDef structure which will be initialized.
  * @retval None
  */
void IF8080_StructInit(IF8080_InitTypeDef *IF8080_InitStruct)
{
    IF8080_InitStruct->IF8080_ClockDiv          = IF8080_CLOCK_DIV_4;
    IF8080_InitStruct->IF8080_Mode              = IF8080_MODE_MANUAL;
    IF8080_InitStruct->IF8080_AutoModeDirection = IF8080_Auto_Mode_Direction_WRITE;
    IF8080_InitStruct->IF8080_InitGuardTimeCmd  = IF8080_INIT_GUARD_TIME_DISABLE;
    IF8080_InitStruct->IF8080_InitGuardTime     = IF8080_INIT_GUARD_TIME_1T;
    IF8080_InitStruct->IF8080_CmdGuardTimeCmd   = IF8080_CMD_GUARD_TIME_DISABLE;
    IF8080_InitStruct->IF8080_CmdGuardTime      = IF8080_CMD_GUARD_TIME_1T;
    IF8080_InitStruct->IF8080_GuardTimeCmd      = IF8080_GUARD_TIME_DISABLE;
    IF8080_InitStruct->IF8080_GuardTime         = IF8080_GUARD_TIME_1T;
    IF8080_InitStruct->IF8080_WRDelay           = IF8080_WR_DELAY_TIME_HALF_BUS_CLK_CYCLE;
    IF8080_InitStruct->IF8080_8BitSwap          = IF8080_8BitSwap_DISABLE;
    IF8080_InitStruct->IF8080_16BitSwap         = IF8080_16BitSwap_DISABLE;
    IF8080_InitStruct->IF8080_TxThr             = 10;
    IF8080_InitStruct->IF8080_TxDMACmd          = IF8080_TX_DMA_DISABLE;
    IF8080_InitStruct->IF8080_VsyncCmd          = IF8080_VSYNC_DISABLE;
    IF8080_InitStruct->IF8080_VsyncPolarity     = IF8080_VSYNC_POLARITY_FALLING;
}

/**
  * @brief  Enable or disable the selected IF8080 operation in auto mode.
  * @param  IF8080_Direction: the IF8080 read or write operation type.
  * @param  NewState: new state of the operation mode.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void IF8080_AutoModeCmd(uint32_t IF8080_Direction, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        if (IF8080_Direction == IF8080_Auto_Mode_Direction_WRITE)
        {
            IF8080->CTRL0 &= IF8080_AUTO_MODE_RW_SEL_CLR;
            IF8080->CTRL0 |= IF8080_AUTO_WRITE_START_MSK;
        }
        else if (IF8080_Direction == IF8080_Auto_Mode_Direction_READ)
        {
            IF8080->CTRL0 |= IF8080_Direction;
            IF8080->CTRL0 |= IF8080_AUTO_READ_START_MSK;
        }
    }
    else
    {
        if (IF8080_Direction == IF8080_Auto_Mode_Direction_WRITE)
        {
            IF8080->CTRL0 &= IF8080_AUTO_WRITE_START_CLR;
        }
        else if (IF8080_Direction == IF8080_Auto_Mode_Direction_READ)
        {
            IF8080->CTRL0 &= IF8080_AUTO_READ_START_CLR;
        }
    }
}

/**
  * @brief  Send command in manual mode.
  * @param  cmd: command which to be sent.
  * @retval None
  */
void IF8080_SendCommand(uint8_t cmd)
{
    /* Fill command */
    IF8080->CMD1 = cmd;

    /* Enable command control */
    IF8080->CFG &= IF8080_CMD_DATA_CTRL_CLR;

    /* Send command */
    IF8080->CFG |= IF8080_WR_TRIGGER_CTRL_MSK;

    /* Check write status */
    while ((IF8080->CFG) & IF8080_WR_TRIGGER_CTRL_MSK);
}

/**
  * @brief  Send data in manual mode.
  * @param  pBuf: buffer address to be sent.
  * @param  len:  data length.
  * @retval None
  */
void IF8080_SendData(uint8_t *pBuf, uint32_t len)
{
    while (len--)
    {
        /* Fill data */
        IF8080->CMD1 = *pBuf++;

        /* Send command */
        IF8080->CFG |= IF8080_CMD_DATA_CTRL_MSK | IF8080_WR_TRIGGER_CTRL_MSK;

        /* Check write status */
        while ((IF8080->CFG) & IF8080_WR_TRIGGER_CTRL_MSK);
    }
}

/**
  * @brief  Receive data in manual mode.
  * @param  pBuf: buffer address to be received.
  * @param  len: data length.
  * @retval None
  */
void IF8080_ReceiveData(uint8_t *pBuf, uint32_t len)
{
    uint32_t reg_value = 0;

    while (len--)
    {
        /* Select read data control */
        IF8080->CFG |= IF8080_CMD_DATA_CTRL_MSK | IF8080_RD_TRIGGER_CTRL_MSK;

        /* Check read status */
        while ((IF8080->CFG) & IF8080_RD_TRIGGER_CTRL_MSK);

        /* Read data */
        reg_value = IF8080->RXDATA;
        if (reg_value & IF8080_RX_DATA_VALID_MSK)
        {
            *pBuf++ = reg_value & IF8080_RX_DATA_MSK;
        }
    }
}

/**
  * @brief  Send command and data buffer in manual mode.
  * @param  cmd: command which to be sent.
  * @param  pBuf: buffer address to be sent.
  * @param  len:  data length.
  * @retval None
  */
void IF8080_Write(uint8_t cmd, uint8_t *pBuf, uint32_t len)
{
    /* Pull CS down */
    IF8080_ResetCS();

    /* Send command */
    IF8080_SendCommand(cmd);

    /* Write data */
    IF8080_SendData(pBuf, len);

    /* Pull CS up */
    IF8080_SetCS();
}

/**
  * @brief  Send command and read data buffer in manual mode.
  * @param  cmd: command which to be sent.
  * @param  pBuf: buffer address to be sent.
  * @param  len:  data length.
  * @retval None
  */
void IF8080_Read(uint8_t cmd, uint8_t *pBuf, uint32_t len)
{
    /* Pull CS down */
    IF8080_ResetCS();

    /* Send command */
    IF8080_SendCommand(cmd);

    /* Read data */
    IF8080_ReceiveData(pBuf, len);

    /* Pull CS up */
    IF8080_SetCS();
}

/**
  * @brief  Configure command sequences in auto mode.
  * @param  pCmdBuf: buffer address which store command sequence.
  * @param  len:  command length.
  * @retval None
  */
FlagStatus IF8080_SetCmdSequence(uint8_t *pCmdBuf, uint8_t len)
{
    uint32_t i = 0;

    /* Check parameters */
    if ((len > 12) || (len < 1))
    {
        return RESET;
    }

    /* Configure command number */
    IF8080->CFG &= IF8080_AUTO_MODE_CMD_NUM_CLR;
    IF8080->CFG |= (len - 1) & IF8080_AUTO_MODE_CMD_NUM_MSK;

    /* Fill command sequence */
    IF8080->CMD1 = 0;
    IF8080->CMD2 = 0;
    IF8080->CMD3 = 0;
    for (i = 0; i < len; i++)
    {
        if (i < 4)
        {
            IF8080->CMD1 |= (*pCmdBuf++) << (i * 8);
        }
        else if (i < 8)
        {
            IF8080->CMD2 |= (*pCmdBuf++) << ((i - 4) * 8);
        }
        else
        {
            IF8080->CMD3 |= (*pCmdBuf++) << ((i - 8) * 8);
        }
    }
    return SET;
}

/**
  * @brief  Mask or unmask the specified IF8080 interrupts.
  * @param  IF8080_INT_MSK: specifies the IF8080 interrupts sources to be mask or unmask.
  *   This parameter can be the following values: IF8080_INT_RF_OF_MSK
  *     @arg IF8080_INT_VSYNC_MSK: Mask Vsync trigger interrupt.
  *     @arg IF8080_INT_RX_AUTO_DONE_MSK: Mask RX auto done interrupt.
  *     @arg IF8080_INT_RF_OF_MSK: Mask RX FIFO overflow interrupt.
  *     @arg IF8080_INT_TX_AUTO_DONE_MSK: Mask TX auto done interrupt.
  *     @arg IF8080_INT_TF_EMPTY_MSK: Mask TX FIFO empty interrupt.
  *     @arg IF8080_INT_TF_OF_MSK: Mask TX FIFO overflow interrupt.
  *     @arg IF8080_INT_TF_LEVEL_MSK: Mask TX FIFO threshold interrupt.
  * @param  NewState: new state of the specified IF8080 interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void IF8080_MaskINTConfig(uint32_t IF8080_INT_MSK, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_IF8080_INT_MSK_CONFIG(IF8080_INT_MSK));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState == ENABLE)
    {
        IF8080->IMR |= IF8080_INT_MSK;
    }
    else
    {
        IF8080->IMR &= ~IF8080_INT_MSK;
    }
}

/**
  * @brief  Get the specified IF8080 interrupt status.
  * @param  IF8080_INT: the specified IF8080 interrupts.
  * This parameter can be one of the following values:
  *     @arg IF8080_INT_SR_VSYNC: Vsync interrupt.
  *     @arg IF8080_INT_SR_RX_AUTO_DONE: RX auto done interrupt.
  *     @arg IF8080_INT_SR_RF_OF: RX FIFO overflow interrupt.
  *     @arg IF8080_INT_SR_TX_AUTO_DONE: TX auto done interrupt.
  *     @arg IF8080_INT_SR_TF_EMPTY: TX FIFO empty interrupt.
  *     @arg IF8080_INT_SR_TF_OF: TX FIFO overflow interrupt.
  *     @arg IF8080_INT_SR_TF_LEVEL: TX FIFO threshold interrupt.
  * @retval The new state of IF8080_INT (SET or RESET).
  */
ITStatus IF8080_GetINTStatus(uint32_t IF8080_INT)
{
    ITStatus bit_status = RESET;

    /* Check the parameters */
    assert_param(IS_IF8080_INT_CONFIG(IF8080_INT));

    if (IF8080->SR & IF8080_INT)
    {
        bit_status = SET;
    }

    /* Return the IF8080_INT status */
    return  bit_status;
}

/**
  * @brief  Get the specified IF8080 flag status.
  * @param  IF8080_INT: the specified IF8080 flag.
  * This parameter can be one of the following values:
  *   This parameter can be the following values:
  *     @arg IF8080_FLAG_RF_EMPTY: Rx FIFO empty flag.
  *     @arg IF8080_FLAG_RF_FULL:  Rx FIFO full flag.
  *     @arg IF8080_FLAG_TF_EMPTY: FIFO empty flag.
  *     @arg IF8080_FLAG_TF_FULL:  Tx FIFO full flag.
  * @retval The new state of IF8080_FLAG (SET or RESET).
  */
FlagStatus IF8080_GetFlagStatus(uint32_t IF8080_FLAG)
{
    FlagStatus bit_status = RESET;

    /* Check the parameters */
    assert_param(IS_IF8080_FLAG_CONFIG(IF8080_FLAG));

    if (IF8080->SR & IF8080_FLAG)
    {
        bit_status = SET;
    }

    /* Return the IF8080_FLAG status */
    return  bit_status;
}

/**
 * @brief  Configure IF8080 multi-block LLI parameters.
 * @param  IF8080_LLIGroupx: pointer to a IF8080_GDMALLITypeDef structure that
 *   contains the configuration information for the If8080 GDMA peripheral.
 * @param  IF8080_LLP: pointer to a IF8080_GDMALLITypeDef structure that
 *   contains the configuration information for the GDMA transmission.
 * @retval None
 */
void IF8080_GDMALLIParamConfig(IF8080_GDMALLITypeDef *IF8080_LLIGroupx,
                               IF8080_GDMALLITypeDef *IF8080_LLP)
{
    IF8080_LLIGroupx->SAR      = IF8080_LLP->SAR;
    IF8080_LLIGroupx->DAR      = IF8080_LLP->DAR;
    IF8080_LLIGroupx->LLP      = IF8080_LLP->LLP;
    IF8080_LLIGroupx->CTL_LOW  = IF8080_LLP->CTL_LOW;
    IF8080_LLIGroupx->CTL_HIGH = IF8080_LLP->CTL_HIGH;
}

/**
 * @brief  Configure IF8080 multi-block LLI offset parameters.
 * @param  IF8080_LLIOffsetGroupx: pointer to a IF8080_GDMALLIOFTTypeDef structure that
 *   contains the configuration information for the If8080 GDMA peripheral.
 * @param  IF8080_LLPOffset: pointer to a IF8080_GDMALLIOFTTypeDef structure that
 *   contains the configuration information for the GDMA transmission.
 * @retval None
 */
void IF8080_GDMALLIOffsetConfig(IF8080_GDMALLIOFTTypeDef *IF8080_LLIOffsetGroupx,
                                IF8080_GDMALLIOFTTypeDef *IF8080_LLPOffset)
{
    IF8080_LLIOffsetGroupx->SAR_OFT = IF8080_LLPOffset->SAR_OFT;
    IF8080_LLIOffsetGroupx->DAR_OFT = IF8080_LLPOffset->DAR_OFT;
}

/**
 * @brief  Configure LLI number.
 * @param  LLI_loop_num: number of LLI.
 * @retval None
 */
void IF8080_GDMALLINumberConfig(uint16_t LLI_loop_num)
{
    IF8080_GDMA->CR &= IF8080_GDMA_LOOP_NUM_CLR;
    IF8080_GDMA->CR |= (LLI_loop_num << IF8080_GDMA_LOOP_NUM_POS) & IF8080_GDMA_LOOP_NUM_MSK;
}

/**
 * @brief  Configure Last LLI address.
 * @param  addr: address of last LLI.
 * @retval None
 */
void IF8080_GDMALastLLIConfig(uint32_t addr)
{
    IF8080_GDMA->LLI = addr;
}

/**
 * @brief  Configure IF8080 multi-block LLI parameters.
 * @param  IF8080_LLIGroup1: pointer to a IF8080_GDMALLITypeDef structure that
 *   contains the group 1 configuration information for the If8080 GDMA peripheral.
 * @param  IF8080_LLIGroup2: pointer to a IF8080_GDMALLITypeDef structure that
 *   contains the group 2 configuration information for the If8080 GDMA peripheral.
 * @param  IF8080_LLP_Group1Offset: pointer to a IF8080_GDMALLIOFTTypeDef structure that
 *   contains the group 1 offset configuration information for the GDMA transmission.
 * @param  IF8080_LLP_Group2Offset: pointer to a IF8080_GDMALLIOFTTypeDef structure that
 *   contains the group 2 offset configuration information for the GDMA transmission.
 * @param  LLI_loop_num: number of LLI.
 * @param  addr: address of last LLI.
 * @retval None
 */
void IF8080_GDMALLIConfig(IF8080_GDMALLITypeDef *IF8080_LLPGroup1,
                          IF8080_GDMALLITypeDef *IF8080_LLPGroup2,
                          IF8080_GDMALLIOFTTypeDef *IF8080_LLPGroup1Offset,
                          IF8080_GDMALLIOFTTypeDef *IF8080_LLPGroup2Offset,
                          uint16_t LLI_loop_num,
                          uint32_t last_LLI_addr)
{
    /* Disable WP */
    IF8080_GDMA->CR &= IF8080_GDMA_G1_WP_CLR;
    IF8080_GDMA->CR &= IF8080_GDMA_G2_WP_CLR;

    /* Clear loop counter */
    IF8080_GDMA->CR |= IF8080_GDMA_LOAD_CNT_CLR_MSK;

    IF8080_GDMALLIParamConfig(IF8080_LLPGROUP1, IF8080_LLPGroup1);
    IF8080_GDMALLIParamConfig(IF8080_LLPGROUP2, IF8080_LLPGroup2);
    IF8080_LLPGROUP1->LLP = (uint32_t)IF8080_LLI_REG2_GDMA_BASE;
    IF8080_LLPGROUP2->LLP = (uint32_t)IF8080_LLI_REG1_GDMA_BASE;

    IF8080_GDMALLIOffsetConfig(IF8080_LLPGROUP1_OFT, IF8080_LLPGroup1Offset);
    IF8080_GDMALLIOffsetConfig(IF8080_LLPGROUP2_OFT, IF8080_LLPGroup2Offset);
    IF8080_GDMALLINumberConfig(LLI_loop_num);
    IF8080_GDMALastLLIConfig(last_LLI_addr);

    /* Enable WP */
    IF8080_GDMA->CR |= IF8080_GDMA_G1_WP_MSK | IF8080_GDMA_G2_WP_MSK;
}

/******************* (C) COPYRIGHT 2019 Realtek Semiconductor Corporation *****END OF FILE****/

