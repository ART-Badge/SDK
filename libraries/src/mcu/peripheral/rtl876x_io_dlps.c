/**
*********************************************************************************************************
*               Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_io_dlps.c
* @brief    This file provides all the IO DLPS control firmware functions.
* @details
* @author
* @date     2020-06-02
* @version  v1.0
*********************************************************************************************************
*/

#include <stddef.h>
#include "rtl876x_io_dlps.h"
#include "rtl876x_nvic.h"
#include "rtl876x_pinmux.h"
#include "rtl876x_rcc.h"
#include "app_section.h"
#include "board.h"
#include "dlps.h"
#include "otp.h"
#include "platform_autoconf.h"
#include "trace.h"

/*============================================================================*
 *                         IO DLPS
 *============================================================================*/

/** @defgroup IO_DLPS_Functions IO DLPS Functions
  * @{
  */

/********************************************** ********************************************************/
/**************************************** [PINMUX DLPS] **************************************************/
/*******************************************************************************************************/
/*******************************************************************************************************/

__STATIC_INLINE void Pinmux_DLPS_Enter(void);
__STATIC_INLINE void Pinmux_DLPS_Exit(void);

uint32_t Pinmux_StoreReg[10];      /*  This array should be placed in RAM ON/Buffer ON.    */

/**
  * @brief  PINMUX enter dlps callback function(Save PINMUX register values when system enter DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE  void Pinmux_DLPS_Enter(void) DATA_RAM_FUNCTION;
void Pinmux_DLPS_Enter(void)
{
    uint8_t i = 0;

    for (i = 0; i < 10; i++)
    {
        Pinmux_StoreReg[i] = PINMUX->CFG[i];
    }

    return;
}

/**
  * @brief  PINMUX exit dlps callback function(Resume PINMUX register values when system exit DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE  void Pinmux_DLPS_Exit(void) DATA_RAM_FUNCTION;
void Pinmux_DLPS_Exit(void)
{
    uint8_t i;

    for (i = 0; i < 10; i++)
    {
        PINMUX->CFG[i] = Pinmux_StoreReg[i];
    }

    return;
}

/********************************************** ********************************************************/
/**************************************** [ADC DLPS] **************************************************/
/*******************************************************************************************************/
/*******************************************************************************************************/

#if USE_ADC_DLPS
__STATIC_INLINE void ADC_DLPS_Enter(void);
__STATIC_INLINE void ADC_DLPS_Exit(void);

uint32_t ADC_StoreReg[14];        /*  This array should be placed in RAM ON/Buffer ON.    */

/**
  * @brief  ADC enter dlps callback function(Save ADC register values when system enter DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void ADC_DLPS_Enter(void) DATA_RAM_FUNCTION;
void ADC_DLPS_Enter(void)
{
    /*Open 10M clock source*/
    SYSBLKCTRL->u_20C.RSVD_20C |= BIT26;
    SYSBLKCTRL->u_20C.RSVD_20C |= BIT28;

    PERIPH->PERI_FUNC1_EN |= (1 << 0);
    PERIPH->PERI_CLK_CTRL1 |= (SYSBLK_ACTCK_ADC_EN_Msk | SYSBLK_SLPCK_ADC_EN_Msk);

    ADC_StoreReg[0] =  ADC->CR;          //0x04
    ADC_StoreReg[1] =  ADC->SCHCR;       //0x08
    ADC_StoreReg[2] =  ADC->INTCR;       //0x0C
    ADC_StoreReg[3] =  ADC->SCHTAB0;     //0x10
    ADC_StoreReg[4] =  ADC->SCHTAB1;     //0x14
    ADC_StoreReg[5] =  ADC->SCHTAB2;     //0x18
    ADC_StoreReg[6] =  ADC->SCHTAB3;     //0x1C
    ADC_StoreReg[7] =  ADC->SCHTAB4;     //0x20
    ADC_StoreReg[8] =  ADC->SCHTAB5;     //0x24
    ADC_StoreReg[9] =  ADC->SCHTAB6;     //0x28
    ADC_StoreReg[10] =  ADC->SCHTAB7;    //0x2C
    ADC_StoreReg[11] =  ADC->PWRDLY;
    ADC_StoreReg[12] =  ADC->DATCLK;
    ADC_StoreReg[13] =  ADC->ANACTL;

    uint8_t reg_value = 0;
    reg_value = btaon_fast_read_safe(0x110);
    btaon_fast_write(0x110, reg_value & (~0x04));

    return;
}

/**
  * @brief  ADC exit dlps callback function(Resume ADC register values when system exit DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void ADC_DLPS_Exit(void) DATA_RAM_FUNCTION;
void ADC_DLPS_Exit(void)
{
    /*Open 10M clock source*/
    SYSBLKCTRL->u_20C.RSVD_20C |= BIT26;
    SYSBLKCTRL->u_20C.RSVD_20C |= BIT28;

    PERIPH->PERI_FUNC1_EN |= (1 << 0);
    PERIPH->PERI_CLK_CTRL1 |= (SYSBLK_ACTCK_ADC_EN_Msk | SYSBLK_SLPCK_ADC_EN_Msk);

#if 0
    //Todo
    ADC->PWRDLY = ADC_StoreReg[10];
    ADC->CR = (ADC_StoreReg[0] & (~((uint32_t)0x03)));
    ADC->SCHTAB0 = ADC_StoreReg[3];
    ADC->SCHTAB1 = ADC_StoreReg[4];
    ADC->SCHTAB2 = ADC_StoreReg[5];
    ADC->SCHTAB3 = ADC_StoreReg[6];
    ADC->SCHTAB4 = ADC_StoreReg[7];
    ADC->SCHTAB5 = ADC_StoreReg[8];
    ADC->SCHTAB6 = ADC_StoreReg[9];
    ADC->SCHCR = ADC_StoreReg[1];
    ADC->INTCR = (ADC_StoreReg[2] & 0x1F);
#else
    /*Disable all interrupt.*/
    ADC->INTCR &= (~0x1f);

    /* Set power mode first */
    ADC->PWRDLY = ADC_StoreReg[11];

    /* Disable schedule table */
    ADC->SCHCR &= (~0xffff);

    ADC->SCHTAB0 = ADC_StoreReg[3];
    ADC->SCHTAB1 = ADC_StoreReg[4];
    ADC->SCHTAB2 = ADC_StoreReg[5];
    ADC->SCHTAB3 = ADC_StoreReg[6];
    ADC->SCHTAB4 = ADC_StoreReg[7];
    ADC->SCHTAB5 = ADC_StoreReg[8];
    ADC->SCHTAB6 = ADC_StoreReg[9];
    ADC->SCHTAB7 = ADC_StoreReg[10];
    ADC->SCHCR   = ADC_StoreReg[1];
    ADC->CR = (ADC_StoreReg[0] & (~((uint32_t)0x03)));
    ADC->DATCLK = ADC_StoreReg[12];
    ADC->ANACTL = ADC_StoreReg[13];

    /*Clear ADC FIFO */
    ADC->CR |= BIT26;
    /* Clear all interrupt */
    ADC->INTCR |= (0x1f << 8);

    /* Restore specify interrupt */
    ADC->INTCR = ADC_StoreReg[2];

#endif

    uint8_t reg_value = 0;
    reg_value = btaon_fast_read_safe(0x110);
    btaon_fast_write(0x110, reg_value | 0x04);

    return;
}

#endif

/********************************************** ********************************************************/
/**************************************** [CODEC DLPS] ************************************************/
/*******************************************************************************************************/
/*******************************************************************************************************/
#if USE_CODEC_DLPS

uint32_t CODEC_StoreReg[14];

/**
  * @brief  CODEC enter dlps callback function(Save CODEC register values when system enter DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void CODEC_DLPS_Enter(void) DATA_RAM_FUNCTION;
void CODEC_DLPS_Enter(void)
{
    /* Enable codec function and clock */
    PERIPH->PERI_BD_FUNC0_EN |= (1 << 0) | (1 << 4);

    CODEC_StoreReg[0] = CODEC->CR0;
    CODEC_StoreReg[1] = CODEC->ADC0_CTRL0;
    CODEC_StoreReg[2] = CODEC->ADC0_CTRL1;
    CODEC_StoreReg[3] = CODEC->ADC1_CTRL0;
    CODEC_StoreReg[4] = CODEC->ADC1_CTRL1;
    CODEC_StoreReg[5] = CODEC->ANA_CR1;
    CODEC_StoreReg[6] = CODEC->ASRC_CR0;
    CODEC_StoreReg[7] = CODEC->ASRC_CR1;
    CODEC_StoreReg[8] = CODEC->CLK_CR1;
    CODEC_StoreReg[9] = CODEC->CLK_CR2;
    CODEC_StoreReg[10] = CODEC->CLK_CR3;
    CODEC_StoreReg[11] = CODEC->DAC_CTRL0;
    CODEC_StoreReg[12] = CODEC->DAC_CTRL1;
    CODEC_StoreReg[13] = CODEC->I2S_CTRL;
}

/**
  * @brief  CODEC exit dlps callback function(Resume CODEC register values when system exit DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void CODEC_DLPS_Exit(void) DATA_RAM_FUNCTION;
void CODEC_DLPS_Exit(void)
{
    /* Enable codec function and clock */
    PERIPH->PERI_BD_FUNC0_EN |= (1 << 0) | (1 << 4);

    /* Initialize CODEC */
    if ((CODEC_StoreReg[3] & 0x900) == 0x900)
    {
        /* Configure AMIC parameters */
        CODEC->CR0 = CODEC_StoreReg[0];
    }

    CODEC->ADC0_CTRL0 = CODEC_StoreReg[1];
    CODEC->ADC0_CTRL1 = CODEC_StoreReg[2];
    CODEC->ADC1_CTRL0 = CODEC_StoreReg[3];
    CODEC->ADC1_CTRL1 = CODEC_StoreReg[4];
    CODEC->ANA_CR1 = CODEC_StoreReg[5];
    CODEC->ASRC_CR0 = CODEC_StoreReg[6];
    CODEC->ASRC_CR1 = CODEC_StoreReg[7];
    CODEC->CLK_CR1 = CODEC_StoreReg[8];
    CODEC->CLK_CR2 = CODEC_StoreReg[9];
    CODEC->CLK_CR3 = CODEC_StoreReg[10];
    CODEC->DAC_CTRL0 = CODEC_StoreReg[11];
    CODEC->DAC_CTRL1 = CODEC_StoreReg[12];
    CODEC->I2S_CTRL = CODEC_StoreReg[13];

}
#endif

/********************************************** ********************************************************/
/**************************************** [CODEC_CH0_EQ1 DLPS] ************************************************/
/*******************************************************************************************************/
/*******************************************************************************************************/
#if USE_CODEC_CH0_EQ1_DLPS

uint32_t CODEC_CH0_EQ1_StoreReg[5];

