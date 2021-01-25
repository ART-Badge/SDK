/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_qdec.c
* @brief    This file provides all the QDEC firmware functions.
* @details
* @author   howie_wang
* @date     2016-05-10
* @version  v0.1
*********************************************************************************************************
*/
#include "rtl876x_rcc.h"
#include "rtl876x_qdec.h"

/**
  * @brief Initializes the Qdecoder peripheral according to the specified
  *   parameters in the QDEC_InitStruct
  * @param  QDECx: selected Qdecoder peripheral.
  * @param  QDEC_InitStruct: pointer to a QDEC_InitStruct structure that
  *     contains the configuration information for the specified Qdecoder peripheral
  * @retval None
  */
void QDEC_Init(QDEC_TypeDef *QDECx, QDEC_InitTypeDef *QDEC_InitStruct)
{
    assert_param(IS_QDEC_PERIPH(QDECx));

    QDECx->REG_DIV = (QDEC_InitStruct->scanClockDiv << 4) | (QDEC_InitStruct->debounceClockDiv);
    if (QDEC_InitStruct->axisConfigX == ENABLE)
    {
        QDECx->REG_CR_X = ((QDEC_InitStruct->debounceTimeX << 4) | (QDEC_InitStruct->counterScaleX << 13) |
                           \
                           (QDEC_InitStruct->debounceEnableX << 12) | (QDEC_InitStruct->initPhaseX));
    }
    if (QDEC_InitStruct->axisConfigY == ENABLE)
    {
        QDECx->REG_CR_Y = ((QDEC_InitStruct->debounceTimeY << 4) | (QDEC_InitStruct->counterScaleY << 13) |
                           \
                           (QDEC_InitStruct->debounceEnableY << 12) | (QDEC_InitStruct->initPhaseY));
    }
    if (QDEC_InitStruct->axisConfigZ == ENABLE)
    {
        QDECx->REG_CR_Z = ((QDEC_InitStruct->debounceTimeZ << 4) | (QDEC_InitStruct->counterScaleZ << 13) |
                           \
                           (QDEC_InitStruct->debounceEnableZ << 12) | (QDEC_InitStruct->initPhaseZ));
    }
    return;
}

/**
  * @brief  Deinitializes the Qdecoder peripheral registers to their default reset values(turn off Qdecoder clock).
  * @param  QDECx: selected Qdecoder peripheral.
  * @retval None
  */
void QDEC_DeInit(QDEC_TypeDef *QDECx)
{
    RCC_PeriphClockCmd(APBPeriph_QDEC, APBPeriph_QDEC_CLOCK, DISABLE);
}


/**
  * @brief  Fills each QDEC_InitStruct member with its default value.
  * @param  QDEC_InitStruct: pointer to an QDEC_InitStruct structure which will be initialized.
  * @retval None
  */
void QDEC_StructInit(QDEC_InitTypeDef *QDEC_InitStruct)
{
    QDEC_InitStruct->scanClockDiv      = 0x27;/*!< 5M/(scanClockDiv+1) = 125KHz */
    QDEC_InitStruct->debounceClockDiv  = 0x0; /*!< 125KHz/(debounceClockDiv +1) = 125K */

    QDEC_InitStruct->axisConfigX       = DISABLE;
    QDEC_InitStruct->debounceTimeX     = 125 * 5; /*!< 5ms */
    QDEC_InitStruct->counterScaleX     = CounterScale_1_Phase;
    QDEC_InitStruct->debounceEnableX   = Debounce_Disable;
    QDEC_InitStruct->initPhaseX        = phaseMode0;

    QDEC_InitStruct->axisConfigY       = DISABLE;
    QDEC_InitStruct->debounceTimeY     = 125 * 5; /*!< 5ms */
    QDEC_InitStruct->counterScaleY     = CounterScale_1_Phase;
    QDEC_InitStruct->debounceEnableY   = Debounce_Disable;
    QDEC_InitStruct->initPhaseY        = phaseMode0;

    QDEC_InitStruct->axisConfigZ       = DISABLE;
    QDEC_InitStruct->debounceTimeZ     = 125 * 5; /*!< 5ms */
    QDEC_InitStruct->counterScaleZ     = CounterScale_1_Phase;
    QDEC_InitStruct->debounceEnableZ   = Debounce_Disable;
    QDEC_InitStruct->initPhaseZ        = phaseMode0;

    return;
}


