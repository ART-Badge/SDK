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
#include "gui_common_menu.h"

static void    *dynamic_timer = NULL;
static void    *display_timer = NULL;
static void    *page_switch_timer   = NULL;
static bool     ClockDotDisplayFlag = true;
static uint8_t item_showing = 0; // 1: heartrate 2: calory 3: steps



static void ButtonCallbackFun(uint8_t type);
static void TouchCallbackFun(RtkTouchInfo_t *info);
static void ConstructorCallbackFun(uint32_t type, void *pValue);
static void DestructorCallbackFun(void);
static void DisplayInfoCallbackFun(UI_MenuTypeDef *return_menu, void *argv);
static void TimerCallbackFun(RtkGuiTimerInfo_t *timer_info);

/*widgets touch callback functions*/
static void CurrentMenuButtonTest(uint32_t button_touch_type);
static void HeartRateTouchCb(uint32_t button_touch_type);
static void StepTouchCb(uint32_t button_touch_type);
static void CaloryTouchCb(uint32_t button_touch_type);

static void CurrentMenuButtonTest(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("[CurrentMenuButtonTest]-->short button test!!");
    }
    else if (button_touch_type == LONG_TOUCH_BUTTON)
    {
        /* change Here for UI */
        pCurrentMenu->MenuSub = &ClockChangeFitbitMenu;
        pCurrentMenu->destructor_func();
        pCurrentMenu = pCurrentMenu->MenuSub;
        pCurrentMenu->constructor_func(ICON_MIDDLE, NULL);
        APP_PRINT_INFO0("[CurrentMenuButtonTest]-->long button test!!");
    }
}

#define CLOCK_FIT_PAGE_NUM      21

const static UI_WidgetTypeDef  clock_fit_icon[CLOCK_FIT_PAGE_NUM] =
{
    [0]     = {
        .x = 0,   .y = 0,    .width = 240, .hight = 240,     .addr = NULL,    .widget_id_type = ICON_BACKGROUND, \
        .touch_capability = true,  .widget_touch_func = CurrentMenuButtonTest
    },
    /*date*/
    [1]     = {.x = 85,  .y = 15,   .width = 39,  .hight = 20,      .addr = COMMON_MONTH_BIN,            .widget_id_type = ICON_WIDGET},
    [2]     = {.x = 135, .y = 15,   .width = 10,  .hight = 19,      .addr = COMMON_P10X19WHITE_BIN,      .widget_id_type = ICON_WIDGET},
    [3]     = {.x = 145, .y = 15,   .width = 10,  .hight = 19,      .addr = COMMON_P10X19WHITE_BIN,      .widget_id_type = ICON_WIDGET},
    /*digital clock*/
    [4]     = {.x = 111, .y = 86,   .width = 18,  .hight = 18,      .addr = CLOCK_DIAL_FIT_RECT_BIN,     .widget_id_type = ICON_WIDGET},
    [5]     = {.x = 111, .y = 146,  .width = 18,  .hight = 18,      .addr = CLOCK_DIAL_FIT_RECT_BIN,     .widget_id_type = ICON_WIDGET},
    [6]     = {.x = 0,   .y = 60,   .width = 70,  .hight = 125,     .addr = NUMBER_0_LARGE_GRAY_BIN,     .widget_id_type = ICON_WIDGET},
    [7]     = {.x = 50,  .y = 60,   .width = 70,  .hight = 125,     .addr = NUMBER_8_LARGE_WHITE_BIN,    .widget_id_type = ICON_WIDGET},
    [8]     = {.x = 120, .y = 60,   .width = 70,  .hight = 125,     .addr = NUMBER_5_LARGE_GRAY_BIN,     .widget_id_type = ICON_WIDGET},
    [9]     = {.x = 170, .y = 60,   .width = 70,  .hight = 125,     .addr = NUMBER_3_LARGE_WHITE_BIN,    .widget_id_type = ICON_WIDGET},
    /*heartrate/steps/calories to show*/
    [10]    = {.x = 30,  .y = 200,  .width = 13,  .hight = 20,      .addr = NULL,                        .widget_id_type = ICON_WIDGET},
    [11]    = {.x = 43,  .y = 200,  .width = 13,  .hight = 20,      .addr = NULL,                        .widget_id_type = ICON_WIDGET},
    [12]    = {.x = 56,  .y = 200,  .width = 13,  .hight = 20,      .addr = NULL,                        .widget_id_type = ICON_WIDGET},
    [13]    = {.x = 69,  .y = 200,  .width = 13,  .hight = 20,      .addr = NULL,                        .widget_id_type = ICON_WIDGET},
    [14]    = {.x = 82,  .y = 200,  .width = 13,  .hight = 20,      .addr = NULL,                        .widget_id_type = ICON_WIDGET},
    /*touch icons*/
    [15]    = {
        .x = 116, .y = 200,  .width = 24,  .hight = 21,      .addr = CLOCK_DIAL_FIT_HR_BIN,           .widget_id_type = ICON_WIDGET, \
        .touch_capability = true, .widget_touch_func = HeartRateTouchCb
    },
    [16]    = {
        .x = 160, .y = 200,  .width = 15,  .hight = 23,      .addr = CLOCK_DIAL_FIT_STEP_BIN,            .widget_id_type = ICON_WIDGET, \
        .touch_capability = true,     .widget_touch_func = StepTouchCb
    },
    [17]    = {
        .x = 195, .y = 200,  .width = 20,  .hight = 23,      .addr = CLOCK_DIAL_FIT_CALORY_BIN,             .widget_id_type = ICON_WIDGET, \
        .touch_capability = true,     .widget_touch_func = CaloryTouchCb
    },
    [18]    = {
        .x = 116, .y = 200, .width = 44,  .hight = 40,    .addr = NULL,      .widget_id_type = ICON_WIDGET, \
        .touch_capability = true,     .widget_touch_func = HeartRateTouchCb
    },
    [19]    = {
        .x = 160, .y = 200, .width = 35,  .hight = 40,    .addr = NULL,      .widget_id_type = ICON_WIDGET, \
        .touch_capability = true,    .widget_touch_func = StepTouchCb
    },
    [20]    = {
        .x = 195, .y = 200, .width = 45,  .hight = 40,    .addr = NULL,      .widget_id_type = ICON_WIDGET, \
        .touch_capability = true,    .widget_touch_func = CaloryTouchCb
    }
};