/**
  * @brief  CODEC_CH0_EQ1 enter dlps callback function(Save CODEC_CH0_EQ1 register values when system enter DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void CODEC_CH0_EQ1_DLPS_Enter(void) DATA_RAM_FUNCTION;
void CODEC_CH0_EQ1_DLPS_Enter(void)
{
    /* Enable codec function and clock */
    PERIPH->PERI_BD_FUNC0_EN |= (1 << 0) | (1 << 4);

    CODEC_CH0_EQ1_StoreReg[0] = CODEC_CH0_EQ1->EQ_H0;
    CODEC_CH0_EQ1_StoreReg[1] = CODEC_CH0_EQ1->EQ_B1;
    CODEC_CH0_EQ1_StoreReg[2] = CODEC_CH0_EQ1->EQ_B2;
    CODEC_CH0_EQ1_StoreReg[3] = CODEC_CH0_EQ1->EQ_A1;
    CODEC_CH0_EQ1_StoreReg[4] = CODEC_CH0_EQ1->EQ_A2;
}

/**
  * @brief  CODEC_EQ1 exit dlps callback function(Resume CODEC_EQ1 register values when system exit DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void CODEC_CH0_EQ1_DLPS_Exit(void) DATA_RAM_FUNCTION;
void CODEC_CH0_EQ1_DLPS_Exit(void)
{
    /* Enable codec function and clock */
    PERIPH->PERI_BD_FUNC0_EN |= (1 << 0) | (1 << 4);

    CODEC_CH0_EQ1->EQ_H0 = CODEC_CH0_EQ1_StoreReg[0];
    CODEC_CH0_EQ1->EQ_B1 = CODEC_CH0_EQ1_StoreReg[1];
    CODEC_CH0_EQ1->EQ_B2 = CODEC_CH0_EQ1_StoreReg[2];
    CODEC_CH0_EQ1->EQ_A1 = CODEC_CH0_EQ1_StoreReg[3];
    CODEC_CH0_EQ1->EQ_A2 = CODEC_CH0_EQ1_StoreReg[4];
}
#endif

/********************************************** ********************************************************/
/**************************************** [CODEC_CH0_EQ2 DLPS] ************************************************/
/*******************************************************************************************************/
/*******************************************************************************************************/
#if USE_CODEC_CH0_EQ2_DLPS

uint32_t CODEC_CH0_EQ2_StoreReg[5];

/**
  * @brief  CODEC_CH0_EQ2 enter dlps callback function(Save CODEC_CH0_EQ2 register values when system enter DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void CODEC_CH0_EQ2_DLPS_Enter(void) DATA_RAM_FUNCTION;
void CODEC_CH0_EQ2_DLPS_Enter(void)
{
    /* Enable codec function and clock */
    PERIPH->PERI_BD_FUNC0_EN |= (1 << 0) | (1 << 4);

    CODEC_CH0_EQ2_StoreReg[0] = CODEC_CH0_EQ2->EQ_H0;
    CODEC_CH0_EQ2_StoreReg[1] = CODEC_CH0_EQ2->EQ_B1;
    CODEC_CH0_EQ2_StoreReg[2] = CODEC_CH0_EQ2->EQ_B2;
    CODEC_CH0_EQ2_StoreReg[3] = CODEC_CH0_EQ2->EQ_A1;
    CODEC_CH0_EQ2_StoreReg[4] = CODEC_CH0_EQ2->EQ_A2;
}

/**
  * @brief  CODEC_CH0_EQ2 exit dlps callback function(Resume CODEC_CH0_EQ2 register values when system exit DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void CODEC_CH0_EQ2_DLPS_Exit(void) DATA_RAM_FUNCTION;
void CODEC_CH0_EQ2_DLPS_Exit(void)
{
    /* Enable codec function and clock */
    PERIPH->PERI_BD_FUNC0_EN |= (1 << 0) | (1 << 4);

    CODEC_CH0_EQ2->EQ_H0 = CODEC_CH0_EQ2_StoreReg[0];
    CODEC_CH0_EQ2->EQ_B1 = CODEC_CH0_EQ2_StoreReg[1];
    CODEC_CH0_EQ2->EQ_B2 = CODEC_CH0_EQ2_StoreReg[2];
    CODEC_CH0_EQ2->EQ_A1 = CODEC_CH0_EQ2_StoreReg[3];
    CODEC_CH0_EQ2->EQ_A2 = CODEC_CH0_EQ2_StoreReg[4];
}
#endif

/********************************************** ********************************************************/
/**************************************** [CODEC_CH0_EQ3 DLPS] ************************************************/
/*******************************************************************************************************/
/*******************************************************************************************************/
#if USE_CODEC_CH0_EQ3_DLPS

uint32_t CODEC_CH0_EQ3_StoreReg[5];

/**
  * @brief  CODEC_CH0_EQ3 enter dlps callback function(Save CODEC_CH0_EQ3 register values when system enter DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void CODEC_CH0_EQ3_DLPS_Enter(void) DATA_RAM_FUNCTION;
void CODEC_CH0_EQ3_DLPS_Enter(void)
{
    /* Enable codec function and clock */
    PERIPH->PERI_BD_FUNC0_EN |= (1 << 0) | (1 << 4);

    CODEC_CH0_EQ3_StoreReg[0] = CODEC_CH0_EQ3->EQ_H0;
    CODEC_CH0_EQ3_StoreReg[1] = CODEC_CH0_EQ3->EQ_B1;
    CODEC_CH0_EQ3_StoreReg[2] = CODEC_CH0_EQ3->EQ_B2;
    CODEC_CH0_EQ3_StoreReg[3] = CODEC_CH0_EQ3->EQ_A1;
    CODEC_CH0_EQ3_StoreReg[4] = CODEC_CH0_EQ3->EQ_A2;
}

/**
  * @brief  CODEC_CH0_EQ3 exit dlps callback function(Resume CODEC_CH0_EQ3 register values when system exit DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void CODEC_CH0_EQ3_DLPS_Exit(void) DATA_RAM_FUNCTION;
void CODEC_CH0_EQ3_DLPS_Exit(void)
{
    /* Enable codec function and clock */
    PERIPH->PERI_BD_FUNC0_EN |= (1 << 0) | (1 << 4);

    CODEC_CH0_EQ3->EQ_H0 = CODEC_CH0_EQ3_StoreReg[0];
    CODEC_CH0_EQ3->EQ_B1 = CODEC_CH0_EQ3_StoreReg[1];
    CODEC_CH0_EQ3->EQ_B2 = CODEC_CH0_EQ3_StoreReg[2];
    CODEC_CH0_EQ3->EQ_A1 = CODEC_CH0_EQ3_StoreReg[3];
    CODEC_CH0_EQ3->EQ_A2 = CODEC_CH0_EQ3_StoreReg[4];
}
#endif

/********************************************** ********************************************************/
/**************************************** [CODEC_CH0_EQ4 DLPS] ************************************************/
/*******************************************************************************************************/
/*******************************************************************************************************/
#if USE_CODEC_CH0_EQ4_DLPS

uint32_t CODEC_CH0_EQ4_StoreReg[5];

/**
  * @brief  CODEC_CH0_EQ4 enter dlps callback function(Save CODEC_CH0_EQ4 register values when system enter DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void CODEC_CH0_EQ4_DLPS_Enter(void) DATA_RAM_FUNCTION;
void CODEC_CH0_EQ4_DLPS_Enter(void)
{
    /* Enable codec function and clock */
    PERIPH->PERI_BD_FUNC0_EN |= (1 << 0) | (1 << 4);

    CODEC_CH0_EQ4_StoreReg[0] = CODEC_CH0_EQ4->EQ_H0;
    CODEC_CH0_EQ4_StoreReg[1] = CODEC_CH0_EQ4->EQ_B1;
    CODEC_CH0_EQ4_StoreReg[2] = CODEC_CH0_EQ4->EQ_B2;
    CODEC_CH0_EQ4_StoreReg[3] = CODEC_CH0_EQ4->EQ_A1;
    CODEC_CH0_EQ4_StoreReg[4] = CODEC_CH0_EQ4->EQ_A2;
}

/**
  * @brief  CODEC_CH0_EQ4 exit dlps callback function(Resume CODEC_CH0_EQ4 register values when system exit DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void CODEC_CH0_EQ4_DLPS_Exit(void) DATA_RAM_FUNCTION;
void CODEC_CH0_EQ4_DLPS_Exit(void)
{
    /* Enable codec function and clock */
    PERIPH->PERI_BD_FUNC0_EN |= (1 << 0) | (1 << 4);

    CODEC_CH0_EQ4->EQ_H0 = CODEC_CH0_EQ4_StoreReg[0];
    CODEC_CH0_EQ4->EQ_B1 = CODEC_CH0_EQ4_StoreReg[1];
    CODEC_CH0_EQ4->EQ_B2 = CODEC_CH0_EQ4_StoreReg[2];
    CODEC_CH0_EQ4->EQ_A1 = CODEC_CH0_EQ4_StoreReg[3];
    CODEC_CH0_EQ4->EQ_A2 = CODEC_CH0_EQ4_StoreReg[4];
}
#endif

/********************************************** ********************************************************/
/**************************************** [CODEC_CH0_EQ5 DLPS] ************************************************/
/*******************************************************************************************************/
/*******************************************************************************************************/
#if USE_CODEC_CH0_EQ5_DLPS

uint32_t CODEC_CH0_EQ5_StoreReg[5];

/**
  * @brief  CODEC_CH0_EQ5 enter dlps callback function(Save CODEC_CH0_EQ5 register values when system enter DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void CODEC_CH0_EQ5_DLPS_Enter(void) DATA_RAM_FUNCTION;
void CODEC_CH0_EQ5_DLPS_Enter(void)
{
    /* Enable codec function and clock */
    PERIPH->PERI_BD_FUNC0_EN |= (1 << 0) | (1 << 4);

    CODEC_CH0_EQ5_StoreReg[0] = CODEC_CH0_EQ5->EQ_H0;
    CODEC_CH0_EQ5_StoreReg[1] = CODEC_CH0_EQ5->EQ_B1;
    CODEC_CH0_EQ5_StoreReg[2] = CODEC_CH0_EQ5->EQ_B2;
    CODEC_CH0_EQ5_StoreReg[3] = CODEC_CH0_EQ5->EQ_A1;
    CODEC_CH0_EQ5_StoreReg[4] = CODEC_CH0_EQ5->EQ_A2;
}

