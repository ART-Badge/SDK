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
#include <string.h>
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
#include "hub_task.h"

static void    *dynamic_timer = NULL;
static void    *display_timer = NULL;
static void    *page_switch_timer   = NULL;
static bool    is_started = false;
static uint8_t select_start_pause = 1; // 1: start 2: pause


static void ButtonCallbackFun(uint8_t type);
static void TouchCallbackFun(RtkTouchInfo_t *info);
static void ConstructorCallbackFun(uint32_t type, void *pValue);
static void DestructorCallbackFun(void);
static void DisplayInfoCallbackFun(UI_MenuTypeDef *return_menu, void *argv);
static void TimerCallbackFun(RtkGuiTimerInfo_t *timer_info);
static void StartIconTouchCb(uint32_t button_touch_type);
static void StopIconTouchCb(uint32_t button_touch_type);

static uint8_t time_hour[3]    = {'0', '0', 0};
static uint8_t time_min[3]     = {'0', '0', 0};
static uint8_t counter_hour[3] = {'0', '0', 0};
static uint8_t counter_min[3]  = {'0', '0', 0};
static uint8_t counter_sec[3]  = {'0', '0', 0};
static uint8_t bpm_num[4]      = {'0', '7', '5', 0};
static uint32_t second_count;

static void StartIconTouchCb(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("[StartIconTouchCb]-->short button test!!");
        if (is_started == false)
        {
            /*change background to start background*/
            os_timer_start(&dynamic_timer);
            is_started = true;
            select_start_pause = 2;
        }
        else // pause
        {
            if (select_start_pause == 1)
            {
                os_timer_start(&dynamic_timer);
                select_start_pause = 2;
            }
            else
            {
                os_timer_stop(&dynamic_timer);
                select_start_pause = 1;
            }
        }
        rtl_gui_menu_update(&ExerciseCycleMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);
    }
}

static void StopIconTouchCb(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON && is_started == true)
    {
        APP_PRINT_INFO0("[StopIconTouchCb]-->short button test!!");
        //change to sports menu
        pCurrentMenu->MenuSub = &ExerciseMenu;
        pCurrentMenu->destructor_func();
        pCurrentMenu = pCurrentMenu->MenuSub;
        pCurrentMenu->constructor_func(ICON_MIDDLE, NULL);
    }
}

#define CYCLC_PAGE_NUM      20

const static UI_WidgetTypeDef  exercise_cycle_icon[CYCLC_PAGE_NUM] =
{
    [0]     = {
        .x = 0,   .y = 0,    .width = 240, .hight = 240,     .addr = EXERCISE_CYCLE_STOP_BACKGROUND_BIN,   .widget_id_type = ICON_BACKGROUND,
    },
    /*time*/
    [1]     = {//for time hour
        .x = 162,   .y = 0,    .width = 10, .hight = 19,     .addr = COMMON_P10X19WHITE_BIN,   .widget_id_type = ICON_NUM, \
        .touch_capability = false,
    },
    [2]     = {//for time dot first
        .x = 183,   .y = 5,    .width = 4, .hight = 5,     .addr = NULL/*COMMON_DOT_FOR_10_19_BIN*/,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = false
    },
    [3]     = {//for time dot second
        .x = 183,   .y = 15,    .width = 4, .hight = 5,     .addr = NULL/*COMMON_DOT_FOR_10_19_BIN*/,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = false
    },
    [4]     = {//for time minute
        .x = 187,   .y = 0,    .width = 10, .hight = 19,     .addr = COMMON_P10X19WHITE_BIN,   .widget_id_type = ICON_NUM, \
        .touch_capability = false,
    },
    /*time elapse since started*/
    [5]     = {//for counter time hour
        .x = 108,   .y = 40,    .width = 19, .hight = 36,     .addr = COMMON_P19X36WHITE_BIN,   .widget_id_type = ICON_NUM, \
        .touch_capability = false,
    },
    [6]     = {//for counter time dot
        .x = 149,   .y = 48,    .width = 6, .hight = 28,     .addr = NULL/*COMMON_COLON_6X28_BIN*/,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = false
    },
    [7]     = {//for counter time minute
        .x = 156,   .y = 40,    .width = 19, .hight = 36,     .addr = COMMON_P19X36WHITE_BIN,   .widget_id_type = ICON_NUM, \
        .touch_capability = false,
    },
    [8]     = {//for counter time dot
        .x = 196,   .y = 48,    .width = 6, .hight = 28,     .addr = NULL/*COMMON_COLON_6X28_BIN*/,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = false
    },
    [9]     = {//for counter time second
        .x = 204,   .y = 40,    .width = 19, .hight = 36,     .addr = COMMON_P19X36WHITE_BIN,   .widget_id_type = ICON_NUM, \
        .touch_capability = false,
    },
    /*bmp*/
    [10]     = {//for 'bmp'
        .x = 133,   .y = 88,    .width = 27, .hight = 19,     .addr = NULL/*COMMON_BMP_BIN*/,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = false
    },
    [11]     = {//for 'bmp' num string
        .x = 163,   .y = 88,    .width = 19, .hight = 36,     .addr = COMMON_P19X36RED_BIN,   .widget_id_type = ICON_NUM, \
        .touch_capability = false,
    },
    /*stop*/
    [12]     = {
        .x = 10,   .y = 200,   .width = 25,   .hight = 25,   .addr = NULL,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = false, .widget_touch_func = StopIconTouchCb
    },
    /*start/pause*/
    [13]     = {
        .x = 200,   .y = 200,   .width = 25,   .hight = 28,   .addr = COMMON_START_BIN,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = false, .widget_touch_func = StartIconTouchCb
    },
    [14]     = {
        .x = 1,   .y = 180,    .width = 120, .hight = 60,     .addr = NULL,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = true,  .widget_touch_func = StopIconTouchCb
    },
    [15]     = {
        .x = 121,  .y = 180,    .width = 120, .hight = 60,     .addr = NULL,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = true,  .widget_touch_func = StartIconTouchCb
    },
};

