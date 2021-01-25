/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_i2c.c
* @brief    This file provides all the I2C firmware functions.
* @details
* @author   elliot chen
* @date     2015-04-29
* @version  v0.1
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtl876x_i2c_wristband.h"
#include "rtl876x_rcc.h"
#include "trace.h"
#include "platform_utils.h"
#include "rtl876x_pinmux.h"
#include "board.h"

#ifdef IIC_LEGACY
static uint32_t timeout_us = 0;
#endif

#define IIC_MAX_TIMEOUT_US      100
/**
  * @brief  Deinitializes the I2Cx peripheral registers to their default reset values.
  * @param  I2Cx: where x can be 0 or 1 to select the I2C peripheral.
  * @retval None
  */
void I2C_DeInit(I2C_TypeDef *I2Cx)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));

    /*Disable I2C IP*/
    if (I2Cx == I2C0)
    {
        RCC_PeriphClockCmd(APBPeriph_I2C0, APBPeriph_I2C0_CLOCK, DISABLE);
    }
    else if (I2Cx == I2C1)
    {
        RCC_PeriphClockCmd(APBPeriph_I2C1, APBPeriph_I2C1_CLOCK, DISABLE);
    }
}

/**
  * @brief  Initializes the I2Cx peripheral according to the specified
  *   parameters in the I2C_InitStruct.
  * @param  I2Cx: where x can be 0 or 1 to select the I2C peripheral.
  * @param  I2C_InitStruct: pointer to a I2C_InitTypeDef structure that
  *   contains the configuration information for the specified I2C peripheral.
  * @retval None
  */
void I2C_Init(I2C_TypeDef *I2Cx, I2C_InitTypeDef *I2C_InitStruct)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
    assert_param(IS_I2C_CLOCK_SPEED(I2C_InitStruct->I2C_ClockSpeed));

    volatile uint32_t I2CSrcClk;

    I2CSrcClk = I2C_InitStruct->I2C_Clock;
    /* Disable I2C device before change configuration */
    I2Cx->IC_ENABLE &= ~0x0001;

    /* ------------------------------ Initialize I2C device ------------------------------*/
    if (I2C_DeviveMode_Master == I2C_InitStruct->I2C_DeviveMode)
    {
        /*configure I2C device mode which can be selected for master or slave*/
        I2Cx->IC_CON = I2C_InitStruct->I2C_DeviveMode | (I2C_InitStruct->I2C_AddressMode << 4) | BIT(5);

        /*set target address*/
        I2Cx->IC_TAR = (I2C_InitStruct->I2C_SlaveAddress & 0x3ff)
                       | (I2C_InitStruct->I2C_AddressMode << 12);
        /*set SDA hold time in master mode*/
        I2Cx->IC_SDA_HOLD = 0x01;

    }
    else
    {
        /* set to slave mode */
        I2Cx->IC_CON = (I2C_InitStruct->I2C_DeviveMode) | (I2C_InitStruct->I2C_AddressMode << 3);
        /* set Ack in slave mode */
        I2Cx->IC_ACK_GENERAL_CALL &= I2C_InitStruct->I2C_Ack;
        /* set slave address */
        I2Cx->IC_SAR = I2C_InitStruct->I2C_SlaveAddress;
        /* set SDA hold time in slave mode */
        I2Cx->IC_SDA_HOLD = 0x08;
        /* set SDA setup time delay only in slave transmitter mode(greater than 2) ,delay time:[(IC_SDA_SETUP - 1) * (ic_clk_period)]*/
        I2Cx->IC_SDA_SETUP = 0x02;
    }

#if 1
    /*set Tx empty level*/
    I2Cx->IC_TX_TL = I2C_InitStruct->I2C_TxThresholdLevel;
    /*set Rx full level*/
    I2Cx->IC_RX_TL = I2C_InitStruct->I2C_RxThresholdLevel;
