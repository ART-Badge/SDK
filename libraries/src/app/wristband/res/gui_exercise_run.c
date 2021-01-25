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
#include "stdio.h"
#include "hub_task.h"

static void    *dynamic_timer = NULL;
static void    *display_timer = NULL;
static void    *page_switch_timer   = NULL;

static bool menu_start_status = false;
static bool run_status = false;
static uint32_t second_counter = 0;


static void ButtonCallbackFun(uint8_t type);
static void TouchCallbackFun(RtkTouchInfo_t *info);
static void ConstructorCallbackFun(uint32_t type, void *pValue);
static void DestructorCallbackFun(void);
static void DisplayInfoCallbackFun(UI_MenuTypeDef *return_menu, void *argv);
static void TimerCallbackFun(RtkGuiTimerInfo_t *timer_info);
static void StartOrPauseMenuButton(uint32_t button_touch_type);
static void StopMenuButton(uint32_t button_touch_type);


static void StartOrPauseMenuButton(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("[StartOrPauseMenuButton]-->short button test!!");
        if (menu_start_status == false)
        {
            menu_start_status = true;
            run_status = true;
        }
        else
        {
            if (run_status == false)
            {
                run_status = true;
            }
            else
            {
                run_status = false;
            }
        }
        rtl_gui_menu_update(&ExerciseRunMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);
    }
    else if (button_touch_type == LONG_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("[StartOrPauseMenuButton]-->long button test!!");
    }
}

static void StopMenuButton(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("[StopMenuButton]-->short button test!!");
        run_status = false;
        menu_start_status = false;
        pCurrentMenu->MenuSub = &ExerciseMenu;
        pCurrentMenu->destructor_func();
        pCurrentMenu = pCurrentMenu->MenuSub;
        pCurrentMenu->constructor_func(ICON_MIDDLE, NULL);
        second_counter = 0;
    }
    else if (button_touch_type == LONG_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("[StopMenuButton]-->long button test!!");
    }
}

const static UI_WidgetTypeDef  icon[] =
{
    [0]     = {
        .x = 0,    .y = 0,    .width = 240,    .hight = 240,    .addr = EXERCISE_RUN_BG_STOP_BIN,  .widget_id_type = ICON_BACKGROUND, \
        .touch_capability = false
    },
    [1]     = {//for time hour
        .x = 162,   .y = 0,    .width = 10, .hight = 19,     .addr = COMMON_P10X19WHITE_BIN,   .widget_id_type = ICON_NUM, \
        .touch_capability = false,
    },
    [2]     = {//for time dot first
        .x = 183,   .y = 5,    .width = 4, .hight = 5,     .addr = COMMON_DOT_FOR_10_19_BIN,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = false
    },
    [3]     = {//for time dot second
        .x = 183,   .y = 15,    .width = 4, .hight = 5,     .addr = COMMON_DOT_FOR_10_19_BIN,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = false
    },
    [4]     = {//for time minute
        .x = 187,   .y = 0,    .width = 10, .hight = 19,     .addr = COMMON_P10X19WHITE_BIN,   .widget_id_type = ICON_NUM, \
        .touch_capability = false,
    },

    [5]     = {//for counter time hour
        .x = 108,   .y = 40,    .width = 19, .hight = 36,     .addr = COMMON_P19X36WHITE_BIN,   .widget_id_type = ICON_NUM, \
        .touch_capability = false,
    },
    [6]     = {//for counter time dot
        .x = 149,   .y = 48,    .width = 6, .hight = 28,     .addr = COMMON_COLON_6X28_BIN,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = false
    },
    [7]     = {//for counter time minute
        .x = 156,   .y = 40,    .width = 19, .hight = 36,     .addr = COMMON_P19X36WHITE_BIN,   .widget_id_type = ICON_NUM, \
        .touch_capability = false,
    },
    [8]     = {//for counter time dot
        .x = 196,   .y = 48,    .width = 6, .hight = 28,     .addr = COMMON_COLON_6X28_BIN,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = false
    },
    [9]     = {//for counter time second
        .x = 204,   .y = 40,    .width = 19, .hight = 36,     .addr = COMMON_P19X36WHITE_BIN,   .widget_id_type = ICON_NUM, \
        .touch_capability = false,
    },

    [10]     = {//for 'bmp'
        .x = 133,   .y = 88,    .width = 27, .hight = 19,     .addr = COMMON_BMP_BIN,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = false
    },
    [11]     = {//for 'km'
        .x = 141,   .y = 137,    .width = 17, .hight = 15,     .addr = COMMON_KM_BIN,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = false
    },
    [12]     = {//for 'step'
        .x = 135,   .y = 168,    .width = 27, .hight = 18,     .addr = COMMON_STEP_BIN,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = false
    },
    [13]     = {//for 'kcal'
        .x = 135,   .y = 197,    .width = 24, .hight = 15,     .addr = COMMON_KCAL_BIN,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = false
    },

    [14]     = {//for 'bmp' num string
        .x = 163,   .y = 88,    .width = 4, .hight = 5,     .addr = COMMON_BMP_BIN,   .widget_id_type = ICON_NUM, \
        .touch_capability = false,
    },
    [15]     = {//for 'km' num string
        .x = 163,   .y = 137,    .width = 4, .hight = 5,     .addr = COMMON_KM_BIN,   .widget_id_type = ICON_NUM, \
        .touch_capability = false,
    },
    [16]     = {//for 'step' num string
        .x = 163,   .y = 168,    .width = 10, .hight = 19,     .addr = COMMON_STEP_BIN,   .widget_id_type = ICON_NUM, \
        .touch_capability = false,
    },
    [17]     = {//for 'kcal' num string
        .x = 163,   .y = 197,    .width = 10, .hight = 19,     .addr = COMMON_KCAL_BIN,   .widget_id_type = ICON_NUM, \
        .touch_capability = false,
    },


    [18]     = {//for stop
        .x = 10,   .y = 200,    .width = 25, .hight = 25,     .addr = COMMON_STOP_BIN,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = true,  .widget_touch_func = StopMenuButton
    },
    [19]     = {//for start
        .x = 200,   .y = 200,    .width = 25, .hight = 28,     .addr = COMMON_START_BIN,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = true,  .widget_touch_func = StartOrPauseMenuButton
    },
    [20]     = {
        .x = 1,   .y = 180,    .width = 120, .hight = 60,     .addr = NULL,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = true,  .widget_touch_func = StopMenuButton
    },
    [21]     = {
        .x = 121,  .y = 180,    .width = 120, .hight = 60,     .addr = NULL,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = true,  .widget_touch_func = StartOrPauseMenuButton
    },
};

