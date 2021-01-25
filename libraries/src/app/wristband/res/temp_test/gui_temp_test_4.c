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
#include "gui_psram.h"
#include "gui_core.h"
#include "gui_slide.h"
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

#define     TEST454_1_NUM_0_BIN    (0x1100000 + 0)

const static UI_WidgetTypeDef  icon[] =
{
    [0]     = {
        .x = 0,   .y = 0,    .width = LCD_WIDTH, .hight = LCD_HIGHT,     .addr = SETTING_BACKGROUND_BIN,   .widget_id_type = ICON_BACKGROUND,
    },
};

/* change Here for UI */
UI_MenuTypeDef TempTest4 =
{
    /* change Here for UI */
    .name = (const char *)"TempTest4",
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

static UI_MenuTypeDef *pLocalMenu = &TempTest4;



static void TimerCallbackFun(RtkGuiTimerInfo_t *timer_info)
{
    uint32_t timer_id = timer_info->timer_id;
    if (memcmp(timer_info->menu_name,  pLocalMenu->name, strlen(pLocalMenu->name)))
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
            rtl_gui_prepare_frame_buffer(pLocalMenu, NULL, &FrameBufferOrigin, false);
            rtl_gui_update_detal(0, 0);
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

    psram_frame_buffer_init();
    rtl_gui_prepare_frame_buffer(pLocalMenu, NULL, &FrameBufferOrigin, true);
    rtl_gui_update_detal(0, 0);

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

    return_menu->current_max_icon = pLocalMenu->current_max_icon;
    /* do memcpy here */
    memcpy(return_menu->pWidgetList, pLocalMenu->pWidgetList,
           pLocalMenu->current_max_icon * sizeof(UI_WidgetTypeDef));
}
static void ButtonCallbackFun(uint8_t type)
{
    if (type == SHORT_BUTTON_0)
    {
        pCurrentMenu->MenuSub = &ClockSpecialMenu;
        pCurrentMenu->destructor_func();
        pCurrentMenu = pCurrentMenu->MenuSub;
        pCurrentMenu->constructor_func(ICON_MIDDLE, NULL);
    }
}

static void TouchCallbackFun(RtkTouchInfo_t *info)
{
    UI_MenuTypeDef *ui_local_cur = pLocalMenu;
    UI_MenuTypeDef *ui_local_down = NULL;
    UI_MenuTypeDef *ui_local_up = &TempTest0;
    uint32_t timer_state;
    os_timer_state_get(&page_switch_timer, &timer_state);
    if (timer_state)
    {
        return;
    }
    APP_PRINT_INFO3("info->type = %d, info->detalX = %d, info->detalY = %d", info->type, info->deltaX,
                    info->deltaY);
    os_timer_restart(&display_timer, 500000);
    os_timer_restart(&dynamic_timer, 500);

    if (info->type == TOUCH_HOLD_Y)
    {
        if (info->deltaY >= 0)
        {
            rtl_gui_prepare_frame_buffer(ui_local_up, NULL, &FrameBufferUp, true);
        }
        else
        {
            rtl_gui_prepare_frame_buffer(ui_local_down, NULL, &FrameBufferDown, true);
        }
        rtl_gui_update_detal(0, info->deltaY);
    }
    else if (info->type == TOUCH_ORIGIN_FROM_Y)
    {
        if (info->deltaY >= 0)
        {
            rtl_gui_prepare_frame_buffer(ui_local_up, NULL, &FrameBufferUp, true);
        }
        else
        {
            rtl_gui_prepare_frame_buffer(ui_local_down, NULL, &FrameBufferDown, true);
        }
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(ui_local_cur, NULL, ui_local_down, NULL, ui_local_up, NULL, 0, 0,
                           info->deltaY, 0);
    }
    else if (info->type == TOUCH_UP_SLIDE)
    {
        rtl_gui_prepare_frame_buffer(ui_local_up, NULL, &FrameBufferUp, true);
        os_timer_restart(&page_switch_timer, PAGE_SWITCH_TIMER_INTERVAL);
        rtl_gui_menu_slide(ui_local_cur, NULL, ui_local_down, NULL, ui_local_up, NULL, 0, 0, info->deltaY,
                           LCD_HIGHT);
    }
    else if (info->type == TOUCH_INVALIDE)
    {
        rtl_gui_update_detal(0, 0);
    }

}