#endif

    /*------------------------------ configure I2C speed ------------------------------*/
    /*Configure I2C speed in standard mode*/
    if (I2C_InitStruct->I2C_ClockSpeed <= 100000)
    {
        I2Cx->IC_CON |= (0x3 << 1);
        I2Cx->IC_CON &= 0xfffb;
        /*configure I2C speed*/
        I2Cx->IC_SS_SCL_HCNT = 20 + (4000 * (I2CSrcClk / 10000)) / (I2C_InitStruct->I2C_ClockSpeed);
        I2Cx->IC_SS_SCL_LCNT = 20 + (4700 * (I2CSrcClk / 10000)) / (I2C_InitStruct->I2C_ClockSpeed);
    }
    /*Configure I2C speed in fast mode*/
    else if (I2C_InitStruct->I2C_ClockSpeed <= 400000)
    {

        I2Cx->IC_CON |= (0x3 << 1);
        I2Cx->IC_CON &= 0xfffd;
        if (I2C_InitStruct->I2C_ClockSpeed == 200000)
        {
            /*configure I2C speed*/
            I2Cx->IC_FS_SCL_HCNT = 32 + (600 * (I2CSrcClk / 10000) * 4) / (I2C_InitStruct->I2C_ClockSpeed);
            I2Cx->IC_FS_SCL_LCNT = (1300 * (I2CSrcClk / 10000) * 4) / (I2C_InitStruct->I2C_ClockSpeed);
        }
        else if (I2C_InitStruct->I2C_ClockSpeed == 400000)
        {
            /*configure I2C speed*/
            I2Cx->IC_FS_SCL_HCNT = 8 + (600 * (I2CSrcClk / 10000) * 4) / (I2C_InitStruct->I2C_ClockSpeed);
            I2Cx->IC_FS_SCL_LCNT = 1 + (1300 * (I2CSrcClk / 10000) * 4) / (I2C_InitStruct->I2C_ClockSpeed);
        }
        else
        {
            I2Cx->IC_FS_SCL_HCNT = 20 + (600 * (I2CSrcClk / 10000) * 4) / (I2C_InitStruct->I2C_ClockSpeed);
            I2Cx->IC_FS_SCL_LCNT = (1300 * (I2CSrcClk / 10000) * 4) / (I2C_InitStruct->I2C_ClockSpeed);
        }
    }
    /*Configure I2C speed in high mode*/
    else
    {
        if (I2C_InitStruct->I2C_ClockSpeed > 3400000)
        {
            I2C_InitStruct->I2C_ClockSpeed = 3400000;
        }

        I2Cx->IC_CON |= (0x3 << 1);
        /*configure I2C speed*/
        I2Cx->IC_HS_SCL_HCNT = 8 + (60 * (I2CSrcClk / 10000) * 30) / (I2C_InitStruct->I2C_ClockSpeed);
        I2Cx->IC_HS_SCL_LCNT = 1 + (120 * (I2CSrcClk / 10000) * 30) / (I2C_InitStruct->I2C_ClockSpeed);

    }

    /*Config I2C dma mode*/
    I2Cx->IC_DMA_CR = ((I2C_InitStruct->I2C_RxDmaEn)\
                       | ((I2C_InitStruct->I2C_TxDmaEn) << 1));

    /*Config I2C waterlevel*/
    I2Cx->IC_DMA_RDLR = I2C_InitStruct->I2C_RxWaterlevel;
    I2Cx->IC_DMA_TDLR = I2C_InitStruct->I2C_TxWaterlevel;

    I2Cx->IC_INTR_MASK = 0;
    for (uint8_t i = 0; i < 9; i++)
    {
        Pad_Config(I2C_InitStruct->I2C_SCL_Pin, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_DOWN, PAD_OUT_ENABLE,
                   PAD_OUT_LOW);
        Pad_Config(I2C_InitStruct->I2C_SCL_Pin, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_ENABLE,
                   PAD_OUT_HIGH);
    }
    Pad_Config(I2C_InitStruct->I2C_SCL_Pin, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE,
               PAD_OUT_LOW);
}


