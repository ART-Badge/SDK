/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file     rtl876x_rcc.h
* \brief    The header file of the clock control and reset driver.
* \details  This file provides all peripheral clock control firmware functions.
* \author   tifnan_ge
* \date     2015-05-16
* \version  v0.1
* *********************************************************************************************************
*/


#ifndef _RTL876X_RCC_H_
#define _RTL876X_RCC_H_

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \addtogroup  IO          Peripheral Drivers
 * \defgroup    RCC         RCC
 *
 * \brief       Manage the RCC peripheral functions.
 *
 * \ingroup     IO
 */


/*============================================================================*
 *                         Includes
 *============================================================================*/
#include "rtl876x.h"


/*============================================================================*
 *                         Constants
 *============================================================================*/

/**
 * \defgroup    RCC_Exported_Constants Macro Definitions
 *
 * \ingroup     RCC
 */

/**
 * \defgroup    RCC_Peripheral_Clock RCC Peripheral Clock
 * \{
 * \ingroup     RCC_Exported_Constants
 */

#define APBPeriph_I2S0_CLOCK                ((uint32_t)((1 << 5) | (1 << 8) | (0x00 << 29)))
#define APBPeriph_I2S1_CLOCK                ((uint32_t)((1 << 6) | (1 << 8) | (0x00 << 29)))
#define APBPeriph_CODEC_CLOCK               ((uint32_t)((1 << 4) | (0x00 << 29)))

#define APBPeriph_GPIO_CLOCK                ((uint32_t)((0x01 << 24) | (0x01 << 29)))
#define APBPeriph_GDMA_CLOCK                ((uint32_t)((0x01 << 16) | (0x01 << 29)))
#define APBPeriph_TIMER_CLOCK               ((uint32_t)((0x01 << 14) | (0x01 << 29)))
#define APBPeriph_UART2_CLOCK               ((uint32_t)((0x01 << 10) | (0x01 << 29)))
#define APBPeriph_UART1_CLOCK               ((uint32_t)((0x01 << 12) | (0x01 << 29)))
#define APBPeriph_FLASH_CLOCK               ((uint32_t)((0x01 << 8)  | (0x01 << 29)))

#define APBPeriph_FLASH2_CLOCK              ((uint32_t)((0x01 << 26) | (0x02 << 29)))
#define APBPeriph_FLASH1_CLOCK              ((uint32_t)((0x01 << 24) | (0x02 << 29)))
#define APBPeriph_IR_CLOCK                  ((uint32_t)((0x01 << 20) | (0x02 << 29)))
#define APBPeriph_SPI1_CLOCK                ((uint32_t)((0x01 << 18) | (0x02 << 29)))
#define APBPeriph_SPI0_CLOCK                ((uint32_t)((0x01 << 16) | (0x02 << 29)))
#define APBPeriph_UART0_CLOCK               ((uint32_t)((0x01 << 0)  | (0x02 << 29)))

#define APBPeriph_IF8080_CLOCK              ((uint32_t)((0x01 << 28) | (0x03 << 29)))
#define APBPeriph_ADC_CLOCK                 ((uint32_t)((0x01 << 24) | (0x03 << 29)))
#define APBPeriph_SPI2W_CLOCK               ((uint32_t)((0x01 << 16) | (0x03 << 29)))
#define APBPeriph_KEYSCAN_CLOCK             ((uint32_t)((0x01 << 6) | (0x03 << 29)))
#define APBPeriph_QDEC_CLOCK                ((uint32_t)((0x01 << 4) | (0x03 << 29)))
#define APBPeriph_I2C1_CLOCK                ((uint32_t)((0x01 << 2) | (0x03 << 29)))
#define APBPeriph_I2C0_CLOCK                ((uint32_t)((0x01 << 0) | (0x03 << 29)))
/** \} */

