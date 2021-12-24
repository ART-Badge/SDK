/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-09-16     RTT       the first version
 */

#include <rtthread.h>
#include "drv_lcd.h"
#include "ugui_port.h"

#define DBG_TAG     "gui_port"
#define DBG_LVL     DBG_LOG
#include <rtdbg.h>

static rt_thread_t ugui_port_thread = RT_NULL;
static void ugui_port_thread_entry(void* parameter);
static rt_device_t dev = RT_NULL;
static rt_uint8_t *lcd_fb = RT_NULL;
static UG_S32 lcd_w = 0x00U;
static UG_S32 lcd_h = 0x00U;
static struct rt_device_graphic_info lcd_info = { 0x00 };
static UG_GUI gui;

/**
 * Description: Open the lcd device
 * @Parameter : null
 * @Return: == RT_EOK open success
 *          != RT_EOK open failed
 * @Others: null
 */
static rt_err_t lcd_open(void)
{
    rt_err_t ret = RT_ERROR;
    dev = rt_device_find("lcd");
    
    if(RT_NULL == dev)
    {
        LOG_E("can't find lcd device");
        return RT_ERROR;
    }

    ret = rt_device_open(dev, RT_DEVICE_FLAG_RDWR);
    if(ret != RT_EOK)
    {
        LOG_E("open lcd device failed");
        return RT_ERROR;
    }
    /* open the screen */
    ret = rt_device_control(dev, RTGRAPHIC_CTRL_POWERON, NULL);
    if(ret != RT_EOK)
    {
        LOG_E("set lcd open failed");
        return RT_ERROR;
    }
    /* read the lcd info */
    ret = rt_device_control(dev, RTGRAPHIC_CTRL_GET_INFO, &lcd_info);
    if(ret != RT_EOK)
    {
        LOG_E("get lcd lcd_info failed");
        return RT_ERROR;
    }
    else
    {
        LOG_I("lcd_pixel  = %d", lcd_info.bits_per_pixel);
        LOG_I("lcd_width  = %d", lcd_info.width);
        LOG_I("lcd_height = %d", lcd_info.height);  
    }
    
    /* Set the corresponding parameters of lcd */
    lcd_fb = lcd_info.framebuffer;
    lcd_w  = lcd_info.width;
    lcd_h  = lcd_info.height;
    /* lcd open success */
    return RT_EOK;
}

/**
 * Description: Close the lcd device
 * @Parameter : null
 * @Return: == RT_EOK close success
 *          != RT_EOK close failed
 * @Others: null
 */
static rt_err_t lcd_close(void)
{
    rt_err_t ret = RT_ERROR;
    ret = rt_device_control(dev, RTGRAPHIC_CTRL_POWEROFF, NULL);
    if(ret != RT_EOK)
    {
        LOG_E("set lcd off failed");
    }
    ret = rt_device_close(dev);
    if(ret == RT_EOK)
    {
        dev = RT_NULL;
    }
    return ret;
}

/**
 * Description: Lcd drawing pixel processing function
 * @Parameter : x     x-axis coordinate
 * @Parameter : y     y-axis coordinate
 * @Parameter : pixel Color data
 * @Return: null
 * @Others: null
 */
void lcd_draw_pixel(UG_S16 x, UG_S16 y, UG_COLOR pixel)
{
    uint32_t addr = 0x00U;
    uint32_t r = 0x00U, g = 0x00U, b = 0x00U;
    uint16_t color = 0x00U;

    if ((x < 0) || (x > lcd_w)) 
    {
        return;
    }
    if ((y < 0) || (x > lcd_h)) 
    {
        return;
    }

    addr = (x) * 2 + (y) * lcd_w * 2;

    r = ((pixel >> 16) & 0xFF);
    g = ((pixel >>  8) & 0xFF);
    b = ((pixel >>  0) & 0xFF);

    r = ((((r >> 3) & 0x1F) << 11) & 0xF800);
    g = ((((g >> 2) & 0x3F) <<  5) & 0x07E0);
    b = ((((b >> 3) & 0x1F) <<  0) & 0x001F);

    color = (r | g | b);
    
    lcd_fb[addr + 0] = ((color >> 0) & 0xFF);
    lcd_fb[addr + 1] = ((color >> 8) & 0xFF);
#if 0
    rt_kprintf("r     = %04X\n", r);
    rt_kprintf("g     = %04X\n", g);
    rt_kprintf("b     = %04X\n", b);
    rt_kprintf("color = %04X\n", color);
#endif
}

/**
 * Description: Lcd reads the color value processing function of the specified position
 * @Parameter : x x-axis coordinate
 * @Parameter : y y-axis coordinate
 * @Return: Color data
 * @Others: null
 */
