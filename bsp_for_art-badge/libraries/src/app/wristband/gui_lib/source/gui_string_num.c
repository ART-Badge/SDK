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
#include "gui_string_num.h"
#include "gui_bmp.h"
#include "flash_adv_cfg.h"


DATA_RAM_FUNCTION
void rtl_gui_show_char_num(uint16_t unicode, UI_WidgetTypeDef *icon_font,
                           int16_t xs,
                           int16_t ys, \
                           int16_t Zs, int16_t Ze, uint8_t *writebuf)
{
    uint8_t height = icon_font->hight;
    uint8_t width  = icon_font->width;
    if ((ys >= Ze) || ((ys + height) <= Zs) || (xs >= LCD_WIDTH) || (xs + width <= 0))
    {
        return;
    }
    uint8_t *pBMP;

    if ((unicode >= 0x30) && (unicode <= 0x39))
    {
        pBMP = (uint8_t *)(icon_font->addr + (unicode - '0') * icon_font->hight * icon_font->width *
                           PIXEL_BYTES);
    }
    else if ((unicode == '.') || (unicode == '-'))
    {
        pBMP = (uint8_t *)(icon_font->addr + 10 * icon_font->hight * icon_font->width * PIXEL_BYTES);
    }
    else if ((unicode == 'a') || (unicode == 'A'))
    {
        pBMP = (uint8_t *)(icon_font->addr + 11 * icon_font->hight * icon_font->width * PIXEL_BYTES);
    }
    else if ((unicode == 'b') || (unicode == 'B'))
    {
        pBMP = (uint8_t *)(icon_font->addr + 12 * icon_font->hight * icon_font->width * PIXEL_BYTES);
    }
    else if ((unicode == 'c') || (unicode == 'C'))
    {
        pBMP = (uint8_t *)(icon_font->addr + 13 * icon_font->hight * icon_font->width * PIXEL_BYTES);
    }
    else if ((unicode == 'd') || (unicode == 'D'))
    {
        pBMP = (uint8_t *)(icon_font->addr + 14 * icon_font->hight * icon_font->width * PIXEL_BYTES);
    }
    else if ((unicode == 'e') || (unicode == 'E'))
    {
        pBMP = (uint8_t *)(icon_font->addr + 15 * icon_font->hight * icon_font->width * PIXEL_BYTES);
    }
    else if ((unicode == 'f') || (unicode == 'F'))
    {
        pBMP = (uint8_t *)(icon_font->addr + 16 * icon_font->hight * icon_font->width * PIXEL_BYTES);
    }
    else if ((unicode == 'v') || (unicode == 'V'))
    {
        pBMP = (uint8_t *)(icon_font->addr + 17 * icon_font->hight * icon_font->width * PIXEL_BYTES);
    }

    rtl_gui_show_bmp_sector_legacy(xs, ys, icon_font->width, icon_font->hight, pBMP, Zs, Ze, writebuf,
                                   icon_font->active_ys, icon_font->active_ye);
}

DATA_RAM_FUNCTION
uint8_t rtl_gui_show_string_num(UI_WidgetTypeDef *icon_font, int16_t detal_x, int16_t detal_y,
                                int Zs, int Ze, uint8_t *writebuf)
{
    int xs = icon_font->x + detal_x;
    int ys = icon_font->y + detal_y;
    if ((ys >= Ze) || ((ys + icon_font->hight) <= Zs) || (xs >= LCD_WIDTH) || \
        ((xs + 2 * icon_font->width) < 0) || \
        (icon_font->u.num_string_addr == 0))
    {
        return 0;
    }
    char *string = (char *)(icon_font->u.num_string_addr);
    uint8_t len = strlen(string);
    for (uint8_t i = 0; i < len; i ++)
    {
        rtl_gui_show_char_num(*(string + i), icon_font, xs + i * icon_font->width, ys, Zs, Ze,
                              writebuf);
    }
    return 0;
}

