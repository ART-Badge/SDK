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
#include "trace.h"
#include "string.h"
#include "os_timer.h"
#include "os_mem.h"
#include "os_sync.h"
#include "math.h"
#include <string.h>
#include <stdarg.h>
#include "wristband_gui.h"
#include "app_section.h"
#include "rtl876x.h"
#include "rtl876x_rcc.h"
#include "rtl876x_gpio.h"
#include "rtl876x_nvic.h"
#include "rtl876x_pinmux.h"
#include "rtl876x_gdma.h"
#include "flash_adv_cfg.h"
#include "app_msg.h"
#include "os_msg.h"
#include "gui_slide.h"
#include "gui_core.h"

static int16_t slide_from_x;
static int16_t slide_to_x;
static int16_t slide_current_x;
static int16_t slide_from_y;
static int16_t slide_to_y;
static int16_t slide_current_y;
static UI_MenuTypeDef *slide_ui_cur;
static UI_MenuTypeDef *slide_ui_sub;
static UI_MenuTypeDef *slide_ui_par;
static void *p_slide_cur_argv;
static void *p_slide_sub_argv;
static void *p_slide_par_argv;

void rtl_gui_menu_slide(UI_MenuTypeDef *ui_cur, void *p_ui_cur_argv, \
                        UI_MenuTypeDef *ui_sub, void *p_ui_sub_argv, \
                        UI_MenuTypeDef *ui_par, void *p_ui_par_argv, \
                        int16_t from_x, int16_t to_x, \
                        int16_t from_y, int16_t to_y)
{
    slide_from_x = from_x;
    slide_to_x = to_x;
    slide_from_y = from_y;
    slide_to_y = to_y;
    slide_ui_cur = ui_cur;
    slide_ui_sub = ui_sub;
    slide_ui_par = ui_par;
    p_slide_cur_argv = p_ui_cur_argv;
    p_slide_sub_argv = p_ui_sub_argv;
    p_slide_par_argv = p_ui_par_argv;
}

UI_MenuTypeDef *rtl_gui_menu_real_slide(void)
{
    int16_t tmp_mid_x = 0;
    int16_t tmp_mid_y = 0;
    slide_current_x = (slide_from_x + slide_to_x) / 2;
    slide_current_y = (slide_from_y + slide_to_y) / 2;
    slide_from_x = slide_current_x;
    slide_from_y = slide_current_y;
    tmp_mid_x = (slide_from_x + slide_to_x) / 2;
    tmp_mid_y = (slide_from_y + slide_to_y) / 2;

    if (FrameBufferMode == FRAMEBUFFER_MODE_NONE_PSRAM)
    {
        rtl_gui_menu_update(slide_ui_cur, p_slide_cur_argv, slide_ui_sub, p_slide_sub_argv, slide_ui_par,
                            p_slide_par_argv, slide_current_x, slide_current_y);
    }
    else
    {
        rtl_gui_update_detal(slide_current_x, slide_current_y);
    }

    if (slide_current_x)
    {
        if ((slide_from_x == tmp_mid_x) || (slide_to_x == tmp_mid_x))
        {
            if (slide_to_x == 0)
            {
                return slide_ui_cur;
            }
            else if (slide_to_x >= (LCD_WIDTH - 1))
            {
                return slide_ui_par;
            }
            else if (slide_to_x <= -(LCD_WIDTH - 1))
            {
                return slide_ui_sub;
            }
            else
            {
                return slide_ui_cur;
            }
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        if ((slide_from_y == tmp_mid_y) || (slide_to_y == tmp_mid_y))
        {
            if (slide_to_y == 0)
            {
                return slide_ui_cur;
            }
            else if (slide_to_y >= (LCD_HIGHT - 1))
            {
                return slide_ui_par;
            }
            else if (slide_to_y <= -(LCD_HIGHT - 1))
            {
                return slide_ui_sub;
            }
            else
            {
                return slide_ui_cur;
            }
        }
        else
        {
            return NULL;
        }
    }
}

