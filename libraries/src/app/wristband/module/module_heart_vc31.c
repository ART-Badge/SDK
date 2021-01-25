
/*********************************************************************************************************
*               Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     module_heart_vc31.c
* @brief
* @details
* @author
* @date
* @version  v0.1
*********************************************************************************************************
*/

#include "trace.h"
#include "rtl876x_spi.h"
#include "rtl876x_i2c_wristband.h"
#include "rtl876x_rcc.h"
#include "rtl876x_gpio.h"
#include "rtl876x_nvic.h"
#include "module_heart_vc31.h"
#include "VC31Hci.h"
#include "algoforRTL8762CJ.h"
#include "board.h"
#include "string.h"
#include "hub_task.h"
#include "platform_utils.h"

vc31_read_cb user_vc31_read_cb = vc31_read;
vc31_write_cb user_vc31_write_cb  = vc31_write;
vc31_gettime_cb user_vc31_gettime_cb = vc31_gettime;

VC31_t TEST_VC31;

VC31Ret_t vc31_read(uint8_t read_reg, uint8_t *buf, uint8_t read_len)
{
    I2C_SetSlaveAddress(HRS_I2C_BUS, HRS_SLAVE_ADDRESS);
    uint8_t addr[1] = {read_reg};

    if (I2C_MasterWrite(HRS_I2C_BUS, addr, 1) != I2C_Success)
    {
        APP_PRINT_INFO1(" vc31 read fail line = %d!", __LINE__);
        return VC31Ret_isERR;
    }
    platform_delay_us(1);
    if (I2C_MasterRead(HRS_I2C_BUS, buf, read_len) != I2C_Success)
    {
        APP_PRINT_INFO1(" vc31 read fail line = %d!", __LINE__);
        return VC31Ret_isERR;
    }

    return VC31Ret_isOK;
}

VC31Ret_t vc31_write(uint8_t write_reg, uint8_t *buf, uint8_t write_len)
{
    I2C_SetSlaveAddress(HRS_I2C_BUS, HRS_SLAVE_ADDRESS);

    uint8_t write_buf[1 + write_len];
    write_buf[0] = write_reg;
    memcpy(write_buf + 1, buf, write_len);
    I2C_MasterWrite(HRS_I2C_BUS, write_buf, write_len + 1);
    return VC31Ret_isOK;
}

uint32_t vc31_gettime(void)
{
    return 0;
}




bool vc31_driver_init(void)
{
    APP_PRINT_INFO0("vc31 driver init\n");


    Pinmux_Config(HRS_INT, DWGPIO);
    Pad_Config(HRS_INT, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
    /* Enable GPIO and hardware timer's clock */
    RCC_PeriphClockCmd(APBPeriph_GPIO,  APBPeriph_GPIO_CLOCK,  ENABLE);

    /* Initialize GPIO as interrupt mode */
    GPIO_InitTypeDef GPIO_Param;
    GPIO_StructInit(&GPIO_Param);
    GPIO_Param.GPIO_Pin = GPIO_GetPin(HRS_INT);
    GPIO_Param.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Param.GPIO_ITCmd = ENABLE;
    GPIO_Param.GPIO_ITTrigger = GPIO_INT_Trigger_EDGE;
    GPIO_Param.GPIO_ITPolarity = GPIO_INT_POLARITY_ACTIVE_LOW;
    GPIO_Init(&GPIO_Param);

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = HRS_INT_IRQ;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    GPIO_INTConfig(GPIO_GetPin(HRS_INT), ENABLE);
    GPIO_MaskINTConfig(GPIO_GetPin(HRS_INT), DISABLE);

    Pinmux_Config(HRS_I2C_SCL, HRS_I2C_FUNC_SCL);
    Pinmux_Config(HRS_I2C_SDA, HRS_I2C_FUNC_SDA);

    Pad_Config(HRS_I2C_SCL, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_Config(HRS_I2C_SDA, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_PullConfigValue(HRS_I2C_SCL, PAD_STRONG_PULL);
    Pad_PullConfigValue(HRS_I2C_SDA, PAD_STRONG_PULL);
    /* Enable I2C clock */
    RCC_I2CClkDivConfig(HRS_I2C_BUS, I2C_CLOCK_DIV_2);
    RCC_PeriphClockCmd(HRS_I2C_APBPeriph, HRS_I2C_APBClock, ENABLE);
    I2C_InitTypeDef  I2C_InitStructure;
    I2C_InitStructure.I2C_Clock = 20000000;
    I2C_InitStructure.I2C_ClockSpeed   = 400000;
    I2C_InitStructure.I2C_DeviveMode   = I2C_DeviveMode_Master;
    I2C_InitStructure.I2C_AddressMode  = I2C_AddressMode_7BIT;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_SDA_Pin = HRS_I2C_SDA;
    I2C_InitStructure.I2C_SCL_Pin = HRS_I2C_SCL;
    I2C_Init(HRS_I2C_BUS, &I2C_InitStructure);
    I2C_Cmd(HRS_I2C_BUS, ENABLE);

    VC31Init(&TEST_VC31, VC31Mode_NormalWork);       //VC31 chip initial
    Algo_Init();                                     //VC31 algorithm initial
    VC31StopSample(&TEST_VC31);
    VC31StartSample(&TEST_VC31);

    return true;
}


void vc31_enter_dlps(void)
{
    Pad_Config(HRS_I2C_SCL, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(HRS_I2C_SDA, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(HRS_INT, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
}
void vc31_exit_dlps(void)
{
    Pad_Config(HRS_I2C_SCL, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_Config(HRS_I2C_SDA, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_Config(HRS_INT, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
}

void vc31_enable(void)
{

}

void vc31_disable(void)
{


}

void vc31_process(void)
{
    VC31GetSampleValues(&TEST_VC31);
}

void HRS_INT_HANDLER(void)
{
    /*  Mask GPIO interrupt */
    GPIO_INTConfig(GPIO_GetPin(HRS_INT), DISABLE);
    GPIO_MaskINTConfig(GPIO_GetPin(HRS_INT), ENABLE);
    GPIO_ClearINTPendingBit(GPIO_GetPin(HRS_INT));

    T_IO_MSG hrs_msg;
    hrs_msg.type = HUB_MSG_HRM;
    hrs_msg.subtype = HRM_SENSOR_MSG_WAKEUP;
    send_msg_to_ui_task(&hrs_msg, __LINE__);


    GPIO_INTConfig(GPIO_GetPin(HRS_INT), ENABLE);
    GPIO_MaskINTConfig(GPIO_GetPin(HRS_INT), DISABLE);
}






