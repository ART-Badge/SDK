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
#include "gui_common_menu.h"
#include <os_msg.h>
#include <ams.h>

static void    *dynamic_timer = NULL;
static void    *display_timer = NULL;
static void    *page_switch_timer   = NULL;

static void ButtonCallbackFun(uint8_t type);
static void TouchCallbackFun(RtkTouchInfo_t *info);
static void ConstructorCallbackFun(uint32_t type, void *pValue);
static void DestructorCallbackFun(void);
static void DisplayInfoCallbackFun(UI_MenuTypeDef *return_menu, void *argv);
static void TimerCallbackFun(RtkGuiTimerInfo_t *timer_info);

static void tp_vol_up(uint32_t button_touch_type);
static void tp_vol_down(uint32_t button_touch_type);
static void tp_av_fwd(uint32_t button_touch_type);
static void tp_av_bwd(uint32_t button_touch_type);
static void tp_start_and_pause(uint32_t button_touch_type);

static const uint16_t vol[] = {0x97F3, 0x91CF, 0x589e, ' ', ' ', ' ', 0x97F3, 0x91CF, 0x51cf};
static const uint16_t truck[] = {0x4E0A, 0x4E00, 0x66f2, ' ', ' ', ' ', 0x4e0b, 0x4E00, 0x66f2};
static const uint16_t start[] = {0x5F00, 0x59cb};
static const uint16_t pause[] = {0x6682, 0x505c};

static void tp_vol_up(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON)
    {
        T_AMS_MSG ams_msg;
        ams_msg.type = AMS_MSG_TYPE_REMOTE_CMD;
        ams_msg.remote_cmd_id = REMOTE_CMD_ID_VOLUME_UP;
        ams_send_msg(0, &ams_msg);
    }
}
static void tp_vol_down(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON)
    {
        T_AMS_MSG ams_msg;
        ams_msg.type = AMS_MSG_TYPE_REMOTE_CMD;
        ams_msg.remote_cmd_id = REMOTE_CMD_ID_VOLUME_DOWN;
        ams_send_msg(0, &ams_msg);
    }
}
static void tp_av_fwd(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON)
    {
        T_AMS_MSG ams_msg;
        ams_msg.type = AMS_MSG_TYPE_REMOTE_CMD;
        ams_msg.remote_cmd_id = REMOTE_CMD_ID_NEXT_TRACK;
        ams_send_msg(0, &ams_msg);
    }
}
static void tp_av_bwd(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON)
    {
        T_AMS_MSG ams_msg;
        ams_msg.type = AMS_MSG_TYPE_REMOTE_CMD;
        ams_msg.remote_cmd_id = REMOTE_CMD_ID_PREVIOUS_TRACK;
        ams_send_msg(0, &ams_msg);
    }
}
static void tp_start_and_pause(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON)
    {
        if (ams_entity_track.player_status == PLAYBACKSTATEPAUSED)
        {
            ams_entity_track.player_status = PLAYBACKSTATEPLAYING;
            rtl_gui_menu_update(&AmsPlayerUIMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);
            T_AMS_MSG ams_msg;
            ams_msg.type = AMS_MSG_TYPE_REMOTE_CMD;
            ams_msg.remote_cmd_id = REMOTE_CMD_ID_PLAY;
            ams_send_msg(0, &ams_msg);
        }
        else if (ams_entity_track.player_status == PLAYBACKSTATEPLAYING)
        {
            ams_entity_track.player_status = PLAYBACKSTATEPAUSED;
            rtl_gui_menu_update(&AmsPlayerUIMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);
            T_AMS_MSG ams_msg;
            ams_msg.type = AMS_MSG_TYPE_REMOTE_CMD;
            ams_msg.remote_cmd_id = REMOTE_CMD_ID_PAUSE;
            ams_send_msg(0, &ams_msg);
        }
    }
}

const static UI_WidgetTypeDef  icon[] =
{
    /*for vol*/
    [0]     = {
        .x = 0,   .y = 0,    .width = 240, .hight = 32,     .addr = NULL,   .widget_id_type = ICON_STRING, \
        .touch_capability = false,
    },
    /*for truck*/
    [1]     = {
        .x = 0,   .y = 208,    .width = 240, .hight = 32,     .addr = NULL,   .widget_id_type = ICON_STRING, \
        .touch_capability = false,
    },
    /*for start and pause*/
    [2]     = {
        .x = 88,   .y = 104,    .width = 240, .hight = 32,     .addr = NULL,   .widget_id_type = ICON_STRING, \
        .touch_capability = false,
    },
    /*for vol +*/
    [3]     = {
        .x = 0,   .y = 0,    .width = 80, .hight = 80,     .addr = NULL,   .widget_id_type = ICON_STRING, \
        .touch_capability = true,  .widget_touch_func = tp_vol_up
    },
    /*for vol -*/
    [4]     = {
        .x = 160,   .y = 0,    .width = 80, .hight = 80,     .addr = NULL,   .widget_id_type = ICON_STRING, \
        .touch_capability = true,  .widget_touch_func = tp_vol_down
    },
    /*for truck +*/
    [5]     = {
        .x = 0,   .y = 160,    .width = 80, .hight = 80,     .addr = NULL,   .widget_id_type = ICON_STRING, \
        .touch_capability = true,  .widget_touch_func = tp_av_bwd
    },
    /*for truck -*/
    [6]     = {
        .x = 160,   .y = 160,    .width = 80, .hight = 80,     .addr = NULL,   .widget_id_type = ICON_STRING, \
        .touch_capability = true,  .widget_touch_func = tp_av_fwd
    },
    /*for start and pause*/
    [7]     = {
        .x = 80,   .y = 80,    .width = 80, .hight = 80,     .addr = NULL,   .widget_id_type = ICON_STRING, \
        .touch_capability = true,  .widget_touch_func = tp_start_and_pause
    },
    /*for title*/
    [8]     = {
        .x = 0,   .y = 32,    .width = 240, .hight = 32,     .addr = NULL,   .widget_id_type = ICON_STRING, \
        .touch_capability = false,
    },
    /*for artist*/
    [9]     = {
        .x = 0,   .y = 64,    .width = 240, .hight = 32,     .addr = NULL,   .widget_id_type = ICON_STRING, \
        .touch_capability = false,
    },
    /*for vol*/
    [10]    = {
        .x = 102,   .y = 0,    .width = 240, .hight = 32,     .addr = NULL,   .widget_id_type = ICON_STRING, \
        .touch_capability = false,
    },
};

