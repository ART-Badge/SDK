/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_module_touch_gesture_button.c
* @brief
* @details
* @author
* @date
* @version  v0.1
*********************************************************************************************************
*/
#include "board.h"
#include "string.h"
#include "os_timer.h"
#include "os_sched.h"
#include "rtl876x_pinmux.h"
#include "module_touch.h"
#include "hub_button.h"
#include "rtl876x_i2c_wristband.h"
#include "rtl876x_rtc.h"
#include "wristband_global_data.h"
#include "app_msg.h"
#include "app_task.h"
#include "flash_device.h"
#include "touch_algorithm.h"
#include "rtl876x_wdg.h"
#include "os_sync.h"
#include "gui_core.h"
#include "hub_task.h"
#include "platform_utils.h"


#define ENTER_DLPS_TIMER_INTERVAL           500

static void     *touch_gesture_release_timer = NULL;
static void     *allowed_enter_dlps_timer = NULL;

static bool touch_initialized = false;
static bool touch_enable_flag = false;
static uint8_t chip_id = 0x59;
extern bool allowed_touch_enter_dlps;

static TOUCH_DATA cur_point;
void touch_gesture_callback(RtkTouchInfo_t *info);


static void allowed_enter_dlps_callback(void *pxTimer)
{
    allowed_touch_enter_dlps = true;
}
/**
  * @brief  Initialize touch device
  * @param  None
  *
  * @retval None
  */
static void cst0x6_device_init(void)
{
    touch_gesture_pad_init();
    /* Enable GPIO and hardware timer's clock */
    RCC_PeriphClockCmd(APBPeriph_GPIO,  APBPeriph_GPIO_CLOCK,  ENABLE);

    /* Initialize GPIO as interrupt mode */
    GPIO_InitTypeDef GPIO_Param;
    GPIO_StructInit(&GPIO_Param);
    GPIO_Param.GPIO_Pin = GPIO_GetPin(TOUCH_INT);
    GPIO_Param.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Param.GPIO_ITCmd = ENABLE;
    GPIO_Param.GPIO_ITTrigger = GPIO_INT_Trigger_EDGE;
    GPIO_Param.GPIO_ITPolarity = GPIO_INT_POLARITY_ACTIVE_LOW;
    GPIO_Init(&GPIO_Param);

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = TOUCH_INT_IRQ;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    RCC_I2CClkDivConfig(TOUCH_I2C_BUS, I2C_CLOCK_DIV_2);
    RCC_PeriphClockCmd(TOUCH_I2C_APBPeriph, TOUCH_I2C_APBClock, ENABLE);
    I2C_InitTypeDef  I2C_InitStructure;
    I2C_InitStructure.I2C_Clock = 20000000;
    I2C_InitStructure.I2C_ClockSpeed   = 400000;
    I2C_InitStructure.I2C_DeviveMode   = I2C_DeviveMode_Master;
    I2C_InitStructure.I2C_AddressMode  = I2C_AddressMode_7BIT;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_SDA_Pin = TOUCH_I2C_SDA;
    I2C_InitStructure.I2C_SCL_Pin = TOUCH_I2C_SCL;
    I2C_Init(TOUCH_I2C_BUS, &I2C_InitStructure);
    I2C_Cmd(TOUCH_I2C_BUS, ENABLE);

    RtkCallbackReg((touch_algorithm_func)touch_gesture_callback);
    System_WakeUpPinEnable(TOUCH_INT, PAD_WAKEUP_POL_LOW, PAD_WK_DEBOUNCE_DISABLE);
    os_timer_create(&allowed_enter_dlps_timer, "touch check dlps timer", 1, ENTER_DLPS_TIMER_INTERVAL,
                    false, allowed_enter_dlps_callback);
}

