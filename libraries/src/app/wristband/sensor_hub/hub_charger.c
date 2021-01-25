#include "board.h"
#include "string.h"
#include "hub_charger.h"
#include "rtl876x_gpio.h"
#include "rtl876x_rcc.h"
#include "rtl876x_nvic.h"
#include "trace.h"
#include "os_timer.h"
#include "os_msg.h"
#include "app_msg.h"
#include "wristband_global_data.h"
#include "app_task.h"
#include "hub_battery.h"
#include "hub_task.h"


static  uint8_t ChargerCheckPinStatusINInt;
static  uint8_t USBCheckPinStatusINInt;
static void charger_timerout(void *pxTimer);
void *charger_debonce_timer = NULL ;
void *usb_debonce_timer = NULL ;
void *chargeMonitorTimer = NULL;
static bool charge_has_installed = false;

bool allowed_charge_enter_dlps = true;
static const T_IO_MODULE_CBS charge_cbs =
{
    wristband_charge_driver_init,
    wristband_charge_enter_dlps,
    wristband_charge_exit_dlps,
    wristband_charge_allowed_enter_dlps_check,
    wristband_charge_system_wakeup_dlps_check,
};

void charge_add_hub_task(void)
{
    charge_has_installed = true;
    wristband_charge_driver_init();
    driver_modules[wristband_sensor_hub_count++] = (T_IO_MODULE_CBS *)&charge_cbs;
}

void wristband_charge_driver_init(void)
{
    Pad_Config(CHARGER_INJECT_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE,
               PAD_OUT_LOW);
    Pad_Config(CHARGER_STATUS_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE,
               PAD_OUT_LOW);
    Pinmux_Config(CHARGER_INJECT_PIN, DWGPIO);
    Pinmux_Config(CHARGER_STATUS_PIN, DWGPIO);


    //create charger timer firstly
    os_timer_create(&charger_debonce_timer, "charger_debonce_timer", CHARGER_DEBOUNCE_TIMER_ID,
                    CHARGER_DEBOUNCE_TIMER_INTERVAL, false,
                    charger_timerout);
    os_timer_create(&usb_debonce_timer, "usb_debonce_timer", USB_DEBOUNCE_TIMTER_ID,
                    CHARGER_USB_DEBOUNCE_TIMTER_INTERVAL, false,
                    charger_timerout);

    //disable charger pin wake up function
    System_WakeUpPinDisable(CHARGER_STATUS_PIN);

    //disable usb check pin wake up function
    System_WakeUpPinDisable(CHARGER_INJECT_PIN);

    //charger pin init
    GPIO_InitTypeDef charger_Param;
    GPIO_StructInit(&charger_Param);
    RCC_PeriphClockCmd(APBPeriph_GPIO, APBPeriph_GPIO_CLOCK, ENABLE);
    charger_Param.GPIO_Pin = GPIO_GetPin(CHARGER_STATUS_PIN);
    charger_Param.GPIO_Mode = GPIO_Mode_IN;
    charger_Param.GPIO_ITPolarity = GPIO_INT_POLARITY_ACTIVE_LOW;
    charger_Param.GPIO_ITTrigger = GPIO_INT_Trigger_LEVEL;
    charger_Param.GPIO_ITCmd = ENABLE;
    GPIO_Init(&charger_Param);
    GPIO_INTConfig(GPIO_GetPin(CHARGER_STATUS_PIN), DISABLE);

    //USB check pin init
    GPIO_InitTypeDef usb_check_Param;
    GPIO_StructInit(&usb_check_Param);
    RCC_PeriphClockCmd(APBPeriph_GPIO, APBPeriph_GPIO_CLOCK, ENABLE);
    usb_check_Param.GPIO_Pin = GPIO_GetPin(CHARGER_INJECT_PIN);
    usb_check_Param.GPIO_Mode = GPIO_Mode_IN;
    usb_check_Param.GPIO_ITPolarity = GPIO_INT_POLARITY_ACTIVE_LOW;
    usb_check_Param.GPIO_ITTrigger = GPIO_INT_Trigger_LEVEL;
    usb_check_Param.GPIO_ITCmd = ENABLE;
    GPIO_Init(&usb_check_Param);
    GPIO_INTConfig(GPIO_GetPin(CHARGER_INJECT_PIN), DISABLE);



    if (GPIO_ReadInputDataBit(GPIO_GetPin(CHARGER_STATUS_PIN)))
    {
        GPIO->INTPOLARITY = GPIO->INTPOLARITY & (~(GPIO_GetPin(CHARGER_STATUS_PIN))); //LOW
        System_WakeUpPinEnable(CHARGER_STATUS_PIN, PAD_WAKEUP_POL_LOW, PAD_WK_DEBOUNCE_DISABLE);
    }
    else
    {
        GPIO->INTPOLARITY = (GPIO->INTPOLARITY | (GPIO_GetPin(CHARGER_STATUS_PIN))); //HIGH
        System_WakeUpPinEnable(CHARGER_STATUS_PIN, PAD_WAKEUP_POL_HIGH, PAD_WK_DEBOUNCE_DISABLE);
    }
    if (GPIO_ReadInputDataBit(GPIO_GetPin(CHARGER_INJECT_PIN)))
    {
        GPIO->INTPOLARITY = GPIO->INTPOLARITY & (~(GPIO_GetPin(CHARGER_INJECT_PIN))); //LOW
        System_WakeUpPinEnable(CHARGER_INJECT_PIN, PAD_WAKEUP_POL_LOW, PAD_WK_DEBOUNCE_DISABLE);
    }
    else
    {
        GPIO->INTPOLARITY = (GPIO->INTPOLARITY | (GPIO_GetPin(CHARGER_INJECT_PIN))); //HIGH
        System_WakeUpPinEnable(CHARGER_INJECT_PIN, PAD_WAKEUP_POL_HIGH, PAD_WK_DEBOUNCE_DISABLE);
    }
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = CHARGER_IRQ;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    NVIC_InitStruct.NVIC_IRQChannel = USB_CHECK_IRQ;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);


    GPIO_ClearINTPendingBit(GPIO_GetPin(CHARGER_STATUS_PIN));
    GPIO_ClearINTPendingBit(GPIO_GetPin(CHARGER_INJECT_PIN));
    GPIO_INTConfig(GPIO_GetPin(CHARGER_STATUS_PIN), ENABLE);
    GPIO_INTConfig(GPIO_GetPin(CHARGER_INJECT_PIN), ENABLE);

    if (GPIO_ReadInputDataBit(GPIO_GetPin(CHARGER_INJECT_PIN)))
    {
        if (GPIO_ReadInputDataBit(GPIO_GetPin(CHARGER_STATUS_PIN)))
        {
            APP_PRINT_INFO0("Init charge FULL...!");
            RtkWristbandSys.charger_status = ChargingComplete;
        }
        else
        {
            APP_PRINT_INFO0("Init charging...!");
            RtkWristbandSys.charger_status = InCharging;
        }
    }
    else
    {
        APP_PRINT_INFO0("Init charger OFF...!");
        RtkWristbandSys.charger_status = NoCharge;
    }
}

