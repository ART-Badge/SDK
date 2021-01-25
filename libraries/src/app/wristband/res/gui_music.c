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
#include "rtl8763bf_task.h"
#include "localplay.h"

static void    *dynamic_timer = NULL;
static void    *display_timer = NULL;
static void    *page_switch_timer   = NULL;



static void ButtonCallbackFun(uint8_t type);
static void TouchCallbackFun(RtkTouchInfo_t *info);
static void ConstructorCallbackFun(uint32_t type, void *pValue);
static void DestructorCallbackFun(void);
static void DisplayInfoCallbackFun(UI_MenuTypeDef *return_menu, void *argv);
static void TimerCallbackFun(RtkGuiTimerInfo_t *timer_info);


static const uint16_t vol[] = {0x97F3, 0x91CF, 0x589e, ' ', ' ', ' ', 0x97F3, 0x91CF, 0x51cf};
static const uint16_t truck[] = {0x4E0A, 0x4E00, 0x66f2, ' ', ' ', ' ', 0x4e0b, 0x4E00, 0x66f2};
static const uint16_t start[] = {0x5F00, 0x59cb};
static const uint16_t pause[] = {0x6682, 0x505c};

static void tp_vol_up(uint32_t button_touch_type);
static void tp_vol_down(uint32_t button_touch_type);
static void tp_av_fwd(uint32_t button_touch_type);
static void tp_av_bwd(uint32_t button_touch_type);
static void tp_start_and_pause(uint32_t button_touch_type);


static void tp_vol_up(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("tp_vol_up test");
        T_HCI_MSG msg;
        msg.type = HCI_MSG_TYPE_8763BF;
        msg.subtype = BBPRO_HCI_VOL_UP;
        bbpro_hci_msg_queue_send(&msg);
    }
}
static void tp_vol_down(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("tp_vol_down test");
        T_HCI_MSG msg;
        msg.type = HCI_MSG_TYPE_8763BF;
        msg.subtype = BBPRO_HCI_VOL_DOWN;
        bbpro_hci_msg_queue_send(&msg);
    }
}
static void tp_av_fwd(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("tp_av_fwd test");
        T_HCI_MSG msg;
        msg.type = HCI_MSG_TYPE_8763BF;
        msg.subtype = BBPRO_HCI_AV_FWD;
        bbpro_hci_msg_queue_send(&msg);
    }
}
static void tp_av_bwd(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("tp_av_bwd test");
        T_HCI_MSG msg;
        msg.type = HCI_MSG_TYPE_8763BF;
        msg.subtype = BBPRO_HCI_AV_BWD;
        bbpro_hci_msg_queue_send(&msg);
    }
}
static void tp_start_and_pause(uint32_t button_touch_type)
{
    if (button_touch_type == SHORT_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("tp_start_and_pause test");
        T_HCI_MSG msg;
        msg.type = HCI_MSG_TYPE_8763BF;
        msg.subtype = BBPRO_HCI_AV_PLAY_PAUSE;
        bbpro_hci_msg_queue_send(&msg);
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
        .touch_capability = true,  .widget_touch_func = tp_av_fwd
    },
    /*for truck -*/
    [6]     = {
        .x = 160,   .y = 160,    .width = 80, .hight = 80,     .addr = NULL,   .widget_id_type = ICON_STRING, \
        .touch_capability = true,  .widget_touch_func = tp_av_bwd
    },
    /*for start and pause*/
    [7]     = {
        .x = 80,   .y = 80,    .width = 80, .hight = 80,     .addr = NULL,   .widget_id_type = ICON_STRING, \
        .touch_capability = true,  .widget_touch_func = tp_start_and_pause
    },


};

/* change Here for UI */
UI_MenuTypeDef MusicUIMenu =
{
    /* change Here for UI */
    .name = (const char *)"MusicUIMenu",
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
    if (memcmp(timer_info->menu_name,  MusicUIMenu.name, strlen(MusicUIMenu.name)))
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
            rtl_gui_menu_update(&MusicUIMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);
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

    rtl_gui_menu_update(&MusicUIMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);

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

static void DisplayInfoCallbackFun(UI_MenuTypeDef *return_menu, void *argv)
{
    APP_PRINT_INFO0("MP test display info");

    if (return_menu == NULL)
    {
        return;
    }
    return_menu->current_max_icon = MusicUIMenu.current_max_icon;
    /* do memcpy here */
    memcpy(return_menu->pWidgetList, MusicUIMenu.pWidgetList,
           MusicUIMenu.current_max_icon * sizeof(UI_WidgetTypeDef));

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

    if (return_menu->pWidgetList[2].u.string_mem.font_string_addr == (uint32_t)start)
    {
        return_menu->pWidgetList[2].u.string_mem.font_string_addr = (uint32_t)pause;
    }
    else
    {
        return_menu->pWidgetList[2].u.string_mem.font_string_addr = (uint32_t)start;
    }
    return_menu->pWidgetList[2].u.string_mem.font_string_addr = (uint32_t)start;
    return_menu->pWidgetList[2].u.string_mem.total_count = 2;
    return_menu->pWidgetList[2].u.string_mem.current_number = 0;//start by the first character
    return_menu->pWidgetList[2].u.string_mem.single_font_height = 32;// font height
    return_menu->pWidgetList[2].u.string_mem.font_color = 0xFFFF;
    return_menu->pWidgetList[2].u.string_mem.new_font = true;

}
static void ButtonCallbackFun(uint8_t type)
{
    if (type == SHORT_BUTTON_0)
    {
        T_HCI_MSG msg;
        msg.type = HCI_MSG_TYPE_8763BF;
        msg.subtype = BBPRO_HCI_PLAY_MEDIA_BY_NAME;
        msg.len = media_info.music[media_info.cur_select].name_len * 2;
        msg.u.buf = (uint8_t *)media_info.music[media_info.cur_select].name_addr;
        bbpro_hci_msg_queue_send(&msg);
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
    UI_MenuTypeDef *ui_local_cur = &MusicUIMenu;
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
        //rtl_gui_menu_update(&PlayerUIMenu, NULL, NULL, NULL, &ClockSpecialMenu, NULL, info->deltaX, 0);
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