void I2C_BusClear(I2C_TypeDef *I2Cx)
{
    //1. do GPIO init AND do GPIO Read, conflict
    if (I2Cx == I2C0)
    {

    }
    else if (I2Cx == I2C1)
    {

    }
}

/**
  * @brief  Fills each I2C_InitStruct member with its default value.
  * @param  I2C_InitStruct : pointer to a I2C_InitTypeDef structure which will be initialized.
  * @retval None
  */
void I2C_StructInit(I2C_InitTypeDef *I2C_InitStruct)
{
    I2C_InitStruct->I2C_Clock             = 40000000;               /* depend on clock divider */
    I2C_InitStruct->I2C_ClockSpeed        = 400000;
    I2C_InitStruct->I2C_DeviveMode        = I2C_DeviveMode_Master;  /* Master mode */
    I2C_InitStruct->I2C_AddressMode       = I2C_AddressMode_7BIT;   /* 7-bit address mode */
    I2C_InitStruct->I2C_SlaveAddress      = 0;
    I2C_InitStruct->I2C_Ack               = I2C_Ack_Enable;
    I2C_InitStruct->I2C_TxThresholdLevel  = 0x00;                 /* tx fifo depth: 24 * 8bits */
    I2C_InitStruct->I2C_RxThresholdLevel  = 0x00;                 /* rx fifo depth: 16 * 8bits */
    I2C_InitStruct->I2C_TxDmaEn           = DISABLE;
    I2C_InitStruct->I2C_RxDmaEn           = DISABLE;
    I2C_InitStruct->I2C_RxWaterlevel      = 1;                    /* Best to equal GDMA Source MSize */
    I2C_InitStruct->I2C_TxWaterlevel      = 15;                   /* Best to equal Tx fifo minus
                                                                      GDMA Source MSize */
}


/**
  * @brief  Enables or disables the specified I2C peripheral.
  * @param  I2Cx: where x can be 0 or 1 to select the I2C peripheral.
  * @param  NewState: new state of the I2Cx peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void I2C_Cmd(I2C_TypeDef *I2Cx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the selected I2C peripheral */
        I2Cx->IC_ENABLE |= 0x0001;
    }
    else
    {
        /* Disable the selected I2C peripheral */
        I2Cx->IC_ENABLE &= ~0x0001;
    }
}


/**
  * @brief  Checks whether the last I2Cx abort status.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @retval I2C_Status: the status of I2Cx.
  */
I2C_Status I2C_CheckAbortStatus(I2C_TypeDef *I2Cx)
{
    uint32_t abort_status = 0;

    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));

    rt_hw_us_delay(50); //400k 2.5us * 10 *2, we must do delay here by howie
    /* Get abort status */
    abort_status = I2Cx->IC_TX_ABRT_SOURCE;

    if (abort_status & MS_ALL_ABORT)
    {
        /* Clear abort status */
        I2Cx->IC_CLR_TX_ABRT;

        /* Check abort type */
        if (abort_status & ABRT_TXDATA_NOACK)
        {
            return I2C_ABRT_TXDATA_NOACK;
        }

        if (abort_status & ABRT_7B_ADDR_NOACK)
        {
            return I2C_ABRT_7B_ADDR_NOACK;
        }

        if (abort_status & ARB_LOST)
        {
            return I2C_ARB_LOST;
        }

        if (abort_status & ABRT_MASTER_DIS)
        {
            return I2C_ABRT_MASTER_DIS;
        }

        if (abort_status & ABRT_10ADDR1_NOACK)
        {
            return I2C_ABRT_10ADDR1_NOACK;
        }

        if (abort_status & ABRT_10ADDR2_NOACK)
        {
            return I2C_ABRT_10ADDR2_NOACK;
        }
    }

    return I2C_Success;
}