UG_COLOR lcd_read_pixel(UG_S16 x, UG_S16 y)
{
    uint32_t addr = 0x00U;
    uint32_t r = 0x00U, g = 0x00U, b = 0x00U;
    UG_COLOR pixel = 0x00U;
    UG_COLOR color = 0x00U;

    if ((x < 0) || (x > lcd_w)) 
    {
        return pixel;
    }
    if ((y < 0) || (x > lcd_h)) 
    {
        return pixel;
    }
    addr = (x) * 2 + (y) * lcd_w * 2;
    color = ((lcd_fb[addr + 0] << 0) | (lcd_fb[addr + 1] << 8));
    
    r = ((((color & 0xF800) >> 11) << 3) & 0xFF);
    g = ((((color & 0x07E0) >>  5) << 2) & 0xFF);
    b = ((((color & 0x001F) >>  0) << 3) & 0xFF);

    pixel = (((r << 16) & 0xFF0000) | ((g << 8) & 0x00FF00) | ((b << 0) & 0x0000FF));
#if 0    
    rt_kprintf("x     = %d\n", x);
    rt_kprintf("y     = %d\n", y);
    rt_kprintf("r     = %02X\n", r);
    rt_kprintf("g     = %02X\n", g);
    rt_kprintf("b     = %02X\n", b);
    rt_kprintf("color = %04X\n", color);
    rt_kprintf("pixel = %06X\n", pixel);
#endif
    return pixel;
}

/**
 * Description: ugui library initialization function
 * @Parameter : null
 * @Return: == RT_EOK init success
 *          != RT_EOK init failed
 * @Others: null
 */
int ugui_port_init(void)
{
    rt_err_t ret = RT_ERROR;
    /* First turn on the LCD device */
    ret = lcd_open();
    /* If it is not opened successfully, an error is returned */
    if(ret != RT_EOK)
    {
        return ret;
    }
    /* Then initialize UGUI */
    UG_Init(&gui, lcd_draw_pixel, lcd_w, lcd_h);
    /* Finally, create a gui refresh thread */    
    ugui_port_thread = rt_thread_create( "ugui_port", ugui_port_thread_entry, RT_NULL, 4096, 4, 10);
    if(ugui_port_thread == RT_NULL)
    {
        LOG_E("ugui port thread create failed!");
        return RT_ERROR;
    }
    else
    {
        rt_thread_startup(ugui_port_thread);
    }
    return RT_EOK;
}

/**
 * Description: ugui library refresh thread
 * @Parameter : parameter User data
 * @Return: null
 * @Others: null
 */
static void ugui_port_thread_entry(void *parameter)
{
    /* Set the background is black color */
    UG_FillScreen(C_BLACK);

    while(1)
    {
        UG_Update();
        rt_device_control(dev, RTGRAPHIC_CTRL_RECT_UPDATE, NULL);
        rt_thread_mdelay(16);
    }
}

/**
 * Description: uGUI reads the specified position color value function
 * @Parameter : x x-axis coordinate
 * @Parameter : y y-axis coordinate
 * @Return: Color data
 * @Others: null
 */
UG_COLOR UG_ReadPixel(UG_S16 x, UG_S16 y)
{
    UG_COLOR color = 0x00; 
    color = lcd_read_pixel(x, y);
    return color;
}

/**
 * Description: ugui library read gui object function
 * @Parameter : null
 * @Return: gui object
 * @Others: null
 */
UG_GUI *UG_GetGui(void)
{
    return &gui;
}

/**
 * Description: ugui library read lcd width value
 * @Parameter : null
 * @Return: lcd width value
 * @Others: null
 */
UG_S32 UG_GetLcdWidth(void)
{
    return lcd_w;
}

/**
 * Description: ugui library read lcd height value
 * @Parameter : null
 * @Return: lcd height value
 * @Others: null
 */
UG_S32 UG_GetLcdHeight(void)
{
    return lcd_h;
}

/**
 * Description: ugui library deinit processing function
 * @Parameter : null
 * @Return: null
 * @Others: null
 */
void ugui_port_deinit(void)
{
    if(ugui_port_thread != RT_NULL)
    {
        rt_thread_delete(ugui_port_thread);
    }
    rt_thread_mdelay(100);
    rt_err_t ret = rt_device_control(dev, RTGRAPHIC_CTRL_POWEROFF, NULL);
    if(ret != RT_EOK)
    {
        LOG_E("set lcd open failed");
    }
}

#ifdef RT_USING_FINSH
#include <rtthread.h>
static void lcd_test(int argv, char *argc[])
{
    uint8_t cnt = 10;
    int y;
    
    if(dev == RT_NULL)
    {
        rt_kprintf("The lcd device is not initialized\n");
        return;
    }
    if(lcd_fb == RT_NULL)
    {
        rt_kprintf("Framebuffer is not initialized\n");
        return;
    }
    
    while(cnt--)
    {
        for (y = 0; y < LCD_PIXEL_WIDTH * LCD_PIXEL_HEIGHT * 2; y ++)
        {
            lcd_fb[y] = 0xaa;
        }
        rt_device_control(dev, RTGRAPHIC_CTRL_RECT_UPDATE, NULL);
        rt_thread_mdelay(500);

        for (y = 0; y < LCD_PIXEL_WIDTH * LCD_PIXEL_HEIGHT * 2; y ++)
        {
            lcd_fb[y] = 0xbb;
        }
        rt_device_control(dev, RTGRAPHIC_CTRL_RECT_UPDATE, NULL);
        rt_thread_mdelay(500);

        for (y = 0; y < LCD_PIXEL_WIDTH * LCD_PIXEL_HEIGHT * 2; y ++)
        {
            lcd_fb[y] = 0xcc;
        }
        rt_device_control(dev, RTGRAPHIC_CTRL_RECT_UPDATE, NULL);
        rt_thread_mdelay(500);
    }
    return;
}
MSH_CMD_EXPORT(lcd_test, lcd_test);
#endif /* RT_USING_FINSH */