/**
  * @brief  CODEC_CH0_EQ5 exit dlps callback function(Resume CODEC_CH0_EQ5 register values when system exit DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void CODEC_CH0_EQ5_DLPS_Exit(void) DATA_RAM_FUNCTION;
void CODEC_CH0_EQ5_DLPS_Exit(void)
{
    /* Enable codec function and clock */
    PERIPH->PERI_BD_FUNC0_EN |= (1 << 0) | (1 << 4);

    CODEC_CH0_EQ5->EQ_H0 = CODEC_CH0_EQ5_StoreReg[0];
    CODEC_CH0_EQ5->EQ_B1 = CODEC_CH0_EQ5_StoreReg[1];
    CODEC_CH0_EQ5->EQ_B2 = CODEC_CH0_EQ5_StoreReg[2];
    CODEC_CH0_EQ5->EQ_A1 = CODEC_CH0_EQ5_StoreReg[3];
    CODEC_CH0_EQ5->EQ_A2 = CODEC_CH0_EQ5_StoreReg[4];
}
#endif

/********************************************** ********************************************************/
/**************************************** [GPIO DLPS] **************************************************/
/*******************************************************************************************************/
/*******************************************************************************************************/
#if USE_GPIO_DLPS

__STATIC_INLINE void GPIO_DLPS_Enter(void);
__STATIC_INLINE void GPIO_DLPS_Exit(void);

uint32_t GPIO_StoreReg[9];         /*  This array should be placed in RAM ON/Buffer ON.    */

/**
  * @brief  GPIO enter dlps callback function(Save GPIO register values when system enter DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void GPIO_DLPS_Enter(void) DATA_RAM_FUNCTION;
void GPIO_DLPS_Enter(void)
{
    PERIPH->PERI_FUNC1_EN |= BIT_PERI_GPIO_EN;
    PERIPH->PERI_CLK_CTRL |= (BIT_SOC_ACTCK_GPIO_EN | SYSBLK_SLPCK_GPIO_EN_Msk);

    GPIO_StoreReg[0] = GPIO->DATAOUT;
    GPIO_StoreReg[1] = GPIO->DATADIR;
    GPIO_StoreReg[2] = GPIO->DATASRC;
    GPIO_StoreReg[3] = GPIO->INTEN;
    GPIO_StoreReg[4] = GPIO->INTMASK;
    GPIO_StoreReg[5] = GPIO->INTTYPE;
    GPIO_StoreReg[6] = GPIO->INTPOLARITY;
    GPIO_StoreReg[7] = GPIO->DEBOUNCE;
    GPIO_StoreReg[8] = *(__IO uint32_t *)(0x40000344UL);

    return;
}

/**
  * @brief  GPIO exit dlps callback function(Resume GPIO register values when system exit DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void GPIO_DLPS_Exit(void) DATA_RAM_FUNCTION;
void GPIO_DLPS_Exit(void)
{
    PERIPH->PERI_FUNC1_EN |= BIT_PERI_GPIO_EN;
    PERIPH->PERI_CLK_CTRL |= (BIT_SOC_ACTCK_GPIO_EN | SYSBLK_SLPCK_GPIO_EN_Msk);

    GPIO->DATADIR       = GPIO_StoreReg[1];
    GPIO->DATASRC       = GPIO_StoreReg[2];
    GPIO->INTMASK       = GPIO_StoreReg[4];
    GPIO->INTTYPE       = GPIO_StoreReg[5];
    GPIO->INTPOLARITY   = GPIO_StoreReg[6];
    GPIO->DEBOUNCE      = GPIO_StoreReg[7];
    GPIO->DATAOUT       = GPIO_StoreReg[0];
    GPIO->INTCLR        = ~(GPIO_StoreReg[1]);
    GPIO->INTEN         = GPIO_StoreReg[3];
    *(__IO uint32_t *)(0x40000344UL) = GPIO_StoreReg[8];

    return;
}
#endif  /* USE_GPIO_DLPS */

/********************************************** ********************************************************/
/**************************************** [I2C0 DLPS] **************************************************/
/*******************************************************************************************************/
/*******************************************************************************************************/
#if USE_I2C0_DLPS
__STATIC_INLINE void I2C0_DLPS_Enter(void);
__STATIC_INLINE void I2C0_DLPS_Exit(void);

uint32_t I2C0_StoreReg[20];        /*  This array should be placed in RAM ON/Buffer ON.    */

/**
  * @brief  I2C0 enter dlps callback function(Save I2C0 register values when system enter DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void I2C0_DLPS_Enter(void) DATA_RAM_FUNCTION;
void I2C0_DLPS_Enter(void)
{
    PERIPH->PERI_CLK_CTRL1 |= (SYSBLK_ACTCK_I2C0_EN_Msk | SYSBLK_SLPCK_I2C0_EN_Msk);
    PERIPH->PERI_FUNC0_EN |= SYSBLK_I2C0_EN_Msk;

    I2C0_StoreReg[0] = I2C0->IC_CON;
    I2C0_StoreReg[1] = I2C0->IC_TAR;
    I2C0_StoreReg[2] = I2C0->IC_SAR;
    I2C0_StoreReg[3] = I2C0->IC_HS_MADDR;

    I2C0_StoreReg[4] = I2C0->IC_SS_SCL_HCNT;
    I2C0_StoreReg[5] = I2C0->IC_SS_SCL_LCNT;
    I2C0_StoreReg[6] = I2C0->IC_FS_SCL_HCNT;
    I2C0_StoreReg[7] = I2C0->IC_FS_SCL_LCNT;
    I2C0_StoreReg[8] = I2C0->IC_HS_SCL_HCNT;
    I2C0_StoreReg[9] = I2C0->IC_HS_SCL_LCNT;

    I2C0_StoreReg[10] = I2C0->IC_INTR_MASK;
    I2C0_StoreReg[11] = I2C0->IC_RX_TL;
    I2C0_StoreReg[12] = I2C0->IC_TX_TL;
    I2C0_StoreReg[13] = I2C0->IC_ENABLE;
    I2C0_StoreReg[14] = I2C0->IC_SDA_HOLD;
    I2C0_StoreReg[15] = I2C0->IC_SLV_DATA_NACK_ONLY;
    I2C0_StoreReg[16] = I2C0->IC_DMA_CR;
    I2C0_StoreReg[17] = I2C0->IC_DMA_TDLR;
    I2C0_StoreReg[18] = I2C0->IC_DMA_RDLR;

    I2C0_StoreReg[19] = I2C0->IC_SDA_SETUP;
}

/**
  * @brief  I2C0 exit dlps callback function(Resume I2C0 register values when system exit DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void I2C0_DLPS_Exit(void) DATA_RAM_FUNCTION;
void I2C0_DLPS_Exit(void)
{
    PERIPH->PERI_CLK_CTRL1 |= (SYSBLK_ACTCK_I2C0_EN_Msk | SYSBLK_SLPCK_I2C0_EN_Msk);
    PERIPH->PERI_FUNC0_EN |= SYSBLK_I2C0_EN_Msk;

    I2C0->IC_CON = I2C0_StoreReg[0];
    I2C0->IC_TAR = I2C0_StoreReg[1];
    I2C0->IC_SAR = I2C0_StoreReg[2];
    I2C0->IC_HS_MADDR = I2C0_StoreReg[3];

    I2C0->IC_SS_SCL_HCNT = I2C0_StoreReg[4];
    I2C0->IC_SS_SCL_LCNT = I2C0_StoreReg[5];
    I2C0->IC_FS_SCL_HCNT = I2C0_StoreReg[6];
    I2C0->IC_FS_SCL_LCNT = I2C0_StoreReg[7];
    I2C0->IC_HS_SCL_HCNT = I2C0_StoreReg[8];
    I2C0->IC_HS_SCL_LCNT = I2C0_StoreReg[9];

    I2C0->IC_INTR_MASK = I2C0_StoreReg[10];
    I2C0->IC_RX_TL = I2C0_StoreReg[11];
    I2C0->IC_TX_TL = I2C0_StoreReg[12];
    I2C0->IC_SDA_HOLD = I2C0_StoreReg[14];
    I2C0->IC_SLV_DATA_NACK_ONLY = I2C0_StoreReg[15];
    I2C0->IC_DMA_CR = I2C0_StoreReg[16];
    I2C0->IC_DMA_TDLR = I2C0_StoreReg[17];
    I2C0->IC_DMA_RDLR = I2C0_StoreReg[18];
    I2C0->IC_SDA_SETUP = I2C0_StoreReg[19];

    I2C0->IC_ENABLE = I2C0_StoreReg[13];
}
#endif

/********************************************** ********************************************************/
/**************************************** [I2C1 DLPS] **************************************************/
/*******************************************************************************************************/
/*******************************************************************************************************/
#if USE_I2C1_DLPS
__STATIC_INLINE void I2C1_DLPS_Enter(void);
__STATIC_INLINE void I2C1_DLPS_Exit(void);

uint32_t I2C1_StoreReg[20];        /*  This array should be placed in RAM ON/Buffer ON.    */

/**
  * @brief  I2C1 enter dlps callback function(Save I2C1 register values when system enter DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void I2C1_DLPS_Enter(void) DATA_RAM_FUNCTION;
void I2C1_DLPS_Enter(void)
{
    PERIPH->PERI_CLK_CTRL1 |= (SYSBLK_ACTCK_I2C1_EN_Msk | SYSBLK_SLPCK_I2C1_EN_Msk);
    PERIPH->PERI_FUNC0_EN |= SYSBLK_I2C1_EN_Msk;

    I2C1_StoreReg[0] = I2C1->IC_CON;
    I2C1_StoreReg[1] = I2C1->IC_TAR;
    I2C1_StoreReg[2] = I2C1->IC_SAR;
    I2C1_StoreReg[3] = I2C1->IC_HS_MADDR;

    I2C1_StoreReg[4] = I2C1->IC_SS_SCL_HCNT;
    I2C1_StoreReg[5] = I2C1->IC_SS_SCL_LCNT;
    I2C1_StoreReg[6] = I2C1->IC_FS_SCL_HCNT;
    I2C1_StoreReg[7] = I2C1->IC_FS_SCL_LCNT;
    I2C1_StoreReg[8] = I2C1->IC_HS_SCL_HCNT;
    I2C1_StoreReg[9] = I2C1->IC_HS_SCL_LCNT;

    I2C1_StoreReg[10] = I2C1->IC_INTR_MASK;
    I2C1_StoreReg[11] = I2C1->IC_RX_TL;
    I2C1_StoreReg[12] = I2C1->IC_TX_TL;
    I2C1_StoreReg[13] = I2C1->IC_ENABLE;
    I2C1_StoreReg[14] = I2C1->IC_SDA_HOLD;
    I2C1_StoreReg[15] = I2C1->IC_SLV_DATA_NACK_ONLY;
    I2C1_StoreReg[16] = I2C1->IC_DMA_CR;
    I2C1_StoreReg[17] = I2C1->IC_DMA_TDLR;
    I2C1_StoreReg[18] = I2C1->IC_DMA_RDLR;

    I2C1_StoreReg[19] = I2C1->IC_SDA_SETUP;
}

/**
  * @brief  I2C1 exit dlps callback function(Resume I2C1 register values when system exit DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void I2C1_DLPS_Exit(void) DATA_RAM_FUNCTION;
void I2C1_DLPS_Exit(void)
{
    PERIPH->PERI_CLK_CTRL1 |= (SYSBLK_ACTCK_I2C1_EN_Msk | SYSBLK_SLPCK_I2C1_EN_Msk);
    PERIPH->PERI_FUNC0_EN |= SYSBLK_I2C1_EN_Msk;

    I2C1->IC_CON = I2C1_StoreReg[0];
    I2C1->IC_TAR = I2C1_StoreReg[1];
    I2C1->IC_SAR = I2C1_StoreReg[2];
    I2C1->IC_HS_MADDR = I2C1_StoreReg[3];

    I2C1->IC_SS_SCL_HCNT = I2C1_StoreReg[4];
    I2C1->IC_SS_SCL_LCNT = I2C1_StoreReg[5];
    I2C1->IC_FS_SCL_HCNT = I2C1_StoreReg[6];
    I2C1->IC_FS_SCL_LCNT = I2C1_StoreReg[7];
    I2C1->IC_HS_SCL_HCNT = I2C1_StoreReg[8];
    I2C1->IC_HS_SCL_LCNT = I2C1_StoreReg[9];

    I2C1->IC_INTR_MASK = I2C1_StoreReg[10];
    I2C1->IC_RX_TL = I2C1_StoreReg[11];
    I2C1->IC_TX_TL = I2C1_StoreReg[12];
    I2C1->IC_SDA_HOLD = I2C1_StoreReg[14];
    I2C1->IC_SLV_DATA_NACK_ONLY = I2C1_StoreReg[15];
    I2C1->IC_DMA_CR = I2C1_StoreReg[16];
    I2C1->IC_DMA_TDLR = I2C1_StoreReg[17];
    I2C1->IC_DMA_RDLR = I2C1_StoreReg[18];
    I2C1->IC_SDA_SETUP = I2C1_StoreReg[19];

    I2C1->IC_ENABLE = I2C1_StoreReg[13];
}
#endif

/********************************************** ********************************************************/
/**************************************** [I2S0 DLPS] **************************************************/
/*******************************************************************************************************/
/*******************************************************************************************************/
#if USE_I2S0_DLPS