#if IIC_LEGACY
/**
  * @brief  Send data in master mode through the I2Cx peripheral.
  * @param  I2Cx: where x can be 0 or 1 to select the I2C peripheral.
  * @param  Data: Byte to be transmitted..
  * @retval None
  */
I2C_Status I2C_MasterWrite(I2C_TypeDef *I2Cx, uint8_t *pBuf, uint16_t len)
{
    uint16_t cnt = 0;
    I2C_Status abort_status = I2C_Success;

    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));

    /* Write in the DR register the data to be sent */
    for (cnt = 0; cnt < len; cnt++)
    {
        if (cnt >= len - 1)
        {
            /*generate stop signal*/
            I2Cx->IC_DATA_CMD = (*pBuf++) | (1 << 9);
        }
        else
        {
            I2Cx->IC_DATA_CMD = *pBuf++;
        }

        /* wait for flag of I2C_FLAG_TFNF */

        /* TX FIFO 24 * 8bit */
        /*if scl == 200k, FIFO NOT full means shift one byte out*/
        /*200K means 5us/bit, one byte 50us*/
        /*so, wait 50us is ok*/

        uint32_t local_time = timeout_us;
        while ((I2Cx->IC_STATUS & (1 << 1)) == 0)
        {
            /* Check abort status */
            abort_status = I2C_CheckAbortStatus(I2Cx);
            if (abort_status != I2C_Success)
            {
                return abort_status;
            }
            rt_hw_us_delay(10);
            local_time =  local_time - 10;

            if (local_time == 0)
            {
                APP_PRINT_ERROR0("[IIC] WRITE TIME OUT");//todo
                I2C_BusClear(I2Cx);
                return I2C_ERR_TIMEOUT;
            }
        }

        /* Check abort status */
        abort_status = I2C_CheckAbortStatus(I2Cx);
        if (abort_status != I2C_Success)
        {
            return abort_status;
        }
    }

    return abort_status;
}
#else
/**
  * @brief  Send data in master mode through the I2Cx peripheral.
  * @param  I2Cx: where x can be 0 or 1 to select the I2C peripheral.
  * @param  Data: Byte to be transmitted..
  * @retval None
  */
I2C_Status I2C_MasterWrite(I2C_TypeDef *I2Cx, uint8_t *pBuf, uint16_t len)
{
    uint16_t cnt = 0;
    I2C_Status abort_status = I2C_Success;

    /* Write in the DR register the data to be sent */
    for (cnt = 0; cnt < len; cnt++)
    {
        if (cnt >= len - 1)
        {
            /*generate stop signal*/
            I2Cx->IC_DATA_CMD = (*pBuf++) | (1 << 9);
            rt_hw_us_delay(50);
            uint32_t iic_timeout = 0;
            while (I2C_GetTxFIFOLen(I2Cx) > 0)
            {
                rt_hw_us_delay(10);// todo add for time out
                iic_timeout = iic_timeout + 10;
                abort_status = I2C_CheckAbortStatus(I2Cx);
                if (abort_status != I2C_Success)
                {
                    APP_PRINT_ERROR1("[IIC] MASTER Write Error line = %d", __LINE__);
                    return abort_status;
                }
                if (iic_timeout > IIC_MAX_TIMEOUT_US)
                {
                    return I2C_ERR_TIMEOUT;
                }
            }
        }
        else
        {
            I2Cx->IC_DATA_CMD = *pBuf++;
            rt_hw_us_delay(50);
            uint32_t iic_timeout = 0;
            while (I2C_GetTxFIFOLen(I2Cx) > 0)
            {
                rt_hw_us_delay(10);// todo add for time out
                iic_timeout = iic_timeout + 10;
                abort_status = I2C_CheckAbortStatus(I2Cx);
                if (abort_status != I2C_Success)
                {
                    APP_PRINT_ERROR1("[IIC] MASTER Write Error line = %d", __LINE__);
                    return abort_status;
                }
                if (iic_timeout > IIC_MAX_TIMEOUT_US)
                {
                    return I2C_ERR_TIMEOUT;
                }
            }
        }
    }
    uint32_t iic_timeout = 0;
    while (I2C_GetFlagState(I2Cx, I2C_FLAG_MST_ACTIVITY) == SET)
    {
        rt_hw_us_delay(10);// todo add for time out
        iic_timeout = iic_timeout + 10;
        abort_status = I2C_CheckAbortStatus(I2Cx);
        if (abort_status != I2C_Success)
        {
            APP_PRINT_ERROR1("[IIC] MASTER Write Error line = %d", __LINE__);
            return abort_status;
        }
        if (iic_timeout > IIC_MAX_TIMEOUT_US)
        {
            return I2C_ERR_TIMEOUT;
        }
    }

    return abort_status;
}
#endif

