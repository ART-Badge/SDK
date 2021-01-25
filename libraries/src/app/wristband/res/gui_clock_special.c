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
#include "hub_battery.h"
#include <stdarg.h>
#include "hub_interaction.h"
#include "wristband_gui.h"
#include "app_msg.h"
#include "app_task.h"
#include "arm_math.h"
#include "rtl8763bf_task.h"
#include "touch_algorithm.h"
#include "gui_slide.h"
#include "gui_core.h"
#include "stdio.h"
#include "hub_task.h"
#include "gui_common_menu.h"


static void    *dynamic_timer = NULL;
static void    *display_timer = NULL;
static void    *page_switch_timer   = NULL;

static void ButtonCallbackFun(uint8_t type);
static void TouchCallbackFun(RtkTouchInfo_t *info);
static void ConstructorCallbackFun(uint32_t type, void *pValue);
static void DestructorCallbackFun(void);
static void DisplayInfoCallbackFun(UI_MenuTypeDef *return_menu, void *argv);
static void TimerCallbackFun(RtkGuiTimerInfo_t *timer_info);


static void CurrentMenuButtonTest(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("[CurrentMenuButtonTest]-->short button test!!");
    }
    else if (button_touch_type == LONG_TOUCH_BUTTON)
    {
        /* change Here for UI */
        pCurrentMenu->MenuSub = &WatchFaceAWMenu;
        pCurrentMenu->destructor_func();
        pCurrentMenu = pCurrentMenu->MenuSub;
        pCurrentMenu->constructor_func(ICON_MIDDLE, NULL);
        APP_PRINT_INFO0("[CurrentMenuButtonTest]-->long button test!!");
    }
}



const static UI_WidgetTypeDef  icon[] =
{
    [0]     = {
        .x = 0,   .y = 0,    .width = LCD_WIDTH, .hight = LCD_HIGHT,     .addr = CLOCK_SPECIAL_BG_BIN,   .widget_id_type = ICON_BACKGROUND, \
        .touch_capability = true,  .widget_touch_func = CurrentMenuButtonTest
    },
    [1]     = {
        .x = 114,   .y = 17,    .width = 26, .hight = 12,     .addr = COMMON_WEEK_WHITE_BIN,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = false
    },
    [2]     = {
        .x = 146,   .y = 17,    .width = 39, .hight = 20,     .addr = COMMON_MONTH_BIN,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = false
    },
    [3]     = {
        .x = 194,   .y = 17,    .width = 10, .hight = 19,     .addr = COMMON_P10X19WHITE_BIN,   .widget_id_type = ICON_NUM, \
        .touch_capability = false
    },
    [4]     = {//for bt connnect/disconnect icon
        .x = 0,   .y = 47,    .width = 22, .hight = 14,     .addr = COMMON_CONNECT_BIN,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = false
    },
    [5]     = {//for battery
        .x = 0,   .y = 70,    .width = 15, .hight = 8,     .addr = CLOCK_SPECIAL_BATTERY_4_BIN,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = false
    },
    [6]     = {//for am or pm
        .x = 0,   .y = 94,    .width = 16, .hight = 11,     .addr = CLOCK_SPECIAL_PM_BIN,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = false
    },
    [7]     = {//for clock hour
        .x = 45,   .y = 47,    .width = 44, .hight = 80,     .addr = CLOCK_SPECIAL_NUM_44_80__BIN,   .widget_id_type = ICON_NUM, \
        .touch_capability = false
    },
    [8]     = {//for clock dot first
        .x = 136,   .y = 67,    .width = 11, .hight = 12,     .addr = CLOCK_SPECIAL_CLOCKDOT_BIN,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = false
    },
    [9]     = {//for clock dot second
        .x = 136,   .y = 108,    .width = 11, .hight = 12,     .addr = CLOCK_SPECIAL_CLOCKDOT_BIN,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = false
    },
    [10]     = {//for clock minute
        .x = 151,   .y = 47,    .width = 44, .hight = 80,     .addr = CLOCK_SPECIAL_NUM_44_80__BIN,   .widget_id_type = ICON_NUM, \
        .touch_capability = false
    },


    [11]     = {//for step
        .x = 109,   .y = 143,    .width = 27, .hight = 18,     .addr = COMMON_STEP_BIN,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = false
    },
    [12]     = {//for step num string
        .x = 146,   .y = 143,    .width = 10, .hight = 19,     .addr = COMMON_P10X19WHITE_BIN,   .widget_id_type = ICON_NUM, \
        .touch_capability = false
    },
    [13]     = {//for kcal
        .x = 109,   .y = 169,    .width = 24, .hight = 15,     .addr = COMMON_KCAL_BIN,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = false
    },
    [14]     = {//for kcal num string
        .x = 146,   .y = 169,    .width = 10, .hight = 19,     .addr = COMMON_P10X19WHITE_BIN,   .widget_id_type = ICON_NUM, \
        .touch_capability = false
    },
    [15]     = {//for km
        .x = 109,   .y = 194,    .width = 17, .hight = 15,     .addr = COMMON_KM_BIN,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = false
    },
    [16]     = {//for km num string
        .x = 146,   .y = 194,    .width = 10, .hight = 19,     .addr = COMMON_P10X19WHITE_BIN,   .widget_id_type = ICON_NUM, \
        .touch_capability = false
    },
    [17]     = {//for bpm
        .x = 109,   .y = 220,    .width = 27, .hight = 19,     .addr = COMMON_BMP_BIN,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = false
    },
    [18]     = {//for bpm num string
        .x = 146,   .y = 220,    .width = 10, .hight = 19,     .addr = COMMON_P10X19RED_BIN,   .widget_id_type = ICON_NUM, \
        .touch_capability = false
    },
};

