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
#include "module_font.h"
#include "hub_display.h"
#include "wristband_picture_res.h"
#include <string.h>
#include <stdarg.h>

#include "wristband_gui.h"
#include "hub_interaction.h"
#include "app_msg.h"
#include "app_task.h"
#include "touch_algorithm.h"
#include "stdio.h"
#include "gui_slide.h"
#include "gui_core.h"
#include "test_mode.h"
#include "hub_task.h"
#include "localplay.h"
#include "rtl8763bf_task.h"
#include "gui_common_menu.h"

static void    *dynamic_timer = NULL;
static void    *display_timer = NULL;
static void    *page_switch_timer   = NULL;

#define MAX_DISPLAY_CNT              6

static uint8_t select = 0;
static uint8_t display_music_cnt; //this mean how many list can displayed in this menu
static uint8_t display_cursor;

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
}


const static UI_WidgetTypeDef  icon[] =
{
    [0]     = {
        .x = 100,   .y = 0,    .width = 240, .hight = 32,     .u.graph_color = 0xFF00,   .widget_id_type = ICON_RECT, \
        .touch_capability = false,
    },
    [1]     = {
        .x = 100,   .y = 0,    .width = 240, .hight = 32,     .addr = NULL,   .widget_id_type = ICON_STRING, \
        .touch_capability = false,
    },
    [2]     = {
        .x = 100,   .y = 32,    .width = 240, .hight = 32,     .addr = NULL,   .widget_id_type = ICON_STRING, \
        .touch_capability = false,
    },
    [3]     = {
        .x = 100,   .y = 32 * 2,    .width = 240, .hight = 32,     .addr = NULL,   .widget_id_type = ICON_STRING, \
        .touch_capability = false,
    },
    [4]     = {
        .x = 100,   .y = 32 * 3,    .width = 240, .hight = 32,     .addr = NULL,   .widget_id_type = ICON_STRING, \
        .touch_capability = false,
    },
    [5]     = {
        .x = 100,   .y = 32 * 4,    .width = 240, .hight = 32,     .addr = NULL,   .widget_id_type = ICON_STRING, \
        .touch_capability = false,
    },
    [6]     = {
        .x = 100,   .y = 32 * 5,    .width = 240, .hight = 32,     .addr = NULL,   .widget_id_type = ICON_STRING, \
        .touch_capability = false,
    },
    [7]     = {
        .x = 100,   .y = 32 * 6,    .width = 240, .hight = 32,     .addr = NULL,   .widget_id_type = ICON_STRING, \
        .touch_capability = false,
    },
    [8]     = {
        .x = 100,  .y = 0,    .width = 240, .hight = 240,     .addr = NULL,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = true,  .widget_touch_func = CurrentMenuButtonTest
    },
};