#if IIC_LEGACY
/**
  * @brief  Read data in master mode through the I2Cx peripheral.
  * @param  I2Cx: where x can be 0 or 1 to select the I2C peripheral.
  * @param  Data: Byte to be transmitted..
  * @retval None
  */
I2C_Status I2C_MasterRead(I2C_TypeDef *I2Cx, uint8_t *pBuf, uint16_t len)
{
    uint16_t cnt = 0;
    uint32_t reg_value = 0;
    I2C_Status abort_status = I2C_Success;

    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));

    /* read in the DR register the data to be sent */
    for (cnt = 0; cnt < len; cnt++)
    {
        if (cnt >= len - 1)
        {
            /* generate stop singal */
            I2Cx->IC_DATA_CMD = (reg_value) | (0x0003 << 8);
        }
        else
        {
            I2Cx->IC_DATA_CMD = (reg_value) | (0x0001 << 8);
        }

        /* read data */
        uint32_t local_time = timeout_us;
        if (cnt > 0)
        {
            /* wait for I2C_FLAG_RFNE flag */
            while ((I2Cx->IC_STATUS & (1 << 3)) == 0)
            {
                /* Check abort status */
                abort_status = I2C_CheckAbortStatus(I2Cx);
                if (abort_status != I2C_Success)
                {
                    return abort_status;
                }

                rt_hw_us_delay(10);
                local_time =  local_time - 10;
                if (local_time == 0)
                {
                    APP_PRINT_ERROR0("[IIC] READ TIME OUT NOT LAST ONE");//todo
                    I2C_BusClear(I2Cx);
                    return I2C_ERR_TIMEOUT;
                }
            }

            *pBuf++ = (uint8_t)I2Cx->IC_DATA_CMD;
        }
    }

    /* wait for I2C_FLAG_RFNE flag */
    uint32_t local_time = timeout_us;
    while ((I2Cx->IC_STATUS & (1 << 3)) == 0)
    {
        /* Check abort status */
        abort_status = I2C_CheckAbortStatus(I2Cx);
        if (abort_status != I2C_Success)
        {
            return abort_status;
        }

        rt_hw_us_delay(10);
        local_time =  local_time - 10;
        if (local_time == 0)
        {
            APP_PRINT_ERROR0("[IIC] READ TIME OUT IS LAST ONE");//todo
            I2C_BusClear(I2Cx);
            return I2C_ERR_TIMEOUT;
        }
    }

    *pBuf = (uint8_t)I2Cx->IC_DATA_CMD;

    return abort_status;
}
#else
I2C_Status I2C_MasterRead(I2C_TypeDef *I2Cx, uint8_t *pBuf, uint16_t len)
{
    uint16_t cnt = 0;
    uint32_t reg_value = 0;
    I2C_Status abort_status = I2C_Success;

    /* read in the DR register the data to be sent */
    for (cnt = 0; cnt < len; cnt++)
    {
        if (cnt >= len - 1)
        {
            /* generate stop singal */
            I2Cx->IC_DATA_CMD = (reg_value) | (0x0003 << 8);
            uint32_t iic_timeout = 0;
            while (I2C_GetTxFIFOLen(I2Cx) > 0)
            {
                rt_hw_us_delay(10);
                iic_timeout = iic_timeout + 10;
                abort_status = I2C_CheckAbortStatus(I2Cx);
                if (abort_status != I2C_Success)
                {
                    APP_PRINT_ERROR1("[IIC] MASTER Read Error line = %d", __LINE__);
                    return abort_status;
                }
                if (iic_timeout > IIC_MAX_TIMEOUT_US)
                {
                    return I2C_ERR_TIMEOUT;
                }
            }
        }
        else
        {
            I2Cx->IC_DATA_CMD = (reg_value) | (0x0001 << 8);
            uint32_t iic_timeout = 0;
            while (I2C_GetTxFIFOLen(I2Cx) > 0)
            {
                rt_hw_us_delay(10);
                iic_timeout = iic_timeout + 10;
                abort_status = I2C_CheckAbortStatus(I2Cx);
                if (abort_status != I2C_Success)
                {
                    APP_PRINT_ERROR1("[IIC] MASTER Read Error line = %d", __LINE__);
                    return abort_status;
                }
                if (iic_timeout > IIC_MAX_TIMEOUT_US)
                {
                    return I2C_ERR_TIMEOUT;
                }
            }
        }

        uint32_t iic_timeout = 0;
        while (I2C_GetRxFIFOLen(I2Cx) == 0)
        {
            rt_hw_us_delay(10);
            iic_timeout = iic_timeout + 10;
            abort_status = I2C_CheckAbortStatus(I2Cx);
            if (abort_status != I2C_Success)
            {
                APP_PRINT_ERROR1("[IIC] MASTER Read Error line = %d", __LINE__);
                return abort_status;
            }
            if (iic_timeout > IIC_MAX_TIMEOUT_US)
            {
                return I2C_ERR_TIMEOUT;
            }
        }
        *pBuf++ = (uint8_t)I2Cx->IC_DATA_CMD;

    }

    uint32_t iic_timeout = 0;
    while (I2C_GetFlagState(I2Cx, I2C_FLAG_MST_ACTIVITY) == SET)
    {
        rt_hw_us_delay(10);// todo add for time out
        iic_timeout = iic_timeout + 10;
        abort_status = I2C_CheckAbortStatus(I2Cx);
        if (abort_status != I2C_Success)
        {
            APP_PRINT_ERROR1("[IIC] MASTER Read Error line = %d", __LINE__);
            return abort_status;
        }
        if (iic_timeout > IIC_MAX_TIMEOUT_US)
        {
            return I2C_ERR_TIMEOUT;
        }
    }

    return abort_status;
}
#endif

