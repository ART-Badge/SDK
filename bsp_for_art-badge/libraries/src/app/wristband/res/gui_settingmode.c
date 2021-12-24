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
#include "module_font.h"
#include "hub_display.h"
#include "wristband_picture_res.h"
#include "wristband_global_data.h"
#include "math.h"
#include "hub_battery.h"
#include <string.h>
#include <stdarg.h>

#include "hub_interaction.h"
#include "wristband_gui.h"
#include "app_msg.h"
#include "app_task.h"
#include "touch_algorithm.h"
#include "gui_slide.h"
#include "gui_core.h"
#include "hub_task.h"

static void    *dynamic_timer = NULL;
static void    *display_timer = NULL;
static void    *page_switch_timer   = NULL;

static uint8_t bright_icon_num = 0;
static uint8_t mute_icon_num = 0;
static uint8_t shake_icon_num = 0;
static uint8_t time_icon_num = 0;

static void ButtonCallbackFun(uint8_t type);
static void TouchCallbackFun(RtkTouchInfo_t *info);
static void ConstructorCallbackFun(uint32_t type, void *pValue);
static void DestructorCallbackFun(void);
static void DisplayInfoCallbackFun(UI_MenuTypeDef *return_menu, void *argv);
static void TimerCallbackFun(RtkGuiTimerInfo_t *timer_info);
static void ResetIconTouchCb(uint32_t button_touch_type);
static void PowerOffIconTouchCb(uint32_t button_touch_type);
static void InformationIconTouchCb(uint32_t button_touch_type);
static void StopWatchIconTouchCb(uint32_t button_touch_type);
static void BrightIconTouchCb(uint32_t button_touch_type);
static void MuteIconTouchCb(uint32_t button_touch_type);
static void ShakeIconTouchCb(uint32_t button_touch_type);
static void TimeIconTouchCb(uint32_t button_touch_type);

static void updateDisplayTime(void)
{
    /*update display timer when short/long touch only*/
    os_timer_restart(&display_timer, 5000);
}

static void ResetIconTouchCb(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("[ResetButtonTest]-->short button test!!");
    }
    else if (button_touch_type == LONG_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("[ResetButtonTest]-->long button test!!");
    }
    updateDisplayTime();
}
static void PowerOffIconTouchCb(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("[PowerOffButtonTest]-->short button test!!");
    }
    else if (button_touch_type == LONG_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("[PowerOffButtonTest]-->long button test!!");
    }
    updateDisplayTime();
}
static void InformationIconTouchCb(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("[InformationButtonTest]-->short button test!!");
    }
    else if (button_touch_type == LONG_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("[InformationButtonTest]-->long button test!!");
    }
    updateDisplayTime();
}

static void StopWatchIconTouchCb(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("[StopWatchButtonTest]-->short button test!!");
    }
    else if (button_touch_type == LONG_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("[StopWatchButtonTest]-->long button test!!");
    }
    updateDisplayTime();
}
static void BrightIconTouchCb(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("[BrightIconTouchCb]-->short button test!!");
        bright_icon_num++;
        if (bright_icon_num > 2)
        {
            bright_icon_num = 0;
        }

        rtl_gui_menu_update(&SettingMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);
    }
    updateDisplayTime();
}

static void MuteIconTouchCb(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("[MuteIconTouchCb]-->short button test!!");
        mute_icon_num++;
        if (mute_icon_num > 1)
        {
            mute_icon_num = 0;
        }

        rtl_gui_menu_update(&SettingMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);
    }
    updateDisplayTime();
}
static void ShakeIconTouchCb(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("[ShakeIconTouchCb]-->short button test!!");
        shake_icon_num++;
        if (shake_icon_num > 2)
        {
            shake_icon_num = 0;
        }

        rtl_gui_menu_update(&SettingMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);
    }
    updateDisplayTime();
}



static void TimeIconTouchCb(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON)
    {
        time_icon_num++;
        if (time_icon_num > 1)
        {
            time_icon_num = 0;
        }

        rtl_gui_menu_update(&SettingMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);
    }
    updateDisplayTime();
}
#define SETTINGMODE_PAGE_NUM      9

