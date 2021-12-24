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
#include "gui_core.h"
#include "gui_slide.h"
#include "gui_common_menu.h"
#include "hub_display.h"


#if 0
void    *gui_dynamic_timer       = NULL;
void    *gui_display_timer       = NULL;
void    *gui_page_switch_timer   = NULL;

void CommonConstructorCallbackFun(uint32_t type, void *pValue)
{
    wristband_display_power_on();
    wristband_display_enable();
    pCurrentMenu->dynamicFlag = true;

    rtl_gui_menu_update(pCurrentMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);

    os_timer_create(&gui_display_timer, "display picture timer", 1, 5000, false,
                    rtl_gui_timer_callback);
    os_timer_create(&gui_dynamic_timer, "dynamic picture timer", 2, 500, false, rtl_gui_timer_callback);
    os_timer_create(&gui_page_switch_timer, "page switch timer", 3, PAGE_SWITCH_TIMER_INTERVAL, true,
                    rtl_gui_timer_callback);
    os_timer_start(&gui_dynamic_timer);
    os_timer_start(&gui_display_timer);
}
void CommonDestructorCallbackFun(void)
{
    pCurrentMenu->dynamicFlag = false;
    if (gui_dynamic_timer)
    {
        os_timer_stop(&gui_dynamic_timer);
        os_timer_delete(&gui_dynamic_timer);
    }
    if (gui_display_timer)
    {
        os_timer_stop(&gui_display_timer);
        os_timer_delete(&gui_display_timer);
    }
    if (gui_page_switch_timer)
    {
        os_timer_stop(&gui_page_switch_timer);
        os_timer_delete(&gui_page_switch_timer);
    }

    gui_dynamic_timer = NULL;
    gui_display_timer = NULL;
    gui_page_switch_timer = NULL;
}

void CommonTimerCallbackFun(RtkGuiTimerInfo_t *timer_info)
{
    uint32_t timer_id = timer_info->timer_id;
    if (memcmp(timer_info->menu_name,  pCurrentMenu->name, strlen(pCurrentMenu->name)))
    {
        return;
    }

    if ((timer_id == 1) && (gui_display_timer != NULL))
    {
        //APP_PRINT_INFO2("timer_id = %d, Name = %s", timer_id, TRACE_STRING(pCurrentMenu->name));
        wristband_display_clear(0, 0, LCD_WIDTH - 1, LCD_HIGHT - 1, BLACK);
        wristband_display_power_off();
        pCurrentMenu->destructor_func();
    }
    else if ((timer_id == 2) && (gui_dynamic_timer != NULL))
    {
        //APP_PRINT_INFO2("timer_id = %d, Name = %s", timer_id, TRACE_STRING(pCurrentMenu->name));
        os_timer_start(&gui_dynamic_timer);
        if (pCurrentMenu->dynamicFlag == true)
        {
            rtl_gui_menu_update(pCurrentMenu, NULL, NULL, NULL, NULL, NULL, 0, 0);
        }
    }
    else if ((timer_id == 3) && (gui_display_timer != NULL))
    {
        UI_MenuTypeDef *p_menu = rtl_gui_menu_real_slide();
        if (p_menu != NULL)
        {
            pCurrentMenu->destructor_func();
            pCurrentMenu = p_menu;
            pCurrentMenu->constructor_func(ICON_MIDDLE, NULL);
            os_timer_stop(&gui_page_switch_timer);
        }
    }
}
#endif

