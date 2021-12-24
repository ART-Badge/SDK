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
#include "gui_graph.h"

DATA_RAM_FUNCTION
uint8_t rtl_gui_show_rect(UI_WidgetTypeDef *widget, int16_t detal_x, int16_t detal_y, int Zs,
                          int Ze, uint8_t *buf)
{
    int xs = widget->x + detal_x;
    int ys = widget->y + detal_y;
    int width = widget->width;
    int height = widget->hight;
    int y_start = widget->active_ys;
    int y_end = widget->active_ye;
    if ((y_start == 0) && (y_end == 0))
    {
        y_start = 0;
        y_end = LCD_HIGHT;
    }

    if (y_start > (Zs + LCD_SECTION_HEIGHT))
    {
        return 0;
    }
    if (y_end < Ze)
    {
        return 0;
    }

    uint16_t loopk;
    uint16_t loopj;
    uint16_t loopy;
    uint16_t addr_hh;
    uint16_t write_off;

    uint16_t *writebuf = (uint16_t *)buf;

    if ((ys >= Ze) || (xs >= LCD_WIDTH) || ((ys + height) <= Zs) || ((xs + width) <= 0) ||
        (width > LCD_WIDTH))
    {
        return 0;
    }

    if (ys > Zs)
    {
        addr_hh = Ze - ys;
        addr_hh = (addr_hh > height) ? height : addr_hh;
        ys = ys - Zs;
    }
    else
    {
        if ((ys + height) < Ze)
        {
            addr_hh = (ys + height) - Zs;
        }
        else
        {
            addr_hh = Ze - Zs;
        }
        ys = 0;
    }
    loopy = ys;

    if ((xs < 0) && ((xs + width) > 0))
    {
        int width_cache;
        width_cache = width + xs;
        for (loopk = 0; loopk < addr_hh; loopk ++, loopy ++)
        {
            write_off = loopy * LCD_WIDTH;
            for (loopj = 0; loopj < width_cache; loopj ++)
            {
                writebuf[write_off + loopj] = (widget->u.graph_color) << 8 | (widget->u.graph_color) >> 8;
            }
        }
    }
    else if (((xs + width) > LCD_WIDTH) && (xs <= LCD_WIDTH))
    {
        int width_cache;
        width_cache = LCD_WIDTH - xs;
        for (loopk = 0; loopk < addr_hh; loopk ++, loopy ++)
        {
            write_off = loopy * LCD_WIDTH;
            for (loopj = 0; loopj < width_cache; loopj ++)
            {
                writebuf[write_off + xs + loopj] = (widget->u.graph_color) << 8 | (widget->u.graph_color) >> 8;
            }
        }
    }
    else if ((xs >= 0) && ((xs + width) <= LCD_WIDTH))
    {
        for (loopk = 0; loopk < addr_hh; loopk ++, loopy ++)
        {
            write_off = loopy * LCD_WIDTH;
            for (loopj = 0; loopj < width; loopj ++)
            {
                writebuf[write_off + xs + loopj] = (widget->u.graph_color) << 8 | (widget->u.graph_color) >> 8;
            }
        }
    }

    return xs + width;
}

