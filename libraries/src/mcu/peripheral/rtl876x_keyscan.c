/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_keyscan.c
* @brief    This file provides all the KEYSCAN firmware functions.
* @details
* @author   tifnan_ge
* @date     2015-04-30
* @version  v0.1
*********************************************************************************************************
*/

#include "rtl876x.h"
#include "rtl876x_bitfields.h"
#include "rtl876x_rcc.h"
#include "rtl876x_keyscan.h"

/**
  * @brief Initializes the KeyScan peripheral according to the specified
  *   parameters in the KeyScan_InitStruct
  * @param  KeyScan: selected KeyScan peripheral.
  * @param  KeyScan_InitStruct: pointer to a KEYSCAN_InitTypeDef structure that
  *   contains the configuration information for the specified KeyScan peripheral
  * @retval None
  */
void KeyScan_Init(KEYSCAN_TypeDef *KeyScan, KEYSCAN_InitTypeDef *KeyScan_InitStruct)
{
    assert_param(IS_KeyScan_PERIPH(KeyScan));
    assert_param(IS_KEYSCAN_ROW_NUM(KeyScan_InitStruct->rowSize));
    assert_param(IS_KEYSCAN_COL_NUM(KeyScan_InitStruct->colSize));
    assert_param(IS_KEYSCAN_DEBOUNCE_EN(KeyScan_InitStruct->debounceEn));
    assert_param(IS_KEYSCAN_DETECT_MODE(KeyScan_InitStruct->detectMode));
    assert_param(IS_KEYSCAN_FIFO_OVR_CTRL(KeyScan_InitStruct->fifoOvrCtrl));
    assert_param(IS_KEYSCAN_MAX_SCAN_DATA(KeyScan_InitStruct->maxScanData));

    /* Set FSM to idle state */
    KeyScan->CR &= ~BIT31;

    /* Mask all keyscan interrupt */
    KeyScan->INTMASK |= 0x1f;

    /* clock divider config */
    KeyScan->CLKDIV &= ~((0x3FF << 8) | 0x1F);
    KeyScan->CLKDIV |= ((KeyScan_InitStruct->clockdiv << 8)\
                        | (KeyScan_InitStruct->delayclk));

    /* Config scan mode and detect mode*/
    KeyScan->CR = (KeyScan_InitStruct-> scanmode | KeyScan_InitStruct-> detectMode |
                   KeyScan_InitStruct->fifoOvrCtrl);
    /* fifo threshol setting */
    KeyScan->CR |= (KeyScan_InitStruct ->fifotriggerlevel << 5);
    /* key limit setting */
    KeyScan->CR |= (KeyScan_InitStruct ->keylimit << 23);

    /* time config */
    KeyScan->TIMERCR = ((KeyScan_InitStruct ->debounceEn)\
                        | (KeyScan_InitStruct->detecttimerEn)\
                        | (KeyScan_InitStruct->scantimerEn));
    /* time count config */
    KeyScan->TIMERCR |= ((KeyScan_InitStruct->debouncecnt << 18)\
                         | (KeyScan_InitStruct->scanInterval << 9)\
                         | (KeyScan_InitStruct->releasecnt));

    /* Set col map, config which col to work */
    KeyScan->COLCR = ((((1 << KeyScan_InitStruct->colSize) - 1) << 8) \
                      | (KeyScan_InitStruct->colSize - 1));

    /* Set col map, config which col to work */
    KeyScan->ROWCR = ((KeyScan_InitStruct->rowSize - 1) << 16\
                      | ((1 << KeyScan_InitStruct->rowSize) - 1));

    /* clear all interrupt status and status flag */
    KeyScan->INTCLR |= 0xff;
    /* Unmask all keyscan interrupt */
    KeyScan->INTMASK &= ~0x1f;

    return;
}

/**
  * @brief  Deinitializes the Keyscan peripheral registers to their default reset values(turn off keyscan clock).
  * @param  KeyScan: selected KeyScan peripheral.
  * @retval None
  */
void KeyScan_DeInit(KEYSCAN_TypeDef *KeyScan)
{
    /* Check the parameters */
    assert_param(IS_KeyScan_PERIPH(KeyScan));

    RCC_PeriphClockCmd(APBPeriph_KEYSCAN, APBPeriph_KEYSCAN_CLOCK, DISABLE);

    return;
}


/**
  * @brief  Fills each I2C_InitStruct member with its default value.
  * @param  KeyScan_InitStruct: pointer to an KEYSCAN_InitTypeDef structure which will be initialized.
  * @retval None
  */
