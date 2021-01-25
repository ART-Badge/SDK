
/*********************************************************************************************************
*               Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file
* @brief
* @details
* @author
* @date
* @version
*********************************************************************************************************
*/

#include "board.h"
#include "trace.h"
#include "string.h"
#include "os_timer.h"
#include "os_mem.h"
#include "os_sync.h"
#include "hub_clock.h"
#include "hub_display.h"
#include "module_vibrate_motor.h"

#include "wristband_global_data.h"
#include "ancs.h"
#include "ancs_client.h"
#include "wristband_picture_res.h"
#include "module_gpio_button.h"
#include "wristband_gui.h"
#include "hub_task.h"
#include "gui_core.h"
#include "hub_button.h"
#include "hub_interaction.h"

PressingDef_t pressingParam;
bool allowed_button_enter_dlps = true;
static bool button_has_installed = false;

static const T_IO_MODULE_CBS button_cbs =
{
    wristband_button_driver_init,
    wristband_button_enter_dlps,
    wristband_button_exit_dlps,
    wristband_button_allowed_enter_dlps_check,
    wristband_button_system_wakeup_dlps_check,
};

void button_add_hub_task(void)
{
    wristband_button_driver_init();
    driver_modules[wristband_sensor_hub_count++] = (T_IO_MODULE_CBS *)&button_cbs;
    button_has_installed = true;
}

void wristband_button_driver_init(void)
{
    //call module init
    gpio_button_init();
}

void wristband_button_enter_dlps(void)
{
    //call module init
    gpio_button_enter_dlps();
}

void wristband_button_exit_dlps(void)
{
    //call module init
    gpio_button_exit_dlps();
}

bool wristband_button_allowed_enter_dlps_check(void)
{
    return allowed_button_enter_dlps;
}

bool wristband_button_system_wakeup_dlps_check(void)
{
    if (System_WakeUpInterruptValue(GPIO_BUTTON_KEY) == SET)
    {
        APP_PRINT_INFO0("Button WakeUp Int");
        Pad_ClearWakeupINTPendingBit(GPIO_BUTTON_KEY);
        allowed_button_enter_dlps = false;
        return true;
    }
    return false;
}

bool doNotificationCancelAction(void)
{
    if (button_has_installed == false)
    {
        return false;
    }
    if (RtkWristbandSys.flag_field.is_calling_action_alive)
    {
        //when calling is alive and motor is still vibrate
        if (RtkWristbandSys.phone_os_version == ANDROID)
        {
            APP_PRINT_INFO0("sending call reject command for Android");
            L1_send_event(L1SEND_RETURN_CALL_REJECT_COMMAND, NULL);
            wristband_system_interact(INTERACT_CALLING_REFUSE, 0, NULL);
            return false;
        }
        else if (RtkWristbandSys.phone_os_version == IOS)
        {
            APP_PRINT_INFO0("sending call reject command for ios");
            extern uint8_t  call_conn_id;
            extern uint32_t call_notification_uid;
            ancs_perform_notification_action(call_conn_id,
                                             call_notification_uid,
                                             CP_ACTION_ID_NEGATIVE);
            wristband_system_interact(INTERACT_CALLING_REFUSE, 0, NULL);
            return false;
        }
    }
    return true;
}

bool doNotificationAcceptAction(void)
{
    if (button_has_installed == false)
    {
        return false;
    }
    if (RtkWristbandSys.flag_field.is_calling_action_alive)
    {
        //when calling is alive and motor is still vibrate
        if (RtkWristbandSys.phone_os_version == ANDROID)
        {
            APP_PRINT_INFO0("sending call reject command for Android");
            //L1_send_event(L1SEND_CALL_REJECT_COMMAND, NULL);
            wristband_system_interact(INTERACT_CALLING_ACCEPT, 0, NULL);
            return false;
        }
        else if (RtkWristbandSys.phone_os_version == IOS)
        {
            APP_PRINT_INFO0("sending call reject command for ios");
            extern uint8_t  call_conn_id;
            extern uint32_t call_notification_uid;
            ancs_perform_notification_action(call_conn_id,
                                             call_notification_uid,
                                             CP_ACTION_ID_POSITIVE);
            wristband_system_interact(INTERACT_CALLING_ACCEPT, 0, NULL);
            return false;
        }
    }
    return true;
}