void wristband_charge_enter_dlps(void)
{
    if (GPIO_ReadInputDataBit(GPIO_GetPin(CHARGER_INJECT_PIN)))
    {
        GPIO->INTPOLARITY = GPIO->INTPOLARITY & (~(GPIO_GetPin(CHARGER_INJECT_PIN))); //LOW
        System_WakeUpPinEnable(CHARGER_INJECT_PIN, PAD_WAKEUP_POL_LOW, PAD_WK_DEBOUNCE_DISABLE);
    }
    else
    {
        GPIO->INTPOLARITY = (GPIO->INTPOLARITY | (GPIO_GetPin(CHARGER_INJECT_PIN))); //HIGH
        System_WakeUpPinEnable(CHARGER_INJECT_PIN, PAD_WAKEUP_POL_HIGH, PAD_WK_DEBOUNCE_DISABLE);
    }
    if (GPIO_ReadInputDataBit(GPIO_GetPin(CHARGER_STATUS_PIN)))
    {
        GPIO->INTPOLARITY = GPIO->INTPOLARITY & (~(GPIO_GetPin(CHARGER_STATUS_PIN))); //LOW
        System_WakeUpPinEnable(CHARGER_STATUS_PIN, PAD_WAKEUP_POL_LOW, PAD_WK_DEBOUNCE_DISABLE);
    }
    else
    {
        GPIO->INTPOLARITY = (GPIO->INTPOLARITY | (GPIO_GetPin(CHARGER_STATUS_PIN))); //HIGH
        System_WakeUpPinEnable(CHARGER_STATUS_PIN, PAD_WAKEUP_POL_HIGH, PAD_WK_DEBOUNCE_DISABLE);
    }

    Pad_Config(CHARGER_INJECT_PIN, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE,
               PAD_OUT_LOW);
    Pad_Config(CHARGER_STATUS_PIN, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE,
               PAD_OUT_LOW);
}
void wristband_charge_exit_dlps(void)
{
    Pad_Config(CHARGER_INJECT_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE,
               PAD_OUT_LOW);
    Pad_Config(CHARGER_STATUS_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE,
               PAD_OUT_LOW);
}