#define IS_APB_PERIPH_CLOCK(CLOCK) (((CLOCK) == APBPeriph_GPIO_CLOCK) || ((CLOCK) == APBPeriph_GDMA_CLOCK)\
                                    || ((CLOCK) == APBPeriph_TIMER_CLOCK) || ((CLOCK) == APBPeriph_IR_CLOCK)\
                                    || ((CLOCK) == APBPeriph_SPI1_CLOCK) || ((CLOCK) == APBPeriph_SPI0_CLOCK)\
                                    || ((CLOCK) == APBPeriph_UART0_CLOCK) || ((CLOCK) == APBPeriph_ADC_CLOCK)\
                                    || ((CLOCK) == APBPeriph_SPI2W_CLOCK) || ((CLOCK) == APBPeriph_KEYSCAN_CLOCK)\
                                    || ((CLOCK) == APBPeriph_QDEC_CLOCK) || ((CLOCK) == APBPeriph_I2C1_CLOCK)\
                                    || ((CLOCK) == APBPeriph_I2C0_CLOCK) || ((CLOCK) == APBPeriph_CODEC_CLOCK)\
                                    || ((CLOCK) == APBPeriph_UART1_CLOCK) || ((CLOCK) == APBPeriph_UART2_CLOCK)\
                                    || ((CLOCK) == APBPeriph_I2S0_CLOCK) || ((CLOCK) == APBPeriph_I2S1_CLOCK)\
                                    || ((CLOCK) == APBPeriph_IF8080_CLOCK))

/**
 * \defgroup    APB_Peripheral_Define APB Peripheral Define
 * \{
 * \ingroup     RCC_Exported_Constants
 */

#define APBPeriph_TIMER                     ((uint32_t)((1 << 16) | (0x00 << 26)))
#define APBPeriph_GDMA                      ((uint32_t)((1 << 13) | (0x00 << 26)))
#define APBPeriph_UART1                     ((uint32_t)((1 << 12) | (0x00 << 26)))
#define APBPeriph_FLASH2                    ((uint32_t)((1 << 5)  | (0x00 << 26)))
#define APBPeriph_FLASH                     ((uint32_t)((1 << 4)  | (0x00 << 26)))
#define APBPeriph_FLASH1                    ((uint32_t)((1 << 3)  | (0x00 << 26)))

#define APBPeriph_IF8080                    ((uint32_t)((1 << 25) | (0x02 << 26)))
#define APBPeriph_SPI2W                     ((uint32_t)((1 << 24) | (0x02 << 26)))
#define APBPeriph_KEYSCAN                   ((uint32_t)((1 << 19) | (0x02 << 26)))
#define APBPeriph_QDEC                      ((uint32_t)((1 << 18) | (0x02 << 26)))
#define APBPeriph_I2C1                      ((uint32_t)((1 << 17) | (0x02 << 26)))
#define APBPeriph_I2C0                      ((uint32_t)((1 << 16) | (0x02 << 26)))
#define APBPeriph_IR                        ((uint32_t)((1 << 10) | (0x02 << 26)))
#define APBPeriph_SPI1                      ((uint32_t)((1 << 9) | (0x02 << 26)))
#define APBPeriph_SPI0                      ((uint32_t)((1 << 8) | (0x02 << 26)))
#define APBPeriph_UART0                     ((uint32_t)((1 << 0) | (0x02 << 26)))
#define APBPeriph_UART2                     ((uint32_t)((1 << 1) | (0x02 << 26)))

#define APBPeriph_GPIO                      ((uint32_t)((1 << 8) | (0x03 << 26)))
#define APBPeriph_ADC                       ((uint32_t)((1 << 0) | (0x03 << 26)))

#define APBPeriph_I2S0                      ((uint32_t)((1 << 1) | (0x00 << 26)))
#define APBPeriph_I2S1                      ((uint32_t)((1 << 2) | (0x00 << 26)))
#define APBPeriph_CODEC                     ((uint32_t)((1 << 0) | (0x00 << 26)))
/** \} */

#define IS_APB_PERIPH(PERIPH) (((PERIPH) == APBPeriph_TIMER) || ((PERIPH) == APBPeriph_GDMA)\
                               || ((PERIPH) == APBPeriph_SPI2W) || ((PERIPH) == APBPeriph_KEYSCAN)\
                               || ((PERIPH) == APBPeriph_QDEC) || ((PERIPH) == APBPeriph_I2C1)\
                               || ((PERIPH) == APBPeriph_I2C0) || ((PERIPH) == APBPeriph_IR)\
                               || ((PERIPH) == APBPeriph_SPI1) || ((PERIPH) == APBPeriph_SPI0)\
                               || ((PERIPH) == APBPeriph_UART0) || ((PERIPH) == APBPeriph_GPIO)\
                               || ((PERIPH) == APBPeriph_ADC) || ((PERIPH) == APBPeriph_CODEC)\
                               || (PERIPH == APBPeriph_UART1) || (PERIPH == APBPeriph_UART2)\
                               || ((PERIPH) == APBPeriph_I2S0) || ((PERIPH) == APBPeriph_I2S1)\
                               || ((PERIPH) == APBPeriph_IF8080))