/**
  * @brief  Enables or disables the specified Qdecoder interrupts.
  * @param  QDECx: selected Qdecoder peripheral.
  * @param  QDEC_IT: specifies the QDECODER interrupts sources to be enabled or disabled.
  *   This parameter can be one of the following values:
  *     @arg QDEC_X_INT_NEW_DATA:
  *     @arg QDEC_X_INT_ILLEAGE:
  *     @arg QDEC_Y_INT_NEW_DATA:
  *     @arg QDEC_Y_INT_ILLEAGE:
  *     @arg QDEC_Z_INT_NEW_DATA:
  *     @arg QDEC_Z_INT_ILLEAGE:
  * @param  NewState: new state of the specified QDECODER interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void QDEC_INTConfig(QDEC_TypeDef *QDECx, uint32_t QDEC_IT, FunctionalState newState)
{
    /* Check the parameters */
    assert_param(IS_QDEC_PERIPH(QDECx));
    assert_param(IS_FUNCTIONAL_STATE(newState));
    assert_param(IS_QDEC_INT_CONFIG(QDEC_IT));

    if (newState == ENABLE)
    {
        /* Enable the selected QDECODER interrupts */
        if (QDEC_IT & QDEC_X_INT_NEW_DATA)
        {
            QDECx->REG_CR_X |= BIT15;
        }
        if (QDEC_IT & QDEC_Y_INT_NEW_DATA)
        {
            QDECx->REG_CR_Y |= BIT15;
        }
        if (QDEC_IT & QDEC_Z_INT_NEW_DATA)
        {
            QDECx->REG_CR_Z |= BIT15;
        }
        if (QDEC_IT & QDEC_X_INT_ILLEAGE)
        {
            QDECx->REG_CR_X |= BIT14;
        }
        if (QDEC_IT & QDEC_Y_INT_ILLEAGE)
        {
            QDECx->REG_CR_Y |= BIT14;
        }
        if (QDEC_IT & QDEC_Z_INT_ILLEAGE)
        {
            QDECx->REG_CR_Z |= BIT14;
        }
    }
    else
    {
        /* Disable the selected QDECODER interrupts */
        if (QDEC_IT & QDEC_X_INT_NEW_DATA)
        {
            QDECx->REG_CR_X &= ~BIT15;
        }
        if (QDEC_IT & QDEC_Y_INT_NEW_DATA)
        {
            QDECx->REG_CR_Y &= ~BIT15;
        }
        if (QDEC_IT & QDEC_Z_INT_NEW_DATA)
        {
            QDECx->REG_CR_Z &= ~BIT15;
        }
        if (QDEC_IT & QDEC_X_INT_ILLEAGE)
        {
            QDECx->REG_CR_X &= ~BIT14;
        }
        if (QDEC_IT & QDEC_Y_INT_ILLEAGE)
        {
            QDECx->REG_CR_X &= ~BIT14;
        }
        if (QDEC_IT & QDEC_Z_INT_ILLEAGE)
        {
            QDECx->REG_CR_X &= ~BIT14;
        }
    }

    return;
}

/**
  * @brief  Checks whether the specified Qdecoder flag is set or not.
  * @param  QDECx: selected Qdecoder peripheral.
  * @param  QDEC_FLAG: specifies the flag to check.
  *   This parameter can be one of the following values:
  *     @arg QDEC_FLAG_ILLEGAL_STATUS_X:
  *     @arg QDEC_FLAG_ILLEGAL_STATUS_Y:
  *     @arg QDEC_FLAG_ILLEGAL_STATUS_Z:
  *     @arg QDEC_FLAG_NEW_STATUS_X:
  *     @arg QDEC_FLAG_NEW_STATUS_Y:
  *     @arg QDEC_FLAG_NEW_STATUS_Z:
  * @retval The new state of QDEC_FLAG (SET or RESET).
  */
