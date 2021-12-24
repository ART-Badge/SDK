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
#include "rtl8763bf_task.h"
#include "gui_slide.h"
#include "gui_core.h"
#include "hub_task.h"

static void    *dynamic_timer = NULL;
static void    *display_timer = NULL;
static void    *page_switch_timer   = NULL;
uint8_t   dynamic_count = 0;
uint8_t   show_count = 0;


static void ButtonCallbackFun(uint8_t type);
static void TouchCallbackFun(RtkTouchInfo_t *info);
static void ConstructorCallbackFun(uint32_t type, void *pValue);
static void DestructorCallbackFun(void);
static void DisplayInfoCallbackFun(UI_MenuTypeDef *return_menu, void *argv);
static void TimerCallbackFun(RtkGuiTimerInfo_t *timer_info);

#define STEPMODE_PAGE_NUM      11

const static UI_WidgetTypeDef  stepmode_icons[STEPMODE_PAGE_NUM] =
{
    [0]     = {.x = 0,      .y = 0,     .width = 240,    .hight = 240,    .addr = STEP_BACKGROUND_BIN,    .widget_id_type = ICON_BACKGROUND},
    [1]     = {.x = 112,    .y = 16,    .width = 27,    .hight = 18,     .addr = COMMON_STEP_BIN,        .widget_id_type = ICON_WIDGET},
    [2]     = {.x = 112,    .y = 64,    .width = 24,     .hight = 15,     .addr = COMMON_KCAL_BIN,        .widget_id_type = ICON_WIDGET},
    [3]     = {.x = 120,    .y = 94,    .width = 17,     .hight = 15,     .addr = COMMON_KM_BIN,          .widget_id_type = ICON_WIDGET},
    [4]     = {.x = 122,    .y = 127,   .width = 64,     .hight = 64,     .addr = STEP_CYCLE000_BIN,      .widget_id_type = ICON_WIDGET},

    [5]      = {.x = 87,   .y = 226,   .width = 8,     .hight = 8,    .addr = COMMON_POINT_GRAY_BIN,   .widget_id_type = ICON_WIDGET},
    [6]      = {.x = 99,   .y = 226,   .width = 8,     .hight = 8,    .addr = COMMON_POINT_GRAY_BIN,   .widget_id_type = ICON_WIDGET},
    [7]     =  {.x = 110,  .y = 226,   .width = 8,     .hight = 8,    .addr = COMMON_POINT_WHITE_BIN,   .widget_id_type = ICON_WIDGET},
    [8]     =  {.x = 122,  .y = 226,   .width = 8,     .hight = 8,    .addr = COMMON_POINT_GRAY_BIN,   .widget_id_type = ICON_WIDGET},
    [9]     =  {.x = 133,  .y = 226,   .width = 8,     .hight = 8,    .addr = COMMON_POINT_GRAY_BIN,   .widget_id_type = ICON_WIDGET},
    [10]     = {.x = 145,  .y = 226,   .width = 8,     .hight = 8,    .addr = COMMON_POINT_GRAY_BIN,   .widget_id_type = ICON_WIDGET},
};

UI_MenuTypeDef StepMenu =
{
    .name = (const char *)"StepMenu",
    .MenuParent = NULL,
    .MenuSub = NULL,
    .button_func = ButtonCallbackFun,
    .touch_func = TouchCallbackFun,
    .constructor_func = ConstructorCallbackFun,
    .destructor_func = DestructorCallbackFun,
    .display_info_func = DisplayInfoCallbackFun,
    .timer_func = TimerCallbackFun,
    .pWidgetList = (UI_WidgetTypeDef *)stepmode_icons,
    .current_max_icon = STEPMODE_PAGE_NUM,
};


