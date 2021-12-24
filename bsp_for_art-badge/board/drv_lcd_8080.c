/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2020-08-04     tyustli  first version
 */

#include "rtthread.h"
#include "rtdevice.h"
#include "board.h"

#ifdef BSP_USING_LCD

#include "drv_lcd_8080.h"
#include "rtl876x_module_lcd_8080.h"
#include "lcd_st7789_240.h"

#ifdef PKG_USING_GUIENGINE
    #include <rtgui/driver.h>
#endif

#ifndef PKG_USING_GUIENGINE

struct rt_device_graphic_info
{
    rt_uint8_t  pixel_format;                           /**< graphic format */
    rt_uint8_t  bits_per_pixel;                         /**< bits per pixel */
    rt_uint16_t pitch;                                  /**< bytes per line */

    rt_uint16_t width;                                  /**< width of graphic device */
    rt_uint16_t height;                                 /**< height of graphic device */

    rt_uint8_t *framebuffer;                            /**< frame buffer */
};

enum
{
    /* pixel format define */
    RTGRAPHIC_PIXEL_FORMAT_MONO = 0,
    RTGRAPHIC_PIXEL_FORMAT_GRAY4,
    RTGRAPHIC_PIXEL_FORMAT_GRAY16,
    RTGRAPHIC_PIXEL_FORMAT_RGB332,
    RTGRAPHIC_PIXEL_FORMAT_RGB444,
    RTGRAPHIC_PIXEL_FORMAT_RGB565,
    RTGRAPHIC_PIXEL_FORMAT_RGB565P,
    RTGRAPHIC_PIXEL_FORMAT_RGB666,
    RTGRAPHIC_PIXEL_FORMAT_RGB888,
    RTGRAPHIC_PIXEL_FORMAT_ARGB888,
    RTGRAPHIC_PIXEL_FORMAT_ALPHA,
    RTGRAPHIC_PIXEL_FORMAT_LUT,
    RTGRAPHIC_PIXEL_FORMAT_COLOR,
    /* color format alias */
    RTGRAPHIC_PIXEL_FORMAT_BGR565 = RTGRAPHIC_PIXEL_FORMAT_RGB565P
};
#define RTGRAPHIC_CTRL_RECT_UPDATE      0
#define RTGRAPHIC_CTRL_POWERON          1
#define RTGRAPHIC_CTRL_POWEROFF         2
#define RTGRAPHIC_CTRL_GET_INFO         3
#define RTGRAPHIC_CTRL_SET_MODE         4
#define RTGRAPHIC_CTRL_GET_EXT          5
#define RTGRAPHIC_CTRL_FRAME_UPDATE     6
#define RTGRAPHIC_CTRL_FRAME_SWITCH     7

#define WHITE                   0xffff
#define BLACK                   0x0000
#define BLUE                    0x001f
#define GREEN                   0x07e0
#define RED                     0xf800
#define YELLOW                  0xffe0
#define BROWN                   0xbc40
#endif

//#define USING_DOUBLE_FRAMBUFFER

#ifdef USING_DOUBLE_FRAMBUFFER
    static uintptr_t framebuffer_1;
    static uintptr_t framebuffer_2;
    static uintptr_t m_cur_fb_base_phys;

    static uintptr_t swap_fb_addr(void);
#endif

struct drv_lcd_device
{
    struct rt_device parent;
    struct rt_device_graphic_info lcd_info;

    struct rt_semaphore lcd_lock;
};
static struct drv_lcd_device _lcd;

static rt_err_t drv_lcd_init(struct rt_device *device)
{
    /* init display */
    lcd_driver_init();
    lcd_st7789_init();
    lcd_st7789_power_on();
    lcd_clear(0, 0, LCD_WIDTH - 1, LCD_HIGHT - 1, 0x0000);

    return RT_EOK;
}

void lcd_frame_sync_handler(void)
{
    #ifdef VHARDWARE_VSYNC
    rt_sem_release(&_lcd.lcd_lock);
    rtgui_send_vsync();
    #endif /* VHARDWARE_VSYNC */
}