uint32_t I2S0_StoreReg[4];

/**
  * @brief  I2S0 enter dlps callback function(Save I2S0 register values when system enter DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void I2S0_DLPS_Enter(void) DATA_RAM_FUNCTION;
void I2S0_DLPS_Enter(void)
{
    PERIPH->PERI_BD_FUNC0_EN |= BIT(1) | BIT(5) | BIT(8);

    I2S0_StoreReg[0] = I2S0->BCLK_DIV;
    I2S0_StoreReg[1] = I2S0->CTRL0;
    I2S0_StoreReg[2] = I2S0->CTRL1;
    I2S0_StoreReg[3] = I2S0->DMA_TRDLR;
}

/**
  * @brief  I2S0 exit dlps callback function(Resume I2S0 register values when system exit DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void I2S0_DLPS_Exit(void) DATA_RAM_FUNCTION;
void I2S0_DLPS_Exit(void)
{
    PERIPH->PERI_BD_FUNC0_EN |= BIT(1) | BIT(5) | BIT(8);

    /* Reset I2S0 module */
    I2S0->CTRL0 |= 1 << 0;
    I2S0->CTRL0 &= ~(1 << 0);

    /* Initialize I2S0 */
    I2S0->BCLK_DIV  = I2S0_StoreReg[0];
    I2S0->CTRL1     = I2S0_StoreReg[2];
    I2S0->DMA_TRDLR = I2S0_StoreReg[3];
    I2S0->CTRL0     = I2S0_StoreReg[1];
}
#endif

/********************************************** ********************************************************/
/**************************************** [I2S1 DLPS] **************************************************/
/*******************************************************************************************************/
/*******************************************************************************************************/
#if USE_I2S1_DLPS

uint32_t I2S1_StoreReg[4];

/**
  * @brief  I2S1 enter dlps callback function(Save I2S1 register values when system enter DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void I2S1_DLPS_Enter(void) DATA_RAM_FUNCTION;
void I2S1_DLPS_Enter(void)
{
    PERIPH->PERI_BD_FUNC0_EN |= BIT(2) | BIT(6) | BIT(8);

    I2S1_StoreReg[0] = I2S1->BCLK_DIV;
    I2S1_StoreReg[1] = I2S1->CTRL0;
    I2S1_StoreReg[2] = I2S1->CTRL1;
    I2S1_StoreReg[3] = I2S1->DMA_TRDLR;
}

/**
  * @brief  I2S1 exit dlps callback function(Resume I2S1 register values when system exit DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void I2S1_DLPS_Exit(void) DATA_RAM_FUNCTION;
void I2S1_DLPS_Exit(void)
{
    PERIPH->PERI_BD_FUNC0_EN |= BIT(2) | BIT(6) | BIT(8);

    /* Reset I2S1 module */
    I2S1->CTRL0 |= 1 << 0;
    I2S1->CTRL0 &= ~(1 << 0);

    /* Initialize I2S1 */
    I2S1->BCLK_DIV  = I2S1_StoreReg[0];
    I2S1->CTRL1     = I2S1_StoreReg[2];
    I2S1->DMA_TRDLR = I2S1_StoreReg[3];
    I2S1->CTRL0     = I2S1_StoreReg[1];
}
#endif

/********************************************** ********************************************************/
/**************************************** [IF8080 DLPS] ************************************************/
/*******************************************************************************************************/
/*******************************************************************************************************/
#if USE_IF8080_DLPS

uint32_t IF8080_StoreReg[5];

/**
  * @brief  IF8080 enter dlps callback function(Save IF8080 register values when system enter DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void IF8080_DLPS_Enter(void) DATA_RAM_FUNCTION;
void IF8080_DLPS_Enter(void)
{
    /* Enable IF8080 interface controller function */
    PERIPH->PERI_FUNC0_EN |= BIT(25);
    /* Enable IF8080 interface controller clock */
    PERIPH->PERI_CLK_CTRL1 |= BIT(28) | BIT(29);

    IF8080_StoreReg[0] = *((volatile uint32_t *)0x400002A8UL) & (0x03 << 28);
    IF8080_StoreReg[1] = IF8080->IMR;
    IF8080_StoreReg[2] = IF8080->CTRL0;
    IF8080_StoreReg[3] = IF8080->CTRL1;
    IF8080_StoreReg[4] = IF8080->CFG;

}

/**
  * @brief  IF8080 exit dlps callback function(Resume IF8080 register values when system exit DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void IF8080_DLPS_Exit(void) DATA_RAM_FUNCTION;
void IF8080_DLPS_Exit(void)
{
    /* Enable IF8080 8080 interface controller function */
    PERIPH->PERI_FUNC0_EN |= BIT(25);
    /* Enable IF8080 8080 interface controller clock */
    PERIPH->PERI_CLK_CTRL1 |= BIT(28) | BIT(29);

    /* Restore Dedicated SDIO pin option */
    *((volatile uint32_t *)0x400002A8UL) &= ~(0x03 << 28);
    *((volatile uint32_t *)0x400002A8UL) |= IF8080_StoreReg[0];

    /* Initialize IF8080 */

    IF8080->IMR    = IF8080_StoreReg[1];
    IF8080->CTRL1  = IF8080_StoreReg[3];
    IF8080->CFG    = IF8080_StoreReg[4];
    IF8080->CTRL0  = IF8080_StoreReg[2];
}
#endif

/********************************************** ********************************************************/
/**************************************** [IR DLPS] **************************************************/
/*******************************************************************************************************/
/*******************************************************************************************************/

#if USE_IR_DLPS
__STATIC_INLINE void IR_DLPS_Enter(void);
__STATIC_INLINE void IR_DLPS_Exit(void);

uint32_t IR_StoreReg[6];        /*  This array should be placed in RAM ON/Buffer ON.    */

/**
  * @brief  IR enter dlps callback function(Save IR register values when system enter DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void IR_DLPS_Enter(void) DATA_RAM_FUNCTION;
void IR_DLPS_Enter(void)
{
    PERIPH->PERI_FUNC0_EN |= (1 << 10);
    PERIPH->PERI_CLK_CTRL0 |= (SYSBLK_ACTCK_IR_EN_Msk | SYSBLK_SLPCK_IR_EN_Msk);

    IR_StoreReg[0] = IR->CLK_DIV;
    IR_StoreReg[1] = IR->TX_CONFIG;
    IR_StoreReg[2] = IR->RX_CONFIG;
    IR_StoreReg[3] = IR->RX_CNT_INT_SEL;


    return;
}

/**
  * @brief  IR exit dlps callback function(Resume IR register values when system exit DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void IR_DLPS_Exit(void) DATA_RAM_FUNCTION;
void IR_DLPS_Exit(void)
{
    PERIPH->PERI_FUNC0_EN |= (1 << 10);
    PERIPH->PERI_CLK_CTRL0 |= (SYSBLK_ACTCK_IR_EN_Msk | SYSBLK_SLPCK_IR_EN_Msk);

    IR->CLK_DIV = IR_StoreReg[0];
    if (IR_StoreReg[1] & BIT31)
    {
        /* RX MODE */
        IR->TX_CONFIG  = IR_StoreReg[1];
        IR->RX_CONFIG  = IR_StoreReg[2];
        IR->RX_CNT_INT_SEL  = IR_StoreReg[3];
    }
    else
    {
        /* TX MODE */
        IR->TX_CONFIG  = IR_StoreReg[1];
        /* If IR TX mode is idle, must write one data firstly */
        IR->TX_FIFO = 0;
    }

    return;
}

#endif

/********************************************** ********************************************************/
/**************************************** [KEYSCAN DLPS] ***********************************************/
/*******************************************************************************************************/
/*******************************************************************************************************/
#if USE_KEYSCAN_DLPS

__STATIC_INLINE void KeyScan_DLPS_Enter(void);
__STATIC_INLINE void KeyScan_DLPS_Exit(void);

uint32_t KeyScan_StoreReg[7];      /*  This array should be placed in RAM ON/Buffer ON.    */

/**
  * @brief  KEYSCAN enter dlps callback function(Save KEYSCAN register values when system enter DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void KeyScan_DLPS_Enter(void) DATA_RAM_FUNCTION;
void KeyScan_DLPS_Enter(void)
{
    /*Open 5M clock source*/
    SYSBLKCTRL->u_20C.RSVD_20C |= BIT26;
    SYSBLKCTRL->u_20C.RSVD_20C |= BIT29;

    PERIPH->PERI_FUNC0_EN |= BIT_PERI_KEYSCAN_EN;
    PERIPH->PERI_CLK_CTRL1 |= (BIT_SOC_ACTCK_KEYSCAN_EN | SYSBLK_SLPCK_KEYSCAN_EN_Msk);

    KeyScan_StoreReg[0] = KEYSCAN->CLKDIV;               /* 0x00 */
    KeyScan_StoreReg[1] = KEYSCAN->TIMERCR;              /* 0x04 */
    KeyScan_StoreReg[2] = KEYSCAN->CR;                   /* 0x08 */
    KeyScan_StoreReg[3] = KEYSCAN->COLCR;                /* 0x0C */
    KeyScan_StoreReg[4] = KEYSCAN->ROWCR;                /* 0x10 */
    KeyScan_StoreReg[6] = KEYSCAN->INTMASK;              /* 0x18 */

    return;
}