void KeyScan_StructInit(KEYSCAN_InitTypeDef *KeyScan_InitStruct)
{
    KeyScan_InitStruct->colSize          = 2;
    KeyScan_InitStruct->rowSize          = 2;

    KeyScan_InitStruct->scanmode         = KeyScan_Auto_Scan_Mode;
    KeyScan_InitStruct->detectMode       = KeyScan_Detect_Mode_Level;
    KeyScan_InitStruct->clockdiv         = 0x1f8;
    KeyScan_InitStruct->delayclk         = 0x01;
    KeyScan_InitStruct->fifotriggerlevel = 1;
    KeyScan_InitStruct->fifoOvrCtrl      = KeyScan_FIFO_OVR_CTRL_DIS_LAST;

    KeyScan_InitStruct->debounceEn       = KeyScan_Debounce_Enable;
    KeyScan_InitStruct->scantimerEn      = KeyScan_ScanInterval_Enable;
    KeyScan_InitStruct->detecttimerEn    = KeyScan_Release_Detect_Enable;

    KeyScan_InitStruct->scanInterval     = 0x10;
    KeyScan_InitStruct->debouncecnt      = 0x10;
    KeyScan_InitStruct->releasecnt       = 0x1;

    KeyScan_InitStruct->keylimit         = 0x03;


    return;
}

/**
  * @brief  Enables or disables the specified KeyScan interrupts.
  * @param  KeyScan: selected KeyScan peripheral.
  * @param  KeyScan_IT: specifies the KeyScan interrupts sources to be enabled or disabled.
  *   This parameter can be any combination of the following values:
  *     @arg KEYSCAN_INT_TIMEOUT: KeyScan timeout interrupt mask
  *     @arg KEYSCAN_INT_OVER_THRESHOLD: Kescan FIFO data over threshold interrupt mask
  *     @arg KEYSCAN_INT_SCAN_END: KeyScan scan end interrupt mask
  * @param  NewState: new state of the specified KeyScan interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void KeyScan_INTConfig(KEYSCAN_TypeDef *KeyScan, uint32_t KeyScan_IT, FunctionalState newState)
{
    /* Check the parameters */
    assert_param(IS_KeyScan_PERIPH(KeyScan));
    assert_param(IS_FUNCTIONAL_STATE(newState));
    assert_param(IS_KEYSCAN_CONFIG_IT(KeyScan_IT));

    if (newState == ENABLE)
    {
        /* Enable the selected KeyScan interrupts */
        KeyScan->CR |= KeyScan_IT;
    }
    else
    {
        /* Disable the selected KeyScan interrupts */
        KeyScan->CR &= (uint32_t)~KeyScan_IT;
    }
}

/**
  * @brief  Enables or disables the specified KeyScan interrupts mask.
  * @param  KeyScan: selected KeyScan peripheral.
  * @param  NewState: new state of the specified KeyScan interrupts mask.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void KeyScan_INTMask(KEYSCAN_TypeDef *KeyScan, uint32_t KeyScan_IT, FunctionalState newState)
{
    /* Check the parameters */
    assert_param(IS_KeyScan_PERIPH(KeyScan));
    assert_param(IS_FUNCTIONAL_STATE(newState));

    if (newState == ENABLE)
    {
        /* mask KeyScan interrupts */
        KeyScan->INTMASK |= KeyScan_IT;
    }
    else
    {
        /* enable KeyScan interrupts */
        KeyScan->INTMASK &= (~KeyScan_IT);
    }
}

/**
  * @brief  Read data from keyscan FIFO.
  * @param  KeyScan: selected KeyScan peripheral.
  * @param[out]  outBuf: buffer to save data read from KeyScan FIFO.
  * @param  count: number of data to be read.
  * @retval None
  */
void KeyScan_Read(KEYSCAN_TypeDef *KeyScan, uint16_t *outBuf, uint16_t count)
{
    /* Check the parameters */
    assert_param(IS_KeyScan_PERIPH(KeyScan));

    uint16_t i = 0;

    for (i = 0; i < count; i++)
    {
        *outBuf++ = (uint16_t)KeyScan->FIFODATA;
    }

    return;
}

/**
  * @brief  Enables or disables the KeyScan peripheral.
  * @param  KeyScan: selected KeyScan peripheral.
  * @param  NewState: new state of the KeyScan peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void KeyScan_Cmd(KEYSCAN_TypeDef *KeyScan, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_KeyScan_PERIPH(KeyScan));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the selected KeyScan peripheral */
        /* In manual mode, bit22 must be write 1 to trigger scan,
        and will be clear to 0 automatically after scan finish */
        if ((KeyScan->CR & BIT30) == 0)
        {
            KeyScan->CR |= BIT22;
        }

        KeyScan->CR |= BIT31;
    }
    else
    {
        /* Disable the selected KeyScan peripheral */
        KeyScan->CR &= ~BIT31;
    }
}

/**
  * @brief  Set filter data.
  * @param  KeyScan: selected KeyScan peripheral.
  * @param  data: config the data to be filtered.
  *   This parameter should not be more than 9 bits
  * @retval none.
  */
void KeyScan_FilterDataConfig(KEYSCAN_TypeDef *KeyScan, uint16_t data, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_KeyScan_PERIPH(KeyScan));

    if (NewState == ENABLE)
    {
        KeyScan->CR &= ~BIT21;

        KeyScan->CR &= ~(0x1ff << 12);
        KeyScan->CR |= ((data & 0x1ff) << 12);

        KeyScan->CR |= BIT21;
    }
    else
    {
        KeyScan->CR &= ~BIT21;

        KeyScan->CR &= ~(0x1ff << 12);
        KeyScan->CR |= ((data & 0x1ff) << 12);
    }

    return;
}
