/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-21     stackRyan    first version
 */

#include <rtdevice.h>
#include <rtthread.h>

#define DBG_TAG VDEVICE_LCD_NAME
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#ifdef VDEVICE_USING_LCD

#define LCD_USING_RGB565
//#define LCD_USING_RGB888

#define LCD_HEIGHT 240
#define LCD_WIDTH 240

#ifdef LCD_USING_RGB565
#define LCD_BITS_PER_PIXEL 16
#define LCD_PIXEL_FORMAT RTGRAPHIC_PIXEL_FORMAT_RGB565
#endif

#ifdef LCD_USING_RGB888
#define LCD_BITS_PER_PIXEL 24
#define LCD_PIXEL_FORMAT RTGRAPHIC_PIXEL_FORMAT_RGB888
#endif

#define LCD_FRAMEBUFFER_SIZE (LCD_WIDTH * LCD_HEIGHT * LCD_BITS_PER_PIXEL / 8)

static rt_uint8_t lcd_framebuffer[LCD_FRAMEBUFFER_SIZE];

static struct rt_device_graphic_info lcd_info;
static struct rt_device lcd_dev;

static rt_err_t _lcd_init(rt_device_t dev)
{
    rt_err_t ret = RT_EOK;

    RT_ASSERT(dev != RT_NULL);

    /* Todo: init LCD */
    LOG_D("lcd_init");

    return ret;
}

static rt_err_t _lcd_open(rt_device_t dev, rt_uint16_t oflag)
{
    rt_err_t ret = RT_EOK;

    RT_ASSERT(dev != RT_NULL);

    /* Todo: enable LCD */
    LOG_D("lcd_open");

    return ret;
}

static rt_err_t _lcd_close(rt_device_t dev)
{
    rt_err_t ret = RT_EOK;

    RT_ASSERT(dev != RT_NULL);

    /* Todo: close LCD */
    LOG_D("lcd_close");

    return ret;
}

static rt_err_t _lcd_control(rt_device_t dev, int cmd, void *args)
{
    rt_err_t ret = RT_EOK;

    RT_ASSERT(dev != RT_NULL);

    switch (cmd)
    {
    case RTGRAPHIC_CTRL_RECT_UPDATE:
    {
        struct rt_device_rect_info *rect_info = (struct rt_device_rect_info *)args;
        LOG_D("lcd_control: RTGRAPHIC_CTRL_RECT_UPDATE:");
        LOG_D("height: %d", rect_info->height);
        LOG_D("width: %d", rect_info->width);
        LOG_D("x: %d", rect_info->x);
        LOG_D("y: %d", rect_info->y);

        /* Todo: update LCD data */

        break;
    }

    case RTGRAPHIC_CTRL_POWERON:
        /* Todo: power on */
        LOG_D("lcd_control: RTGRAPHIC_CTRL_POWERON");
        break;

    case RTGRAPHIC_CTRL_POWEROFF:
        /* Todo: power off */
        LOG_D("lcd_control: RTGRAPHIC_CTRL_POWEROFF");
        break;

    case RTGRAPHIC_CTRL_GET_INFO:
    {
        struct rt_device_graphic_info *info = (struct rt_device_graphic_info *)args;
        info->pixel_format = lcd_info.pixel_format;
        info->bits_per_pixel = lcd_info.bits_per_pixel;
        info->width = lcd_info.width;
        info->height = lcd_info.height;
        info->framebuffer = lcd_info.framebuffer;

        LOG_D("lcd_control: RTGRAPHIC_CTRL_GET_INFO:");
        LOG_D("pixel_format: %d", info->pixel_format);
        LOG_D("bits_per_pixel: %d", info->bits_per_pixel);
        LOG_D("width: %d", info->width);
        LOG_D("height: %d", info->height);
        LOG_D("framebuffer: %d", info->framebuffer);

        break;
    }

    case RTGRAPHIC_CTRL_SET_MODE:
        LOG_D("lcd_control: RTGRAPHIC_CTRL_SET_MODE");

        break;
    default:
        break;
    }

    return ret;
}

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops _lcd_ops =
{
    _lcd_init,
    _lcd_open,
    _lcd_close,
    RT_NULL,
    RT_NULL,
    _lcd_control
};
#endif

int rt_hw_lcd_init(void)
{
    rt_err_t ret = RT_EOK;

    lcd_info.bits_per_pixel = LCD_BITS_PER_PIXEL;
    lcd_info.framebuffer = lcd_framebuffer;
    lcd_info.height = LCD_HEIGHT;
    lcd_info.width = LCD_WIDTH;
    lcd_info.pixel_format = LCD_PIXEL_FORMAT;

    lcd_dev.type = RT_Device_Class_Graphic;
    lcd_dev.rx_indicate = RT_NULL;
    lcd_dev.tx_complete = RT_NULL;

#ifdef RT_USING_DEVICE_OPS
    lcd_dev.ops = &_lcd_ops;
#else
    lcd_dev.init = _lcd_init;
    lcd_dev.open = _lcd_open;
    lcd_dev.close = _lcd_close;
    lcd_dev.read = RT_NULL;
    lcd_dev.write = RT_NULL;
    lcd_dev.control = _lcd_control;
#endif

    lcd_dev.user_data = (void *)&lcd_info;

    ret = rt_device_register(&lcd_dev, VDEVICE_LCD_NAME, RT_DEVICE_FLAG_RDWR);

    LOG_D("rt_hw_lcd_init succeed!");
    return ret;
}
INIT_DEVICE_EXPORT(rt_hw_lcd_init);

#endif /* VDEVICE_USING_LCD */