/**
  * @brief  KEYSCAN exit dlps callback function(Resume KEYSCAN register values when system exit DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void KeyScan_DLPS_Exit(void) DATA_RAM_FUNCTION;
void KeyScan_DLPS_Exit(void)
{
    /*Open 5M clock source*/
    SYSBLKCTRL->u_20C.RSVD_20C |= BIT26;
    SYSBLKCTRL->u_20C.RSVD_20C |= BIT29;

    PERIPH->PERI_FUNC0_EN |= BIT_PERI_KEYSCAN_EN;
    PERIPH->PERI_CLK_CTRL1 |= (BIT_SOC_ACTCK_KEYSCAN_EN | SYSBLK_SLPCK_KEYSCAN_EN_Msk);

    /* Set FSM to idle state */
    KEYSCAN->CR &= ~BIT31;
    KEYSCAN->CLKDIV = KeyScan_StoreReg[0];
    KEYSCAN->CR = (KeyScan_StoreReg[2] & (~(BIT31)));
    KEYSCAN->TIMERCR = KeyScan_StoreReg[1];
    KEYSCAN->COLCR = KeyScan_StoreReg[3];
    KEYSCAN->ROWCR = KeyScan_StoreReg[4];
    KEYSCAN->INTMASK = KeyScan_StoreReg[6];
    KEYSCAN->CR |= (KeyScan_StoreReg[2] & ((BIT31)));

    return;
}
#endif /* USE_KEYSCAN_DLPS */


/********************************************** ********************************************************/
/**************************************** [QDEC DLPS] **************************************************/
/*******************************************************************************************************/
/*******************************************************************************************************/
#if USE_QDECODER_DLPS
__STATIC_INLINE void QuadDecoder_DLPS_Enter(void);
__STATIC_INLINE void QuadDecoder_DLPS_Exit(void);

uint32_t QuadDecoder_StoreReg[5];  /*  This array should be placed in RAM ON/Buffer ON.    */

/**
  * @brief  QDEC enter dlps callback function(Save QDEC register values when system enter DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void QuadDecoder_DLPS_Enter(void) DATA_RAM_FUNCTION;
void QuadDecoder_DLPS_Enter(void)
{
    /*Open 20M clock source*/
    SYSBLKCTRL->u_20C.RSVD_20C |= BIT26;
    SYSBLKCTRL->u_20C.RSVD_20C |= BIT27;
    SYSBLKCTRL->u_218.PERI_FUNC0_EN |= SYSBLK_QDECODE_EN_Msk;
    SYSBLKCTRL->u_238.PERI_CLK_CTRL1 |= (SYSBLK_ACTCK_QDECODE_EN_Msk | SYSBLK_SLPCK_QDECODE_EN_Msk);

    QuadDecoder_StoreReg[0] = QDEC->REG_DIV;
    QuadDecoder_StoreReg[1] = QDEC->REG_CR_X;
    QuadDecoder_StoreReg[2] = QDEC->REG_CR_Y;
    QuadDecoder_StoreReg[3] = QDEC->REG_CR_Z;
    QuadDecoder_StoreReg[4] = QDEC->INT_MASK;

    return;
}

/**
  * @brief  QDEC exit dlps callback function(Resume QDEC register values when system exit DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void QuadDecoder_DLPS_Exit(void) DATA_RAM_FUNCTION;
void QuadDecoder_DLPS_Exit(void)
{
    /*Open 20M clock source*/
    SYSBLKCTRL->u_20C.RSVD_20C |= BIT26;
    SYSBLKCTRL->u_20C.RSVD_20C |= BIT27;
    SYSBLKCTRL->u_218.PERI_FUNC0_EN |= SYSBLK_QDECODE_EN_Msk;
    SYSBLKCTRL->u_238.PERI_CLK_CTRL1 |= (SYSBLK_ACTCK_QDECODE_EN_Msk | SYSBLK_SLPCK_QDECODE_EN_Msk);

    //clear flags
    QDEC->REG_DIV  = QuadDecoder_StoreReg[0];
    QDEC->REG_CR_X = QuadDecoder_StoreReg[1];
    QDEC->REG_CR_Y = QuadDecoder_StoreReg[2];
    QDEC->REG_CR_Z = QuadDecoder_StoreReg[3];
    QDEC->INT_MASK = QuadDecoder_StoreReg[4];

    return;
}
#endif

/********************************************** ********************************************************/
/**************************************** [SPI0 DLPS] **************************************************/
/*******************************************************************************************************/
/*******************************************************************************************************/
#if USE_SPI0_DLPS
__STATIC_INLINE void SPI0_DLPS_Enter(void);
__STATIC_INLINE void SPI0_DLPS_Exit(void);

uint32_t SPI0_StoreReg[14];        /*  This array should be placed in RAM ON/Buffer ON.    */

/**
  * @brief  SPI0 enter dlps callback function(Save SPI0 register values when system enter DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void SPI0_DLPS_Enter(void) DATA_RAM_FUNCTION;
void SPI0_DLPS_Enter(void)
{
    SYSBLKCTRL->u_218.PERI_FUNC0_EN |=  SYSBLK_SPI0_EN_Msk;
    SYSBLKCTRL->u_234.PERI_CLK_CTRL0 |= (SYSBLK_ACTCK_SPI0_EN_Msk | SYSBLK_SLPCK_SPI0_EN_Msk);

    SPI0_StoreReg[0] = SPI0->CTRLR0;
    SPI0_StoreReg[1] = SPI0->CTRLR1;
    SPI0_StoreReg[2] = SPI0->SSIENR;
    SPI0_StoreReg[3] = SPI0->SER;
    SPI0_StoreReg[4] = SPI0->BAUDR;
    SPI0_StoreReg[5] = SPI0->TXFTLR;
    SPI0_StoreReg[6] = SPI0->RXFTLR;
    SPI0_StoreReg[7] = SPI0->IMR;
    SPI0_StoreReg[8] = SPI0->DMACR;
    SPI0_StoreReg[9] = SPI0->DMATDLR;
    SPI0_StoreReg[10] = SPI0->DMARDLR;
    SPI0_StoreReg[11] = SPI0->RX_SAMPLE_DLY;
    SPI0_StoreReg[12] = *(volatile uint32_t *)0x40000308;
    SPI0_StoreReg[13] = *(volatile uint32_t *)0x4000035CUL;
}

/**
  * @brief  SPI0 exit dlps callback function(Resume SPI0 register values when system exit DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void SPI0_DLPS_Exit(void) DATA_RAM_FUNCTION;
void SPI0_DLPS_Exit(void)
{
    *(volatile uint32_t *)0x4000035CUL = SPI0_StoreReg[13];
    *(volatile uint32_t *)0x40000308 = SPI0_StoreReg[12];
    SYSBLKCTRL->u_218.PERI_FUNC0_EN |=  SYSBLK_SPI0_EN_Msk;
    SYSBLKCTRL->u_234.PERI_CLK_CTRL0 |= (SYSBLK_ACTCK_SPI0_EN_Msk | SYSBLK_SLPCK_SPI0_EN_Msk);

    SPI0->CTRLR0 = SPI0_StoreReg[0];
    SPI0->CTRLR1 = SPI0_StoreReg[1];
    SPI0->SER = SPI0_StoreReg[3];
    SPI0->BAUDR = SPI0_StoreReg[4];
    SPI0->TXFTLR = SPI0_StoreReg[5];
    SPI0->RXFTLR = SPI0_StoreReg[6];
    SPI0->IMR = SPI0_StoreReg[7];
    SPI0->DMACR = SPI0_StoreReg[8];
    SPI0->DMATDLR = SPI0_StoreReg[9];
    SPI0->DMARDLR = SPI0_StoreReg[10];
    SPI0->RX_SAMPLE_DLY = SPI0_StoreReg[11];

    /* Enable the selected SPI peripheral */
    SPI0->SSIENR = SPI0_StoreReg[2];
}
#endif


/********************************************** ********************************************************/
/**************************************** [SPI1 DLPS] **************************************************/
/*******************************************************************************************************/
/*******************************************************************************************************/
#if USE_SPI1_DLPS
__STATIC_INLINE void SPI1_DLPS_Enter(void);
__STATIC_INLINE void SPI1_DLPS_Exit(void);

uint32_t SPI1_StoreReg[14];        /*  This array should be placed in RAM ON/Buffer ON.    */

/**
  * @brief  SPI1 enter dlps callback function(Save SPI1 register values when system enter DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void SPI1_DLPS_Enter(void) DATA_RAM_FUNCTION;
void SPI1_DLPS_Enter(void)
{
    SYSBLKCTRL->u_218.PERI_FUNC0_EN |=  SYSBLK_SPI1_EN_Msk;
    SYSBLKCTRL->u_234.PERI_CLK_CTRL0 |= (SYSBLK_ACTCK_SPI1_EN_Msk | SYSBLK_SLPCK_SPI1_EN_Msk);

    SPI1_StoreReg[0] = SPI1->CTRLR0;
    SPI1_StoreReg[1] = SPI1->CTRLR1;
    SPI1_StoreReg[2] = SPI1->SSIENR;
    SPI1_StoreReg[3] = SPI1->SER;
    SPI1_StoreReg[4] = SPI1->BAUDR;
    SPI1_StoreReg[5] = SPI1->TXFTLR;
    SPI1_StoreReg[6] = SPI1->RXFTLR;
    SPI1_StoreReg[7] = SPI1->IMR;
    SPI1_StoreReg[8] = SPI1->DMACR;
    SPI1_StoreReg[9] = SPI1->DMATDLR;
    SPI1_StoreReg[10] = SPI1->DMARDLR;
    SPI1_StoreReg[11] = SPI1->RX_SAMPLE_DLY;
    SPI1_StoreReg[12] = *(volatile uint32_t *)0x40000308;
    SPI1_StoreReg[13] = *(volatile uint32_t *)0x4000035CUL;
}

/**
  * @brief  SPI1 exit dlps callback function(Resume SPI1 register values when system exit DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void SPI1_DLPS_Exit(void) DATA_RAM_FUNCTION;
void SPI1_DLPS_Exit(void)
{
    *(volatile uint32_t *)0x4000035CUL = SPI1_StoreReg[13];
    *(volatile uint32_t *)0x40000308 = SPI1_StoreReg[12];
    SYSBLKCTRL->u_218.PERI_FUNC0_EN |=  SYSBLK_SPI1_EN_Msk;
    SYSBLKCTRL->u_234.PERI_CLK_CTRL0 |= (SYSBLK_ACTCK_SPI1_EN_Msk | SYSBLK_SLPCK_SPI1_EN_Msk);

    SPI1->CTRLR0 = SPI1_StoreReg[0];
    SPI1->CTRLR1 = SPI1_StoreReg[1];
    SPI1->SER = SPI1_StoreReg[3];
    SPI1->BAUDR = SPI1_StoreReg[4];
    SPI1->TXFTLR = SPI1_StoreReg[5];
    SPI1->RXFTLR = SPI1_StoreReg[6];
    SPI1->IMR = SPI1_StoreReg[7];
    SPI1->DMACR = SPI1_StoreReg[8];
    SPI1->DMATDLR = SPI1_StoreReg[9];
    SPI1->DMARDLR = SPI1_StoreReg[10];
    SPI1->RX_SAMPLE_DLY = SPI1_StoreReg[11];

    /* Enable the selected SPI peripheral */
    SPI1->SSIENR = SPI1_StoreReg[2];
}
#endif

