/**
*********************************************************************************************************
*               Copyright(c) 2019, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_adc.c
* @brief    This file provides all the ADC firmware functions.
* @details
* @author   yuan
* @date     2019-11-14
* @version  v1.0
*********************************************************************************************************
*/
#include "rtl876x_rcc.h"
#include "rtl876x_adc.h"

/**
  * @brief  Deinitializes the ADC peripheral registers to their default reset values(turn off ADC clock).
  *
  * @param  ADCx: selected ADC peripheral.
  * @retval None.
  */
void ADC_DeInit(ADC_TypeDef *ADCx)
{
    assert_param(IS_ADC_PERIPH(ADCx));

    RCC_PeriphClockCmd(APBPeriph_ADC, APBPeriph_ADC_CLOCK, DISABLE);

    return;
}

/**
  * @brief  Initializes the ADC peripheral according to the specified
  *         parameters in the ADC_InitStruct
  * @param  ADCx: selected ADC peripheral.
  * @param  ADC_InitStruct: pointer to a ADCInitTypeDef structure that
  *         contains the configuration information for the specified ADC peripheral
  * @retval None
  */
void ADC_Init(ADC_TypeDef *ADCx, ADC_InitTypeDef *ADC_InitStruct)
{
    assert_param(IS_ADC_PERIPH(ADCx));
    assert_param(IS_ADC_DATA_WRITE_TO_FIFO_CMD(ADC_InitStruct->ADC_DataWriteToFifo));
    assert_param(IS_ADC_FIFO_THRESHOLD(ADC_InitStruct->ADC_FifoThdLevel));
    assert_param(IS_ADC_WATER_LEVEL_CONFIG(ADC_InitStruct->ADC_WaterLevel));
    assert_param(IS_ADC_OVERWRITE_MODE(ADC_InitStruct->ADC_FifoOverWriteEn));
    assert_param(IS_ADC_LATCH_DATA_EDGE(ADC_InitStruct->ADC_DataLatchEdge));
    assert_param(IS_ADC_DATA_AVG_NUM(ADC_InitStruct->ADC_DataAvgSel));
    assert_param(IS_ADC_DATA_AVG_EN(ADC_InitStruct->ADC_DataAvgEn));
    assert_param(IS_ADC_POWER_ON_MODE(ADC_InitStruct->ADC_PowerOnMode));
    assert_param(IS_ADC_RG2X_0_DELAY_TIME(ADC_InitStruct->ADC_RG2X0Dly));
    assert_param(IS_ADC_RG0X_1_DELAY_TIME(ADC_InitStruct->ADC_RG0X1Dly));
    assert_param(IS_ADC_RG0X_0_DELAY_TIME(ADC_InitStruct->ADC_RG0X0Dly));
    assert_param(IS_ADC_DATA_MINUS_CMD(ADC_InitStruct->ADC_DataMinusEn));
    assert_param(IS_ADC_DATA_ALIGN(ADC_InitStruct->ADC_DataAlign));
    assert_param(IS_ADC_TIMER_TRIGGER_CMD(ADC_InitStruct->ADC_TimerTriggerEn));
    assert_param(IS_ADC_POWER_ALWAYS_ON(ADC_InitStruct->ADC_PowerAlwaysOnEn));
    assert_param(IS_ADC_CONVERT_TIME(ADC_InitStruct->ADC_ConvertTime));

    /* Added to stabilize the power supply! */
    uint8_t reg_value = 0;
    reg_value = btaon_fast_read_safe(0x110);
    btaon_fast_write(0x110, reg_value | 0x04);

    uint8_t index = 0;

    /*Disable all interrupt.*/
    ADCx->INTCR &= (~0x1f);

    /* Set power mode first */
    ADCx->PWRDLY = (((ADC_InitStruct->ADC_DataLatchDly & 0x7) << 6) |
                    ADC_InitStruct->ADC_PowerAlwaysOnEn |
                    ADC_InitStruct->ADC_PowerOnMode);
    if (ADC_InitStruct->ADC_PowerOnMode == ADC_POWER_ON_AUTO)
    {
        ADCx->PWRDLY |= (ADC_InitStruct->ADC_RG2X0Dly \
                         | ADC_InitStruct->ADC_RG0X1Dly \
                         | ADC_InitStruct->ADC_RG0X0Dly);
    }

    if (ADC_InitStruct->ADC_DataAvgEn == ADC_DATA_AVERAGE_ENABLE)
    {
        ADCx->PWRDLY |= (ADC_InitStruct->ADC_DataAvgEn | ADC_InitStruct->ADC_DataAvgSel);
        /* Disable schedule table */
        ADCx->SCHCR &= (~0xffff);
        /* Set schedule table */
        ADCx->SCHTAB0 = ADC_InitStruct->ADC_SchIndex[0];
        ADCx->SCHCR |= (uint16_t)0x0001;
    }
    else
    {
        /* Disable schedule table */
        ADCx->SCHCR &= (~0xffff);

        /* Set schedule table */
        for (index = 0; index < 8; index++)
        {
            *(__IO uint32_t *)((uint32_t *)(&ADCx->SCHTAB0) + index) = (ADC_InitStruct->ADC_SchIndex[index * 2]
                                                                        |
                                                                        (ADC_InitStruct->ADC_SchIndex[index * 2 + 1] << 16));
        }
        ADCx->SCHCR = ADC_InitStruct->ADC_Bitmap;
    }


    /* Set ADC mode */
    ADCx->CR = ((ADC_InitStruct->ADC_DataWriteToFifo)
                | ((ADC_InitStruct->ADC_FifoThdLevel & 0x3F) << 20)
                | ((ADC_InitStruct->ADC_WaterLevel & 0x3F) << 14)
                | (ADC_InitStruct->ADC_FifoOverWriteEn)
                | (ADC_InitStruct->ADC_DataLatchEdge));

    /* adc data and clock config */
    if ((ADC_InitStruct->ADC_SampleTime & 0x38FF) < 19)
    {
        /* adc sample period max 400kHz */
        ADC_InitStruct->ADC_SampleTime = 19;
    }

    ADCx->DATCLK = ((ADC_InitStruct->ADC_DataMinusEn)
                    | (ADC_InitStruct->ADC_DataAlign)
                    | (ADC_InitStruct->ADC_TimerTriggerEn)
                    | ((ADC_InitStruct->ADC_DataMinusOffset & 0xFFF) << 16)
                    | ((ADC_InitStruct->ADC_ConvertTime))
                    | ((ADC_InitStruct->ADC_SampleTime & 0x3800) << 2)
                    | (ADC_InitStruct->ADC_SampleTime & 0xFF));
    ADCx->ANACTL |= (0x03 << 10);

    /*clear adc fifo*/
    ADCx->CR |= BIT26;
    /*clear all interrupt*/
    ADCx->INTCR |= (0x1f << 8);

    return;
}