int fps(int deltaTime)
{
    static int fps = 0;
    static int timeLeft = 1000; // 取固定时间间隔为1秒
    static int frameCount = 0;

    ++frameCount;
    timeLeft -= deltaTime;

    if (timeLeft < 0)
    {
        fps = frameCount;
        frameCount = 0;
        timeLeft = 1000;
    }

    return fps;
}
static int oldTick = 0;
static rt_err_t drv_lcd_control(struct rt_device *device, int cmd, void *args)
{
    struct drv_lcd_device *lcd = rt_list_entry(device, struct drv_lcd_device, parent);

    switch (cmd)
    {
        case RTGRAPHIC_CTRL_RECT_UPDATE:
        {
//            rt_kprintf("Draw a frame took %d ms\n", rt_tick_get() - oldTick);
//            rt_kprintf("Draw a frame took %d ms\n", fps(rt_tick_get() - oldTick));
            oldTick = rt_tick_get();
            lcd_st7789_set_window(0, 0, 239, 239);
            lcd_dma_set_multi_block_for_x((uint32_t)(lcd->lcd_info.framebuffer), 0, 0, LCD_WIDTH, LCD_HIGHT);
            #ifdef USING_DOUBLE_FRAMBUFFER
            m_cur_fb_base_phys = swap_fb_addr();
            pm_set_lcd_framebuffer(m_cur_fb_base_phys);
            #else
            lcd_wait_lcd_control_transfer(0);
            #endif
        }
        break;

        case RTGRAPHIC_CTRL_GET_INFO:
        {
            struct rt_device_graphic_info *info = (struct rt_device_graphic_info *)args;
            RT_ASSERT(info != RT_NULL);
            info->pixel_format = lcd->lcd_info.pixel_format;
            info->bits_per_pixel = lcd->lcd_info.bits_per_pixel;
            info->width = lcd->lcd_info.width;
            info->height = lcd->lcd_info.height;
            info->framebuffer = lcd->lcd_info.framebuffer;
        }
        break;

        case RTGRAPHIC_CTRL_SET_BRIGHTNESS:
        {
            uint8_t dat = *(uint8_t *)args;
            lcd_set_backlight((uint32_t)dat);
        }
        break;
    }

    return RT_EOK;
}

#ifdef RT_USING_DEVICE_OPS
static const struct rt_device_ops lcd_ops =
{
    .init = drv_lcd_init,
    .control = drv_lcd_control,
};
#endif


#ifdef USING_DOUBLE_FRAMBUFFER
static uintptr_t swap_fb_addr(void)
{
    if(_lcd.lcd_info.framebuffer ==  (rt_uint8_t*)framebuffer_1)
    {
        _lcd.lcd_info.framebuffer =  (rt_uint8_t*)framebuffer_1;
    }
    else
    {
        _lcd.lcd_info.framebuffer =  (rt_uint8_t*)framebuffer_2;
    }

    return (uintptr_t)_lcd.lcd_info.framebuffer;
}
#endif

int rt_hw_8080_lcd_init(void)
{
    rt_err_t result = RT_EOK;

    struct rt_device *device = &_lcd.parent;

    result = rt_sem_init(&_lcd.lcd_lock, "led_lock", 0, RT_IPC_FLAG_FIFO);

    if (result != RT_EOK)
    {
        return -RT_ERROR;
    }

    #ifdef USING_DOUBLE_FRAMBUFFER
    framebuffer_1 = rt_malloc(LCD_HIGHT * LCD_WIDTH * 2);
    RT_ASSERT(framebuffer_1);
    framebuffer_2 = rt_malloc(LCD_HIGHT * LCD_WIDTH * 2);
    RT_ASSERT(framebuffer_2);
    #else
    rt_uint8_t * framebuffer_1 = rt_malloc(LCD_HIGHT * LCD_WIDTH * 2);
    #endif

    /* config LCD dev info */
    _lcd.lcd_info.height = LCD_HIGHT;
    _lcd.lcd_info.width = LCD_WIDTH;
    _lcd.lcd_info.bits_per_pixel = PIXEL_FORMAT;
    _lcd.lcd_info.pixel_format = RTGRAPHIC_PIXEL_FORMAT_RGB565;
    _lcd.lcd_info.framebuffer = framebuffer_1; //0;//(void *)lcd_framebuffer; /* the frame buffer address */

    if(_lcd.lcd_info.framebuffer == RT_NULL)
    {
        rt_kprintf("framebuffer is null\r\n");
    }

    #ifdef RT_USING_DEVICE_OPS
    device->ops = &lcd_ops;
    #else
    device->init = drv_lcd_init;
    device->control = drv_lcd_control;
    device->type = RT_Device_Class_Graphic;
    #endif
    result = rt_device_register(device, "lcd", RT_DEVICE_FLAG_RDWR);

    if (result != RT_EOK)
    {
        rt_kprintf("register lcd device failed, %d\r\n", result);
    }
    else
    {
        rt_kprintf("register lcd device succeed\r\n", result);
    }

    return RT_EOK;
}
//INIT_BOARD_EXPORT(rt_hw_8080_lcd_init);

#endif /* BSP_USING_LCD */

/****************** end of file *********************/