const static UI_WidgetTypeDef  settingmode_icons[SETTINGMODE_PAGE_NUM] =
{
    [0]     = {.x = 0,    .y = 0,    .width = 240,    .hight = 240,    .addr = SETTING_BACKGROUND_BIN,    .widget_id_type = ICON_BACKGROUND},
    [1]     = {
        .x = 0,    .y = 19,    .width = 70,    .hight = 70,    .addr = SETTING_RESET_BIN,    .widget_id_type = ICON_WIDGET, \
        .touch_capability = true,  .widget_touch_func = ResetIconTouchCb
    },
    [2]     = {
        .x = 85,    .y = 19,    .width = 70,    .hight = 70,    .addr = SETTING_TIME_12_BIN,    .widget_id_type = ICON_WIDGET, \
        .touch_capability = true,  .widget_touch_func = TimeIconTouchCb
    },
    [3]     = {
        .x = 170,    .y = 19,    .width = 70,    .hight = 70,    .addr = SETTING_POWEROFF_BIN,    .widget_id_type = ICON_WIDGET, \
        .touch_capability = true,  .widget_touch_func = PowerOffIconTouchCb
    },
    [4]     = {
        .x = 0,    .y = 102,    .width = 70,    .hight = 70,    .addr = SETTING_INFO_BIN,    .widget_id_type = ICON_WIDGET, \
        .touch_capability = true,  .widget_touch_func = InformationIconTouchCb
    },
    [5]     = {
        .x = 85,    .y = 102,    .width = 70,    .hight = 70,    .addr = SETTING_STOPWATCH_I_BIN,    .widget_id_type = ICON_WIDGET, \
        .touch_capability = true,  .widget_touch_func = StopWatchIconTouchCb
    },
    [6]     = {
        .x = 170,    .y = 102,    .width = 70,    .hight = 70,    .addr = SETTING_MUTE_BIN,    .widget_id_type = ICON_WIDGET, \
        .touch_capability = true,  .widget_touch_func = MuteIconTouchCb
    },
    [7]     = {
        .x = 42,    .y = 175,    .width = 66,    .hight = 66,    .addr = SETTING_BRIGHT_1_BIN,    .widget_id_type = ICON_WIDGET, \
        .touch_capability = true,  .widget_touch_func = BrightIconTouchCb
    },
    [8]     = {
        .x = 133,    .y = 175,    .width = 66,    .hight = 66,    .addr = SETTING_SHAKE_1_BIN,    .widget_id_type = ICON_WIDGET, \
        .touch_capability = true,  .widget_touch_func = ShakeIconTouchCb
    },
};

UI_MenuTypeDef SettingMenu =
{
    .name = (const char *)"SettingMenu",
    .MenuParent = NULL,
    .MenuSub = NULL,
    .button_func = ButtonCallbackFun,
    .touch_func = TouchCallbackFun,
    .constructor_func = ConstructorCallbackFun,
    .destructor_func = DestructorCallbackFun,
    .display_info_func = DisplayInfoCallbackFun,
    .timer_func = TimerCallbackFun,
    .pWidgetList = (UI_WidgetTypeDef *)settingmode_icons,
    .current_max_icon = SETTINGMODE_PAGE_NUM,
};


static void TimerCallbackFun(RtkGuiTimerInfo_t *timer_info)
{
    uint32_t timer_id = timer_info->timer_id;
    if (memcmp(timer_info->menu_name,  SettingMenu.name, strlen(SettingMenu.name)))
    {
        return;
    }

    if ((timer_id == 1) && (display_timer != NULL))
    {
        APP_PRINT_INFO2("timer_id = %d, Name = %s", timer_id, TRACE_STRING(pCurrentMenu->name));
        wristband_display_clear(0, 0, LCD_WIDTH - 1, LCD_HIGHT - 1, BLACK);
        wristband_display_power_off();
        pCurrentMenu->destructor_func();
    }
    else if ((timer_id == 2) && (display_timer != NULL))
    {
#if 0
        APP_PRINT_INFO2("timer_id = %d, Name = %s", timer_id, TRACE_STRING(pCurrentMenu->name));
#endif
    }
    else if ((timer_id == 3) && (display_timer != NULL))
    {
        UI_MenuTypeDef *p_menu = rtl_gui_menu_real_slide();
        if (p_menu != NULL)
        {
            pCurrentMenu->MenuSub = p_menu;
            pCurrentMenu->destructor_func();
            pCurrentMenu = pCurrentMenu->MenuSub;
            pCurrentMenu->constructor_func(ICON_MIDDLE, NULL);
            os_timer_stop(&page_switch_timer);
        }
    }
}