#if IIC_LEGACY
/**
  * @brief  Sends data and read data in master mode through the I2Cx peripheral.Attention:Read data with time out mechanism.
  * @param  I2Cx: where x can be 0 or 1 to select the I2C peripheral.
  * @param  Data: Byte to be transmitted..
  * @retval Actual length of read data
  */
I2C_Status I2C_RepeatRead(I2C_TypeDef *I2Cx, uint8_t *pWriteBuf, uint16_t Writelen,
                          uint8_t *pReadBuf, uint16_t Readlen)
{
    uint16_t cnt = 0;
    uint32_t reg_value = 0;
    I2C_Status abort_status = I2C_Success;

    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));

    /*------------------------------ write data section ------------------------------*/
    /* write data in the IC_DATA_CMD register */
    for (cnt = 0; cnt < Writelen; cnt++)
    {
        I2Cx->IC_DATA_CMD = *pWriteBuf++;

        /*wait for I2C_FLAG_TFNF flag that Tx FIFO is not full*/
        uint32_t local_time = timeout_us;
        while ((I2Cx->IC_STATUS & BIT(1)) == 0)
        {
            /* Check abort status */
            abort_status = I2C_CheckAbortStatus(I2Cx);
            if (abort_status != I2C_Success)
            {
                return abort_status;
            }

            rt_hw_us_delay(10);
            local_time =  local_time - 10;
            if (local_time == 0)
            {
                APP_PRINT_ERROR0("[IIC] REPEART TIME OUT WRITE");//todo
                I2C_BusClear(I2Cx);
                return I2C_ERR_TIMEOUT;
            }
        }

        /* Check abort status */
        abort_status = I2C_CheckAbortStatus(I2Cx);
        if (abort_status != I2C_Success)
        {
            return abort_status;
        }
    }

    /*------------------------------ read data section ------------------------------*/
    for (cnt = 0; cnt < Readlen; cnt++)
    {
        if (cnt >= Readlen - 1)
        {
            /*generate stop singal in last byte which to be sent*/
            I2Cx->IC_DATA_CMD = reg_value | BIT(8) | BIT(9);
        }
        else
        {
            I2Cx->IC_DATA_CMD = reg_value | BIT(8);
        }

        /*read data */
        uint32_t local_time = timeout_us;
        if (cnt > 0)
        {
            /* wait for I2C_FLAG_RFNE flag */
            while ((I2Cx->IC_STATUS & (1 << 3)) == 0)
            {
                /* Check abort status */
                abort_status = I2C_CheckAbortStatus(I2Cx);
                if (abort_status != I2C_Success)
                {
                    return abort_status;
                }

                rt_hw_us_delay(10);
                local_time =  local_time - 10;
                if (local_time == 0)
                {
                    APP_PRINT_ERROR0("[IIC] REPEAT TIME OUT NOT LAST ONE");//todo
                    I2C_BusClear(I2Cx);
                    return I2C_ERR_TIMEOUT;
                }
            }

            *pReadBuf++ = (uint8_t)I2Cx->IC_DATA_CMD;
        }
    }

    /*read data*/
    uint32_t local_time = timeout_us;
    while ((I2Cx->IC_STATUS & (1 << 3)) == 0)
    {
        /* Check abort status */
        abort_status = I2C_CheckAbortStatus(I2Cx);
        if (abort_status != I2C_Success)
        {
            return abort_status;
        }

        rt_hw_us_delay(10);
        local_time =  local_time - 10;
        {
            APP_PRINT_ERROR0("[IIC] REPEAT TIME OUT IS LAST ONE");//todo
            I2C_BusClear(I2Cx);
            return I2C_ERR_TIMEOUT;
        }
    }

    *pReadBuf = (uint8_t)I2Cx->IC_DATA_CMD;

    return abort_status;
}
#else
I2C_Status I2C_RepeatRead(I2C_TypeDef *I2Cx, uint8_t *pWriteBuf, uint16_t Writelen,
                          uint8_t *pReadBuf, uint16_t Readlen)
{
    I2C_Status ret = I2C_Success;
    ret = I2C_MasterWrite(I2Cx, pWriteBuf, Writelen);
    if (ret != I2C_Success)
    {
        return ret;
    }
    rt_hw_us_delay(1);
    return I2C_MasterRead(I2Cx, pReadBuf, Readlen);
}
#endif