/**
 * \defgroup    RCC_Peripheral_Clock RCC Peripheral Clock
 * \{
 * \ingroup     RCC_Exported_Constants
 */

#define CLOCK_GATE_5M                       ((uint32_t)(0x01 << 29))/* 5M clock source for adc and keyscan */
#define CLOCK_GATE_20M                      ((uint32_t)(0x01 << 27))/* 20M clock source for 2wssi and qdec */
#define CLOCK_GATE_10M                      ((uint32_t)(0x01 << 28))/* 10M clock source for bluewiz */
/** \} */
#define IS_CLOCK_GATE(CLOCK) (((CLOCK) == CLOCK_GATE_5M) || ((CLOCK) == CLOCK_GATE_20M)\
                              || ((CLOCK) == CLOCK_GATE_10M))

/**
 * \defgroup    I2C_Clock_Divider I2C Clock Divider
 * \{
 * \ingroup     RCC_Exported_Constants
 */

#define I2C_CLOCK_DIV_1                     ((uint16_t)0x0)
#define I2C_CLOCK_DIV_2                     ((uint16_t)0x1)
#define I2C_CLOCK_DIV_4                     ((uint16_t)0x2)
#define I2C_CLOCK_DIV_8                     ((uint16_t)0x3)
/** \} */
#define IS_I2C_DIV(DIV)              (((DIV) == I2C_CLOCK_DIV_1) || \
                                      ((DIV) == I2C_CLOCK_DIV_2) || \
                                      ((DIV) == I2C_CLOCK_DIV_4) || \
                                      ((DIV) == I2C_CLOCK_DIV_8))
/**
 * \defgroup    SPI_Clock_Divider SPI Clock Divider
 * \{
 * \ingroup     RCC_Exported_Constants
 */

#define SPI_CLOCK_DIV_1                     ((uint16_t)0x0)
#define SPI_CLOCK_DIV_2                     ((uint16_t)0x1)
#define SPI_CLOCK_DIV_4                     ((uint16_t)0x2)
#define SPI_CLOCK_DIV_8                     ((uint16_t)0x3)
/** \} */
#define IS_SPI_DIV(DIV)              (((DIV) == SPI_CLOCK_DIV_1) || \
                                      ((DIV) == SPI_CLOCK_DIV_2) || \
                                      ((DIV) == SPI_CLOCK_DIV_4) || \
                                      ((DIV) == SPI_CLOCK_DIV_8))

/**
 * \defgroup    UART_Clock_Divider UART Clock Divider
 * \{
 * \ingroup     RCC_Exported_Constants
 */

#define UART_CLOCK_DIV_1                    ((uint16_t)0x0)
#define UART_CLOCK_DIV_2                    ((uint16_t)0x1)
#define UART_CLOCK_DIV_4                    ((uint16_t)0x2)
#define UART_CLOCK_DIV_16                   ((uint16_t)0x3)
/** \} */
#define IS_UART_DIV(DIV)              (((DIV) == UART_CLOCK_DIV_1) || \
                                       ((DIV) == UART_CLOCK_DIV_2) || \
                                       ((DIV) == UART_CLOCK_DIV_4) || \
                                       ((DIV) == UART_CLOCK_DIV_16))


/*============================================================================*
 *                         Functions
 *============================================================================*/

/**
 * \defgroup    RCC_Exported_Functions Peripheral APIs
 * \ingroup     RCC
 * \{
 */

