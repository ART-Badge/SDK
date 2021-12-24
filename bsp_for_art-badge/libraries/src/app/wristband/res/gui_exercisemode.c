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


static void ButtonCallbackFun(uint8_t type);
static void TouchCallbackFun(RtkTouchInfo_t *info);
static void ConstructorCallbackFun(uint32_t type, void *pValue);
static void DestructorCallbackFun(void);
static void DisplayInfoCallbackFun(UI_MenuTypeDef *return_menu, void *argv);
static void TimerCallbackFun(RtkGuiTimerInfo_t *timer_info);
static void ExerciseClimbButton(uint32_t button_touch_type);
static void ExerciseRunButton(uint32_t button_touch_type);
static void ExerciseFootballButton(uint32_t button_touch_type);
static void ExerciseRopeButton(uint32_t button_touch_type);
static void ExerciseCycleButton(uint32_t button_touch_type);

#define EXER_PAGE_NUM      6

const static UI_WidgetTypeDef  exercisemode_icons[EXER_PAGE_NUM] =
{
    [0]     = {
        .x = 0,     .y = 0,     .width = 240,   .hight = 240,    .addr = EXERCISE_BACKGROUND_BIN,   .widget_id_type = ICON_BACKGROUND, \
        .touch_capability = false,
    },
    [1]     = {
        .x = 0,     .y = 67,    .width = 67,    .hight = 67,     .addr = NULL,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = true,     .widget_touch_func = ExerciseClimbButton
    },
    [2]     = {
        .x = 86,    .y = 67,    .width = 67,    .hight = 67,     .addr = NULL,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = true,     .widget_touch_func = ExerciseFootballButton
    },
    [3]     = {
        .x = 173,   .y = 67,    .width = 67,    .hight = 67,     .addr = NULL,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = true,     .widget_touch_func = ExerciseRunButton
    },
    [4]     = {
        .x = 45,    .y = 149,   .width = 67,    .hight = 67,     .addr = NULL,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = true,     .widget_touch_func = ExerciseRopeButton
    },
    [5]     = {
        .x = 129,   .y = 149,   .width = 67,    .hight = 67,     .addr = NULL,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = true,     .widget_touch_func = ExerciseCycleButton
    },
};

UI_MenuTypeDef ExerciseMenu =
{
    .name = (const char *)"ExerciseMenu",
    .MenuParent = NULL,
    .MenuSub = NULL,
    .button_func = ButtonCallbackFun,
    .touch_func = TouchCallbackFun,
    .constructor_func = ConstructorCallbackFun,
    .destructor_func = DestructorCallbackFun,
    .display_info_func = DisplayInfoCallbackFun,
    .timer_func = TimerCallbackFun,
    .pWidgetList = (UI_WidgetTypeDef *)exercisemode_icons,
    .current_max_icon = EXER_PAGE_NUM,
};

static void ExerciseClimbButton(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("[ExerciseClimbButtonTest]-->short button test!!");
        /* change Here for UI */
    }
    else if (button_touch_type == LONG_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("[ExerciseClimbButtonTest]-->long button test!!");
    }
}
static void ExerciseRunButton(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("[ExerciseRunButtonTest]-->short button test!!");
    }
    else if (button_touch_type == LONG_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("[ExerciseRunButtonTest]-->long button test!!");
    }
}
static void ExerciseFootballButton(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("[ExerciseFootballButtonTest]-->short button test!!");
    }
    else if (button_touch_type == LONG_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("[ExerciseFootballButtonTest]-->long button test!!");
    }
}

static void ExerciseRopeButton(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("[ExerciseRopeButtonTest]-->short button test!!");
    }
    else if (button_touch_type == LONG_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("[ExerciseRopeButtonTest]-->long button test!!");
    }
}

static void ExerciseCycleButton(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("[ExerciseCycleButtonTest]-->short button test!!");
    }
    else if (button_touch_type == LONG_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("[ExerciseCycleButtonTest]-->long button test!!");
    }
}

static void TimerCallbackFun(RtkGuiTimerInfo_t *timer_info)
{
    uint32_t timer_id = timer_info->timer_id;
    if (memcmp(timer_info->menu_name,  ExerciseMenu.name, strlen(ExerciseMenu.name)))
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

    rtl_gui_menu_update(&ExerciseMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);

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
    return_menu->current_max_icon = ExerciseMenu.current_max_icon;
    /* do memcpy here */
    memcpy(return_menu->pWidgetList, ExerciseMenu.pWidgetList,
           ExerciseMenu.current_max_icon * sizeof(UI_WidgetTypeDef));
    //to do, set steps, distance, calorie, progress.

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

    if (info->type == TOUCH_HOLD)
    {
        rtl_gui_menu_update(&ExerciseMenu, NULL, &ClockSpecialMenu, NULL, NULL, NULL, info->deltaX, 0);
    }

    if (info->type == TOUCH_RIGHT_SLIDE)
    {
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(&ExerciseMenu, NULL, \
                           &ClockSpecialMenu, NULL, \
                           NULL, NULL, \
                           info->deltaX, 0, 0, 0);
    }
    if (info->type == TOUCH_LEFT_SLIDE)
    {
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(&ExerciseMenu, NULL, \
                           &ClockSpecialMenu, NULL, \
                           NULL, NULL, \
                           info->deltaX, -240, 0, 0);
    }
    if (info->type == TOUCH_ORIGIN_FROM_X)
    {
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(&ExerciseMenu, NULL, \
                           &ClockSpecialMenu, NULL, \
                           NULL, NULL, \
                           info->deltaX, 0, 0, 0);
    }
}

