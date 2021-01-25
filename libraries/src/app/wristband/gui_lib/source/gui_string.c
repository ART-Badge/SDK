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
#include "app_section.h"
#include "rtl876x.h"
#include "gui_string.h"
#include "gui_lcd_callback.h"
#include "flash_adv_cfg.h"

static uint8_t dot_width;
static uint8_t font_width;


#if 0

uint8_t font_data_buf[LCD_WIDTH * 32 * 2 / 8];
uint8_t font_width_array[LCD_WIDTH / 16];

DATA_RAM_FUNCTION
uint8_t rtl_gui_show_string(UI_WidgetTypeDef *icon_font, int16_t detal_x, int16_t detal_y, int Zs,
                            int Ze, uint8_t *writebuf)
{
    int xs = icon_font->x + detal_x;
    int ys = icon_font->y + detal_y;
    if (icon_font->u.string_mem.font_string_addr == NULL)
    {
        return 0;
    }
    if ((ys >= Ze) || ((ys + icon_font->hight) <= Zs) || (xs >= LCD_WIDTH) ||
        ((xs + icon_font->width) <= 0) || (icon_font->width > LCD_WIDTH))
    {
        return 0;
    }

    uint8_t font_height = icon_font->u.string_mem.single_font_height;
    uint32_t font_addr;
    uint16_t *uincode = (uint16_t *)icon_font->u.string_mem.font_string_addr;
    int16_t start_x = xs;
    int16_t string_y_end = ys + icon_font->hight;
    int16_t row_start = 0;
    uint16_t i;
    uint32_t color = icon_font->u.string_mem.font_color;
    uint16_t buf_offset = 0;

    if (icon_font->active_ys || icon_font->active_ye)
    {
        if (icon_font->active_ys > (Zs + LCD_SECTION_HEIGHT))
        {
            return 0;
        }
        if (icon_font->active_ye < Ze)
        {
            return 0;
        }

        if ((icon_font->u.string_mem.current_number == 0) && ((Zs - ys) / font_height > 0))
        {
            for (i = 0; i < icon_font->u.string_mem.total_count; i++)
            {
                if (rtl_gui_get_font_dot(uincode[i], &font_addr, &dot_width, &font_width))
                {
                    xs += font_width;
                    if (xs > (start_x + icon_font->width))
                    {
                        xs = start_x + font_width;
                        row_start++;
                        if (ys + (row_start + 1) * font_height > Zs)
                        {
                            icon_font->u.string_mem.current_number = i;
                            break;
                        }

                    }
                }
            }
            if (i == icon_font->u.string_mem.total_count)
            {
                return 0;
            }
        }
    }

    row_start = (Zs - ys) / font_height;
    xs = start_x;

    if (Zs < ys)
    {
        row_start = 0;
    }
    ys += font_height * row_start;

    if (icon_font->u.string_mem.new_font)
    {
        if (row_start && icon_font->u.string_mem.current_number == 0)
        {
            for (i = icon_font->u.string_mem.current_number; i < icon_font->u.string_mem.total_count; i++)
            {
                if (rtl_gui_get_font_dot(uincode[i], &font_addr, &dot_width, &font_width))
                {
                    if ((xs + font_width) > (start_x + icon_font->width))
                    {
                        row_start--;
                        xs = start_x;
                        if (row_start == 0)
                        {
                            icon_font->u.string_mem.current_number = i;
                            break;
                        }
                    }
                    xs = xs + font_width;
                }
            }
        }
        xs = start_x;
        if (icon_font->u.string_mem.new_font)
        {
            buf_offset = 0;
            for (i = icon_font->u.string_mem.current_number; i < icon_font->u.string_mem.total_count; i++)
            {
                if (rtl_gui_get_font_dot(uincode[i], &font_addr, &dot_width, &font_width))
                {
                    font_width_array[i - icon_font->u.string_mem.current_number] = font_width;
                    if ((xs + font_width) > (start_x + icon_font->width))
                    {
                        break;
                    }
                    xs = xs + font_width;
                    flash_ioctl(flash_ioctl_set_seq_trans_enable, 0, 0);
                    flash_read_quick_locked(font_addr, \
                                            icon_font->u.string_mem.single_font_height * ((dot_width + 7) >> 3), font_data_buf + buf_offset);
                    flash_ioctl(flash_ioctl_set_seq_trans_enable, 1, 0);
                    buf_offset += dot_width * icon_font->u.string_mem.single_font_height / 8;
                }
            }
            icon_font->u.string_mem.new_font = false;
        }
    }

    buf_offset = 0;
    xs = start_x;

    for (i = icon_font->u.string_mem.current_number; i < icon_font->u.string_mem.total_count; i++)
    {
        font_width = font_width_array[i - icon_font->u.string_mem.current_number];

        if ((xs + font_width) > (start_x + icon_font->width))
        {
            if (ys + font_height > Ze)
            {
                break;
            }
            else if (ys + font_height < Ze)
            {
                xs = start_x;
                ys += font_height;
                icon_font->u.string_mem.current_number = i;
                if (ys >= string_y_end)
                {
                    break;
                }
                uint16_t write_offset = 0;
                int16_t xss = xs;
                for (uint16_t j = icon_font->u.string_mem.current_number; j < icon_font->u.string_mem.total_count;
                     j++)
                {
                    if (rtl_gui_get_font_dot(uincode[j], &font_addr, &dot_width, &font_width))
                    {
                        font_width_array[j - icon_font->u.string_mem.current_number] = font_width;
                        if ((xss + font_width) > (start_x + icon_font->width))
                        {
                            break;
                        }
                        xss = xss + font_width;
                        flash_ioctl(flash_ioctl_set_seq_trans_enable, 0, 0);
                        flash_read_quick_locked(font_addr, \
                                                icon_font->u.string_mem.single_font_height * ((dot_width + 7) >> 3), font_data_buf + write_offset);
                        flash_ioctl(flash_ioctl_set_seq_trans_enable, 1, 0);
                        write_offset += dot_width * icon_font->u.string_mem.single_font_height / 8;
                    }
                }
                buf_offset = 0;
            }
            else
            {
                icon_font->u.string_mem.current_number = i;
                icon_font->u.string_mem.new_font = true;
                break;
            }
        }

        if (((uincode[i] < 0x80) && (uincode[i] >= 0x20)) || ((uincode[i] <= 0xFFE5) &&
                                                              (uincode[i] >= 0x00A1)))
        {
            rtl_show_char(dot_width, font_height,
                          (const unsigned char *)(font_data_buf + buf_offset), xs, ys,
                          color, Zs, Ze,
                          writebuf);
            xs = xs + font_width;
            buf_offset += dot_width * icon_font->u.string_mem.single_font_height / 8;
        }
    }

    if (icon_font->u.string_mem.total_count == i)
    {
        if (ys + font_height <= Ze)
        {
            icon_font->u.string_mem.current_number = i;
        }
    }

    return 0;
}
#else
DATA_RAM_FUNCTION
uint8_t rtl_gui_show_string(UI_WidgetTypeDef *icon_font, int16_t detal_x, int16_t detal_y, int Zs,
                            int Ze, uint8_t *writebuf)
{
    int xs = icon_font->x + detal_x;
    int ys = icon_font->y + detal_y;
    if (icon_font->u.string_mem.font_string_addr == NULL)
    {
        return 0;
    }
    if ((ys >= Ze) || ((ys + icon_font->hight) <= Zs) || (xs >= LCD_WIDTH) ||
        ((xs + icon_font->width) <= 0) || (icon_font->width > LCD_WIDTH))
    {
        return 0;
    }

    uint8_t font_height = icon_font->u.string_mem.single_font_height;
    uint32_t font_addr;
    uint16_t *uincode = (uint16_t *)icon_font->u.string_mem.font_string_addr;
    int16_t start_x = xs;
    int16_t string_y_end = ys + icon_font->hight;
    int16_t row_start = 0;
    uint16_t i;
    uint32_t color = icon_font->u.string_mem.font_color;

    if (icon_font->active_ys || icon_font->active_ye)
    {
        if (icon_font->active_ys > (Zs + LCD_SECTION_HEIGHT))
        {
            return 0;
        }
        if (icon_font->active_ye < Ze)
        {
            return 0;
        }

        if ((icon_font->u.string_mem.current_number == 0) && ((Zs - ys) / font_height > 0))
        {
            for (i = 0; i < icon_font->u.string_mem.total_count; i++)
            {
                if (rtl_gui_get_font_dot(uincode[i], &font_addr, &dot_width, &font_width))
                {
                    xs += font_width;
                    if (xs > (start_x + icon_font->width))
                    {
                        xs = start_x + font_width;
                        row_start++;
                        if (ys + (row_start + 1) * font_height > Zs)
                        {
                            icon_font->u.string_mem.current_number = i;
                            break;
                        }
                    }
                }
            }
            if (i == icon_font->u.string_mem.total_count)
            {
                return 0;
            }
        }
    }

    row_start = (Zs - ys) / font_height;
    xs = start_x;

    if (Zs < ys)
    {
        row_start = 0;
    }
    ys += font_height * row_start;


    for (i = icon_font->u.string_mem.current_number; i < icon_font->u.string_mem.total_count; i++)
    {
        if (rtl_gui_get_font_dot(uincode[i], &font_addr, &dot_width, &font_width))
        {
            if ((xs + font_width) > (start_x + icon_font->width))
            {
                if (ys + font_height > Ze)
                {
                    break;
                }
                else if (ys + font_height < Ze)
                {
                    xs = start_x;
                    ys += font_height;
                    icon_font->u.string_mem.current_number = i;
                    if (ys >= string_y_end)
                    {
                        break;
                    }
                }
                else
                {
                    icon_font->u.string_mem.current_number = i;
                    break;
                }
            }

            if ((uincode[i] < 0x80) && (uincode[i] >= 0x20))
            {
                rtl_show_char(dot_width, font_height, (const unsigned char *)font_addr, xs, ys,
                              color, Zs, Ze,
                              writebuf);
                xs = xs + font_width;
            }
            else if ((uincode[i] <= 0xFFE5) && (uincode[i] >= 0x00A1))
            {
                rtl_show_char(dot_width, font_height, (const unsigned char *)font_addr, xs, ys,
                              color, Zs, Ze,
                              writebuf);
                xs = xs + font_width;
            }
        }
    }

    if (icon_font->u.string_mem.total_count == i)
    {
        if (ys + font_height <= Ze)
        {
            icon_font->u.string_mem.current_number = i;
        }
    }

    return 0;
}
#endif
