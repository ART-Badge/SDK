
/*********************************************************************************************************
*               Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_module_gsensor_stk8321.c
* @brief
* @details
* @author
* @date
* @version  v0.1
*********************************************************************************************************
*/

#include "trace.h"
#include "string.h"
#include "rtl876x_spi.h"
#include "rtl876x_i2c_wristband.h"
#include "rtl876x_rcc.h"
#include "module_gsensor_stk8321.h"
#include "board.h"
#include "platform_utils.h"



bool stk8321_read(uint8_t read_reg, uint8_t *buf, uint8_t read_len)
{
    uint8_t addr[1] = {read_reg};
    I2C_SetSlaveAddress(GSENSOR_I2C_BUS, GSENSOR_SLAVE_ADDRESS);

    if (I2C_MasterWrite(GSENSOR_I2C_BUS, addr, 1) != I2C_Success)
    {
        APP_PRINT_INFO1(" Gsensor read fail = %d!", __LINE__);
        return false;
    }
    platform_delay_us(1);
    if (I2C_MasterRead(GSENSOR_I2C_BUS, buf, read_len) != I2C_Success)
    {
        APP_PRINT_INFO1(" Gsensor read fail line = %d!", __LINE__);
        return false;
    }

    return true;
}

bool stk8321_write(uint8_t write_reg, uint8_t *buf, uint8_t write_len)
{
    //DBG_DIRECT("[stk8321], write enter!");
    I2C_SetSlaveAddress(GSENSOR_I2C_BUS, GSENSOR_SLAVE_ADDRESS);

    uint8_t write_buf[1 + write_len];
    write_buf[0] = write_reg;
    memcpy(write_buf + 1, buf, write_len);
    I2C_Status ret = I2C_MasterWrite(GSENSOR_I2C_BUS, write_buf, write_len + 1);
#if 0
    DBG_DIRECT("[stk8321], write exit ret = %d!", ret);
    platform_delay_us(100); //400k 2.5us * 10 *2, we must do delay here by howie
    uint8_t len = I2C_GetTxFIFOLen(GSENSOR_I2C_BUS);
    while ((len != 0) && (time_out-- != 0));
    DBG_DIRECT("[stk8321], write exit len = %d!", len);
#endif
    return true;
}

uint8_t stk8321_get_fifo_length(void)
{
    uint8_t len;

    stk8321_read(0x0C, &len, 1);
    len = len & 0x7f;

    return len;
}

bool stk8321_get_fifo_data(uint8_t *len, AxesRaw_t *buf)
{
    uint8_t fifo_len;
    uint8_t accReg[6 * 32] = {0};
    uint8_t reg = 0x3F;
    uint8_t shift_num = 4;

    fifo_len = stk8321_get_fifo_length();

    if ((fifo_len > 0) && (fifo_len <= 32))
    {
        if (I2C_MasterWrite(GSENSOR_I2C_BUS, &reg, 1) != I2C_Success)
        {
            APP_PRINT_INFO1(" Gsensor read fifo fail = %d!", __LINE__);
            return false;
        }
        platform_delay_us(1);
        if (I2C_MasterRead(GSENSOR_I2C_BUS, accReg, 6 * fifo_len) != I2C_Success)
        {
            APP_PRINT_INFO1(" Gsensor read fifo fail line = %d!", __LINE__);
            return false;
        }
        for (uint8_t i = 0; i < fifo_len; i++)
        {
            (&buf[i])->AXIS_X = ((int16_t)(accReg[i * 6 + 1] << 8 | accReg[i * 6]))     >> shift_num;
            (&buf[i])->AXIS_Y = ((int16_t)(accReg[i * 6 + 3] << 8 | accReg[i * 6 + 2])) >> shift_num;
            (&buf[i])->AXIS_Z = ((int16_t)(accReg[i * 6 + 5] << 8 | accReg[i * 6 + 4])) >> shift_num;
        }
        return true;
    }
    else
    {
        return false;
    }
}


bool stk8321_driver_init(void)
{
    APP_PRINT_INFO0("stk8321 driver init\n");

    Pinmux_Config(GSENSOR_I2C_SCL, GSENSOR_I2C_FUNC_SCL);
    Pinmux_Config(GSENSOR_I2C_SDA, GSENSOR_I2C_FUNC_SDA);
    {
        /* add protect for I2C deadlock */
        Pad_Config(GSENSOR_I2C_SCL, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_ENABLE, PAD_OUT_HIGH);
        Pad_Config(GSENSOR_I2C_SDA, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_ENABLE, PAD_OUT_LOW);
    }

    Pad_Config(GSENSOR_I2C_SCL, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE,
               PAD_OUT_LOW);
    Pad_Config(GSENSOR_I2C_SDA, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE,
               PAD_OUT_LOW);
    /* Enable I2C clock */
    RCC_I2CClkDivConfig(GSENSOR_I2C_BUS, I2C_CLOCK_DIV_2);
    RCC_PeriphClockCmd(GSENSOR_I2C_APBPeriph, GSENSOR_I2C_APBClock, ENABLE);
    I2C_InitTypeDef  I2C_InitStructure;
    I2C_InitStructure.I2C_Clock = 20000000;
    I2C_InitStructure.I2C_ClockSpeed   = 400000;
    I2C_InitStructure.I2C_DeviveMode   = I2C_DeviveMode_Master;
    I2C_InitStructure.I2C_AddressMode  = I2C_AddressMode_7BIT;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_SDA_Pin = TOUCH_I2C_SDA;
    I2C_InitStructure.I2C_SCL_Pin = TOUCH_I2C_SCL;
    I2C_Init(GSENSOR_I2C_BUS, &I2C_InitStructure);
    I2C_Cmd(GSENSOR_I2C_BUS, ENABLE);
    uint8_t value = 0x56;
    stk8321_write(0x11, &value, 1);
//    value = 0x03;
//    stk8321_write(0x0F, &value, 1);
//    value = 0x0F;
//    stk8321_write(0x10, &value, 1);
//    value = 0x04;
//    stk8321_write(0x34, &value, 1);
//    value = 0x20;
//    stk8321_write(0x3D, &value, 1);
//    value = 0x20;
//    stk8321_write(0x3E, &value, 1);
//    value = 0x01;
//    stk8321_write(0x19, &value, 1);
//    value = 0x40;
//    stk8321_write(0x1A, &value, 1);
//    value = 0x8F;
//    stk8321_write(0x21, &value, 1);
//    value = 0x05;
//    stk8321_write(0x20, &value, 1);
//    value = 0x00;
//    stk8321_write(0x17, &value, 1);
    return true;
}


void stk8321_enter_dlps(void)
{
    Pad_Config(GSENSOR_I2C_SCL, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(GSENSOR_I2C_SDA, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
}
void stk8321_exit_dlps(void)
{
    Pad_Config(GSENSOR_I2C_SCL, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE,
               PAD_OUT_LOW);
    Pad_Config(GSENSOR_I2C_SDA, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE,
               PAD_OUT_LOW);
}

void stk8321_enable(void)
{
    stk8321_driver_init();
}

void stk8321_disable(void)
{
    uint8_t value = 0x80;
    stk8321_write(0x11, &value, 1);        // Deep Supend Mode
}







