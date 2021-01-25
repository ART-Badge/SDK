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
#include "board.h"
#include "wristband_gui.h"
#include "app_section.h"
#include "gui_lcd_callback.h"

rtl_gui_lcd_power_on_cb                 user_lcd_power_on_cb = NULL;
rtl_gui_lcd_power_off_cb                user_lcd_power_off_cb = NULL;
rtl_gui_lcd_set_window_cb               user_lcd_set_window_cb = NULL;
rtl_gui_lcd_init_cb                     user_lcd_init_cb = NULL;
rtl_gui_lcd_te_enable_cb                user_lcd_te_enable_cb = NULL;
rtl_gui_lcd_te_disable_cb               user_lcd_te_disable_cb = NULL;
rtl_gui_lcd_clear_cb                    user_lcd_clear_cb = NULL;
rtl_gui_dma_single_block_init_cb        user_dma_single_block_init_cb = NULL;
rtl_gui_dma_single_block_start_cb       user_dma_single_block_start_cb = NULL;
rtl_gui_wait_dma_transfer_cb            user_wait_dma_transfer_cb = NULL;
rtl_gui_wait_lcd_transfer_cb            user_wait_lcd_transfer_cb = NULL;
rtl_gui_dma_set_multi_block_for_x_cb    user_dma_set_multi_block_for_x_cb = NULL;
rtl_gui_dma_set_multi_block_for_y_cb    user_dma_set_multi_block_for_y_cb = NULL;
rtl_gui_get_font_dot_cb                 user_lcd_get_font_dot_cb = NULL;
rtl_gui_show_char_cb                    user_show_char_cb = NULL;
rtl_gui_lcd_frame_rate_cb               user_lcd_frame_rate_cb = NULL;


void rtl_gui_dma_single_block_init(uint32_t dir_type)
{
    if (user_dma_single_block_init_cb != NULL)
    {
        user_dma_single_block_init_cb(dir_type);
    }
}


void rtl_gui_dma_single_block_start(uint32_t destination_addr, uint32_t source_addr, uint32_t len)
{
    if (user_dma_single_block_start_cb != NULL)
    {
        user_dma_single_block_start_cb(destination_addr, source_addr, len);
    }
}


void rtl_gui_dma_set_multi_block_for_x(uint32_t picture0_addr, uint32_t picture1_addr,
                                       uint32_t offset,
                                       uint32_t yStart, uint32_t yEnd)//offset mean left menu not display in current menu
{
    if (user_dma_set_multi_block_for_x_cb != NULL)
    {
        user_dma_set_multi_block_for_x_cb(picture0_addr, picture1_addr, offset, yStart, yEnd);
    }
}

void rtl_gui_dma_set_multi_block_for_y(uint32_t picture0_addr, uint32_t picture1_addr,
                                       uint32_t offset,
                                       uint32_t yStart, uint32_t yEnd)//offset mean up menu not display in current menu
{
    if (user_dma_set_multi_block_for_y_cb != NULL)
    {
        user_dma_set_multi_block_for_y_cb(picture0_addr, picture1_addr, offset, yStart, yEnd);
    }
}

void rtl_gui_wait_dma_transfer(void)
{
    if (user_wait_dma_transfer_cb != NULL)
    {
        user_wait_dma_transfer_cb();
    }
}

void rtl_gui_wait_lcd_control_transfer(uint32_t count)
{
    if (user_wait_lcd_transfer_cb != NULL)
    {
        user_wait_lcd_transfer_cb(count);
    }
}


void rtl_gui_lcd_sectionconfig(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd)
{
    if (user_lcd_set_window_cb != NULL)
    {
        user_lcd_set_window_cb(xStart, yStart, xEnd, yEnd);
    }
}


void rtl_gui_lcd_clear(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd,
                       uint16_t color)
{
    if (user_lcd_clear_cb != NULL)
    {
        user_lcd_clear_cb(xStart, yStart, xEnd, yEnd, color);
    }
}



void rtl_gui_lcd_power_on(void)
{
    if (user_lcd_power_on_cb != NULL)
    {
        user_lcd_power_on_cb();
    }
}

void rtl_gui_lcd_power_off(void)
{
    if (user_lcd_power_off_cb != NULL)
    {
        user_lcd_power_off_cb();
    }
}

void rtl_gui_lcd_te_enable(void)
{
    if (user_lcd_te_enable_cb != NULL)
    {
        user_lcd_te_enable_cb();
    }
}

void rtl_gui_lcd_te_disable(void)
{
    if (user_lcd_te_disable_cb != NULL)
    {
        user_lcd_te_disable_cb();
    }
}

bool rtl_gui_get_font_dot(uint16_t unicode, uint32_t *addr, uint8_t *dot_width, uint8_t *font_width)
{
    if (user_lcd_get_font_dot_cb != NULL)
    {
        return user_lcd_get_font_dot_cb(unicode, addr, dot_width, font_width);
    }
    return false;
}

void rtl_show_char(uint16_t font_width, uint16_t font_height,
                   const unsigned char BMP[], int16_t xs,
                   int16_t ys, \
                   uint32_t colour, int16_t Zs, int16_t Ze, uint8_t *buf)
{
    if (user_show_char_cb != NULL)
    {
        user_show_char_cb(font_width, font_height, BMP, xs, ys, colour, Zs, Ze, buf);
    }
}

void rtl_frame_rate(uint8_t RTNA, uint8_t FPA, uint8_t BPA)
{
    if (user_lcd_frame_rate_cb != NULL)
    {
        user_lcd_frame_rate_cb(RTNA, FPA, BPA);
    }
}