/* change Here for UI */
UI_MenuTypeDef PlayerUIMenu =
{
    /* change Here for UI */
    .name = (const char *)"PlayerUIMenu",
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
    if (memcmp(timer_info->menu_name,  PlayerUIMenu.name, strlen(PlayerUIMenu.name)))
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
        os_timer_start(&dynamic_timer);
        if (pCurrentMenu->dynamicFlag == true)
        {
            rtl_gui_menu_update(&PlayerUIMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);
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
    select = 0;
    wristband_display_power_on();
    wristband_display_enable();
    pCurrentMenu->dynamicFlag = true;

    rtl_gui_menu_update(&PlayerUIMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);

    os_timer_create(&display_timer, "display picture timer", 1, 5000, false, rtl_gui_timer_callback);
    os_timer_create(&dynamic_timer, "dynamic picture timer", 2, 500, false, rtl_gui_timer_callback);
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

    return_menu->current_max_icon = PlayerUIMenu.current_max_icon;
    /* do memcpy here */
    memcpy(return_menu->pWidgetList, PlayerUIMenu.pWidgetList,
           PlayerUIMenu.current_max_icon * sizeof(UI_WidgetTypeDef));

    /* for menu page title */
    static uint16_t title[] = {0x6B4C, 0x66F2, 0x5217, 0x8868};
    return_menu->pWidgetList[1].u.string_mem.font_string_addr = (uint32_t)title;
    return_menu->pWidgetList[1].u.string_mem.total_count = 4;
    return_menu->pWidgetList[1].u.string_mem.current_number = 0;//start by the first character
    return_menu->pWidgetList[1].u.string_mem.single_font_height = 32;// font height
    return_menu->pWidgetList[1].u.string_mem.font_color = BLUE;
    return_menu->pWidgetList[1].u.string_mem.new_font = true;
    return_menu->pWidgetList[1].x = 160;


    /* for cursor back */
    display_music_cnt = (media_info.had_scaned_list_num > MAX_DISPLAY_CNT) ? MAX_DISPLAY_CNT :
                        media_info.had_scaned_list_num;
    media_info.cur_select = select % media_info.had_scaned_list_num;
    display_cursor = 0;

    if (media_info.cur_select < MAX_DISPLAY_CNT)
    {
        display_cursor = media_info.cur_select;
    }
    else
    {
        display_cursor = MAX_DISPLAY_CNT - 1;
    }

    return_menu->pWidgetList[0].y = 32 * (display_cursor + 1);

    /* for list display */
    uint8_t start_music = 0;

    if (media_info.cur_select < MAX_DISPLAY_CNT)
    {
        start_music = 0;
    }
    else
    {
        start_music = media_info.cur_select - MAX_DISPLAY_CNT + 1;
    }

    uint8_t list_start_widget = 2;

    for (uint8_t i = 0; i < display_music_cnt; i++)
    {
        return_menu->pWidgetList[i + list_start_widget].u.string_mem.font_string_addr =
            (uint32_t)media_info.music[start_music +
                                                   i].name_addr;
        return_menu->pWidgetList[i + list_start_widget].u.string_mem.total_count =
            media_info.music[start_music + i].name_len;//set the total characters count
        return_menu->pWidgetList[i + list_start_widget].u.string_mem.current_number =
            0;//start by the first character
        return_menu->pWidgetList[i + list_start_widget].u.string_mem.single_font_height = 32;// font height
        return_menu->pWidgetList[i + list_start_widget].u.string_mem.font_color = 0xFFFF;
        return_menu->pWidgetList[i + list_start_widget].u.string_mem.new_font = true;
    }



}
static void ButtonCallbackFun(uint8_t type)
{
    if (type == SHORT_BUTTON_0)
    {
        select++;
        rtl_gui_menu_update(&PlayerUIMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);
    }
    else if (type == LONG_BUTTON_0)
    {
        pCurrentMenu->MenuSub = &MusicUIMenu;
        pCurrentMenu->destructor_func();
        pCurrentMenu = pCurrentMenu->MenuSub;
        pCurrentMenu->constructor_func(ICON_MIDDLE, NULL);
    }
}

static void TouchCallbackFun(RtkTouchInfo_t *info)
{
    UI_MenuTypeDef *ui_local_right = NULL;
    UI_MenuTypeDef *ui_local_left = &ClockSpecialMenu;
    UI_MenuTypeDef *ui_local_cur = &PlayerUIMenu;
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
        //rtl_gui_menu_update(&PlayerUIMenu, NULL, NULL, NULL, &ClockSpecialMenu, NULL, info->deltaX, 0);
    }
    else if (info->type == TOUCH_ORIGIN_FROM_X)
    {
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(ui_local_cur, NULL, ui_local_right, NULL, ui_local_left, NULL, info->deltaX, 0,
                           0, 0);
    }
    else if (info->type == TOUCH_RIGHT_SLIDE)
    {
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(ui_local_cur, NULL, ui_local_right, NULL, ui_local_left, NULL, info->deltaX, 240,
                           0, 0);
    }
    else if (info->type == TOUCH_INVALIDE)
    {
        rtl_gui_menu_update(ui_local_cur, NULL, ui_local_right, NULL, ui_local_left, NULL, 0, 0);
    }
}

