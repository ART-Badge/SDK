
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
#include "gui_core.h"
#include "wristband_global_data.h"
#include "ancs.h"
#include "ancs_client.h"
#include "wristband_picture_res.h"
#include "module_touch.h"
#include "wristband_gui.h"
#include "hub_task.h"
#include "hub_button.h"
#include "hub_interaction.h"

extern bool display_has_installed;
T_IO_HUB_INTERACTION system_interact_param;
void wristband_system_interact(INTERACT_Type type, uint8_t frame_num, void *pValue)
{
    T_IO_MSG hub_interact_msg;
    hub_interact_msg.type = HUB_MSG_LCD;
    hub_interact_msg.subtype = LCD_MSG_INTERACT;
    system_interact_param.type = type;
    system_interact_param.frame_num = frame_num;
    system_interact_param.pValue = pValue;
    hub_interact_msg.u.buf = &system_interact_param;
    send_msg_to_ui_task(&hub_interact_msg, __LINE__);
}
void hub_system_interact(INTERACT_Type type, uint8_t frame_num, void *pValue)
{
    if (display_has_installed == false)
    {
        return;
    }
    if ((pCurrentMenu != NULL) && (pCurrentMenu->destructor_func != NULL))
    {
        pCurrentMenu->destructor_func();
    }
    APP_PRINT_INFO1("wristband_system_interact type = %d", type);
    switch (type)
    {
    case INTERACT_LOGO:
        {
            pCurrentMenu = &ClockMenu;
            pCurrentMenu->constructor_func(ICON_MIDDLE, pValue);
            break;
        }
    case INTERACT_LOGIN:
        {
            pCurrentMenu = &ClockMenu;
            pCurrentMenu->constructor_func(ICON_MIDDLE, pValue);
            break;
        }
    case INTERACT_CHARGESTATUS:
        {
            pCurrentMenu = &ClockMenu;
            pCurrentMenu->constructor_func(ICON_MIDDLE, pValue);
            break;
        }
    case INTERACT_ALARM:
        {
            pCurrentMenu = &ClockMenu;
            pCurrentMenu->constructor_func(ICON_MIDDLE, pValue);
            break;
        }
    case INTERACT_CANCEL_BOND:
        {
            pCurrentMenu = &ClockMenu;
            pCurrentMenu->constructor_func(ICON_MIDDLE, pValue);
            break;
        }
    case INTERACT_BONDED:
        {
            pCurrentMenu = &ClockMenu;
            pCurrentMenu->constructor_func(ICON_MIDDLE, pValue);
            break;
        }
    case INTERACT_FIND_ME:
        {
            pCurrentMenu = &ClockMenu;
            pCurrentMenu->constructor_func(ICON_MIDDLE, pValue);
            break;
        }
    case INTERACT_PHONEMESSAGE:
        {
            pCurrentMenu = &MessageMenu;
            pCurrentMenu->constructor_func(ICON_MIDDLE, pValue);
            break;
        }
    case INTERACT_QQ:
        {
            pCurrentMenu = &MessageMenu;
            pCurrentMenu->constructor_func(ICON_MIDDLE, pValue);
            break;
        }
    case INTERACT_WECHAT:
        {
            pCurrentMenu = &MessageMenu;
            pCurrentMenu->constructor_func(ICON_MIDDLE, pValue);
            break;
        }
    case INTERACT_CALLING:
        {
            RtkWristbandSys.flag_field.is_calling_action_alive = true;
            pCurrentMenu = &ClockMenu;
            pCurrentMenu->constructor_func(ICON_MIDDLE, pValue);
            break;
        }
    case INTERACT_CALLING_ACCEPT:
        {
            if (RtkWristbandSys.flag_field.is_calling_action_alive == true)
            {
                RtkWristbandSys.flag_field.is_calling_action_alive = false;
                wristband_system_interact(INTERACT_STOP_OLED_ONLY, 0, NULL);
            }
            break;
        }
    case INTERACT_CALLING_REFUSE:
        {
            if (RtkWristbandSys.flag_field.is_calling_action_alive == true)
            {
                RtkWristbandSys.flag_field.is_calling_action_alive = false;
                wristband_system_interact(INTERACT_STOP_OLED_ONLY, 0, NULL);
            }
            break;
        }
    case INTERACT_TARGET:
        {
            pCurrentMenu = &ClockMenu;
            pCurrentMenu->constructor_func(ICON_MIDDLE, pValue);
            break;
        }
    case INTERACT_NO_MOVEMENT:
        {
            pCurrentMenu = &ClockMenu;
            pCurrentMenu->constructor_func(ICON_MIDDLE, pValue);
            break;
        }
    case INTERACT_STOP_OLED_ONLY:
        {
            wristband_display_clear(0, 0, LCD_WIDTH - 1, LCD_WIDTH - 1, BLACK);
            wristband_display_power_off();
            pCurrentMenu->destructor_func();
            break;
        }
    default:
        break;
    }
}

