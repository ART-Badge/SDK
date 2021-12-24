/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_rcc.c
* @brief    This file provides all the IO clock firmware functions..
* @details
* @author   tifnan_ge
* @date     2015-05-16
* @version  v1.0
*********************************************************************************************************
*/
#include "rtl876x.h"
#include "rtl876x_bitfields.h"
#include "rtl876x_rcc.h"

/**
  * @brief  Enables or disables the APB peripheral clock.
  * @param  APBPeriph: specifies the APB peripheral to gates its clock.
  *   This parameter can be one of the following values:
  *     @arg APBPeriph_TIMER
  *     @arg APBPeriph_GDMA
  *     @arg APBPeriph_IF8080
  *     @arg APBPeriph_SPI2W
  *     @arg APBPeriph_KEYSCAN
  *     @arg APBPeriph_QDEC
  *     @arg APBPeriph_I2C1
  *     @arg APBPeriph_I2C0
  *     @arg APBPeriph_IR
  *     @arg APBPeriph_SPI1
  *     @arg APBPeriph_SPI0
  *     @arg APBPeriph_UART0
  *     @arg APBPeriph_UART1
  *     @arg APBPeriph_UART2
  *     @arg APBPeriph_GPIO
  *     @arg APBPeriph_ADC
  *     @arg APBPeriph_I2S0
  *     @arg APBPeriph_I2S1
  *     @arg APBPeriph_CODEC
  * @param  APBPeriph_Clock: specifies the APB peripheral clock config.
  *   This parameter can be one of the following values(must be the same with APBPeriph):
  *     @arg APBPeriph_TIMER_CLOCK
  *     @arg APBPeriph_GDMA_CLOCK
  *     @arg APBPeriph_IF8080_CLOCK
  *     @arg APBPeriph_SPI2W_CLOCK
  *     @arg APBPeriph_KEYSCAN_CLOCK
  *     @arg APBPeriph_QDEC_CLOCK
  *     @arg APBPeriph_I2C1_CLOCK
  *     @arg APBPeriph_I2C0_CLOCK
  *     @arg APBPeriph_IR_CLOCK
  *     @arg APBPeriph_SPI1_CLOCK
  *     @arg APBPeriph_SPI0_CLOCK
  *     @arg APBPeriph_UART0_CLOCK
  *     @arg APBPeriph_UART1_CLOCK
  *     @arg APBPeriph_UART2_CLOCK
  *     @arg APBPeriph_GPIO_CLOCK
  *     @arg APBPeriph_ADC_CLOCK
  *     @arg APBPeriph_I2S0_CLOCK
  *     @arg APBPeriph_I2S1_CLOCK
  *     @arg APBPeriph_CODEC_CLOCK
  * @param  NewState: new state of the specified peripheral clock.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_PeriphClockCmd(uint32_t APBPeriph, uint32_t APBPeriph_Clock, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_APB_PERIPH(APBPeriph));
    assert_param(IS_APB_PERIPH_CLOCK(APBPeriph_Clock));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    uint32_t apbRegOff = ((APBPeriph & (0x03 << 26)) >> 26);
    uint32_t clkRegOff = ((APBPeriph_Clock & (0x03 << 29)) >> 29);

    /*Open clock gating first*/
    if (NewState == ENABLE)
    {
        if (APBPeriph_Clock == APBPeriph_KEYSCAN_CLOCK)
        {
            /*Open 5M clock source*/
            SYSBLKCTRL->u_20C.RSVD_20C |= BIT26;
            SYSBLKCTRL->u_20C.RSVD_20C |= BIT29;
        }
        else if (APBPeriph_Clock == APBPeriph_ADC_CLOCK)
        {
            /*Open 10M clock source*/
            SYSBLKCTRL->u_20C.RSVD_20C |= BIT26;
            SYSBLKCTRL->u_20C.RSVD_20C |= BIT28;
        }
        else if ((APBPeriph_Clock == APBPeriph_QDEC_CLOCK) || (APBPeriph_Clock == APBPeriph_SPI2W_CLOCK))
        {
            /*Open 20M clock source*/
            SYSBLKCTRL->u_20C.RSVD_20C |= BIT26;
            SYSBLKCTRL->u_20C.RSVD_20C |= BIT27;
        }
        else if (APBPeriph_Clock == APBPeriph_TIMER_CLOCK)
        {
            /* Enable TIM0~1 fixed 40M */
            CLK_SOURCE_REG_2 |= BIT9;
            /* Enable TIM2~7 ICG*/
            CLK_SOURCE_REG_0 |= BIT4;
        }
    }

    /* Special register handle */
    if (NewState == ENABLE)
    {
        if ((APBPeriph_Clock == APBPeriph_I2S0_CLOCK) || ((APBPeriph_Clock == APBPeriph_I2S1_CLOCK)) ||
            (APBPeriph_Clock == APBPeriph_CODEC_CLOCK))
        {
            SYSBLKCTRL->u_220 |= APBPeriph | APBPeriph_Clock;
            return;
        }
    }
    else
    {
        if ((APBPeriph_Clock == APBPeriph_I2S0_CLOCK) || ((APBPeriph_Clock == APBPeriph_I2S1_CLOCK)) ||
            (APBPeriph_Clock == APBPeriph_CODEC_CLOCK))
        {
            SYSBLKCTRL->u_220 &= ~(APBPeriph | APBPeriph_Clock);
            return;
        }
    }

    /* clear flag */
    APBPeriph &= (~(0x03 << 26));
    APBPeriph_Clock &= (~(0x03 << 29));

    if (NewState == ENABLE)
    {
        //enable peripheral
        *((uint32_t *)(&(SYSBLKCTRL->u_210.SOC_FUNC_EN)) + apbRegOff) |= APBPeriph;
        //enable peripheral clock
        *((uint32_t *)(&(SYSBLKCTRL->u_230.CLK_CTRL)) + clkRegOff - 1) |= APBPeriph_Clock;
        //enable peripheral clock in sleep mode
        *((uint32_t *)(&(SYSBLKCTRL->u_230.CLK_CTRL)) + clkRegOff - 1) |= (APBPeriph_Clock << 1);
    }
    else
    {
        //disable peripheral
        *((uint32_t *)(&(SYSBLKCTRL->u_210.SOC_FUNC_EN)) + apbRegOff) &= (~APBPeriph);
        //disable peripheral clock
        *((uint32_t *)(&(SYSBLKCTRL->u_230.CLK_CTRL)) + clkRegOff - 1) &= (~APBPeriph_Clock);
        //disable peripheral clock in sleep mode
        *((uint32_t *)(&(SYSBLKCTRL->u_230.CLK_CTRL)) + clkRegOff - 1) &= (~(APBPeriph_Clock << 1));
    }

    return;
}