/**
  * @brief mask the specified I2C interrupt.
  * @param  I2Cx: where x can be 0 or 1
  * @param  I2C_INT
  * This parameter can be one of the following values:
  *     @arg I2C_INT_GEN_CALL: Set only when a General Call address is received and it is acknowledged.
  *     @arg I2C_INT_START_DET: Indicates whether a START or RESTART condition has occurred on the I2C
                              interface regardless of whether I2C is operating in slave or master mode.
  *     @arg I2C_INT_STOP_DET:  Indicates whether a STOP condition has occurred on the I2C interface regardless
                              of whether I2C is operating in slave or master mode
  *     @arg I2C_INT_ACTIVITY:  This bit captures I2C activity and stays set until it is cleared.
  *     @arg I2C_INT_RX_DONE:   When the I2C is acting as a slave-transmitter, this bit is set to 1 if the
                              master does not acknowledge a transmitted byte. This occurs on the last byte of
                              the transmission, indicating that the transmission is done.
  *     @arg I2C_INT_TX_ABRT:   This bit indicates if I2C as an I2C transmitter, is unable to complete the
                              intended actions on the contents of the transmit FIFO.
  *     @arg I2C_INT_RD_REQ:    This bit is set to 1 when acting as a slave and another I2C master
                              is attempting to read data.
  *     @arg I2C_INT_TX_EMPTY:  This bit is set to 1 when the transmit buffer is at or below the threshold value set
                              in the IC_TX_TL register.
  *     @arg I2C_INT_TX_OVER:   Set during transmit if the transmit buffer is filled to IC_TX_BUFFER_DEPTH and
                              the processor attempts to issue another I2C command.
  *     @arg I2C_INT_RX_FULL:   Set when the receive buffer reaches or goes above the RX_TL threshold in the
                              IC_RX_TL register
  *     @arg I2C_INT_RX_OVER:   Set if the receive buffer is completely filled to IC_RX_BUFFER_DEPTH and an
                               additional byte is received from an external I2C device.
  *     @arg I2C_INT_RX_UNDER:   Set if the processor attempts to read the receive buffer when it is empty by reading.
  * @retval None.
  */