/**
  * @brief  Fills each ADC_InitStruct member with its default value.
  * @param  ADC_InitStruct: pointer to an ADC_InitTypeDef structure which will be initialized.
  * @retval None
  */
void ADC_StructInit(ADC_InitTypeDef *ADC_InitStruct)
{
    ADC_InitStruct->ADC_SampleTime      = 255;  /* (n + 1) cycle of 10MHz,n = 0~255 or n = 2048~14591 */
    ADC_InitStruct->ADC_ConvertTime     = ADC_CONVERT_TIME_500NS;

    ADC_InitStruct->ADC_DataWriteToFifo = ADC_DATA_WRITE_TO_FIFO_DISABLE;
    ADC_InitStruct->ADC_FifoThdLevel    = 0x0A;
    ADC_InitStruct->ADC_WaterLevel      = 0x1;
    ADC_InitStruct->ADC_FifoOverWriteEn = ADC_FIFO_OVER_WRITE_ENABLE;
    ADC_InitStruct->ADC_DataLatchEdge   = ADC_LATCH_DATA_Positive;

    ADC_InitStruct->ADC_SchIndex[0]     = 0;
    ADC_InitStruct->ADC_SchIndex[1]     = 0;
    ADC_InitStruct->ADC_SchIndex[2]     = 0;
    ADC_InitStruct->ADC_SchIndex[3]     = 0;
    ADC_InitStruct->ADC_SchIndex[4]     = 0;
    ADC_InitStruct->ADC_SchIndex[5]     = 0;
    ADC_InitStruct->ADC_SchIndex[6]     = 0;
    ADC_InitStruct->ADC_SchIndex[7]     = 0;
    ADC_InitStruct->ADC_SchIndex[8]     = 0;
    ADC_InitStruct->ADC_SchIndex[9]     = 0;
    ADC_InitStruct->ADC_SchIndex[10]    = 0;
    ADC_InitStruct->ADC_SchIndex[11]    = 0;
    ADC_InitStruct->ADC_SchIndex[12]    = 0;
    ADC_InitStruct->ADC_SchIndex[13]    = 0;
    ADC_InitStruct->ADC_SchIndex[14]    = 0;
    ADC_InitStruct->ADC_SchIndex[15]    = 0;
    ADC_InitStruct->ADC_Bitmap          = 0x0;

    ADC_InitStruct->ADC_TimerTriggerEn  = ADC_TIMER_TRIGGER_DISABLE;
    ADC_InitStruct->ADC_DataAlign       = ADC_DATA_ALIGN_LSB;
    ADC_InitStruct->ADC_DataMinusEn     = ADC_DATA_MINUS_DISABLE;
    ADC_InitStruct->ADC_DataMinusOffset = 0;

    ADC_InitStruct->ADC_DataAvgEn       = ADC_DATA_AVERAGE_DISABLE;
    ADC_InitStruct->ADC_DataAvgSel      = ADC_DATA_AVERAGE_OF_2;
    /* Reserved parameter, please do not change values*/
    ADC_InitStruct->ADC_PowerOnMode     = ADC_POWER_ON_AUTO;
    ADC_InitStruct->ADC_PowerAlwaysOnEn = ADC_POWER_ALWAYS_ON_DISABLE;
    ADC_InitStruct->ADC_DataLatchDly    = 0x1;
    ADC_InitStruct->ADC_RG2X0Dly        = ADC_RG2X_0_DELAY_10_US;
    ADC_InitStruct->ADC_RG0X1Dly        = ADC_RG0X_1_DELAY_20_US;
    ADC_InitStruct->ADC_RG0X0Dly        = ADC_RG0X_0_DELAY_30_US;

    return;
}

