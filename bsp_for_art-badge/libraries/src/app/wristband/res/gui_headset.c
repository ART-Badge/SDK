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
#include "headset.h"
#include "rtl8763bf_task.h"
#include "wristband_global_data.h"

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
#if 0
    if (button_touch_type == SHORT_TOUCH_BUTTON)
    {
        APP_PRINT_INFO0("bt inquiry menu ButtonCallbackFun test");
        T_HCI_MSG msg;
        msg.type = HCI_MSG_TYPE_8763BF;
        msg.subtype = BBPRO_HCI_CREATE_CONNECTION;
        msg.len = 6;
        msg.u.buf = (uint8_t *)headset_info.headset[headset_info.cur_select].store_mac_addr;
        bbpro_hci_msg_queue_send(&msg);
    }
#endif
}


const static UI_WidgetTypeDef  icon[] =
{
    [0]     = {
        .x = 0,   .y = 0,    .width = 240, .hight = 32,     .addr = NULL,   .widget_id_type = ICON_STRING, \
        .touch_capability = false,
    },
    [1]     = {
        .x = 0,   .y = 104,    .width = 240, .hight = 32,     .addr = NULL,   .widget_id_type = ICON_STRING, \
        .touch_capability = false,
    },
    [2]     = {
        .x = 0,   .y = 208,    .width = 240, .hight = 32,     .addr = NULL,   .widget_id_type = ICON_STRING, \
        .touch_capability = false,
    },
    [3]     = {
        .x = 0,  .y = 0,    .width = 240, .hight = 240,     .addr = NULL,   .widget_id_type = ICON_WIDGET, \
        .touch_capability = true,  .widget_touch_func = CurrentMenuButtonTest
    },

};

/* change Here for UI */
UI_MenuTypeDef HeadSetDevUIMenu =
{
    /* change Here for UI */
    .name = (const char *)"HeadSetDevUIMenu",
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
    if (memcmp(timer_info->menu_name,  HeadSetDevUIMenu.name, strlen(HeadSetDevUIMenu.name)))
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
            rtl_gui_menu_update(&HeadSetDevUIMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);
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
    T_HCI_MSG msg;
    msg.type = HCI_MSG_TYPE_8763BF;
    msg.subtype = BBPRO_HCI_INQUIRY_STOP;
    bbpro_hci_msg_queue_send(&msg);
    wristband_display_power_on();
    wristband_display_enable();
    pCurrentMenu->dynamicFlag = true;

    rtl_gui_menu_update(&HeadSetDevUIMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);

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


//static const uint16_t dev_mac[] = {0x4E0A, 0x4E00, 0x66f2, ' ',' ',' ', 0x4e0b, 0x4E00, 0x66f2};
static const uint16_t link_connect[] = {0x8BBE, 0x5907, 0x5DF2, 0x8FDE, 0x63A5};
static const uint16_t link_disconnect[] = {0x8BBE, 0x5907, 0x5DF2, 0x65AD, 0x5F00};

static void DisplayInfoCallbackFun(UI_MenuTypeDef *return_menu, void *argv)
{
    if (return_menu == NULL)
    {
        return;
    }
    return_menu->current_max_icon = pCurrentMenu->current_max_icon;
    /* do memcpy here */
    memcpy(return_menu->pWidgetList, pCurrentMenu->pWidgetList,
           pCurrentMenu->current_max_icon * sizeof(UI_WidgetTypeDef));


    return_menu->pWidgetList[0].u.string_mem.font_string_addr = (uint32_t)
                                                                headset_info.headset[headset_info.cur_select].store_name_addr;
    return_menu->pWidgetList[0].u.string_mem.total_count =
        headset_info.headset[headset_info.cur_select].store_name_len;
    return_menu->pWidgetList[0].u.string_mem.current_number = 0;//start by the first character
    return_menu->pWidgetList[0].u.string_mem.single_font_height = 32;// font height
    return_menu->pWidgetList[0].u.string_mem.font_color = 0xFFFF;
    return_menu->pWidgetList[0].u.string_mem.new_font = true;

#if  0
    return_menu->pWidgetList[1].string = (uint32_t)
                                         headset_info.headset[headset_info.cur_select].store_name_addr;
    return_menu->pWidgetList[1].u.string_mem.total_count =
        headset_info.headset[headset_info.cur_select].store_name_len;
    return_menu->pWidgetList[1].u.string_mem.current_number = 0;//start by the first character
    return_menu->pWidgetList[1].u.string_mem.single_font_height = 32;// font height
    return_menu->pWidgetList[1].u.string_mem.font_color = 0xFFFF;
    return_menu->pWidgetList[1].u.string_mem.new_font = true;
#endif

    if (RtkWristbandSys.bbpro_hci_link_status == BBPRO_HCI_LINK_STATE_CONNECTED)
    {
        return_menu->pWidgetList[2].u.string_mem.font_string_addr = (uint32_t)link_connect;
    }
    else
    {
        return_menu->pWidgetList[2].u.string_mem.font_string_addr = (uint32_t)link_disconnect;
    }

    return_menu->pWidgetList[2].u.string_mem.font_string_addr = (uint32_t)link_connect;
    return_menu->pWidgetList[2].u.string_mem.total_count = 5;
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
        msg.subtype = BBPRO_HCI_CREATE_CONNECTION;
        msg.len = 6;
        msg.u.buf = (uint8_t *)headset_info.headset[headset_info.cur_select].store_mac_addr;
        bbpro_hci_msg_queue_send(&msg);
    }
    else if (type == LONG_BUTTON_0)
    {
        //pCurrentMenu->MenuSub = &ClockSpecialMenu;
        //pCurrentMenu->destructor_func();
        //pCurrentMenu = pCurrentMenu->MenuSub;
        //pCurrentMenu->constructor_func(ICON_MIDDLE, NULL);
    }
}

static void TouchCallbackFun(RtkTouchInfo_t *info)
{
    UI_MenuTypeDef *ui_local_right = &BTInquiryUIMenu;
    UI_MenuTypeDef *ui_local_left = &ClockMenu;
    UI_MenuTypeDef *ui_local_cur = &HeadSetDevUIMenu;
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

