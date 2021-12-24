/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_module_gpio_button.c
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
#include "hub_task.h"
#include "ancs_client.h"
#include "ancs.h"
#include "rtl876x_pinmux.h"
#include "hub_clock.h"
#include "health_algo.h"
#include "module_gpio_button.h"
#include "wristband_global_data.h"


#define ENTER_DLPS_TIMER_INTERVAL           500


static void     *longpress_timer = NULL;
static void     *debounce_timer = NULL;
static void     *pressing_timer = NULL;
static void     *allowed_enter_dlps_timer = NULL;
static bool     KeyCurrentStataus = SET;
extern bool    allowed_button_enter_dlps;




static void longpress_timeout(void *pxTimer)
{
    APP_PRINT_INFO0("wristband long button callback!");
    T_IO_MSG button_msg;
    button_msg.type = HUB_MSG_BUTTON;
    button_msg.subtype = LONG_BUTTON_0;
    send_msg_to_ui_task(&button_msg, __LINE__);
}
static void pressing_timer_handle(void *pxTimer)
{
    T_IO_MSG pressing_msg;
    pressing_msg.type = HUB_MSG_BUTTON;
    pressing_msg.subtype = PRESSING_BUTTON_0;
    if (send_msg_to_ui_task(&pressing_msg, __LINE__) == false)
    {
        APP_PRINT_ERROR0("send pressing_timer_msg to io queue fail !\n");
    }
}
static void debounce_timeout(void *pxTimer)
{
    static uint32_t pressed_time = 0;
    static uint32_t released_time = 0;
    if (KeyCurrentStataus != GPIO_ReadInputDataBit(GPIO_GetPin(GPIO_BUTTON_KEY)))
    {
        GPIO_MaskINTConfig(GPIO_GetPin(GPIO_BUTTON_KEY), DISABLE);
        GPIO_INTConfig(GPIO_GetPin(GPIO_BUTTON_KEY), ENABLE);
        APP_PRINT_ERROR0("GPIO button debounce Fail!");
        return;
    }
    /* this means button released */
    if (KeyCurrentStataus == SET)
    {
        /* Change GPIO Interrupt Polarity, Enable Interrupt */
        GPIO->INTPOLARITY &= ~ GPIO_GetPin(GPIO_BUTTON_KEY);
        GPIO_MaskINTConfig(GPIO_GetPin(GPIO_BUTTON_KEY), DISABLE);
        GPIO_INTConfig(GPIO_GetPin(GPIO_BUTTON_KEY), ENABLE);

        released_time = os_sys_time_get();
        os_timer_stop(&longpress_timer);
        uint32_t detal_time = released_time - pressed_time;
        if (detal_time < 2000)
        {
            T_IO_MSG button_msg;
            button_msg.type = HUB_MSG_BUTTON;
            button_msg.subtype = SHORT_BUTTON_0;
            send_msg_to_ui_task(&button_msg, __LINE__);
        }
        T_IO_MSG button_msg;
        button_msg.type = HUB_MSG_BUTTON;
        button_msg.subtype = RELEASE_BUTTON_0;
        send_msg_to_ui_task(&button_msg, __LINE__);
    }
    /* this means button pressed */
    else if (KeyCurrentStataus == RESET)
    {
        /* Change GPIO Interrupt Polarity, Enable Interrupt */
        GPIO->INTPOLARITY |= GPIO_GetPin(GPIO_BUTTON_KEY);
        GPIO_MaskINTConfig(GPIO_GetPin(GPIO_BUTTON_KEY), DISABLE);
        GPIO_INTConfig(GPIO_GetPin(GPIO_BUTTON_KEY), ENABLE);

        pressed_time = os_sys_time_get();
        os_timer_start(&longpress_timer);
        APP_PRINT_INFO0("power button pressed!");
        T_IO_MSG button_msg;
        button_msg.type = HUB_MSG_BUTTON;
        button_msg.subtype = PRESS_BUTTON_0;
        send_msg_to_ui_task(&button_msg, __LINE__);

    }
}
static void allowed_enter_dlps_callback(void *pxTimer)
{
    allowed_button_enter_dlps = true;
}


void gpio_button_pad_init(void)
{
    Pinmux_Config(GPIO_BUTTON_KEY, DWGPIO);
    Pad_Config(GPIO_BUTTON_KEY, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE,
               PAD_OUT_LOW);
}
void gpio_button_enter_dlps(void)
{
    Pad_Config(GPIO_BUTTON_KEY, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
}
void gpio_button_exit_dlps(void)
{
    Pad_Config(GPIO_BUTTON_KEY, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE,
               PAD_OUT_LOW);
}
/**
  * @brief  Initialize GPIO as input pin
  * @param  short button callback
  * @param  long  button callback
  *
  * @retval None
  */
void gpio_button_init(void)
{
    gpio_button_pad_init();
    RCC_PeriphClockCmd(APBPeriph_GPIO,  APBPeriph_GPIO_CLOCK,  ENABLE);
    GPIO_InitTypeDef GPIO_Param;
    GPIO_StructInit(&GPIO_Param);
    GPIO_Param.GPIO_Pin = GPIO_GetPin(GPIO_BUTTON_KEY);
    GPIO_Param.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Param.GPIO_ITCmd = ENABLE;
    GPIO_Param.GPIO_ITTrigger = GPIO_INT_Trigger_LEVEL;
    GPIO_Param.GPIO_ITPolarity = GPIO_INT_POLARITY_ACTIVE_LOW;
    GPIO_Init(&GPIO_Param);

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = GPIO_BUTTON_IRQ;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    os_timer_create(&longpress_timer, "long press timer", 1, 2000, false, longpress_timeout);
    os_timer_create(&debounce_timer, "debounce timer", 1, 60, false, debounce_timeout);
    os_timer_create(&pressing_timer, "pressing_timer", 3, 100, true, pressing_timer_handle);

    GPIO_INTConfig(GPIO_GetPin(GPIO_BUTTON_KEY), ENABLE);
    GPIO_MaskINTConfig(GPIO_GetPin(GPIO_BUTTON_KEY), DISABLE);
    System_WakeUpPinEnable(GPIO_BUTTON_KEY, PAD_WAKEUP_POL_LOW, PAD_WK_DEBOUNCE_DISABLE);
    os_timer_create(&allowed_enter_dlps_timer, "allowed enter dlps", 1, ENTER_DLPS_TIMER_INTERVAL,
                    false, allowed_enter_dlps_callback);

}

void GPIO_BUTTON_HANDLER(void)
{
    GPIO_INTConfig(GPIO_GetPin(GPIO_BUTTON_KEY), DISABLE);
    GPIO_MaskINTConfig(GPIO_GetPin(GPIO_BUTTON_KEY), ENABLE);
    GPIO_ClearINTPendingBit(GPIO_GetPin(GPIO_BUTTON_KEY));

    KeyCurrentStataus = GPIO_ReadInputDataBit(GPIO_GetPin(GPIO_BUTTON_KEY));
    APP_PRINT_INFO1("[Key Button] KeyCurrentStataus = %d", KeyCurrentStataus);
    if (os_timer_start(&debounce_timer) == false)
    {
        GPIO_INTConfig(GPIO_GetPin(GPIO_BUTTON_KEY), ENABLE);
        GPIO_MaskINTConfig(GPIO_GetPin(GPIO_BUTTON_KEY), DISABLE);
    }
    os_timer_restart(&allowed_enter_dlps_timer, ENTER_DLPS_TIMER_INTERVAL);
}