void I2C_INTConfig(I2C_TypeDef *I2Cx, uint16_t I2C_INT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
    assert_param(I2C_GET_INT(I2C_INT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the selected I2C interrupts */
        I2Cx->IC_INTR_MASK |= I2C_INT;
    }
    else
    {
        /* Disable the selected I2C interrupts */
        I2Cx->IC_INTR_MASK &= (uint16_t)~I2C_INT;
    }
}

/**
  * @brief clear the specified I2C interrupt.
  * @param  I2Cx: where x can be 0 or 1
  * @retval None.
  */
void I2C_ClearINTPendingBit(I2C_TypeDef *I2Cx, uint16_t I2C_IT)
{
    /* Check the parameters */
    assert_param(IS_I2C_ALL_PERIPH(I2Cx));
    assert_param(I2C_GET_INT(I2C_IT));

    switch (I2C_IT)
    {
    case I2C_INT_RX_UNDER:
        {
            I2Cx->IC_CLR_RX_UNDER;
            break;
        }
    case I2C_INT_RX_OVER:
        {
            I2Cx->IC_CLR_RX_OVER;
            break;
        }
    case I2C_INT_TX_OVER:
        {
            I2Cx->IC_CLR_TX_OVER;
            break;
        }
    case I2C_INT_RD_REQ:
        {
            I2Cx->IC_CLR_RD_REQ;
            break;
        }
    case I2C_INT_TX_ABRT:
        {
            I2Cx->IC_CLR_TX_ABRT;
            break;
        }
    case I2C_INT_RX_DONE:
        {
            I2Cx->IC_CLR_RX_DONE;
            break;
        }
    case I2C_INT_ACTIVITY:
        {
            I2Cx->IC_CLR_ACTIVITY;
            break;
        }
    case I2C_INT_STOP_DET:
        {
            I2Cx->IC_CLR_STOP_DET;
            break;
        }
    case I2C_INT_START_DET:
        {
            I2Cx->IC_CLR_START_DET;
            break;
        }
    case I2C_INT_GEN_CALL:
        {
            I2Cx->IC_CLR_GEN_CALL;
            break;
        }
    default:
        {
            break;
        }
    }
}

/******************* (C) COPYRIGHT 2015 Realtek Semiconductor Corporation *****END OF FILE****/