FlagStatus QDEC_GetFlagState(QDEC_TypeDef *QDECx, uint32_t QDEC_FLAG)
{
    FlagStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_QDEC_PERIPH(QDECx));
    assert_param(IS_QDEC_CLR_INT_STATUS(QDEC_FLAG));

    if (((QDEC_FLAG & 0x7) | (QDEC_FLAG & (0x7 << 12))))
    {
        if (QDECx->INT_SR & QDEC_FLAG)
        {
            bitstatus = SET;
        }
    }
    else if (QDEC_FLAG == QDEC_FLAG_OVERFLOW_X)
    {
        if (QDECx->REG_SR_X & (BIT18))
        {
            bitstatus = SET;
        }
    }
    else if (QDEC_FLAG == QDEC_FLAG_OVERFLOW_Y)
    {
        if (QDECx->REG_SR_Y & (BIT18))
        {
            bitstatus = SET;
        }
    }
    else if (QDEC_FLAG == QDEC_FLAG_OVERFLOW_Z)
    {
        if (QDECx->REG_SR_Z & (BIT18))
        {
            bitstatus = SET;
        }
    }
    else if (QDEC_FLAG == QDEC_FLAG_UNDERFLOW_X)
    {
        if (QDECx->REG_SR_X & (BIT19))
        {
            bitstatus = SET;
        }
    }
    else if (QDEC_FLAG == QDEC_FLAG_UNDERFLOW_Y)
    {
        if (QDECx->REG_SR_Y & (BIT19))
        {
            bitstatus = SET;
        }
    }
    else if (QDEC_FLAG == QDEC_FLAG_UNDERFLOW_Z)
    {
        if (QDECx->REG_SR_Z & (BIT19))
        {
            bitstatus = SET;
        }
    }

    return bitstatus;
}

/**
  * @brief  Enables or disables mask the specified Qdecoder axis interrupts.
  * @param  QDECx: selected Qdecoder peripheral.
  * @param  QDEC_AXIS: specifies the Qdecoder axis.
  *   This parameter can be one or logical OR of the following values:
  *     @arg QDEC_X_CT_INT_MASK: The qdecoder X axis.
  *     @arg QDEC_X_ILLEAGE_INT_MASK: The qdecoder Y axis.
  *     @arg QDEC_Y_CT_INT_MASK: The qdecoder Z axis.
  *     @arg QDEC_Y_ILLEAGE_INT_MASK: The qdecoder X axis.
  *     @arg QDEC_Z_CNT_INT_MASK: The qdecoder Y axis.
  *     @arg QDEC_Z_ILLEAGE_INT_MASK: The qdecoder Z axis.
  * @param  NewState: new state of the specified Qdecoder interrupts mask.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void QDEC_INTMask(QDEC_TypeDef *QDECx, uint32_t QDEC_AXIS, FunctionalState newState)
{
    /* Check the parameters */
    assert_param(IS_QDEC_PERIPH(QDECx));
    assert_param(IS_QDEC_INT_MASK_CONFIG(QDEC_AXIS));
    assert_param(IS_FUNCTIONAL_STATE(newState));

    if (newState == ENABLE)
    {
        /* mask the selected QDEC interrupts */
        QDECx->INT_MASK |= (QDEC_AXIS);
    }
    else
    {
        /* unmask the selected QDEC interrupts */
        QDEC->INT_MASK &= (~(QDEC_AXIS));
    }

    return;
}

/**
  * @brief  Get Qdecoder Axis(x/y/z) direction.
  * @param  QDECx: selected Qdecoder peripheral.
  * @param  QDEC_AXIS: specifies the Qdecoder axis.
  *   This parameter can be one of the following values:
  *     @arg QDEC_AXIS_X: The qdecoder X axis.
  *     @arg QDEC_AXIS_Y: The qdecoder Y axis.
  *     @arg QDEC_AXIS_Z: The qdecoder Z axis.
  * @param  newState
  *   This parameter can be one of the following values:
  *     @arg ENABLE: Pause.
  *     @arg DISABLE: Resume.
  * @retval The count of the axis.
  */
void QDEC_Cmd(QDEC_TypeDef *QDECx, uint32_t QDEC_AXIS,
              FunctionalState newState)
{
    /* Check the parameters */
    assert_param(IS_QDEC_PERIPH(QDECx));
    assert_param(IS_QDEC_AXIS_DIR(QDEC_AXIS));

    if (newState == ENABLE)
    {
        *((volatile uint32_t *)(&QDECx->REG_CR_X) + QDEC_AXIS / 2) |= BIT30 | BIT31;
    }
    else
    {
        *((volatile uint32_t *)(&QDECx->REG_CR_X) + QDEC_AXIS / 2) &= ~(BIT30 | BIT31);
    }
}