bool wristband_charge_allowed_enter_dlps_check(void)
{
    return allowed_charge_enter_dlps;
}

bool wristband_charge_system_wakeup_dlps_check(void)
{
    if (System_WakeUpInterruptValue(CHARGER_INJECT_PIN) == SET)
    {
        APP_PRINT_INFO0("Changer Inject WakeUp Int");
        Pad_ClearWakeupINTPendingBit(CHARGER_INJECT_PIN);
//        allowed_charge_enter_dlps = false;
        return true;
    }
    return false;
}

static void charger_timerout(void *pxTimer)
{
    uint32_t charger_timer_id = 0;
    if (os_timer_id_get(&pxTimer, &charger_timer_id) == false)
    {
        APP_PRINT_INFO0("charge pin or detect pin debounce time out!");
    }

    if (CHARGER_DEBOUNCE_TIMER_ID == charger_timer_id)
    {
        if (ChargerCheckPinStatusINInt != GPIO_ReadInputDataBit(GPIO_GetPin(CHARGER_STATUS_PIN)))
        {
            APP_PRINT_INFO0("chareger pin debounce fail!");
            GPIO_ClearINTPendingBit(GPIO_GetPin(CHARGER_STATUS_PIN));
            GPIO_INTConfig(GPIO_GetPin(CHARGER_STATUS_PIN), ENABLE);
            GPIO_MaskINTConfig(GPIO_GetPin(CHARGER_STATUS_PIN), DISABLE);
            return;
        }
    }

    if (USB_DEBOUNCE_TIMTER_ID == charger_timer_id)
    {
        if (USBCheckPinStatusINInt != GPIO_ReadInputDataBit(GPIO_GetPin(CHARGER_INJECT_PIN)))
        {
            APP_PRINT_INFO0("usb pin debounce fail!");
            GPIO_ClearINTPendingBit(GPIO_GetPin(CHARGER_INJECT_PIN));
            GPIO_INTConfig(GPIO_GetPin(CHARGER_INJECT_PIN), ENABLE);     //disable usb check pin int
            GPIO_MaskINTConfig(GPIO_GetPin(CHARGER_INJECT_PIN), DISABLE);
            return;
        }
    }

    if (GPIO_ReadInputDataBit(GPIO_GetPin(CHARGER_STATUS_PIN)))
    {
        GPIO->INTPOLARITY = GPIO->INTPOLARITY & (~(GPIO_GetPin(CHARGER_STATUS_PIN))); //LOW
        System_WakeUpPinEnable(CHARGER_STATUS_PIN, PAD_WAKEUP_POL_LOW, PAD_WK_DEBOUNCE_DISABLE);
    }
    else
    {
        GPIO->INTPOLARITY = (GPIO->INTPOLARITY | (GPIO_GetPin(CHARGER_STATUS_PIN))); //HIGH
        System_WakeUpPinEnable(CHARGER_STATUS_PIN, PAD_WAKEUP_POL_HIGH, PAD_WK_DEBOUNCE_DISABLE);
    }
    if (GPIO_ReadInputDataBit(GPIO_GetPin(CHARGER_INJECT_PIN)))
    {
        GPIO->INTPOLARITY = GPIO->INTPOLARITY & (~(GPIO_GetPin(CHARGER_INJECT_PIN))); //LOW
        System_WakeUpPinEnable(CHARGER_INJECT_PIN, PAD_WAKEUP_POL_LOW, PAD_WK_DEBOUNCE_DISABLE);
    }
    else
    {
        GPIO->INTPOLARITY = (GPIO->INTPOLARITY | (GPIO_GetPin(CHARGER_INJECT_PIN))); //HIGH
        System_WakeUpPinEnable(CHARGER_INJECT_PIN, PAD_WAKEUP_POL_HIGH, PAD_WK_DEBOUNCE_DISABLE);
    }

    bool newstate = false;

    if (GPIO_ReadInputDataBit(GPIO_GetPin(CHARGER_INJECT_PIN)))
    {
        if (GPIO_ReadInputDataBit(GPIO_GetPin(CHARGER_STATUS_PIN)))
        {
            APP_PRINT_INFO0("charger FULL...!");
            if (RtkWristbandSys.charger_status != ChargingComplete)
            {
                RtkWristbandSys.charger_status = ChargingComplete;
                newstate = true;
            }
        }
        else
        {
            APP_PRINT_INFO0("In charging...!");
            if (RtkWristbandSys.charger_status != InCharging)
            {
                RtkWristbandSys.charger_status = InCharging;
                newstate = true;
            }
            if (RtkWristbandSys.flag_field.power_off_flag == true)
            {
                T_IO_MSG msg;
                msg.type = HUB_MSG_BATTERY_ADC;
                msg.subtype = BATTERY_MSG_POWER_ON;
                send_msg_to_ui_task(&msg, __LINE__);
            }
        }
    }
    else
    {
        APP_PRINT_INFO0("charger OFF...!");
        if (RtkWristbandSys.charger_status != NoCharge)
        {
            RtkWristbandSys.charger_status = NoCharge;
            newstate = true;
        }
    }

    GPIO_ClearINTPendingBit(GPIO_GetPin(CHARGER_STATUS_PIN));
    GPIO_MaskINTConfig(GPIO_GetPin(CHARGER_STATUS_PIN), DISABLE);
    GPIO_INTConfig(GPIO_GetPin(CHARGER_STATUS_PIN), ENABLE);
    GPIO_ClearINTPendingBit(GPIO_GetPin(CHARGER_INJECT_PIN));
    GPIO_MaskINTConfig(GPIO_GetPin(CHARGER_INJECT_PIN), DISABLE);
    GPIO_INTConfig(GPIO_GetPin(CHARGER_INJECT_PIN), ENABLE);

    if (newstate)
    {
        L1_send_event(L1SEND_RETURN_CHARGE_STATUS, NULL);
        T_IO_MSG button_msg;
        button_msg.type = HUB_MSG_BUTTON;
        button_msg.subtype = SHORT_BUTTON_0;
        send_msg_to_ui_task(&button_msg, __LINE__);
    }

}