/**
  * @brief  Enables or disables the ADC peripheral.
  * @param  ADCx: selected ADC peripheral.
  * @param  adcMode: adc mode select.
        This parameter can be one of the following values:
  *     @arg ADC_ONE_SHOT_MODE: one shot mode.
  *     @arg ADC_CONTINUOUS_MODE: continuous mode.
  * @param  NewState: new state of the ADC peripheral.
  *     This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_Cmd(ADC_TypeDef *ADCx, uint8_t adcMode, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADC_PERIPH(ADCx));
    assert_param(IS_ADC_SAMPLE_MODE(adcMode));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState == ENABLE)
    {
        /* In case manual mode */
        if (ADCx->PWRDLY  & ADC_POWER_ON_MANUAL)
        {
            ADCx->PWRDLY  |= 0x3C00;
//            ADC_DelayUs(80);
            platform_delay_us(80);
            ADCx->PWRDLY  |= (BIT14 | BIT15);
//            ADC_DelayUs(320);
            platform_delay_us(320);
            ADCx->PWRDLY  |= (BIT16 | BIT17);
//            ADC_DelayUs(240);
            platform_delay_us(240);
            ADCx->PWRDLY  |= BIT18;
        }
        /* Reset ADC mode first */
        ADCx->CR &= ~0x03;
        /* Enable ADC */
        ADCx->CR |= adcMode;
    }
    else
    {
        ADCx->CR &= ~0x03;
    }

    return;
}