/********************************************** ********************************************************/
/**************************************** [SPI2W DLPS] **************************************************/
/*******************************************************************************************************/
/*******************************************************************************************************/
#if USE_SPI2W_DLPS
__STATIC_INLINE void SPI2W_DLPS_Enter(void);
__STATIC_INLINE void SPI2W_DLPS_Exit(void);

uint32_t SPI2W_StoreReg[1];        /*  This array should be placed in RAM ON/Buffer ON.    */

/**
  * @brief  SPI2W enter dlps callback function(Save SPI2W register values when system enter DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void SPI2W_DLPS_Enter(void) DATA_RAM_FUNCTION;
void SPI2W_DLPS_Enter(void)
{
    /*Open 20M clock source*/
    SYSBLKCTRL->u_20C.RSVD_20C |= BIT26;
    SYSBLKCTRL->u_20C.RSVD_20C |= BIT27;

    SYSBLKCTRL->u_218.PERI_FUNC0_EN |=  SYSBLK_SPI2W_EN_Msk;
    SYSBLKCTRL->u_238.PERI_CLK_CTRL1 |= (SYSBLK_ACTCK_SPI2WIRE_EN_Msk | SYSBLK_SLPCK_SPI2WIRE_EN_Msk);

    SPI2W_StoreReg[0] = SPI3WIRE->CFGR;
}

/**
  * @brief  SPI2W exit dlps callback function(Resume SPI2W register values when system exit DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void SPI2W_DLPS_Exit(void) DATA_RAM_FUNCTION;
void SPI2W_DLPS_Exit(void)
{
    /*Open 20M clock source*/
    SYSBLKCTRL->u_20C.RSVD_20C |= BIT26;
    SYSBLKCTRL->u_20C.RSVD_20C |= BIT27;

    SYSBLKCTRL->u_218.PERI_FUNC0_EN |=  SYSBLK_SPI2W_EN_Msk;
    SYSBLKCTRL->u_238.PERI_CLK_CTRL1 |= (SYSBLK_ACTCK_SPI2WIRE_EN_Msk | SYSBLK_SLPCK_SPI2WIRE_EN_Msk);

    SPI3WIRE->CFGR = SPI2W_StoreReg[0];
}
#endif


/********************************************** ********************************************************/
/**************************************** [Timer & PWM DLPS] *******************************************/
/*******************************************************************************************************/
/*******************************************************************************************************/
#if USE_TIM_DLPS

#include "rtl876x_tim.h"
__STATIC_INLINE void TIM_DLPS_Enter(void);
__STATIC_INLINE void TIM_DLPS_Exit(void);

//uint32_t TIM_StoreReg[26];      /*  This array should be placed in RAM ON/Buffer ON.    */
uint32_t TIM_StoreReg[28];
/* PWM, use with timer */
uint32_t PWM0_StoreReg;         /*  This array should be placed in RAM ON/Buffer ON.    */

uint32_t PWM1_StoreReg;         /*  This array should be placed in RAM ON/Buffer ON.    */

/**
  * @brief  TIMER enter dlps callback function(Save TIMER register values when system enter DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void TIM_DLPS_Enter(void) DATA_RAM_FUNCTION;
void TIM_DLPS_Enter(void)
{
    SYSBLKCTRL->u_210.SOC_FUNC_EN |= BIT(16);
    SYSBLKCTRL->u_230.CLK_CTRL |= (SYSBLK_ACTCK_TIMER_EN_Msk | SYSBLK_SLPCK_TIMER_EN_Msk);

    TIM_StoreReg[0] = TIM0->LoadCount;
    TIM_StoreReg[1] = TIM0->ControlReg;
    TIM_StoreReg[2] = TIMER0_LOAD_COUNT2;

    TIM_StoreReg[3] = TIM1->LoadCount;
    TIM_StoreReg[4] = TIM1->ControlReg;
    TIM_StoreReg[5] = TIMER1_LOAD_COUNT2;

    TIM_StoreReg[6] = TIM2->LoadCount;
    TIM_StoreReg[7] = TIM2->ControlReg;
    TIM_StoreReg[8] = TIMER2_LOAD_COUNT2;

    TIM_StoreReg[9] = TIM3->LoadCount;
    TIM_StoreReg[10] = TIM3->ControlReg;
    TIM_StoreReg[11] = TIMER3_LOAD_COUNT2;

    TIM_StoreReg[12] = TIM4->LoadCount;
    TIM_StoreReg[13] = TIM4->ControlReg;
    TIM_StoreReg[14] = TIMER4_LOAD_COUNT2;

    TIM_StoreReg[15] = TIM5->LoadCount;
    TIM_StoreReg[16] = TIM5->ControlReg;
    TIM_StoreReg[17] = TIMER5_LOAD_COUNT2;

    TIM_StoreReg[18] = TIM6->LoadCount;
    TIM_StoreReg[19] = TIM6->ControlReg;
    TIM_StoreReg[20] = TIMER6_LOAD_COUNT2;

    TIM_StoreReg[21] = TIM7->LoadCount;
    TIM_StoreReg[22] = TIM7->ControlReg;
    TIM_StoreReg[23] = TIMER7_LOAD_COUNT2;

//   TIM_StoreReg[24] = *((volatile uint32_t *)0x4000035CUL);
//   TIM_StoreReg[25] = *((volatile uint32_t *)0x40000360UL);

    TIM_StoreReg[24] = *((volatile uint32_t *)0x40000348UL);
    TIM_StoreReg[25] = *((volatile uint32_t *)0x40000360UL);

    TIM_StoreReg[26] = *((volatile uint32_t *)0x4000600CUL);
    TIM_StoreReg[27] = *((volatile uint32_t *)0x40000384UL);
    PWM0_StoreReg = TIMER_PWM0_CR;

    PWM1_StoreReg = TIMER_PWM1_CR;
}

/**
  * @brief  TIMER exit dlps callback function(Resume TIMER register values when system exit DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void TIM_DLPS_Exit(void) DATA_RAM_FUNCTION;
void TIM_DLPS_Exit(void)
{
    /* Enable timer IP clock and function */
    SYSBLKCTRL->u_210.SOC_FUNC_EN |= BIT(16);
    SYSBLKCTRL->u_230.CLK_CTRL |= (SYSBLK_ACTCK_TIMER_EN_Msk | SYSBLK_SLPCK_TIMER_EN_Msk);

    *((volatile uint32_t *)0x4000035CUL) = TIM_StoreReg[24];
    *((volatile uint32_t *)0x40000360UL) = TIM_StoreReg[25];
//  *((volatile uint32_t *)0x40000348UL) = TIM_StoreReg[24] ;
//   *((volatile uint32_t *)0x40000360UL) = TIM_StoreReg[25] ;
    TIM0->LoadCount = TIM_StoreReg[0];
    TIM0->ControlReg = TIM_StoreReg[1];
    TIMER0_LOAD_COUNT2 = TIM_StoreReg[2];

    TIM1->LoadCount = TIM_StoreReg[3];
    TIM1->ControlReg = TIM_StoreReg[4];
    TIMER1_LOAD_COUNT2 = TIM_StoreReg[5];

    TIM2->LoadCount = TIM_StoreReg[6];
    TIM2->ControlReg = TIM_StoreReg[7];
    TIMER2_LOAD_COUNT2 = TIM_StoreReg[8];

    TIM3->LoadCount = TIM_StoreReg[9];
    TIM3->ControlReg = TIM_StoreReg[10];
    TIMER3_LOAD_COUNT2 = TIM_StoreReg[11];

    TIM4->LoadCount = TIM_StoreReg[12];
    TIM4->ControlReg = TIM_StoreReg[13];
    TIMER4_LOAD_COUNT2 = TIM_StoreReg[14];

    TIM5->LoadCount = TIM_StoreReg[15];
    TIM5->ControlReg = TIM_StoreReg[16];
    TIMER5_LOAD_COUNT2 = TIM_StoreReg[17];

    TIM6->LoadCount = TIM_StoreReg[18];
    TIM6->ControlReg = TIM_StoreReg[19];
    TIMER6_LOAD_COUNT2 = TIM_StoreReg[20];

    TIM7->LoadCount = TIM_StoreReg[21];
    TIM7->ControlReg = TIM_StoreReg[22];
    TIMER7_LOAD_COUNT2 = TIM_StoreReg[23];

//   *((volatile uint32_t *)0x4000600CUL) = TIM_StoreReg[26];
//   *((volatile uint32_t *)0x40000384UL) = TIM_StoreReg[27];
    TIMER_PWM0_CR = PWM0_StoreReg;

    TIMER_PWM1_CR = PWM1_StoreReg;
}
#endif  /* USE_TIM_DLPS */


/********************************************** ********************************************************/
/**************************************** [UART DLPS] **************************************************/
/*******************************************************************************************************/
/*******************************************************************************************************/
#if USE_UART0_DLPS
#include "rtl876x_uart.h"

__STATIC_INLINE void UART0_DLPS_Enter(void);
__STATIC_INLINE void UART0_DLPS_Exit(void);

uint32_t UART0_StoreReg[12];        /*  This array should be placed in RAM ON/Buffer ON.    */