void ChargerIntrHandler(void)
{
    GPIO_INTConfig(GPIO_GetPin(CHARGER_STATUS_PIN), DISABLE);
    GPIO_MaskINTConfig(GPIO_GetPin(CHARGER_STATUS_PIN), ENABLE);
    GPIO_ClearINTPendingBit(GPIO_GetPin(CHARGER_STATUS_PIN));

    APP_PRINT_INFO0("charger pin interrupt...!");
    //read charger pin bit
    ChargerCheckPinStatusINInt = GPIO_ReadInputDataBit(GPIO_GetPin(CHARGER_STATUS_PIN));
    if (charger_debonce_timer != NULL)
    {
        APP_PRINT_INFO0("start charger_debonce_timer");
        //start charger pin debounce timer
        os_timer_start(&charger_debonce_timer);
    }
}

void UsbCheckHandler(void)
{
    GPIO_INTConfig(GPIO_GetPin(CHARGER_INJECT_PIN), DISABLE);
    GPIO_MaskINTConfig(GPIO_GetPin(CHARGER_INJECT_PIN), ENABLE);
    GPIO_ClearINTPendingBit(GPIO_GetPin(CHARGER_INJECT_PIN));

    APP_PRINT_INFO0("usb pin interrupt...!");

    USBCheckPinStatusINInt = GPIO_ReadInputDataBit(GPIO_GetPin(CHARGER_INJECT_PIN));

    //read charger pin bit
    if (usb_debonce_timer != NULL)
    {
        APP_PRINT_INFO0("start inject_debonce_timer");
        //start charger pin debounce timer
        os_timer_start(&usb_debonce_timer);
    }
}

void charge_event_handler(T_IO_MSG msg)
{
    if (charge_has_installed == false)
    {
        return;
    }
    switch (msg.subtype)
    {
    case CHARGE_MSG_WAKEUP:
        {
            break;
        }
    default:
        break;
    }
}