/**
  * @brief  I2C clock divider config.
  * @param  I2Cx: where x can be 0 or 1 to select the I2C peripheral.
  * @param  ClockDiv: specifies the APB peripheral to gates its clock.
  *   This parameter can be one of the following values:
  *     @arg I2C_CLOCK_DIV_1
  *     @arg I2C_CLOCK_DIV_2
  *     @arg I2C_CLOCK_DIV_4
  *     @arg I2C_CLOCK_DIV_8
  * @retval None
  */
void RCC_I2CClkDivConfig(I2C_TypeDef *I2Cx, uint16_t ClockDiv)
{
    assert_param(IS_I2C_DIV(ClockDiv));

    /* Config I2C clock divider */
    if (I2Cx == I2C0)
    {
        /* disable clock first */
        SYSBLKCTRL->u_238.PERI_CLK_CTRL1 &= ~SYSBLK_ACTCK_I2C0_EN_Msk;
        //platform_delay_us(1);

        CLK_SOURCE_REG_1 &= ~(0x03 << 15);
        CLK_SOURCE_REG_1 |= (ClockDiv << 15);

        //platform_delay_us(1);
        SYSBLKCTRL->u_238.PERI_CLK_CTRL1 |= SYSBLK_ACTCK_I2C0_EN_Msk;
    }
    else if (I2Cx == I2C1)
    {
        SYSBLKCTRL->u_238.PERI_CLK_CTRL1 &= ~SYSBLK_ACTCK_I2C1_EN_Msk;
        //platform_delay_us(1);

        CLK_SOURCE_REG_1 &= ~(0x03 << 17);
        CLK_SOURCE_REG_1 |= (ClockDiv << 17);

        //platform_delay_us(1);
        SYSBLKCTRL->u_238.PERI_CLK_CTRL1 |= SYSBLK_ACTCK_I2C1_EN_Msk;
    }

    return;
}