/* change Here for UI */
UI_MenuTypeDef ClockSpecialMenu =
{
    /* change Here for UI */
    .name = (const char *)"ClockSpecialMenu",
    .MenuParent = NULL,
    .MenuSub = NULL,
    .button_func = ButtonCallbackFun,
    .touch_func = TouchCallbackFun,
    .constructor_func = ConstructorCallbackFun,
    .destructor_func = DestructorCallbackFun,
    .display_info_func = DisplayInfoCallbackFun,
    .timer_func = TimerCallbackFun,
    .pWidgetList = (UI_WidgetTypeDef *)icon,
    .current_max_icon = sizeof(icon) / sizeof(UI_WidgetTypeDef),
};


static void TimerCallbackFun(RtkGuiTimerInfo_t *timer_info)
{
    uint32_t timer_id = timer_info->timer_id;
    if (memcmp(timer_info->menu_name,  ClockSpecialMenu.name, strlen(ClockSpecialMenu.name)))
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
        os_timer_start(&dynamic_timer);
        if (pCurrentMenu->dynamicFlag == true)
        {
            rtl_gui_menu_update(&ClockSpecialMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);
        }
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

    rtl_gui_menu_update(&ClockSpecialMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);

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
    APP_PRINT_INFO2("file = %s, line = %d", TRACE_STRING(__FILE__), __LINE__);
}