/**
  * @brief  UART0 enter dlps callback function(Save UART0 register values when system enter DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void UART0_DLPS_Enter(void) DATA_RAM_FUNCTION;
void UART0_DLPS_Enter(void)
{
    PERIPH->PERI_FUNC0_EN |= (1 << 0);
    PERIPH->PERI_CLK_CTRL0 |= (SYSBLK_ACTCK_UART0DATA_EN_Msk | SYSBLK_SLPCK_UART0DATA_EN_Msk);

    //access DLH and DLL
    UART0->LCR |= (1 << 7);
    UART0_StoreReg[0] = UART0->DLL;
    UART0_StoreReg[1] = UART0->DLH_INTCR;
    UART0->LCR &= (~(1 << 7));

    //save other registers
    UART0_StoreReg[2] = UART0->DLH_INTCR;
    UART0_StoreReg[4] = UART0->LCR;
    UART0_StoreReg[5] = UART0->MCR;
    UART0_StoreReg[6] = UART0->SPR;
    UART0_StoreReg[7] = UART0->STSR;
    UART0_StoreReg[8] = UART0->RX_IDLE_TOCR;
    UART0_StoreReg[9] = UART0->RX_IDLE_INTCR;
    UART0_StoreReg[10] = UART0->MISCR;
    UART0_StoreReg[11] = UART0->INTMASK;

    return;
}

/**
  * @brief  UART0 exit dlps callback function(Resume UART0 register values when system exit DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void UART0_DLPS_Exit(void) DATA_RAM_FUNCTION;
void UART0_DLPS_Exit(void)
{
    PERIPH->PERI_FUNC0_EN |= (1 << 0);
    PERIPH->PERI_CLK_CTRL0 |= (SYSBLK_ACTCK_UART0DATA_EN_Msk | SYSBLK_SLPCK_UART0DATA_EN_Msk);

    //access DLH and DLL
    UART0->LCR |= (1 << 7);
    UART0->DLL = UART0_StoreReg[0];
    UART0->DLH_INTCR =  UART0_StoreReg[1];
    UART0->LCR &= (~(1 << 7));

    //access other registers
    UART0->INTID_FCR = (((UART0_StoreReg[7] & BIT24) >> 21) | ((UART0_StoreReg[7] & 0x7C000000) >> 18) |
                        (1));
    UART0->LCR = UART0_StoreReg[4];
    UART0->MCR = UART0_StoreReg[5];
    UART0->SPR = UART0_StoreReg[6];
    UART0->STSR = UART0_StoreReg[7];
    UART0->DLH_INTCR = UART0_StoreReg[2];
    UART0->RX_IDLE_TOCR = UART0_StoreReg[8];
    UART0->RX_IDLE_INTCR = UART0_StoreReg[9];
    UART0->MISCR = UART0_StoreReg[10];
    UART0->INTMASK = UART0_StoreReg[11];

    return;
}
#endif


#if USE_UART1_DLPS
#include "rtl876x_uart.h"

__STATIC_INLINE void UART1_DLPS_Enter(void);
__STATIC_INLINE void UART1_DLPS_Exit(void);

uint32_t UART1_StoreReg[12];        /*  This array should be placed in RAM ON/Buffer ON.    */

/**
  * @brief  UART1 enter dlps callback function(Save UART1 register values when system enter DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void UART1_DLPS_Enter(void) DATA_RAM_FUNCTION;
void UART1_DLPS_Enter(void)
{
    //enable log uart peripheral & clock
    SYSBLKCTRL->u_210.SOC_FUNC_EN |= (1 << 12);
    SYSBLKCTRL->u_230.CLK_CTRL |= ((1 << 12) | (1 << 13));

    //access DLH and DLL
    UART1->LCR |= (1 << 7);
    UART1_StoreReg[0] = UART1->DLL;
    UART1_StoreReg[1] = UART1->DLH_INTCR;
    UART1->LCR &= (~(1 << 7));

    //save other registers
    UART1_StoreReg[2] = UART1->DLH_INTCR;
    UART1_StoreReg[4] = UART1->LCR;
    UART1_StoreReg[5] = UART1->MCR;
    UART1_StoreReg[6] = UART1->SPR;
    UART1_StoreReg[7] = UART1->STSR;
    UART1_StoreReg[8] = UART1->RX_IDLE_TOCR;
    UART1_StoreReg[9] = UART1->RX_IDLE_INTCR;
    UART1_StoreReg[10] = UART1->MISCR;
    UART1_StoreReg[11] = UART1->INTMASK;

    return;
}

/**
  * @brief  UART1 exit dlps callback function(Resume UART1 register values when system exit DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void UART1_DLPS_Exit(void) DATA_RAM_FUNCTION;
void UART1_DLPS_Exit(void)
{
    //enable log uart peripheral & clock
    SYSBLKCTRL->u_210.SOC_FUNC_EN |= (1 << 12);
    SYSBLKCTRL->u_230.CLK_CTRL |= ((1 << 12) | (1 << 13));

    //access DLH and DLL
    UART1->LCR |= (1 << 7);
    UART1->DLL = UART1_StoreReg[0];
    UART1->DLH_INTCR =  UART1_StoreReg[1];
    UART1->LCR &= (~(1 << 7));

    //access other registers
    UART1->INTID_FCR = (((UART1_StoreReg[7] & BIT24) >> 21) | ((UART1_StoreReg[7] & 0x7C000000)
                                                               >> 18) | (1));
    UART1->LCR = UART1_StoreReg[4];
    UART1->MCR = UART1_StoreReg[5];
    UART1->SPR = UART1_StoreReg[6];
    UART1->STSR = UART1_StoreReg[7];
    UART1->DLH_INTCR = UART1_StoreReg[2];
    UART1->RX_IDLE_TOCR = UART1_StoreReg[8];
    UART1->RX_IDLE_INTCR = UART1_StoreReg[9];
    UART1->MISCR = UART1_StoreReg[10];
    UART1->INTMASK = UART1_StoreReg[11];

    return;
}
#endif

#if USE_LOG_UART2_DLPS
#include "rtl876x_uart.h"

__STATIC_INLINE void LOG_UART2_DLPS_Enter(void);
__STATIC_INLINE void LOG_UART2_DLPS_Exit(void);

uint32_t LOG_UART2_StoreReg[12];        /*  This array should be placed in RAM ON/Buffer ON.    */

/**
  * @brief LOG_UART enter dlps callback function(Save LOG_UART register values when system enter DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void LOG_UART2_DLPS_Enter(void) DATA_RAM_FUNCTION;
void LOG_UART2_DLPS_Enter(void)
{
    //enable log uart peripheral & clock
    SYSBLKCTRL->u_210.SOC_FUNC_EN |= (1 << 12);
    SYSBLKCTRL->u_230.CLK_CTRL |= ((1 << 12) | (1 << 13));

    //access DLH and DLL
    UART2->LCR |= (1 << 7);
    LOG_UART2_StoreReg[0] = UART2->DLL;
    LOG_UART2_StoreReg[1] = UART2->DLH_INTCR;
    UART2->LCR &= (~(1 << 7));

    //save other registers
    LOG_UART2_StoreReg[2] = UART2->DLH_INTCR;
    LOG_UART2_StoreReg[4] = UART2->LCR;
    LOG_UART2_StoreReg[5] = UART2->MCR;
    LOG_UART2_StoreReg[6] = UART2->SPR;
    LOG_UART2_StoreReg[7] = UART2->STSR;
    LOG_UART2_StoreReg[8] = UART2->RX_IDLE_TOCR;
    LOG_UART2_StoreReg[9] = UART2->RX_IDLE_INTCR;
    LOG_UART2_StoreReg[10] = UART2->MISCR;
    LOG_UART2_StoreReg[11] = UART2->INTMASK;

    return;
}

/**
  * @brief  LOG_UART exit dlps callback function(Resume LOG_UART register values when system exit DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void LOG_UART2_DLPS_Exit(void) DATA_RAM_FUNCTION;
void LOG_UART2_DLPS_Exit(void)
{
    //enable log uart peripheral & clock
    SYSBLKCTRL->u_210.SOC_FUNC_EN |= (1 << 12);
    SYSBLKCTRL->u_230.CLK_CTRL |= ((1 << 12) | (1 << 13));

    //access DLH and DLL
    UART2->LCR |= (1 << 7);
    UART2->DLL = LOG_UART2_StoreReg[0];
    UART2->DLH_INTCR =  LOG_UART2_StoreReg[1];
    UART2->LCR &= (~(1 << 7));

    //access other registers
    UART2->INTID_FCR = (((LOG_UART2_StoreReg[7] & BIT24) >> 21) | ((LOG_UART2_StoreReg[7] & 0x7C000000)
                                                                   >> 18) | (1));
    UART2->LCR = LOG_UART2_StoreReg[4];
    UART2->MCR = LOG_UART2_StoreReg[5];
    UART2->SPR = LOG_UART2_StoreReg[6];
    UART2->STSR = LOG_UART2_StoreReg[7];
    UART2->DLH_INTCR = LOG_UART2_StoreReg[2];
    UART2->RX_IDLE_TOCR = LOG_UART2_StoreReg[8];
    UART2->RX_IDLE_INTCR = LOG_UART2_StoreReg[9];
    UART2->MISCR = LOG_UART2_StoreReg[10];
    UART2->INTMASK = LOG_UART2_StoreReg[11];

    return;
}
#endif

/** @} */ /* End of group IO_DLPS_Functions */

/*============================================================================*
 *                         Platform DLPS
 *============================================================================*/

/** @defgroup Platform_DLPS_Functions Platform DLPS Functions
  * @{
  */

/********************************************** ********************************************************/
/*********************************** [USER Enter DLPS CALLBACK FUCN] *****************************************/
/*******************************************************************************************************/
/*******************************************************************************************************/
#if USE_USER_DEFINE_DLPS_ENTER_CB

DLPS_IO_EnterDlpsCB User_IO_EnterDlpsCB = NULL;

#endif /* USE_USER_DEFINE_DLPS_EXIT_CB */

/********************************************** ********************************************************/
/*********************************** [USER Exit DLPS CALLBACK FUCN] *****************************************/
/*******************************************************************************************************/
/*******************************************************************************************************/
#if USE_USER_DEFINE_DLPS_EXIT_CB

DLPS_IO_ExitDlpsCB User_IO_ExitDlpsCB = NULL;

#endif /* USE_USER_DEFINE_DLPS_EXIT_CB */


#if USE_IO_DRIVER_DLPS

/********************************************** ********************************************************/
/**************************************** [CPU & PINMUX DLPS]*******************************************/
/*******************************************************************************************************/
/*******************************************************************************************************/

__STATIC_INLINE void CPU_DLPS_Enter(void);
__STATIC_INLINE void CPU_DLPS_Exit(void);
__STATIC_INLINE void Log_SWD_DLPS_Enter(void);
__STATIC_INLINE void Log_SWD_DLPS_Exit(void);

uint32_t CPU_StoreReg[33];         /*  This array should be placed in RAM ON/Buffer ON.    */
uint32_t PeriIntStoreReg = 0;