/* change Here for UI */
UI_MenuTypeDef ClockFitMenu =
{
    /* change Here for UI */
    .name = (const char *)"ClockFitMenu",
    .MenuParent = NULL,
    .MenuSub = NULL,
    .button_func = ButtonCallbackFun,
    .touch_func = TouchCallbackFun,
    .constructor_func = ConstructorCallbackFun,
    .destructor_func = DestructorCallbackFun,
    .display_info_func = DisplayInfoCallbackFun,
    .timer_func = TimerCallbackFun,
    .pWidgetList = (UI_WidgetTypeDef *)clock_fit_icon,
    .current_max_icon = CLOCK_FIT_PAGE_NUM,
};


static void TimerCallbackFun(RtkGuiTimerInfo_t *timer_info)
{
    uint32_t timer_id = timer_info->timer_id;
    if (memcmp(timer_info->menu_name,  ClockFitMenu.name, strlen(ClockFitMenu.name)))
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
        if (ClockDotDisplayFlag == true)
        {
            ClockDotDisplayFlag = false;
        }
        else
        {
            ClockDotDisplayFlag = true;
        }

        if (pCurrentMenu->dynamicFlag == true)
        {
            rtl_gui_menu_update(&ClockFitMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);
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

    rtl_gui_menu_update(&ClockFitMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);

    os_timer_create(&display_timer, "display picture timer", 1, 500000, false, rtl_gui_timer_callback);
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
    item_showing = 0;
    APP_PRINT_INFO2("file = %s, line = %d", TRACE_STRING(__FILE__), __LINE__);
}

static void DisplayInfoCallbackFun(UI_MenuTypeDef *return_menu, void *argv)
{
    uint8_t  dummy_heart_rate = 78;
    uint32_t dummy_calories = 12345;
    uint32_t dummy_steps = 54321;
    if (return_menu == NULL)
    {
        return;
    }

    return_menu->current_max_icon = ClockFitMenu.current_max_icon;
    /* do memcpy here */
    memcpy(return_menu->pWidgetList, ClockFitMenu.pWidgetList,
           ClockFitMenu.current_max_icon * sizeof(UI_WidgetTypeDef));
    /*Month and Date*/
    if (RtkWristbandSys.flag_field.bond_state == true)
    {
        return_menu->pWidgetList[1].addr = COMMON_MONTH_BIN + \
                                           (RtkWristbandSys.Global_Time.month - 1) * 2 * (return_menu->pWidgetList[1].width) *
                                           (return_menu->pWidgetList[1].hight);
        return_menu->pWidgetList[2].addr = COMMON_P10X19WHITE_BIN + \
                                           (RtkWristbandSys.Global_Time.day / 10) * 2 * (return_menu->pWidgetList[2].width) *
                                           (return_menu->pWidgetList[2].hight);
        return_menu->pWidgetList[3].addr = COMMON_P10X19WHITE_BIN + \
                                           (RtkWristbandSys.Global_Time.day % 10) * 2 * (return_menu->pWidgetList[3].width) *
                                           (return_menu->pWidgetList[3].hight);
    }
    /*Digital Clock*/
    if (ClockDotDisplayFlag == true)
    {
        return_menu->pWidgetList[4].addr = CLOCK_DIAL_FIT_RECT_BIN;
        return_menu->pWidgetList[5].addr = CLOCK_DIAL_FIT_RECT_BIN;
    }
    else
    {
        return_menu->pWidgetList[4].addr = NULL;
        return_menu->pWidgetList[5].addr = NULL;
    }
    if (RtkWristbandSys.flag_field.bond_state == true)
    {
        return_menu->pWidgetList[6].addr = NUMBER_0_LARGE_GRAY_BIN + \
                                           (RtkWristbandSys.Global_Time.hour / 10) * 17500 * 2;
        return_menu->pWidgetList[7].addr = NUMBER_0_LARGE_WHITE_BIN + \
                                           (RtkWristbandSys.Global_Time.hour % 10) * 17500 * 2;
        return_menu->pWidgetList[8].addr = NUMBER_0_LARGE_GRAY_BIN + \
                                           (RtkWristbandSys.Global_Time.minutes / 10) * 17500 * 2;
        return_menu->pWidgetList[9].addr = NUMBER_0_LARGE_WHITE_BIN + \
                                           (RtkWristbandSys.Global_Time.minutes % 10) * 17500 * 2;
    }
    /*Heart Rate / Calories / Steps 1: heartrate 2: calory 3: steps*/
    //if (RtkWristbandSys.flag_field.bond_state == true)
    {
        if (item_showing == 0)
        {
            return_menu->pWidgetList[10].addr = NULL;
            return_menu->pWidgetList[11].addr = NULL;
            return_menu->pWidgetList[12].addr = NULL;
            return_menu->pWidgetList[13].addr = NULL;
            return_menu->pWidgetList[14].addr = NULL;
        }
        else if (item_showing == 1)
        {
            return_menu->pWidgetList[10].addr = NULL;
            return_menu->pWidgetList[11].addr = NULL;
            return_menu->pWidgetList[12].addr = (dummy_heart_rate / 100) ? (NUMBERS_13X20_RED_BIN +
                                                                            (dummy_heart_rate / 100) * 2 * 13 * 20) : NULL;
            return_menu->pWidgetList[13].addr = NUMBERS_13X20_RED_BIN + (dummy_heart_rate / 10 % 10) * 2 * 13 *
                                                20;
            return_menu->pWidgetList[14].addr = NUMBERS_13X20_RED_BIN + (dummy_heart_rate % 10) * 2 * 13 * 20;
        }
        else if (item_showing == 2)
        {
            return_menu->pWidgetList[10].addr = (dummy_calories / 10000) ? (NUMBERS_13X20_BLUE_BIN +
                                                                            (dummy_calories / 10000 % 10) * 2 * 13 * 20) : NULL;
            return_menu->pWidgetList[11].addr = (dummy_calories / 1000) ? (NUMBERS_13X20_BLUE_BIN +
                                                                           (dummy_calories / 1000 % 10) * 2 * 13 * 20) : NULL;
            return_menu->pWidgetList[12].addr = (dummy_calories / 100) ? (NUMBERS_13X20_BLUE_BIN +
                                                                          (dummy_calories / 100 % 10) * 2 * 13 * 20) : NULL;
            return_menu->pWidgetList[13].addr = (dummy_calories / 10) ? (NUMBERS_13X20_BLUE_BIN +
                                                                         (dummy_calories / 10 % 10) * 2 * 13 * 20) : NULL;
            return_menu->pWidgetList[14].addr = NUMBERS_13X20_BLUE_BIN + (dummy_calories % 10) * 2 * 13 * 20;
        }
        else if (item_showing == 3)
        {
            return_menu->pWidgetList[10].addr = (dummy_steps / 10000) ? (NUMBERS_13X20_BROWN_BIN +
                                                                         (dummy_steps / 10000 % 10) * 2 * 13 * 20) : NULL;
            return_menu->pWidgetList[11].addr = (dummy_steps / 1000) ? (NUMBERS_13X20_BROWN_BIN +
                                                                        (dummy_steps / 1000 % 10) * 2 * 13 * 20) : NULL;
            return_menu->pWidgetList[12].addr = (dummy_steps / 100) ? (NUMBERS_13X20_BROWN_BIN +
                                                                       (dummy_steps / 100 % 10) * 2 * 13 * 20) : NULL;
            return_menu->pWidgetList[13].addr = (dummy_steps / 10) ? (NUMBERS_13X20_BROWN_BIN +
                                                                      (dummy_steps / 10 % 10) * 2 * 13 * 20) : NULL;
            return_menu->pWidgetList[14].addr = NUMBERS_13X20_BROWN_BIN + (dummy_steps % 10) * 2 * 13 * 20;
        }
    }
}
static void ButtonCallbackFun(uint8_t type)
{
    if (type == SHORT_BUTTON_0)
    {
        /* change Here for UI */
        // pCurrentMenu->MenuSub = &ExerciseMenu;
        // pCurrentMenu->destructor_func();
        // pCurrentMenu = pCurrentMenu->MenuSub;
        // pCurrentMenu->constructor_func(ICON_MIDDLE, NULL);
    }
    else if (type == LONG_BUTTON_0)
    {
        pCurrentMenu->MenuSub = &ClockChangeFitbitMenu;
        pCurrentMenu->destructor_func();
        pCurrentMenu = pCurrentMenu->MenuSub;
        pCurrentMenu->constructor_func(ICON_MIDDLE, NULL);
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
        rtl_gui_menu_update(&ClockFitMenu, NULL, &StepMenu, NULL, &ExerciseMenu, NULL, info->deltaX, 0);
    }
    if (info->type == TOUCH_HOLD_Y)
    {
        rtl_gui_menu_update(&ClockFitMenu, NULL, &SettingMenu, NULL, &MessageMenu, NULL, 0, info->deltaY);
    }
    APP_PRINT_INFO3("info->type = %d, info->detalX = %d, info->detalY = %d", info->type, info->deltaX,
                    info->deltaY);

    if (info->type == TOUCH_ORIGIN_FROM_X)
    {
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(&ClockFitMenu, NULL, &StepMenu, NULL, &ExerciseMenu, NULL, info->deltaX, 0, 0,
                           0);
    }
    if (info->type == TOUCH_ORIGIN_FROM_Y)
    {
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(&ClockFitMenu, NULL, &StepMenu, NULL, &ExerciseMenu, NULL, 0, 0, info->deltaY,
                           0);
    }
    else if (info->type == TOUCH_RIGHT_SLIDE)
    {
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(&ClockFitMenu, NULL, &StepMenu, NULL, &ExerciseMenu, NULL, info->deltaX, 240, 0,
                           0);
    }
    else if (info->type == TOUCH_LEFT_SLIDE)
    {
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(&ClockFitMenu, NULL, &StepMenu, NULL, &ExerciseMenu, NULL, info->deltaX, -240, 0,
                           0);
    }
    else if (info->type == TOUCH_UP_SLIDE)
    {
        APP_PRINT_INFO0("===TOUCH_UP_SLIDE===");
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(&ClockFitMenu, NULL, &SettingMenu, NULL, &NotificationMenu, NULL, 0, 0,
                           info->deltaY,
                           240);
    }
    else if (info->type == TOUCH_DOWN_SLIDE)
    {
        APP_PRINT_INFO0("===TOUCH_DOWN_SLIDE===");
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(&ClockFitMenu, NULL, &SettingMenu, NULL, &NotificationMenu, NULL, 0, 0,
                           info->deltaY,
                           -240);
    }
    else if (info->type == TOUCH_INVALIDE)
    {
        rtl_gui_menu_update(&ClockFitMenu, NULL, &StepMenu, NULL, &ExerciseMenu, NULL, 0, 0);
    }
}

static void HeartRateTouchCb(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON)
    {
        item_showing = 1;
        APP_PRINT_INFO0("[HeartRateTouchCb]");
        rtl_gui_menu_update(&ClockFitMenu, NULL, &BPMenu, NULL, NULL, NULL, 0, 0);
    }
}

static void StepTouchCb(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON)
    {
        item_showing = 2;
        APP_PRINT_INFO0("[StepTouchCb]");
        rtl_gui_menu_update(&ClockFitMenu, NULL, &BPMenu, NULL, NULL, NULL, 0, 0);
    }
}

static void CaloryTouchCb(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON)
    {
        item_showing = 3;
        APP_PRINT_INFO0("[CaloryTouchCb]");
        rtl_gui_menu_update(&ClockFitMenu, NULL, &BPMenu, NULL, NULL, NULL, 0, 0);
    }
}