/**
  * @brief  SPI clock divider config.
  * @param  SPIx: where x can be 0 or 1 to select the SPI peripheral.
  * @param  ClockDiv: specifies the APB peripheral to gates its clock.
  *   This parameter can be one of the following values:
  *     @arg SPI_CLOCK_DIV_1
  *     @arg SPI_CLOCK_DIV_2
  *     @arg SPI_CLOCK_DIV_4
  *     @arg SPI_CLOCK_DIV_8
  * @retval None
  */
void RCC_SPIClkDivConfig(SPI_TypeDef *SPIx, uint16_t ClockDiv)
{
    assert_param(IS_SPI_DIV(ClockDiv));

    /* Config I2C clock divider */
    if (SPIx == SPI0)
    {
        /* disable clock first */
        SYSBLKCTRL->u_234.PERI_CLK_CTRL0 &= ~SYSBLK_ACTCK_SPI0_EN_Msk;
        //platform_delay_us(1);

        CLK_SOURCE_REG_1 &= ~(0x03 << 19);
        CLK_SOURCE_REG_1 |= (ClockDiv << 19);

        //platform_delay_us(1);
        SYSBLKCTRL->u_234.PERI_CLK_CTRL0 |= SYSBLK_ACTCK_SPI0_EN_Msk;
    }
    else if (SPIx == SPI1)
    {
        SYSBLKCTRL->u_234.PERI_CLK_CTRL0 &= ~SYSBLK_ACTCK_SPI1_EN_Msk;
        //platform_delay_us(1);

        CLK_SOURCE_REG_1 &= ~(0x03 << 21);
        CLK_SOURCE_REG_1 |= (ClockDiv << 21);

        //platform_delay_us(1);
        SYSBLKCTRL->u_234.PERI_CLK_CTRL0 |= SYSBLK_ACTCK_SPI1_EN_Msk;
    }
    return;
}

/**
  * @brief  UART clock divider config.
  * @param  UARTx: selected UART peripheral.
  * @param  ClockDiv: specifies the APB peripheral to gates its clock.
  *   This parameter can be one of the following values:
  *     @arg UART_CLOCK_DIV_1
  *     @arg UART_CLOCK_DIV_2
  *     @arg UART_CLOCK_DIV_4
  *     @arg UART_CLOCK_DIV_16
  * @retval None
  */
void RCC_UARTClkDivConfig(UART_TypeDef *UARTx, uint16_t ClockDiv)
{
    assert_param(IS_UART_DIV(ClockDiv));

    /* Config UART clock divider */
    if (UARTx == UART0)
    {
        /* disable clock first */
        SYSBLKCTRL->u_234.PERI_CLK_CTRL0 &= ~SYSBLK_ACTCK_UART0DATA_EN_Msk;
        //platform_delay_us(1);

        CLK_SOURCE_REG_1 &= ~(0x03 << 9);
        CLK_SOURCE_REG_1 |= (ClockDiv << 9);

        //platform_delay_us(1);
        SYSBLKCTRL->u_234.PERI_CLK_CTRL0 |= SYSBLK_ACTCK_UART0DATA_EN_Msk;
    }
    else if (UARTx == UART1)
    {
        SYSBLKCTRL->u_230.CLK_CTRL &= ~SYSBLK_ACTCK_LOGUART_EN_Msk;
        //platform_delay_us(1);

        CLK_SOURCE_REG_1 &= ~(0x03 << 11);
        CLK_SOURCE_REG_1 |= (ClockDiv << 11);

        //platform_delay_us(1);
        SYSBLKCTRL->u_230.CLK_CTRL |= SYSBLK_ACTCK_LOGUART_EN_Msk;
    }

    return;
}