/**
  * @brief  Enables or disables the specified ADC interrupts.
  * @param  ADCx: selected ADC peripheral.
  * @param  ADC_INT: specifies the ADC interrupts sources to be enabled or disabled.
  *         This parameter can be any combination of the following values:
  *         @arg ADC_INT_FIFO_RD_REQ :FIFO read request
  *         @arg ADC_INT_FIFO_RD_ERR :FIFO read error
  *         @arg ADC_INT_FIFO_THD :ADC FIFO size > thd
  *         @arg ADC_INT_FIFO_FULL :ADC FIFO overflow
  *         @arg ADC_INT_ONE_SHOT_DONE :ADC one shot mode done
  * @param  NewState: new state of the specified ADC interrupts.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_INTConfig(ADC_TypeDef *ADCx, uint32_t ADC_INT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADC_PERIPH(ADCx));
    assert_param(IS_ADC_INT(ADC_INT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the selected ADC interrupts */
        ADCx->INTCR |= ADC_INT;
    }
    else
    {
        /* Disable the selected ADC interrupts */
        ADCx->INTCR &= (uint32_t)~ADC_INT;
    }
}

/**
  * @brief  Read ADC data according to specific channel.
  * @param  ADCx: selected ADC peripheral.
  * @param  index: can be 0 to 15
  * @retval The 12-bit converted ADC data.
  */
uint16_t ADC_ReadRawData(ADC_TypeDef *ADCx, uint8_t index)
{
    /* Check the parameters */
    assert_param(IS_ADC_PERIPH(ADCx));
    assert_param(index < 16);

    if (index & BIT(0))
    {
        return ((*(uint32_t *)((uint32_t *)(&ADCx->SCHD0) + (index >> 1))) >> 16);
    }
    else
    {
        return (*(uint32_t *)((uint32_t *)(&ADCx->SCHD0) + (index >> 1)));
    }
}

/**
  * @brief  Read ADC average data from ADC schedule table0.
  * @param  ADCx: selected ADC peripheral.
  * @param[out]  OutBuf: buffer to save data read from ADC FIFO.
  * @retval The 12-bit converted ADC data.
  */
uint16_t ADC_ReadAvgRawData(ADC_TypeDef *ADCx)
{
    /* Check the parameters */
    assert_param(IS_ADC_PERIPH(ADCx));

    uint16_t data = 0;
    data = (uint16_t)ADCx->SCHD0;

    return data;
}

/**
  * @brief  Get one data from ADC FIFO.
  * @param  ADCx: selected ADC peripheral.
  * @retval adc FIFO data.
  */
uint16_t ADC_ReadFIFO(ADC_TypeDef *ADCx)
{
    /* Check the parameters */
    assert_param(IS_ADC_PERIPH(ADCx));

    return (uint16_t)((ADCx->FIFO) & 0xFFF);
}

/**
  * @brief  Read data from ADC FIFO.
  * @param  ADCx: selected ADC peripheral.
  * @param[out]  outBuf: buffer to save data read from ADC FIFO.
  * @param  num: number of data to be read.
  * @retval None
  */
void ADC_ReadFIFOData(ADC_TypeDef *ADCx, uint16_t *outBuf, uint16_t num)
{
    /* Check the parameters */
    assert_param(IS_ADC_PERIPH(ADCx));

    while (num--)
    {
        *outBuf++ = (uint16_t)ADCx->FIFO;
    }

    return;
}

/**
  * @brief  Get ADC fifo data number.
  * @param  ADCx: selected ADC peripheral.
  * @retval current data number in adc fifo.
  */
uint8_t ADC_GetFIFODataLen(ADC_TypeDef *ADCx)
{
    /* Check the parameters */
    assert_param(IS_ADC_PERIPH(ADCx));

    return ((uint8_t)(((ADCx->SCHCR) >> 16) & 0x3F));
}