static void ConstructorCallbackFun(uint32_t type, void *pValue)
{
    wristband_display_power_on();
    wristband_display_enable();
    pCurrentMenu->dynamicFlag = true;

    rtl_gui_menu_update(&SettingMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);

    os_timer_create(&display_timer, "display picture timer", 1, 5000, false, rtl_gui_timer_callback);
    os_timer_create(&dynamic_timer, "dynamic picture timer", 2, 500, true, rtl_gui_timer_callback);
    os_timer_create(&page_switch_timer, "page switch timer", 3, PAGE_SWITCH_TIMER_INTERVAL, true,
                    rtl_gui_timer_callback);
    os_timer_start(&dynamic_timer);
    os_timer_start(&display_timer);
}


static void DestructorCallbackFun(void)
{
    pCurrentMenu->dynamicFlag = false;
    if (dynamic_timer)
    {
        os_timer_stop(&dynamic_timer);
        os_timer_delete(&dynamic_timer);
    }
    if (display_timer)
    {
        os_timer_stop(&display_timer);
        os_timer_delete(&display_timer);
    }
    if (page_switch_timer)
    {
        os_timer_stop(&page_switch_timer);
        os_timer_delete(&page_switch_timer);
    }
    dynamic_timer = NULL;
    display_timer = NULL;
    page_switch_timer = NULL;
}
static void DisplayInfoCallbackFun(UI_MenuTypeDef *return_menu, void *argv)
{
    if (return_menu == NULL)
    {
        return;
    }
    return_menu->current_max_icon = SettingMenu.current_max_icon;
    /* do memcpy here */
    memcpy(return_menu->pWidgetList, SettingMenu.pWidgetList,
           SettingMenu.current_max_icon * sizeof(UI_WidgetTypeDef));
    //to do, set steps, distance, calorie, progress.
#if 1
    return_menu->pWidgetList[2].addr = SETTING_TIME_12_BIN + time_icon_num *
                                       return_menu->pWidgetList[2].hight * return_menu->pWidgetList[2].width * 2;
    return_menu->pWidgetList[6].addr = SETTING_MUTE_BIN + mute_icon_num *
                                       return_menu->pWidgetList[6].hight * return_menu->pWidgetList[6].width * 2;
    return_menu->pWidgetList[7].addr = SETTING_BRIGHT_1_BIN + bright_icon_num *
                                       return_menu->pWidgetList[7].hight * return_menu->pWidgetList[7].width * 2;
    return_menu->pWidgetList[8].addr = SETTING_SHAKE_1_BIN + shake_icon_num *
                                       return_menu->pWidgetList[8].hight * return_menu->pWidgetList[8].width * 2;
#endif
}
static void ButtonCallbackFun(uint8_t type)
{
    if (type == SHORT_BUTTON_0)
    {
        pCurrentMenu->MenuSub = &TempTest0;
        pCurrentMenu->destructor_func();
        pCurrentMenu = pCurrentMenu->MenuSub;
        pCurrentMenu->constructor_func(ICON_MIDDLE, NULL);
    }
    else if (type == LONG_BUTTON_0)
    {

    }
}

static void TouchCallbackFun(RtkTouchInfo_t *info)
{
#if 1
    uint32_t timer_state;
    os_timer_state_get(&page_switch_timer, &timer_state);
    if (timer_state)
    {
        return;
    }
#endif
    os_timer_restart(&display_timer, 5000);
    os_timer_restart(&dynamic_timer, 500);

    if (info->type == TOUCH_HOLD_Y)
    {
        rtl_gui_menu_update(&SettingMenu, NULL, NULL, NULL, &ClockSpecialMenu, NULL, 0, info->deltaY);
    }
    APP_PRINT_INFO3("info->type = %d, info->detalX = %d, info->detalY = %d", info->type, info->deltaX,
                    info->deltaY);

    if (info->type == TOUCH_ORIGIN_FROM_Y)
    {
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(&SettingMenu, NULL, NULL, NULL, &ClockSpecialMenu, NULL, 0, 0, info->deltaY, 0);
    }
    if (info->type == TOUCH_UP_SLIDE)
    {
        APP_PRINT_INFO0("===TOUCH_UP_SLIDE===");
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(&SettingMenu, NULL, NULL, NULL, &ClockSpecialMenu, NULL, 0, 0,
                           info->deltaY,
                           240);
    }
    if (info->type == TOUCH_DOWN_SLIDE)
    {
        APP_PRINT_INFO0("===TOUCH_DOWN_SLIDE===");
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(&SettingMenu, NULL, NULL, NULL, &ClockSpecialMenu, NULL, 0, 0,
                           info->deltaY,
                           0);
    }
}

