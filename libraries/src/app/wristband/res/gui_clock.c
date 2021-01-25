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
#include "gui_common_menu.h"


static void ButtonCallbackFun(uint8_t type);
static void TouchCallbackFun(RtkTouchInfo_t *info);
static void ConstructorCallbackFun(uint32_t type, void *pValue);
static void DestructorCallbackFun(void);
static void DisplayInfoCallbackFun(UI_MenuTypeDef *return_menu, void *argv);
static void TimerCallbackFun(RtkGuiTimerInfo_t *timer_info);


//UI_MenuTypeDef *pInitMenu = &ClockSpecialMenu;
//UI_MenuTypeDef *pInitMenu = &Test390Menu0;
//UI_MenuTypeDef *pInitMenu = &Test390Menu1;
UI_MenuTypeDef *pInitMenu = &Test454MenuMain;
//UI_MenuTypeDef *pInitMenu = &TempTest0;
/* change Here for UI */
UI_MenuTypeDef ClockMenu =
{
    /* change Here for UI */
    .name = (const char *)"ClockMenu",
    .MenuParent = NULL,
    .MenuSub = NULL,
    .button_func = ButtonCallbackFun,
    .touch_func = TouchCallbackFun,
    .constructor_func = ConstructorCallbackFun,
    .destructor_func = DestructorCallbackFun,
    .display_info_func = DisplayInfoCallbackFun,
    .timer_func = TimerCallbackFun,
    .pWidgetList = NULL,
    .current_max_icon = 0,
};

static void TimerCallbackFun(RtkGuiTimerInfo_t *timer_info)
{
    pInitMenu->timer_func(timer_info);
}

static void ConstructorCallbackFun(uint32_t type, void *pValue)
{
    pCurrentMenu = pInitMenu;
    pInitMenu->constructor_func(ICON_MIDDLE, pValue);
}

static void DestructorCallbackFun(void)
{
    pInitMenu->destructor_func();
}

static void DisplayInfoCallbackFun(UI_MenuTypeDef *return_menu, void *argv)
{
    pInitMenu->display_info_func(return_menu, argv);
}

static void ButtonCallbackFun(uint8_t type)
{
    pInitMenu->button_func(type);
}
static void TouchCallbackFun(RtkTouchInfo_t *info)
{
    pInitMenu->touch_func(info);
}