bool specialHandleForShortButton(uint8_t button_value)
{
    if (RtkWristbandSys.flag_field.power_off_flag == true)
    {
        if (RtkWristbandSys.charger_status == InCharging)
        {
            wristband_system_interact(INTERACT_CHARGESTATUS, 0, NULL);
        }
        return false;
    }
    if (button_value == LONG_BUTTON_0 && doNotificationCancelAction() == false)
    {
        return false;
    }
    if (button_value == SHORT_BUTTON_0 && doNotificationAcceptAction() == false)
    {
        return false;
    }
    if (button_value == SHORT_BUTTON_0 && motor_getstatus() == true)
    {
        wristband_system_interact(INTERACT_STOP_MOTOR_ONLY, 0, NULL);
    }
    return true;
}



void button_event_handler(T_IO_MSG msg)
{
    if (button_has_installed == false)
    {
        return;
    }
    uint8_t value = msg.subtype;
    if (specialHandleForShortButton(value) == false)
    {
        APP_PRINT_INFO1("specialHandleForShortButton line = %d",  __LINE__);
        return;
    }
    switch (value)
    {
    case PRESS_BUTTON_0:
        {
            if (pCurrentMenu != NULL)
            {
                pCurrentMenu->button_func(value);
            }
            break;
        }
    case RELEASE_BUTTON_0:
        {
            if (pCurrentMenu != NULL)
            {
                pCurrentMenu->button_func(value);
            }
            break;
        }
    case SHORT_BUTTON_0:
        {
            /* if LCD is not active, wake up screen */
            if ((wristbandMenuStatus.DisplayActiveFlag == false) || (pCurrentMenu == NULL))
            {
                pCurrentMenu = pInitMenu;
                pCurrentMenu->constructor_func(ICON_MIDDLE, NULL);
            }
            else
            {
                pCurrentMenu->button_func(value);
            }
            break;
        }
    case LONG_BUTTON_0:
        {
            if (RtkWristbandSys.flag_field.power_off_flag == true)
            {
                wristband_display_power_on();
                wristband_system_interact(INTERACT_LOGO, 0, NULL);
                APP_PRINT_INFO0("wristband poweron message handle!");
            }
            else
            {
                if (pCurrentMenu != NULL)
                {
                    /* if OLED is not active, can not ack long button*/
                    if (wristbandMenuStatus.DisplayActiveFlag == false)
                    {
                        return;
                    }
                    pCurrentMenu->button_func(value);
                }
            }
            break;
        }
    case PRESS_BUTTON_1:
        {
            T_HCI_MSG msg;
            msg.type = HCI_MSG_TYPE_8763BF;
            msg.subtype = BBPRO_HCI_SEND_EXIT_PAIRING_MODE;
            bbpro_hci_msg_queue_send(&msg);
            msg.subtype = BBPRO_HCI_SEND_POWER_OFF_PRESSED;
            bbpro_hci_msg_queue_send(&msg);
            msg.subtype = BBPRO_HCI_SEND_POWER_OFF_RELEASED;
            bbpro_hci_msg_queue_send(&msg);
            break;
        }
    case RELEASE_BUTTON_1:
        {
            T_HCI_MSG msg;
            msg.type = HCI_MSG_TYPE_8763BF;
            msg.subtype = BBPRO_HCI_SEND_POWER_ON_PRESSED;
            bbpro_hci_msg_queue_send(&msg);
            msg.subtype = BBPRO_HCI_SEND_POWER_ON_RELEASED;
            bbpro_hci_msg_queue_send(&msg);
            msg.subtype = BBPRO_HCI_SEND_ENTER_PAIRING_MODE;
            bbpro_hci_msg_queue_send(&msg);
            msg.subtype = BBPRO_HCI_LINK_LAST_DEVICE;
            bbpro_hci_msg_queue_send(&msg);
            break;
        }
    case SHORT_BUTTON_1:
        {
            break;
        }
    case LONG_BUTTON_1:
        {
            break;
        }
    default:
        {
            APP_PRINT_INFO2("file = %s, line = %d", TRACE_STRING(__FILE__), __LINE__);
            break;
        }
    }
}


