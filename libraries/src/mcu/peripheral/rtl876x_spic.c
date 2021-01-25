/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_spi.c
* @brief    This file provides all the Spi firmware functions.
* @details
* @author   elliot chen
* @date     2015-05-06
* @version  v0.1
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtl876x_spic.h"
#include "rtl876x_rcc.h"

/**
  * @brief  Deinitializes the SPIx peripheral registers to their default reset values.
  * @param  SPIx: where x can be 0 or 1 to select the SPI peripheral.
  * @retval None
  */
void SPIC_DeInit(SPIC_TypeDef *SPICx)
{

}

/**
  * @brief  Initializes the SPIx peripheral according to the specified
  *   parameters in the SPIC_InitStruct.
  * @param  SPIx: where x can be 0 or 1 to select the SPI peripheral.
  * @param  SPIC_InitStruct: pointer to a SPIC_InitTypeDef structure that
  *   contains the configuration information for the specified SPI peripheral.
  * @retval None
  */
void SPIC_Init(SPIC_TypeDef *SPICx, SPIC_InitTypeDef *SPIC_InitStruct)
{

}

/**
  * @brief  Fills each SPIC_InitStruct member with its default value.
  * @param  SPIC_InitStruct : pointer to a SPIC_InitTypeDef structure which will be initialized.
  * @retval None
  */
void SPIC_StructInit(SPIC_InitTypeDef *SPIC_InitStruct)
{

}


/**
  * @brief  Transmits a number of bytes through the SPICx peripheral.
  * @param  SPICx: where x can be 0 or 2
  * @param  Data : bytes to be transmitted.
  * @retval None
  */
void SPIC_SendBuffer(SPIC_TypeDef *SPICx, uint8_t *pBuf, uint16_t len)
{
    SPICx->ser = BIT0;
    SPICx->ssienr = 0x00;//disable SPIC2

    SPICx->ctrlr0 &= ~(BIT_CMD_CH(3) | BIT_ADDR_CH(3) | BIT_DATA_CH(3));//SET CHANNEL NUM
    SPICx->ctrlr0 &= ~(BIT_TMOD(3)); //tx mode

    for (uint32_t i = 0; i < len; i++)
    {
        SPICx->dr[0].byte = pBuf[i];
    }
    SPICx->ssienr = 0x01;//enable SPIC2

    while (SPICx->sr & BIT0); // wait bus busy

    SPICx->ssienr = 0x00;//disable SPIC2
}

void SPIC_Calibration(SPIC_TypeDef *SPICx)
{

}

/**
  * @brief  Transmits a number of bytes through the SPICx peripheral.
  * @param  SPICx: where x can be 0 or 2
  * @param  Data : bytes to be transmitted.
  * @retval None
  */
void SPIC_ReceiveBuffer(SPIC_TypeDef *SPICx, uint8_t *pBuf, uint16_t len)
{

}




/******************* (C) COPYRIGHT 2015 Realtek Semiconductor Corporation *****END OF FILE****/