/**
  * @brief  CPU enter dlps callback function(Save CPU register values when system enter DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE void CPU_DLPS_Enter(void) DATA_RAM_FUNCTION;
void CPU_DLPS_Enter(void)
{
    //NVIC store
    uint32_t i;

    CPU_StoreReg[0] = NVIC->ISER[0];
    CPU_StoreReg[1] = NVIC->ISPR[0];
    CPU_StoreReg[2] = NVIC->IABR[0];

    //The priority of system on interrupt is 0 at first DLPS ENTER CB, so store and restore is skipped
    for (i = 3; i < 32; ++i)  //skip System_IRQn, WDG_IRQn, BTMAC_IRQn which are handled in rom
    {
        CPU_StoreReg[i] = NVIC->IP[i];
    }

    CPU_StoreReg[32] = SCB->VTOR;
    /* Save Vendor register */
    PeriIntStoreReg = PERIPHINT->EN;

    return;
}

/**
  * @brief  CPU exit dlps callback function(Resume CPU register values when system exit DLPS)
  * @param  None
  * @retval None
  */
__STATIC_INLINE  void CPU_DLPS_Exit(void) DATA_RAM_FUNCTION;
void CPU_DLPS_Exit(void)
{
    //NVIC restore
    uint32_t i;

    //Don't restore NVIC pending register, but report warning
    //NVIC->ISPR[0] = CPU_StoreReg[1];
    if (CPU_StoreReg[0] & CPU_StoreReg[1])
    {
        /* During enter and exit dlps, system will disable all interrupts. If any interrupt occurs during this period, this log will be printed.
        Every bit of pending register corresponds to an interrupt. Please refer to IRQn_Type from System_IRQn to UART2_IRQn.
        For example:  "miss interrupt: pending register: 0x42000"
        It means that RTC and ADC interrupt occur during dlps store and restore flow. But because all interrupts are masked, these interrupts are pending.
        */
        DLPS_PRINT_WARN1("miss interrupt: pending register: 0x%x", CPU_StoreReg[1]);
    }
    NVIC->IABR[0] = CPU_StoreReg[2];

    //The priority of system on interrupt is 0 at first DLPS ENTER CB, so store and restore is skipped
    for (i = 3; i < 32; ++i) //skip System_IRQn, WDG_IRQn, BTMAC_IRQn which are handled in rom
    {
        NVIC->IP[i] = CPU_StoreReg[i];
    }

    SCB->VTOR = CPU_StoreReg[32];
    PERIPHINT->EN = PeriIntStoreReg;
    NVIC->ISER[0] = CPU_StoreReg[0];

#if (DEBUG_WATCHPOINT_ENABLE == 1)
    void debug_monitor_enable(void);
    debug_monitor_enable();
#endif

    return;
}

/********************************************** ********************************************************/
/**************************************** [LOG & SWD DLPS]*******************************************/
/*******************************************************************************************************/
/*******************************************************************************************************/

/**
  * @brief  Set Log and SWD pins to SW mode.
  * @param  void.
  * @retval void.
  */
__STATIC_INLINE void Log_SWD_DLPS_Enter(void) DATA_RAM_FUNCTION;
void Log_SWD_DLPS_Enter(void)
{
    if (OTP->SWD_ENABLE)
    {
        Pad_Config(P1_0, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_DOWN, PAD_OUT_DISABLE, PAD_OUT_LOW);
        Pad_Config(P1_1, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_DOWN, PAD_OUT_DISABLE, PAD_OUT_LOW);
    }

    Pad_Config(OTP->logPin, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
}

/**
  * @brief  Set Log and SWD pins to PINMUX mode.
  * @param  void.
  * @retval void.
  */
__STATIC_INLINE void Log_SWD_DLPS_Exit(void) DATA_RAM_FUNCTION;
void Log_SWD_DLPS_Exit(void)
{
    Pad_Config(OTP->logPin, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE, PAD_OUT_LOW);

    if (OTP->SWD_ENABLE)
    {
        Pad_Config(P1_0, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
        Pad_Config(P1_1, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
    }
}

/********************************************** ********************************************************/
/*********************************** [Enter & Exit DLPS CALLBACK FUNC] *********************************/
/*******************************************************************************************************/
/*******************************************************************************************************/

/**
  * @brief  IO enter dlps callback function
  * @param  None
  * @retval None
  */
void DLPS_IO_EnterDlpsCb(void) DATA_RAM_FUNCTION;
void DLPS_IO_EnterDlpsCb(void)
{
    /* low stack do it instead */
//    Pad_ClearAllWakeupINT();

//    DBG_DIRECT("DLPS_IO_EnterDlpsCb");
    NVIC_DisableIRQ(System_IRQn);
    CPU_DLPS_Enter();

    Pinmux_DLPS_Enter();

#if USE_USER_DEFINE_DLPS_ENTER_CB
    if (User_IO_EnterDlpsCB)
    {
        User_IO_EnterDlpsCB();
    }
#endif

#if USE_ADC_DLPS
    ADC_DLPS_Enter();
#endif

#if USE_CODEC_DLPS
    CODEC_DLPS_Enter();
#endif

#if USE_CODEC_CH0_EQ1_DLPS
    CODEC_CH0_EQ1_DLPS_Enter();
#endif

#if USE_CODEC_CH0_EQ2_DLPS
    CODEC_CH0_EQ2_DLPS_Enter();
#endif

#if USE_CODEC_CH0_EQ3_DLPS
    CODEC_CH0_EQ3_DLPS_Enter();
#endif

#if USE_CODEC_CH0_EQ4_DLPS
    CODEC_CH0_EQ4_DLPS_Enter();
#endif

#if USE_CODEC_CH0_EQ5_DLPS
    CODEC_CH0_EQ5_DLPS_Enter();
#endif

#if USE_GPIO_DLPS
    GPIO_DLPS_Enter();
#endif

#if USE_I2C0_DLPS
    I2C0_DLPS_Enter();
#endif

#if USE_I2C1_DLPS
    I2C1_DLPS_Enter();
#endif

#if USE_I2S0_DLPS
    I2S0_DLPS_Enter();
#endif

#if USE_I2S1_DLPS
    I2S1_DLPS_Enter();
#endif

#if USE_IF8080_DLPS
    IF8080_DLPS_Enter();
#endif

#if USE_IR_DLPS
    IR_DLPS_Enter();
#endif

#if USE_KEYSCAN_DLPS
    KeyScan_DLPS_Enter();
#endif

#if USE_QDECODER_DLPS
    QuadDecoder_DLPS_Enter();
#endif

#if USE_SPI0_DLPS
    SPI0_DLPS_Enter();
#endif

#if USE_SPI1_DLPS
    SPI1_DLPS_Enter();
#endif

#if USE_SPI2W_DLPS
    SPI2W_DLPS_Enter();
#endif

#if USE_TIM_DLPS
    TIM_DLPS_Enter();
#endif

#if USE_UART0_DLPS
    UART0_DLPS_Enter();
#endif

#if USE_UART1_DLPS
    UART1_DLPS_Enter();
#endif

#if USE_LOG_UART2_DLPS
    LOG_UART2_DLPS_Enter();
#endif

    Log_SWD_DLPS_Enter();
}

/**
  * @brief  IO exit dlps callback function.
  * @param  None
  * @retval None
  */
void DLPS_IO_ExitDlpsCb(void) DATA_RAM_FUNCTION;
void DLPS_IO_ExitDlpsCb(void)
{

//    DBG_BUFFER(TYPE_BUMBLEBEE3, SUBTYPE_FORMAT, MODULE_DLPS, LEVEL_INFO,
//               "DLPS_IO_ExitDlpsCb",0);

    Pinmux_DLPS_Exit();

    Log_SWD_DLPS_Exit();

#if USE_ADC_DLPS
    ADC_DLPS_Exit();
#endif

#if USE_CODEC_DLPS
    CODEC_DLPS_Exit();
#endif

#if USE_CODEC_CH0_EQ1_DLPS
    CODEC_CH0_EQ1_DLPS_Exit();
#endif

#if USE_CODEC_CH0_EQ2_DLPS
    CODEC_CH0_EQ2_DLPS_Exit();
#endif

#if USE_CODEC_CH0_EQ3_DLPS
    CODEC_CH0_EQ3_DLPS_Exit();
#endif

#if USE_CODEC_CH0_EQ4_DLPS
    CODEC_CH0_EQ4_DLPS_Exit();
#endif

#if USE_CODEC_CH0_EQ5_DLPS
    CODEC_CH0_EQ5_DLPS_Exit();
#endif

#if USE_GPIO_DLPS
    GPIO_DLPS_Exit();
#endif

#if USE_I2C0_DLPS
    I2C0_DLPS_Exit();
#endif

#if USE_I2C1_DLPS
    I2C1_DLPS_Exit();
#endif

#if USE_I2S0_DLPS
    I2S0_DLPS_Exit();
#endif

#if USE_I2S1_DLPS
    I2S1_DLPS_Exit();
#endif

#if USE_IF8080_DLPS
    IF8080_DLPS_Exit();
#endif

#if USE_IR_DLPS
    IR_DLPS_Exit();
#endif

#if USE_KEYSCAN_DLPS
    KeyScan_DLPS_Exit();
#endif

#if USE_QDECODER_DLPS
    QuadDecoder_DLPS_Exit();
#endif

#if USE_SPI0_DLPS
    SPI0_DLPS_Exit();
#endif

#if USE_SPI1_DLPS
    SPI1_DLPS_Exit();
#endif

#if USE_SPI2W_DLPS
    SPI2W_DLPS_Exit();
#endif

#if USE_TIM_DLPS
    TIM_DLPS_Exit();
#endif

#if USE_UART0_DLPS
    UART0_DLPS_Exit();
#endif

#if USE_UART1_DLPS
    UART1_DLPS_Exit();
#endif

#if USE_LOG_UART2_DLPS
    LOG_UART2_DLPS_Exit();
#endif

#if USE_USER_DEFINE_DLPS_EXIT_CB
    if (User_IO_ExitDlpsCB)
    {
        User_IO_ExitDlpsCB();
    }
#endif

    NVIC_InitTypeDef nvic_init_struct;
    nvic_init_struct.NVIC_IRQChannel         = System_IRQn;
    nvic_init_struct.NVIC_IRQChannelCmd      = (FunctionalState)ENABLE;
    nvic_init_struct.NVIC_IRQChannelPriority = 3;
    NVIC_Init(&nvic_init_struct); //Enable SYSTEM_ON Interrupt

    CPU_DLPS_Exit();
}

/**
  * @brief  register IO DLPS callback function
  * @param  None
  * @retval None
  */
void DLPS_IORegister(void)
{
    dlps_hw_control_cb_reg(DLPS_IO_EnterDlpsCb, DLPS_ENTER);
    dlps_hw_control_cb_reg(DLPS_IO_ExitDlpsCb, DLPS_EXIT4_BT_READY);

    return;
}

#endif /* USE_IO_DRIVER_DLPS */

/** @} */ /* End of group Platform_DLPS_Functions */

/******************* (C) COPYRIGHT 2020 Realtek Semiconductor Corporation *****END OF FILE****/