static void TimerCallbackFun(RtkGuiTimerInfo_t *timer_info)
{
    uint32_t timer_id = timer_info->timer_id;
    if (memcmp(timer_info->menu_name,  StepMenu.name, strlen(StepMenu.name)))
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
        APP_PRINT_INFO2("timer_id = %d, Name = %s", timer_id, TRACE_STRING(pCurrentMenu->name));
        if (pCurrentMenu->dynamicFlag)
        {
            rtl_gui_menu_update(&StepMenu, NULL, &HeartRateMenu, NULL, &ClockSpecialMenu, NULL, 0, 0);
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
    pCurrentMenu->dynamicFlag = false;

    uint16_t percent = 100 * RtkWristbandSys.gPedoData.global_steps /
                       RtkWristbandSys.gPedoData.daily_step_target;
    if (percent > 100)
    {
        percent = 100;
    }
    dynamic_count = percent / 5;
    show_count = 0;

    APP_PRINT_INFO2("create info count = %d, addr = 0x%x", dynamic_count, StepMenu.pWidgetList[4].addr);
    rtl_gui_menu_update(&StepMenu, NULL, &HeartRateMenu, NULL, &ClockSpecialMenu, NULL, 0, 0);

    os_timer_create(&display_timer, "display picture timer", 1, 50000, false, rtl_gui_timer_callback);
    os_timer_create(&dynamic_timer, "dynamic picture timer", 2, 50, true, rtl_gui_timer_callback);
    os_timer_create(&page_switch_timer, "page switch timer", 3, PAGE_SWITCH_TIMER_INTERVAL, true,
                    rtl_gui_timer_callback);

    os_timer_start(&display_timer);
    if (dynamic_count)
    {
        os_timer_start(&dynamic_timer);
        pCurrentMenu->dynamicFlag = true;
    }
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
    return_menu->current_max_icon = StepMenu.current_max_icon;
    /* do memcpy here */
    memcpy(return_menu->pWidgetList, StepMenu.pWidgetList,
           StepMenu.current_max_icon * sizeof(UI_WidgetTypeDef));
    //to do, set steps, distance, calorie, progress.

    if (dynamic_count)
    {
        if (show_count <= dynamic_count)
        {
            return_menu->pWidgetList[4].addr += show_count * 8192;
            show_count++;
            if (show_count > dynamic_count)
            {
                os_timer_stop(&dynamic_timer);
                pCurrentMenu->dynamicFlag = false;
            }
        }
        else
        {
            return_menu->pWidgetList[4].addr += (show_count - 1) * 8192;
        }
    }
}
static void ButtonCallbackFun(uint8_t type)
{
    if (type == SHORT_BUTTON_0)
    {
        pCurrentMenu->MenuSub = &HeartRateMenu;
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
        msg.subtype = BBPRO_HCI_SEND_ENTER_PAIRING_MODE;
        bbpro_hci_msg_queue_send(&msg);
        msg.subtype = BBPRO_HCI_GET_BT_ADDR;
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

    if (pCurrentMenu->dynamicFlag)
    {
        os_timer_stop(&dynamic_timer);
    }

    if (info->type == TOUCH_HOLD)
    {
        rtl_gui_menu_update(&StepMenu, NULL, &HeartRateMenu, NULL, &ClockSpecialMenu, NULL, info->deltaX,
                            0);
    }

    if (info->type == TOUCH_RIGHT_SLIDE)
    {
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(&StepMenu, NULL, \
                           &HeartRateMenu, NULL, \
                           &ClockSpecialMenu, NULL, \
                           info->deltaX, 240, 0, 0);
    }
    if (info->type == TOUCH_LEFT_SLIDE)
    {
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(&StepMenu, NULL, \
                           &HeartRateMenu, NULL, \
                           &ClockSpecialMenu, NULL, \
                           info->deltaX, -240, 0, 0);
    }

    if (info->type == TOUCH_ORIGIN_FROM_X)
    {
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(&StepMenu, NULL, \
                           &HeartRateMenu, NULL, \
                           &ClockSpecialMenu, NULL, \
                           info->deltaX, 0, 0, 0);
    }
}