/**
  * @brief  Config ADC schedule table.
  * @param  ADCx: selected ADC peripheral.
  * @param  adcMode: ADC mode.
  *     This parameter can be one of the following values:
  *     @arg EXT_SINGLE_ENDED(index)
  *     @arg EXT_DIFFERENTIAL(index)
  *     @arg INTERNAL_VBAT_MODE
  * @param  Index: Schedule table index.
  * @return none.
  */
void ADC_SchIndexConfig(ADC_TypeDef *ADCx, uint8_t adcMode, uint16_t index)
{
    /* Check the parameters */
    assert_param(IS_ADC_PERIPH(ADCx));
    assert_param(IS_ADC_SCHEDULE_INDEX_CONFIG(adcMode));

    if (index & BIT0)
    {
        *(uint32_t *)((uint32_t *)(&ADCx->SCHTAB0) + (index >> 1)) |= (adcMode << 16);
    }
    else
    {
        *(uint32_t *)((uint32_t *)(&ADCx->SCHTAB0) + (index >> 1)) |= adcMode;
    }

    return;
}

/**
  * @brief  Same as function ADC_SchIndexConfig,this function is version bee2.
  * @param  ADCx: selected ADC peripheral.
  * @param  adcMode: ADC mode.
  *     This parameter can be one of the following values:
  *     @arg EXT_SINGLE_ENDED(index)
  *     @arg EXT_DIFFERENTIAL(index)
  *     @arg INTERNAL_VBAT_MODE
  * @param  Index: Schedule table index.
  * @return none.
  */
void ADC_SchTableConfig(ADC_TypeDef *ADCx, uint16_t Index, uint8_t adcMode)
{
    /* Check the parameters */
    assert_param(IS_ADC_PERIPH(ADCx));
    assert_param(IS_ADC_SCHEDULE_INDEX_CONFIG(adcMode));

    if (Index & BIT0)
    {
        *(uint32_t *)((uint32_t *)(&ADCx->SCHTAB0) + (Index >> 1)) |= (adcMode << 16);
    }
    else
    {
        *(uint32_t *)((uint32_t *)(&ADCx->SCHTAB0) + (Index >> 1)) |= adcMode;
    }

    return;
}

/**
  * @brief  Set adc schedule table.
  * @param  ADCx: selected ADC peripheral.
  * @param  channelMap: ADC channel map.
  * @param  NewState: new state of the ADC peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @return none.
  */
void ADC_BitMapConfig(ADC_TypeDef *ADCx, uint16_t bitMap, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADC_PERIPH(ADCx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState == ENABLE)
    {
        ADCx->SCHCR |= bitMap;
    }
    else
    {
        ADCx->SCHCR &= (~bitMap);
    }

    return;
}

/**
  * @brief  Power on ADC manually.
  * @param  ADCx: selected ADC peripheral.
  * @param  NewState: new state of the ADC power on.
  *     This parameter can be: ENABLE or DISABLE.
  *     @note If enable, ADC power will always on.
  * @retval None
  */
void ADC_ManualPowerOnCmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADC_PERIPH(ADCx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        ADCx->PWRDLY |= BIT19;
    }
    else
    {
        ADCx->PWRDLY &= ~(BIT19);
    }
}

/**
  * @brief  Enbale or disable stop fifo from writing data.
  * @param  ADCx: selected ADC peripheral.
  * @param  NewState: new state of the ADC fifo write.
  *     This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_WriteFIFOCmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADC_PERIPH(ADCx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState == DISABLE)
    {
        ADCx->PWRDLY |= BIT21;
    }
    else
    {
        ADCx->PWRDLY &= ~(BIT21);
    }
}

/**
  * @brief  Config ADC bypass resistor.Attention!!!Channels using bypass mode cannot over 0.9V!!!!
  * @param  channelNum: external channel number, can be 0~7.
  * @param  NewState: ENABLE or DISABLE.
  * @retval None
  */