/**
 * \brief  Enables or disables the APB peripheral clock.
 * \param  APBPeriph: Specifies the APB peripheral to gates its clock.
 *     This parameter can be one of the following values:
 *     \arg APBPeriph_TIMER
 *     \arg APBPeriph_GDMA
 *     \arg APBPeriph_IF8080
 *     \arg APBPeriph_SPI2W
 *     \arg APBPeriph_KEYSCAN
 *     \arg APBPeriph_QDEC
 *     \arg APBPeriph_I2C1
 *     \arg APBPeriph_I2C0
 *     \arg APBPeriph_IR
 *     \arg APBPeriph_SPI1
 *     \arg APBPeriph_SPI0
 *     \arg APBPeriph_UART0
 *     \arg APBPeriph_UART1
 *     \arg APBPeriph_UART2
 *     \arg APBPeriph_GPIO
 *     \arg APBPeriph_ADC
 *     \arg APBPeriph_I2S0
 *     \arg APBPeriph_I2S1
 *     \arg APBPeriph_CODEC
 * \param  APBPeriph_Clock: Specifies the APB peripheral clock config.
 *     This parameter can be one of the following values(must be the same with APBPeriph):
 *     \arg APBPeriph_TIMER_CLOCK
 *     \arg APBPeriph_GDMA_CLOCK
 *     \arg APBPeriph_IF8080_CLOCK
 *     \arg APBPeriph_SPI2W_CLOCK
 *     \arg APBPeriph_KEYSCAN_CLOCK
 *     \arg APBPeriph_QDEC_CLOCK
 *     \arg APBPeriph_I2C1_CLOCK
 *     \arg APBPeriph_I2C0_CLOCK
 *     \arg APBPeriph_IR_CLOCK
 *     \arg APBPeriph_SPI1_CLOCK
 *     \arg APBPeriph_SPI0_CLOCK
 *     \arg APBPeriph_UART0_CLOCK
 *     \arg APBPeriph_UART1_CLOCK
 *     \arg APBPeriph_UART2_CLOCK
 *     \arg APBPeriph_GPIO_CLOCK
 *     \arg APBPeriph_ADC_CLOCK
 *     \arg APBPeriph_I2S0_CLOCK
 *     \arg APBPeriph_I2S1_CLOCK
 *     \arg APBPeriph_CODEC_CLOCK
 * \param  NewState: New state of the specified peripheral clock.
 *     This parameter can be: ENABLE or DISABLE.
 * \retval None.
 */
void RCC_PeriphClockCmd(uint32_t APBPeriph, uint32_t APBPeriph_Clock,
                        FunctionalState NewState);
/**
 * \brief  Enables or disables the APB peripheral clock.
 * \param  APBPeriph_Clock: Specifies the APB peripheral clock config.
 *     This parameter can be one of the following values(must be the same with APBPeriph):
 *     \arg APBPeriph_TIMER_CLOCK
 *     \arg APBPeriph_GDMA_CLOCK
 *     \arg APBPeriph_SPI2W_CLOCK
 *     \arg APBPeriph_KEYSCAN_CLOCK
 *     \arg APBPeriph_QDEC_CLOCK
 *     \arg APBPeriph_I2C1_CLOCK
 *     \arg APBPeriph_I2C0_CLOCK
 *     \arg APBPeriph_IR_CLOCK
 *     \arg APBPeriph_SPI1_CLOCK
 *     \arg APBPeriph_SPI0_CLOCK
 *     \arg APBPeriph_UART0_CLOCK
 *     \arg APBPeriph_UART1_CLOCK
 *     \arg APBPeriph_UART2_CLOCK
 *     \arg APBPeriph_GPIO_CLOCK
 *     \arg APBPeriph_ADC_CLOCK
 *     \arg APBPeriph_I2S0_CLOCK
 *     \arg APBPeriph_I2S1_CLOCK
 *     \arg APBPeriph_CODEC_CLOCK
 * \param  NewState: New state of the specified peripheral clock.
 *     This parameter can be: ENABLE or DISABLE.
 * \retval None.
 */
void RCC_PeriClockConfig(uint32_t APBPeriph_Clock, FunctionalState NewState);

/**
 * \brief  Enables or disables the APB peripheral clock.
 * \param  APBPeriph: Specifies the APB peripheral to gates its clock.
 *     This parameter can be one of the following values:
 *     \arg APBPeriph_TIMER
 *     \arg APBPeriph_GDMA
 *     \arg APBPeriph_IF8080
 *     \arg APBPeriph_SPI2W
 *     \arg APBPeriph_KEYSCAN
 *     \arg APBPeriph_QDEC
 *     \arg APBPeriph_I2C1
 *     \arg APBPeriph_I2C0
 *     \arg APBPeriph_IR
 *     \arg APBPeriph_SPI1
 *     \arg APBPeriph_SPI0
 *     \arg APBPeriph_UART0
 *     \arg APBPeriph_UART1
 *     \arg APBPeriph_UART2
 *     \arg APBPeriph_GPIO
 *     \arg APBPeriph_ADC
 *     \arg APBPeriph_I2S0
 *     \arg APBPeriph_I2S1
 *     \arg APBPeriph_CODEC
 * \param  NewState: New state of the specified peripheral clock.
 *     This parameter can be: ENABLE or DISABLE.
 * \retval None.
 */
void RCC_PeriFunctionConfig(uint32_t APBPeriph, FunctionalState NewState);

