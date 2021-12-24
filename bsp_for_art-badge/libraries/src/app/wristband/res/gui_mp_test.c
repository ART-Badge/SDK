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

static void    *dynamic_timer = NULL;
static void    *display_timer = NULL;
static void    *page_switch_timer   = NULL;



static void ButtonCallbackFun(uint8_t type);
static void TouchCallbackFun(RtkTouchInfo_t *info);
static void ConstructorCallbackFun(uint32_t type, void *pValue);
static void DestructorCallbackFun(void);
static void DisplayInfoCallbackFun(UI_MenuTypeDef *return_menu, void *argv);
static void TimerCallbackFun(RtkGuiTimerInfo_t *timer_info);


const static UI_WidgetTypeDef  icon[] =
{
    [0]     = {
        .x = 16,   .y = 64,    .width = 240, .hight = 32,     .addr = NULL,   .widget_id_type = ICON_STRING, \
        .touch_capability = false,
    },
    [1]     = {
        .x = 16,   .y = 120,    .width = 240, .hight = 32,     .addr = NULL,   .widget_id_type = ICON_STRING, \
        .touch_capability = false,
    },

};

/* change Here for UI */
UI_MenuTypeDef MPTestUIMenu =
{
    /* change Here for UI */
    .name = (const char *)"MPTestUIMenu",
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
    if (memcmp(timer_info->menu_name,  MPTestUIMenu.name, strlen(MPTestUIMenu.name)))
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
            rtl_gui_menu_update(&MPTestUIMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);
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

    rtl_gui_menu_update(&MPTestUIMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);

    os_timer_create(&display_timer, "display picture timer", 1, 500000, false, rtl_gui_timer_callback);
    os_timer_create(&dynamic_timer, "dynamic picture timer", 2, 500, false, rtl_gui_timer_callback);
    os_timer_create(&page_switch_timer, "page switch timer", 3, PAGE_SWITCH_TIMER_INTERVAL, true,
                    rtl_gui_timer_callback);
//    os_timer_start(&dynamic_timer);
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

static uint16_t single_tone[] = {' ', 0x2192, 'S', 'i', 'n', 'g', 'l', 'e', ' ', 'T', 'o', 'n', 'e'};
static uint16_t auto_kRF[] = {' ', ' ', 'C', 'a', 'l', ' ', 'R', 'F'};
static uint8_t  cur_select = 0;
static void DisplayInfoCallbackFun(UI_MenuTypeDef *return_menu, void *argv)
{
    APP_PRINT_INFO0("MP test display info");

    if (return_menu == NULL)
    {
        return;
    }

    return_menu->current_max_icon = MPTestUIMenu.current_max_icon;
    /* do memcpy here */
    memcpy(return_menu->pWidgetList, MPTestUIMenu.pWidgetList,
           MPTestUIMenu.current_max_icon * sizeof(UI_WidgetTypeDef));
    if (cur_select == 0)
    {
        return_menu->pWidgetList[0].u.num_string_addr = (uint32_t)(single_tone + 1);
        return_menu->pWidgetList[0].u.string_mem.total_count = 12;//set the total characters count
    }
    else
    {
        return_menu->pWidgetList[0].u.num_string_addr = (uint32_t)single_tone;
        return_menu->pWidgetList[0].u.string_mem.total_count = 13;//set the total characters count
    }

    return_menu->pWidgetList[0].u.string_mem.current_number = 0;//start by the first character
    return_menu->pWidgetList[0].u.string_mem.single_font_height = 32;// font height
    return_menu->pWidgetList[0].u.string_mem.font_color = 0xFFFF;
    return_menu->pWidgetList[0].u.string_mem.new_font = true;

    if (cur_select == 0)
    {
        return_menu->pWidgetList[1].u.num_string_addr = (uint32_t)auto_kRF;
        return_menu->pWidgetList[1].u.string_mem.total_count = 8;//set the total characters count
    }
    else
    {
        return_menu->pWidgetList[1].u.num_string_addr = (uint32_t)(auto_kRF + 1);
        return_menu->pWidgetList[1].u.string_mem.total_count = 7;//set the total characters count
    }
    return_menu->pWidgetList[1].u.string_mem.current_number = 0;//start by the first character
    return_menu->pWidgetList[1].u.string_mem.single_font_height = 32;// font height
    return_menu->pWidgetList[1].u.string_mem.font_color = 0xFFFF;
    return_menu->pWidgetList[1].u.string_mem.new_font = true;

}
static void ButtonCallbackFun(uint8_t type)
{
    if (type == SHORT_BUTTON_0)
    {
        cur_select++;
        if (cur_select >= 2)
        {
            cur_select = 0;
        }
        if (cur_select)
        {
            single_tone[1] = ' ';
            auto_kRF[1] = 0x2192;
        }
        else
        {
            single_tone[1] = 0x2192;
            auto_kRF[1] = ' ';
        }
        rtl_gui_menu_update(&MPTestUIMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);
    }
    else if (type == LONG_BUTTON_0)
    {
        if (cur_select == 0)
        {
            DBG_DIRECT("Switch to Single Tone test mode!");
            switch_to_test_mode(SINGLE_TONE_MODE);
        }
        else if (cur_select == 1)
        {
            DBG_DIRECT("Switch to Calibration RF test mode!");
        }
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
//    os_timer_restart(&display_timer, 500000);
//    os_timer_restart(&dynamic_timer, 500);
}