void ADC_BypassCmd(uint8_t ChannelNum, FunctionalState NewState)
{
    assert_param(ChannelNum <= 7);
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        ADC->ANACTL |= BIT(ChannelNum + 16);
    }
    else
    {
        ADC->ANACTL &= ~BIT(ChannelNum + 16);
    }
}

/**
  * @brief  Config ADC power supply.
  * @param  NewState: new state of the ADC power supply.
  *     This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_PowerSupplyConfig(FunctionalState NewState)
{
    uint8_t reg_value = 0;

    /* Enable ADC 1V8 LDO */
    if (NewState != DISABLE)
    {
        reg_value = btaon_fast_read_safe(0x110);
        btaon_fast_write(0x110, reg_value | 0x04);
    }
    else
    {
        reg_value = btaon_fast_read_safe(0x110);
        btaon_fast_write(0x110, reg_value & (~0x04));
    }
}

/**
  * @brief  Config ADC fast power supply.
  * @param  NewState: new state of the ADC fast power supply.
  *     This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_FastPowerSupplyConfig(FunctionalState NewState)
{
    uint8_t reg_value = 0;

    if (NewState != DISABLE)
    {
        reg_value = btaon_fast_read_safe(0x113);
        btaon_fast_write(0x113, reg_value | 0x04);
    }
    else
    {
        reg_value = btaon_fast_read_safe(0x113);
        btaon_fast_write(0x113, reg_value & (~0x04));
    }
}

/**
  * @brief  Checks whether the specified ADC interrupt status flag is set or not.
  * @param  ADCx: selected ADC peripheral.
  * @param  ADC_INT_FLAG: specifies the interrupt status flag to check.
  *     This parameter can be one of the following values:
  *     @arg ADC_INT_ONE_SHOT_DONE: ADC once convert end interrupt
  *     @arg ADC_INT_FIFO_OVERFLOW: ADC FIFO overflow interrupt
  *     @arg ADC_INT_FIFO_THD: fifo larger than threshold
  *     @arg ADC_INT_FIFO_RD_ERR: ADC read FIFO error interrupt
  *     @arg ADC_INT_FIFO_RD_REQ: ADC read FIFO request interrupt
  * @retval The new state of ADC_INT (SET or RESET).
  */
ITStatus ADC_GetINTStatus(ADC_TypeDef *ADCx, uint32_t ADC_INT)
{
    /* Check the parameters */
    assert_param(IS_ADC_PERIPH(ADCx));
    assert_param(IS_ADC_INT(ADC_INT));

    FlagStatus bitstatus = RESET;

    if ((ADCx->INTCR & (ADC_INT << 16)) != 0)
    {
        bitstatus = SET;
    }

    return bitstatus;
}

/**
  * @brief  Clear the ADC interrupt pending bit.
  * @param  ADCx: selected ADC peripheral.
  * @param  ADC_INT: specifies the interrupt pending bit to clear.
  *     This parameter can be any combination of the following values:
  *     @arg ADC_INT_ONE_SHOT_DONE: ADC once convert end interrupt
  *     @arg ADC_INT_FIFO_OVERFLOW: ADC FIFO overflow interrupt
  *     @arg ADC_INT_FIFO_THD: fifo larger than threshold
  *     @arg ADC_INT_FIFO_RD_ERR: ADC read FIFO error interrupt
  *     @arg ADC_INT_FIFO_RD_REQ: ADC read FIFO request interrupt
  * @retval None
  */
void ADC_ClearINTPendingBit(ADC_TypeDef *ADCx, uint32_t ADC_INT)
{
    /* Check the parameters */
    assert_param(IS_ADC_PERIPH(ADCx));
    assert_param(IS_ADC_INT(ADC_INT));

    ADCx->INTCR |= (ADC_INT << 8);

    return;
}

/******************* (C) COPYRIGHT 2020 Realtek Semiconductor Corporation *****END OF FILE****/