/* change Here for UI */
UI_MenuTypeDef ExerciseRunMenu =
{
    /* change Here for UI */
    .name = (const char *)"ExerciseRunMenu",
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
    if (memcmp(timer_info->menu_name,  ExerciseRunMenu.name, strlen(ExerciseRunMenu.name)))
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
        if (run_status == true)
        {
            second_counter++;
            rtl_gui_menu_update(&ExerciseRunMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);
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

    rtl_gui_menu_update(&ExerciseRunMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);

    os_timer_create(&display_timer, "display picture timer", 1, 500000, false, rtl_gui_timer_callback);
    os_timer_create(&dynamic_timer, "dynamic picture timer", 2, 1000, true, rtl_gui_timer_callback);
    os_timer_create(&page_switch_timer, "page switch timer", 3, PAGE_SWITCH_TIMER_INTERVAL, true,
                    rtl_gui_timer_callback);
    os_timer_start(&dynamic_timer);
    os_timer_start(&display_timer);
    menu_start_status = false;
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

    return_menu->current_max_icon = ExerciseRunMenu.current_max_icon;
    /* do memcpy here */

    if (menu_start_status == false)
    {
        memcpy(return_menu->pWidgetList, ExerciseRunMenu.pWidgetList,
               ExerciseRunMenu.current_max_icon * sizeof(UI_WidgetTypeDef));
        for (uint8_t i = 0; i < return_menu->current_max_icon; i++)
        {
            return_menu->pWidgetList[i].addr = NULL;
        }
        //not start menu, only need display BG and start icon
        return_menu->pWidgetList[0].addr = EXERCISE_RUN_BG_STOP_BIN;
        return_menu->pWidgetList[19].addr = COMMON_START_BIN;

    }
    else
    {
        memcpy(return_menu->pWidgetList, ExerciseRunMenu.pWidgetList,
               ExerciseRunMenu.current_max_icon * sizeof(UI_WidgetTypeDef));
        return_menu->pWidgetList[0].addr = EXERCISE_RUN_BG_START_BIN;
        if (run_status == false)
        {
            return_menu->pWidgetList[19].addr = COMMON_START_BIN;
        }
        else
        {
            return_menu->pWidgetList[19].addr = COMMON_PAUSE_BIN;
        }
        //for utc hour string
        static char utc_hour_string[4] = {'\0'};
        memset(utc_hour_string, '\0', sizeof(utc_hour_string));
        int32_t display_utc_hour = 0;
        sprintf(utc_hour_string, "%02d", display_utc_hour);
        return_menu->pWidgetList[1].u.num_string_addr = (uint32_t)utc_hour_string;
        //for utc min string
        static char utc_min_string[4] = {'\0'};
        memset(utc_min_string, '\0', sizeof(utc_min_string));
        int32_t display_min_hour = 0;
        sprintf(utc_min_string, "%02d", display_min_hour);
        return_menu->pWidgetList[4].u.num_string_addr = (uint32_t)utc_min_string;
        //for hour string
        static char exercise_run_hour_string[4] = {'\0'};
        memset(exercise_run_hour_string, '\0', sizeof(exercise_run_hour_string));
        int32_t display_hour = second_counter / 3600;
        sprintf(exercise_run_hour_string, "%02d", display_hour);
        return_menu->pWidgetList[5].u.num_string_addr = (uint32_t)exercise_run_hour_string;
        //APP_PRINT_INFO1("exercise_run_hour_string = %s", TRACE_BINARY(10, exercise_run_hour_string));
        //for min string
        static char exercise_run_min_string[4] = {'\0'};
        memset(exercise_run_min_string, '\0', sizeof(exercise_run_min_string));
        int32_t display_min = (second_counter / 60) % 60;
        sprintf(exercise_run_min_string, "%02d", display_min);
        return_menu->pWidgetList[7].u.num_string_addr = (uint32_t)exercise_run_min_string;
        //APP_PRINT_INFO1("exercise_run_min_string = %s", TRACE_BINARY(10, exercise_run_min_string));
        //for second string
        static char exercise_run_second_string[4] = {'\0'};
        memset(exercise_run_second_string, '\0', sizeof(exercise_run_second_string));
        int32_t display_second = second_counter % 60;
        sprintf(exercise_run_second_string, "%02d", display_second);
        return_menu->pWidgetList[9].u.num_string_addr = (uint32_t)exercise_run_second_string;
        //APP_PRINT_INFO1("exercise_run_second_string = %s", TRACE_BINARY(10, exercise_run_second_string));
    }
}

static void ButtonCallbackFun(uint8_t type)
{
    if (type == SHORT_BUTTON_0)
    {
        wristband_display_power_on();
        wristband_display_enable();
        pCurrentMenu->MenuSub = &ClockSpecialMenu;
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
    //NO NEED DO SLIDE ACTION
}