/* change Here for UI */
UI_MenuTypeDef ExerciseCycleMenu =
{
    /* change Here for UI */
    .name = (const char *)"ExerciseCycleMenu",
    .MenuParent = NULL,
    .MenuSub = NULL,
    .button_func = ButtonCallbackFun,
    .touch_func = TouchCallbackFun,
    .constructor_func = ConstructorCallbackFun,
    .destructor_func = DestructorCallbackFun,
    .display_info_func = DisplayInfoCallbackFun,
    .timer_func = TimerCallbackFun,
    .pWidgetList = (UI_WidgetTypeDef *)exercise_cycle_icon,
    .current_max_icon = CYCLC_PAGE_NUM,
};



static void TimerCallbackFun(RtkGuiTimerInfo_t *timer_info)
{
    uint32_t timer_id = timer_info->timer_id;
    if (memcmp(timer_info->menu_name,  ExerciseCycleMenu.name, strlen(ExerciseCycleMenu.name)))
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
        //APP_PRINT_INFO2("timer_id = %d, Name = %s", timer_id, TRACE_STRING(pCurrentMenu->name));
        second_count++;
        counter_sec[1] = second_count % 10 + '0';
        counter_sec[0] = second_count % 60 / 10 + '0';
        counter_min[1] = (second_count % 3600) / 60 % 10 + '0';
        counter_min[0] = (second_count % 3600) / 60 / 10 + '0';
        counter_hour[1] = (second_count / 3600) % 10 + '0';
        counter_hour[0] = (second_count / 3600) / 10 + '0';
        if (counter_hour[0] > '9')
        {
            counter_hour[0] = '0';
        }
        rtl_gui_menu_update(&ExerciseCycleMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);
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
    second_count = 0;
    counter_hour[0] = counter_hour[1] = '0';
    counter_min[0] = counter_min[1] = '0';
    counter_sec[0] = counter_sec[1] = '0';

    rtl_gui_menu_update(&ExerciseCycleMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);

    os_timer_create(&display_timer, "display picture timer", 1, 500000, false, rtl_gui_timer_callback);
    os_timer_create(&dynamic_timer, "dynamic picture timer", 2, 1000, true, rtl_gui_timer_callback);
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
    is_started = false;
    select_start_pause = 1;
    APP_PRINT_INFO2("file = %s, line = %d", TRACE_STRING(__FILE__), __LINE__);
}


static void DisplayInfoCallbackFun(UI_MenuTypeDef *return_menu, void *argv)
{
    if (return_menu == NULL)
    {
        return;
    }

    return_menu->current_max_icon = ExerciseCycleMenu.current_max_icon;
    /* do memcpy here */
    memcpy(return_menu->pWidgetList, ExerciseCycleMenu.pWidgetList,
           ExerciseCycleMenu.current_max_icon * sizeof(UI_WidgetTypeDef));

    if (is_started == false)
    {
        return_menu->pWidgetList[0].addr = EXERCISE_CYCLE_STOP_BACKGROUND_BIN;
        return_menu->pWidgetList[13].addr = COMMON_START_BIN;
    }
    else
    {
        return_menu->pWidgetList[0].addr = EXERCISE_CYCLE_START_BACKGROUND_BIN;
        return_menu->pWidgetList[1].u.num_string_addr = (uint32_t)time_hour;
        return_menu->pWidgetList[2].addr = COMMON_DOT_FOR_10_19_BIN;
        return_menu->pWidgetList[3].addr = COMMON_DOT_FOR_10_19_BIN;
        return_menu->pWidgetList[4].u.num_string_addr = (uint32_t)time_min;

        return_menu->pWidgetList[5].u.num_string_addr = (uint32_t)counter_hour;
        return_menu->pWidgetList[6].addr = COMMON_COLON_6X28_BIN;
        return_menu->pWidgetList[7].u.num_string_addr = (uint32_t)counter_min;
        return_menu->pWidgetList[8].addr = COMMON_COLON_6X28_BIN;
        return_menu->pWidgetList[9].u.num_string_addr = (uint32_t)counter_sec;

        return_menu->pWidgetList[10].addr = COMMON_BMP_BIN;
        return_menu->pWidgetList[11].u.num_string_addr = (uint32_t)bpm_num;
        if (select_start_pause == 1)
        {
            return_menu->pWidgetList[13].addr = COMMON_START_BIN;
            return_menu->pWidgetList[13].hight = 28;
        }
        else
        {
            return_menu->pWidgetList[13].addr = COMMON_PAUSE_BIN;
            return_menu->pWidgetList[13].hight = 25;
        }
        return_menu->pWidgetList[12].addr = COMMON_STOP_BIN;
    }
}
static void ButtonCallbackFun(uint8_t type)
{
    if (type == SHORT_BUTTON_0)
    {
        pCurrentMenu->MenuSub = &ExerciseMenu;
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

}