/**
 * \brief  I2C clock divider config.
 * \param  I2Cx: Where x can be 0 or 1 to select the I2C peripheral.
 * \param  ClockDiv: Specifies the APB peripheral to gates its clock.
 *     This parameter can be one of the following values:
 *     \arg I2C_CLOCK_DIV_1
 *     \arg I2C_CLOCK_DIV_2
 *     \arg I2C_CLOCK_DIV_4
 *     \arg I2C_CLOCK_DIV_8
 * \retval None.
 */
void RCC_I2CClkDivConfig(I2C_TypeDef *I2Cx, uint16_t ClockDiv);

/**
 * \brief  SPI clock divider config.
 * \param  SPIx: Where x can be 0 or 1 to select the SPI peripheral.
 * \param  ClockDiv: Specifies the APB peripheral to gates its clock.
 *     This parameter can be one of the following values:
 *     \arg SPI_CLOCK_DIV_1
 *     \arg SPI_CLOCK_DIV_2
 *     \arg SPI_CLOCK_DIV_4
 *     \arg SPI_CLOCK_DIV_8
 * \retval None.
 */
void RCC_SPIClkDivConfig(SPI_TypeDef *SPIx, uint16_t ClockDiv);

/**
 * \brief  UART clock divider config.
 * \param  UARTx: Selected UART peripheral.
 * \param  ClockDiv: Specifies the APB peripheral to gates its clock.
 *     This parameter can be one of the following values:
 *     \arg UART_CLOCK_DIV_1
 *     \arg UART_CLOCK_DIV_2
 *     \arg UART_CLOCK_DIV_4
 *     \arg UART_CLOCK_DIV_16
 * \retval None.
 */
void RCC_UARTClkDivConfig(UART_TypeDef *UARTx, uint16_t ClockDiv);

/**
 * \brief  IR clock config.
 * \param  NewState: New state of the specified peripheral clock.
 *      This parameter can be one of the following values:
 *      \arg DISABLE: Clock source is Fixed 40M clock.
 *      \arg ENABLE: Clock source is PLL clock.
 * \retval None.
 */
void RCC_IRClockConfig(FunctionalState NewState);

/**
 * \brief  SPIx peripheral clock config.
 * \param  SPIx: SPI0.
 * \param  NewState: New state of the specified peripheral clock.
 *      This parameter can be one of the following values:
 *      \arg DISABLE: Clock source is Fixed 40M clock.
 *      \arg ENABLE: Clock source is PLL clock.
 * \retval None.
  */
void RCC_SPIClockConfig(SPI_TypeDef *SPIx, FunctionalState NewState);

/**
 * \brief  Config timer2~timer7 clock source to fixed 40Mhz clock or PLL.
 * \param  TIMx: Where x can be 2 to 7 to select the TIM peripheral.
 * \param  NewState: New state of the specified peripheral clock.
 *      This parameter can be one of the following values:
 *      \arg DISABLE: Clock source is Fixed 40M clock.
 *      \arg ENABLE: Clock source is PLL clock.
 * \retval None.
 */
void RCC_TimerClockConfig(TIM_TypeDef *TIMx, FunctionalState NewState);

/**
 * \brief   Enable clock 5M source.
 * \param   None.
 * \return  None.
 */
__STATIC_INLINE void RCC_ClockSrc5MCmd(void)
{
    /*Open 5M clock source*/
    SYSBLKCTRL->u_20C.RSVD_20C |= BIT26;
    SYSBLKCTRL->u_20C.RSVD_20C |= BIT29;
}

/**
 * \brief   Enable clock 10M source.
 * \param   None.
 * \return  None.
 */
__STATIC_INLINE void RCC_ClockSrc10MCmd(void)
{
    /*Open 10M clock source*/
    SYSBLKCTRL->u_20C.RSVD_20C |= BIT26;
    SYSBLKCTRL->u_20C.RSVD_20C |= BIT28;
}

/**
 * \brief   Enable clock 20M source.
 * \param   None.
 * \return  None.
 */
__STATIC_INLINE void RCC_ClockSrc20MCmd(void)
{
    /*Open 20M clock source*/
    SYSBLKCTRL->u_20C.RSVD_20C |= BIT26;
    SYSBLKCTRL->u_20C.RSVD_20C |= BIT27;
}

/** \} */ /* End of group RCC_Exported_Functions */

#ifdef __cplusplus
}
#endif

#endif /* _RTL876X_RCC_H_ */


/******************* (C) COPYRIGHT 2015 Realtek Semiconductor *****END OF FILE****/



