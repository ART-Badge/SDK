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
#include "gui_rotation.h"

DATA_RAM_FUNCTION
uint8_t rtl_gui_show_clock_pointer(UI_WidgetTypeDef *icon_clock, int16_t detal_x, int16_t detal_y,
                                   int Zs, int Ze, uint8_t *buf)
{
    int xs = icon_clock->x + detal_x;
    int ys = icon_clock->y + detal_y;
    float cosa = icon_clock->u.clock_pointer.rotation_cosa;
    float sina = icon_clock->u.clock_pointer.rotation_sina;

    if ((ys >= Ze) || ((ys + icon_clock->hight) <= Zs) || (xs >= LCD_WIDTH) ||
        ((xs + icon_clock->width) <= 0) || (icon_clock->width > LCD_WIDTH))
    {
        return 0;
    }

    int16_t m, n;
    float p;
    float x, y;
    uint16_t loopk;
    uint16_t loopj;
    uint16_t loopy;
    uint16_t addr_hh;
    uint16_t *data;
    int16_t x_move = xs - icon_clock->x;
    int16_t y_move = ys - icon_clock->y;
    int16_t clock_width = icon_clock->u.clock_pointer.source_pic_width;
    int16_t clock_center_height = icon_clock->u.clock_pointer.source_pic_center_height;
    int16_t center_x = icon_clock->u.clock_pointer.center_x;
    int16_t center_y = icon_clock->u.clock_pointer.center_y;
    int16_t y_range_up = - icon_clock->u.clock_pointer.source_pic_center_height;
    int16_t y_range_down = icon_clock->u.clock_pointer.source_pic_height -
                           icon_clock->u.clock_pointer.source_pic_center_height ;


#if PIXEL_FORMAT == RGB16BIT_565
    uint16_t *writebuf = (uint16_t *)buf;
#elif PIXEL_FORMAT == RGB24BIT_888
    uint8_t *writebuf = buf;
#endif

    if (ys > Zs)
    {
        addr_hh = Ze - ys;
        addr_hh = (addr_hh > icon_clock->hight) ? icon_clock->hight : addr_hh;
        ys = ys - Zs;
    }
    else
    {
        if ((ys + icon_clock->hight) < Ze)
        {
            addr_hh = (ys + icon_clock->hight) - Zs;
        }
        else
        {
            addr_hh = Ze - Zs;
        }
        ys = 0;
    }
    loopy = ys;

    data = (uint16_t *)(icon_clock->addr);
    float tempx1, tempx2, tempy1, tempy2;

    if (xs < 0)
    {
        int width_cache;
        width_cache = icon_clock->width + xs;
        for (loopk = 0; loopk < addr_hh; loopk ++, loopy ++)
        {
            tempy1 = (Zs + loopy - (center_y + y_move)) * sina;
            tempy2 = (Zs + loopy - (center_y + y_move)) * cosa;

            for (loopj = 0; loopj < width_cache; loopj ++)
            {
                //calculate the position in the source picture
                tempx1 = (loopj - (center_x + x_move)) * cosa;
                tempx2 = (loopj - (center_x + x_move)) * sina;

                x = tempx1 - tempy1;
                y = tempx2 + tempy2;

                if (y > 0)
                {
                    m = (int16_t)y;
                }
                else
                {
                    m = (int16_t)(y - 1);
                }
                if (x > 0)
                {
                    n = (int16_t)x;
                }
                else
                {
                    n = (int16_t)(x - 1);
                }

                if ((n >= clock_width / 2) || (n <= -clock_width / 2))
                {
                    if ((n >= clock_width / 2) && (cosa > 0))
                    {
                        break;
                    }
                    else if (n >= clock_width / 2)
                    {
                        float delta = n - clock_width / 2;
                        float loopj_step = delta / (-cosa);

                        loopj += (uint16_t)loopj_step;

                        if (loopj >= width_cache)
                        {
                            break;
                        }
                        continue;
                    }

                    if ((n <= -clock_width / 2) && (cosa < 0))
                    {
                        break;
                    }
                    else if (n <= -clock_width / 2)
                    {
                        float delta = -clock_width / 2 - n;
                        float loopj_step = delta / cosa;

                        loopj += (uint16_t)loopj_step;

                        if (loopj >= width_cache)
                        {
                            break;
                        }
                        continue;
                    }
                }

                if (m < y_range_up || m >= y_range_down)
                {
                    continue;
                }

                uint32_t pixel_num;
                pixel_num = (m + clock_center_height) * clock_width + (n + clock_width / 2);

#if PIXEL_FORMAT == RGB16BIT_565
                RGB565 pixel1, pixel2;

                pixel1.pixel = data[pixel_num];
                pixel2.pixel = data[pixel_num + 1];

                if (pixel1.pixel == 0)
                {
                    if (pixel2.pixel == 0)
                    {
                        continue;
                    }
                    pixel1.pixel = writebuf[loopy * LCD_WIDTH + loopj]; //new pixel = 0, need to modify!!!
                    pixel1.pixel = (pixel1.pixel << 8) | (pixel1.pixel >> 8);
                }
                if (pixel2.pixel == 0)
                {
                    if (pixel1.pixel == 0)
                    {
                        continue;
                    }
                    pixel2.pixel = writebuf[loopy * LCD_WIDTH + loopj];
                    pixel2.pixel = (pixel2.pixel << 8) | (pixel2.pixel >> 8);
                }



                //x' = (1-p)*x1+p*x2
                p = x - n;
                uint16_t r, g, b;
                float r1, r2, g1, g2, b1, b2, q;
                r1 = pixel1.r;
                r2 = pixel2.r;
                g1 = pixel1.g;
                g2 = pixel2.g;
                b1 = pixel1.b;
                b2 = pixel2.b;
                q = 1.0f - p;

                r = q * r1 + p * r2;
                g = q * g1 + p * g2;
                b = q * b1 + p * b2;

                uint16_t rgb = (r << 11) | (g << 5) | b;
                uint16_t rgb_swap = (rgb << 8) | (rgb >> 8);

                writebuf[loopy * LCD_WIDTH + loopj] = rgb_swap;
#elif PIXEL_FORMAT == RGB24BIT_888
                RGB888 pixel1, pixel2;
                pixel1.pixel = 0;
                pixel2.pixel = 0;
                pixel1.pixel = data[pixel_num * PIXEL_BYTES] + (data[pixel_num * PIXEL_BYTES + 1] << 8) +
                               (data[pixel_num * PIXEL_BYTES + 2] << 16);
                pixel2.pixel = data[(pixel_num + 1) * PIXEL_BYTES] + (data[(pixel_num + 1) * PIXEL_BYTES + 1] << 8)
                               + (data[(pixel_num + 1) * PIXEL_BYTES + 2] << 16);

                if (pixel1.pixel == 0 && pixel2.pixel == 0)
                {
                    continue;
                }

                //get the backimage of the pointer.
                if (pixel1.pixel == 0)
                {
                    pixel1.b = writebuf[(loopy * LCD_WIDTH + loopj) * PIXEL_BYTES]; //new pixel = 0, need to modify!!!
                    pixel1.g = writebuf[(loopy * LCD_WIDTH + loopj) * PIXEL_BYTES + 1];
                    pixel1.r = writebuf[(loopy * LCD_WIDTH + loopj) * PIXEL_BYTES + 2];
                }
                if (pixel2.pixel == 0)
                {
                    pixel2.b = writebuf[(loopy * LCD_WIDTH + loopj) * PIXEL_BYTES]; //new pixel = 0, need to modify!!!
                    pixel2.g = writebuf[(loopy * LCD_WIDTH + loopj) * PIXEL_BYTES + 1];
                    pixel2.r = writebuf[(loopy * LCD_WIDTH + loopj) * PIXEL_BYTES + 2];
                }

                uint16_t r, g, b;
                r = (1.0f - p) * pixel1.r + p * pixel2.r;
                g = (1.0f - p) * pixel1.g + p * pixel2.g;
                b = (1.0f - p) * pixel1.b + p * pixel2.b;

                if (r || g || b)
                {
                    writebuf[(loopy * LCD_WIDTH + loopj) * PIXEL_BYTES] = b;
                    writebuf[(loopy * LCD_WIDTH + loopj) * PIXEL_BYTES + 1] = g;
                    writebuf[(loopy * LCD_WIDTH + loopj) * PIXEL_BYTES + 2] = r;
                }
#endif

            }
        }
    }
    else if ((xs + icon_clock->width) > LCD_WIDTH)
    {
        int width_cache;
        width_cache = LCD_WIDTH - xs;
        for (loopk = 0; loopk < addr_hh; loopk ++, loopy ++)
        {
            tempy1 = (Zs + loopy - (center_y + y_move)) * sina;
            tempy2 = (Zs + loopy - (center_y + y_move)) * cosa;

            for (loopj = 0; loopj < width_cache; loopj ++)
            {
                //calculate the position in the source picture
                tempx1 = (xs + loopj - (center_x + x_move)) * cosa;
                tempx2 = (xs + loopj - (center_x + x_move)) * sina;

                x = tempx1 - tempy1;
                y = tempx2 + tempy2;

                if (y > 0)
                {
                    m = (int16_t)y;
                }
                else
                {
                    m = (int16_t)(y - 1);
                }
                if (x > 0)
                {
                    n = (int16_t)x;
                }
                else
                {
                    n = (int16_t)(x - 1);
                }

                if ((n >= clock_width / 2) || (n <= -clock_width / 2))
                {
                    if ((n >= clock_width / 2) && (cosa > 0))
                    {
                        break;
                    }
                    else if (n >= clock_width / 2)
                    {
                        float delta = n - clock_width / 2;
                        float loopj_step = delta / (-cosa);

                        loopj += (uint16_t)loopj_step;

                        if (loopj >= width_cache)
                        {
                            break;
                        }
                        continue;
                    }

                    if ((n <= -clock_width / 2) && (cosa < 0))
                    {
                        break;
                    }
                    else if (n <= -clock_width / 2)
                    {
                        float delta = -clock_width / 2 - n;
                        float loopj_step = delta / cosa;

                        loopj += (uint16_t)loopj_step;

                        if (loopj >= width_cache)
                        {
                            break;
                        }

                        continue;
                    }
                }

                if (m < y_range_up || m >= y_range_down)
                {
                    continue;
                }

                uint32_t pixel_num;
                pixel_num = (m + clock_center_height) * clock_width + (n + clock_width / 2);

#if PIXEL_FORMAT == RGB16BIT_565
                RGB565 pixel1, pixel2;

                pixel1.pixel = data[pixel_num];
                pixel2.pixel = data[pixel_num + 1];

                //get the backimage of the pointer.
                if (pixel1.pixel == 0)
                {
                    if (pixel2.pixel == 0)
                    {
                        continue;
                    }
                    pixel1.pixel = writebuf[loopy * LCD_WIDTH + xs + loopj]; //new pixel = 0, need to modify!!!
                    pixel1.pixel = (pixel1.pixel << 8) | (pixel1.pixel >> 8);
                }
                if (pixel2.pixel == 0)
                {
                    if (pixel1.pixel == 0)
                    {
                        continue;
                    }
                    pixel2.pixel = writebuf[loopy * LCD_WIDTH + xs + loopj];
                    pixel2.pixel = (pixel2.pixel << 8) | (pixel2.pixel >> 8);
                }

                //x' = (1-p)*x1+p*x2
                p = x - n;

                uint16_t r, g, b;
                float r1, r2, g1, g2, b1, b2, q;
                r1 = pixel1.r;
                r2 = pixel2.r;
                g1 = pixel1.g;
                g2 = pixel2.g;
                b1 = pixel1.b;
                b2 = pixel2.b;
                q = 1.0f - p;

                r = q * r1 + p * r2;
                g = q * g1 + p * g2;
                b = q * b1 + p * b2;

                uint16_t rgb = (r << 11) | (g << 5) | b;
                uint16_t rgb_swap = (rgb << 8) | (rgb >> 8);

                writebuf[loopy * LCD_WIDTH + xs + loopj] = rgb_swap;
#elif PIXEL_FORMAT == RGB24BIT_888
                RGB888 pixel1, pixel2;
                pixel1.pixel = 0;
                pixel2.pixel = 0;
                pixel1.pixel = data[pixel_num * PIXEL_BYTES] + (data[pixel_num * PIXEL_BYTES + 1] << 8) +
                               (data[pixel_num * PIXEL_BYTES + 2] << 16);
                pixel2.pixel = data[(pixel_num + 1) * PIXEL_BYTES] + (data[(pixel_num + 1) * PIXEL_BYTES + 1] << 8)
                               + (data[(pixel_num + 1) * PIXEL_BYTES + 2] << 16);

                if (pixel1.pixel == 0 && pixel2.pixel == 0)
                {
                    continue;
                }

                //get the backimage of the pointer.
                if (pixel1.pixel == 0)
                {
                    pixel1.b = writebuf[(loopy * LCD_WIDTH + xs + loopj) *
                                                                         PIXEL_BYTES]; //new pixel = 0, need to modify!!!
                    pixel1.g = writebuf[(loopy * LCD_WIDTH + xs + loopj) * PIXEL_BYTES + 1];
                    pixel1.r = writebuf[(loopy * LCD_WIDTH + xs + loopj) * PIXEL_BYTES + 2];
                }
                if (pixel2.pixel == 0)
                {
                    pixel2.b = writebuf[(loopy * LCD_WIDTH + xs + loopj) *
                                                                         PIXEL_BYTES]; //new pixel = 0, need to modify!!!
                    pixel2.g = writebuf[(loopy * LCD_WIDTH + xs + loopj) * PIXEL_BYTES + 1];
                    pixel2.r = writebuf[(loopy * LCD_WIDTH + xs + loopj) * PIXEL_BYTES + 2];
                }

                uint16_t r, g, b;
                r = (1.0f - p) * pixel1.r + p * pixel2.r;
                g = (1.0f - p) * pixel1.g + p * pixel2.g;
                b = (1.0f - p) * pixel1.b + p * pixel2.b;

                if (r || g || b)
                {
                    writebuf[(loopy * LCD_WIDTH + xs + loopj) * PIXEL_BYTES] = b;
                    writebuf[(loopy * LCD_WIDTH + xs + loopj) * PIXEL_BYTES + 1] = g;
                    writebuf[(loopy * LCD_WIDTH + xs + loopj) * PIXEL_BYTES + 2] = r;
                }
#endif
            }
        }
    }
    else
    {
        for (loopk = 0; loopk < addr_hh; loopk ++, loopy ++)
        {
            tempy1 = (Zs + loopy - (center_y + y_move)) * sina;
            tempy2 = (Zs + loopy - (center_y + y_move)) * cosa;

            for (loopj = 0; loopj < icon_clock->width; loopj ++)
            {
                //calculate the position in the source picture
                tempx1 = (xs + loopj - (center_x + x_move)) * cosa;
                tempx2 = (xs + loopj - (center_x + x_move)) * sina;

                x = tempx1 - tempy1;
                y = tempx2 + tempy2;

                if (y > 0)
                {
                    m = (int16_t)y;
                }
                else
                {
                    m = (int16_t)(y - 1);
                }
                if (x > 0)
                {
                    n = (int16_t)x;
                }
                else
                {
                    n = (int16_t)(x - 1);
                }

                if ((n >= clock_width / 2) || (n <= -clock_width / 2))
                {
                    if ((n >= clock_width / 2) && (cosa > 0))
                    {
                        break;
                    }
                    else if (n >= clock_width / 2)
                    {
                        float delta = n - clock_width / 2;
                        float loopj_step = delta / (-cosa);

                        loopj += (uint16_t)loopj_step;

                        if (loopj >= icon_clock->width)
                        {
                            break;
                        }
                        continue;
                    }

                    if ((n <= -clock_width / 2) && (cosa < 0))
                    {
                        break;
                    }
                    else if (n <= -clock_width / 2)
                    {
                        float delta = -clock_width / 2 - n;
                        float loopj_step = delta / cosa;

                        loopj += (uint16_t)loopj_step;

                        if (loopj >= icon_clock->width)
                        {
                            break;
                        }
                        continue;
                    }
                }

                //use clock pointer height to optimize if need.
                if (m < y_range_up || m >= y_range_down)
                {
                    continue;
                }

                uint32_t pixel_num;
                pixel_num = (m + clock_center_height) * clock_width + (n + clock_width / 2);
#if PIXEL_FORMAT == RGB16BIT_565
                RGB565 pixel1, pixel2;

                pixel1.pixel = data[pixel_num];
                pixel2.pixel = data[pixel_num + 1];

                //get the backimage of the pointer.

                if (pixel1.pixel == 0)
                {
                    if (pixel2.pixel == 0)
                    {
                        continue;
                    }
                    pixel1.pixel = writebuf[loopy * LCD_WIDTH + xs + loopj]; //new pixel = 0, need to modify!!!
                    pixel1.pixel = (pixel1.pixel << 8) | (pixel1.pixel >> 8);
                }
                if (pixel2.pixel == 0)
                {
                    if (pixel1.pixel == 0)
                    {
                        continue;
                    }
                    pixel2.pixel = writebuf[loopy * LCD_WIDTH + xs + loopj];
                    pixel2.pixel = (pixel2.pixel << 8) | (pixel2.pixel >> 8);
                }


                //x' = (1-p)*x1+p*x2
                p = x - n;
                uint16_t r, g, b;
                float r1, r2, g1, g2, b1, b2, q;
                r1 = pixel1.r;
                r2 = pixel2.r;
                g1 = pixel1.g;
                g2 = pixel2.g;
                b1 = pixel1.b;
                b2 = pixel2.b;
                q = 1.0f - p;

                r = q * r1 + p * r2;
                g = q * g1 + p * g2;
                b = q * b1 + p * b2;

                uint16_t rgb = (r << 11) | (g << 5) | b;
                uint16_t rgb_swap = (rgb << 8) | (rgb >> 8);

                writebuf[loopy * LCD_WIDTH + xs + loopj] = rgb_swap;
#elif PIXEL_FORMAT == RGB24BIT_888
                RGB888 pixel1, pixel2;
                pixel1.pixel = 0;
                pixel2.pixel = 0;
                pixel1.pixel = data[pixel_num * PIXEL_BYTES] + (data[pixel_num * PIXEL_BYTES + 1] << 8) +
                               (data[pixel_num * PIXEL_BYTES + 2] << 16);
                pixel2.pixel = data[(pixel_num + 1) * PIXEL_BYTES] + (data[(pixel_num + 1) * PIXEL_BYTES + 1] << 8)
                               + (data[(pixel_num + 1) * PIXEL_BYTES + 2] << 16);

                if (pixel1.pixel == 0 && pixel2.pixel == 0)
                {
                    continue;
                }

                //get the backimage of the pointer.
                if (pixel1.pixel == 0)
                {
                    pixel1.b = writebuf[(loopy * LCD_WIDTH + xs + loopj) *
                                                                         PIXEL_BYTES]; //new pixel = 0, need to modify!!!
                    pixel1.g = writebuf[(loopy * LCD_WIDTH + xs + loopj) * PIXEL_BYTES + 1];
                    pixel1.r = writebuf[(loopy * LCD_WIDTH + xs + loopj) * PIXEL_BYTES + 2];
                }
                if (pixel2.pixel == 0)
                {
                    pixel2.b = writebuf[(loopy * LCD_WIDTH + xs + loopj) *
                                                                         PIXEL_BYTES]; //new pixel = 0, need to modify!!!
                    pixel2.g = writebuf[(loopy * LCD_WIDTH + xs + loopj) * PIXEL_BYTES + 1];
                    pixel2.r = writebuf[(loopy * LCD_WIDTH + xs + loopj) * PIXEL_BYTES + 2];
                }

                uint16_t r, g, b;
                r = (1.0f - p) * pixel1.r + p * pixel2.r;
                g = (1.0f - p) * pixel1.g + p * pixel2.g;
                b = (1.0f - p) * pixel1.b + p * pixel2.b;

                if (r || g || b)
                {
                    writebuf[(loopy * LCD_WIDTH + xs + loopj) * PIXEL_BYTES] = b;
                    writebuf[(loopy * LCD_WIDTH + xs + loopj) * PIXEL_BYTES + 1] = g;
                    writebuf[(loopy * LCD_WIDTH + xs + loopj) * PIXEL_BYTES + 2] = r;
                }
#endif
            }
        }
    }

    return 0;
}
