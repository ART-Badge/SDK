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
#include "rtl876x_rcc.h"
#include "rtl876x_nvic.h"
#include "hub_clock.h"
#include "hub_display.h"
#include "module_lcd_8080.h"
#include "module_font.h"
#include "hub_display.h"
#include "wristband_picture_res.h"
#include "wristband_global_data.h"
#include "math.h"
#include "version.h"
#include "hub_battery.h"
#include <string.h>
#include <stdarg.h>
#include "module_touch.h"
#include "wristband_gui.h"
#include "gui_lcd_callback.h"
#include "gui_core.h"
#include "hub_task.h"
#include "hub_interaction.h"
#include "rtl876x_lib_platform.h"
#include "psram_platform.h"


bool allowed_lcd_backlight_enter_dlps = true;
static T_UTC_TIME last_dial_utc_time;
bool display_has_installed = false;
MenuStatus wristbandMenuStatus;

static const T_IO_MODULE_CBS display_cbs =
{
    wristband_display_driver_init,
    wristband_display_enter_dlps,
    wristband_display_exit_dlps,
    wristband_lcd_backlight_allowed_enter_dlps_check,
};

void display_add_hub_task(void)
{
    wristband_display_driver_init();
    driver_modules[wristband_sensor_hub_count++] = (T_IO_MODULE_CBS *)&display_cbs;
    wristband_system_interact(INTERACT_LOGO, 0, NULL);
}
uint8_t *picture_thomas;
uint8_t *picture_thomas2;
void wristband_display_driver_init(void)
{
    lcd_driver_init();
    user_lcd_init_cb();
    display_has_installed = true;
    /* power on display device*/
    wristband_display_power_on();
    /* clear all screen here */
    picture_thomas = (uint8_t *)(0x6800000 + 0x600000);
	  memset(picture_thomas, 0x0f, LCD_WIDTH*LCD_HIGHT*PIXEL_BYTES);

	    picture_thomas2 = (uint8_t *)(0x6800000 + 0x500000);
	  memset(picture_thomas2, 0xff, LCD_WIDTH*LCD_HIGHT*PIXEL_BYTES);
	    psram_seq_trans_enable(true);

	  lcd_rm69330_qspi_390_set_window(0, 0, LCD_WIDTH - 1, LCD_HIGHT - 1);
    lcd_dma_set_multi_block_for_x((uint32_t)(picture_thomas), 0, 390/2,
                                      0, 0);
    lcd_wait_lcd_control_transfer(0);

    psram_seq_trans_enable(false);
	
//    wristband_display_clear(0, 0, LCD_WIDTH - 1, LCD_HIGHT - 1, YELLOW);
    while (1);

    gui_init();

}

void GPIO_FRAME_SYNC_HANDLER(void)
{
    lcd_frame_sync_handler();
}

void wristband_display_enter_dlps(void)
{
    lcd_enter_dlps();
}

void wristband_display_exit_dlps(void)
{
    lcd_exit_dlps();
}

bool wristband_lcd_backlight_allowed_enter_dlps_check(void)
{
    return allowed_lcd_backlight_enter_dlps;
}

void wristband_display_power_on(void)
{
    if (display_has_installed == false)
    {
        return;
    }
    if (set_system_clock(SYSTEM_90MHZ) == 0)
    {
        DBG_DIRECT("Set system clock 90M Fail!");
    }
    extern bool allowed_lcd_backlight_enter_dlps;
    allowed_lcd_backlight_enter_dlps = false;
    rtl_gui_lcd_power_on();
    wristbandMenuStatus.ActiveTime = 0;
    wristbandMenuStatus.DisplayActiveFlag = true;
}

void wristband_display_power_off(void)
{
    if (display_has_installed == false)
    {
        return;
    }
    if (set_system_clock(SYSTEM_40MHZ) == 0)
    {
        DBG_DIRECT("Set system clock 40M Fail!");
    }
    extern bool allowed_lcd_backlight_enter_dlps;
    allowed_lcd_backlight_enter_dlps = true;
    rtl_gui_lcd_power_off();
    wristbandMenuStatus.DisplayActiveFlag = false;
}

void wristband_display_enable(void)
{
    //null
    //power on will do display enable
}


void wristband_display_clear(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd,
                             uint16_t color)
{
    rtl_gui_lcd_clear(xStart, yStart, xEnd, yEnd, color);
}

void display_event_handler(T_IO_MSG msg)
{
    uint8_t value = msg.subtype;
    switch (value)
    {
    case LCD_MSG_TIMER_MENU:
        {
            pCurrentMenu->timer_func((RtkGuiTimerInfo_t *)(msg.u.buf));
            break;
        }
    case LCD_MSG_FRAME_SYNC:
        {
            os_mutex_take(flash_mutex_handle, 500);
            rtl_gui_lcd_update();
            os_mutex_give(flash_mutex_handle);
            break;
        }
    case LCD_MSG_INTERACT:
        {
            hub_system_interact(((T_IO_HUB_INTERACTION *)msg.u.buf)->type,
                                ((T_IO_HUB_INTERACTION *)msg.u.buf)->frame_num,
                                ((T_IO_HUB_INTERACTION *)msg.u.buf)->pValue);
        }
    default:
        break;
    }
}