static void DisplayInfoCallbackFun(UI_MenuTypeDef *return_menu, void *argv)
{
    if (return_menu == NULL)
    {
        return;
    }

    UI_MenuTypeDef *ui_local_cur = &ClockSpecialMenu;
    return_menu->current_max_icon = ui_local_cur->current_max_icon;
    /* do memcpy here */
    memcpy(return_menu->pWidgetList, ui_local_cur->pWidgetList,
           ui_local_cur->current_max_icon * sizeof(UI_WidgetTypeDef));

    T_UTC_TIME display_utc_time;
    uint32_t display_total_second = 0;
    DAY_OF_WEEK day_of_week;
    if (RtkWristbandSys.flag_field.bond_state == true)
    {
        display_total_second = RtkWristbandSys.SecondCountRTC;
        display_utc_time.year = RtkWristbandSys.Global_Time.year;
        display_utc_time.month = RtkWristbandSys.Global_Time.month;
        display_utc_time.day = RtkWristbandSys.Global_Time.day;
        display_utc_time.hour = RtkWristbandSys.Global_Time.hour;
        display_utc_time.minutes = RtkWristbandSys.Global_Time.minutes;
        display_utc_time.seconds = get_system_clock_second();
    }
    else
    {
        display_total_second = 0;
        display_utc_time.year = 2000;
        display_utc_time.month = 1;
        display_utc_time.day = 1;
        display_utc_time.hour = 0;
        display_utc_time.minutes = 0;
        display_utc_time.seconds = 0;
    }
    day_of_week = get_day_of_week(display_total_second); // day of week
    return_menu->pWidgetList[1].addr = COMMON_WEEK_WHITE_BIN + \
                                       (day_of_week) * 2 * (return_menu->pWidgetList[1].width) *
                                       (return_menu->pWidgetList[1].hight);
    return_menu->pWidgetList[2].addr = COMMON_MONTH_BIN + \
                                       (RtkWristbandSys.Global_Time.month - 1) * 2 * (return_menu->pWidgetList[2].width) *
                                       (return_menu->pWidgetList[2].hight);

    //for utc day string
    static char clock_special_utc_time_day_string[4] = {'\0'};
    memset(clock_special_utc_time_day_string, '\0', sizeof(clock_special_utc_time_day_string));
    sprintf(clock_special_utc_time_day_string, "%02d", display_utc_time.day);
    return_menu->pWidgetList[3].u.num_string_addr = (uint32_t)clock_special_utc_time_day_string;

    //for step string
    static char clock_special_step_string[8] = {'\0'};
    memset(clock_special_step_string, '\0', sizeof(clock_special_step_string));
    sprintf(clock_special_step_string, "%d", RtkWristbandSys.gPedoData.global_steps);
    return_menu->pWidgetList[12].u.num_string_addr = (uint32_t)clock_special_step_string;

    //for kcal string
    static char clock_special_kcal_string[8] = {'\0'};
    memset(clock_special_kcal_string, '\0', sizeof(clock_special_kcal_string));
    sprintf(clock_special_kcal_string, "%d", RtkWristbandSys.gPedoData.global_calorys);
    return_menu->pWidgetList[14].u.num_string_addr = (uint32_t)clock_special_kcal_string;

    //for km string
    static char clock_special_km_string[8] = {'\0'};
    memset(clock_special_km_string, '\0', sizeof(clock_special_km_string));
    sprintf(clock_special_km_string, "%d", RtkWristbandSys.gPedoData.global_distance);
    return_menu->pWidgetList[16].u.num_string_addr = (uint32_t)clock_special_km_string;

    //for bmp string
    static char clock_special_bpm_string[8] = {'\0'};
    memset(clock_special_bpm_string, '\0', sizeof(clock_special_bpm_string));
    sprintf(clock_special_bpm_string, "%02d", 80);
    return_menu->pWidgetList[18].u.num_string_addr = (uint32_t)clock_special_bpm_string;

    //for hour string
    static char clock_special_utc_time_hour_string[4] = {'\0'};
    memset(clock_special_utc_time_hour_string, '\0', sizeof(clock_special_utc_time_hour_string));
    sprintf(clock_special_utc_time_hour_string, "%02d", display_utc_time.hour);
    return_menu->pWidgetList[7].u.num_string_addr = (uint32_t)clock_special_utc_time_hour_string;

    //for hour string
    static char clock_special_utc_time_min_string[4] = {'\0'};
    memset(clock_special_utc_time_min_string, '\0', sizeof(clock_special_utc_time_min_string));
    sprintf(clock_special_utc_time_min_string, "%02d", display_utc_time.minutes);
    return_menu->pWidgetList[10].u.num_string_addr = (uint32_t)clock_special_utc_time_min_string;

    if (RtkWristbandSys.flag_field.bond_state == true)
    {


    }
    else
    {

    }
    uint8_t battery_level = RtkWristbandSys.battery_level_value;
    if (battery_level >= 90)
    {
        return_menu->pWidgetList[5].addr = CLOCK_SPECIAL_BATTERY_5_BIN;
    }
    else if (battery_level >= 75)
    {
        return_menu->pWidgetList[5].addr = CLOCK_SPECIAL_BATTERY_4_BIN;
    }
    else if (battery_level >= 50)
    {
        return_menu->pWidgetList[5].addr = CLOCK_SPECIAL_BATTERY_3_BIN;
    }
    else if (battery_level >= 25)
    {
        return_menu->pWidgetList[5].addr = CLOCK_SPECIAL_BATTERY_1_BIN;
    }
    else
    {
        return_menu->pWidgetList[5].addr = CLOCK_SPECIAL_BATTERY_EMPTY_BIN;
    }

#if 0
    char *testString = "12";
    memcpy(num_12_24_string, testString, strlen(testString) + 1);

    return_menu->pWidgetList[3].u.num_string_addr = (uint32_t)num_12_24_string;
#endif
}
static void ButtonCallbackFun(uint8_t type)
{
    if (type == SHORT_BUTTON_0)
    {
        pCurrentMenu->destructor_func();
        pCurrentMenu = &TempTest0;
        pCurrentMenu->constructor_func(ICON_MIDDLE, NULL);
    }
    else if (type == LONG_BUTTON_0)
    {

    }
}

