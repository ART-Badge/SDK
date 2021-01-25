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
#include "stdio.h"
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
static void CurrentMenuButtonTest(uint32_t button_touch_type);


static void CurrentMenuButtonTest(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("[CurrentMenuButtonTest]-->short button test!!");
    }
    else if (button_touch_type == LONG_TOUCH_BUTTON)
    {
        /* change Here for UI */
        pCurrentMenu->MenuSub = &ClockChangeSpecialMenu;
        pCurrentMenu->destructor_func();
        pCurrentMenu = pCurrentMenu->MenuSub;
        pCurrentMenu->constructor_func(ICON_MIDDLE, NULL);
        APP_PRINT_INFO0("[CurrentMenuButtonTest]-->long button test!!");
    }
}


const static UI_WidgetTypeDef  icon[] =
{
    [0]     = {
        .x = 0,   .y = 0,    .width = 240, .hight = 240,     .addr = NULL,   .widget_id_type = ICON_BACKGROUND, \
        .touch_capability = true,  .widget_touch_func = CurrentMenuButtonTest
    },
    [1]     = {
        .x = 0,   .y = 0,    .width = 40, .hight = 40,     .addr = SET_TIME_CANCLE_BIN,   .widget_id_type = ICON_WIDGET, \
    },
    [2]     = {
        .x = 200,   .y = 0,    .width = 40, .hight = 40,     .addr = SET_TIME_CONFIRM_BIN,   .widget_id_type = ICON_WIDGET, \
    },
    [3]     = {
        .x = 0,   .y = 122,    .width = 240, .hight = 41,     .addr = SET_TIME_SELECT_BIN,   .widget_id_type = ICON_WIDGET, \
    },


    /*for year*/
    [4]     = {
        .x = 0,   .y = 42,    .width = 19, .hight = 36,     .addr = COMMON_P19X36RED_BIN,   .widget_id_type = ICON_NUM, \
        .active_ys = 42,      .active_ye = 240
    },
    [5]     = {
        .x = 0,   .y = 82,    .width = 19, .hight = 36,     .addr = COMMON_P19X36WHITE_BIN,   .widget_id_type = ICON_NUM, \
        .active_ys = 42,      .active_ye = 240
    },
    [6]     = {
        .x = 0,   .y = 122,    .width = 19, .hight = 36,     .addr = COMMON_P19X36WHITE_BIN,   .widget_id_type = ICON_NUM, \
        .active_ys = 42,      .active_ye = 240
    },
    [7]     = {
        .x = 0,   .y = 162,    .width = 19, .hight = 36,     .addr = COMMON_P19X36WHITE_BIN,   .widget_id_type = ICON_NUM, \
        .active_ys = 42,      .active_ye = 240
    },
    [8]     = {
        .x = 0,   .y = 202,    .width = 19, .hight = 36,     .addr = COMMON_P19X36WHITE_BIN,   .widget_id_type = ICON_NUM, \
        .active_ys = 42,      .active_ye = 240
    },
    [9]     = {
        .x = 0,   .y = 242,    .width = 19, .hight = 36,     .addr = COMMON_P19X36WHITE_BIN,   .widget_id_type = ICON_NUM, \
        .active_ys = 42,      .active_ye = 240
    },

    /*for month*/
    [10]     = {
        .x = 90,   .y = 42,    .width = 19, .hight = 36,     .addr = COMMON_P19X36RED_BIN,   .widget_id_type = ICON_NUM, \
    },
    [11]     = {
        .x = 90,   .y = 82,    .width = 19, .hight = 36,     .addr = COMMON_P19X36WHITE_BIN,   .widget_id_type = ICON_NUM, \
    },
    [12]     = {
        .x = 90,   .y = 122,    .width = 19, .hight = 36,     .addr = COMMON_P19X36WHITE_BIN,   .widget_id_type = ICON_NUM, \
    },
    [13]     = {
        .x = 90,   .y = 162,    .width = 19, .hight = 36,     .addr = COMMON_P19X36WHITE_BIN,   .widget_id_type = ICON_NUM, \
    },
    [14]     = {
        .x = 90,   .y = 202,    .width = 19, .hight = 36,     .addr = COMMON_P19X36WHITE_BIN,   .widget_id_type = ICON_NUM, \
    },
    [15]     = {
        .x = 90,   .y = 242,    .width = 19, .hight = 36,     .addr = COMMON_P19X36WHITE_BIN,   .widget_id_type = ICON_NUM, \
    },
    /*for date*/
    [16]     = {
        .x = 168,   .y = 42,    .width = 19, .hight = 36,     .addr = COMMON_P19X36RED_BIN,   .widget_id_type = ICON_NUM, \
    },
    [17]     = {
        .x = 168,   .y = 82,    .width = 19, .hight = 36,     .addr = COMMON_P19X36WHITE_BIN,   .widget_id_type = ICON_NUM, \
    },
    [18]     = {
        .x = 168,   .y = 122,    .width = 19, .hight = 36,     .addr = COMMON_P19X36WHITE_BIN,   .widget_id_type = ICON_NUM, \
    },
    [19]     = {
        .x = 168,   .y = 162,    .width = 19, .hight = 36,     .addr = COMMON_P19X36WHITE_BIN,   .widget_id_type = ICON_NUM, \
    },
    [20]     = {
        .x = 168,   .y = 202,    .width = 19, .hight = 36,     .addr = COMMON_P19X36WHITE_BIN,   .widget_id_type = ICON_NUM, \
    },
    [21]     = {
        .x = 168,   .y = 242,    .width = 19, .hight = 36,     .addr = COMMON_P19X36WHITE_BIN,   .widget_id_type = ICON_NUM, \
    },
};

