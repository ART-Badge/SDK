/**
**********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_nvic.c
* @brief    This file provides all the NVIC firmware functions.
* @details
* @author   elliot chen
* @date     2015-05-19
* @version  v0.1
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtl876x_nvic.h"

/**
  * @brief  Initializes the NVIC peripheral according to the specified
  *         parameters in the NVIC_InitStruct.
  * @param  NVIC_InitStruct: pointer to a NVIC_InitTypeDef structure that contains
  *         the configuration information for the specified NVIC peripheral.
  * @retval None
  */
void NVIC_Init(NVIC_InitTypeDef *NVIC_InitStruct)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NVIC_InitStruct->NVIC_IRQChannelCmd));

    if (NVIC_InitStruct->NVIC_IRQChannelCmd != DISABLE)
    {
        if (NVIC_InitStruct->NVIC_IRQChannel >= Peripheral_First_IRQn)
        {
            PERIPHINT->EN |= BIT(NVIC_InitStruct->NVIC_IRQChannel - Peripheral_First_IRQn);
            NVIC_InitStruct->NVIC_IRQChannel = Peripheral_IRQn;
        }

        NVIC_ClearPendingIRQ(NVIC_InitStruct->NVIC_IRQChannel);
        NVIC_SetPriority(NVIC_InitStruct->NVIC_IRQChannel,
                         NVIC_InitStruct->NVIC_IRQChannelPriority);
        NVIC_EnableIRQ(NVIC_InitStruct->NVIC_IRQChannel);
    }
    else
    {
        if (NVIC_InitStruct->NVIC_IRQChannel >= Peripheral_First_IRQn)
        {
            PERIPHINT->EN &= ~BIT(NVIC_InitStruct->NVIC_IRQChannel - Peripheral_First_IRQn);

            if (PERIPHINT->EN == 0)
            {
                /* Disable Peripheral IRQ Channel */
                NVIC_DisableIRQ(Peripheral_IRQn);
            }
        }
        else
        {
            /* Disable the Selected IRQ Channels */
            NVIC_DisableIRQ(NVIC_InitStruct->NVIC_IRQChannel);
        }
    }
}


/******************* (C) COPYRIGHT 2015 Realtek Semiconductor Corporation *****END OF FILE****/

