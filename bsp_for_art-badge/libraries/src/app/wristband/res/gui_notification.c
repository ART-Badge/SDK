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
//static RtkTouchInfo_t TouchInfo;

uint8_t menu_string[256] = {0};


static void ButtonCallbackFun(uint8_t type);
static void TouchCallbackFun(RtkTouchInfo_t *info);
static void ConstructorCallbackFun(uint32_t type, void *pValue);
static void DestructorCallbackFun(void);
static void DisplayInfoCallbackFun(UI_MenuTypeDef *return_menu, void *argv);
static void TimerCallbackFun(RtkGuiTimerInfo_t *timer_info);

#define  NOTIFICATION_PAGE_NUM      3

const static UI_WidgetTypeDef notification_icon[NOTIFICATION_PAGE_NUM] =
{
    [0]     = {.x = 0,   .y = 0,    .width = 240,  .hight = 240,  .addr = MESSAGE_BACKGROUND1_BIN,         .widget_id_type = ICON_BACKGROUND},
    [1]     = {.x = 100, .y = 0,    .width = 40,   .hight = 40,   .addr = NOTIFY_WECHAT_BIN,      .widget_id_type = ICON_WIDGET},
    [2]     = {.x = 0,   .y = 40,   .width = 240,  .hight = 200,  .addr = NULL,                   .widget_id_type = ICON_STRING},
};

/* change Here for UI */
UI_MenuTypeDef NotificationMenu =
{
    /* change Here for UI */
    .name = (const char *)"NotificationMenu",
    .MenuParent = NULL,
    .MenuSub = NULL,
    .button_func = ButtonCallbackFun,
    .touch_func = TouchCallbackFun,
    .constructor_func = ConstructorCallbackFun,
    .destructor_func = DestructorCallbackFun,
    .display_info_func = DisplayInfoCallbackFun,
    .timer_func = TimerCallbackFun,
    .pWidgetList = (UI_WidgetTypeDef *)notification_icon,
    .current_max_icon = NOTIFICATION_PAGE_NUM,
};


static void TimerCallbackFun(RtkGuiTimerInfo_t *timer_info)
{
    uint32_t timer_id = timer_info->timer_id;
    if ((timer_id == 1) && (display_timer != NULL))
    {
        APP_PRINT_INFO2("timer_id = %d, Name = %s", timer_id, TRACE_STRING(pCurrentMenu->name));
        wristband_display_clear(0, 0, LCD_WIDTH - 1, LCD_HIGHT - 1, BLACK);
        wristband_display_power_off();
        pCurrentMenu->destructor_func();
    }
    else if ((timer_id == 2) && (display_timer != NULL))
    {
        APP_PRINT_INFO2("timer_id = %d, Name = %s", timer_id, TRACE_STRING(pCurrentMenu->name));
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

    rtl_gui_menu_update(&NotificationMenu, pValue, NULL, NULL, NULL, NULL, 0, 0);

    os_timer_create(&display_timer, "display picture timer", 1, 15000, false, rtl_gui_timer_callback);
    os_timer_create(&dynamic_timer, "dynamic picture timer", 2, 500, true, rtl_gui_timer_callback);
    os_timer_create(&page_switch_timer, "page switch timer", 3, PAGE_SWITCH_TIMER_INTERVAL, true,
                    rtl_gui_timer_callback);
    os_timer_start(&dynamic_timer);
    os_timer_start(&display_timer);
    //TouchInfo.deltaX = 0;
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

    return_menu->current_max_icon = NotificationMenu.current_max_icon;
    /* do memcpy here */
    memcpy(return_menu->pWidgetList, NotificationMenu.pWidgetList,
           NotificationMenu.current_max_icon * sizeof(UI_WidgetTypeDef));

    if (argv != NULL)
    {
        memcpy(menu_string, argv, strlen((const char *)argv));
    }
    else
    {
        menu_string[0] = '\0';
    }
    return_menu->pWidgetList[2].u.string_mem.font_string_addr = (uint32_t)menu_string;
    return_menu->pWidgetList[2].u.string_mem.total_count = 0;//set the total characters count
    return_menu->pWidgetList[2].u.string_mem.current_number = 0;//start by the first character
    return_menu->pWidgetList[2].u.string_mem.single_font_height = 32;// font height
    return_menu->pWidgetList[0].u.string_mem.font_color = 0xFFFF;
    return_menu->pWidgetList[0].u.string_mem.new_font = true;
}

static void ButtonCallbackFun(uint8_t type)
{
    if (type == SHORT_BUTTON_0)
    {
        /* change Here for UI */
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

    if (info->type == TOUCH_HOLD_Y)
    {
        rtl_gui_menu_update(&NotificationMenu, NULL, &ClockSpecialMenu, NULL, &NotificationMenu, NULL, 0,
                            info->deltaY);
    }
    APP_PRINT_INFO3("info->type = %d, info->detalX = %d, info->detalY = %d", info->type, info->deltaX,
                    info->deltaY);

    if (info->type == TOUCH_ORIGIN_FROM_Y)
    {
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(&NotificationMenu, NULL, &ClockSpecialMenu, NULL, &NotificationMenu, NULL, 0, 0,
                           info->deltaY, 0);
    }
    else if (info->type == TOUCH_UP_SLIDE)
    {
        APP_PRINT_INFO0("===TOUCH_UP_SLIDE===");
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(&NotificationMenu, NULL, &ClockSpecialMenu, NULL, &NotificationMenu, NULL, 0, 0,
                           info->deltaY,
                           240);
    }
    else if (info->type == TOUCH_DOWN_SLIDE)
    {
        APP_PRINT_INFO0("===TOUCH_DOWN_SLIDE===");
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(&NotificationMenu, NULL, &ClockSpecialMenu, NULL, &NotificationMenu, NULL, 0, 0,
                           info->deltaY,
                           -240);
    }
    else if (info->type == TOUCH_INVALIDE)
    {
        rtl_gui_menu_update(&NotificationMenu, NULL, &ClockSpecialMenu, NULL, &NotificationMenu, NULL, 0,
                            0);
    }
}