/* change Here for UI */
UI_MenuTypeDef AmsPlayerUIMenu =
{
    /* change Here for UI */
    .name = (const char *)"AmsPlayerUIMenu",
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
    if (memcmp(timer_info->menu_name,  AmsPlayerUIMenu.name, strlen(AmsPlayerUIMenu.name)))
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
            rtl_gui_menu_update(&AmsPlayerUIMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);
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

    rtl_gui_menu_update(&AmsPlayerUIMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);

    os_timer_create(&display_timer, "display picture timer", 1, 500000, false, rtl_gui_timer_callback);
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
    return_menu->current_max_icon = AmsPlayerUIMenu.current_max_icon;
    /* do memcpy here */
    memcpy(return_menu->pWidgetList, AmsPlayerUIMenu.pWidgetList,
           AmsPlayerUIMenu.current_max_icon * sizeof(UI_WidgetTypeDef));

    return_menu->pWidgetList[0].u.string_mem.font_string_addr = (uint32_t)vol;
    return_menu->pWidgetList[0].u.string_mem.total_count = 9;
    return_menu->pWidgetList[0].u.string_mem.current_number = 0;//start by the first character
    return_menu->pWidgetList[0].u.string_mem.single_font_height = 32;// font height
    return_menu->pWidgetList[0].u.string_mem.font_color = 0xFFFF;
    return_menu->pWidgetList[0].u.string_mem.new_font = true;

    return_menu->pWidgetList[1].u.string_mem.font_string_addr = (uint32_t)truck;
    return_menu->pWidgetList[1].u.string_mem.total_count = 9;
    return_menu->pWidgetList[1].u.string_mem.current_number = 0;//start by the first character
    return_menu->pWidgetList[1].u.string_mem.single_font_height = 32;// font height
    return_menu->pWidgetList[1].u.string_mem.font_color = 0xFFFF;
    return_menu->pWidgetList[1].u.string_mem.new_font = true;

    if (ams_entity_track.player_status == PLAYBACKSTATEPAUSED)
    {
        return_menu->pWidgetList[2].u.string_mem.font_string_addr = (uint32_t)start;
    }
    else
    {
        return_menu->pWidgetList[2].u.string_mem.font_string_addr = (uint32_t)pause;
    }
    return_menu->pWidgetList[2].u.string_mem.total_count = 2;
    return_menu->pWidgetList[2].u.string_mem.current_number = 0;//start by the first character
    return_menu->pWidgetList[2].u.string_mem.single_font_height = 32;// font height
    return_menu->pWidgetList[2].u.string_mem.font_color = 0xFFFF;
    return_menu->pWidgetList[2].u.string_mem.new_font = true;

    return_menu->pWidgetList[8].u.string_mem.font_string_addr = (uint32_t)ams_entity_track.title_addr;
    return_menu->pWidgetList[8].u.string_mem.total_count = ams_entity_track.title_len;
    return_menu->pWidgetList[8].u.string_mem.current_number = 0;
    return_menu->pWidgetList[8].u.string_mem.single_font_height = 32;
    return_menu->pWidgetList[8].u.string_mem.font_color = 0xFFFF;
    return_menu->pWidgetList[8].u.string_mem.new_font = true;

    return_menu->pWidgetList[9].u.string_mem.font_string_addr = (uint32_t)ams_entity_track.artist_addr;
    return_menu->pWidgetList[9].u.string_mem.total_count = ams_entity_track.artist_len;
    return_menu->pWidgetList[9].u.string_mem.current_number = 0;
    return_menu->pWidgetList[9].u.string_mem.single_font_height = 32;
    return_menu->pWidgetList[9].u.string_mem.font_color = 0xFFFF;
    return_menu->pWidgetList[9].u.string_mem.new_font = true;
}

static void ButtonCallbackFun(uint8_t type)
{
    if (type == SHORT_BUTTON_0)
    {
    }
    else if (type == LONG_BUTTON_0)
    {
        pCurrentMenu->MenuSub = &ClockSpecialMenu;
        pCurrentMenu->destructor_func();
        pCurrentMenu = pCurrentMenu->MenuSub;
        pCurrentMenu->constructor_func(ICON_MIDDLE, NULL);
    }
}

static void TouchCallbackFun(RtkTouchInfo_t *info)
{
    UI_MenuTypeDef *ui_local_right = &ClockMenu;
    UI_MenuTypeDef *ui_local_left = &PlayerUIMenu;
    UI_MenuTypeDef *ui_local_cur = &AmsPlayerUIMenu;
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