static void touch_gesture_pad_init(void)
{
    Pinmux_Config(TOUCH_I2C_SCL, TOUCH_I2C_FUNC_SCL);
    Pinmux_Config(TOUCH_I2C_SDA, TOUCH_I2C_FUNC_SDA);
    Pinmux_Config(TOUCH_INT, DWGPIO);

    Pad_Config(TOUCH_I2C_SCL, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_Config(TOUCH_I2C_SDA, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_Config(TOUCH_INT, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_Config(TOUCH_RST, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
}

void touch_gesture_exit_dlps(void)
{
    Pad_Config(TOUCH_I2C_SCL, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_Config(TOUCH_I2C_SDA, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_Config(TOUCH_INT, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
    //Pad_Config(TOUCH_RST, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE, PAD_OUT_LOW);
}

void touch_gesture_enter_dlps(void)
{
    Pad_Config(TOUCH_I2C_SCL, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_Config(TOUCH_I2C_SDA, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_Config(TOUCH_INT, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
    //Pad_Config(TOUCH_RST, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
}

void cst0x6_driver_init(void)
{
    cst0x6_device_init();
    Pad_Config(TOUCH_RST, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_LOW);
    os_delay(1);
    Pad_Config(TOUCH_RST, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);

    os_delay(30);
    os_timer_create(&touch_gesture_release_timer, "touch gesture release timer", 1, 20, false,
                    touch_gesture_release_timeout);
    GPIO_INTConfig(GPIO_GetPin(TOUCH_INT), ENABLE);
    GPIO_MaskINTConfig(GPIO_GetPin(TOUCH_INT), DISABLE);

    touch_enable_flag = true;
    touch_initialized = true;
    cst0x6_get_chip_id(&chip_id);
#if 0
    uint32_t s;
    s = os_lock();
    WDG_Disable();
    extern void hctp_update_test();
    hctp_update_test();
    //WDG_Enable();
    os_unlock(s);
    DBG_DIRECT("TP updata Ok! loop here!!");
    DBG_DIRECT("TP updata Ok! loop here!!");
    DBG_DIRECT("TP updata Ok! loop here!!");
    while (1);
#endif
}

void enable_touch_sensor(void)
{
    if (touch_enable_flag == false)
    {
        touch_enable_flag = true;
        if (touch_initialized)
        {
            cst0x6_resume();
        }
        GPIO_INTConfig(GPIO_GetPin(TOUCH_INT), ENABLE);
        GPIO_MaskINTConfig(GPIO_GetPin(TOUCH_INT), DISABLE);
    }
}

void disable_touch_sensor(void)
{
    touch_enable_flag = false;
    GPIO_INTConfig(GPIO_GetPin(TOUCH_INT), DISABLE);
    GPIO_MaskINTConfig(GPIO_GetPin(TOUCH_INT), ENABLE);
    if (touch_initialized)
    {
        cst0x6_suspend();
    }
}

void cst0x6_write(uint8_t reg, uint8_t data)
{
    uint8_t I2C_WriteBuf[2] = {0x0, 0x0};
    I2C_WriteBuf[0] = reg;
    I2C_WriteBuf[1] = data;
    I2C_SetSlaveAddress(TOUCH_I2C_BUS, TOUCH_SLAVE_ADDRESS);
    I2C_MasterWrite(TOUCH_I2C_BUS, I2C_WriteBuf, 2);
}

void cst0x6_read(uint8_t reg, uint8_t *p_data)
{
    uint8_t buf[1] = {reg};
    I2C_SetSlaveAddress(TOUCH_I2C_BUS, TOUCH_SLAVE_ADDRESS);
    I2C_MasterWrite(TOUCH_I2C_BUS, buf, 1);
    platform_delay_us(1);
    I2C_MasterRead(TOUCH_I2C_BUS, p_data, 1);
}

void cst0x6_read_key_value(void)
{
    uint8_t buf[1] = {0};
    uint8_t I2C_ReadBuf[64];
    I2C_SetSlaveAddress(TOUCH_I2C_BUS, TOUCH_SLAVE_ADDRESS);
    I2C_MasterWrite(TOUCH_I2C_BUS, buf, 1);
    platform_delay_us(1);
    I2C_MasterRead(TOUCH_I2C_BUS, I2C_ReadBuf, 24);
    uint8_t point_num = I2C_ReadBuf[2] & 0x0f;

    if (point_num == 0)
    {
        return;
    }

    for (uint8_t i = 0; i < point_num; i++)
    {
        /* use x,y low byte*/
#if 0
        cur_point.x = 240 - I2C_ReadBuf[3 + 6 * i + 1];
        cur_point.y = 240 - I2C_ReadBuf[3 + 6 * i + 3];
#else
        cur_point.x = I2C_ReadBuf[3 + 6 * i + 1];
        cur_point.y = I2C_ReadBuf[3 + 6 * i + 3];
#endif
        cur_point.is_press = true;
    }
    RtkTouchRawData_t In[1];
    In->x = cur_point.x;
    In->y = cur_point.y;
    In->counter = cur_point.t;
    In->rtc_counter = RTC_GetCounter();
    In->pressFlag = cur_point.is_press;
    RtkTouchProcess(In);

}

void cst0x6_get_chip_id(uint8_t *p_chip_id)
{
    cst0x6_read(FTS_REG_CHIP_ID, p_chip_id);
    APP_PRINT_INFO1("[cst0x6 get chip id] -- %X", *p_chip_id);
}

void cst0x6_resume(void)
{
#if 0
    Pad_Config(TOUCH_RST, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_LOW);
    os_delay(1);
    Pad_Config(TOUCH_RST, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);

    os_delay(30);
    APP_PRINT_INFO0("[cst0x6_resume]");
#endif
}

void cst0x6_suspend(void)
{
#if 0
    uint8_t sleep_cmd = CHIP_59_ENTER_SLEEP;
    if (chip_id != 0x59)
    {
        sleep_cmd = CHIP_OTHER_ENTER_SLEEP;
    }

    cst0x6_write(FTS_REG_LOW_POWER, sleep_cmd);
    APP_PRINT_INFO0("[cst0x6_suspend]");
#endif
}

void TOUCH_GESTURE_HANDLER(void)
{
    /*  Mask GPIO interrupt */
    GPIO_INTConfig(GPIO_GetPin(TOUCH_INT), DISABLE);
    GPIO_MaskINTConfig(GPIO_GetPin(TOUCH_INT), ENABLE);
    GPIO_ClearINTPendingBit(GPIO_GetPin(TOUCH_INT));

    cur_point.t++;
    if ((lcd_screen_updating_flag == false) && (cur_point.t > 2))
    {
        T_IO_MSG touch_msg;
        touch_msg.type = HUB_MSG_TOUCH;
        touch_msg.subtype = TOUCH_MSG_INT;
        send_msg_to_ui_task(&touch_msg, __LINE__);
    }
    os_timer_restart(&touch_gesture_release_timer, 30);

    GPIO_INTConfig(GPIO_GetPin(TOUCH_INT), ENABLE);
    GPIO_MaskINTConfig(GPIO_GetPin(TOUCH_INT), DISABLE);
    os_timer_restart(&allowed_enter_dlps_timer, ENTER_DLPS_TIMER_INTERVAL);
}

void touch_gesture_process_timeout(void)
{
    APP_PRINT_INFO1("touch gesture release timeout t=%d", cur_point.t);
    cur_point.is_press = false;
    RtkTouchRawData_t In[1];
    In->x = cur_point.x;
    In->y = cur_point.y;
    In->counter = cur_point.t;
    In->rtc_counter = RTC_GetCounter();
    In->pressFlag = cur_point.is_press;
    RtkTouchProcess(In);
    cur_point.t = 0;
}

static void touch_gesture_release_timeout(void *pxTimer)
{
    T_IO_MSG touch_msg;
    touch_msg.type = HUB_MSG_TOUCH;
    touch_msg.subtype = TOUCH_MSG_TIMEOUT;
    send_msg_to_ui_task(&touch_msg, __LINE__);

}

void touch_gesture_callback(RtkTouchInfo_t *info)
{
    pCurrentMenu->touch_func(info);
}