static void TouchCallbackFun(RtkTouchInfo_t *info)
{
    UI_MenuTypeDef *ui_local_right = &PlayerUIMenu;
    UI_MenuTypeDef *ui_local_left = &ExerciseMenu;
    UI_MenuTypeDef *ui_local_cur = &ClockSpecialMenu;
    UI_MenuTypeDef *ui_local_down = &SettingMenu;
    UI_MenuTypeDef *ui_local_up = &NotificationMenu;
    uint32_t timer_state;
    os_timer_state_get(&page_switch_timer, &timer_state);
    if (timer_state)
    {
        return;
    }

    os_timer_restart(&display_timer, 5000);
    os_timer_restart(&dynamic_timer, 500);


    if (info->type == TOUCH_HOLD)
    {
        rtl_gui_menu_update(ui_local_cur, NULL, ui_local_right, NULL, ui_local_left, NULL, info->deltaX, 0);
    }
    else if (info->type == TOUCH_HOLD_Y)
    {
        rtl_gui_menu_update(ui_local_cur, NULL, ui_local_down, NULL, ui_local_up, NULL, 0, info->deltaY);
    }

    else if (info->type == TOUCH_ORIGIN_FROM_X)
    {
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(ui_local_cur, NULL, ui_local_right, NULL, ui_local_left, NULL, info->deltaX, 0,
                           0, 0);
    }
    else if (info->type == TOUCH_ORIGIN_FROM_Y)
    {
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(ui_local_cur, NULL, ui_local_right, NULL, ui_local_left, NULL, 0, 0,
                           info->deltaY, 0);
    }
    else if (info->type == TOUCH_RIGHT_SLIDE)
    {
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(ui_local_cur, NULL, ui_local_right, NULL, ui_local_left, NULL, info->deltaX, 240,
                           0, 0);
    }
    else if (info->type == TOUCH_LEFT_SLIDE)
    {
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(ui_local_cur, NULL, ui_local_right, NULL, ui_local_left, NULL, info->deltaX,
                           -240, 0, 0);
    }
    else if (info->type == TOUCH_UP_SLIDE)
    {
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(ui_local_cur, NULL, ui_local_down, NULL, ui_local_up, NULL, 0, 0, info->deltaY,
                           240);
    }
    else if (info->type == TOUCH_DOWN_SLIDE)
    {
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(ui_local_cur, NULL, ui_local_down, NULL, ui_local_up, NULL, 0, 0, info->deltaY,
                           -240);
    }
    else if (info->type == TOUCH_INVALIDE)
    {
        rtl_gui_menu_update(ui_local_cur, NULL, ui_local_right, NULL, ui_local_left, NULL, 0, 0);
    }
}