/**
  * @brief  Enables or disables the APB peripheral clock.
  * @param  APBPeriph_Clock: specifies the APB peripheral clock config.
  *   This parameter can be one of the following values(must be the same with APBPeriph):
  *     @arg APBPeriph_TIMER_CLOCK
  *     @arg APBPeriph_GDMA_CLOCK
  *     @arg APBPeriph_SPI2W_CLOCK
  *     @arg APBPeriph_KEYSCAN_CLOCK
  *     @arg APBPeriph_QDEC_CLOCK
  *     @arg APBPeriph_I2C1_CLOCK
  *     @arg APBPeriph_I2C0_CLOCK
  *     @arg APBPeriph_IR_CLOCK
  *     @arg APBPeriph_SPI1_CLOCK
  *     @arg APBPeriph_SPI0_CLOCK
  *     @arg APBPeriph_UART0_CLOCK
  *     @arg APBPeriph_UART1_CLOCK
  *     @arg APBPeriph_UART2_CLOCK
  *     @arg APBPeriph_GPIO_CLOCK
  *     @arg APBPeriph_ADC_CLOCK
  *     @arg APBPeriph_I2S0_CLOCK
  *     @arg APBPeriph_I2S1_CLOCK
  *     @arg APBPeriph_CODEC_CLOCK
  * @param  NewState: new state of the specified peripheral clock.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_PeriClockConfig(uint32_t APBPeriph_Clock, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_APB_PERIPH_CLOCK(APBPeriph_Clock));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    uint32_t clkRegOff = ((APBPeriph_Clock & (0x03 << 29)) >> 29);

    /* Special register handle */
    if (NewState == ENABLE)
    {
        if ((APBPeriph_Clock == APBPeriph_I2S0_CLOCK) || ((APBPeriph_Clock == APBPeriph_I2S1_CLOCK)) ||
            (APBPeriph_Clock == APBPeriph_CODEC_CLOCK))
        {
            SYSBLKCTRL->u_220 |= APBPeriph_Clock;
            return;
        }
    }
    else
    {
        if ((APBPeriph_Clock == APBPeriph_I2S0_CLOCK) || ((APBPeriph_Clock == APBPeriph_I2S1_CLOCK)) ||
            (APBPeriph_Clock == APBPeriph_CODEC_CLOCK))
        {
            SYSBLKCTRL->u_220 &= ~(APBPeriph_Clock);
            return;
        }
    }

    APBPeriph_Clock &= (~(0x03 << 29));

    if (NewState == ENABLE)
    {
        //enable peripheral clock
        *((uint32_t *)(&(SYSBLKCTRL->u_230.CLK_CTRL)) + clkRegOff - 1) |= APBPeriph_Clock;
        //enable peripheral clock in sleep mode
        *((uint32_t *)(&(SYSBLKCTRL->u_230.CLK_CTRL)) + clkRegOff - 1) |= (APBPeriph_Clock << 1);
    }
    else
    {
        //disable peripheral clock
        *((uint32_t *)(&(SYSBLKCTRL->u_230.CLK_CTRL)) + clkRegOff - 1) &= (~APBPeriph_Clock);
        //disable peripheral clock in sleep mode
        *((uint32_t *)(&(SYSBLKCTRL->u_230.CLK_CTRL)) + clkRegOff - 1) &= (~(APBPeriph_Clock << 1));
    }

    return;
}