/* change Here for UI */
UI_MenuTypeDef TestUISettingTimeMenu =
{
    /* change Here for UI */
    .name = (const char *)"TestUISettingTimeMenu",
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
    if (memcmp(timer_info->menu_name,  TestUISettingTimeMenu.name, strlen(TestUISettingTimeMenu.name)))
    {
        return;
    }

    if ((timer_id == 1) && (display_timer != NULL))
    {
        //APP_PRINT_INFO2("timer_id = %d, Name = %s", timer_id, TRACE_STRING(pCurrentMenu->name));
        wristband_display_clear(0, 0, LCD_WIDTH - 1, LCD_HIGHT - 1, BLACK);
        wristband_display_power_off();
        pCurrentMenu->destructor_func();
    }
    else if ((timer_id == 2) && (dynamic_timer != NULL))
    {
        //APP_PRINT_INFO2("timer_id = %d, Name = %s", timer_id, TRACE_STRING(pCurrentMenu->name));
        //os_timer_start(&dynamic_timer);
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

    rtl_gui_menu_update(&TestUISettingTimeMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);

    os_timer_create(&display_timer, "display picture timer", 1, 500000, false, rtl_gui_timer_callback);
    os_timer_create(&dynamic_timer, "dynamic picture timer", 2, 500, false, rtl_gui_timer_callback);
    os_timer_create(&page_switch_timer, "page switch timer", 3, PAGE_SWITCH_TIMER_INTERVAL, true,
                    rtl_gui_timer_callback);
    //os_timer_start(&dynamic_timer);
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

static int16_t year_detal = 0;

static void DisplayInfoCallbackFun(UI_MenuTypeDef *return_menu, void *argv)
{
    if (return_menu == NULL)
    {
        return;
    }

    return_menu->current_max_icon = TestUISettingTimeMenu.current_max_icon;
    /* do memcpy here */
    memcpy(return_menu->pWidgetList, TestUISettingTimeMenu.pWidgetList,
           TestUISettingTimeMenu.current_max_icon * sizeof(UI_WidgetTypeDef));


    static char *year_string_4 = "2017";
//    for(uint8_t i = 0; i < 6; i++)
//    {
//        return_menu->pWidgetList[4 + i].u.num_string_addr = (uint32_t)year_string[i];
//        return_menu->pWidgetList[4 + i].y += year_detal;
//    }
    return_menu->pWidgetList[4].u.num_string_addr = (uint32_t)year_string_4;
    return_menu->pWidgetList[4].y += year_detal;

    static char *test_string_5 = "2018";
    return_menu->pWidgetList[5].u.num_string_addr = (uint32_t)test_string_5;
    return_menu->pWidgetList[5].y += year_detal;

    static char *test_string_6 = "2019";
    return_menu->pWidgetList[6].u.num_string_addr = (uint32_t)test_string_6;
    return_menu->pWidgetList[6].y += year_detal;

    static char *test_string_7 = "2020";
    return_menu->pWidgetList[7].u.num_string_addr = (uint32_t)test_string_7;
    return_menu->pWidgetList[7].y += year_detal;

    static char *year_string_8 = "2021";
    return_menu->pWidgetList[8].u.num_string_addr = (uint32_t)year_string_8;
    return_menu->pWidgetList[8].y += year_detal;

#if 0
    static char *test_string_5 = "2018";
    return_menu->pWidgetList[5].u.num_string_addr = (uint32_t)test_string_5;
    return_menu->pWidgetList[5].y += year_detal;
    static char *test_string_6 = "2019";
    return_menu->pWidgetList[6].u.num_string_addr = (uint32_t)test_string_6;
    return_menu->pWidgetList[6].y += year_detal;
    static char *test_string_7 = "2020";
    return_menu->pWidgetList[7].u.num_string_addr = (uint32_t)test_string_7;
    return_menu->pWidgetList[7].y += year_detal;
    static char *year_string_8 = "2021";
    return_menu->pWidgetList[8].u.num_string_addr = (uint32_t)year_string_8;
    return_menu->pWidgetList[8].y += year_detal;
#endif
}


static void ButtonCallbackFun(uint8_t type)
{
    if (type == SHORT_BUTTON_0)
    {
        pCurrentMenu->MenuSub = &SlpModeMenu;
        pCurrentMenu->destructor_func();
        pCurrentMenu = pCurrentMenu->MenuSub;
        pCurrentMenu->constructor_func(ICON_MIDDLE, NULL);
    }
    else if (type == LONG_BUTTON_0)
    {
        T_HCI_MSG msg;
        msg.type = HCI_MSG_TYPE_8763BF;
        msg.subtype = BBPRO_HCI_SEND_POWER_ON_PRESSED;
        bbpro_hci_msg_queue_send(&msg);
        msg.subtype = BBPRO_HCI_SEND_POWER_ON_RELEASED;
        bbpro_hci_msg_queue_send(&msg);

        extern uint8_t bbpro_peer_bd_addr[];
        msg.subtype = BBPRO_HCI_CREATE_CONNECTION;
        msg.len = 6;
        msg.u.buf = bbpro_peer_bd_addr;
        bbpro_hci_msg_queue_send(&msg);
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
    os_timer_restart(&display_timer, 50000);
    //os_timer_restart(&dynamic_timer, 500);

    APP_PRINT_INFO3("info->type = %d, info->detalX = %d, info->detalY = %d", info->type, info->deltaX,
                    info->deltaY);

    if (info->type == TOUCH_HOLD_Y)
    {
        rtl_gui_menu_update(&ClockSpecialMenu, NULL, &SettingMenu, NULL, &NotificationMenu, NULL, 0, 0);
        year_detal = info->deltaY;
    }
    else if ((info->type == TOUCH_ORIGIN_FROM_Y) || \
             (info->type == TOUCH_LEFT_SLIDE) || \
             (info->type == TOUCH_DOWN_SLIDE))
    {
        rtl_gui_menu_update(&ClockSpecialMenu, NULL, &SettingMenu, NULL, &NotificationMenu, NULL, 0, 0);
    }
    else if (info->type == TOUCH_INVALIDE)
    {
        rtl_gui_menu_update(&ClockSpecialMenu, NULL, &StepMenu, NULL, &ExerciseMenu, NULL, 0, 0);
    }
}