/**
  * @brief  Enables or disables the APB peripheral clock.
  * @param  APBPeriph: specifies the APB peripheral to gates its clock.
  *   This parameter can be one of the following values:
  *     @arg APBPeriph_TIMER
  *     @arg APBPeriph_GDMA
  *     @arg APBPeriph_IF8080
  *     @arg APBPeriph_SPI2W
  *     @arg APBPeriph_KEYSCAN
  *     @arg APBPeriph_QDEC
  *     @arg APBPeriph_I2C1
  *     @arg APBPeriph_I2C0
  *     @arg APBPeriph_IR
  *     @arg APBPeriph_SPI1
  *     @arg APBPeriph_SPI0
  *     @arg APBPeriph_UART0
  *     @arg APBPeriph_UART1
  *     @arg APBPeriph_UART2
  *     @arg APBPeriph_GPIO
  *     @arg APBPeriph_ADC
  *     @arg APBPeriph_I2S0
  *     @arg APBPeriph_I2S1
  *     @arg APBPeriph_CODEC
  * @param  NewState: new state of the specified peripheral clock.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_PeriFunctionConfig(uint32_t APBPeriph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_APB_PERIPH(APBPeriph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    uint32_t apbRegOff = ((APBPeriph & (0x03 << 26)) >> 26);

    /* Special register handle */
    if (NewState == ENABLE)
    {
        if ((APBPeriph == APBPeriph_I2S0) || ((APBPeriph == APBPeriph_I2S1)) ||
            (APBPeriph == APBPeriph_CODEC))
        {
            SYSBLKCTRL->u_220 |= APBPeriph;
            return;
        }
    }
    else
    {
        if ((APBPeriph == APBPeriph_I2S0) || ((APBPeriph == APBPeriph_I2S1)) ||
            (APBPeriph == APBPeriph_CODEC))
        {
            SYSBLKCTRL->u_220 &= ~(APBPeriph);
            return;
        }
    }

    /* clear flag */
    APBPeriph &= (~(0x03 << 26));

    if (NewState == ENABLE)
    {
        //enable peripheral
        *((uint32_t *)(&(SYSBLKCTRL->u_210.SOC_FUNC_EN)) + apbRegOff) |= APBPeriph;
    }
    else
    {
        //disable peripheral
        *((uint32_t *)(&(SYSBLKCTRL->u_210.SOC_FUNC_EN)) + apbRegOff) &= (~APBPeriph);
    }

    return;
}

/**
  * @brief  IR clock config.
  * @param  NewState: 0 : IR source clock from fixed 40M clock
                      1 : IR source clock from PLL clock
  * @retval None
  */
void RCC_IRClockConfig(FunctionalState NewState)
{
    if (NewState != ENABLE)
    {
        *((volatile uint32_t *)0x40000308) = *((volatile uint32_t *)0x40000308) & (~BIT(9));
    }
    else
    {
        *((volatile uint32_t *)0x40000230) = *((volatile uint32_t *)0x40000230) | BIT(31);
        *((volatile uint32_t *)0x40000308) = *((volatile uint32_t *)0x40000308) | BIT(9);
    }
}

/**
  * @brief  SPIx peripheral clock config
  * @param  SPIx: where x can be 0 to select the SPI peripheral.
  * @param  NewState: 0 : SPI source clock from fixed 40M clock
                      1 : SPI source clock from PLL clock
  * @retval None
  */
void RCC_SPIClockConfig(SPI_TypeDef *SPIx, FunctionalState NewState)
{
    if (SPIx == SPI0)
    {
        if (NewState != ENABLE)
        {
            *((volatile uint32_t *)0x40000308) = *((volatile uint32_t *)0x40000308) & (~BIT(7));
        }
        else
        {
            *((volatile uint32_t *)0x40000230) = *((volatile uint32_t *)0x40000230) | BIT(31);
            *((volatile uint32_t *)0x40000308) = *((volatile uint32_t *)0x40000308) | BIT(7);
        }
    }
}

/**
  * @brief  Config timer2~timer7 clock source to fixed 40Mhz clock or PLL.
  *@param   TIMx:where x can be 2 to 7 to select the TIM peripheral.
  * @param  NewState: 0 : source clock from fixed 40M clock
                      1 : source clock from PLL clock
  * @retval None
  */
void RCC_TimerClockConfig(TIM_TypeDef *TIMx, FunctionalState NewState)
{
    uint32_t timerid = 0;
    uint32_t tempreg = 0;
    tempreg = (uint32_t)TIMx;
    timerid = (tempreg - TIM0_REG_BASE) / 20;
    if (NewState != ENABLE)
    {
        *((volatile uint32_t *)0x4000035C) = *((volatile uint32_t *)0x4000035C) & (~BIT((22 + timerid)));
    }
    else
    {
        *((volatile uint32_t *)0x40000230) = *((volatile uint32_t *)0x40000230) | BIT(31);
        *((volatile uint32_t *)0x4000035C) = *((volatile uint32_t *)0x4000035C) | BIT((22 + timerid));
    }
}
